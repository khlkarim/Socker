// Station 2

#include "client.h"

int main(int argc, char** argv){
    // Create an endpoint
    struct Endpoint* e = create_endpoint(TCP, "localhost", "127.0.0.1", 8080);

    // Connect to it
    connect_to(e);

    // Communicate with it
    send_to(e, "Bonjour");

    char* responce = NULL;
    do{
        if(responce != NULL) free(responce);
        responce = receive_from(e);
        printf("Server: %s (%zu)\n", responce, strlen(responce));
    }while(strlen(responce) > 0 && strcmp(responce, "Au Revoir") != 0);

    free(responce);
    free_endpoint(e);
    return 0;
}