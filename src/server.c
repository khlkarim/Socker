#include "server.h"

void bind_to(struct Endpoint* e){
	if(e == NULL || e->sockfd < 0 || e->address == NULL){
        throw_error(e, "Invalid argument passed to bind_to(...)");
	}

	if (bind(e->sockfd, (struct sockaddr*)e->address, sizeof(*(e->address))) == -1) {
		throw_error(e, "Bind failed");
	}

	logger(INFO, "Bound successfully");
}

void listen_to(struct Endpoint* e){
	bind_to(e);

	if ((e->protocol == TCP) && (listen(e->sockfd, BACKLOG) == -1)) {
		throw_error(e, "Listen failed");
	}

	logger(INFO, "Server listening ...");
}

// Used in TCP communication
struct Endpoint* accept_connexion(struct Endpoint* e){
	if(e == NULL || e->sockfd < 0 || e->protocol != TCP || e->address == NULL){
        throw_error(e, "Invalid argument passed to accept_connexion(...)");
	}

	struct Endpoint* client = (struct Endpoint*) malloc(sizeof(struct Endpoint));
	int address_size = sizeof(struct sockaddr);

	client->sockfd = accept(e->sockfd, (struct sockaddr *)e->address, &address_size);
	client->protocol = e->protocol;

	if(client->sockfd < 0){
		throw_error(e, "Failed to accept connexion");
	}
	logger(INFO, "Connection accepted successfully");

	return client;
}