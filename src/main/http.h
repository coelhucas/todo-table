#ifndef http_h
#define http_h
#include <curl/curl.h>

CURLcode httpGet(const char *url, char **result);

#endif