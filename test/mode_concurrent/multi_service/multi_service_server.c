// Forking

// Echo Server
// Time Server
// Random Number Generator Server

#include "server.h"
#include <time.h>
#include <math.h>
#define N 10

char* get_current_time();
int get_random_int();
void handle_connection(struct Endpoint*);

void echo_service(struct Endpoint*);
void time_service(struct Endpoint*);
void random_service(struct Endpoint*);

int main(int argc, char** argv){
    // Create endpoint
    struct Endpoint* e = create_endpoint(TCP, "localhost", "127.0.0.1", 8080);
    struct Endpoint* client = NULL;

    // Start listening
    listen_to(e);

    // Accept connexions
    while(client = accept_connexion(e)){
        pid_t pid = fork();

        if(pid == 0){
            handle_connection(client);
            return 0;
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

int get_random_int(){
    srand(time(NULL));
    return rand() % 100;
}

void handle_connection(struct Endpoint* client){
    // determine the demanded services and put it through

    // Communicate
    char* service = receive_from(client);
    printf("Service Demanded: %s\n", service);

    if(strcmp(service, "Echo") == 0){
        echo_service(client);
    }else if(strcmp(service, "Time") == 0){
        time_service(client);
    }else if(strcmp(service, "Random") == 0){
        random_service(client);
    }else{
        send_to(client, "Unsupported service");
    }

    logger(INFO, "Connection ended");
    free_endpoint(client);
}

void echo_service(struct Endpoint* client){
    char* request;

    do{
        if(request != NULL) free(request);

        request = receive_from(client);
        printf("Client said: %s\n", request);
        send_to(client, request);
    }while(strcmp(request, "exit") != 0);

    free(request);
}

void time_service(struct Endpoint* client){
    char* current_time;
    for(int i = 0;i<N;i++){
        current_time = get_current_time();
        send_to(client, current_time);
        free(current_time);

        sleep(1);
    }
    send_to(client, "Au Revoir");
}

void random_service(struct Endpoint* client){
    char responce[4];
    sprintf(responce, "%d", get_random_int());
    send_to(client, responce);
}