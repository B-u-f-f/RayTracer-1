#include <stdlib.h>
#include <stdbool.h>

#include "util.h"

float clamp(float c, float lower, float upper){
    if(c <= lower){
        return lower;
    }

    if(c >= upper){
        return upper;
    }

    return c;
}

int randomRange(int lower, int upper){
    return (rand() % (upper - lower + 1)) + lower;
}

vec3 randomInUnitSphere(){
    float x, y, z;
    while(true){
        x = randomFloat(0.0f, 1.0f);
        y = randomFloat(0.0f, 1.0f);
        z = randomFloat(0.0f, 1.0f);

        if(x*x + y*y + z*z >= 1){
            continue;
        }else{
            break;
        }
    }

    vec3 r;
    vector3_setf3(&r, x, y, z);
    return r;
}

float randomFloat(float lower, float upper){
    float scale = rand() / (float) RAND_MAX;
    return scale * (upper - lower) + lower;
}