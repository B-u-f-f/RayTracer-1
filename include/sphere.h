#ifndef SPHERE_H
#define SPHERE_H

#include "hypatiaINC.h"
#include "hitRecord.h"

typedef struct sphere{
    vec3 center;
    float radius;
} Sphere;

Sphere* setSphere(vec3 cen, float r);

HitRecord* hit(Sphere s, vec3 origin, vec3 direction, float t_min, float t_max);

void deleteSphere(Sphere* s);

#endif