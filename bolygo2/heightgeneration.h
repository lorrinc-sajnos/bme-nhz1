#ifndef HEIGHTGENERATION_INC
#define HEIGHTGENERATION_INC
#include "typedefs.h"
#include "vector.h"
#include "planetdata.h"
#include "generationsettings.h"

void generateHeightData(PlanetData* planetData, GenSettings* settings);

ushort* getHeightmapCell(ushort *heightmap, int x, int y, int dataHeight);

void freeGenSettings(GenSettings *settings);
#endif
