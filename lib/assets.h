#include <stdio.h>
#include <SDL2/SDL.h>

//typedef struct blockDB blockDB_t;

extern Uint32 numBlocks;
extern char ** blockNames;
extern char ** assetFiles;

void loadAssets(const char * assetListFilename);