#ifndef SERVER_H
#define SERVER_H

#include "endpoint.h"

void listen_to(struct Endpoint*);
struct Endpoint* accept_connexion(struct Endpoint*);

#endif