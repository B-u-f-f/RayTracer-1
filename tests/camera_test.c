#include <stdlib.h>

#define HYPATIA_IMPLEMENTATION

#include <check.h>
#include <stdbool.h>

#include "hypatiaINC.h"
#include "camera.h"
#include "types.h"
#include "testutils.h"


START_TEST(check_set_camera){
    // set camera
    Camera c = {
        .origin = {
            .x = 2.0,
            .y = 4.0,
            .z = 5.0
        },

        .aspectRatio = 11.0/2.0,
        .viewportHeight = 2.0,
        .focalLength = 1.0,
        .verticalFOV = 5.0
    };

    vec3 ip_origin = {
        .x = 0.0,
        .y = 0.0,
        .z = 0.0
    };

    CFLOAT ip_aspect_ratio = 16.0/9.0;
    CFLOAT ip_focalLength = 5.0;
    CFLOAT ip_vfov = 10.0;
    

    cam_setCamera(&c, ip_origin, ip_aspect_ratio, ip_focalLength, ip_vfov);

    CFLOAT exp_aspect_ratio = 16.0/9.0;
    CFLOAT exp_focalLength = 5.0;    
    CFLOAT exp_vfov = 0.174533174533174533; 
    

    CFLOAT exp_viewportHeight = 0.0874887087488708;
    CFLOAT exp_viewportWidth  = 0.1555354822202148;
    CFLOAT exp_verticalY   = 0.0874887087488708;
    CFLOAT exp_horizontalX = 0.1555354822202148;
    
    vec3 exp_lower_left_corner = {
        .x = -0.0777677411101073,
        .y = 0.0437443543744354, 
        .z = -5.0
    };
    
    vec3 exp_horizontal = {
        .x = exp_horizontalX, 
        .y = 0.0,
        .z = 0.0
    };
    
    vec3 exp_vertical = {
        .x = 0.0,
        .y = exp_verticalY,
        .z = 0.0
    };
    
    ck_float_equal(exp_viewportWidth, c.viewportWidth);
    ck_float_equal(exp_aspect_ratio, c.aspectRatio);
    ck_float_equal(exp_focalLength, c.focalLength);
    ck_float_equal(exp_vfov, c.verticalFOV);
    ck_float_equal(exp_viewportHeight, c.viewportHeight);
    ck_assert_ld_vec3_eq(c.horizontal, exp_horizontal);
    ck_assert_ld_vec3_eq(c.vertical, exp_vertical);
    ck_assert_ld_vec3_eq(c.lowerLeftCorner, exp_lower_left_corner);
}
END_TEST 

START_TEST(check_camera_getRay){
    // camera get ray
      Camera c = {
        .origin = {
            .x = 0.0,
            .y = 0.0,
            .z = 0.0
        },

        .u = {
            .x = 0.3,
            .y = 0.4,
            .z = 0.9
        },

        .v = {
            .x = 0.5,
            .y = 0.2,
            .z = 0.9
        },

        .vertical = {
            .x = 0.0,
            .y = 0.0874887087488708,
            .z = 0.0
        },
        
        .horizontal = {
            .x = 0.1555354822202147, 
            .y = 0.0,
            .z = 0.0
        },

        .lowerLeftCorner = {
            .x = -0.0777677411101073,
            .y = 0.0437443543744354,
            .z = -5.0
        },

        .lensRadius = 1.0
    };
    
    //{0.68037543430941905, -0.21123414636181392, 0}
    // u = 0.5, v = 0.6
    // offset = {0.098495557111918755, 0.229903344451404836, 0.422227159152844617}
    // origin + abv = {0.098495557111918755, 0.229903344451404836, 0.422227159152844617}
    // veri*v = {0, 0.05249322524932248, 0}
    // hori*u = (0.07776774111010735,0,0)
    // lowerleft + veri*v + hori*u = {0, 0.09623757962375788, -5.0}
    // abv - origin = {0, 0.09623757962375788, -5.0}
    // abv - offset = (-0.098495557111918755, -0.13366576482764696, -5.422227159152844617)

    vec3 exp_direction = {
        .x = -0.098495557111918755,
        .y = -0.13366576482764696,
        .z = -5.422227159152844617
    };

    vec3 exp_origin = {
        .x = 0.098495557111918755, 
        .y = 0.229903344451404836, 
        .z = 0.422227159152844617
    };

    Ray t_ray = cam_getRay(&c, 0.5, 0.6);

    ck_assert_ld_vec3_eq(exp_direction, t_ray.direction);
    ck_assert_ld_vec3_eq(exp_origin, t_ray.origin);
    
}END_TEST

START_TEST(check_set_look_at_camera){
    Camera c = {0};

    vec3 ip_lookfrom = {
        .x = 3.0,
        .y = 3.0, 
        .z = 2.0
    };

    vec3 ip_lookat = {
        .x = 0.0, 
        .y = 0.0, 
        .z = -1.0
    };

    vec3 ip_up = {
        .x = 0.0, 
        .y = 1.0, 
        .z = 0.0
    };

    CFLOAT ip_vfov = 10.0;
    CFLOAT ip_aspect_ratio = 16.0/9.0;
    CFLOAT ip_aperture = 2.0;
    CFLOAT ip_focusDist = 5.19615242271;
        
    cam_setLookAtCamera(&c,ip_lookfrom,ip_lookat,ip_up,ip_vfov,ip_aspect_ratio,ip_aperture,ip_focusDist);
    
    CFLOAT exp_aspect_ratio = 16.0/9.0;
    CFLOAT exp_verticalFOV = 0.174533174533174533; 
    CFLOAT exp_viewportHeight = 0.1749781749781749;
    CFLOAT exp_viewportWidth = 0.3110723110723109333;
    
    // w = normalize c-w = (0.5773502691896257, 0.5773502691896257, 0.5773502691896257)
    // cross(up, w) = (-0.5773502691896257, 0, 0.5773502691896257)
    // u = normalized = (-0.707106781186548, 0, 0.707106781186548)
    // v = cross(w, u) = (0.408248290463863, -0.816496580927726, 0.408248290463863)
    // c->horizontal = {-1.14295265285397, 0, 1.14295265285397}
    // c->vertical = {0.371184762260, -0.742369524521, 0.371184762260}
    // horizontal + vertical = (-0.771767890594, -0.742369524521, 1.514137415114)
    // lower_left = {-0.385883945297, -0.371184762260, -1.757068707557}
    // lens_radius = 1.0

    vec3 exp_w = {
        .x = 0.5773502691896257,
        .y = 0.5773502691896257,
        .z = 0.5773502691896257
    };

    vec3 exp_u = {
        .x = -0.707106781186548,
        .y = 0,
        .z = 0.707106781186548
    };

    vec3 exp_v = {
        .x = 0.408248290463863,
        .y = -0.816496580927726,
        .z = 0.408248290463863
    };

    vec3 vertical = {
        .x = 0.371184762260,
        .y = -0.742369524521,
        .z = 0.371184762260
    };

    vec3 horizontal = {
        .x = -1.14295265285397, .y = 0, .z = 1.14295265285397
    };

    CFLOAT exp_lens_radius = 1.0;

    vec3 exp_lower_left_corner = {
        .x = -0.385883945297,
        .y = -0.371184762260,
        .z = -1.757068707557
    };

    ck_assert_ld_vec3_eq(c.origin, ip_lookfrom);
    ck_assert_ld_vec3_eq(c.horizontal, horizontal);
    ck_assert_ld_vec3_eq(c.vertical, vertical);
    ck_assert_ld_vec3_eq(c.u, exp_u);
    ck_assert_ld_vec3_eq(c.v, exp_v);
    ck_assert_ld_vec3_eq(c.w, exp_w);
    ck_float_equal(exp_aspect_ratio, c.aspectRatio);
    ck_float_equal(exp_lens_radius, c.lensRadius);
    ck_float_equal(exp_viewportWidth, c.viewportWidth);
    ck_float_equal(exp_viewportHeight, c.viewportHeight);        
    ck_float_equal(exp_verticalFOV, c.verticalFOV);
    ck_assert_ld_vec3_eq(c.lowerLeftCorner, exp_lower_left_corner);
}END_TEST


Suite* hr_suite(void){
    Suite *s;
    TCase *tc_core;

    s = suite_create("Camera");

    //Core test case
    tc_core = tcase_create("Core");

    tcase_add_test(tc_core, check_set_camera);
    tcase_add_test(tc_core, check_camera_getRay);
    tcase_add_test(tc_core, check_set_look_at_camera);
    suite_add_tcase(s, tc_core);

    return s;
}

int main(void){
    srand(1);
    
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
