#define HYPATIA_IMPLEMENTATION
#include <stdlib.h>
#include <stdbool.h>
#include <tgmath.h>
#include <check.h>

#include "hypatiaINC.h"
#include "material.h"
#include "hitRecord.h"

START_TEST(check_material_1){
    LambertianMat t_lmat = {
        .albedo = {
            .x = 1.0,
            .y = 2.0,
            .z = 3.0
        }
    };

    Ray rayIn = {
        .origin = {
            .x = 0,
            .y = 0,
            .z = 0
        },

        .direction = {
            .x = -2.0,
            .y = -3.0,
            .z = -2.0
        }
    };

    HitRecord t_hr =  {
        .point = {
            .x = 4.0,
            .y = 3.0,
            .z = 2.0
        },
        .normal = {
            .x = -1.0,
            .y = 3.0,
            .z = 2.0
        },
        .distanceFromOrigin = 2.0,
        
        .frontFace = true,
        .valid = true,

        .hitObjMat = {
            .mat = &t_lmat,
            .matType = LAMBERTIAN
        }  
        
    };
    
    vec3 color =  {
        .x = 0,
        .y = 0,
        .z = 0
    };

    Ray out = {
        .origin = {
            .x = 0,
            .y = 0,
            .z = 0
        },
        .direction = {
            .x = 0,
            .y attenua= 0,
            .z = 0
        }
        
    };

    vec3 attenuation = {
        .x = 0.0,
        .y = 0.0,
        .z = 0.0
    };

    
    bool b = mat_scatter(rayIn,t_hr,attenuation,out);

    scattered_direction = {
        .x = -1.0,
        .y = 3.0,
        .z = 2.0
    };

    ck_assert_ldouble_le(scattered_direction.x,0.0);
    ck_assert_ldouble_ge(scattered_direction.x,-1.0);
    ck_assert_ldouble_le(scattered_direction.y,4.0);
    ck_assert_ldouble_ge(scattered_direction.y,3.0);
    ck_assert_ldouble_le(scattered_direction.z,3.0);
    ck_assert_ldouble_ge(scattered_direction.z,2.0);

    Ray temp = ray_create()

    

}
END_TEST

START_TEST (check_material_metal_false){

    MetalMat nmetalMat = {
        .albedo = {
            .x = 1.0,
            .y = 2.0,
            .z = 3.0
        }
    };

    Ray rayIn = {
        .origin = {
            .x = 0,
            .y = 0,
            .z = 0
        },

        .direction = {
            .x = -2.0,
            .y = -3.0,
            .z = -2.0
        }
    };

    HitRecord rec = {
        .point = {
            .x = 4.0,
            .y = 3.0, 
            .z = 2.0
        },
        
        .normal = {
            .x = -1.0,
            .y = 3.0,
            .z = -2.0
        },
        
        .distanceFromOrigin = 3.0,

        .frontFace = true,

        .valid = true,

        .hitObjMat = {
            .mat = &nmetalMat,
            .matType = METAL
        }
    };

    vec3 attenuation = {
        .x = 0,
        .y = 0,
        .z = 0
    };

    Ray out = {
        .origin = {
            .x = 0,
            .y = 0,
            .z = 0
        },

        .direction = {
            .x = 0,
            .y = 0,
            .z = 0
        }
    };

    Ray exp_out = {

        .origin = {
            .x = 4.0,
            .y = 3.0, 
            .z = 2.0 
        },

        .direction = {
            .x = -0.47058824,
            .y = 0.88235294,
            .z = 0.58823529
        }

    };

    vec3 exp_attenuation = {
        .x = 1.0,
        .y = 2.0,
        .z = 3.0
    }


    bool b = mat_scatter(&rayIn, &rec, &attenuation, &out);
    
    ck_assert_ld_vec3_eq(exp_attenuation, attenuation);


}END_TEST


Suite* util_suite(void)
{
    Suite *s;
    TCase *tc_core;

    s = suite_create("Material");

    //Core test case
    tc_core = tcase_create("Core");
    suite_add_tcase(s, tc_core);
    tcase_add_test(tc_core, check_material_1);
    tcase_add_test(tc_core, check_material_2);

    return s;
}

int main(void){
    srand(1);

    int number_failed;
    Suite* s;
    SRunner *sr;

    s = util_suite();
    sr = srunner_create(s);
    srunner_run_all(sr, CK_NORMAL);
    number_failed = srunner_ntests_failed(sr);
    srunner_free(sr);


    return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
