#ifndef TESTUTILS_H
#define TESTUTILS_H

#include <check.h>

#define ck_assert_ld_vec3_eq(v1, v2) { \
    ck_assert_ldouble_eq((v1).x, (v2).x); \
    ck_assert_ldouble_eq((v1).y, (v2).y); \
    ck_assert_ldouble_eq((v1).z, (v2).z); \
} \


#endif 