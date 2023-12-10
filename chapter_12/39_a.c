#include "csapp.h"
#include "url.h"

#define HTTP_DEFAULT_PORT "80"
#define LOG_FILE "proxy.log"
#define BLOCKED_FILE "blocked.conf"
#define CONTENT_LENGTH_HEADER_PREFIX "Content-Length:"
#define CONTENT_LENGTH_HEADER_PREFIX_LEN 15
#define VIA_HEADER "Via"
#define CSAPP_PROXY "csapp-proxy"

static void serve(int connfd, int logfd);
static void logline(int logfd, const char *format, ...);
char** read_lines(int fd, int *line_count);
int is_blocked(char *url);

static char **blocked_list;
static int blocked_cnt;

int main(int argc, char* argv[]) {
    char *port;
    int listenfd, connfd, logfd, blockedfd;
    struct sockaddr_storage clientaddr;
    socklen_t clientlen;
    char client_host[MAXLINE], client_port[MAXLINE];
    char logbuf[MAXBUF];

    if (argc != 2) {
        printf("Usage: %s <port>", argv[0]);
    }

    port = argv[1];
    listenfd = Open_listenfd(port);

    // Open log file in append mode or create it if it doesn't exist
    logfd = Open(LOG_FILE, O_WRONLY | O_APPEND | O_CREAT, S_IRUSR | S_IWUSR);
    blockedfd = Open(BLOCKED_FILE, O_RDONLY, 0);

    blocked_list = read_lines(blockedfd, &blocked_cnt);

    while (1) {
	    clientlen = sizeof(clientaddr);
        connfd = Accept(listenfd, (SA *) &clientaddr, &clientlen);
        Getnameinfo((SA *) &clientaddr, clientlen, client_host, MAXLINE, client_port, MAXLINE, 0);

        logline(logfd, "Accepted connection from %s:%s", client_host, client_port);
        
        serve(connfd, logfd);
        Close(connfd);
    }

    Close(logfd);
}

static void serve(int connfd, int logfd) {
    int remote_server_fd;
    rio_t client_rio, remote_server_rio;
    ssize_t line_size;
    char method[MAXLINE], uri[MAXLINE], version[MAXLINE], host[MAXLINE], port[MAXLINE], path[MAXLINE];
    char buf[MAXBUF], logbuf[MAXBUF];
    http_url_error parse_err;
    Rio_readinitb(&client_rio, connfd);
    if (!Rio_readlineb(&client_rio, buf, MAXLINE))
        return;

    sscanf(buf, "%s %s %s", method, uri, version);
    logline(logfd, "method = %s, URI = %s, version = %s", method, uri, version);

    if (is_blocked(uri)) {
        logline(logfd, "URI %s is blocked and won't be forwarded", uri);
        sprintf(buf, "HTTP/1.1 400 No way Jose!\r\n\r\nYou're trying to reach %s through this proxy\r\nnot on my watch!\r\n", uri);
        Rio_writen(connfd, buf, strlen(buf));
        return;
    }
    
    parse_err = parse_http_url(uri, host, MAXLINE, port, MAXLINE, path, MAXLINE);
    if (parse_err != VALID_URL) {
        logline(logfd, "Error parsing destination URL (%s), traffic will not be forwarded", http_url_parse_error_msg(parse_err));
        return;
    }

    if (port[0] == '\0') strcpy(port, HTTP_DEFAULT_PORT);
    logline(logfd, "host = %s, port = %s, path = %s", host, port, path);

    // connect remote server
    remote_server_fd = Open_clientfd(host, port);

    // write a modified request (just the path part, not the full URL)
    sprintf(buf, "%s %s %s\r\n", method, path, version);
    Write(remote_server_fd, buf, strlen(buf));
    
    // forward request headers from the client to the remote server
    while (strcmp(buf, "\r\n")) {
        line_size = Rio_readlineb(&client_rio, buf, MAXBUF - 1);
        buf[line_size] = '\0';
        Rio_writen(remote_server_fd, &buf, line_size);
    }

    // send the remote server response back to the client
    Rio_readinitb(&remote_server_rio, remote_server_fd);
    int content_length = -1;
    int tmp = 0;
    char content_length_buf[50];
    
    // send response headers back to the client
    while ((line_size = Rio_readlineb(&remote_server_rio, buf, MAXLINE - 1)) > 0) {
        if (!strncasecmp(buf, CONTENT_LENGTH_HEADER_PREFIX, CONTENT_LENGTH_HEADER_PREFIX_LEN) &&
            sscanf(buf, "%49s%*[ \t]%d", content_length_buf, &tmp) == 2) { // Content-Length header
            content_length = tmp;
        }
        
        if (!strncmp(buf, "\r\n\r\n", line_size)) { // end of headers
            sprintf(buf, "%s:%s\r\n\r\n", VIA_HEADER, CSAPP_PROXY); // Add our signature :)
            Rio_writen(connfd, buf, strlen(buf));
            break;
        }

        Rio_writen(connfd, buf, line_size);
    }

    // send response body back to the client
    if (content_length >= 0) {
        ssize_t written_bytes = 0;
        ssize_t remaining = content_length;
        while ((written_bytes = Rio_readnb(&remote_server_rio, buf, remaining))) {
            remaining -= written_bytes;
            Rio_writen(connfd, buf, written_bytes);
        }
    } else {
        // Assume response ends up with double CRLF ("\r\n\r\n")
        while ((line_size = Rio_readlineb(&remote_server_rio, buf, MAXLINE - 1)) > 0) { // read and forward
            Rio_writen(connfd, buf, line_size);
            if (!strncmp(buf, "\r\n\r\n", line_size)) break;
        }
    }

    Close(remote_server_fd);
}

static void logline(int logfd, const char *format, ...) {
    char logbuf[MAXBUF];
    static char newline = '\n';
    time_t current_time;

    va_list args;
    va_start(args, format);

    time(&current_time);
    struct tm *timeinfo;
    timeinfo = localtime(&current_time); // get local time
    strftime(logbuf, sizeof(logbuf), "[%Y-%m-%d %H:%M:%S]\t", timeinfo);

    Write(logfd, logbuf, strlen(logbuf));
    vsprintf(logbuf, format, args);
    Write(logfd, logbuf, strlen(logbuf));
    Write(logfd, &newline, 1);

    va_end(args);
}

char** read_lines(int fd, int *line_count) {
    char** lines = NULL;
    char buf[MAXBUF];
    rio_t rio;
    int count = 0;

    Rio_readinitb(&rio, fd);
    ssize_t line_size;
    while ((line_size = Rio_readlineb(&rio, buf, MAXBUF)) > 0) {
        lines = Realloc(lines, (count + 1) * sizeof(char*));
        buf[line_size - 1] = '\0'; // replace '\n' with a null terminator
        lines[count] = strdup(buf);
        count++;
    }

    *line_count = count;
    return lines;
}

int is_blocked(char *url) {
    for (int i = 0; i < blocked_cnt; i++) {
        if (strcmp(url, blocked_list[i]) == 0) return 1;
    }
    return 0;
}
