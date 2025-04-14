#include "client.h"

int main(int argc, char** argv){
    struct Endpoint* e = create_endpoint(TCP, "localhost", "127.0.0.1", 8001);

    connect_to(e);

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