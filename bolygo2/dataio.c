#include <stdio.h>
#include <SDL.h>
#include <SDL2_gfxPrimitives.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>

#include "commands.h"
#include "commandlist.h"
#include "planetdata.h"
#include "scenesettings.h"
#include "rendering.h"
#include "planetparam.h"
#include "misc.h"
#include "typedefs.h"
#include "memorymang.h"
#include "colorlist.h"
#include "dataio.h"



//#define 0 VERSION

static const char HEADER[] = "PLNTFL";

void DEBUG_printEof(FILE * file) {
	if(feof(file)) printf("FAJL VEGE!");
}

size_t writeUshort(ushort *value, FILE *file) {
	return fwrite(value,sizeof(ushort),1,file);
}
size_t writePrc(float value,FILE *file) {
	ushort conv = prcToUshort(value);

	return writeUshort(&conv, file);
}

size_t readUshort(ushort *dest, FILE *file) {
	return fread(dest,sizeof(ushort),1,file);
}
size_t readPrc(float *dest, FILE *file) {
	ushort conv;
	size_t result = readUshort(&conv,file);
	*dest = ushortToPrc(conv);
	return result;
}

long plntFileSize(ushort dataWidth, ushort dataHeight, ushort colorSize) {
	const int S_USHORT = sizeof(ushort);

	long headerSize = strlen(HEADER);
	long versionSize = sizeof(uint8);

	long dataSizeParamSize = 3*S_USHORT;

	long heightDataSize = S_USHORT*dataWidth*dataHeight;
	long colorListSize = colorSize*(sizeof(RGB)+S_USHORT);
	long paramSize = sizeof(PlanetParam);

	long miscSize = 0;

	long totalSize = headerSize+versionSize+dataSizeParamSize+heightDataSize+colorListSize+paramSize+miscSize;

	return totalSize;
}

bool eofCheck(FILE *file) {
	return feof(file) != 0;
}

void modIndexes(PlanetData *planetData, int *x, int *y) {
	int width = planetData->dataW;
	int height = planetData->dataH;
	if(0>*y || *y>=height) {
		*x+=width/2;
		*y=*y>0?(*y<height?*y:2*height-*y-2):-*y;
	}
	*x = posmod(*x,width);
}

Vec3** generateNormalMap(PlanetData *planetData, bool flatOcean) {
	int width = planetData->dataW;
	int height = planetData->dataH;

	float difQ = 25;

	float oceanLevel = planetData->planetParam->oceanLevel;

	Vec3** normalMap = (Vec3**) malloc2D(width,height,sizeof(Vec3));
	if (normalMap==NULL) return NULL;

	float heights[3][3];

	for(int x=0; x<width; x++) {
		for(int y=0; y<height; y++) {
			float northAvg = 0, southAvg= 0,eastAvg = 0, westAvg = 0;

			for(int i=-1; i<=1; i++) {
				for(int j=-1; j<=1; j++) {
					int nx = x+i, ny=y+j;
					modIndexes(planetData,&nx,&ny);
					float currHeight = planetData->heightData[ny][nx]/USHORT_MAX_F;
					currHeight = flatOcean ? fmax(currHeight,oceanLevel):currHeight; //Ha elrejti
					heights[j+1][i+1] = currHeight;

				}
			}

			for(int i=0; i<3; i++) {
				northAvg += heights[0][i]/3;
				southAvg += heights[2][i]/3;

				westAvg += heights[i][0]/3;
				eastAvg += heights[i][2]/3;
			}

			float dx = (northAvg-southAvg)*difQ;
			float dy = (westAvg-eastAvg)*difQ;

			Vec3 dir = newVec(dx,dy,1);

			normalMap[y][x]=normalize(dir);
			//normalMap[y][x]=normalize(planetData->sphereMap[y][x]);
		}
	}
	return normalMap;
}

int exportHeightmap(char *filename, PlanetData *planetData) {
	int width = planetData->dataW;
	int height = planetData->dataH;

	SDL_Surface *image = SDL_CreateRGBSurface(0,width,height,32,0,0,0,0);


	Uint8 *pixelPointer = image->pixels;
	for(int y =0; y<height; y++) {
		for(int x=0; x<width; x++) {
            //Az adatok fordítva vannak tárolva: emiatt fordítás
            int ny = height-y-1;
			ushort dataRaw = planetData->heightData[ny][x];
			float data = dataRaw/USHORT_MAX_F;
			RGB color = newRGBf(data,data,data);

			*pixelPointer++=color.b;
			*pixelPointer++=color.g;
			*pixelPointer++=color.r;
			*pixelPointer++=255;
		}
	}

	int result = SDL_SaveBMP(image,filename);

	SDL_FreeSurface(image);

	return result;
}

int exportTexture(char *filename, PlanetData *planetData) {
	int width = planetData->dataW;
	int height = planetData->dataH;

	SDL_Surface *image = SDL_CreateRGBSurface(0,width,height,32,0,0,0,0);


	Uint8 *pixelPointer = image->pixels;
	for(int y=0; y<height; y++) {
		for(int x=0; x<width; x++) {
            int ny = height-y-1;
			Vec3 pos = planetData->sphereMap[ny][x];
			ushort dataRaw = planetData->heightData[ny][x];

			float data = dataRaw/USHORT_MAX_F;

			RGB color = getTerrainColor(data, pos, planetData, isIceCap(planetData, pos));// getColor(x,y,renderSet,sceneSet);

			*pixelPointer++=color.b;
			*pixelPointer++=color.g;
			*pixelPointer++=color.r;
			*pixelPointer++=255;


		}
	}

	int result = SDL_SaveBMP(image,filename);

	SDL_FreeSurface(image);

	return result==0?IO_SUCC:IO_ERR_SDL;
}

int exportSattelite(char *filename, SceneSettings *sceneSettings) {
	PlanetData *planetData = sceneSettings->planetData;
	int width = planetData->dataW;
	int height = planetData->dataH;

	SDL_Surface *image = SDL_CreateRGBSurface(0,width,height,32,0,0,0,0);


	Uint8 *pixelPointer = image->pixels;
	for(int y=0; y<height; y++) {
		for(int x=0; x<width; x++) {
            int ny = height-y-1;
			int nx = posmod((int)(x-width*(sceneSettings->time*sceneSettings->rotationQ)*RC_2PI)+1,width);

			Vec3 pos = planetData->sphereMap[ny][nx];

			RGB color = getSurfaceColor(pos,sceneSettings);

			*pixelPointer++=color.b;
			*pixelPointer++=color.g;
			*pixelPointer++=color.r;
			*pixelPointer++=255;


		}
	}

	int result = SDL_SaveBMP(image,filename);

	SDL_FreeSurface(image);

	return result==0?IO_SUCC:IO_ERR_SDL;
}

int exportNormalmap(char *filename, PlanetData *planetData, bool flatOcean) {
	Vec3 **normalVectors = generateNormalMap(planetData, flatOcean);

	int width = planetData->dataW;
	int height = planetData->dataH;

	SDL_Surface *image = SDL_CreateRGBSurface(0,width,height,32,0,0,0,0);


	Uint8 *pixelPointer = image->pixels;
	for(int y=0; y<height; y++) {
		for(int x=0; x<width; x++) {
            int ny = height-y-1;
			Vec3 normal = normalVectors[ny][x];

			RGB color = fromNormalVector(normal);

			*pixelPointer++=color.b;
			*pixelPointer++=color.g;
			*pixelPointer++=color.r;
			*pixelPointer++=255;
		}
	}

	int result = SDL_SaveBMP(image,filename);

	free2D(normalVectors,height);
	SDL_FreeSurface(image);

	return result==0?IO_SUCC:IO_ERR_SDL;
}

int exportShadedMap(char *filename, PlanetData *planetData, float maxShade, float maxAngle, bool flatOcean) {
	Vec3 **normalVectors = generateNormalMap(planetData, flatOcean);
	Vec3 lightDir = newVec(1,0,0);

	float minAngle = 90*ANG2RAD;

	int width = planetData->dataW;
	int height = planetData->dataH;

	SDL_Surface *image = SDL_CreateRGBSurface(0,width,height,32,0,0,0,0);


	Uint8 *pixelPointer = image->pixels;
	for(int y=0; y<height; y++) {
		for(int x=0; x<width; x++) {
            int ny = height-y-1;
			Vec3 pos = planetData->sphereMap[ny][x];
			Vec3 normal = normalVectors[ny][x];

			float angle = getAngle(lightDir,normal);
			float angleClamped = clamp(angle,minAngle,maxAngle);
			float shade = easef(scale(angleClamped,minAngle,maxAngle))*maxShade;

			if(y>300 && x>240) {
				int debug = 0;
			}

			ushort dataRaw = planetData->heightData[ny][x];
			float data = dataRaw/USHORT_MAX_F;

			RGB terrainColor = getTerrainColor(data, pos, planetData, isIceCap(planetData, pos));
			RGB color = blend_lin(terrainColor,BLACK,shade);

			*pixelPointer++=color.b;
			*pixelPointer++=color.g;
			*pixelPointer++=color.r;
			*pixelPointer++=255;


		}
	}

	int result = SDL_SaveBMP(image,filename);

	free2D((void**)normalVectors,height);
	SDL_FreeSurface(image);

	return result==0?IO_SUCC:IO_ERR_SDL;
}

int savePlanetData(char *filename, const PlanetData *planetData) {
	const int HEADER_SIZE = strlen(HEADER);

	const uint8 VERSION =0xCA;

	ushort dataWidth = (ushort)planetData->dataW;
	ushort dataHeight = (ushort)planetData->dataH;
	ushort colorListSize = (ushort)getSize(planetData->terrainColors);

	//int totalBytes = 0xC1CA;
	//int totalBytesPos = HEADER_SIZE+1;
	FILE *file = fopen(filename,"wb");

	//0. a header adatok beírása
	fwrite(HEADER,HEADER_SIZE,1,file);
	fwrite(&VERSION,sizeof(uint8),1,file);
	//0. az adatstruktúrák méretének beleírása
	writeUshort(&dataHeight,file);
	writeUshort(&dataWidth,file);
	writeUshort(&colorListSize,file);

	//1. Paraméterek
	fwrite(planetData->planetParam,sizeof(PlanetParam),1,file);

	//2. Színlista
	ColorList *p = planetData->terrainColors;

	for(int i=0; i<colorListSize; i++) {
		ushort height = prcToUshort(p->height);
		writeUshort(&height,file);

		fwrite(&p->color,sizeof(RGB),1,file);
		p=p->nextColor;
	}

	//3. Magasság adatai:
	//*
	for(int y=0; y<dataHeight; y++) {
		for(int x=0; x<dataWidth; x++) {
			ushort value = planetData->heightData[y][x];
			writeUshort(&value,file);
		}
	}
	fclose(file);
	return 0;

}

int readPlanetData(char *filename, PlanetData *dest) {
	PlanetData *planetData = dest;
	FILE *file = fopen(filename,"rb");
	if(file==NULL)
		return IO_ERR_FILE_NOT_EXISTS;
	//Féjl méretének leolvasása: Végére ugrik, megnézi hol van, visszaugrik
	fseek(file,0,SEEK_END);
	long trueSize = ftell(file);
	rewind(file);

	//Header ellenõrzés
	int headerLength = strlen(HEADER);
	char tempChar = '_';
	for(int i=0; i<headerLength; i++) {
		fread(&tempChar,sizeof(char),1,file);
		if(tempChar!=HEADER[i])
			return IO_ERR_WRONG_FILETYPE;
	}

	uint8 version;
	fread(&version,sizeof(uint8),1,file);


	ushort dataWidth,dataHeight,colorListSize;

	readUshort(&dataHeight,file);
	readUshort(&dataWidth,file);
	readUshort(&colorListSize,file);

	long calcSize = plntFileSize(dataWidth,dataHeight,colorListSize);
	//printf("\nSzamolt:\t%ld\nKimert:\t\t%ld\n",calcSize,trueSize);
	if(calcSize != trueSize)
		return IO_ERR_WRONG_SIZE;

	//Mivel ellenõriztük a fájl méretét, ezért biztonságos az olvasás

	//planetData inicializálása
	freePlanetData(planetData);
	initPlanetData(planetData,dataWidth,dataHeight);

	//1. Paraméterek
	fread(planetData->planetParam,sizeof(PlanetParam),1,file);

	//2. Színlista
	if(planetData->terrainColors!=NULL)
        freeList(planetData->terrainColors);
	ColorList *colorList = NULL;

	for(int i=0; i<colorListSize; i++) {
		float height;
		readPrc(&height,file);
		RGB color;
		fread(&color,sizeof(RGB),1,file);
		colorList = addToEnd(colorList,height,color);
	}

	planetData->terrainColors=colorList;

	//3. Magassági adatok
	for(int y=0; y<dataHeight; y++) {
		for(int x=0; x<dataWidth; x++) {
            ushort value = 0;
			fread(&value,sizeof(ushort),1,file);
			planetData->heightData[y][x]=value;
		}
	}

	fclose(file);
	return IO_SUCC;
}

//--------------------
void writeColor(FILE *file, char* commandName, RGB color) {
	fprintf(file, "%s %X\n",  commandName, rgbToInt(color));
}



int savePlanetParam(CommandList *paramCmds, char *filename, PlanetData *src) {
	PlanetParam *planetParam = src->planetParam;
	FILE *file = fopen(filename,"w+");

	fprintf(file, "%s %g\n",  cmdList_getName(paramCmds,ID_oceanLevel), planetParam->oceanLevel);
	writeColor(file,cmdList_getName(paramCmds,ID_oceanBot), planetParam->oceanBotColor);
	fprintf(file, "%s %X\n",  cmdList_getName(paramCmds,ID_oceanTop), rgbToInt(planetParam->oceanTopColor));

	fprintf(file, "%s %X\n",  cmdList_getName(paramCmds,ID_city), rgbToInt(planetParam->cityColor));

	fprintf(file, "%s %X\n",  cmdList_getName(paramCmds,ID_sunset), rgbToInt(planetParam->sunsetColor));
	fprintf(file, "%s %g\n",  cmdList_getName(paramCmds,ID_sunsetInt), planetParam->sunsetIntensity);

	fprintf(file, "%s %X\n",  cmdList_getName(paramCmds,ID_atmosphere), rgbToInt(planetParam->atmosphereColor));
	fprintf(file, "%s %X\n",  cmdList_getName(paramCmds,ID_cloud), rgbToInt(planetParam->cloudColor));


	fprintf(file, "%s %g\n",  cmdList_getName(paramCmds,ID_sunsetInt), planetParam->iceCapRad);
	fprintf(file, "%s %X\n",  cmdList_getName(paramCmds,ID_polarIce), rgbToInt(planetParam->polarIceColor));
	fprintf(file, "%s %X\n",  cmdList_getName(paramCmds,ID_polarCapMin), rgbToInt(planetParam->polarCapMin));
	fprintf(file, "%s %X\n",  cmdList_getName(paramCmds,ID_polarCapMax), rgbToInt(planetParam->polarCapMax));

	fprintf(file, "%s %ld\n",  cmdList_getName(paramCmds,ID_polarCapMax), planetParam->visualSeed);

	ColorList *p = src->terrainColors;
	while(p!=NULL) {
		fprintf(file,"%s %f %X\n",cmdList_getName(paramCmds,ID_biome_add),p->height,rgbToInt(p->color));
		p=p->nextColor;
	}

	fclose(file);
	return IO_SUCC;
}


int readPlanetParam(CommandList *paramCmds, char *filename, PlanetData *dest) {
	ColorList *listCache = dest->terrainColors;
	dest->terrainColors=NULL;

	FILE *file = fopen(filename,"r");
	if(file==NULL) return IO_ERR_FILE_NOT_EXISTS;

	char line[64+1];
	char command[64+1];

	while(fgets (line, 64, file)!=NULL) {
		if(line[0]=='#')
			continue;
		if(executeParamCmd(paramCmds,line, dest) < 0) return IO_ERR_BAD_TXT_FILE;
	}

	if(dest->terrainColors==NULL) {
		dest->terrainColors = listCache;
	} else {
		freeList(listCache);
	}

	fclose(file);
	return IO_SUCC;
}
//
