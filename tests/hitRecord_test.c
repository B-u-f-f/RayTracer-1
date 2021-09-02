#include <stdlib.h>

#define HYPATIA_IMPLEMENTATION

#include <check.h>
#include <stdbool.h>

#include "hypatiaINC.h"
#include "hitRecord.h"
#include "types.h"
#include "testutils.h"


START_TEST(check_hr_1){
    vec3 point;
    vector3_setf3(&point,-1.0L,-2.0L,-3.0L);
    vec3 normal;
    vector3_setf3(&normal,3.0L,1.0L,-7.0L);
    vec3 direction;
    vector3_setf3(&direction,2.0L,3.0L,10.0L);
    HitRecord temp_hr_1 = hr_setRecord(4.0L,point,normal,direction);
    ck_assert_ld_vec3_eq(point, temp_hr_1.point);
    ck_assert_ld_vec3_eq(normal, temp_hr_1.normal);
    ck_assert_int_eq(1, temp_hr_1.frontFace);
    ck_assert_ldouble_eq(4.0L, temp_hr_1.distanceFromOrigin);
    ck_assert_int_eq(1, temp_hr_1.valid);
    
}
END_TEST

START_TEST(check_hr_2){
    vec3 p;
    vector3_setf3(&p, -3.0L, -2.0L, -4.0L);
    vec3 n;
    vector3_setf3(&n, -4.0L, -5.0L, -9.0L);
    vec3 d;
    vector3_setf3(&d, -2.0L, -7.0L, -4.0L);
    HitRecord t_hr = hr_setRecord(7.0L, p, n, d);

    vec3 ne_n;
    vector3_setf3(&ne_n, 4.0L, 5.0L, 9.0L);

    ck_assert_ld_vec3_eq(p, t_hr.point);
    ck_assert_ld_vec3_eq(ne_n, t_hr.normal);
    ck_assert_ldouble_eq(7.0L, t_hr.distanceFromOrigin);
    ck_assert_int_eq(0, t_hr.frontFace);
    ck_assert_int_eq(1, t_hr.valid);
}
END_TEST

Suite* hr_suite(void){
    Suite *s;
    TCase *tc_core;

    s = suite_create("hitRecord");

    //Core test case
    tc_core = tcase_create("Core");

    tcase_add_test(tc_core, check_hr_1);
    tcase_add_test(tc_core, check_hr_2);
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