#ifndef MATERIAL_H
#define MATERIAL_H

#include "types.h"
#include "ray.h"
#include "hitRecord.h"
#include "hypatiaINC.h"
#include "color.h"

#include <stdbool.h>

typedef struct metalMat {
    RGBColorF albedo;
    CFLOAT fuzz;
} MetalMat;

typedef struct lambertianMat {
    RGBColorF albedo;
} LambertianMat;

typedef struct dielectricMat {
    CFLOAT ir;
} DielectricMat;

typedef enum materialType {
    LAMBERTIAN,
    METAL,
    DIELECTRIC
} MaterialType;

typedef struct material {
    const MetalMat * matMetal;
    const LambertianMat * matLamb;
    const DielectricMat * matDielectric;
    MaterialType matType;
} Material;


bool mat_scatter (const Ray * restrict rayIn, const HitRecord * restrict rec, RGBColorF * restrict attenuation, Ray * restrict out);

#endif
