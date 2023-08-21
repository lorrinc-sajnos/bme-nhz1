#ifndef PERLIN_INC
#define PERLIN_INC
#include "typedefs.h"

float perlin(float x, float y, float z, int32 seed);
float layeredPerlin(float x, float y, float z, int32 seed, float layerWeights[], float layerScales[], int layerCount);

#endif

