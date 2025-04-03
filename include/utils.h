#ifndef UTILS_H
#define UTILS_H

#define _POSIX_C_SOURCE 200112L // netdb.h kill switch

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <unistd.h>

#include "log.h"

typedef enum{
    TCP=1,
    UDP=2
} Protocol;

int create_socket(Protocol);
struct sockaddr_in* setup_address(const char*, const int);
char* dns_lookup(const char* hostname);

#endif