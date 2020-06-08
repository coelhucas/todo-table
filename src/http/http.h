#ifndef http_h
#define http_h

typedef enum {
    http_err_ok = 0,
    http_err_request_failed = 1,
    http_err_host_error = 2,
    http_err_proxy_error = 3,
    http_err_write_error = 4,

} http_err;

/*
 * httpGet attempts to make a get request based on a provided (char *)
 * and writes the result content onto a (char **)
 *
 * url    - String from where we should point our request
 * result - Uninitialized double pointer string where the obtained content
 *          will be written on success.
 *
 * Returns a http_err. It returns a `http_err_write_error` if the
 * response could not be allocated, a `http_err_ok` on success, a
 * proxy or host error based on the curl_easy_perform result. In
 * case of any other curl error, it returns a generic `http_err_request_failed`.
 */
http_err httpGet(const char *url, char **result);

#endif
