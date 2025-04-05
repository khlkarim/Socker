#include "client.h"

int main(int argc, char** argv){
    // Create an endpoint
    struct Endpoint* e = create_endpoint(TCP, "localhost", "127.0.0.1", 8080);

    // Connect to it
    connect_to(e);

    // Communicate with it
    send_to(e, "Random");
    
    char* request = receive_from(e);
    printf("Server: %s\n", request);
    free(request);

    free_endpoint(e);
    return 0;
}