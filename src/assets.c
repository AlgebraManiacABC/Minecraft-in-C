#include "assets.h"
#include <string.h>
#include <ctype.h>
#include "textures.h"
#include "csv.h"

//	Should have [ID,NAME,FILE] (only 2 commas)
#define COMMA_COUNT 2
#define EXPECTED_FORMAT "ID,NAME,FILE"

Uint32 numBlocks = 0;
char ** blockNames = NULL;
char ** assetFiles = NULL;
GLuint * blockTextures = NULL;
GLuint fontMapTexture = 0;

NODISCARD
int commaCount(const char * line)
{
	int occ = 0;
	for(int i=0; line[i]; i++)
		line[i] == ',' ? occ++ : 0;
	return occ;
}

NODISCARD
char * strchrOrEOL(char * line, char c)
{
	char * ret = strchr(line,c);
	if(ret) return ret;
	return &(line[strlen(line)]);
}

void strncpyButIgnoreChars(char *__restrict__ dest, const char *__restrict__ source, size_t count, const char * toIgnore)
{
	size_t maxlen = strnlen(source,count);
	char * newSource = malloc(sizeof(char)*(maxlen+1));
	size_t ignorables = strlen(toIgnore);
	size_t removed = 0;
	for(size_t i=0, j=0; source[i] && i < maxlen; i++)
	{
		bool include = true;
		for(size_t c=0; c<ignorables; c++)
		{
			if(strchr(source+i,toIgnore[c]) == source+i)
			{
				include = false;
				removed++;
			}
		}
		if(include)
			newSource[j++] = source[i];
	}
	strncpy(dest,newSource,maxlen-removed);
	free(newSource);
}

NODISCARD
int loadBlockTextures()
{
	if(!blockNames || !assetFiles) ERR_NULLP_RET_FAIL;

	blockTextures = calloc(numBlocks,sizeof(GLuint));
	if(!blockTextures) ERR_NULLP_RET_FAIL;

	for(int i=0; i<numBlocks; i++)
	{
		if(!assetFiles[i]) ERR_NULLP_RET_FAIL;
		blockTextures[i] = textureFromFile(assetFiles[i]);
		if(!blockTextures[i]) return EXIT_FAILURE;
	}
	return EXIT_SUCCESS;
}

void loadAssets(const char * assetListFilename)
{
	csv_t csv = CSV_loadFromFile(assetListFilename);
	if(!csv) ERR_NULLP_RETURN;
	//CSV_fprint(stderr,csv);

	int lineCount = CSV_lineCount(csv);
	size_t maxBlockID = 0;
	for(int i=0; i<lineCount; i++)
	{
		//	First value: ID (size_t: ensure positivity)
		long temp = atol(CSV_getValueAt(csv,i,0));
		if(temp < 0)
		{
			setError(ERR_MESG,"Negative block ID at line %d of asset list \"%s\"!",lineCount,assetListFilename);
			numBlocks = 0;
			return;
		}
		size_t currentID = temp;
		if(currentID >= maxBlockID)
		{
			maxBlockID = currentID;
			blockNames = realloc(blockNames,sizeof(char*)*(currentID+1));
			assetFiles = realloc(assetFiles,sizeof(char*)*(currentID+1));
			if(!blockNames || !assetFiles)
			{
				setError(ERR_CODE,ERR_NOMEM);
				numBlocks = 0;
				return;
			}
		}
		/*******************************************/

		//	Second value: NAME (string)
		const char * currentName = CSV_getValueAt(csv,i,1);
		blockNames[currentID] = strdup(currentName);
		/*******************************/

		//	Third value: FILENAME (string)
		const char * currentFilename = CSV_getValueAt(csv,i,2);
		assetFiles[currentID] = strdup(currentFilename);
		/********************************/

		numBlocks++;
	}

	if(loadBlockTextures())
	{
		numBlocks = 0;
		return;
	}
	CSV_free(csv);
}

void loadFontMap(const char * fontMapFilename)
{
	fontMapTexture = textureFromFile(fontMapFilename);
	return;
}

NODISCARD
Uint8 IDof(const char * blockName)
{
	for(int i=0; i<numBlocks; i++)
	{
		if(!strcmp(blockNames[i],blockName)) return i;
	}
	return 0;
}