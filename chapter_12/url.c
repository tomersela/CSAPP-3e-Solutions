#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "url.h"

#define HTTP_SCHEME "http://"
#define HTTP_SCHEME_LEN 7
#define ISDIGIT(x) isdigit((unsigned char)(x))

/*
    Naive implementation of URL parsing (No hostname or path structure validation)
*/
http_url_error parse_http_url(const char *url, char *hostbuf, const int hostbuflen, char *portbuf, const int portbuflen,
                              char *pathbuf, const int pathbuflen) {
    int i, hostbuf_i = 0, portbuf_i = 0, pathbuf_i = 0;

    if (strncasecmp(url, HTTP_SCHEME, 5)) {
        return INVALID_SCHEMA_ERR;
    }
    
    // parse host
    hostbuf_i = 0;
    for (i = HTTP_SCHEME_LEN; url[i] && url[i] != ':' && url[i] != '/' && hostbuf_i < hostbuflen; i++, hostbuf_i++) {
        hostbuf[hostbuf_i] = url[i];
    }
    if (hostbuf_i >= hostbuflen) return HOST_BUFFER_TOO_SHORT_ERROR; // not enough space in host buffer
    if (i == HTTP_SCHEME_LEN) return INVALID_HOST_ERR; // no host specified
    hostbuf[hostbuf_i] = '\0'; // terminate host string

    // parse port
    if (url[i] == ':') { // the url might have a port
        // We have a colon delimiting the hostname. It could mean that a port number is following it
        if (ISDIGIT(url[++i])) {  // A port number
            portbuf_i = 0;
            for (; url[i] && url[i] != '/' && portbuf_i < portbuflen; i++, portbuf_i++) {
                if (!ISDIGIT(url[i])) return INVALID_PORT_ERROR;
                portbuf[portbuf_i] = url[i];
            }
            if (portbuf_i >= portbuflen) return PORT_BUFFER_TOO_SHORT_ERROR; // not enough space in port buffer
            portbuf[portbuf_i] = '\0'; // terminate port string
        }
    } else {
        portbuf[0] = '\0';
    }

    // parse path
    if (url[i] == '/') {
        for (; url[i] != '\0'; i++ && pathbuf_i < pathbuflen, pathbuf_i++) {
            pathbuf[pathbuf_i] = url[i];
        }
        if (pathbuf_i >= pathbuflen) return PATH_BUFFER_TOO_SHORT_ERROR; // not enough space in path buffer
        pathbuf[pathbuf_i] = '\0'; // terminate path string
    } else {
        pathbuf[0] = '\0';
    }

    return VALID_URL;
}

char *http_url_parse_error_msg(http_url_error error) {
    switch (error)
    {
    case VALID_URL:
        return "URL is valid";
    case INVALID_SCHEMA_ERR:
        return "Invalid URL schema";
    case INVALID_HOST_ERR:
        return "Invalid hostname";
    case HOST_BUFFER_TOO_SHORT_ERROR:
        return "Not enough buffer space for storing the parsed hostname";
    case INVALID_PORT_ERROR:
        return "Invalid port";
    case PORT_BUFFER_TOO_SHORT_ERROR:
        return "Not enough buffer space for storing the parsed port";
    case PATH_BUFFER_TOO_SHORT_ERROR:
        return "Not enough buffer space for storing the parsed path";

    default:
        fprintf(stderr, "Undefined url parsing error");
        exit(1);
    }
}
