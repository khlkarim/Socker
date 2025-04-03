// Station 1

#include "client.h"

int main(int argc, char** argv){
    // Create an endpoint
    struct Endpoint* e = create_endpoint(UDP, "localhost", "127.0.0.1", 8080);

    // Communicate with it
    send_to(e, "Bonjour");

    char* responce;
    do{
        responce = receive_from(e);
        printf("Server: %s (%zu)\n", responce, strlen(responce));
    }while(strlen(responce) > 0 && strcmp(responce, "Au Revoir") != 0);

    free_endpoint(e);
    return 0;
}