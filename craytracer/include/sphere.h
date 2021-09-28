#ifndef SPHERE_H
#define SPHERE_H

#include <stdbool.h>

#include "hypatiaINC.h"
#include "hitRecord.h"

#include "types.h"
#include "ray.h"
#include "material.h"

typedef struct {
    // center of the sphere
    vec3 center;

    //material
    Material sphMat;

    // radius of the sphere
    CFLOAT radius;
} Sphere;

extern void hit(const Sphere* restrict s, Ray r, CFLOAT t_min, CFLOAT t_max, HitRecord * outRecord);

#endif

