#ifndef PLANETDATA_INC
#define PLANETDATA_INC

#include "typedefs.h"
#include "planetparam.h"
#include "colorlist.h"
#include "vector.h"

typedef struct PlanetData {

    int dataW;
    int dataH;

    ushort** heightData;

    PlanetParam* planetParam;

    ColorList* terrainColors;

    //Ideiglenes
    Vec3** sphereMap;

} PlanetData;

PlanetData emptyPlanetData();

void initPlanetData(PlanetData* planetData, int dataW, int dataH);

void generateSphereMap(PlanetData* planetData);

void freePlanetData(PlanetData* planetData);
#endif
