#include "endpoint.h"

struct Endpoint* create_endpoint(Protocol protocol, char* hostname, char* ip_address, int port) {
    if((protocol != TCP && protocol != UDP) || (hostname == NULL && ip_address == NULL) || port <= 1024){
        throw_error(NULL, "Invalid arguments to create_endpoint(...)");
    }

    struct Endpoint* e = (struct Endpoint*) malloc(sizeof(struct Endpoint));

    e->sockfd = create_socket(protocol);
    e->protocol = protocol;
    e->hostname = hostname;
    e->ip_address = ip_address;
    e->port = port;

    if(e->ip_address != NULL){
        e->address = setup_address(e->ip_address, e->port);
    }else{
        // TODO: DNS lookup
    }

    return e;
}

struct Endpoint* create_udp_client(struct Endpoint* server){
    struct Endpoint* client = (struct Endpoint*) malloc(sizeof(struct Endpoint));

    client->sockfd = server->sockfd;
    client->protocol = UDP;
    client->address = (struct sockaddr_in*) malloc(sizeof(struct sockaddr_in));

    return client;
}

void validate_endpoint(struct Endpoint* e){
    if(e == NULL || e->address == NULL){
        throw_error(e, "Invalid struct Endpoint passed as argument");
    }
}

void send_to(struct Endpoint* e, const char* request) {
    validate_endpoint(e);

    // Send via TCP
    if (e->protocol == TCP 
        && send(e->sockfd, request, strlen(request), 0) < 0) {
        throw_error(e, "Send failed");
    }else if(e->protocol == TCP){
        logger(INFO, "Request sent successfully");
        return;
    }

    // Send via UDP
    if (e->protocol == UDP 
        && sendto(e->sockfd, request, sizeof(request), 0, (struct sockaddr*)e->address, sizeof(*(e->address))) < 0){
        throw_error(e, "Send failed");
    }else if(e->protocol == UDP){
        logger(INFO, "Request sent successfully");
        return;
    }

    throw_error(e, "Unsupported Protocol");
}

char* receive_from(struct Endpoint* e) {
    validate_endpoint(e);

    char* buffer = (char*) malloc(MAX_BUFFER_SIZE * sizeof(char));
    int bytes_received, address_size = sizeof(*(e->address));

    // Receive via TCP
    if (e->protocol == TCP 
        && (bytes_received = recv(e->sockfd, buffer, MAX_BUFFER_SIZE - 1, 0)) > 0) {
        buffer[bytes_received] = '\0';
        return buffer;
    }else if(e->protocol == TCP){
        throw_error(e, "Failed to receive message");
    }

    // Receive via UDP
    if (e->protocol == UDP 
        && (bytes_received = recvfrom(e->sockfd, buffer, MAX_BUFFER_SIZE - 1, 0, (struct sockaddr*)e->address, &address_size)) > 0) {
        buffer[bytes_received] = '\0';
        return buffer;
    }else if(e->protocol == UDP){
        throw_error(e, "Failed to receive message");
    }

    throw_error(e, "Unsupported Protocol");
}

void throw_error(struct Endpoint* e, const char* message) {
    free_endpoint(e);
    logger(ERROR, message);    
    exit(EXIT_FAILURE);
}

void free_endpoint(struct Endpoint* e){
    if(e == NULL) return;

    if(e->sockfd >= 0) close(e->sockfd);
    if(e->hostname != NULL) free(e->hostname);
    if(e->ip_address != NULL) free(e->ip_address);
    if(e->address != NULL) free(e->address);
    
    free(e);
}

void log_endpoint(struct Endpoint* e){
    printf(
        "Endpoint {\n"
        "\tsockfd: %d\n"
        "\tprotocol: %d\n"
        "\thostname: %s\n"
        "\tip_address: %s\n"
        "\tport: %d\n"
        "}\n",
        e->sockfd, (int)(e->protocol), e->hostname, e->ip_address, e->port  
    );
}