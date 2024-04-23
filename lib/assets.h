#include <stdio.h>
#include <GL/glew.h>
#include <SDL2/SDL.h>
#include "debug.h"

//typedef struct blockDB blockDB_t;

extern Uint32 numBlocks;
extern char ** blockNames;
extern char ** assetFiles;
extern GLuint * blockTextures;
extern GLuint fontMapTexture;

#define FONT_MAP_VERT_CHARS 16
#define FONT_MAP_HORZ_CHARS 16
#define FONT_MAP_UNK_CHAR_INDEX ('\xb1')

void loadAssets(const char * assetListFilename);

void loadFontMap(const char * fontMapFilename);

NODISCARD
/**
 * Returns the ID of the given block name, or UINT8_MAX if not found.
 *
 * @param blockName
 * @return ID of given block, or 0 if not found.
 */
Uint8 IDof(const char * blockName);