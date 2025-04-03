#ifndef ENDPOINT_H
#define ENDPOINT_H 

#include "config.h"
#include "utils.h"

struct Endpoint{
    int sockfd;
    struct sockaddr_in *address;  
};

struct Endpoint* create_endpoint();
struct Endpoint* create_udp_client(struct Endpoint*);

int valid_endpoint(struct Endpoint*);
void handle_error(struct Endpoint*, const char*);
Protocol get_socket_type(struct Endpoint*);
void free_endpoint(struct Endpoint*);

void send_to(struct Endpoint*, const char*);
char* receive_from(struct Endpoint*);

#endif