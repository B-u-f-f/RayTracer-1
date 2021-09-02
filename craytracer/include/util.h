#ifndef UTIL_H
#define UTIL_H

#include "hypatiaINC.h"
#include "types.h"

cray_ld util_ldClamp(cray_ld c, cray_ld lower, cray_ld upper);

// not in use
// int randomRange(int lower, int upper);


vec3 util_randomInUnitSphere();
cray_ld util_randomLD(cray_ld lower, cray_ld upper);

#endif

