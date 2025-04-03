#include "client.h"

int main(int argc, char** argv){
    // create an endpoint
    struct Endpoint* e = create_endpoint(TCP, "localhost", "127.0.0.1", 8080);

    // connect to it
    connect_to(e);
    log_endpoint(e);

    // communicate with it
    send_to(e, "hello");

    return 0;
}