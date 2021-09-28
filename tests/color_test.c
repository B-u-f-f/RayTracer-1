#include <stdlib.h>
#include <check.h>

#define HYPATIA_IMPLEMENTATION

#include "color.h"
#include "testutils.h"

START_TEST(check_uint8_create_color){

    RGBColorU8 res = coloru8_create(0, 0, 1);

    ck_assert_int_eq(res.x, 0);
    ck_assert_int_eq(res.y, 0);
    ck_assert_int_eq(res.z, 1);

}END_TEST

START_TEST(check_f_create_color){

    RGBColorU8 res = colorf_create(0.5, 0.3, 0.9);

    ck_float_equal(res.x, 0.5);
    ck_float_equal(res.y, 0.3);
    ck_float_equal(res.z, 0.9);

}END_TEST

START_TEST(convert_u8_f){

    RGBColorU8 in = {
        .x = 0,
        .y = 1.
        .z = 1
    };

    RGBColorF out = convertU8toF(in);

    ck_float_equal(out.x, 0.0);
    ck_float_equal(out.y, 1.0);
    ck_float_equal(out.z, 1.0);

}END_TEST

START_TEST(convert_f_u8){

    RGBColorF in = {
        .x = 0.5,
        .y = 0.3,
        .z = 0.2
    };

    RGBColorF out = convertU8toF(in);

    ck_assert_int_eq(out.x, 0);
    ck_assert_int_eq(out.y, 0);
    ck_assert_int_eq(out.z, 0);

}END_TEST

Suite* hr_suite(void){
    Suite *s;
    TCase *tc_core;

    s = suite_create("Camera");

    //Core test case
    tc_core = tcase_create("Core");

    tcase_add_test(tc_core, check_uint8_create_color);
    tcase_add_test(tc_core, check_f_create_color);
    tcase_add_test(tc_core, convert_u8_f);
    tcase_add_test(tc_core, convert_f_u8);
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