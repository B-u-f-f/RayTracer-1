#ifndef HITRECORD_H
#define HITRECORD_H

#include <stdbool.h>

#include "hypatiaINC.h"
#include "types.h"

typedef struct {
    // intersection point of ray and sphere
    vec3 point;

    // normal at the point of intersection
    vec3 normal;

    // distance between origin and point
    cray_ld distanceFromOrigin;
    
    // the surface of the object where ray
    // intersected the object
    bool frontFace;

    // true when this structure is valid
    bool valid;
} HitRecord;

// construct a hit record object and return it
HitRecord hr_setRecord(cray_ld distanceFromOrigin, vec3 point, vec3 normal, vec3 direction);

#endif

