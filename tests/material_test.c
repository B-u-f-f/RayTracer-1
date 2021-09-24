#define HYPATIA_IMPLEMENTATION
#include <stdlib.h>
#include <stdbool.h>
#include <tgmath.h>
#include <check.h>

#include "hypatiaINC.h"
#include "material.h"
#include "hitRecord.h"

START_TEST(check_material_1){
/*
    LambertianMat t_lmat = {
        .albedo = {
            .x = 1.0,
            .y = 2.0,
            .z = 3.0
        }
    };

    Ray t_rayIn = {
        .origin = {
            .x = 0.0,
            .y = 0.0,
            .z = 0.0
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
            .mat = NULL,
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
            .y = 0,
            .z = 0
        }
        
    };

*/

}
END_TEST

START_TEST (check_material_2){
/*
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
            .mat = NULL,

            .matType = METAL
        }
    };

    vec3 color = {
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

    bool ans = mat_metalScatter(&nmetalMat, &rayIn, &rec, &color, &out);
*/
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
