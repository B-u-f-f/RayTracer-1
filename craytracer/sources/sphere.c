#include "sphere.h"

#include <tgmath.h>

HitRecord hit(Sphere s, vec3 origin, vec3 direction, cray_ld t_min, cray_ld t_max){
    vec3 oc = origin;

    /* 
     * center - center of the sphere
     * radius - radius of the sphere
     * direction - direction of the ray
     */

    // oc = origin - center
    vector3_subtract(&oc, &s.center);

    // a = dot(direction, direction)
    cray_ld a = vector3_dot_product(&direction, &direction);

    // half_b = dot((origin - center), direction)
    cray_ld half_b = vector3_dot_product(&oc, &direction);
    
    // c = dot(origin, origin) - radius^2
    cray_ld c = vector3_dot_product(&oc, &oc) - s.radius * s.radius;

    // discri = half_b^2 - a * c
    cray_ld discri = half_b * half_b - a*c;

    // If the discriminant is less than 0 then no intersection
    if(discri < 0){
        return (HitRecord){0};
    }

    // sqrtd = sqrt(discri)
    cray_ld sqrtd = sqrt(discri);

    // root1 = (-half_b - sqrtd) / a
    cray_ld root = (-half_b - sqrtd) / a;

    // If the intersection point corresponding to this root is 
    // not in the intersection range then check the other point    
    if(root < t_min || t_max < root){
        root = (-half_b + sqrtd) / a;

        // If neither roots correspond to an intersection point in
        // the intersection range then return invalid
        if(root < t_min || t_max < root)
            return (HitRecord){0};
    }

    // t = root
    cray_ld t = root;

    // inter1 = direction
    vec3 inter1 = direction;

    // inter1 = root * direction
    vector3_multiplyf(&inter1, root);

    // p = inter1 + origin
    vec3 p = *(vector3_add(&inter1, &origin));
    // n = p
    vec3 n = p;
    
    // n = p - center
    vector3_subtract(&n, &s.center);
    
    // n = (p - center)/radius
    vector3_multiplyf(&n, 1/s.radius);
    
    return hr_setRecord(t, p, n, direction);
}

