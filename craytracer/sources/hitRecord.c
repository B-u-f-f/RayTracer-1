#include "hitRecord.h"

HitRecord hr_setRecord(cray_ld distanceFromOrigin, vec3 point, vec3 normal, vec3 direction){
    // if temp < 0 then the ray has intersected the object at the front face
    // otherwise it has intersected the object at the back face 
    cray_ld temp = vector3_dot_product(&direction, &normal);
    bool frontFace = (temp < 0) ? true : false;

    // Adjusting the normal so it always points away in the opposite direction 
    // of the ray
    if(!frontFace) {
        vector3_negate(&normal);
    }
    
    return (HitRecord){
        .point = point,
        .distanceFromOrigin = distanceFromOrigin,
        .normal = normal,
        .valid = true,
        .frontFace = frontFace
    };
}

