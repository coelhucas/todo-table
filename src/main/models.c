#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <json-c/json.h>
#include "models.h"

void freeTODOList(TODOEntry **list, size_t listLength) {
    for (size_t i = 0; i < listLength; i++) {
        if (list[i] != NULL) {
            free((char *)list[i]->title);
        }
        free(list[i]);
    }
    free(list);
}

void freeRows(char ***rows, size_t amount) {
    for (size_t i = 0; i < amount; i++) {
        free(rows[i][0]);
        free(rows[i][1]);
        free(rows[i]);
    }
}

/*
 * JSON_GETTER is a macro responsible for expanding a function to get values from
 * a json object.
 *
 * c_type        - Type to be returned.
 * name          - Name of the type to be used in expanded function signature.
 * json_type     - Name of the type in `json-c`.
 * default_value - Default value to be returned in case of inconsistencies/errors.
 *
 * The expandend function should return the `default_value` or a `c_type` contained
 * in a json object key.
 */
#define JSON_GETTER(c_type, name, json_type, defaultValue) c_type getJson##name(json_object *jsonObj, const char *key) { \
    json_object *jsonWrapper = json_object_object_get(jsonObj, key); \
    if (json_object_get_type(jsonWrapper) != json_type_##json_type) { \
        return defaultValue; \
    } \
    return json_object_get_##json_type(jsonWrapper); \
}

JSON_GETTER(int, Int, int, 0);
JSON_GETTER(bool, Bool, boolean, false);
JSON_GETTER(const char *, String, string, NULL);

/*
 * parseTODOObject attempts to parse an individual TODOEntry based on
 * a json_object value and a TODOEntry out.
 *
 * entry - Json object to be parsed.
 * todo  - `TODOEntry **` where the parsed TODOEntry will be written
 *
 * Returns a `json_err_alloc_failed` if it cannot allocate a TODOEntry.
 * Otherwise, returns a `json_err_ok`.
 */
json_err parseTODOObject(json_object *entry, TODOEntry **todo) {
    char *entryTitle = strdup(getJsonString(entry, "title"));

    if (entryTitle == NULL) {
        return json_err_alloc_failed;
    }

    TODOEntry *todoEntry = calloc(1, sizeof(TODOEntry));

    todoEntry->userID = getJsonInt(entry, "userId");
    todoEntry->ID = getJsonInt(entry, "id");
    todoEntry->title = entryTitle;
    todoEntry->completed = getJsonBool(entry, "completed");
    *todo = todoEntry;
    return json_err_ok;
}

json_err parseTODOList(char *json, size_t jsonSize, size_t *outListLength, TODOEntry ***outEntries) {
    struct json_tokener *tok = json_tokener_new();
    enum json_type type;
    json_object *jsonObj = json_tokener_parse_ex(tok, json, jsonSize);
    type = json_object_get_type(jsonObj);

    if (jsonObj == NULL) {
        json_tokener_free(tok);
        return json_err_parse_failed;
    }

    if (json_tokener_get_error(tok) != json_tokener_success &&
        json_tokener_get_error(tok) != json_tokener_continue) {
            json_object_put(jsonObj);
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
        json_object_put(jsonObj);
        json_tokener_free(tok);
        return json_err_alloc_failed;
    }

    if (type == json_type_object) {
        json_err err = parseTODOObject(jsonObj, &entries[0]);
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
        json_err err = parseTODOObject(entry, &entries[i]);

        if (err != json_err_ok) {
            freeTODOList(entries, listLength);
            json_object_put(jsonObj);
            json_tokener_free(tok);
            return err;
        }
    }
    json_object_put(jsonObj);
    json_tokener_free(tok);
    *outListLength = listLength;
    *outEntries = entries;
    return json_err_ok;
}
