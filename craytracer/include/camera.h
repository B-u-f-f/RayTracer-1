#ifndef CAMERA_H
#define CAMERA_H

#include<stdbool.h>

#include "hypatiaINC.h"
#include "types.h"
#include "ray.h"

typedef struct camera {
    vec3 origin;
    CFLOAT aspect_ratio;
    CFLOAT viewport_height;
    CFLOAT focal_length;

    CFLOAT viewport_width;
    vec3 lower_left_corner;
    vec3 horizontal;
    vec3 vertical;

} Camera;

void cam_setCamera(Camera * restrict c);
Ray cam_getRay(const Camera * restrict cam, CFLOAT u, CFLOAT v);


#endif

