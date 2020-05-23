#ifndef table_h
#define table_h
#include <stdlib.h>

typedef struct {
    char **headers;
    char ***rows;
    size_t headersCount;
    size_t rowsCount;
    size_t *columnsWidth;
} Table;

void drawTable(Table *table);
#endif