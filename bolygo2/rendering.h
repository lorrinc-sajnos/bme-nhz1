#ifndef RENDERING_INC
#define RENDERING_INC
#include <stdbool.h>

#include "rgb.h"
#include "vector.h"

#include "rendersettings.h"
#include "scenesettings.h"



ushort getHeightValue( const PlanetData *planetData, float x, float y);
float getShadowQ(float angle, const  SceneSettings *sceneSettings);
RGB selectElevationColor( const ColorList* terrainColors, float elevation);
RGB getSpaceColor( const PlanetParam *planetParam, float radSqd);


int renderFrame(RenderSettings *renderSet,  const SceneSettings *sceneSet);


RGB getColor(int x, int y, const  RenderSettings *renderSet, const  SceneSettings *sceneSet);
RGB getSurfaceColor(Vec3 hitPos, const  SceneSettings *sceneSet);
RGB getTerrainColor(float data, const Vec3 hitPos, const PlanetData *planetData, bool iceCap);

bool isIceCap(const PlanetData *planetData, Vec3 pos);
#endif
