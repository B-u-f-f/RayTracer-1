#include "camera.h"
#include <stdlib.h>

void cam_setCamera(Camera * restrict c){    
    c->viewport_width = c->aspect_ratio*c->viewport_height;
    c->vertical.y = c->viewport_height;
    c->horizontal.x = c->viewport_width;

    c->lower_left_corner.x = c->origin.x - c->horizontal.x/2 - c->vertical.x/2 - 0.0;
    c->lower_left_corner.y = c->origin.y - c->horizontal.y/2 - c->vertical.y/2 - 0.0;
    c->lower_left_corner.z = c->origin.z - c->horizontal.z/2 - c->vertical.z/2 - c->focal_length; 
}


Ray cam_getRay(const Camera * restrict cam, CFLOAT u, CFLOAT v){
    vec3 direction = {
        .x = cam->lower_left_corner.x + u * cam->horizontal.x + v * cam->vertical.x - cam->origin.x,
        .y = cam->lower_left_corner.y + u * cam->horizontal.y + v * cam->vertical.y - cam->origin.y,
        .z = cam->lower_left_corner.z + u * cam->horizontal.z + v * cam->vertical.z - cam->origin.z
    };
    
    return ray_create(cam->origin, direction);
}
