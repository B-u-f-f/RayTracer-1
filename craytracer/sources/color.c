#include "color.h"
#include "util.h"

RGBColorU8 coloru8_create(uint8_t x, uint8_t y, uint8_t z){
    uint8_t a = util_uint8Clamp(x, 0, 1);
    uint8_t b = util_uint8Clamp(y, 0, 1);
    uint8_t c = util_uint8Clamp(z, 0, 1);

    return (RGBColorU8){
        .r = a,
        .g = b,
        .b = c
    };
}


RGBColorF colorf_create(CFLOAT x, CFLOAT y, CFLOAT z){
    CFLOAT a = util_floatClamp(x, 0, 1.0);
    CFLOAT b = util_floatClamp(y, 0, 1.0);
    CFLOAT c = util_floatClamp(z, 0, 1,0);

    return (RGBColorF){
        .r = a,
        .g = b,
        .b = c
    };
}

RGBColorF colorf_add(RGBColorF x, RGBColorF y){
    
    return (RGBColorF){
        .r = x.r + y.r,
        .g = x.g + y.g,
        .b = x.b + y.b
    };
}

RGBColorF colorf_multiply(RGBColorF x, RGBColorF y){
    return (RGBColorF) {
        .r = x.r * y.r,
        .g = x.g * y.g,
        .b = x.b * y.b
    };
}


RGBColorF convertU8toF(RGBColorU8 in){
    CFLOAT a = (CFLOAT)in.x;
    CFLOAT b = (CFLOAT)in.y;
    CFLOAT c = (CFLOAT)in.z;

    return (RGBColorF){
        .r = a,
        .g = b,
        .b = c
    };
}


RGBColorU8 convertFtoU8(RGBColorF in){

    uint8_t a = (uint8_t)in.x;
    uint8_t b = (uint8_t)in.y;
    uint8_t c = (uint8_t)in.z;

    return (RGBColorU8){
        .r = a,
        .g = b,
        .b = c
    };
}