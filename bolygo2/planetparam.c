#include "debugmalloc.h"
#include "planetparam.h"


void init_PlanetParam(PlanetParam *planetParam) {

	PlanetParam newPlanetParam = {
		.oceanLevel = 0,
		.iceCapRad = 0,

		//Színek
		.oceanBotColor = BLACK,
		.oceanTopColor = BLACK,
		.cityColor = BLACK,

		.sunsetColor = BLACK,
		.sunsetIntensity = 0,
		.atmosphereColor = BLACK,
		.cloudColor = BLACK,

		.polarIceColor = BLACK,
		.polarCapMin = BLACK,
		.polarCapMax = BLACK,

		.visualSeed = 0
	};
	*planetParam=newPlanetParam;
}
