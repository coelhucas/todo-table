#ifndef table_h
#define table_h
#include <stdlib.h>

/*
 * Table represents a virtual table structure, used by drawTable to return a formatted 
 * version of it.
 * 
 * headers      - Double pointer string holding each Table header, which will be drawn 
 *                on the first line
 * headersCount - Length of the provided headers value.
 * rows         - Triple pointer string holding each row (a char **row must be the same
 *                size as headersCount)
 * rowsCount    - Length of the Table rows field.
 * columnsWidth - Responsible for holding each column width. It's calculated
 *                dynamically padding the smaller words of each cell in a 
 *                column.
 */ 
typedef struct {
    char **headers;
    size_t headersCount;
    char ***rows;
    size_t rowsCount;
    size_t *columnsWidth;
} Table;

/*
 * Draw a Table with the following format:
 * ╭─────────┬────┬────────────────────┬────────────╮
 * │ User ID │ ID │ Title              │ Completed? │
 * ├─────────┼────┼────────────────────┼────────────┤
 * │ 1       │ 1  │ delectus aut autem │ No         │
 * ╰─────────┴────┴────────────────────┴────────────╯
 * Where the first row is made from the table->headers and the others from table->rows.
 */
void drawTable(Table *table);
#endif
