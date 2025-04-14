#include  "client.h"

#define HTTP_PORT 80

struct Request {
    char method[16];
    char target[256];
    char version[16];
    char host[256];
    char headers[1024];
    char body[2048];
};

char* http_stringify_request(const struct Request* request) {
    size_t buffer_size = 4096; // Increased buffer size to accommodate headers and body
    char* s = (char*) malloc(buffer_size * sizeof(char));

    snprintf(s, buffer_size, "%s %s %s\r\nHost: %s\r\n%s\r\n\r\n%s",
             request->method, request->target, request->version, request->host,
             request->headers, request->body);

    return s;
}

struct Request* http_build_request() {
    struct Request* r = (struct Request*) malloc(sizeof(struct Request));

    logger(INFO, "Constructing HTTP request:");
    printf("Method: ");
    scanf("%15s", r->method);

    printf("Target: ");
    scanf("%255s", r->target);

    printf("Version: ");
    scanf("%15s", r->version);

    printf("Host: ");
    scanf("%255s", r->host);

    printf("Headers (end with an empty line):\n");
    r->headers[0] = '\0';
    char header_line[256];
    while (1) {
        fgets(header_line, sizeof(header_line), stdin);
        if (strcmp(header_line, "\n") == 0) break;
        strncat(r->headers, header_line, sizeof(r->headers) - strlen(r->headers) - 1);
    }

    printf("Body (end with an empty line):\n");
    r->body[0] = '\0';
    char body_line[256];
    while (1) {
        fgets(body_line, sizeof(body_line), stdin);
        if (strcmp(body_line, "\n") == 0) break;
        strncat(r->body, body_line, sizeof(r->body) - strlen(r->body) - 1);
    }

    return r;
}

int main(int argc, char** argv){
    struct Request* request = http_build_request();

    struct Endpoint* e = create_endpoint(TCP, request->host, NULL, HTTP_PORT);
    connect_to(e);

    char* buffer = http_stringify_request(request);
    send_to(e, buffer);
    free(buffer);

    do {
        buffer = receive_from(e);
        printf("%s", buffer);
        free(buffer);

    } while (buffer && strlen(buffer) > 0);

    free_endpoint(e);
    free(request);
    return 0;
}