#include "camera.h"

#include <stdlib.h>

Camera* createCamera(){
    Camera* c = (Camera*) malloc(sizeof(Camera));
    vector3_zero(&c->origin);
    vector3_zero(&c->lower_left_corner);
    vector3_zero(&c->horizontal);
    vector3_zero(&c->vertical);
    
    c->aspect_ratio = 16.0/9.0;
    c->viewport_height = 2.0;
    c->viewport_width = c->aspect_ratio*c->viewport_height;
    c->focal_length = 1.0;
    
    c->vertical.y = c->viewport_height;
    c->horizontal.x = c->viewport_width;

    c->lower_left_corner.x = c->origin.x - c->horizontal.x/2 - c->vertical.x/2 - 0.0;
    c->lower_left_corner.y = c->origin.y - c->horizontal.y/2 - c->vertical.y/2 - 0.0;
    c->lower_left_corner.z = c->origin.z - c->horizontal.z/2 - c->vertical.z/2 - c->focal_length;
    
    return c;    
}


void destroyCamera(Camera* c){
    free(c);
}


void getRay(const Camera* cam, cray_ld u, cray_ld v, vec3* outOrigin, vec3* outDirection){
    *outOrigin = cam->origin;

    outDirection->x = cam->lower_left_corner.x + u * cam->horizontal.x + v * cam->vertical.x - cam->origin.x;
    outDirection->y = cam->lower_left_corner.y + u * cam->horizontal.y + v * cam->vertical.y - cam->origin.y;
    outDirection->z = cam->lower_left_corner.z + u * cam->horizontal.z + v * cam->vertical.z - cam->origin.z;
}
