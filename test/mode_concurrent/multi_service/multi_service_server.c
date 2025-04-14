#include "server.h"
#include <time.h>
#include <math.h>
#include <pthread.h>
#include <sys/select.h>

#define N 60
#define NB_SERVICES 3

char* get_current_time();
int get_random_int();

void* echo_service(void*);
void* time_service(void*);
void* random_number_service(void*);

struct Args{
    struct Endpoint* endpoint;
    void* (*service)(void*);
};

int main(int argc, char** argv){
    struct Endpoint* endpoints[NB_SERVICES];
    fd_set read_fds, active_fds;
    int max_fd = 0;

    for (int i = 0; i < NB_SERVICES; i++) {
        endpoints[i] = create_endpoint(TCP, NULL, "127.0.0.1", 8000+i);
        listen_to(endpoints[i]);
        if (endpoints[i]->sockfd > max_fd) {
            max_fd = endpoints[i]->sockfd;
        }
    }

    void* (*services[])(void*) = {echo_service, time_service, random_number_service};

    FD_ZERO(&active_fds);
    for (int i = 0; i < NB_SERVICES; i++) {
        FD_SET(endpoints[i]->sockfd, &active_fds);
    }

    while (1) {
        read_fds = active_fds;

        if (select(max_fd + 1, &read_fds, NULL, NULL, NULL) < 0) {
            perror("select");
            break;
        }

        for (int i = 0; i < NB_SERVICES; i++) {
            if (FD_ISSET(endpoints[i]->sockfd, &read_fds)) {
                struct Endpoint* client = accept_connexion(endpoints[i]);
                if (client != NULL) {
                    pthread_t thread;
                    struct Args *args = malloc(sizeof(struct Args*));

                    args->endpoint = client;
                    args->service = services[i];

                    pthread_create(&thread, NULL, args->service, args->endpoint);
                    pthread_detach(thread);
                }
            }
        }
    }

    for (int i = 0; i < NB_SERVICES; i++) {
        free_endpoint(endpoints[i]);
    }

    return 0;
}

void* time_service(void* arg){
    struct Endpoint *client = (struct Endpoint *)arg;

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

    free(arg);
}

void* random_number_service(void* arg){
    struct Endpoint *client = (struct Endpoint *)arg;

    char response[4];
    sprintf(response, "%d", get_random_int());
    send_to(client, response);

    logger(INFO, "Connection ended");

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