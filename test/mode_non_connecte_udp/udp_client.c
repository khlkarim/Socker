// Station 2

#include "client.h"
#define N 60

int main(int argc, char** argv){
    // Create an endpoint
    struct Endpoint* e = create_endpoint(UDP, "localhost", "127.0.0.1", 8080);

    // Communicate with it
    for(int i = 0;i<N; i++){
        send_to(e, "hello");
        sleep(1);
    }

    free_endpoint(e);
    return 0;
}