#include "client.h"

int main(int argc, char** argv){
    struct Endpoint* e = create_endpoint(TCP, "localhost", "127.0.0.1", 8000);

    connect_to(e);

    char* request = NULL;
    do{
        if(request != NULL) free(request);
        request = (char*) malloc(MAX_BUFFER_SIZE * sizeof(char));
        
        printf("Message: ");
        scanf("%s", request);
        
        send_to(e, request);
        free(request);

        request = receive_from(e);
        printf("Server: %s\n", request);
    }while(strlen(request) > 0 && strcmp(request, "exit") != 0);

    free_endpoint(e);
    return 0;
}