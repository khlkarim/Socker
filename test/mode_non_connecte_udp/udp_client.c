// Station 2

#include "client.h"
#define N 60

int main(int argc, char** argv){
    // create an endpoint
    struct Endpoint* e = create_endpoint(UDP);

    // communicate with it
    for(int i = 0;i<N; i++){
        send_to(e, "hello");
        // sleep(1);
    }

    return 0;
}