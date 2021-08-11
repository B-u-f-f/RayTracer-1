#ifndef CAMERA_H
#define CAMERA_H

#include<stdbool.h>

#include "hypatiaINC.h"

typedef struct camera {
    vec3 origin;
    vec3 lower_left_corner;
    vec3 horizontal;
    vec3 vertical;
    float aspect_ratio;
    float viewport_height;
    float viewport_width;
    float focal_length;

} Camera;

Camera* createCamera(void);
void destroyCamera(Camera* c);
void getRay(const Camera* cam, float u, float v, vec3* outOrigin, vec3* outDirection);


#endif