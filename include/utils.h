#ifndef UTILS_H
#define UTILS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

#include "log.h"

typedef enum{
    TCP=1,
    UDP=2
} Protocol;

int create_socket(Protocol);
struct sockaddr_in* setup_address(const char*, int);

#endif