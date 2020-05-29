#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "table.h"

const size_t CELL_SPACING = 2;

/*
 * calculateWidths attempts to calculate the correct width for each table column
 * based on a received table.
 *
 * table           - Source Table to get rows and headers to properly calculate
 *                   each column cell size.
 * outColumnsWidth - Where to write columns widths after calculation.
 *
 * In case of an allocation error, it returns a `table_err_allocation_failed`,
 * which is exposed for the consumer of table.
 */
void calculateWidths(Table *table, size_t *outColumnsWidth) {
    for (size_t i = 0; i < table->headersCount; i++) {
        size_t cellWidth = strlen(table->headers[i]);

        for (size_t j = 0; j < table->rowsCount; j++) {
            size_t size = strlen(table->rows[j][i]);
            if (size > cellWidth) {
                cellWidth = size;
            }
        }

        outColumnsWidth[i] = cellWidth;
    }
}

/*
 * padString attempts to add spacing to a string until it
 * reaches the desired size.
 *
 * string      - A string to receive the padding
 * desiredSize - A number representing the desired size.
 * outResult   - Where to place the result string.
 *
 * Returns a `table_err`, which becames `table_err_invalid_input`
 * in case of an invalid input, `table_err_allocation_failed` or
 * a `table_err_ok` if it's everything okay.
 */
table_err padString(char *string, size_t desiredSize, char **outResult) {
    if (string == NULL) {
        return table_err_invalid_input;
    }

    if (strlen(string) > desiredSize) {
        return table_err_invalid_input;
    }

    char *result = calloc(desiredSize + 1, sizeof(char));

    if (result == NULL) {
        return table_err_allocation_failed;
    }

    strcpy(result, string);
    for (size_t i = strlen(string); i < desiredSize; i++) {
        result[i] = ' ';
    }

    *outResult = result;
    return table_err_ok;
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
    if (c == NULL) {
        return "";
    }

    size_t cSize = strlen(c);
    size_t finalSize = cSize * times;
    char *result = calloc(finalSize + 1, sizeof(char));

    if (result == NULL) {
        return NULL;
    }

    for (size_t i = 0; i < times; i++) {
        snprintf(result + (i * cSize), finalSize, "%s", c);
    }

    return result;
}

/*
 * makeLine prints a line based on a char to be repeated, a separator
 * and a end string.
 *
 * str          - A string to be repeated based on the table cell size.
 * separator    - A string to be drawn between cells.
 * end          - A string to be drawn at the end of a line.
 * columnsWidth - The list containing the width for each column cell.
 * headersCount - Amount of headers of the current table.
 */
table_err makeLine(char *str, char *separator, char *end, size_t *columnsWidth, size_t headersCount) {
    for (size_t i = 0; i < headersCount; i++) {
        char *tableLine = repeat(str, columnsWidth[i] + CELL_SPACING);

        if (tableLine == NULL) {
            return table_err_allocation_failed;
        }

        printf("%s", tableLine);
        free(tableLine);

        if (i < headersCount - 1 && headersCount != 1) {
            printf("%s", separator);
        }
    }

    printf("%s\n", end);
    return table_err_ok;
}

table_err drawTable(Table *table) {
    size_t columnsWidth[table->headersCount];
    calculateWidths(table, columnsWidth);

    printf("╭");
    table_err err = makeLine("─", "┬", "╮", columnsWidth, table->headersCount);

    if (err != table_err_ok) {
        return err;
    }

    printf("│");

    for (size_t i = 0; i < table->headersCount; i++) {
        char *cellContent = NULL;
        err = padString(table->headers[i], (size_t)columnsWidth[i], &cellContent);

        if (err != table_err_ok) {
            return err;
        }

        printf(" %s │", cellContent);
        free(cellContent);

        if (i == table->headersCount - 1) {
            printf("\n");
        }
    }

    if (table->headersCount > 0) {
        printf("├");
    }

    err = makeLine("─", "┼", table->headersCount > 0 ? "┤" : "│", columnsWidth, table->headersCount);

    if (err != table_err_ok) {
        return table_err_allocation_failed;
    }

    for (size_t i = 0; i < table->rowsCount; i++) {
        printf("│");
        for (size_t j = 0; j < table->headersCount; j++) {
            char *cellContent = NULL;
            err = padString(table->rows[i][j], (size_t)columnsWidth[j], &cellContent);

            if (cellContent == NULL) {
                return table_err_allocation_failed;
            }

            printf(" %s │", cellContent);
            free(cellContent);
        }
        printf("\n");
    }
    printf("╰");
    err = makeLine("─", "┴", "╯", columnsWidth, table->headersCount);

    if (err != table_err_ok) {
        return table_err_allocation_failed;
    }

    return table_err_ok;
}
