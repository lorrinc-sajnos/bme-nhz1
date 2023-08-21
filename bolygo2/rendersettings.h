#ifndef RENDERSET_INC
#define RENDERSET_INC

#include <SDL.h>
#include <SDL2_gfxPrimitives.h>
#include <stdbool.h>

#include "vector.h"

#define RENDER_MODE_HGTMP 100

typedef struct RenderSettings {
    int renderMode;
	bool frameInProgress;
	int renderW;
	int renderH;
	Uint8 *pixels;
	SDL_Renderer *renderer;
} RenderSettings;
#endif
