#include <stdlib.h>
#include <stdbool.h>

#include "util.h"

cray_ld util_ldClamp(cray_ld c, cray_ld lower, cray_ld upper){
    if(c <= lower){
        return lower;
    }

    if(c >= upper){
        return upper;
    }

    return c;
}

/*
int randomRange(int lower, int upper){
    return (rand() % (upper - lower + 1)) + lower;
}
*/

vec3 util_randomInUnitSphere(){
    cray_ld x, y, z;
    while(true){
        x = util_randomLD(0.0, 1.0);
        y = util_randomLD(0.0, 1.0);
        z = util_randomLD(0.0, 1.0);

        if(x*x + y*y + z*z >= 1){
            continue;
        }else{
            break;
        }
    }

    return (vec3){
        .x = x,
        .y = y,
        .z = z
    };
}

cray_ld util_randomLD(cray_ld lower, cray_ld upper){
    cray_ld scale = rand() / (cray_ld) RAND_MAX;
    return scale * (upper - lower) + lower;
}
