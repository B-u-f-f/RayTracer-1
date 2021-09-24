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

    // radius of the sphere
    CFLOAT radius;

    //material
    Material sphMat;
} Sphere;

extern HitRecord* hit(Sphere s, Ray r, CFLOAT t_min, CFLOAT t_max);

#endif

