#include "hitRecord.h"

HitRecord* setRecord(float T, vec3 P, vec3 Normal, vec3 direction){
    HitRecord* hit = (HitRecord*)malloc(sizeof(HitRecord));
    hit->t = T;
    hit->p = P;
    float temp;
    temp = vector3_dot_product(&direction, &Normal);
    if(temp < 0) hit->front_face = true;
    else hit->front_face = false;

    hit->normal = Normal;

    if(!hit->front_face) 
        vector3_negate(&hit->normal);

    return hit;
}

void deleteRecord(HitRecord* h){
    free(h);
}