#include <SDL.h>
#include <SDL2_gfxPrimitives.h>
#include <math.h>
#include <stdbool.h>

#include "rgb.h"
#include "typedefs.h"
#include "vector.h"
#include "perlin.h"
#include "hashfunc.h"
#include "misc.h"

#include "scenesettings.h"
#include "rendersettings.h"
#include "planetparam.h"
#include "rendering.h"

int renderFrame(RenderSettings *renderSet, const  SceneSettings *sceneSet) {
	renderSet->frameInProgress = true;

	int width = renderSet->renderW;
	int height = renderSet->renderH;

	Uint8 *pixelPointer =  renderSet->pixels;
	for(int y=0; y<height; y++) {
		for(int x=0; x<width; x++) {
			RGB color = getColor(x,y,renderSet,sceneSet);
			//color = newRGB(x%256,x%256,x%256);
			*pixelPointer++=color.b;
			*pixelPointer++=color.g;
			*pixelPointer++=color.r;
			*pixelPointer++=255;
		}
	}
	//SDL_RenderPresent(renderSet->renderer);
	renderSet->frameInProgress = false;
	return 1;
}

RGB getColor(int x, int y,  const RenderSettings *renderSet,const SceneSettings *sceneSet) {
	int width = renderSet->renderW;
	int height = renderSet->renderH;

	/*DEBUG
	ushort val = getHeightValue(sceneSet->planetData,1.0*x/width,1.0*y/height);
	float f = 1.0 - val/USHORT_MAX_F;
	return newRGBf(f,f,f);
	return newRGBi(hash_i((int32)val,0,0,123));//*/


	float unitPerPixel = sceneSet->cameraSize/height;

	float scX = ((float)x-width/2.0)*unitPerPixel;
	float scY = -((float)y-height/2.0)*unitPerPixel;

	float radSqd = scX*scX+scY*scY;

	if(radSqd>1.0) {
		//Ekkor biztos nem ütközik bele a bolygóba, ezért ûr színét kell meghatározni
		return getSpaceColor(sceneSet->planetData->planetParam, radSqd);
	}

	float d = sqrtf(1.0-scX*scX-scY*scY);
	Vec3 hitPos = sum3(
	                  mult(sceneSet->right,scX),
	                  mult(sceneSet->up,scY),
	                  mult(sceneSet->cameraDir,-d)
	              );
	return getSurfaceColor(hitPos, sceneSet);
}

RGB getSurfaceColor(const Vec3 hitPos,const SceneSettings *sceneSet) {
	PlanetData *planetData = sceneSet->planetData;
	PlanetParam* planetParam = planetData->planetParam;
	float shadowMax = sceneSet->shadowMaxVal;
	//------------------------------
	Pol3 polar = posToPolar(hitPos);

	float rotationDelta = sceneSet->time*sceneSet->rotationQ;//sceneSet->time/5.0*M_PI;
	polar.a+=rotationDelta;

	float x = modf((polar.a)*RC_2PI,NULL);
	float y = (polar.b+M_PI*0.5)*RC_PI;



	ushort data = getHeightValue(planetData,x,y);
	float dataF = data/USHORT_MAX_F;

	//return newRGBf(dataF,dataF,dataF);

	bool iceCap = isIceCap(planetData, hitPos);

	float angle = getAngle(hitPos,sceneSet->lightDir);
	float shadowQ = getShadowQ(angle, sceneSet);
	float shadowVal = easef(shadowQ)*shadowMax;
	//Esti oldal színe
	RGB nightColor = BLACK;
	if(shadowQ!=0) {
		nightColor = blend_lin(BLACK,planetParam->sunsetColor,bell(shadowQ)*planetParam->sunsetIntensity);

		//Város fényei
		if(dataF>=planetData->planetParam->oceanLevel && !iceCap) {
			float scl = 100;
			float noise = perlin(
			                  polar.a*scl,
			                  polar.b*scl,
			                  0,
			                  1
			              );
			nightColor = blend_lin(nightColor,planetParam->cityColor,fmaxf(noise*noise,0));
			if (shadowQ==1)
				return nightColor;
		}
	}



	RGB terrainColor = getTerrainColor(dataF,hitPos,planetData,iceCap);
	//Árnyékolás
	return blend_lin(terrainColor,nightColor,shadowVal);
}

RGB getTerrainColor(float data, const Vec3 hitPos, const PlanetData *planetData, bool iceCap) {
	PlanetParam* planetParam = planetData->planetParam;
	float oceanLevel= planetData->planetParam->oceanLevel;

    //bool iceCap = isIceCap(planetData,hitPos);

	if(data<oceanLevel) {
		if(iceCap)
			return planetParam->polarIceColor;

		float oceanDepth = scale(data,0,oceanLevel);
		return blend_lin(planetParam->oceanBotColor,planetParam->oceanTopColor,oceanDepth);
	}
	float elevation = scale(data,oceanLevel,1);
	if(iceCap)
        return blend_lin(planetParam->polarCapMin,planetParam->polarCapMax,easef(elevation));
	return selectElevationColor(planetData->terrainColors,elevation);
}


RGB getSpaceColor(const PlanetParam * planetParam, float radSqd) {
	return BLACK;
}

ushort getHeightValue( const PlanetData *planetData, float x, float y) {
	int indX = (int)(x*planetData->dataW);
	int indY = (int)(y*planetData->dataH);
	return planetData->heightData[indY][indX];
}

float getShadowQ(float angle,  const SceneSettings *sceneSettings) {
	float min = sceneSettings->sunsetStart;
	float size = sceneSettings->sunsetSize;

	if(angle<min)
		return 0;
	angle-=min;
	if(angle>size)
		return 1;
	return angle/size;

}

RGB selectElevationColor( const ColorList* terrainColors, float elevation) {
    if(terrainColors==NULL) return RED;
	ColorList *listP = terrainColors;
	while(listP->nextColor!=NULL) {
		float currMin = listP->height;
		float currMax = listP->nextColor->height;
		if(elevation<currMax) {
			float grad = scale(elevation,currMin,currMax);
			return blend_lin(listP->color,listP->nextColor->color,easef(grad));
		}
		listP=listP->nextColor;
	}

	return listP->color;
}

bool isIceCap(const PlanetData *planetData, Vec3 pos){
    float distanceSqd = pos.x*pos.x+pos.y*pos.y;
    float capRadius = planetData->planetParam->iceCapRad;

    if(distanceSqd>capRadius*capRadius)
        return false;

    return true;

}

