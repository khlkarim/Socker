#include "client.h"

int main(int argc, char** argv){
    // Create an endpoint
    struct Endpoint* e = create_endpoint(TCP, "localhost", "127.0.0.1", 8080);

    // Connect to it
    connect_to(e);

    // Communicate with it
    send_to(e, "Time");

    char* request;
    do{
        request = receive_from(e);
        printf("Server: %s\n", request);
    }while(strlen(request) > 0 && strcmp(request, "Au Revoir") != 0);

    free_endpoint(e);
    return 0;
}