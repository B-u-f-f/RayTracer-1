#ifndef MATERIAL_H
#define MATERIAL_H

#include "types.h"
#include "ray.h"
#include "hitRecord.h"
#include "hypatiaINC.h"

#include <stdbool.h>

typedef struct metalMat {
    vec3 albedo;
} MetalMat;

typedef struct lambertianMat {
    vec3 albedo;
} LambertianMat;

typedef enum materialType {
    LAMBERTIAN,
    METAL
} MaterialType;

typedef struct material {
    void * mat;
    MaterialType matType;
} Material;


bool mat_scatter (const Material * restrict m, const Ray * restrict rayIn, const HitRecord * restrict rec, const vec3 * restrict color, Ray * restrict out);

#endif