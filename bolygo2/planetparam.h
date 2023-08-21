#ifndef PLANETPARAM_INC
#define PLANETPARAM_INC
#include "rgb.h"

//Ennek a struktúrának az a lényege, hogy nem tartalmaz semmilyen pointert, így egyszerre lehet beolvasni õket
typedef struct PlanetParam{
    float oceanLevel;
    float iceCapRad;

    //Színek
    RGB oceanBotColor;
    RGB oceanTopColor;

    RGB cityColor;

    RGB sunsetColor;
    float sunsetIntensity;
    RGB atmosphereColor;
    RGB cloudColor;

    RGB polarIceColor;
    RGB polarCapMin;
    RGB polarCapMax;

    int32 visualSeed;
} PlanetParam;

void init_PlanetParam(PlanetParam *planetParam);
#endif // COLORDATA_INC
