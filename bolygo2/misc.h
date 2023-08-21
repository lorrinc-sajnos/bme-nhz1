#ifndef MISC_INC
#define MISC_INC

#include "typedefs.h"

#define ANG2RAD 0.01745329251
#define RAD2ANG 57.2957795131

float linint(float x, float y, float t);
float easef(float t);

float ushortToPrc(ushort val);
ushort prcToUshort(float prc);

float sc01(float x);
float sc11(float x);
float scale(float x, float min, float max);


float scaleInf(float x);
float scaleInf2(float x, float ampl);
float clamp(float d, float min, float max);

float cubicInt(float x);
float bell(float x);

int posmod(int x, int mod);


int64 encStr(char *str);
void decStr(char *str, int64 code);

void strcut(char *origin, char *first, char *rest);
#endif
