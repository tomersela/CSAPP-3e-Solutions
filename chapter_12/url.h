#ifndef __URL_H__
#define __URL_H__

#include <stdio.h>
#include <stdlib.h>

typedef enum
{
    VALID_URL,
    INVALID_SCHEMA_ERR,
    INVALID_HOST_ERR,
    HOST_BUFFER_TOO_SHORT_ERROR,
    INVALID_PORT_ERROR,
    PORT_BUFFER_TOO_SHORT_ERROR,
    PATH_BUFFER_TOO_SHORT_ERROR,
} http_url_error;

char *http_url_parse_error_msg(http_url_error error);

http_url_error parse_http_url(const char *url, char *hostbuf, int hostbuflen,
                              char *portbuf, int portbuflen, char *pathbuf, int pathbuflen);

#endif /* __URL_H__ */
