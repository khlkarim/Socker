// Station 1

#include "server.h"

int main(int argc, char** argv){
    // create endpoint
    struct Endpoint* e = create_endpoint(TCP);

    // start listen
    listen_to(e);

    // accept connexions
    struct Endpoint* client = accept_connexion(e);

    // communicate
    char* request;

    while(request = receive_from(client)){
        printf("Client said: %s\n", request);
    }

    return 0;
}