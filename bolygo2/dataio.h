#ifndef DATAIO_INC
#define DATAIO_INC

#include "commandlist.h"

#define IO_SUCC 0
#define IO_ERR_FATAL -1
#define IO_ERR_OPEN -2
#define IO_ERR_WRONG_FILETYPE -3
#define IO_ERR_WRONG_SIZE -4
#define IO_ERR_EOF -5
#define IO_ERR_BAD_TXT_FILE -6
#define IO_ERR_FILE_NOT_EXISTS -7
#define IO_ERR_SDL -100

int exportTexture(char *filename, PlanetData *planetData);
int exportHeightmap(char *filename, PlanetData *planetData);
int exportSattelite(char *filename, SceneSettings *sceneSettings);
int exportNormalmap(char *filename, PlanetData *planetData, bool flatOcean);
int exportShadedMap(char *filename, PlanetData *planetData, float maxShade, float maxAngle, bool flatOcean);

int savePlanetData(char *filename, const PlanetData *planetData);
int readPlanetData(char *filename, PlanetData *planetData);

int savePlanetParam(CommandList *paramCmds, char *filename, PlanetData *src);
int readPlanetParam(CommandList *paramCmds, char *filename, PlanetData *dest);
#endif // DATAIO_INC
