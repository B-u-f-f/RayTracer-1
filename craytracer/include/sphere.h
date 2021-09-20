#ifndef SPHERE_H
#define SPHERE_H

#include "hypatiaINC.h"
#include "hitRecord.h"

#include "types.h"

typedef struct {
    // center of the sphere
    vec3 center;

    // radius of the sphere
    CFLOAT radius;
} Sphere;

HitRecord hit(Sphere s, vec3 origin, vec3 direction, CFLOAT t_min, CFLOAT t_max);

#endif

