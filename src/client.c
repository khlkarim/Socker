#include "client.h"

// Used in TCP communication
void connect_to(struct Endpoint* server) {
    if(server == NULL || server->sockfd < 0 || server->protocol != TCP || server->address == NULL){
        throw_error(server, "Invalid argument passed to connect_to(...)");
    }

    if (connect(server->sockfd, (struct sockaddr *)(server->address), sizeof(struct sockaddr_in)) < 0) {
        throw_error(server, "Failed to connect to the server");
    }
    logger(INFO, "Connected to the server successfully");
}
