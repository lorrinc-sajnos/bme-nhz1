#include "generationsettings.h"

GenSettings defaultGenSettings(float *edgeWgh, float *terrScl, float *terrWgh){
	GenSettings genSettings =  {
		.seed=6,

		.continentCount=7,
		.minContinentNodes=2,

		.maxContinentNodes=3,
		.minNodeDist = 0.5,
		.maxNodeDist = 0.9,

		.edgeLayerCount=4,
		.edgeWgh=edgeWgh,

		.terrLayerCount=4,
		.terrScl=terrScl,
		.terrWgh=terrWgh,
		.genInProgress = true
	};
	return genSettings;
}

