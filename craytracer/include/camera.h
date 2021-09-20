#ifndef CAMERA_H
#define CAMERA_H

#include<stdbool.h>

#include "hypatiaINC.h"
#include "types.h"

typedef struct camera {
    vec3 origin;
    vec3 lower_left_corner;
    vec3 horizontal;
    vec3 vertical;
    cray_ld aspect_ratio;
    cray_ld viewport_height;
    cray_ld viewport_width;
    cray_ld focal_length;

} Camera;

Camera* createCamera(void);
void destroyCamera(Camera* c);
void getRay(const Camera* cam, cray_ld u, cray_ld v, vec3* outOrigin, vec3* outDirection);


#endif

