#include <assert.h>
#include <tgmath.h>

#include "material.h"
#include "util.h"
#include "ray.h"
#include "types.h"


static bool mat_metalScatter(const MetalMat * restrict nmetalMat, 
                             const Ray * restrict rayIn, 
                             const HitRecord * restrict rec, 
                             RGBColorF * restrict attenuation, Ray * restrict out){
    
    vec3 direction = rayIn->direction;
        
    vec3 reflected = util_vec3Reflect(direction, rec->normal);

    vec3 fuzz_rndm = util_randomUnitSphere();

    CFLOAT fuzz = nmetalMat->fuzz;

    fuzz = util_floatClamp(fuzz, 0, 1.0);

    vector3_multiplyf(&fuzz_rndm, fuzz);

    vector3_add(&fuzz_rndm, &reflected);

    *out = ray_create(rec->point, fuzz_rndm);

    *attenuation = nmetalMat->albedo;

    CFLOAT dot_prod = vector3_dot_product(&out->direction, &rec->normal);

    return (dot_prod > 0);
}

static bool mat_lambScatter(const LambertianMat * restrict nlambMat, 
                            const HitRecord * restrict rec, 
                            RGBColorF * restrict attenuation, Ray * restrict out){
    
    vec3 scatter_direction = rec->normal;
    vec3 rndm = util_randomUnitVector();

    vector3_add(&scatter_direction, &rndm);

    if(util_isVec3Zero(scatter_direction)){
        scatter_direction = rec->normal;
    }

    *out = ray_create(rec->point,scatter_direction);

    *attenuation = nlambMat->albedo;

    return true;
}

static bool mat_dielectricScatter(const DielectricMat * restrict ndielectricMat, const Ray * restrict rayIn, const HitRecord * restrict rec, RGBColorF * restrict attenuation, Ray * restrict out ){
     
    *attenuation = (RGBColorF){
        .r = 1.0,
        .g = 1.0,
        .b = 1.0
    };

    CFLOAT refraction_ratio = rec->frontFace ? (1.0/ndielectricMat->ir) : ndielectricMat->ir;

    // vec3 negate_rayIn_direction = vector3_negate(&rayIn->direction);
    // CFLOAT cos_theta = CF_MIN(vector3_dot_product(&negate_rayIn_direction, &rec->normal), 1.0);
    // CFLOAT sin_theta = sqrt(1 - cos_theta*cos_theta);

    // bool cannot_refract = (refraction_ratio * sin_theta) > 1.0;
    // vec3 direction;

    // if(cannot_refract){
    //     direction = util_vec3Reflect(rayIn->direction, rec->normal);
    // }else{
    //     direction = util_vec3Refract(rayIn->direction, rec->normal, refraction_ratio);
    // }

    vec3 refracted = util_vec3Refract(rayIn->direction, rec->normal, refraction_ratio);


    Ray r = {
        .origin = rec->point,
        .direction = refracted
    };

    *out = r;

    return true;
}


bool mat_scatter (const Ray * restrict rayIn, const HitRecord * restrict rec, RGBColorF * restrict attenuation, Ray * restrict out){
    const Material * restrict m = rec->hitObjMat;

    if(m->matType == METAL){
        return mat_metalScatter((MetalMat * ) m->matMetal, rayIn, rec, attenuation, out);
    }else if(m->matType == DIELECTRIC){
        return mat_dielectricScatter((DielectricMat * ) m->matDielectric, rayIn, rec, attenuation, out);
    }else if (m->matType == LAMBERTIAN){
        return mat_lambScatter((LambertianMat * ) m->matLamb, rec, attenuation, out);
    }

    assert(0);
}
