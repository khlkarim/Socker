// Station 1

#include "server.h"

int main(int argc, char** argv){
    // create endpoint
    struct Endpoint *e = create_endpoint(UDP);
    struct Endpoint *client = create_udp_client(e);

    // start listen
    listen_to(e);

    // communicate
    char* request;

    while(request = receive_from(client)){
        printf("Client said: %s\n", request);
    }

    return 0;
}