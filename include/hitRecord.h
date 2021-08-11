#ifndef HITRECORD_H
#define HITRECORD_H

#include<stdbool.h>

#include "hypatiaINC.h"

typedef struct hit_record{
    vec3 p;
    vec3 normal;
    float t;
    bool front_face;
} HitRecord;

HitRecord* setRecord(float T, vec3 P, vec3 Normal, vec3 direction);
void deleteRecord(HitRecord* h);

#endif