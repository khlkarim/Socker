// Station 2

#include "client.h"

int main(int argc, char** argv){
    struct Endpoint* e = create_endpoint(TCP, "localhost", "127.0.0.1", 8080);

    connect_to(e);

    send_to(e, "Bonjour");

    int nbMessages = 0;
    char* responce = NULL;

    do{
        if(responce != NULL) free(responce);

        responce = receive_from(e);
        printf("Server: %s (%zu)\n", responce, strlen(responce));

        nbMessages++;
    }while(strlen(responce) > 0 && strcmp(responce, "Au Revoir") != 0);
    free(responce);

    printf("Nombre de messages reçues: %d\n", nbMessages);

    free_endpoint(e);    
    return 0;
}