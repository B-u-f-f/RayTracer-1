#include "sphere.h"
#include <tgmath.h>

HitRecord* hit(Sphere s, Ray r, CFLOAT t_min, CFLOAT t_max){
    vec3 oc = r.origin;
    vec3 direction = r.direction;

    /* 
     * center - center of the sphere
     * radius - radius of the sphere
     * direction - direction of the ray
     */

    // oc = origin - center
    vector3_subtract(&oc, &s.center);

    // a = dot(direction, direction)
    CFLOAT a = vector3_dot_product(&direction, &direction);

    // half_b = dot((origin - center), direction)
    CFLOAT half_b = vector3_dot_product(&oc, &direction);
    
    // c = dot(origin, origin) - radius^2
    CFLOAT c = vector3_dot_product(&oc, &oc) - s.radius * s.radius;

    // discri = half_b^2 - a * c
    CFLOAT discri = half_b * half_b - a*c;

    // If the discriminant is less than 0 then no intersection
    if(discri < 0){
        return NULL;
    }

    // sqrtd = sqrt(discri)
    CFLOAT sqrtd = sqrt(discri);

    // root1 = (-half_b - sqrtd) / a
    CFLOAT root = (-half_b - sqrtd) / a;

    // If the intersection point corresponding to this root is 
    // not in the intersection range then check the other point    
    if(root < t_min || t_max < root){
        root = (-half_b + sqrtd) / a;

        // If neither roots correspond to an intersection point in
        // the intersection range then return invalid
        if(root < t_min || t_max < root){
            return NULL;
     
        }
    }

    // t = root
    CFLOAT t = root;

    // inter1 = direction
    vec3 inter1 = direction;

    // inter1 = root * direction
    vector3_multiplyf(&inter1, root);

    // p = inter1 + origin
    vec3 p = *(vector3_add(&inter1, &r.origin));
    // n = p
    vec3 n = p;
    
    // n = p - center
    vector3_subtract(&n, &s.center);
    
    // n = (p - center)/radius
    vector3_multiplyf(&n, 1/s.radius);
    
    HitRecord * outRecord = (HitRecord *) malloc(sizeof(HitRecord));
    hr_setRecordi(t, p, n, direction, outRecord);
    return outRecord;
}

