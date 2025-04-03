#ifndef SERVER_H
#define SERVER_H

#include "endpoint.h"

void bind_to(struct Endpoint*);
void listen_to(struct Endpoint*);
struct Endpoint* accept_connexion(struct Endpoint*);

#endif