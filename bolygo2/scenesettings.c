#include <math.h>

#include "scenesettings.h"
#include "vector.h"
#include "typedefs.h"
#include "planetdata.h"
#include "misc.h"

SceneSettings defaultSceneSettings(PlanetData *planetData){

	Vec3 cameraDir = normalize(newVec(-1, -1, -1));
	Vec3 cameraPos = mult(cameraDir,-10);

	float cameraSize = 2.0;

	Vec3 right = normalize(newVec(cameraDir.y,-cameraDir.x,0));
	Vec3 up = normalize(cross(right,cameraDir));

	float sunsetAngle = 10;

	SceneSettings sceneSettings = {
		.cameraPos=cameraPos,
		.cameraDir=cameraDir,

		.cameraSize=cameraSize,
		.right = right,
		.up = up,

		.time=0,

		.lightDir = newVec(0,1,0.3),
		.sunsetStart = (90-sunsetAngle/2)*ANG2RAD,
		.sunsetSize = sunsetAngle*ANG2RAD,
		.shadowMaxVal = 1.0,

		.planetData = planetData,
		.rotationQ = 0.2*M_PI,
		.frameCount=0
	};

	return sceneSettings;
}

void setCameraPos(SceneSettings *sceneSettings, Pol3 pos){
    Pol3 newPol = pos;
    newPol.b = clamp(newPol.b,-80*ANG2RAD,80*ANG2RAD);
    newPol.r=1;

    sceneSettings->cameraDir=polarToPos(newPol);
	sceneSettings->cameraPos = mult(sceneSettings->cameraDir,-10);
	sceneSettings->right = normalize(newVec(sceneSettings->cameraDir.y,-sceneSettings->cameraDir.x,0));
	sceneSettings->up = normalize(cross(sceneSettings->right,sceneSettings->cameraDir));
}

void setCameraDir(SceneSettings *sceneSettings,Vec3 newDir){

	sceneSettings->cameraDir = normalize(newDir);
	sceneSettings->cameraPos = mult(sceneSettings->cameraDir,-10);
}

void setLightDir(SceneSettings *sceneSettings,Vec3 newDir){
    sceneSettings->lightDir = normalize(newDir);
}
