#ifndef CAMERA_H
#define CAMERA_H

#include<stdbool.h>

#include "hypatiaINC.h"
#include "types.h"
#include "ray.h"

typedef struct camera {
    vec3 origin;
    vec3 lower_left_corner;
    vec3 horizontal;
    vec3 vertical;
    CFLOAT aspect_ratio;
    CFLOAT viewport_height;
    CFLOAT viewport_width;
    CFLOAT focal_length;

} Camera;

Camera* createCamera(void);
void destroyCamera(Camera* c);
void getRay(const Camera* cam, CFLOAT u, CFLOAT v, Ray* outRay);


#endif

