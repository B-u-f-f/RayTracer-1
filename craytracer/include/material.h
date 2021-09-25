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
    const MetalMat * matMetal;
    const LambertianMat * matLamb;
    MaterialType matType;
} Material;


bool mat_scatter (const Ray * restrict rayIn, const HitRecord * restrict rec, vec3 * restrict attenuation, Ray * restrict out);

#endif
