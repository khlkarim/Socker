#ifndef ENDPOINT_H
#define ENDPOINT_H 

#include "config.h"
#include "utils.h"

struct Endpoint{
    int sockfd;
    struct sockaddr_in *address;  
};

struct Endpoint* create_endpoint();

int valid_endpoint(struct Endpoint*);
void handle_error(struct Endpoint*, const char*);
void free_endpoint(struct Endpoint*);

void send_to(struct Endpoint*, const char*);
char* receive_from(struct Endpoint*);

#endif