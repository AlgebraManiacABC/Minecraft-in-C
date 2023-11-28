#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "debug.h"

typedef unsigned int uint;

typedef struct csv *csv_t;

NODISCARD
/**
 * Loads a whole CSV file from the given filename and stores its information in the returned csv object.
 *
 * @param filename the filename of the csv file
 * @returns a csv object with the data, NULL on failure
 */
csv_t CSV_loadFromFile(const char * filename);

NODISCARD
/**
 * Returns the number of lines in the given csv.
 *
 * @param csv the csv object
 * @returns an integer number of lines, which may be 0, but will return -1 on failure.
 */
int CSV_lineCount(csv_t csv);

NODISCARD
/**
 * Returns the value located at the specified line and column index of the csv.
 * This string should not be freed by the user.
 *
 * @param csv the csv object
 * @param line the line number, starting at 0
 * @param index the column index, starting at 0
 * @returns a string value if successful, NULL on failure.
 */
const char * CSV_getValueAt(csv_t csv, uint line, uint index);

void CSV_fprint(FILE * fp, csv_t csv);
void CSV_print(csv_t csv);

/**
 * Deallocates memory reserved for the csv object.
 *
 * @param csv the csv object to free
 */
void CSV_free(csv_t csv);