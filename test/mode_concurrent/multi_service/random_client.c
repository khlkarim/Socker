#include "client.h"

int main(int argc, char** argv){
    struct Endpoint* e = create_endpoint(TCP, "localhost", "127.0.0.1", 8002);

    connect_to(e);

    send_to(e, "Random");
    
    char* request = receive_from(e);
    printf("Server: %s\n", request);
    free(request);

    free_endpoint(e);
    return 0;
}