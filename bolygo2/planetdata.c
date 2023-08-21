#include "debugmalloc.h"
#include "memorymang.h"

#include "planetdata.h"
#include "vector.h"
#include "planetparam.h"

PlanetData emptyPlanetData(){
    PlanetData planetData;
    planetData.dataH=0;
    planetData.dataW=0;
    planetData.heightData=NULL;
    planetData.planetParam=NULL;
    planetData.sphereMap=NULL;
    planetData.terrainColors=NULL;
    return planetData;
}

void initPlanetData(PlanetData* planetData, int dataW, int dataH) {
	planetData->dataW = dataW;
	planetData->dataH = dataH;


	planetData->heightData = (ushort **)malloc2D(dataW,dataH,sizeof(ushort));
    planetData->terrainColors=NULL;
	planetData->sphereMap = (Vec3 **)malloc2D(dataW,dataH,sizeof(Vec3));

	generateSphereMap(planetData);
}

void generateSphereMap(PlanetData* planetData) {
	int width = planetData->dataW;
	int height = planetData->dataH;

	Vec3 **sphereMap = planetData->sphereMap;

	for(int y=0; y<height; y++) {
		for(int x=0; x<width; x++) {
			Pol3 pol = coordsToPol((float)x/width,(float)y/height,1);
			Vec3 pos = polarToPos(pol);
			sphereMap[y][x] = pos;
		}
	}
}


void freePlanetData(PlanetData* planetData) {
	ushort **pHeightData = planetData->heightData;
	Vec3 **pSphereMap = planetData->sphereMap;

	freeList(planetData->terrainColors);

	free2D(pHeightData,planetData->dataH);
	free2D(pSphereMap,planetData->dataH);
	//free(planetData->planetParam);
	//Vec3 **pNormalMap = planetData->normalMap;
	/*
	for(int i=0;i<planetData->dataH;i++){
	    free(pHeightData[i]);

	    free(pSphereMap[i]);
	    //free(pNormalMap[i]);
	}
	free(planetData->heightData);
	free(planetData->sphereMap);*/

	//free(planetData->normalMap);
}
