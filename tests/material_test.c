#define HYPATIA_IMPLEMENTATION
#include <stdlib.h>
#include <stdbool.h>
#include <tgmath.h>
#include <check.h>

#include "hypatiaINC.h"
#include "material.h"
#include "hitRecord.h"
#include "ray.h"
#include "testutils.h"

START_TEST(check_material_lambertian){
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

    Material ma = {
        .mat = &t_lmat,
        .matType = LAMBERTIAN,
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
        .hitObjMat = &ma
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

    vec3 attenuation = {
        .x = 0.0,
        .y = 0.0,
        .z = 0.0
    };

    //x = 0.37963565706642355, y = 0.87001058857811875, z = 0.31457645119382399
    
    bool b = mat_scatter(&rayIn, &t_hr, &attenuation, &out);

    // (-0.1362891883338393, 0.850211021907733, 0.5084943218659496)
    Ray expected_ray = {
        .origin = {
            .x = 4.0,
            .y = 3.0,
            .z = 2.0
        },
        .direction = {
        .x = -0.1362891883338393,
        .y = 0.850211021907733,
        .z = 0.5084943218659496
        }   
    };

    vec3 exp_attenuation = {
        .x = 1.0,
        .y = 2.0,
        .z = 3.0
    };

    ck_assert_ld_vec3_eq(expected_ray.origin, out.origin);
    ck_assert_ld_vec3_eq(expected_ray.direction, out.direction);
    ck_assert_ld_vec3_eq(exp_attenuation, t_lmat.albedo);
    ck_assert_int_eq(b, 1);
}
END_TEST

START_TEST (check_material_metal_true){

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
            .x = -0.48507125007266594,
            .y = -0.7276068751089989,
            .z = -0.48507125007266594
        }
    };
    // dot_prod = -0.198029508595334818600171328015662
    // 2*dot_prod = -0.396059017190669637200342656031324
    // normal * 2*dot_prod = {0.1616904166908886, -0.3233808333817773, 0.1616904166908886}
    // d - n = (-0.6467616667635545, -0.404226041727222, -0.6467616667635545)
    
    // 0.1980295085953345561950357848837
    
    Material ma = {
            .mat = &nmetalMat,
            .matType = METAL
    };

    HitRecord rec = {
        .point = {
            .x = 4.0,
            .y = 3.0, 
            .z = 2.0
        },
        
        .normal = {
            .x = -0.4082482904638631,
            .y = 0.8164965809277261,
            .z = -0.4082482904638631
        },
        
        .distanceFromOrigin = 3.0,

        .frontFace = true,
        .valid = true,
        .hitObjMat = &ma
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
            .x = -0.6467616667635545,
            .y = -0.404226041727222,
            .z = -0.6467616667635545
        }
    };

    vec3 exp_attenuation = {
        .x = 1.0,
        .y = 2.0,
        .z = 3.0
    };


    bool b = mat_scatter(&rayIn, &rec, &attenuation, &out);
    
    ck_assert_ld_vec3_eq(exp_attenuation, attenuation);
    ck_assert_ld_vec3_eq(exp_out.origin, out.origin);
    ck_assert_ld_vec3_eq(exp_out.direction, out.direction);
    ck_assert_int_eq(1, b);

}END_TEST

START_TEST (check_material_metal_false) {

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
            .x = -0.48507125007266594,
            .y = -0.7276068751089989,
            .z = -0.48507125007266594
        }
    };

    // 2*dot_prod = 1.980295085953348833406939841573836
    // n * 2_dot_prod = {-0.8084520834544435, -1.616904166908887, -0.8084520834544435}
    // (0.3233808333817776, 0.889297291799888, 0.3233808333817776)
    // normalized = (0.323380833381777, 0.88929729179989, 0.323380833381777)

    Material ma = {
        .mat = &nmetalMat,
        .matType = METAL
    };

    HitRecord rec = {
        .point = {
            .x = 4.0,
            .y = 3.0, 
            .z = 2.0
        },
        
        .normal = {
            .x = -0.4082482904638631,
            .y = -0.8164965809277261,
            .z = -0.4082482904638631
        },
        
        .distanceFromOrigin = 3.0,
        .frontFace = true,
        .valid = true,
        .hitObjMat = &ma
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
            .x = 1.0,
            .y = 4.0,
            .z = 3.0
        }
    };

    Ray exp_out = {

        .origin = {
            .x = 4.0,
            .y = 3.0, 
            .z = 2.0 
        },

        .direction = {
            .x = 0.323380833381777,
            .y = 0.88929729179989,
            .z = 0.323380833381777
        }
    };

    vec3 exp_attenuation = {
        .x = 1.0,
        .y = 2.0,
        .z = 3.0
    };

    bool b = mat_scatter(&rayIn, &rec, &attenuation, &out);
    
    ck_assert_ld_vec3_eq(exp_attenuation, attenuation);
    ck_assert_ld_vec3_eq(exp_out.origin, out.origin);
    ck_assert_ld_vec3_eq(exp_out.direction, out.direction);
    ck_assert_int_eq(0, b);


    
}END_TEST

Suite* util_suite(void)
{
    Suite *s;
    TCase *tc_core;

    s = suite_create("Material");

    //Core test case
    tc_core = tcase_create("Core");
    suite_add_tcase(s, tc_core);
    tcase_add_test(tc_core, check_material_lambertian);
    tcase_add_test(tc_core, check_material_metal_true);
    tcase_add_test(tc_core, check_material_metal_false);    

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
