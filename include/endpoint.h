#ifndef ENDPOINT_H
#define ENDPOINT_H 

#include "config.h"
#include "utils.h"

struct Endpoint{
    int sockfd;
    Protocol protocol;
    char* hostname;
    char* ip_address;
    int port;
    struct sockaddr_in* address;  
};

struct Endpoint* create_endpoint(Protocol, char*, char*, int);
struct Endpoint* create_udp_client(struct Endpoint*);

void validate_endpoint(struct Endpoint*);
void throw_error(struct Endpoint*, const char*);
void log_endpoint(struct Endpoint*);
void free_endpoint(struct Endpoint*);

void send_to(struct Endpoint*, const char*);
char* receive_from(struct Endpoint*);

#endif