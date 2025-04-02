#include "client.h"

int main(int argc, char** argv){
    // create an endpoint
    struct Endpoint* e = create_endpoint();

    // connect to it
    connect_to(e);

    // communicate with it
    send_to(e, "hello");

    return 0;
}