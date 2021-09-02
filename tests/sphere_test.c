#define HYPATIA_IMPLEMENTATION

#include <stdlib.h>
#include <stdio.h>
#include <check.h>
#include <float.h>

#include "hypatiaINC.h"
#include "sphere.h"
#include "testutils.h"


//discriminant < 0 so no intersection
START_TEST(check_sphere_1){
    vec3 center;
    vector3_setf3(&center, 2.0L, 4.0L, 7.0L);
    Sphere test_sphere = {
        .center = center,
        .radius = 5.0L
    };

    vec3 origin;
    vector3_setf3(&origin, 0.0L, -20.0L, 0.0L);
    vec3 direction;
    vector3_setf3(&direction, 72.1L, 33.03L, 0.0L);
    
    HitRecord t_hr = hit(test_sphere, origin, direction, 0.001L, FLT_MAX);
    
    ck_assert_int_eq(t_hr.valid, 0);
}
END_TEST

// ray originates from outside and hits the sphere
START_TEST(check_sphere_2){
    vec3 cen;
    vector3_setf3(&cen, -2.0L, -4.0L, -7.0L);
    Sphere t_sphere = {
        .center = cen,
        .radius = 3.0L
    };

    vec3 origin;
    vector3_setf3(&origin, 0.0L, 0.0L, 0.0L);
    vec3 direction;
    vector3_setf3(&direction, -2.0L, -4.0L, -7.0L);
    vector3_normalize(&direction);

    HitRecord t_hr = hit(t_sphere, origin, direction, 0.001L, FLT_MAX);

    vec3 exp_point;
    vector3_setf3(&exp_point,-1.27768L, -2.55537L, -4.471897L);
    vec3 exp_normal = direction;
    vector3_normalize(&exp_normal);
    vector3_negate(&exp_normal);

    ck_assert_int_eq(530662, (int)(t_hr.distanceFromOrigin * 100000));
    ck_assert_int_eq(1 ,t_hr.valid);
    ck_assert_ld_vec3_eq(exp_point, t_hr.point);
    ck_assert_ld_vec3_eq(exp_normal, t_hr.normal);
    ck_assert_int_eq(t_hr.frontFace,1);
}
END_TEST

// ray coming from outside - root < t_min
START_TEST(check_sphere_3){
    vec3 cen;
    vector3_setf3(&cen, -2.0L, -4.0L, -7.0L);
    Sphere t_sphere = {
        .center = cen,
        .radius = 3.0L
    };

    vec3 origin;
    vector3_setf3(&origin, 0.0L, 0.0L, 0.0L);
    vec3 direction;
    vector3_setf3(&direction, -2.0L, -4.0L, -7.0L);
    vector3_normalize(&direction);

    HitRecord t_hr = hit(t_sphere, origin, direction, 100.0L, FLT_MAX);
    ck_assert_int_eq(0 ,t_hr.valid);
   
}
END_TEST

// ray coming from outside - root > t_max 
START_TEST(check_sphere_4){
    vec3 cen;
    vector3_setf3(&cen, -2.0L, -4.0L, -7.0L);
    Sphere t_sphere = {
        .center = cen,
        .radius = 3.0L
    };

    vec3 origin;
    vector3_setf3(&origin, 0.0L, 0.0L, 0.0L);
    vec3 direction;
    vector3_setf3(&direction, -2.0L, -4.0L, -7.0L);
    vector3_normalize(&direction);

    HitRecord t_hr = hit(t_sphere, origin, direction, 0.001L, 4.0L);
    
    ck_assert_int_eq(0 ,t_hr.valid);
}
END_TEST

// ray originates from within the sphere
START_TEST(check_sphere_5){
    vec3 cen;
    vector3_setf3(&cen, -2.0L, -4.0L, -7.0L);
    Sphere t_sphere = {
        .center = cen,
        .radius = 3.0L
    };

    vec3 origin;
    vector3_setf3(&origin, -2L, -4.00L, -7.00L);
    vec3 direction;
    vector3_setf3(&direction, -1.0L, -2.0L, -2.0L);
    vector3_normalize(&direction);

    HitRecord t_hr = hit(t_sphere, origin, direction, 0.001L, FLT_MAX);
    
    vec3 exp_point;
    vector3_setf3(&exp_point,-4.35L, -4.29L, -8.77L);
    vec3 exp_normal = direction;
    vector3_normalize(&exp_normal);
    vector3_negate(&exp_normal);
    ck_assert_ldouble_eq(1.208L, t_hr.distanceFromOrigin);
    ck_assert_int_eq(1 ,t_hr.valid);
    ck_assert_ld_vec3_eq(exp_point, t_hr.point);
    ck_assert_ld_vec3_eq(exp_normal, t_hr.normal);
    ck_assert_int_eq(t_hr.frontFace, 0);
    
}
END_TEST

// ray originates inside the sphere and and root < t_min
START_TEST(check_sphere_6){
    vec3 cen;
    vector3_setf3(&cen, -2.0L, -4.0L, -7.0L);
    Sphere t_sphere = {
        .center = cen,
        .radius = 3.0L
    };

    vec3 origin;
    vector3_setf3(&origin, -3.95L, -3.48L, -7.96L);
    vec3 direction;
    vector3_setf3(&direction, -0.36L, -2.0L, -2.17L);
    vector3_normalize(&direction);

    HitRecord t_hr = hit(t_sphere, origin, direction, 10000.0L, FLT_MAX);

    ck_assert_int_eq(0 ,t_hr.valid);
    
}
END_TEST


// ray originates inside the sphere and and root > t_max
START_TEST(check_sphere_7){
    vec3 cen;
    vector3_setf3(&cen, -2.0L, -4.0L, -7.0L);
    Sphere t_sphere = {
        .center = cen,
        .radius = 3.0L
    };

    vec3 origin;
    vector3_setf3(&origin, -3.95L, -3.48L, -7.96L);
    vec3 direction;
    vector3_setf3(&direction, -0.36L, -2.0L, -2.17L);

    HitRecord t_hr = hit(t_sphere, origin, direction, 0.001L, 1.0L);

    ck_assert_int_eq(0 ,t_hr.valid);

    
}
END_TEST


Suite* sphere_suite(void){
    Suite *s;
    TCase *tc_core;

    s = suite_create("sphere");

    //Core test case
    tc_core = tcase_create("Core");

    tcase_add_test(tc_core, check_sphere_1);
    tcase_add_test(tc_core, check_sphere_2);
    tcase_add_test(tc_core, check_sphere_3);
    tcase_add_test(tc_core, check_sphere_4);
    tcase_add_test(tc_core, check_sphere_5);
    tcase_add_test(tc_core, check_sphere_6);
    tcase_add_test(tc_core, check_sphere_7);
    suite_add_tcase(s, tc_core);

    return s;
}

int main(void){

    int number_failed;
    Suite* s;
    SRunner *sr;

    s = sphere_suite();
    sr = srunner_create(s);
    srunner_run_all(sr, CK_NORMAL);
    number_failed = srunner_ntests_failed(sr);
    srunner_free(sr);


    return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}