// Station 1

#include "server.h"
#include <time.h>
#define N 10

char* get_current_time() {
    time_t raw_time;
    struct tm *time_info;
    char* buffer = (char*) malloc(9 * sizeof(char));

    time(&raw_time);
    time_info = localtime(&raw_time);
    strftime(buffer, 9, "%H:%M:%S", time_info);

    return buffer;
}

int main(int argc, char** argv){
    // Create endpoint
    struct Endpoint* e = create_endpoint(TCP, "localhost", "127.0.0.1", 8080);
    struct Endpoint* client = NULL;

    // Start listening
    listen_to(e);

    // Accept connexions
    while(client = accept_connexion(e)){
        // Communicate
        char* request = receive_from(client);
        printf("Client said: %s\n", request);
        free(request);

        char* current_time;
        for(int i = 0;i<N;i++){
            current_time = get_current_time();
            send_to(client, current_time);
            free(current_time);

            sleep(1);
        }
        send_to(client, "Au Revoir");
        free_endpoint(client);

        logger(INFO, "Connection ended");
    }

    free_endpoint(e);
    return 0;
}