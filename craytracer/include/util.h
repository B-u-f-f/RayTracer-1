#ifndef UTIL_H
#define UTIL_H

#include "hypatiaINC.h"

float clamp(float c, float lower, float upper);
int randomRange(int lower, int upper);
vec3 randomInUnitSphere();

float randomFloat(float lower, float upper);

#endif

