// Station 1

#include "server.h"

int main(int argc, char** argv){
    // create endpoint
    struct Endpoint *e = create_endpoint(UDP, "localhost", "127.0.0.1", 8080);
    struct Endpoint *client = create_udp_client(e);

    // start listen
    listen_to(e);

    char* request;
    
    while(request = receive_from(client)){
        printf("Client said: %s\n", request);
    }

    return 0;
}