#include "endpoint.h"

struct Endpoint* create_endpoint(Protocol protocol) {
    struct Endpoint* e = (struct Endpoint*) malloc(sizeof(struct Endpoint));

    e->sockfd = create_socket(protocol);
    e->address = setup_address(IP_ADDRESS, PORT);

    return e;
}

struct Endpoint* create_udp_client(struct Endpoint* server){
    struct Endpoint* client = (struct Endpoint*) malloc(sizeof(struct Endpoint));

    client->sockfd = server->sockfd;
    client->address = (struct sockaddr_in*) malloc(sizeof(struct sockaddr_in));

    return client;
}

// NEEDS FIXING
// int valid_endpoint(struct Endpoint* e){
//     return e != NULL && e->sockfd >= 0 && e->address != NULL;
// }

void send_to(struct Endpoint* e, const char* request) {
    // if(!valid_endpoint(e)) return;

    Protocol protocol = get_socket_type(e);

    if (protocol == TCP && send(e->sockfd, request, strlen(request), 0) < 0) {
        handle_error(e, "Send failed");
    }

    if(protocol == UDP && sendto(e->sockfd, request, sizeof(request), 0, (struct sockaddr*)e->address, sizeof(*(e->address))) < 0){
        handle_error(e, "Send failed");
    }

    printf("Request sent successfully\n");
}

char* receive_from(struct Endpoint* e) {
    // if(e == NULL || e->sockfd < 0) return NULL;

    Protocol protocol = get_socket_type(e);
    int address_size = sizeof(*(e->address));

    char* buffer = (char*) malloc(MAX_BUFFER_SIZE * sizeof(char));
    int bytes_received;

    if((protocol == TCP) && (bytes_received = recv(e->sockfd, buffer, MAX_BUFFER_SIZE - 1, 0)) > 0) {
        buffer[bytes_received] = '\0';
    }

    if((protocol == UDP) && (bytes_received = recvfrom(e->sockfd, buffer, MAX_BUFFER_SIZE - 1, 0, (struct sockaddr*)e->address, &address_size)) > 0) {
        buffer[bytes_received] = '\0';
    }

    if(bytes_received < 0) return NULL;

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

Protocol get_socket_type(struct Endpoint* e){
    Protocol type;
    socklen_t len = sizeof(type);

    if (getsockopt(e->sockfd, SOL_SOCKET, SO_TYPE, &type, &len) == -1) {
        handle_error(NULL, "getsockopt failed");
    }

    return type;
}