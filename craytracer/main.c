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

vec3 writeColor(vec3 pixel_color, int sample_per_pixel){
    CFLOAT r = pixel_color.x;
    CFLOAT g = pixel_color.y;
    CFLOAT b = pixel_color.z;

    CFLOAT scale = 1.0/sample_per_pixel;

    r = sqrt(scale * r);
    g = sqrt(scale * g);
    b = sqrt(scale * b);

    vec3 temp;
    
    vector3_setf3(&temp, 255.999 * util_floatClamp(r,0.0,0.999),
                         255.999 * util_floatClamp(g,0.0,0.999),
                         255.999 * util_floatClamp(b,0.0,0.999));

    return temp;
}

HitRecord* hittableList(int n, Sphere *sphere[n], Ray ray, CFLOAT t_min, CFLOAT t_max){
    HitRecord * r = NULL;
    HitRecord * h = NULL;

    for(int i = 0; i < n; i++){
        r = hit(*sphere[i], ray, t_min, t_max);

        if(r != NULL){
            h = r;
        }
    }    
    return h;
}

vec3 ray_c(Ray r, int n, Sphere* sphere[n], int depth){
    if(depth <= 0){
        return (vec3){0};
    }

    HitRecord *  rec = hittableList(n, sphere, r, 0.1, FLT_MAX);
    if(rec != NULL){
        Ray scattered = {0};
        vec3 attenuation = {0};
        
        if(mat_scatter(&r, rec, &attenuation, &scattered)){
            vec3 color = ray_c(scattered, n, sphere, depth - 1);
            vector3_multiply(&color, &attenuation);
            
            return color;
        }

        return (vec3){0};
    }

    vec3 ud = r.direction;
    vector3_normalize(&ud);
    CFLOAT t = 0.5 * (ud.y + 1.0);
    vec3 inter4;
    vector3_setf3(&inter4, 1.0 - t, 1.0 - t, 1.0 - t);
    vec3 inter3;
    vector3_setf3(&inter3, 0.5 * t, 0.7 * t, 1.0 * t);
    vector3_add(&inter3, &inter4);
    
    return inter3;
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
    
    vec3* image = (vec3*) malloc(sizeof(vec3) * HEIGHT * WIDTH);

    LambertianMat materialGround = {
        .albedo = {.x = 0.8, .y = 0.8, .z = 0.0}
    };

    LambertianMat materialCenter = {
        .albedo = {.x = 0.7, .y = 0.3, .z = 0.3}
    }; 

    MetalMat materialLeft = {
        .albedo = {.x = 0.8, .y = 0.8, .z = 0.8}
    };
    MetalMat materialRight = {
        .albedo = {.x = 0.8, .y = 0.6,.z = 0.2}
    }; 


    Sphere * s[4];
    
    Sphere s1 = {
        .center = { .x = 0.0, .y = -100.5, .z = -1.0},
        .radius = 100,
        .sphMat = {
            .mat = &materialGround,
            .matType = LAMBERTIAN
        },
    };
    Sphere s2 = {
        .center = {
            .x = 0.0, 
            .y = 0.0,
            .z = -1.0
        },
        .radius = 0.5,
        .sphMat = {
            .mat = &materialCenter,
            .matType = LAMBERTIAN,
        },
    };
    Sphere s3 = {
        .center = {
            .x = -1.0, 
            .y = 0.0,
            .z = -1.0
        },
        .radius = 0.5,
        .sphMat = {
            .mat = &materialLeft,
            .matType = METAL,
        },
    };
    Sphere s4 = {
        .center = {
            .x = 1.0, 
            .y = 0.0,
            .z = -1.0
        },
        
        .radius = 0.5,
        .sphMat = {
            .mat = &materialRight,
            .matType = METAL,
        },
    };


    s[0] = &s1;
    s[1] = &s2;
    s[2] = &s3;
    s[3] = &s4;

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

    vec3 pixel_color;
    Ray r;
    vec3 temp;
    

    for (int j = HEIGHT - 1; j >= 0; j--){

        for (int i = 0; i < WIDTH; i++){

            vector3_zero(&pixel_color);

            for(int k = 0; k < SAMPLES_PER_PIXEL; k++){
                CFLOAT u = ((CFLOAT)i + util_randomFloat(0.0, 1.0)) / (WIDTH - 1);
                CFLOAT v = ((CFLOAT)j + util_randomFloat(0.0, 1.0)) / (HEIGHT - 1);
                r = cam_getRay(&c, u, v);
                temp = ray_c(r, 4, s, MAX_DEPTH);
                vector3_add(&pixel_color, &temp);    
            }

            temp = writeColor(pixel_color, SAMPLES_PER_PIXEL);
            vector3_set(&image[i + WIDTH * (HEIGHT - 1 - j)], &temp);
        }

        printProgressBar(HEIGHT - 1 - j, HEIGHT - 1);
    }

    writeToPPM(argv[1], WIDTH, HEIGHT, image);
    free(image);

    return 0;
}
