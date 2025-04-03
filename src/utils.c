#include "utils.h"

int create_socket(Protocol protocol) {
    int sockfd = socket(AF_INET, protocol, 0);
    if (sockfd < 0) {
        logger(ERROR, "Failed to create socket");
    }
    logger(INFO, "Socket created successfully");
    return sockfd;
}

// make this support dns lookups
struct sockaddr_in* setup_address(const char* ip_address, int port) {
    struct sockaddr_in* address = (struct sockaddr_in*) malloc(sizeof(struct sockaddr_in));

    memset(address, 0, sizeof(struct sockaddr_in));
    address->sin_family = AF_INET;
    address->sin_port = htons(port);

    if (inet_pton(AF_INET, ip_address, &(address->sin_addr)) <= 0) {
        logger(ERROR, "Invalid address/Address conversion failed");
    }
    logger(INFO, "Address setup successfully");

    return address;
}