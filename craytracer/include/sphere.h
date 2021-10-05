#ifndef SPHERE_H
#define SPHERE_H

#include <stdbool.h>

#include "hypatiaINC.h"
#include "hitRecord.h"

#include "types.h"
#include "ray.h"
#include "material.h"
#include "allocator.h"

typedef struct {
    // center of the sphere
    vec3 center;

    //material
    Material sphMat;

    // radius of the sphere
    CFLOAT radius;
} Sphere;

extern void obj_sphereHit(const Sphere* restrict s, Ray r, CFLOAT t_min, CFLOAT t_max, HitRecord * outRecord);


// enum contaning different types of objects
typedef enum {
    SPHERE
} ObjectType;

// node of the linked list 
typedef struct objectLLNode ObjectLLNode;
typedef struct objectLLNode{
    // ptr to the object stored in this node
    void * restrict object;

    // type of the object 
    ObjectType objType; 
    
    // points to the next node
    ObjectLLNode * restrict next;
} ObjectLLNode;

// linked list storing pointer to objects in the scene
typedef struct objectLL {
    // number of objects 
    size_t numObjects;
    
    // points to the first node in the linked list 
    ObjectLLNode * restrict head;
     
    // whether the object is valid or not
    bool valid;
} ObjectLL; 


   
// function to add an object to the linked list
// returns true if the operation is successful
extern bool obj_objectLLAdd(
        ObjectLL * restrict objll, 
        DynamicStackAlloc * restrict dsa, 
        void * restrict obj, 
        ObjectType objType
);

#define OBJ_OBJECTLLADD(objll, obj) _Generic((obj), \
        (Sphere*): obj_objectLLAdd(objll, (void*) obj, SPHERE))
 
// remove an object at any index
extern bool obj_objectLLRemove(ObjectLL * restrict objll, size_t index); 



#endif

