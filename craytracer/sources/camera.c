#include "camera.h"
#include <stdlib.h>
#include <tgmath.h>

static CFLOAT degreesToRadians(CFLOAT deg){
    return (M_PI / 180.0) * deg; 
}

void cam_setCamera(Camera * restrict c, vec3 origin, CFLOAT aspectRatio, 
                          CFLOAT focalLength, CFLOAT vfov){      
    
    c->origin = origin;
    c->aspectRatio = aspectRatio;
    c->focalLength = focalLength;

    c->verticalFOV = degreesToRadians(vfov); 

    c->viewportHeight = 2.0 * tanf(c->verticalFOV / 2.0); 
    c->viewportWidth = c->aspectRatio * c->viewportHeight;

    c->vertical.y = c->viewportHeight;
    c->horizontal.x = c->viewportWidth;

    c->lowerLeftCorner.x = c->origin.x - c->horizontal.x/2 - c->vertical.x/2 - 0.0;
    c->lowerLeftCorner.y = c->origin.y - c->horizontal.y/2 - c->vertical.y/2 - 0.0;
    c->lowerLeftCorner.z = c->origin.z - c->horizontal.z/2 - c->vertical.z/2 - c->focalLength; 
}

void cam_setLookAtCamera(Camera * restrict c, vec3 lookFrom, 
                         vec3 lookAt, vec3 up, CFLOAT vfov, 
                         CFLOAT aspectRatio){

    c->aspectRatio = aspectRatio;

    c->verticalFOV = degreesToRadians(vfov); 

    c->viewportHeight = 2.0 * tanf(c->verticalFOV / 2.0); 
    c->viewportWidth = c->aspectRatio * c->viewportHeight;
    
    // w = dir(lookFrom - lookAt)
    vec3 w = lookFrom;
    vector3_subtract(&w, &lookAt);
    vector3_normalize(&w);
    
    // u = dir(up - w)
    vec3 u;
    vector3_cross_product(&u, &up, &w);
    vector3_normalize(&u);
    
    // v = cross (w, u)
    vec3 v;
    vector3_cross_product(&v, &w, &u);
    
    //origin = lookFrom 
    c->origin = lookFrom;

    // horizontal = viewport_width * u
    c->horizontal = u;
    vector3_multiplyf(&c->horizontal, c->viewportWidth);
    
    // vertical = viewport_height * v
    c->vertical = v;
    vector3_multiplyf(&c->vertical, c->viewportHeight);
    
    // lower_left_corner = origin - (horizontal + vertical)/2 - w
    c->lowerLeftCorner = c->horizontal;
    vector3_add(&c->lowerLeftCorner, &c->vertical);
    vector3_multiplyf(&c->lowerLeftCorner, -0.5);
    vector3_subtract(&c->lowerLeftCorner, &w);
    vector3_add(&c->lowerLeftCorner, &c->origin);
}

Ray cam_getRay(const Camera * restrict cam, CFLOAT u, CFLOAT v){
    vec3 direction = {
        .x = cam->lowerLeftCorner.x + u * cam->horizontal.x + v * cam->vertical.x - cam->origin.x,
        .y = cam->lowerLeftCorner.y + u * cam->horizontal.y + v * cam->vertical.y - cam->origin.y,
        .z = cam->lowerLeftCorner.z + u * cam->horizontal.z + v * cam->vertical.z - cam->origin.z
    };
    
    return ray_create(cam->origin, direction);
}
