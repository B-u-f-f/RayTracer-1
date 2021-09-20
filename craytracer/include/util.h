#ifndef UTIL_H
#define UTIL_H

#include "hypatiaINC.h"
#include "types.h"

CFLOAT util_ldClamp(CFLOAT c, CFLOAT lower, CFLOAT upper);

// not in use
// int randomRange(int lower, int upper);


vec3 util_randomInUnitSphere();
CFLOAT util_randomLD(CFLOAT lower, CFLOAT upper);

#endif

