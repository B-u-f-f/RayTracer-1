#ifndef RAY_H
#define RAY_H

#include "hypatiaINC.h"

typedef struct {

    // origin of the ray
    vec3 origin;

    // direction of the ray
    vec3 direction;
} Ray;

void normalize_ray_direction(Ray *ray);

#endif