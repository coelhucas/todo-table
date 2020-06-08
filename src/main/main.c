#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <json-c/json.h>
#include <table/table.h>
#include <http/http.h>
#include "models.h"

int main() {
    char *data;
    http_err requestErr = httpGet("https://jsonplaceholder.typicode.com/todos", &data);

    if (requestErr != http_err_ok) {
        printf("Error: (HTTP Request) get request error: %d", requestErr);
        return 1;
    }

    TODOEntry **outEntries = NULL;
    size_t outListLength;
    json_err err = parseTODOList(data, strlen(data), &outListLength, &outEntries);

    if (err != json_err_ok) {
        printf("Error: (Json Error ID) %d.\n", err);
        return 1;
    }

    TABLE_DATA_ITEM headers[4] = { "User ID", "ID", "Title", "Completed?" };
    TABLE_DATA_ROW rows[outListLength];

    for (size_t i = 0; i < outListLength; i++) {
        TODOEntry *e = outEntries[i];
        TABLE_DATA_ITEM userID = calloc(11, sizeof(char));

        if (userID == NULL) {
            printf("Error: (UserID) Could not allocate memory.\n");
            freeRows(rows, i);
            freeTODOList(outEntries, outListLength);
            return 1;
        }

        TABLE_DATA_ITEM ID = calloc(11, sizeof(char));

        if (ID == NULL) {
            free(userID);
            printf("Error: (ID) Could not allocate memory.\n");
            freeRows(rows, i);
            freeTODOList(outEntries, outListLength);
            return 1;
        }

        snprintf(userID, 10, "%d", e->userID);
        snprintf(ID, 10, "%d", e->ID);
        TABLE_DATA_ROW row = calloc(4, sizeof(TABLE_DATA_ITEM));

        if (row == NULL) {
            free(ID);
            free(userID);
            freeRows(rows, i);
            freeTODOList(outEntries, outListLength);
            return 1;
        }

        row[0] = userID;
        row[1] = ID;
        row[2] = (char *)e->title;
        row[3] = e->completed ? "Yes" : "No";
        rows[i] = row;
    }

    Table table = { headers: headers, headersCount: 4, rows: rows, rowsCount: outListLength };
    table_err drawErr = drawTable(&table);

    free(data);
    freeTODOList(outEntries, outListLength);
    freeRows(rows, outListLength);
    if (drawErr != table_err_ok) {
        printf("Error: (drawTable) Could not draw. err %d.\n", drawErr);
        return 1;
    }
    return 0;
}
