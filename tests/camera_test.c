#include <stdlib.h>

#define HYPATIA_IMPLEMENTATION

#include <check.h>
#include <stdbool.h>

#include "hypatiaINC.h"
#include "camera.h"
#include "types.h"
#include "testutils.h"


START_TEST(check_camera_1){
    Camera c = {
        .origin = {
            .x = 0.0,
            .y = 0.0
        },

        .aspect_ratio = 16.0/9.0,
        .viewport_height = 2.0,
        .focal_length = 1.0
    };
    cam_setCamera(&c);

    CFLOAT exp_viewport_width = 3.5555555555555556;
    CFLOAT exp_vertical_y = 2.0;
    CFLOAT exp_horizontal_x = 3.5555555555555556;
    
    vec3 exp_lower_left_corner = {
        .x = -1.0,
        .y = 1.7777777777777778,
        .z = -1.0
    };
    
    vec3 exp_horizontal = {
        .x = exp_horizontal_x, 
        .y = 0.0, 
        .z = 0.0
    };
    
    vec3 exp_vertical = {
        .x = 0.0,
        .y = exp_vertical_y,
        .z = 0.0
    };
    
    ck_float_equal(exp_viewport_width, c.viewport_width);
    ck_assert_ld_vec3_eq(c.horizontal, exp_horizontal);

    ck_assert_ld_vec3_eq(c.vertical, exp_vertical);

    ck_assert_ld_vec3_eq(c.lower_left_corner, exp_lower_left_corner);

    Ray t_ray = cam_getRay(c,)

}
END_TEST 

Suite* hr_suite(void){
    Suite *s;
    TCase *tc_core;

    s = suite_create("Camera");

    //Core test case
    tc_core = tcase_create("Core");

    tcase_add_test(tc_core, check_camera_1);
    suite_add_tcase(s, tc_core);

    return s;
}

int main(void){

    int number_failed;
    Suite* s;
    SRunner *sr;

    s = hr_suite();
    sr = srunner_create(s);
    srunner_run_all(sr, CK_NORMAL);
    number_failed = srunner_ntests_failed(sr);
    srunner_free(sr);


    return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}