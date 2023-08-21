#ifndef COMMANDS_INC
#define COMMANDS_INC

#define CMD_SUCC 1
#define CMD_ERR_DOESNT_EXIST -1
#define CMD_ERR_MALFORMED -2
#define CMD_LIST_ERR -3
#define CMD_ERR_INDEX -4
#define CMD_ERR_IO -5
#define CMD_ERR_EXEC -100
#define CMD_QUIT -69420

#include "planetdata.h"
#include "generationsettings.h"
#include "scenesettings.h"
#include "commandlist.h"


int executeCommand(CommandCache *commands, char *command, SceneSettings *sceneSettings, GenSettings *genSettings);

int executeParamCmd(CommandList *paramCmds, char *line, PlanetData *planetData);
int executeGenCmd(CommandList *genCmds, char *line, PlanetData *planetData, GenSettings *genSettings);
int executeImportCmd(CommandList *impCmds, char *line, PlanetData *planetData, CommandList *paramCmds);
int executeExportCmd(CommandList *expCmds, char *line, SceneSettings *sceneSettings, CommandList *paramCmds);

void printErrorCode(int errorCode);
#endif // COMMANDS_INC
