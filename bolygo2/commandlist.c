#include <string.h>
#include <stdbool.h>
#include "commandlist.h"

int cmdList_getId(CommandList *list, char *command) {
	CommandList *p = list;
	while(p!=NULL) {
		if(strcmp(p->command,command)==0)
			return p->id;
		p=p->nextElement;
	}
	return -1;
}

const char *cmdList_getName(CommandList *list, int id) {
	CommandList *p = list;
	while(p!=NULL) {
		if(p->id==id)
			return p->command;
		p=p->nextElement;
	}
	return NULL;
}

CommandList* cmdList_addToEnd(const CommandList *list, int id, char *command) {
	//Listaelem inicializásála
	CommandList *elementPnt = (CommandList*)malloc(sizeof(CommandList));

	elementPnt->nextElement=NULL;
	elementPnt->id=id;
	char *cmdPnt = malloc(sizeof(char)*(strlen(command)+1));
	cmdPnt = strcpy(cmdPnt,command);
	elementPnt->command=cmdPnt;

	CommandList *p = list;
    //Utolsó elem keresése
	if(p == NULL)
		return elementPnt;

	while(p->nextElement!=NULL)
		p=p->nextElement;
	p->nextElement=elementPnt;
	return list;
}

void cmdList_free(CommandList *list) {
	CommandList *p = list;
	CommandList *temp;

	while (p!=NULL) {
		temp = p->nextElement;
		free(p->command);
		free(p);
		p = temp;
	}
}


void initCommands(CommandCache *commands, SDL_Event *quitEvent) {
    commands->quitEvent=quitEvent;

	CommandList *params = NULL;

	params = cmdList_addToEnd(params, ID_oceanLevel,"oceanLevel");
	params = cmdList_addToEnd(params, ID_oceanBot,"oceanBot");
	params = cmdList_addToEnd(params, ID_oceanTop,"oceanTop");

	params = cmdList_addToEnd(params, ID_city,"city");

	params = cmdList_addToEnd(params, ID_sunset,"sunset");
	params = cmdList_addToEnd(params, ID_sunsetInt,"sunsetInt");
	params = cmdList_addToEnd(params, ID_atmosphere,"atmosphere");
	params = cmdList_addToEnd(params, ID_cloud,"cloud");

	params = cmdList_addToEnd(params, ID_iceCapRad,"iceCapRad");
	params = cmdList_addToEnd(params, ID_polarIce,"polarIce");
	params = cmdList_addToEnd(params, ID_polarCapMin,"polarCapMin");
	params = cmdList_addToEnd(params, ID_polarCapMax,"polarCapMax");

	params = cmdList_addToEnd(params, ID_visualSeed,"visualSeed");

	params = cmdList_addToEnd(params, ID_biome_add,"biome_add");
	params = cmdList_addToEnd(params, ID_biome_add_at,"biome_add_at");

	params = cmdList_addToEnd(params, ID_biome_rmv,"biome_rmv");

	params = cmdList_addToEnd(params, ID_biome_ch_lvl,"biome_ch_lvl");
	params = cmdList_addToEnd(params, ID_biome_ch_col,"biome_ch_col");

	params = cmdList_addToEnd(params, ID_biome_list,"biome_list");

	params = cmdList_addToEnd(params, ID_biome_clear,"biome_clear");

	commands->params=params;
    //--------------------
	CommandList *genSet = NULL;
    genSet = cmdList_addToEnd(genSet, ID_G_seed, "seed");
    genSet = cmdList_addToEnd(genSet, ID_G_continentCount, "continentCount");
    genSet = cmdList_addToEnd(genSet, ID_G_minContinentNodes, "minContinentNodes");
    genSet = cmdList_addToEnd(genSet, ID_G_maxContinentNodes, "maxContinentNodes");
    genSet = cmdList_addToEnd(genSet, ID_G_minNodeDist, "minNodeDist");
    genSet = cmdList_addToEnd(genSet, ID_G_maxNodeDist, "maxNodeDist");

    genSet = cmdList_addToEnd(genSet, ID_G_edgeLayer_list, "edgeLayer_list");
    genSet = cmdList_addToEnd(genSet, ID_G_edgeLayerCount, "edgeLayerCount");
    genSet = cmdList_addToEnd(genSet, ID_G_edgeWgh, "edgeWgh");

    genSet = cmdList_addToEnd(genSet, ID_G_terrLayer_list, "terrLayer_list");
    genSet = cmdList_addToEnd(genSet, ID_G_terrLayerCount, "terrLayerCount");
    genSet = cmdList_addToEnd(genSet, ID_G_terrScl, "terrScl");
    genSet = cmdList_addToEnd(genSet, ID_G_terrWgh, "terrWgh");

    genSet = cmdList_addToEnd(genSet, ID_G_generate, "generate");

    commands->gen=genSet;
    //--------------------
    CommandList *import = NULL;
    import = cmdList_addToEnd(import,ID_I_planetData,"planetData");
    import = cmdList_addToEnd(import,ID_I_planetParam,"planetParam");

    commands->import=import;

    //--------------------
    CommandList *exportCmds = NULL;
    exportCmds = cmdList_addToEnd(exportCmds,ID_E_texture ,"texture");
    exportCmds = cmdList_addToEnd(exportCmds,ID_E_heightmap,"heightmap");
    exportCmds = cmdList_addToEnd(exportCmds,ID_E_sattelite,"sattelite");
    exportCmds = cmdList_addToEnd(exportCmds,ID_E_normalmap,"normalmap");
    exportCmds = cmdList_addToEnd(exportCmds,ID_E_shadedMap,"shadedMap");
    exportCmds = cmdList_addToEnd(exportCmds,ID_E_planetData ,"planetData");
    exportCmds = cmdList_addToEnd(exportCmds,ID_E_planetParam ,"planetParam");

    commands->export=exportCmds;
}


void freeCommands(CommandCache *commands){
    cmdList_free(commands->params);
}
