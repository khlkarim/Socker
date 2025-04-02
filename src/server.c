#include "server.h"

void listen_to(struct Endpoint* e){
	if(!valid_endpoint(e)) return;

	if (bind(e->sockfd, (struct sockaddr*)e->address, sizeof(*(e->address))) == -1) {
		perror("Bind failed");
		printf("sockfd : %d\n port : %d\n", e->sockfd, ntohs(e->address->sin_port));
		handle_error(e, "Bind failed");
		return;
	}
	logger(INFO, "Bound successfully");

	if (listen(e->sockfd, BACKLOG) == -1) {
		handle_error(e, "Listen failed");
		return;
	}
	printf("Server listening on port %d\n", TCP_PORT);
}

struct Endpoint* accept_connexion(struct Endpoint* e){
	if(!valid_endpoint(e)) return NULL;

	struct Endpoint* client = (struct Endpoint*) malloc(sizeof(struct Endpoint));

	int address_size = sizeof(struct sockaddr);

	client->sockfd = accept(e->sockfd, (struct sockaddr *)e->address, &address_size);
	client->address = NULL;

	if(client->sockfd < 0){
		free_endpoint(client);
		handle_error(e, "Failed to accept connexion");
	}

	return client;
}