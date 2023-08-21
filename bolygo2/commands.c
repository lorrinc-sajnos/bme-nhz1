#include <SDL.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#include "commands.h"
#include "commandlist.h"
#include "misc.h"
#include "typedefs.h"
#include "generationsettings.h"
#include "scenesettings.h"
#include "planetdata.h"
#include "heightgeneration.h"
#include "dataio.h"


int executeCommand(CommandCache *commands, char *command, SceneSettings *sceneSettings, GenSettings *genSettings ) {
    PlanetData *planetData = sceneSettings->planetData;

	char branch[64+1] = "";
	char subCommand[64+1] = "";
	strcut(command,branch,subCommand);

	int result = CMD_ERR_DOESNT_EXIST;

	if(strcmp(branch,"param")==0){
		result = executeParamCmd(commands->params,subCommand,planetData);
	}
	else if (strcmp(branch, "gen")==0) {
		result = executeGenCmd(commands->gen,subCommand,planetData,genSettings);
	}
	else if (strcmp(branch, "import")==0) {
		result = executeImportCmd(commands->import,subCommand,planetData,commands->params);
	}
	else if (strcmp(branch, "export")==0) {
		result = executeExportCmd(commands->export,subCommand,sceneSettings,commands->params);
	}
	else if(strcmp(branch, "exit")==0){
        SDL_PushEvent(commands->quitEvent);
        result = CMD_QUIT;
	}
	return result;
}

int assignColor(char *str, char* cmd, RGB *dest) {
	if(strcmp(str,cmd)==0) {
		printf("%06X\n",*dest);
		return 0;
	}
	int value=0;
	int result = sscanf(str,"%s %x",cmd,&value);
	if(result!=2)
		return -1;
	*dest = newRGBi(value);

	return 1;
}

int assignInt32(char *str, char* cmd, int32 *dest) {
	if(strcmp(str,cmd)==0) {
		printf("%ld\n",*dest);
		return 0;
	}

	long int value=0;
	int result = sscanf(str,"%s %ld",cmd,&value);
	if(result!=2)
		return -1;
	*dest = (int32)value;
	return 1;
}

int assignPrc(char *str, char* cmd, float *dest) {
	if(strcmp(str,cmd)==0) {
		printf("%f\n",*dest);
		return 0;
	}
	float value=0;
	int result = sscanf(str,"%s %f",cmd,&value);
	if(result!=2)
		return -1;
	*dest=clamp(value,0,1);
	return 1;
}
int assignInt(char *str, char* cmd, int *dest) {
	if(strcmp(str,cmd)==0) {
		printf("%d\n",*dest);
		return 0;
	}
	int value=0;
	int result = sscanf(str,"%s %d",cmd,&value);
	if(result!=2)
		return -1;
	*dest=value;
	return 1;
}

int charToYesNo(char chr){
    //Kihasználjuk, hogy a kisbetűknek nagyobb a számuk
    chr = chr<'a'?chr+'A'-'a':chr;
    if(chr=='y')
        return 1;
    if(chr=='n')
        return 0;
    return -1;
}


int executeParamCmd(CommandList *paramCmds, char *line, PlanetData *planetData) {
	PlanetParam *planetParam = planetData->planetParam;
	//ColorList *terrainColors = planetData->terrainColors;

	char command[32+1];

	if(sscanf(line,"%s ",command)!=1) {
		return CMD_ERR_MALFORMED;
	}

	int cmdId = cmdList_getId(paramCmds,command);
	if(cmdId==-1) {
		return CMD_ERR_MALFORMED;
	}
	float floatParam=0;
	int intParam=0;
	int intParam2=0;
	ColorList *tempP;

	switch(cmdId) {
	case ID_oceanLevel :
		if (assignPrc(line,command,&planetParam->oceanLevel)<0) return CMD_ERR_MALFORMED;
		break;

	case ID_oceanBot :
		if (assignColor(line,command,&planetParam->oceanBotColor)<0) return CMD_ERR_MALFORMED;
		break;

	case ID_oceanTop :
		if (assignColor(line,command,&planetParam->oceanTopColor)<0) return CMD_ERR_MALFORMED;
		break;

	case ID_city :
		if (assignColor(line,command,&planetParam->cityColor)<0) return CMD_ERR_MALFORMED;
		break;

	case ID_sunset :
		if (assignColor(line,command,&planetParam->sunsetColor)<0) return CMD_ERR_MALFORMED;
		break;

	case ID_sunsetInt :
		if (assignPrc(line,command,&planetParam->sunsetIntensity)<0) return CMD_ERR_MALFORMED;
		break;

	case ID_atmosphere :
		if (assignColor(line,command,&planetParam->atmosphereColor)<0) return CMD_ERR_MALFORMED;
		break;

	case ID_cloud :
		if (assignColor(line,command,&planetParam->cloudColor)<0) return CMD_ERR_MALFORMED;
		break;

	case ID_iceCapRad :
		if (assignPrc(line,command,&planetParam->iceCapRad)<0) return CMD_ERR_MALFORMED;
		break;

	case ID_polarIce :
		if (assignColor(line,command,&planetParam->polarIceColor)<0) return CMD_ERR_MALFORMED;
		break;

	case ID_polarCapMin :
		if (assignColor(line,command,&planetParam->polarCapMin)<0) return CMD_ERR_MALFORMED;
		break;

	case ID_polarCapMax :
		if (assignColor(line,command,&planetParam->polarCapMax)<0) return CMD_ERR_MALFORMED;
		break;

	case ID_visualSeed :
		if (assignInt32(line,command,&planetParam->visualSeed)) return CMD_ERR_MALFORMED;
		break;

	case ID_biome_add :
		if (sscanf(line,"%s %f %x",command,&floatParam,&intParam)!=3) return CMD_ERR_MALFORMED;

		planetData->terrainColors = addToEnd(planetData->terrainColors,floatParam,newRGBi(intParam));
		break;
	case ID_biome_add_at :
		if (sscanf(line,"%s %d %f %x",command,&intParam,&floatParam,&intParam2)!=4) return CMD_ERR_MALFORMED;

		//RGB elementColor = newRGBi(intParam);
		tempP = getElement(planetData->terrainColors,intParam);
		if(tempP==NULL) return CMD_LIST_ERR;
		planetData->terrainColors = addAt(tempP,floatParam,newRGBi(intParam2));
		break;
	case ID_biome_rmv :
		if (sscanf(line,"%s %d",command,&intParam)!=2) return CMD_ERR_MALFORMED;
		tempP = removeElement(planetData->terrainColors,intParam);
		if(tempP==NULL) return CMD_LIST_ERR;
		planetData->terrainColors = tempP;
		break;

	case ID_biome_ch_lvl:
		if (sscanf(line,"%s %d %f",command,&intParam,&floatParam)!=3) return CMD_ERR_MALFORMED;
		tempP = getElement(planetData->terrainColors,intParam);
		if(tempP==NULL) return CMD_LIST_ERR;
		tempP->height=floatParam;
		break;

	case ID_biome_ch_col:
		if (sscanf(line,"%s %d %x",command,&intParam,&intParam2)!=3) return CMD_ERR_MALFORMED;
		tempP = getElement(planetData->terrainColors,intParam);
		if(tempP==NULL) return CMD_LIST_ERR;
		tempP->color=newRGBi(intParam2);
		break;

	case ID_biome_list :
		tempP = planetData->terrainColors;
		int cnt = 0;
		while(tempP!=NULL) {
			printf("\t%d: Mag.: %g\t\tSzin:%06x\n",cnt,tempP->height,rgbToInt(tempP->color));
			tempP=tempP->nextColor;
			cnt++;
		}
		break;

	case ID_biome_clear :
		freeList(planetData->terrainColors);
		planetData->terrainColors=NULL;
		break;

	default:
		return CMD_ERR_MALFORMED;
	}
	return CMD_SUCC;
}

int executeGenCmd(CommandList *genCmds, char *line, PlanetData *planetData, GenSettings *genSettings) {
	char command[32+1];

	if(sscanf(line,"%s ",command)!=1) {
		return CMD_ERR_MALFORMED;
	}

	int cmdId = cmdList_getId(genCmds,command);
	if(cmdId==-1) {
		return CMD_ERR_MALFORMED;
	}

	float floatParam=0;
	int intParam=0;

	switch(cmdId) {
	case ID_G_seed :
		if (assignInt32(line,command,&genSettings->seed)<0) return CMD_ERR_MALFORMED;
		break;
	//Kontinens
	case ID_G_continentCount :
		if (assignInt(line,command,&genSettings->continentCount)<0) return CMD_ERR_MALFORMED;
		break;

	case ID_G_minContinentNodes :
		if (assignInt(line,command,&genSettings->minContinentNodes)<0) return CMD_ERR_MALFORMED;
		break;

	case ID_G_maxContinentNodes :
		if (assignInt(line,command,&genSettings->maxContinentNodes)<0) return CMD_ERR_MALFORMED;
		break;


	case ID_G_minNodeDist :
		if (assignPrc(line,command,&genSettings->minNodeDist)<0) return CMD_ERR_MALFORMED;
		break;

	case ID_G_maxNodeDist :
		if (assignPrc(line,command,&genSettings->maxNodeDist)<0) return CMD_ERR_MALFORMED;
		break;

	//Él param.
	case ID_G_edgeLayer_list:
		printf("Elek:\n\tSulyok\n");
		for(int i=0; i<genSettings->edgeLayerCount; i++)
			printf("\t%g",genSettings->edgeWgh[i]);

		printf("\n");
		break;

	case ID_G_edgeLayerCount :
		if (assignInt(line,command,&genSettings->edgeLayerCount)<0) return CMD_ERR_MALFORMED;

		if(genSettings->edgeLayerCount>5) {
			genSettings->edgeLayerCount=5;
			return CMD_ERR_INDEX;
		}
		if(genSettings->edgeLayerCount<0) {
			genSettings->edgeLayerCount=0;
			return CMD_ERR_INDEX;
		}
		break;

	case ID_G_edgeWgh :
		if (sscanf(line,"%s %d %f",command,&intParam,&floatParam)!=3) return CMD_ERR_MALFORMED;
		if(intParam<0 || intParam>=genSettings->edgeLayerCount)
			return CMD_ERR_INDEX;
		genSettings->edgeWgh[intParam]=clamp(floatParam,0,1);
		break;

	//Domb. param.
	case ID_G_terrLayer_list:
		printf("Domborzat:\n\tSkala:\t\n");
		for(int i=0; i<genSettings->terrLayerCount; i++)
			printf("\t%g",genSettings->terrScl[i]);
		printf("\n\tSulyok:\t\n");
		for(int i=0; i<genSettings->terrLayerCount; i++)
			printf("\t%g",genSettings->terrWgh[i]);

		printf("\n");
		break;

	case ID_G_terrLayerCount :
		if (assignInt(line,command,&genSettings->terrLayerCount)<0) return CMD_ERR_MALFORMED;

		if(genSettings->terrLayerCount>5) {
			genSettings->terrLayerCount=5;
			return CMD_ERR_INDEX;
		}
		if(genSettings->terrLayerCount<0) {
			genSettings->terrLayerCount=0;
			return CMD_ERR_INDEX;
		}
		break;

	case ID_G_terrScl :
		if (sscanf(line,"%s %d %f",command,&intParam,&floatParam)!=3) return CMD_ERR_MALFORMED;
		if(intParam<0 || intParam>=genSettings->terrLayerCount)
			return CMD_ERR_INDEX;
		genSettings->terrScl[intParam]=clamp(floatParam,0,1);
		break;

	case ID_G_terrWgh :
		if (sscanf(line,"%s %d %f",command,&intParam,&floatParam)!=3) return CMD_ERR_MALFORMED;
		if(intParam<0 || intParam>=genSettings->terrLayerCount)
			return CMD_ERR_INDEX;
		genSettings->terrWgh[intParam]=clamp(floatParam,0,1);
		break;

	case ID_G_generate :
		generateHeightData(planetData,genSettings);
		break;

	default:
		return CMD_ERR_MALFORMED;
	}
	return CMD_SUCC;
}

int executeImportCmd(CommandList *impCmds, char *line, PlanetData *planetData, CommandList *paramCmds) {
	char command[32+1];
	char strParam[64+1];

	if(sscanf(line,"%s ",command)!=1) {
		return CMD_ERR_MALFORMED;
	}

	int cmdId = cmdList_getId(impCmds,command);
	if(cmdId==-1) {
		return CMD_ERR_MALFORMED;
	}

	switch(cmdId) {
	case ID_I_planetData :
		if (sscanf(line,"%s %s",command,strParam)!=2) return CMD_ERR_MALFORMED;

	    if (readPlanetData(strParam,planetData) != IO_SUCC)
            return CMD_ERR_IO;

		break;

	case ID_I_planetParam :
		if (sscanf(line,"%s %s",command,strParam)!=2) return CMD_ERR_MALFORMED;

	    if (readPlanetParam(paramCmds,strParam,planetData)!= IO_SUCC)
            return CMD_ERR_IO;

		break;

	default:
		return CMD_ERR_MALFORMED;
	}
	return CMD_SUCC;
}

int processExport(int result){
    if(result!=IO_SUCC)
        return CMD_ERR_IO;
    printf("Fajlexport sikeres.\n");
    return CMD_SUCC;
}
int executeExportCmd(CommandList *expCmds, char *line, SceneSettings *sceneSettings, CommandList *paramCmds) {
    PlanetData *planetData = sceneSettings->planetData;
	char command[32+1];

	char strParam[64+1];
	char charParam = '\0';
	int tempInt = 0;
	bool flatOcean = false;
	float maxAngle=0,maxShade=0;

	if(sscanf(line,"%s ",command)!=1) {
		return CMD_ERR_MALFORMED;
	}

	int cmdId = cmdList_getId(expCmds,command);
	if(cmdId==-1) {
		return CMD_ERR_MALFORMED;
	}

	switch(cmdId) {
    case ID_E_texture :
		if (sscanf(line,"%s %s",command,strParam)!=2) return CMD_ERR_MALFORMED;

	    return processExport(exportTexture(strParam,planetData));
            return CMD_ERR_IO;
        break;

    case ID_E_heightmap :
		if (sscanf(line,"%s %s",command,strParam)!=2) return CMD_ERR_MALFORMED;

	    return processExport(exportHeightmap(strParam,planetData));
        break;

    case ID_E_sattelite :
		if (sscanf(line,"%s %s",command,strParam)!=2) return CMD_ERR_MALFORMED;

	    return processExport(exportSattelite(strParam,sceneSettings));
        break;

    case ID_E_normalmap :
		if (sscanf(line,"%s %s %c",command,strParam,&charParam)!=3) return CMD_ERR_MALFORMED;

        tempInt = charToYesNo(charParam);
        if(tempInt==-1)
            return CMD_ERR_MALFORMED;

        flatOcean = tempInt==1;

	    return processExport(exportNormalmap(strParam,planetData,flatOcean));
        break;

    case ID_E_shadedMap :
		if (sscanf(line,"%s %s %c %f %f",command,strParam,&charParam,&maxAngle,&maxShade)!=5) return CMD_ERR_MALFORMED;

        tempInt = charToYesNo(charParam);
        if(tempInt==-1)
            return CMD_ERR_MALFORMED;

        flatOcean = tempInt==1;
        maxAngle*=ANG2RAD;

	    return processExport(exportShadedMap(strParam,planetData,maxShade,maxAngle,flatOcean));
        break;

    case ID_E_planetData :
		if (sscanf(line,"%s %s",command,strParam)!=2) return CMD_ERR_MALFORMED;

	    return processExport(savePlanetData(strParam,planetData));
        break;

    case ID_E_planetParam :
		if (sscanf(line,"%s %s",command,strParam)!=2) return CMD_ERR_MALFORMED;

	    return processExport(savePlanetParam(paramCmds,strParam,planetData));
        break;

	default:
		return CMD_ERR_MALFORMED;
	}
	return CMD_SUCC;
}

void printErrorCode(int errorCode) {
	switch(errorCode) {
	case CMD_ERR_DOESNT_EXIST:
		printf("NEMLETEZO PARANCS\n");
		break;
	case CMD_ERR_EXEC:
		printf("VEGREHAJTASI HIBA\n");
		break;
	case CMD_ERR_INDEX:
		printf("ROSSZ INDEX\n");
		break;
	case CMD_ERR_MALFORMED:
		printf("HIBAS PARANCS\n");
		break;
	case CMD_LIST_ERR:
		printf("LISTA HIBA\n");
		break;
	case CMD_ERR_IO:
		printf("FAJL I/O HIBA\n");
		break;
	}
}

