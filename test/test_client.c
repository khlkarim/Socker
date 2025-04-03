#include "client.h"

int main(int argc, char** argv){
    printf("%s\n", dns_lookup("example.com"));

    return 0;
}