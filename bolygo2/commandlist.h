#ifndef COMMANDDLIST_INC
#define COMMANDDLIST_INC
#include <SDL.h>

typedef struct CommandList{
    int id;
    char *command;
    struct CommandList* nextElement;
} CommandList;

typedef struct CommandCache{
    CommandList *params;
    CommandList *gen;
    CommandList *import;
    CommandList *export;
    CommandList *scene;
    SDL_Event *quitEvent;
} CommandCache;

enum ParamCmdIDs {
	ID_oceanLevel,
	ID_oceanBot,
	ID_oceanTop,
	ID_city,
	ID_sunset,
	ID_sunsetInt,
	ID_atmosphere,
	ID_cloud,
	ID_iceCapRad,
	ID_polarIce,
	ID_polarCapMin,
	ID_polarCapMax,
	ID_visualSeed,
	ID_biome_add,
	ID_biome_add_at,
	ID_biome_rmv,
	ID_biome_ch_lvl,
	ID_biome_ch_col,
	ID_biome_list,
	ID_biome_clear
};


enum GenCmdIds {
    ID_G_seed,

    ID_G_continentCount,
    ID_G_minContinentNodes,
    ID_G_maxContinentNodes,
    ID_G_minNodeDist,
    ID_G_maxNodeDist,

    ID_G_edgeLayer_list,
    ID_G_edgeLayerCount,
    ID_G_edgeWgh,

    ID_G_terrLayer_list,
    ID_G_terrLayerCount,
    ID_G_terrScl,
    ID_G_terrWgh,


    ID_G_generate
};

enum ImportCmdIds {
    ID_I_planetData,
    ID_I_planetParam
};

enum ExportCmdIds {
    ID_E_texture,
    ID_E_heightmap,
    ID_E_sattelite,
    ID_E_normalmap,
    ID_E_shadedMap,
    ID_E_planetData,
    ID_E_planetParam
};


enum SceneCmdIds {
    ID_S_cameraDir,
    ID_S_lightDir
};

void initCommands(CommandCache *commands, SDL_Event *quitEvent);

int cmdList_getId(CommandList *list, char *command);
const char *cmdList_getName(CommandList *list, int id);

void freeCommands(CommandCache *commands);
#endif // COMMANDDLIST_INC
