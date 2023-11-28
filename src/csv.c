#include "csv.h"

struct csv
{
	uint lineCount;
	char ** lineStrings;
	uint * valueCounts;
	char *** values;
};

NODISCARD
/**
 * Determines if a quote is properly escaped.
 *
 * @param quote the char pointer to a quote in a string
 * @param startOfString the start of said string, so as to not underseek
 * @returns true if escaped properly (odd number of backslashes immediately preceding quote), false otherwise.
 */
bool CSV_quoteIsEscaped(char * quote, const char * startOfString)
{
	if(!quote || !startOfString) return false;
	size_t backslashCount = 0;
	for(char * scanner = quote; scanner >= startOfString; scanner--)
	{
		if(*scanner == '\\') backslashCount++;
		else break;
	}
	//	If there is an odd number of backslashes before the quote, then it is properly escaped.
	if(backslashCount % 2) return true;
	//	An even number of backslashes just means there are half as many escaped backslashes before an unescaped quote.
	return false;
}

NODISCARD
/**
 * Determines if the current string value pointed at by `str` is contained within quotes or not.
 *
 * @param str a pointer to the value in question in a string
 * @returns a pointer to the last quote, if the value was contained in quotes, or `NULL` if not. `(-1)` on failure.
 */
char * CSV_valueContainedInQuotes(char * str)
{
	//	Assume always a valid string
	if(!str) ERR_NULLP_RET_X((char*)-1);

	//	Skip all leading whitespace
	while(isblank(str[0])) str++;
	//	String must begin with quote (we have ignored leading whitespace)
	if(str[0] != '"') return NULL;

	int len = strlen(str);

	//	Find the next quote which isn't escaped
	char * nextQuote = str;
	do
	{
		nextQuote = strchr(nextQuote+1,'"');
		if(!nextQuote) return NULL;

	} while(CSV_quoteIsEscaped(nextQuote,str));
	/********************************/

	//	What is the char after the `"`?
	switch(*(nextQuote+1))
	{
		//	Case of newlines, end of line (str), comma
		case '\0':
		case '\n':
		case '\r':
		case ',':
			//	At this point we have: "[stuff]"[,\r\n\0]
			//	In all of these cases, we have a properly quoted string.
			return nextQuote;
		case ' ':
		{
			//	Although we will eventually ignore spaces,
			//	if after the spaces there is a valid char,
			//	it is still properly quoted.
			bool reachedCommaOrNewline = false;
			//	Check the following chars for a non-space character.
			for(char *c=nextQuote+2; c<(nextQuote+len); c++)
			{
				//	If newline/comma, it's properly quoted
				if(*c == '\n' || *c == '\r' || *c == ',' || *c == '\0')
				{
					reachedCommaOrNewline = true;
					break;
				}
				//	If it's not a space OR any of the above,
				//	This is not a properly quoted string.
				if(*c != ' ') break;
			}
			if(reachedCommaOrNewline)
				return nextQuote;
			return NULL;
		}
		//	Any character other than newlines, end of str, comma, or other whitespace.
		//	These are not properly quoted strings.
		default:
			return NULL;
	}

	//	Should never reach here
	ERR_CODE_RETURN_X(ERR_UNDEF,(char*)-1);
	//	TODO: test if commenting out is safe.
}

NODISCARD
/**
 * Given the beginning of a csv value string, removes containing quotes (if applicable) by setting them to `'\0'`,
 * and sets the given value pointer to the start of the modified value (if parameter is not `NULL`)
 * setValue also turns the following delimiter to `'\0'`, if it exists.
 *
 * @param startOfValue a pointer to the original first character in the value
 * @param valuePointer an address to a char pointer, which will be updated to the beginning of the modified value string.
 * If parameter is `NULL`, ignored.
 * @returns a char pointer to the beginning of the next value, `NULL` if on final value, or `(-1)` on fail (getError)
 */
char * CSV_setValue(char * startOfValue, char ** valuePointer)
{
	char * nextQuote = CSV_valueContainedInQuotes(startOfValue);
	if(nextQuote < 0) return (char*)-1;

	//	Ignore leading whitespace
	while(isblank(startOfValue[0])) startOfValue++;
	/**************************/

	//	Length from the new startOfValue to the real null terminator of the string
	//	(As in, not counting the ones we will add)
	size_t fullLen = strlen(startOfValue);

	//	Value was contained within quotes, which now must be nullified.
	if(nextQuote)
	{
		//	startOfValue[0] should be '"' right now. Nullify it.
		//	Technically this is uneccessary, but I said I would do it in the documentation.
		startOfValue[0] = '\0';
		//	Nullify the ending quote
		(*nextQuote) = '\0';
		//	The "real" beginning of the value starts here:
		startOfValue++;
		fullLen--;

		//	Unfortunately, because we use quotes to surround a value which may contain a comma,
		//	there now may be escaped quotes or backslashes in our string, which we need to de-escape.
		//	For simplicity, only '\\' and '\"' are recognized. Any other backslash is erroneous.
		char * setter, * looker;
		for(setter = looker = startOfValue; (*looker); setter++, looker++)
		{
			//	Because the starting and ending quotes are already nullified, and we already checked
			//	to ensure proper escaping of quotes, we should really only need to worry about this:
			if((*looker) == '\\')
			{
				switch(*(looker+1))
				{
					case '\\':
					case '\"':
						(*setter) = *(looker+1);
						looker++;
						break;
					//	Erroneous backslash:
					default:
						setError(ERR_MESG,"The CSV reader only acknowledges '\\\\' and '\\\"' while inside a quoted string.");
						return (char*)(-1);
				}
			}
			else
			{
				(*setter) = (*looker);
			}
		}
		//	Finish setting the final string by terminating it (which is not done in the loop, as looker becomes null and the loop terminates).
		(*setter) = '\0';
	}
	/*************************************************/

	//	At this point, only need to nullify trailing whitespace and determine next value start.

	char * nextValueStart = NULL;
	char * nextComma = strchr(startOfValue,',');
	if(!nextComma)
	{
		//	No next value
		nextValueStart = NULL;
		//	fullLen relies on the actual null terminator of the string, which is now the endpoint for this value.
		while(isblank(startOfValue[fullLen-1]) && fullLen > 0)
		{
			startOfValue[fullLen-1] = '\0';
			fullLen--;
		}
	}
	else
	{
		//	There is a next value
		nextValueStart = nextComma+1;
		(*nextComma) = '\0';

		char * blankTest = nextComma-1;
		while(isblank(*blankTest) && blankTest > startOfValue)
		{
			*blankTest = '\0';
			blankTest--;
		}
	}
	/******************************************************/

	if(valuePointer) *valuePointer = startOfValue;
	return nextValueStart;
}


csv_t CSV_loadFromFile(const char * filename)
{
	if(!filename) ERR_NULLP_RET_NULL;
	FILE * csvFile = fopen(filename,"r");
	if(!csvFile) ERR_NOFIL_RET_NULL;
	csv_t csv = calloc(1,sizeof(struct csv));
	if(!csv) ERR_NOMEM_RET_NULL;

	while(!feof(csvFile))
	{
		csv->values = realloc(csv->values,sizeof(char**)*(1 + csv->lineCount));
		if(!csv->values) ERR_NOMEM_RET_NULL;
		csv->values[csv->lineCount] = NULL;

		csv->valueCounts = realloc(csv->valueCounts,sizeof(int)*(1 + csv->lineCount));
		if(!csv->valueCounts) ERR_NOMEM_RET_NULL;
		csv->valueCounts[csv->lineCount] = 0;

		csv->lineStrings = realloc(csv->lineStrings,sizeof(char*)*(1 + csv->lineCount));
		if(!csv->lineStrings) ERR_NOMEM_RET_NULL;

		char * currentLine = NULL;
		fscanf(csvFile," %m[^\n\r] ",&currentLine);
		if(!currentLine) ERR_NOMEM_RET_NULL;

		csv->lineStrings[csv->lineCount] = currentLine;
		//fprintf(stderr,"csv line %d: %s\n",csv->lineCount+1,currentLine);

		//	At this time, the string exists in lineStrings.
		//	We need to parse the string and set the next elements of this line's value array.
		//	We are currently ignoring leading whitespace and any fully-surrounding quotes

		char * startOfValue = currentLine;
		char * nextValue = currentLine;
		for(int i=0; nextValue; i++)
		{
			csv->values[csv->lineCount] = realloc(csv->values[csv->lineCount],(i+1)*sizeof(char*));
			if(!(csv->values[csv->lineCount])) ERR_NOMEM_RET_NULL;

			nextValue = CSV_setValue(startOfValue,&startOfValue);
			if(nextValue < 0) return NULL;
			csv->values[csv->lineCount][i] = startOfValue;
			startOfValue = nextValue;
			csv->valueCounts[csv->lineCount]++;
		}

		/********************************************************************************/

		csv->lineCount++;
	}

	fclose(csvFile);
	return csv;
}

int CSV_lineCount(csv_t csv)
{
	if(!csv) ERR_NULLP_RET_X(-1);
	return csv->lineCount;
}

const char * CSV_getValueAt(csv_t csv, uint line, uint index)
{
	if(!csv || !csv->lineCount || !csv->values || !csv->values[line] || !csv->values[line][index])
		ERR_NULLP_RET_NULL;
	return csv->values[line][index];
}

void CSV_fprint(FILE * fp, csv_t csv)
{
	if(!csv) return;
	for(int i=0; i<csv->lineCount; i++)
	{
		for(int j=0; j<csv->valueCounts[i]; j++)
		{
			if(strchr(csv->values[i][j],','))
			{
				//fprintf(fp,"\"%s\"%c",csv->values[i][j],(j==csv->valueCounts[i]-1)?'\n':',');
				fprintf(fp,"\"%s\"%c",CSV_getValueAt(csv,i,j),(j==csv->valueCounts[i]-1)?'\n':',');
			}
			else
			{
				//fprintf(fp,"%s%c",csv->values[i][j],(j==csv->valueCounts[i]-1)?'\n':',');
				fprintf(fp,"%s%c",CSV_getValueAt(csv,i,j),(j==csv->valueCounts[i]-1)?'\n':',');
			}
		}
	}
}

void CSV_print(csv_t csv)
{
	CSV_fprint(stdout,csv);
}

void CSV_free(csv_t csv)
{
	if(!csv) return;

	for(int i=0; i<csv->lineCount; i++)
	{
		//	Free the space allocated for each char* offset
		free(csv->values[i]);
		//	Free the entire string for the line
		free(csv->lineStrings[i]);
	}
	//	Free the allocated space for all lines
	free(csv->values);
	//
	free(csv->valueCounts);
	// Frees the line string container
	free(csv->lineStrings);
	//	Free the space allocated for the csv itself
	free(csv);
}