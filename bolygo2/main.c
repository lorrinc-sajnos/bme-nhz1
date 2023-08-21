#include <SDL.h>
#include <SDL2_gfxPrimitives.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
//---
#include "debugmalloc.h"
#include "memorycheck.h"

#include "rendering.h"

#include "planetdata.h"
#include "dataio.h"

#include "commands.h"
#include "commandlist.h"
#include "generationsettings.h"
#include "heightgeneration.h"
#include "scenesettings.h"
#include "rendersettings.h"
#include "planetparam.h"
#include "misc.h"

#define FPS 10.0

/* ablak letrehozasa */
void sdl_init(char const *felirat, int szeles, int magas, SDL_Window **pwindow, SDL_Renderer **prenderer) {
	if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
		SDL_Log("Nem indithato az SDL: %s", SDL_GetError());
		exit(1);
	}
	SDL_Window *window = SDL_CreateWindow(felirat, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, szeles, magas, 0);
	if (window == NULL) {
		SDL_Log("Nem hozhato letre az ablak: %s", SDL_GetError());
		exit(1);
	}
	SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_SOFTWARE);
	if (renderer == NULL) {
		SDL_Log("Nem hozhato letre a megjelenito: %s", SDL_GetError());
		exit(1);
	}
	SDL_RenderClear(renderer);

	*pwindow = window;
	*prenderer = renderer;
}

//Forrás InfoC
Uint32 timer(Uint32 ms, void *param) {
	SDL_Event ev;
	ev.type = SDL_USEREVENT;
	ev.user.data1=param;
	SDL_PushEvent(&ev);
	return ms;
}

void restartRender(long *finishedTick, long *startTick, int *frameCount) {
	*finishedTick = SDL_GetTicks();
	*startTick = *finishedTick;
	SDL_FlushEvents(SDL_USEREVENT, SDL_USEREVENT);
	*frameCount=0;
}

int main(int argc, char *argv[]) {

	SDL_Event quitEvent;
	quitEvent.type=SDL_QUIT;
	CommandCache commands;
	initCommands(&commands,&quitEvent);

	//Memória ellenőrzése
	if(memoryCheck(100)==-1) {
		printf("NINCS ELEG MEMORIA");
		return -1;
	}

	/* ablak letrehozasa */
	SDL_Window *window;
	SDL_Renderer *renderer;
	sdl_init("Bolygo-gen", 640, 480, &window, &renderer);

	debugmalloc_max_block_size(100000000);
	//debugmalloc_log_file("debugmalloc_log.txt");

	SDL_Surface *surface = SDL_GetWindowSurface(window);


	//Bolygó adatainak inicializálása
	PlanetData planetData = emptyPlanetData();
	readPlanetData("default2.plnt",&planetData);

	PlanetParam planetParam;
	init_PlanetParam(&planetParam);
	planetData.planetParam = &planetParam;

	readPlanetParam(commands.params,"defaultparams.txt",&planetData);//*/




	float edgeWgh[5] = {0.77,0.44,0.22,0.10,0};

	float terrScl[5] = {0.5,0.2,0.1,0.05,0};
	float terrWgh[5] = {0.60,0.20,0.13,0.07,0};

	GenSettings genSettings = defaultGenSettings(edgeWgh,terrScl,terrWgh);

	SceneSettings sceneSettings = defaultSceneSettings(&planetData);

	RenderSettings renderSettings = {
		.renderMode = -1,
		.frameInProgress = false,
		.renderW = 640,
		.renderH = 480,
		.renderer = renderer,
		.pixels = surface->pixels
	};





	int frameCount=0;
	bool running = true;

	long finishedTick = 0;

	long ticksPerFrame = (int)(1000/FPS);
	SDL_TimerID timerId = SDL_AddTimer(ticksPerFrame, timer, &finishedTick);

	//Export test
	/*
		exportHeightmap("test_hgt.bmp",&planetData);
		exportTexture("test_text.bmp",&planetData);
		sceneSettings.time = .7;
		exportSattelite("test_sat.bmp",&sceneSettings);
		exportNormalmap("test_nrm.bmp",&planetData, true);//
		exportShadedMap("test_shd.bmp",&planetData,0.6,110*ANG2RAD,true);//
	*/


	//savePlanetParam(commands.params,"test_planet_param.txt",&planetData);
	//printf("%d\n",result);


	bool canRenderFrame = true;
	bool mousePressed = false;
	char line[128+1] = "";
	long prevFrameTick=0;
	long startTick=SDL_GetTicks();
	while (running) {
		SDL_Event event;
		SDL_WaitEvent(&event);

		switch (event.type) {
		case SDL_USEREVENT:

			if(canRenderFrame && finishedTick<=startTick+frameCount*ticksPerFrame) {
				//if(canRenderFrame){
				renderFrame(&renderSettings,&sceneSettings);
				finishedTick = SDL_GetTicks();

				//SDL_PushEvent()
			}

			frameCount++;
			if(canRenderFrame) {
				sceneSettings.time+=1.0/FPS;
			}
			break;

		/*
		case SDL_MOUSEBUTTONDOWN:
		    if(event.button.state==SDL_PRESSED ){
		        printf("Lenyomva\n");
		        SDL_GetGlobalMouseState(&orgX,&orgY);
		        mousePressed=true;
		    }
		    break;
		case SDL_MOUSEBUTTONUP:
		    if(event.button.state==SDL_RELEASED ) {
		        printf("Felengedve!\n");
		        mousePressed = false;
		    }

		    break;*/
		case SDL_QUIT:
			running=false;
			break;

		case SDL_KEYDOWN :
			if(event.key.keysym.scancode==SDL_SCANCODE_SPACE) {
				canRenderFrame=canRenderFrame?false:true;
				break;
			}
			if(event.key.keysym.scancode==SDL_SCANCODE_C) {
				gets(line);
				int result = executeCommand(&commands,line,&sceneSettings,&genSettings);

				if(result != CMD_SUCC)
					printErrorCode(result);
                restartRender(&finishedTick,&startTick,&frameCount);
			}
			if(event.key.keysym.scancode==SDL_SCANCODE_E) {
				bool takingCommands =true;
				while(takingCommands) {
					gets(line);
					if(strcmp(line,"endcmd")==0) {
						takingCommands=false;
					} else {
						int result = executeCommand(&commands,line,&sceneSettings,&genSettings);

						if(result != CMD_SUCC)
							printErrorCode(result);
					}
                restartRender(&finishedTick,&startTick,&frameCount);
				}

			}
			break;
		}



		SDL_RenderPresent(renderer);
	}
	/* ablak bezarasa */
	SDL_RemoveTimer(timerId);
	SDL_Quit();

	//További memória felszabadítása

	freePlanetData(&planetData);


	return 0;
}
