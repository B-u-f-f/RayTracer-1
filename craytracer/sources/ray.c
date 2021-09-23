#include "ray.h"

void normalize_ray_direction(Ray *ray){
    vec3 d = ray->direction;

    vector3_normalize(&direction);

    ray->direction = d;
}