#ifndef table_h
#define table_h
#include <stdlib.h>

typedef char *TABLE_DATA_ITEM;
typedef TABLE_DATA_ITEM *TABLE_DATA_ROW;

typedef enum {
    table_err_ok = 0,
    table_err_allocation_failed = 1,
    table_err_invalid_input = 2
} table_err;

/*
 * Table represents a virtual table structure, used by drawTable to return a formatted
 * version of it.
 *
 * headers      - TABLE_DATA_ROW holding each Table TABLE_DATA_ITEM, which will be drawn
 *                on the first line
 * headersCount - Length of the provided headers value.
 * rows         - TABLE_DATA_ITEM list holding each row (each TABLE_DATA_ITEM must have
 *                the same size of headersCount).
 * rowsCount    - Length of the Table rows list.
 * columnsWidth - Responsible for holding each column width. It's calculated
 *                dynamically padding the smaller words of each cell in a
 *                column.
 */
typedef struct {
    TABLE_DATA_ROW headers;
    size_t headersCount;
    TABLE_DATA_ROW *rows;
    size_t rowsCount;
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
table_err drawTable(Table *table);
#endif
