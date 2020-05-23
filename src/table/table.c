#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "table.h"

const size_t CELL_SPACING = 2;

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