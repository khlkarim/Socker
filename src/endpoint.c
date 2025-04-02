#include "endpoint.h"

struct Endpoint* create_endpoint() {
    struct Endpoint* e = (struct Endpoint*) malloc(sizeof(struct Endpoint));

    e->sockfd = create_socket();
    e->address = setup_address(IP_ADDRESS, TCP_PORT);

    return e;
}

int valid_endpoint(struct Endpoint* e){
    return e != NULL && e->sockfd >= 0 && e->address != NULL;
}

void send_to(struct Endpoint* e, const char* request) {
    if(!valid_endpoint(e)) return;

    if (send(e->sockfd, request, strlen(request), 0) < 0) {
        handle_error(e, "Send failed");
    }
    printf("Request sent successfully\n");
}

char* receive_from(struct Endpoint* e) {
    if(e == NULL || e->sockfd < 0) return NULL;

    char* buffer = (char*) malloc(MAX_BUFFER_SIZE * sizeof(char));
    int bytes_received;

    if((bytes_received = recv(e->sockfd, buffer, MAX_BUFFER_SIZE - 1, 0)) > 0) {
        buffer[bytes_received] = '\0';
    }else{
        return NULL;
    }

    return buffer;
}

void handle_error(struct Endpoint* e, const char* message) {
    logger(ERROR, message);
    free_endpoint(e);
    
    exit(EXIT_FAILURE);
}

void free_endpoint(struct Endpoint* e){
    if(e == NULL) return;

    if(e->sockfd >= 0) close(e->sockfd);
    if(e->address != NULL) free(e->address);
    
    free(e);
}