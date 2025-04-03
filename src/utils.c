#include "utils.h"

int create_socket(Protocol protocol) {
    int sockfd = socket(AF_INET, protocol, 0);
    
    if (sockfd < 0) {
        logger(ERROR, "Failed to create socket");
    }
    logger(INFO, "Socket created successfully");
    
    return sockfd;
}

struct sockaddr_in* setup_address(const char* ip_address, const int port) {
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

char* dns_lookup(const char* hostname) {
    struct addrinfo hints, *res;
    char* ip = (char*) malloc(INET_ADDRSTRLEN * sizeof(char));

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;

    int status = getaddrinfo(hostname, NULL, &hints, &res);
    if (status != 0) {
        free(ip);
        logger(ERROR, "DNS lookup failed");
        exit(EXIT_FAILURE);
    }

    struct sockaddr_in* ipv4 = (struct sockaddr_in*)res->ai_addr;
    inet_ntop(AF_INET, &(ipv4->sin_addr), ip, INET_ADDRSTRLEN);

    freeaddrinfo(res);
    return ip;
}