#ifndef COLOR_H
#define COLOR_H

#include "hypatiaINC.h"
#include <stdint.h>

typedef struct rgbColorU8 {

    uint8_t r;

    uint8_t g;

    uint8_t b;

} RGBColorU8;

typedef struct rgbColorF {

    CFLOAT r;

    CFLOAT g;

    CFLOAT b;

} RGBColorF;

extern RGBColorU8 coloru8_create(uint8_t r, uint8_t g, uint8_t b);
extern RGBColorF colorf_create(CFLOAT r, CFLOAT g, CFLOAT b);
extern RGBColorF convertU8toF(RGBColorU8 in);
extern RGBColorU8 convertFtoU8(RGBColorF in);

#endif