#include <stdlib.h>
#include <stdbool.h>
#include <tgmath.h>
#include <assert.h>

#include "util.h"

static bool nearly_zero(CFLOAT a, CFLOAT epsilon, CFLOAT abs_th){
    assert(F_EPSILON <= epsilon);
    assert(epsilon < 1.0);

    if (a == 0.0) return true;

    CFLOAT diff = fabs(a);
    CFLOAT norm = fmin(diff, F_MAX);
    return diff < fmax(abs_th, epsilon * norm);
}


CFLOAT util_floatClamp(CFLOAT c, CFLOAT lower, CFLOAT upper){
    if(c <= lower){
        return lower;
    }

    if(c >= upper){
        return upper;
    }

    return c;
}

uint8_t util_uint8Clamp(uint8_t c, uint8_t lower, uint8_t upper){
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

vec3 util_randomUnitSphere(){
    CFLOAT x, y, z;
    while(true){
        x = util_randomFloat(0.0, 1.0);
        y = util_randomFloat(0.0, 1.0);
        z = util_randomFloat(0.0, 1.0);

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

CFLOAT util_randomFloat(CFLOAT lower, CFLOAT upper){
    CFLOAT scale = rand() / (CFLOAT) RAND_MAX;
    return scale * (upper - lower) + lower;
}


vec3 util_randomUnitVector(){
    
    CFLOAT x, y, z;
    while(true){
        x = util_randomFloat(0.0, 1.0);
        y = util_randomFloat(0.0, 1.0);
        z = util_randomFloat(0.0, 1.0);

        if(x*x + y*y + z*z > 1){
            continue;
        }else{
            break;
        }
    }

    CFLOAT len = sqrt(x*x + y*y + z*z);        

    return (vec3){
        .x = x/len,
        .y = y/len,
        .z = z/len
    };
}

vec3 util_vec3Reflect(vec3 v,vec3 n){
    CFLOAT two_dot_product = 2*vector3_dot_product(&v, &n);
    vector3_multiplyf(&n, two_dot_product);
    vector3_subtract(&v,&n);
    return v;
}

vec3 util_vec3Refract(vec3 uv, vec3 n, CFLOAT etai_over_etat){
    vec3 negate_uv = uv;
    vector3_negate(&negate_uv);
    CFLOAT cos_theta = CF_MIN(vector3_dot_product(&negate_uv, &n), 1.0);
    vec3 r_out_perp = n;
    
    vector3_multiplyf(&r_out_perp, cos_theta);
    vector3_add(&r_out_perp, &uv);
    vector3_multiplyf(&r_out_perp, etai_over_etat);
    
    CFLOAT r_out_perp_length_squared = (r_out_perp.x*r_out_perp.x + r_out_perp.y*r_out_perp.y + r_out_perp.z*r_out_perp.z);

    r_out_perp_length_squared = 1.0 - r_out_perp_length_squared;
    r_out_perp_length_squared = fabs(r_out_perp_length_squared);
    r_out_perp_length_squared = - sqrt(r_out_perp_length_squared);

    vec3 r_out_parallel = n;
    vector3_multiplyf(&r_out_parallel, r_out_perp_length_squared);

    vector3_add(&r_out_perp, &r_out_parallel);

    return r_out_perp;
}

#define float_zero(a) nearly_zero(a, 128 * F_EPSILON, F_MIN)

bool util_isVec3Zero(vec3 v){
    return (float_zero(v.x)) && (float_zero(v.y)) && (float_zero(v.z));
}

#undef float_zero
