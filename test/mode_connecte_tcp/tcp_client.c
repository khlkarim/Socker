// Station 2

#include "client.h"
#define N 60

int main(int argc, char** argv){
    char* hostname = "localhost";
    char* ip_address = "127.0.0.1";

    // Create an endpoint
    struct Endpoint* e = create_endpoint(TCP, hostname, ip_address, 8080);

    // Connect to it
    connect_to(e);

    // Communicate with it
    for(int i = 0;i<N; i++){
        // printf("Message: ");
        // scanf("%s", msg);
        send_to(e, "hello");
        sleep(1);
    }

    free_endpoint(e);
    return 0;
}