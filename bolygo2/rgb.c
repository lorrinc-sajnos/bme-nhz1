#include <SDL.h>
#include <SDL2_gfxPrimitives.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

#include <windows.h>
#include <time.h>

#include "rgb.h"
#include "typedefs.h"
#include "vector.h"
#include "misc.h"

 const RGB BLACK = {.r=0,.g=0,.g=0};
 const RGB WHITE = {.r=255,.g=255,.b=255};

 const RGB RED = {.r=255,.g=0,.b=0};
 const RGB GREEN = {.r=0,.g=255,.b=0};
 const RGB BLUE = {.r=0,.g=0,.b=255};

RGB newRGB(int R, int G, int B){
    RGB C = {.r=R%256,.g=G%256,.b=B%256};
    return C;
}

RGB newRGBi(int32 color){
    int b = color%256;

    color>>=8;
    int g = color%256;

    color>>=8;
    int r = color%256;
    return newRGB(r,g,b);
}

RGB newRGBf(float R, float G, float B){
    return newRGB((int)(R*255),(int)(G*255),(int)(B*255));
}

 RGB blend_lin(RGB A, RGB B, double blend){
    double t = blend;
    int dR = B.r-A.r;
    int dG = B.g-A.g;
    int dB = B.b-A.b;

    A.r+=(int)(dR*t);
    A.g+=(int)(dG*t);
    A.b+=(int)(dB*t);
    return A;
 }

 RGB fromNormalVector(Vec3 vec){
    //finalColor = vec4(getSrfNormal(currPos)*0.5+0.5,1.0);
    return newRGBf(sc01(vec.x),sc01(vec.y),sc01(vec.z));
 }

 int rgbToInt(RGB color){
     return  color.r*0x010000+color.g*0x000100+color.b*0x000001;
 }

 bool equals(RGB A, RGB B) {
    return A.r==B.r && A.g==B.g && A.b==B.b;
 }
