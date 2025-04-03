#include "client.h"

// Used in TCP communication
void connect_to(struct Endpoint* server) {
    validate_endpoint(server);

    if (connect(server->sockfd, (struct sockaddr *)(server->address), sizeof(struct sockaddr_in)) < 0) {
        throw_error(server, "Failed to connect to the server");
    }
    logger(INFO, "Connected to the server successfully");
}
