#ifndef SERVER_H
#define SERVER_H

#include "endpoint.h"

void bind_to(struct Endpoint* server);
void listen_to(struct Endpoint* server);
struct Endpoint* accept_connexion(struct Endpoint* server);

#endif