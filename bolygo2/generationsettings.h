#ifndef GENSETTINGS_INC
#define GENSETTINGS_INC
#include <stdbool.h>

#include "typedefs.h"
#include "vector.h"
#include "planetdata.h"

typedef struct GenSettings {
    int32 seed;

    int continentCount;
    int minContinentNodes, maxContinentNodes;
    float minNodeDist,maxNodeDist;

    int edgeLayerCount;
    float *edgeWgh;

    int terrLayerCount;
    float *terrScl;
    float *terrWgh;


    bool genInProgress;
} GenSettings;

GenSettings defaultGenSettings(float *edgeWgh, float *terrScl, float *terrWgh);
#endif
