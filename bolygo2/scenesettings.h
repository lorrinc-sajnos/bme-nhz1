#ifndef SCENESETTINGS_INC
#define SCENESETTINGS_INC

#include "typedefs.h"
#include "planetdata.h"
#include "planetdata.h"

typedef struct SceneSettings {
	Vec3 cameraPos;
	Vec3 cameraDir;
	float cameraSize;

	Vec3 right;
	Vec3 up;

	Vec3 lightDir;

	PlanetData* planetData;
    float rotationQ;

	float sunsetStart;
	float sunsetSize;
	float shadowMaxVal;

	float time;

	int frameCount;
} SceneSettings;

SceneSettings defaultSceneSettings(PlanetData *planetData);

void setCameraPos(SceneSettings *sceneSettings, Pol3 pos);
void setCameraSize(Vec3 newPos);
#endif
