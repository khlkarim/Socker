#include "client.h"

void connect_to(struct Endpoint* e) {
    if(!valid_endpoint(e)) return;

    if (connect(e->sockfd, (struct sockaddr *)(e->address), sizeof(struct sockaddr_in)) < 0) {
        handle_error(e, "Failed to connect to the server");
    }
    logger(INFO, "Connected to the server successfully");
}
