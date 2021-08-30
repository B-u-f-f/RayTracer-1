#include "sphere.h"

#include <tgmath.h>

Sphere* setSphere(vec3 cen, float r){
    Sphere* s = (Sphere*)malloc(sizeof(Sphere));
    s->center = cen;
    s->radius = r;

    return s;
}

HitRecord* hit(Sphere s, vec3 origin, vec3 direction, float t_min, float t_max){
    vec3 oc = origin;
    vector3_subtract(&oc, &s.center);
    float a = vector3_dot_product(&direction, &direction);
    float half_b = vector3_dot_product(&oc, &direction);
    float c = vector3_dot_product(&oc, &oc) - s.radius * s.radius;

    float discri = half_b * half_b - a*c;
    if(discri < 0) return NULL;

    float sqrtd = sqrt(discri);
    float root = (-half_b - sqrtd) / a;
    
    if(root < t_min || t_max < root){
        root = (-half_b + sqrtd) / a;

        if(root < t_min || t_max < root)
            return NULL;
    }

    float t = root;
    vec3 inter1 = direction;
    vector3_multiplyf(&inter1, root);
    vec3 p = *(vector3_add(&inter1, &origin));
    vec3 n = p;
    vector3_subtract(&n, &s.center);
    vector3_multiplyf(&n, 1/s.radius);

    HitRecord* h = (HitRecord*)malloc(sizeof(HitRecord));
    h = setRecord(t, p, n, direction);

    return h;
}

void deleteSphere(Sphere* s){
    free(s);
}