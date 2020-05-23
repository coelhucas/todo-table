#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <json-c/json.h>
#include <table/table.h>
#include "http.h"

typedef struct {
    int userID;
    int ID;
    const char *title;
    bool completed;
} TODOEntry;

typedef enum {
    json_err_ok = 0,
    json_err_parse_failed = 1,
    json_err_alloc_failed = 2,
    json_err_invalid_type = 3
} json_err;

char *jsonErrors[4] = { "Ok", "Parse failed", "Allocation failed", "Invalid JSON type" };

#define JSON_GETTER(c_type, name, json_type, default_value) c_type getJson##name(json_object *jsonObj, const char *key) { \
    json_object *jsonWrapper = json_object_object_get(jsonObj, key); \
    if (json_object_get_type(jsonWrapper) != json_type_##json_type) { \
        return default_value; \
    } \
    return json_object_get_##json_type(jsonWrapper); \
}

JSON_GETTER(int, Int, int, 0);
JSON_GETTER(bool, Bool, boolean, false);
JSON_GETTER(const char *, String, string, NULL);

void freeTODOList(TODOEntry **list, size_t listLength) {
    for (size_t i = 0; i < listLength; i++) {
        if (list[i] != NULL) {
            free((char *)list[i]->title);
        }
        free(list[i]);
    }
    free(list);
}

json_err parseTODO(json_object *entry, TODOEntry **todo) {
    char *entryTitle = strdup(getJsonString(entry, "title"));

    if (entryTitle == NULL) {
        return json_err_alloc_failed;
    }

    TODOEntry *todoEntry = calloc(1, sizeof(TODOEntry));

    if (todoEntry == NULL) {
        free(entryTitle);
        return json_err_alloc_failed;
    }

    todoEntry->userID = getJsonInt(entry, "userId");
    todoEntry->ID = getJsonInt(entry, "id");
    todoEntry->title = entryTitle;
    todoEntry->completed = getJsonBool(entry, "completed");
    *todo = todoEntry;
    return json_err_ok;
}

json_err parseTODOS(char *json, size_t jsonSize, size_t *outListLength, TODOEntry ***outEntries) {
    struct json_tokener *tok = json_tokener_new();
    enum json_type type;
    json_object *jsonObj = json_tokener_parse_ex(tok, json, jsonSize);
    type = json_object_get_type(jsonObj);

    if (jsonObj == NULL || json_tokener_get_error(tok) != json_tokener_success &&
        json_tokener_get_error(tok) != json_tokener_continue) {
            json_tokener_free(tok);
            return json_err_parse_failed;
        }

    if (type != json_type_object && type != json_type_array) {
        json_object_put(jsonObj);
        json_tokener_free(tok);
        return json_err_invalid_type;
    }

    size_t listLength = type == json_type_object ? 1 : json_object_array_length(jsonObj);
    TODOEntry **entries = calloc(listLength, sizeof(TODOEntry *));

    if (entries == NULL) {
        return json_err_alloc_failed;
    }

    if (type == json_type_object) {
        json_err err = parseTODO(jsonObj, &entries[0]);
        json_object_put(jsonObj);
        json_tokener_free(tok);
        if (err != json_err_ok) {
            freeTODOList(entries, listLength);
            return err;
        }
        *outListLength = listLength;
        *outEntries = entries;
        return err;
    }

    for (size_t i = 0; i < listLength; i++) {
        json_object *entry = json_object_array_get_idx(jsonObj, i);
        json_err err = parseTODO(entry, &entries[i]);

        if (err != json_err_ok) {
            freeTODOList(entries, listLength);
            json_object_put(jsonObj);
            json_tokener_free(tok);
            return err;
        }
    }
    *outListLength = listLength;
    *outEntries = entries;
    return json_err_ok;
}

void freeRows(char ***rows, size_t amount) {
    for (size_t i = 0; i < amount; i++) {
        free(rows[i][0]);
        free(rows[i][1]);
        free(rows[i]);
    }
}

int main() {
    curl_global_init(CURL_GLOBAL_ALL);
    char *data;
    CURLcode curlErr = httpGet("https://jsonplaceholder.typicode.com/todos/1", &data);
    
    if (curlErr != CURLE_OK) {
        printf("Curl error: %d", curlErr);
        return 1;
    }

    size_t outListLength;
    TODOEntry **outEntries = NULL;
    json_err err = parseTODOS(data, strlen(data), &outListLength, &outEntries);

    if (err != json_err_ok) {
        printf("Json Error (%d): %s.\n", err, jsonErrors[err]);
        return 1;
    }
    char *headers[4] = { "User ID", "ID", "Title", "Completed?" };

    char **rows[outListLength];
    
    for (size_t i = 0; i < outListLength; i++) {
        TODOEntry *e = outEntries[i];
        char *userID = calloc(11, sizeof(char));

        if (userID == NULL) {
            printf("Error: (UserID) Could not allocate memory.\n");
            freeRows(rows, i);
            freeTODOList(outEntries, outListLength);
            return 1;
        }

        char *ID = calloc(11, sizeof(char));

        if (ID == NULL) {
            free(userID);
            printf("Error: (ID) Could not allocate memory.\n");
            freeRows(rows, i);
            freeTODOList(outEntries, outListLength);
            return 1;
        }

        sprintf(userID, "%d", e->userID);
        sprintf(ID, "%d", e->ID);
        char **row = calloc(4, sizeof(char *));

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
    drawTable(&table);
    freeTODOList(outEntries, outListLength);
    freeRows(rows, outListLength);
    return 0;
}