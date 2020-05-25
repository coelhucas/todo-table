#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "table.h"

const size_t CELL_SPACING = 2;

/*
 * calculateWidths attempts to calculate the correct width for each table column
 * based on a received table.
 * 
 * In case of an allocation error, it exits the application.
 */
void calculateWidths(Table *table) {
    table->columnsWidth = calloc(table->headersCount, sizeof(size_t));

    if (table->columnsWidth == NULL) {
        exit(1);
    }

    for (size_t i = 0; i < table->headersCount; i++) {
        size_t cellWidth = strlen(table->headers[i]);

        for (size_t j = 0; j < table->rowsCount; j++) {
            size_t size = strlen(table->rows[j][i]);
            if (size > cellWidth) {
                cellWidth = size;
            }
        }

        table->columnsWidth[i] = cellWidth;
    }
}

/*
 * padString attempts to add spacing to a string until it
 * reaches the desired size.
 * 
 * string      - A string to receive the padding
 * desiredSize - A number representing the desired size.
 * 
 * Returns the string with the desired padding in spaces, added
 * to the right of the string. Returned values must be manually freed by the 
 * caller once they are no longer needed.
 */
char *padString(char *string, size_t desiredSize) {
    char *result = calloc(desiredSize + 1, sizeof(char));
    if (result == NULL) {
        return NULL;
    }

    strcpy(result, string);
    for (size_t i = strlen(string); i < desiredSize; i++) {
        result[i] = ' ';
    }

    return result;
}

/*
 * repeat attempts to return a string repeated N times.
 * 
 * c     - String to be repeated.
 * times - Amount of times to repeat the string.
 * 
 * Returns NULL in case allocation fails. Otherwise, returns the generated string. 
 * The result of this function must be manually freed after it is no longer used.
 */
char *repeat(char *c, size_t times) {
    size_t cSize = strlen(c);
    char *result = calloc((cSize * times) + 1, sizeof(char));
    
    if (result == NULL) {
        return NULL;
    }

    for (size_t i = 0; i < times; i++) {
        sprintf(result + (i * cSize), "%s", c);
    }

    return result;
}

/*
 * makeLine prints a line based on a char to be repeated, a separator
 * and a end string.
 * 
 * c         - A string to be repeated based on the table cell size.
 * separator - A string to be drawn between cells.
 * end       - A string to be drawn at the end of a line.
 * table     - The Table used as reference for the line sizes.
 */
void makeLine(char *c, char *separator, char *end, Table *table) {
    for (size_t i = 0; i < table->headersCount; i++) {
        char *tableLine = repeat(c, table->columnsWidth[i] + CELL_SPACING);
        printf("%s", tableLine);
        free(tableLine);

        if (i < table->headersCount - 1 && table->headersCount != 1) {
            printf("%s", separator);
        }
    }

    printf("%s\n", end);
}

void drawTable(Table *table) {
    calculateWidths(table);

    printf("╭");
    makeLine("─", "┬", "╮", table);
    printf("│");

    for (size_t i = 0; i < table->headersCount; i++) {
        char *cellContent = padString(table->headers[i], table->columnsWidth[i]);
        printf(" %s │", cellContent);
        free(cellContent);

        if (i == table->headersCount - 1) {
            printf("\n");
        }
    }

    if (table->headersCount > 0) {
        printf("├");
    }

    makeLine("─", "┼", table->headersCount > 0 ? "┤" : "│", table);
    
    for (size_t i = 0; i < table->rowsCount; i++) {
        printf("│");
        for (size_t j = 0; j < table->headersCount; j++) {
            printf(" %s │", padString(table->rows[i][j], table->columnsWidth[j]));
        }
        printf("\n");
    }
    printf("╰");
    makeLine("─", "┴", "╯", table);
    free(table->columnsWidth);
}
