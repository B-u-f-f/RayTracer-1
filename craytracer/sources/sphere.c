#include "sphere.h"
#include <stdalign.h>
#include <tgmath.h>

void obj_sphereHit(const Sphere* restrict s, Ray r, CFLOAT t_min, CFLOAT t_max, HitRecord * outRecord){
    vec3 oc = r.origin;
    vec3 direction = r.direction;

    /* 
     * center - center of the sphere
     * radius - radius of the sphere
     * direction - direction of the ray
     */

    // oc = origin - center
    vector3_subtract(&oc, &s->center);

    // a = dot(direction, direction)
    CFLOAT a = vector3_dot_product(&direction, &direction);

    // half_b = dot((origin - center), direction)
    CFLOAT half_b = vector3_dot_product(&oc, &direction);
    
    // c = dot(origin, origin) - radius^2
    CFLOAT c = vector3_dot_product(&oc, &oc) - s->radius * s->radius;

    // discri = half_b^2 - a * c
    CFLOAT discri = half_b * half_b - a*c;

    // If the discriminant is less than 0 then no intersection
    if(discri < 0){
        outRecord->valid = false;
        return;
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
            outRecord->valid = false; 
            return;
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
    vector3_subtract(&n, &s->center);
    
    // n = (p - center)/radius
    vector3_multiplyf(&n, 1/s->radius);
    
    hr_setRecordi(t, p, n, direction, outRecord, &s->sphMat);
}

bool obj_objectLLAdd(
        ObjectLL * restrict objll, 
        DynamicStackAlloc * restrict dsa, 
        void * restrict obj, 
        ObjectType objType
)
{
    if(!objll || !objll->valid){
        return false;
    }

    ObjectLLNode * olln = alloc_dynamicStackAllocAllocate(dsa, 
                          sizeof(ObjectLLNode), alignof(ObjectLLNode)); 
    
    olln->object = obj; 
    olln->objType = objType;
    if(!objll->head){
        olln->next = NULL;
    }else{
        olln->next = objll->head->next;
    }
    objll->head = olln;
    objll->numObjects += 1;

    return true;
}

bool obj_objectLLRemove(ObjectLL * restrict objll, size_t index){
    if(!objll || !objll->valid){
        return false;
    }
    
    if(objll->numObjects <= index){
        return false;
    }
    
    if(index == 0){
        objll->head = objll->head->next;
        return true;  
    }

    size_t i = 0;
    ObjectLLNode * cur = objll->head;
    while(i < index){
        cur = cur->next;
        i++;
    }
    
    cur->next = cur->next->next;

    return true;
}

