#include <assert.h>

#include "material.h"
#include "util.h"
#include "ray.h"


static bool mat_metalScatter(const MetalMat * restrict nmetalMat, const Ray * restrict rayIn, const HitRecord * restrict rec, const vec3 * restrict color, Ray * restrict out){
    
    vec3 direction = rayIn->direction;
    vector3_normalize(&direction);
        
    vec3 reflected = util_vec3Reflect(direction, rec->normal);

    *out = ray_create(rec->point, reflected);

    *color = nmetalMat->albedo;

    int dot_prod = vector3_dot_product(out->direction, rec->normal);

    return (dot_prod > 0);
}

static bool mat_lambScatter(const LambertianMat * restrict nlambMat, const Ray * restrict rayIn, const HitRecord * restrict rec, const vec3 * restrict color, Ray * restrict out){
    
    vec3 scatter_direction = rec->normal;
    vec3 rndm = util_randomUnitVector();

    vector3_add(&scatter_direction, &rndm);

    if(util_isVec3Zero(scatter_direction)){
        scatter_direction = rec->normal;
    }

    *out = ray_create(rec->point,scatter_direction);

    *color = nlambMat->albedo;

    return true;
}


bool mat_scatter (const Material * restrict m, const Ray * restrict rayIn, const HitRecord * restrict rec, const vec3 * restrict color, Ray * restrict out){
    
    if(mat->matType == METAL){
        return mat_metalScatter((MetalMat * ) m->mat, rayIn, rec, color, out);
    }else if (mat->matType == LAMBERTIAN){
        return mat_lambScatter((LambertianMat * ) m->mat, rayIn, rec, color, out);
    }

    assert();
}