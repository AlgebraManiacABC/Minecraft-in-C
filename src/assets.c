#include "assets.h"
#include <string.h>
#include <ctype.h>
#include "textures.h"

//	Should have [ID,NAME,FILE] (only 2 commas)
#define COMMA_COUNT 2
#define EXPECTED_FORMAT "ID,NAME,FILE"

Uint32 numBlocks = 0;
char ** blockNames = NULL;
char ** assetFiles = NULL;
GLuint * blockTextures = NULL;

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
	blockTextures = calloc(numBlocks,sizeof(GLuint));
	if(!blockTextures) return EXIT_FAILURE;
	for(int i=0; i<numBlocks; i++)
	{
		blockTextures[i] = textureFromFile(assetFiles[i]);
		if(!blockTextures[i]) return EXIT_FAILURE;
	}
	return EXIT_SUCCESS;
}

void loadAssets(const char * assetListFilename)
{
	FILE * assetList = fopen(assetListFilename,"r");
	if(!assetList) ERR_NOFIL_RETURN;

	int lineCount = 0;
	size_t maxBlockID = 0;
	while(!feof(assetList))
	{
		//char *lineBuffer = NULL;
		char lineBuffer[1024] = {'\0'};
		fgets(lineBuffer,1024,assetList);
		//fscanf(assetList," %m[^\n\r] ",&lineBuffer);
		//fprintf(stderr,"lineBuffer: [%s]\n",lineBuffer);
		lineCount++;

		if(commaCount(lineBuffer) != COMMA_COUNT)
		{
			setError(ERR_MESG,"Asset list \"%s\" has incorrect format at line %d! Expected \"%s\"",assetListFilename,lineCount,EXPECTED_FORMAT);
			numBlocks = 0;
			return;
		}

		size_t currentID = 0;
		char * latter = lineBuffer;
		char * oldLatter;
		for(size_t c=0; c<=COMMA_COUNT; c++)
		{
			oldLatter = latter;
			latter = strchrOrEOL(oldLatter,',') + 1;
			//	latter = [Possible comma if not first element][Current value][Any following commas and values][\0]
			size_t valueLen = strlen(oldLatter) - strlen(latter) - 1;
			if(valueLen <= 0)
			{
				setError(ERR_MESG,"Asset list \"%s\" has missing value [#%ld] at line %d! Expected \"%s\"",assetListFilename,c,lineCount,EXPECTED_FORMAT);
				numBlocks = 0;
				return;
			}

			//	Switch statement which is dependent on expected format. There are more "dynamic" methods, but this is a quick and dirty solution.
			//	oldLatter[0] to oldLatter[valueLen-1] will contain the string of interest.
			switch(c)
			{
				case 0:
				//	First value: ID (Uint8)
					currentID = atol(oldLatter);
					if(currentID < 0)
					{
						setError(ERR_MESG,"Negative block ID at line %d of asset list \"%s\"!",lineCount,assetListFilename);
						numBlocks = 0;
						return;
					}
					if(currentID == 0 || currentID > maxBlockID)
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
					numBlocks++;
					fprintf(stderr,"ID: [%lld]\t",(long long)currentID);
					break;
				case 1:
				//	Second vaue: NAME (string)
					blockNames[currentID] = calloc(valueLen,sizeof(char));
					strncpy(blockNames[currentID],oldLatter,valueLen);
					fprintf(stderr,"NAME: [%s]\t",blockNames[currentID]);
					break;
				case 2:
				//	Third value: FILE (string)
					assetFiles[currentID] = calloc(valueLen,sizeof(char));
					strncpyButIgnoreChars(assetFiles[currentID],oldLatter,valueLen,"\"\n\r");
					fprintf(stderr,"FILE: [%s]\n",assetFiles[currentID]);
					break;
			}
		}

		//free(lineBuffer);
	}

	fclose(assetList);

	if(loadBlockTextures())
	{
		numBlocks = 0;
		return;
	}
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