#ifndef http_h
#define http_h
#include <curl/curl.h>

/* 
 * httpGet attempts to make a get request based on a provided (char *)
 * and writes the result content onto a (char **)
 * 
 * url    - String from where we should point our request
 * result - Uninitialized double pointer string where the obtained content
 *          will be written on success.
 * 
 * Returns a CURLcode. It may be a `CURLE_WRITE_ERROR` if the
 * response could not be allocated, a `CURLE_OK` on success or
 * any other `CURLE_*` based on the easy_perform result.
 */
CURLcode httpGet(const char *url, char **result);

#endif
