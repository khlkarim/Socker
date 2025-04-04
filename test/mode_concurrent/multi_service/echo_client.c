#include "client.h"

int main(int argc, char** argv){
    // Create an endpoint
    struct Endpoint* e = create_endpoint(TCP, "localhost", "127.0.0.1", 8080);

    // Connect to it
    connect_to(e);

    // Communicate with it
    send_to(e, "Echo");

    char* request = (char*) malloc(MAX_BUFFER_SIZE * sizeof(char));
    do{
        printf("Message: ");scanf("%s", request);
        send_to(e, request);
        request = receive_from(e);
        printf("Server: %s\n", request);
    }while(strlen(request) > 0 && strcmp(request, "exit") != 0);

    free_endpoint(e);
    return 0;
}