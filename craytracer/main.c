#define HYPATIA_IMPLEMENTATION

#include <stdio.h>
#include <assert.h>
#include <tgmath.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>

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

RGBColorU8 writeColor(RGBColorF pixel_color, int sample_per_pixel){
    CFLOAT r = pixel_color.r;
    CFLOAT g = pixel_color.g;
    CFLOAT b = pixel_color.b;

    CFLOAT scale = 1.0/sample_per_pixel;

    r = sqrt(scale * r);
    g = sqrt(scale * g);
    b = sqrt(scale * b);

    return COLOR_U8CREATE(r, g, b);
}

HitRecord* hittableList(int n, Sphere sphere[n], Ray ray, CFLOAT t_min, CFLOAT t_max){
    HitRecord * r = NULL;
    HitRecord * h = NULL;

    for(int i = 0; i < n; i++){
        r = hit(&sphere[i], ray, t_min, t_max);

        if(r != NULL){
            h = r;
        }
    }    
    return h;
}

RGBColorF ray_c(Ray r, int n, Sphere sphere[n], int depth){
    if(depth <= 0){
        return (RGBColorF){0};
    }

    HitRecord *  rec = hittableList(n, sphere, r, 0.1, FLT_MAX);
    if(rec != NULL){
        Ray scattered = {0};
        RGBColorF attenuation = {0};
        
        if(mat_scatter(&r, rec, &attenuation, &scattered)){
            RGBColorF color = ray_c(scattered, n, sphere, depth - 1);
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


int main(int argc, char *argv[]){
    if(argc < 2){
        printf("FATAL ERROR: Output file name not provided.\n");
        printf("EXITING ...\n");
        return 0;
    }

    srand(time(NULL));

    printf("Using Hypatia Version:%s\n", HYPATIA_VERSION);

    const CFLOAT aspect_ratio = 16.0 / 9.0;
    const int WIDTH = 100;
    const int HEIGHT = (int)(WIDTH/aspect_ratio);
    const int SAMPLES_PER_PIXEL = 100;
    const int MAX_DEPTH = 50;

    LambertianMat materialGround = {
        .albedo = {.r = 0.8, .g = 0.8, .b = 0.0}
    };

    LambertianMat materialCenter = {
        .albedo = {.r = 0.7, .g = 0.3, .b = 0.3}
    }; 

    MetalMat materialLeft = {
        .albedo = {.r = 0.8, .g = 0.8, .b = 0.8},
        .fuzz = 0.3
    };
    MetalMat materialRight = {
        .albedo = {.r = 0.8, .g = 0.6,.b = 0.2},
        .fuzz = 1.0
    }; 


    Sphere  s[4] = {
        {
        .center = { .x = 0.0, .y = -100.5, .z = -1.0},
        .radius = 100,
        .sphMat = {.matLamb = &materialGround, .matType = LAMBERTIAN },
        },
        
        {
        .center = {.x = 0.0, .y = 0.0, .z = -1.0},
        .radius = 0.5,
        .sphMat = {.matLamb = &materialCenter, .matType = LAMBERTIAN },
        },

        
        {
        .center = {.x = -1.0, .y = 0.0, .z = -1.0},
        .radius = 0.5,
        .sphMat = {.matMetal = &materialLeft, .matType = METAL},
        },
        
        {
        .center = {.x = 1.0, .y = 0.0, .z = -1.0},
        
        .radius = 0.5,
        .sphMat = {.matMetal = &materialRight, .matType = METAL},
        }
    };
    
    Camera c = {
        .origin = {
            .x = 0.0,
            .y = 0.0
        },

        .aspect_ratio = 16.0/9.0,
        .viewport_height = 2.0,
        .focal_length = 1.0
    };

    cam_setCamera(&c);

    RGBColorF pixel_color;
    Ray r;
    RGBColorF temp;
    
    RGBColorU8* image = (RGBColorU8*) malloc(sizeof(RGBColorF) * HEIGHT * WIDTH);

    for (int j = HEIGHT - 1; j >= 0; j--){
        for (int i = 0; i < WIDTH; i++){

            pixel_color = (RGBColorF){.r = 0.0, .g = 0.0, .b = 0.0};

            for(int k = 0; k < SAMPLES_PER_PIXEL; k++){
                CFLOAT u = ((CFLOAT)i + util_randomFloat(0.0, 1.0)) / (WIDTH - 1);
                CFLOAT v = ((CFLOAT)j + util_randomFloat(0.0, 1.0)) / (HEIGHT - 1);
                r = cam_getRay(&c, u, v);
                temp = ray_c(r, 4, s, MAX_DEPTH);
                pixel_color = colorf_add(pixel_color, temp);   
            }

            image[i + WIDTH * (HEIGHT - 1 - j)] = writeColor(pixel_color, SAMPLES_PER_PIXEL);
            
        }

        printProgressBar(HEIGHT - 1 - j, HEIGHT - 1);
    }

    writeToPPM(argv[1], WIDTH, HEIGHT, image);
    free(image);

    return 0;
}
