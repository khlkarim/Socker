#include "server.h"

int main(int argc, char** argv){
    // create endpoint
    struct Endpoint* e = create_endpoint(TCP, "localhost", "127.0.0.1", 8080);

    // start listen
    listen_to(e);
    log_endpoint(e);

    // accept connexions
    struct Endpoint* client = accept_connexion(e);
    log_endpoint(client);

    // communicate
    char* request = receive_from(client);

    printf("Client said: %s\n", request);

    return 0;
}