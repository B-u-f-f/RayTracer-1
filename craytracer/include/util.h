#ifndef UTIL_H
#define UTIL_H

#include "hypatiaINC.h"
#include "types.h"

extern CFLOAT util_floatClamp(CFLOAT c, CFLOAT lower, CFLOAT upper);

// not in use
// int randomRange(int lower, int upper);


extern vec3 util_randomUnitSphere();
extern CFLOAT util_randomFloat(CFLOAT lower, CFLOAT upper);

#endif

