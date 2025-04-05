// Multithreading

#include "server.h"
#include <time.h>
#include <pthread.h>
#define N 10
#define NB_THREADS 5

char* get_current_time();
void* get_time_service(void*);

int main(int argc, char** argv){
    // create endpoint
    struct Endpoint *e = create_endpoint(TCP, "localhost", "127.0.0.1", 8080);

    // start listen
    listen_to(e);

    // Create 5 threads
    pthread_t threads[NB_THREADS];

    for (int i = 0; i < NB_THREADS; i++) {
        if (pthread_create(&threads[i], NULL, get_time_service, e) != 0) {
            perror("Failed to create thread");
            return 1;
        }
    }

    // Join all threads to ensure they complete before exiting
    for (int i = 0; i < NB_THREADS; i++) {
        if (pthread_join(threads[i], NULL) != 0) {
            perror("Failed to join thread");
            return 1;
        }
    }
    
    free_endpoint(e);
    return 0;
}

char* get_current_time() {
    time_t raw_time;
    struct tm *time_info;
    char* buffer = (char*) malloc(9 * sizeof(char));

    time(&raw_time);
    time_info = localtime(&raw_time);
    strftime(buffer, 9, "%H:%M:%S", time_info);

    return buffer;
}

void* get_time_service(void* arg){
    struct Endpoint *e = (struct Endpoint*)arg;
    struct Endpoint *client = NULL;

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

    return NULL;
}