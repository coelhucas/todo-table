#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>
#include "http.h"

typedef struct {
    char *response;
    size_t size;
} MemoryStruct;

size_t writeCallback(void *contents, size_t size, size_t nMembers, void *rawMemory) {
    size_t realSize = size * nMembers;
    MemoryStruct *memory = (MemoryStruct *)rawMemory;
    char *newMemory = realloc(memory->response, realSize + memory->size + 1);

    if (newMemory == NULL) {
        return 0;
    }

    memory->response = newMemory;
    memcpy(memory->response + memory->size, contents, realSize);
    memory->size += realSize;
    memory->response[memory->size] = '\0';
    return realSize;
}

http_err httpGet(const char *url, char **result) {
    curl_global_init(CURL_GLOBAL_ALL);
    CURL *curl = curl_easy_init();
    MemoryStruct memory;
    memory.response = calloc(1, sizeof(char));

    if (memory.response == NULL) {
        curl_easy_cleanup(curl);
        return http_err_write_error;
    }

    memory.size = 0;
    curl_easy_setopt(curl, CURLOPT_URL, url);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&memory);
    CURLcode err = curl_easy_perform(curl);

    if (err != CURLE_OK) {
        free(memory.response);
        printf("(ERROR http.c) CURL ERROR: %d.\n", err);
        curl_easy_cleanup(curl);
        switch(err) {
            case CURLE_COULDNT_RESOLVE_HOST: return http_err_host_error;
            case CURLE_COULDNT_RESOLVE_PROXY: return http_err_proxy_error;
            default: return http_err_request_failed;
        }
    }

    *result = memory.response;
    curl_easy_cleanup(curl);
    return http_err_ok;
}
