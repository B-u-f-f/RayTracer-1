#include "sphere.h"
#include <stdalign.h>
#include <tgmath.h>


static void hit(
        const ObjectLLNode * restrict objLLn, 
        Ray r, 
        CFLOAT t_min, 
        CFLOAT t_max, HitRecord * outRecord )
{
    if(objLLn->objType == SPHERE){
        obj_sphereHit((const Sphere *)objLLn->object, r, t_min, t_max, outRecord);
    }/*else if(objLLn->objType == OBJLL){
        obj_
    }*/
}


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
        void * restrict obj, 
        ObjectType objType
)
{
    if(!objll || !objll->valid){
        return false;
    }

    ObjectLLNode * olln = alloc_dynamicStackAllocAllocate(objll->dsa, 
                          sizeof(ObjectLLNode), alignof(ObjectLLNode)); 
    
    olln->object = obj;
    olln->objType = objType;

    if(!objll->head){
        olln->next = NULL;
    }else{
        olln->next = objll->head;
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
    objll->numObjects -= 1;

    return true;
}

HitRecord * obj_objLLHit (const ObjectLL* restrict objll,
                   Ray r, 
                   CFLOAT t_min,
                   CFLOAT t_max){
    
    if(!objll || !objll->valid){
        return NULL;
    }

    HitRecord * hr = (HitRecord *) alloc_linearAllocFCAllocate(objll->hrAlloc);
    HitRecord * h = NULL;

    ObjectLLNode * cur = objll->head;
    while(cur != NULL){
        hit(cur, r, t_min, t_max, hr);
        
        if(hr->valid){
            if(h == NULL ){
                h = hr;
                hr = (HitRecord *) alloc_linearAllocFCAllocate(objll->hrAlloc);
            }else if(hr->distanceFromOrigin < h->distanceFromOrigin){
                h = hr;
                hr = (HitRecord *) alloc_linearAllocFCAllocate(objll->hrAlloc);
            }

        }

        cur = cur->next;
    }


    return h;
}


bool obj_objLLAddSphere(ObjectLL * restrict objll,
        Sphere sphere)
{
    Sphere * s = alloc_dynamicStackAllocAllocate(objll->dsa, sizeof(Sphere), alignof(Sphere));
    *s = sphere; 
    if(!s) { return false; }

    return obj_objectLLAdd(objll, (void *) s, SPHERE);
}

ObjectLL * obj_createObjectLL(    
    DynamicStackAlloc * restrict dsaAlloc, 
    DynamicStackAlloc * restrict dsaObjs
){
    
    ObjectLL * objLL = alloc_dynamicStackAllocAllocate(dsaAlloc, sizeof(ObjectLL), alignof(ObjectLL));

    objLL->numObjects = 0;
    objLL->head = NULL;
    objLL->valid = true;
    objLL->dsa = dsaObjs;
    objLL->hrAlloc = NULL; 

    return objLL;
}

bool obj_AABBHit(const AABB* restrict s, Ray r, CFLOAT t_min, CFLOAT t_max){
    for(int i = 0; i < 3; i++){
        CFLOAT t0 = CF_MIN(
            (s->minimum.v[i] - r.origin.v[i])/(r.direction.v[i]),
            (s->maximum.v[i] - r.origin.v[i])/(r.direction.v[i])
        );

        CFLOAT t1 = CF_MAX(
            (s->minimum.v[i] - r.origin.v[i]) / r.direction.v[i],
            (s->maximum.v[i] - r.origin.v[i]) / r.direction.v[i]
        );

        t_min = CF_MAX(t0, t_min);
        t_max = CF_MIN(t1, t_max);

        if(t_max<=t_min){
            return false;
        }
    }
    return true;
}

bool obj_sphereCalcBoundingBox(const Sphere* restrict s, AABB * outbox){
    outbox->minimum.x = s->center.x - s->radius;
    outbox->minimum.y = s->center.y - s->radius;
    outbox->minimum.z = s->center.z - s->radius;

    outbox->maximum.x = s->center.x + s->radius;
    outbox->maximum.y = s->center.y + s->radius;
    outbox->maximum.z = s->center.z + s->radius;
    
    return true;
}

static AABB surrounding_box(const AABB* restrict box0, const AABB* restrict box1){
    vec3 small;
    vec3 big;
    AABB temp_AABB;

    small.x = CF_MIN(box0->minimum.x, box1->minimum.x);
    small.y = CF_MIN(box0->minimum.y, box1->minimum.y);
    small.z = CF_MIN(box0->minimum.z, box1->minimum.z);
    
    big.x = CF_MAX(box0->maximum.x, box1->maximum.x);
    big.y = CF_MAX(box0->maximum.y, box1->maximum.y);
    big.z = CF_MAX(box0->maximum.z, box1->maximum.z);

    temp_AABB.minimum = small;
    temp_AABB.maximum = big;
    return temp_AABB;
    
}

static bool boundingBox(const ObjectLLNode * restrict objLLn, AABB* outbox){
    if(objLLn->objType == SPHERE){
        return obj_sphereCalcBoundingBox(((const Sphere *)objLLn->object), outbox);
    }
    
    return false;
}

bool obj_objectLLCalcBoundingBox(const ObjectLL* restrict objll, AABB * outbox){
    if(objll->numObjects == 0) return false;
    
    AABB tempBox;
    bool firstBox = true;

    ObjectLLNode* cur = objll->head;

    while(cur != NULL){
        
        if(!boundingBox(cur, &tempBox)) return false;

        
        if(firstBox){
            *outbox = tempBox;
        }else{
            *outbox = surrounding_box(outbox, &tempBox);
        }
        
        firstBox = false;
        cur = cur->next;
    }    

    return true;
}

