// Station 1

#include "server.h"

int main(int argc, char** argv){
    // Create endpoint
    struct Endpoint* e = create_endpoint(TCP, "localhost", "127.0.0.1", 8080);

    // Start listen
    listen_to(e);

    // Accept connexions
    struct Endpoint* client = accept_connexion(e);

    // Communicate
    char* request = receive_from(client);
    while(request[0] != '\0'){
        printf("Client said: %s\n", request);
        request = request = receive_from(client);
    }

    logger(INFO, "Connection ended");

    free_endpoint(e);
    free_endpoint(client);
    return 0;
}