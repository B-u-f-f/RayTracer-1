#include <stdlib.h>
#include <check.h>
#include "../include/util.h"

START_TEST(check_util_h_1){
    ck_assert_float_eq(clamp(8.9, 6.0, 8.0), 8.0);
}
END_TEST

START_TEST(check_util_h_2){
    ck_assert_float_eq(clamp(5.9, 6.0, 8.0), 6.0);
}
END_TEST

START_TEST(check_util_h_3){
    ck_assert_float_eq(clamp(7.0, 6.0, 8.0),7.0);
}
END_TEST

Suite * clamp_suite(void)
{
    Suite *s;
    TCase *tc_core;

    s = suite_create("Clamp");

    /* Core test case */
    tc_core = tcase_create("Core");

    tcase_add_test(tc_core, check_util_h_1);
    tcase_add_test(tc_core, check_util_h_2);
    tcase_add_test(tc_core, check_util_h_3);
    suite_add_tcase(s, tc_core);

    return s;
}

int main(void){

    int number_failed;
    Suite* s;
    SRunner *sr;

    s = clamp_suite();
    sr = srunner_create(s);
    srunner_run_all(sr, CK_NORMAL);
    number_failed = srunner_ntests_failed(sr);
    srunner_free(sr);


    return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}