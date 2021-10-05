#define HYPATIA_IMPLEMENTATION

#include <stdio.h>
#include <assert.h>
#include <tgmath.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>
#include <stdalign.h>

#include "outfile.h"
#include "util.h"
#include "camera.h"
#include "sphere.h"
#include "hitRecord.h"
#include "types.h"
#include "hypatiaINC.h"
#include "ray.h"
#include "material.h"
#include "color.h"
#include "allocator.h"

RGBColorU8 writeColor(CFLOAT r, CFLOAT g, CFLOAT b, int sample_per_pixel){
    CFLOAT scale = 1.0/sample_per_pixel;

    r = sqrt(scale * r);
    g = sqrt(scale * g);
    b = sqrt(scale * b);

    return COLOR_U8CREATE(r, g, b);
}

HitRecord* hittableList(int n, Sphere sphere[n], Ray ray, LinearAllocFC * restrict hrAlloc, CFLOAT t_min, CFLOAT t_max){
    HitRecord * r = (HitRecord *) alloc_linearAllocFCAllocate(hrAlloc);
    HitRecord * h = NULL;

    for(int i = 0; i < n; i++){
        hit(sphere + i, ray, t_min, t_max, r);

        if(r->valid){

            if(h == NULL ){
                h = r;
                r = (HitRecord *) alloc_linearAllocFCAllocate(hrAlloc);
            }else if(r->distanceFromOrigin < h->distanceFromOrigin){
                // alloc_poolAllocFree(hrAlloc, h);
                h = r;
                r = (HitRecord *) alloc_linearAllocFCAllocate(hrAlloc);
            }
        }
    }    
    return h;
}


RGBColorF ray_c(Ray r, int n, Sphere sphere[n], int depth, LinearAllocFC * restrict hrAlloc){
    if(depth <= 0){
        return (RGBColorF){0};
    }

    HitRecord *  rec = hittableList(n, sphere, r, hrAlloc, 0.00001, FLT_MAX);
    if(rec != NULL){
        Ray scattered = {0};
        RGBColorF attenuation = {0};
        
        if(mat_scatter(&r, rec, &attenuation, &scattered)){
            RGBColorF color = ray_c(scattered, n, sphere, depth - 1, hrAlloc);
            color = colorf_multiply(color, attenuation); 

            
            return color;
        }

        return (RGBColorF){0};
    }

    vec3 ud = r.direction;
    vector3_normalize(&ud);
    CFLOAT t = 0.5 * (ud.y + 1.0);
    vec3 inter4;
    vector3_setf3(&inter4, 1.0 - t, 1.0 - t, 1.0 - t);
    vec3 inter3;
    vector3_setf3(&inter3, 0.5 * t, 0.7 * t, 1.0 * t);
    vector3_add(&inter3, &inter4);
    
    return (RGBColorF){
        .r = inter3.x,
        .g = inter3.y,
        .b = inter3.z
    };
}
        
void printProgressBar(int i, int max){
    int p = (int)(100 * (CFLOAT)i/max);

    printf("|");
    for(int j = 0; j < p; j++){
        printf("=");
    }

    for(int j = p; j < 100; j++){
        printf("*");
    }

    if(p == 100){
        printf("|\n");
    }else{
        printf("|\r");
    }
}

#define randomFloat() util_randomFloat(0.0, 1.0)

void randomSpheres(int n, Sphere spheres[n], int * out, DynamicStackAlloc * dsa){

    if(n < 500){
        return;
    }

    int i = 0;
    LambertianMat* materialGround = alloc_dynamicStackAllocAllocate(dsa, 
                                    sizeof(LambertianMat), 
                                    alignof(LambertianMat));
    materialGround->albedo.r = 0.5;
    materialGround->albedo.g = 0.5;
    materialGround->albedo.b = 0.5;

    spheres[i] = (Sphere){
        .center = {.x = 0, .y = -1000, .z = 0},
        .radius = 1000,
        .sphMat = MAT_CREATE_LAMB_IP(materialGround) 
    };

    i += 1;

    for (int a = -11; a < 11; a++){
        for (int b = -11; b < 11; b++){
            CFLOAT chooseMat = randomFloat();
            vec3 center = {
                .x = a + 0.9 * randomFloat(), 
                .y = 0.2, 
                .z = b + 0.9 * randomFloat()
            };

            CFLOAT length = sqrtf((center.x - 4) * (center.x - 4) 
                        +   (center.y - 0.2) * (center.y - 0.2)
                        +   (center.z - 0) * (center.z - 0));
            
            if(length > 0.9){
                if(chooseMat < 0.8){
                    // diffuse
                    RGBColorF albedo = {
                        .r = randomFloat() * randomFloat(),
                        .g = randomFloat() * randomFloat(),
                        .b = randomFloat() * randomFloat(),
                    };

                    LambertianMat* lambMat = alloc_dynamicStackAllocAllocate(dsa, 
                                            sizeof(LambertianMat), 
                                            alignof(LambertianMat));

                    lambMat->albedo = albedo;
                    spheres[i] = (Sphere) {
                        .center = center,
                        .radius = 0.2,
                        .sphMat = MAT_CREATE_LAMB_IP(lambMat) 
                    };

                    i += 1;
                }else if(chooseMat < 0.95){
                    // metal
                    RGBColorF albedo = {
                        .r = util_randomFloat(0.5, 1.0),
                        .g = util_randomFloat(0.5, 1.0),
                        .b = util_randomFloat(0.5, 1.0)
                    };
                    CFLOAT fuzz = util_randomFloat(0.5, 1.0);
                        
                    MetalMat* metalMat = alloc_dynamicStackAllocAllocate(dsa, 
                                         sizeof(MetalMat), 
                                         alignof(MetalMat));

                    metalMat->albedo = albedo;
                    metalMat->fuzz = fuzz;

                    spheres[i] = (Sphere) {
                        .center = center,
                        .radius = 0.2,
                        .sphMat = MAT_CREATE_METAL_IP(metalMat) 
                    };

                    i += 1;
 
                }else{
                    DielectricMat * dMat = alloc_dynamicStackAllocAllocate(dsa, 
                                           sizeof(DielectricMat), 
                                           alignof(DielectricMat));
                    dMat->ir = 0.5;
                    spheres[i] = (Sphere) {
                        .center = center,
                        .radius = 0.2,
                        .sphMat = MAT_CREATE_DIELECTRIC_IP(dMat)
                    };

                    i += 1;
                }

            }
        }
    }

    DielectricMat* material1 = alloc_dynamicStackAllocAllocate(dsa, 
                                           sizeof(DielectricMat), 
                                           alignof(DielectricMat));    
    material1->ir = 1.5;

    spheres[i] = (Sphere){
        .center = {.x = 0, .y = 1, .z = 0},
        .radius = 1.0,
        .sphMat = MAT_CREATE_DIELECTRIC_IP(material1) 
    };

    i += 1;
    
    LambertianMat* material2 = alloc_dynamicStackAllocAllocate(dsa, 
                                           sizeof(LambertianMat), 
                                           alignof(LambertianMat));    
    material2->albedo.r = 0.4;
    material2->albedo.g = 0.2;
    material2->albedo.b = 0.1;

    spheres[i] = (Sphere){
        .center = {.x = -4, .y = 1, .z = 0},
        .radius = 1.0,
        .sphMat = MAT_CREATE_LAMB_IP(material2) 
    };
        
    i += 1;

    MetalMat* material3 = alloc_dynamicStackAllocAllocate(dsa, 
                                       sizeof(MetalMat), 
                                       alignof(MetalMat));     
    material3->albedo.r = 0.7;
    material3->albedo.g = 0.6;
    material3->albedo.b = 0.5;
    material3->fuzz = 0.0;

    spheres[i] = (Sphere){
        .center = {.x = 4, .y = 1, .z = 0},
        .radius = 1.0,
        .sphMat = MAT_CREATE_METAL_IP(material3) 
    };

    i += 1;
    *out = i;
}

#undef randomFloat

int main(int argc, char *argv[]){

    if(argc < 2){
        printf("FATAL ERROR: Output file name not provided.\n");
        printf("EXITING ...\n");
        return 0;
    }

    srand(time(NULL));

    printf("Using Hypatia Version:%s\n", HYPATIA_VERSION);

    const CFLOAT aspect_ratio = 3.0 / 2.0;
    const int WIDTH = 1200;
    const int HEIGHT = (int)(WIDTH/aspect_ratio);
    const int SAMPLES_PER_PIXEL = 500;
    const int MAX_DEPTH = 50;

/*
    LambertianMat materialGround = {
        .albedo = {.r = 0.8, .g = 0.8, .b = 0.0}
    };

    LambertianMat materialCenter = {
        .albedo = {.r = 0.1, .g = 0.2, .b = 0.5}
    }; 

    DielectricMat materialLeft = {
        .ir = 1.5
    };

    MetalMat materialRight = {
        .albedo = {.r = 0.8, .g = 0.6,.b = 0.2},
        .fuzz = 0.0
    }; 

#define numSpheres 5

    Sphere s[numSpheres] = {
        {
        .center = { .x = 0.0, .y = -100.5, .z = -1.0},
        .radius = 100.0,
        .sphMat = {.matLamb = &materialGround, .matType = LAMBERTIAN },
        }, 
         
        {
        .center = { .x = 0.0, .y = 0.0, .z = -1.0},
        .radius = 0.5,
        .sphMat = {.matLamb = &materialCenter, .matType = LAMBERTIAN },
        }, 
        
        {
        .center = {.x = -1.0, .y = 0.0, .z = -1.0},
        .radius = 0.5,
        .sphMat = {.matDielectric = &materialLeft, .matType = DIELECTRIC },
        },

        {
        .center = {.x = -1.0, .y = 0.0, .z = -1.0},
        .radius = -0.45,
        .sphMat = {.matDielectric = &materialLeft, .matType = DIELECTRIC },
        },

        {
        .center = {.x = 1.0, .y = 0.0, .z = -1.0},
        .radius = 0.5,
        .sphMat = {.matMetal = &materialRight, .matType = METAL },
        }
    };
*/  

    DynamicStackAlloc * dsa = alloc_createDynamicStackAllocD(1024, 100);

    Sphere *s = malloc(500 * sizeof(Sphere));
    int numSpheres = 0;

    randomSpheres(500, s, &numSpheres, dsa);

    /*
    Camera c;
    cam_setLookAtCamera(&c, 
                        (vec3){.x = 13.0, .y = 2.0, .z = 3.0}, 
                        (vec3){.x = 0.0, .y = 0.0, .z = 0.0},
                        (vec3){.x = 0.0, .y = 1.0, .z = 0.0}, 
                        20, 
                        aspect_ratio);
   */

    vec3 lookFrom = {.x = 13.0, .y = 2.0, .z = 3.0};
    vec3 lookAt = {.x = 0.0, .y = 0.0, .z = 0.0};
    vec3 up = {.x = 0.0, .y = 1.0, .z = 0.0};

    CFLOAT distToFocus = 10.0;
    CFLOAT aperture = 0.1;

    Camera c;
    cam_setLookAtCamera(&c, lookFrom, lookAt, up, 20, aspect_ratio, aperture, distToFocus);

    Ray r;
    RGBColorF temp;
    
    RGBColorU8* image = (RGBColorU8*) malloc(sizeof(RGBColorF) * HEIGHT * WIDTH);
//    PoolAlloc * hrpa = alloc_createPoolAllocator(sizeof(HitRecord) * MAX_DEPTH * SAMPLES_PER_PIXEL * 2, alignof(HitRecord), sizeof(HitRecord)); 
    LinearAllocFC * lafc = alloc_createLinearAllocFC(MAX_DEPTH * numSpheres, sizeof(HitRecord), alignof(HitRecord));

    CFLOAT pcR, pcG, pcB;

    for (int j = HEIGHT - 1; j >= 0; j--){
        for (int i = 0; i < WIDTH; i++){
            
            pcR = pcG = pcB = 0.0;

            for(int k = 0; k < SAMPLES_PER_PIXEL; k++){
                CFLOAT u = ((CFLOAT)i + util_randomFloat(0.0, 1.0)) / (WIDTH - 1);
                CFLOAT v = ((CFLOAT)j + util_randomFloat(0.0, 1.0)) / (HEIGHT - 1);
                r = cam_getRay(&c, u, v);
              
                temp = ray_c(r, numSpheres, s, MAX_DEPTH, lafc);

                pcR += temp.r;
                pcG += temp.g;
                pcB += temp.b;   

                alloc_linearAllocFCFreeAll(lafc);
            }

            image[i + WIDTH * (HEIGHT - 1 - j)] = writeColor(pcR, pcG, pcB, SAMPLES_PER_PIXEL);
//            alloc_poolAllocFreeAll(hrpa);
        }

        printProgressBar(HEIGHT - 1 - j, HEIGHT - 1);
    }

    writeToPPM(argv[1], WIDTH, HEIGHT, image);
    free(image);
    free(s);
    
//    alloc_freePoolAllocator(hrpa);
    alloc_freeLinearAllocFC(lafc);
    alloc_freeDynamicStackAllocD(dsa);
    return 0;
}
