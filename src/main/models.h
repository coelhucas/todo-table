#ifndef models_h
#define models_h
#include <stdlib.h>

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

/*
 * Utility function to free memory allocated for each dynamic
 * value from a row and then the row itself.
 *
 * rows   - A string containing the rows to be freed.
 * amount - A number representing until when we still free the rows
 *          (for cases where an error occurs before allocating all existing rows)
 *
 * Be aware that we are considering here that the first two elements of each row
 * were allocated and we're freeing them.
 */
void freeRows(char ***rows, size_t amount);

/*
 * Utility function to free memory allocated for each dynamic
 * TODOEntry from a TODOEntry list and then the list itself.
 *
 * list       - List containing each allocated TODOEntry.
 * listLength - A number representing the size of the list.
 */
void freeTODOList(TODOEntry **list, size_t listLength);

/*
 * parseTODOList attempts to parse a json containing a TODO list.
 *
 * json          - A string containing the json contents.
 * jsonSize      - A number representing the size of json.
 * outListLength - A number which will receive the final TODO list size
 *                  after parsing the list.
 * outEntries    - Responsible for receiving a `TODOEntry ***` containing the
 *                 resulted todo entries list.
 *
 * Returns a `json_err_aloc_failed` if any of the allocations fail. It also
 * can returns a `json_err_parse-failed` if there's a error from json_tokener or
 * the parsed json object became null.
 *
 * If the received json is not an object or an array, it can also return a `json_err_invalid_type`.
 * Otherwise, returns a `json_err_ok`.
 */
json_err parseTODOList(char *json, size_t jsonSize, size_t *outListLength, TODOEntry ***outEntries);
#endif

