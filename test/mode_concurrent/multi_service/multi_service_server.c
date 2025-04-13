// Forking

// Echo Server
// Time Server
// Random Number Generator Server

#include "server.h"
#include <time.h>
#include <math.h>
#include <pthread.h>
#define N 10

char* get_current_time();
int get_random_int();

void* echo_service(void*);
void* time_service(void*);
void* random_service(void*);

void mono_service(void* (*service)(void*), int port);

int main(int argc, char** argv){
    pid_t pid = fork();

    printf("forked\n");

    if(pid == 0){
        pid = fork();
    }

    int ports[] = {8000, 8001, 8002};
    void* (*services[])(void*) = {echo_service, time_service, random_service};

    mono_service(services[pid], ports[pid]);
    return 0;
}

void mono_service(void* (*service)(void*), int port){
    // create endpoint
    struct Endpoint *e = create_endpoint(TCP, "localhost", "127.0.0.1", port);
    struct Endpoint *client;

    // start listen
    listen_to(e);

    while ((client = accept_connexion(e)) != NULL) {
        // Create a new thread to handle the client
        pthread_t thread;
        if (pthread_create(&thread, NULL, service, client) != 0) {
            perror("Failed to create thread");
            free_endpoint(client);
            continue;
        }

        // Detach the thread to allow it to clean up after itself
        pthread_detach(thread);
    }

    free_endpoint(e);
}

void* time_service(void* arg){
    struct Endpoint *client = (struct Endpoint *)arg;

    // Communicate
    char* request = receive_from(client);
    printf("Client said: %s\n", request);
    free(request);

    char* current_time;
    for(int i = 0; i < N; i++){
        current_time = get_current_time();
        send_to(client, current_time);
        free(current_time);

        sleep(1);
    }
    send_to(client, "Au Revoir");
    free_endpoint(client);

    logger(INFO, "Connection ended");

    // Free the thread arguments
    free(arg);
}

void* echo_service(void* arg){
    struct Endpoint *client = (struct Endpoint *)arg;

    char* request = NULL;

    do{
        if(request != NULL) free(request);

        request = receive_from(client);
        printf("Client said: %s\n", request);
        send_to(client, request);
    }while(strcmp(request, "exit") != 0);

    free(request);

    logger(INFO, "Connection ended");

    // Free the thread arguments
    free(arg);
}

void* random_service(void* arg){
    struct Endpoint *client = (struct Endpoint *)arg;

    // Communicate
    char response[4];
    sprintf(response, "%d", get_random_int());
    send_to(client, response);

    logger(INFO, "Connection ended");

    // Free the thread arguments
    free(arg);
}

int get_random_int(){
    srand(time(NULL));
    return rand() % 100;
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