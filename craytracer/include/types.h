#ifndef TYPES_H
#define TYPES_H

#include <float.h>

#ifdef C_RAY_SINGLE_PRECISION 
    typedef float CFLOAT;
    #define F_EPSILON FLT_EPSILON
    #define F_MAX FLT_MAX
    #define F_MIN FLT_MIN
#else
    typedef double CFLOAT;
    #define F_EPSILON DBL_EPSILON
    #define F_MAX DBL_MAX
    #define F_MIN DBL_MIN
#endif

#endif

