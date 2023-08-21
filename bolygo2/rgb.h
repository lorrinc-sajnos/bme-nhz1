#ifndef RGB_INC
#define RGB_INC
#include <stdbool.h>
#include "typedefs.h"
#include "vector.h"

typedef struct RGB {
    uint8 r;
    uint8 g;
    uint8 b;
 } RGB;

 const RGB BLACK;
 const RGB WHITE;
 const RGB RED;
 const RGB GREEN;
 const RGB BLUE;


 RGB blend_lin(RGB A, RGB B, double blend);

 RGB newRGB(int R, int G, int B);
 RGB newRGBf(float R, float G, float B);
 RGB newRGBi(int32 color);

 RGB fromNormalVector(Vec3 vec);
int rgbToInt(RGB color);
 bool equals(RGB A, RGB B);
 #endif
