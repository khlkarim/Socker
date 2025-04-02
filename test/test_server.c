#include "server.h"

int main(int argc, char** argv){
    // create endpoint
    struct Endpoint* e = create_endpoint();

    // start listen
    listen_to(e);

    // accept connexions
    struct Endpoint* client = accept_connexion(e);

    // communicate
    char* request = receive_from(client);

    printf("Client said: %s\n", request);

    return 0;
}