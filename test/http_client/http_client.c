#include  "client.h"

struct Request {
    char method[16];
    char target[256];
    char version[16];
    char host[256];
};

char* http_stringify_request(const struct Request* request) {
    char* s = (char*) malloc(600 * sizeof(char));

    snprintf(s, 600, "%s %s %s\r\nHost: %s\r\nConnection: close\r\n\r\n",
             request->method, request->target, request->version, request->host);

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

    return r;
}

int main(int argc, char** argv){
    // Construct http request
    struct Request* request = http_build_request();

    // Create endpoint
    struct Endpoint* e = create_endpoint(TCP, request->host, NULL, 80);

    // Connect to the server
    connect_to(e);

    // Communicate with the server
    char* buffer = http_stringify_request(request);
    send_to(e, buffer);

    do{
        free(buffer);
        buffer = receive_from(e);
        printf("%s", buffer);
    } while(strlen(buffer)>0);

    free_endpoint(e);
    return 0;
}