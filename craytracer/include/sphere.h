#ifndef SPHERE_H
#define SPHERE_H

#include "hypatiaINC.h"
#include "hitRecord.h"

#include "types.h"

typedef struct {
    // center of the sphere
    vec3 center;

    // radius of the sphere
    cray_ld radius;
} Sphere;

HitRecord hit(Sphere s, vec3 origin, vec3 direction, cray_ld t_min, cray_ld t_max);

#endif

