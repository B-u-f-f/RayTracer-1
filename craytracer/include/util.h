#ifndef UTIL_H
#define UTIL_H

#include "hypatiaINC.h"
#include "types.h"

#include <stdbool.h>

extern CFLOAT util_floatClamp(CFLOAT c, CFLOAT lower, CFLOAT upper);

// not in use
// int randomRange(int lower, int upper);


extern vec3 util_randomUnitSphere();

extern vec3 util_randomUnitVector();
extern CFLOAT util_randomFloat(CFLOAT lower, CFLOAT upper);
extern vec3 util_vec3Reflect(vec3 v,vec3 n);

extern bool util_isVec3Zero(vec3 v);

#endif

