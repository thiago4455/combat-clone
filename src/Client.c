#include "Client.h"
#include <stdio.h> 
#include <stdlib.h> 
#include <string.h>


void send_reliable_package();

unsigned int package_buffer[100];
unsigned int last_package_id = 0;

void server_loop(){
    struct ServerPackage package;
    float buffer = (float) 10;

    send_reliable_package(&buffer, sizeof(float));
    if(recive_message(&package, sizeof(struct ServerPackage)) > 0){
        printf("Server : %f\n", package.buffer);
    }
}

void send_package(PackageType type, unsigned int package_identifier, unsigned int size, void *buffer){
    struct ServerPackage *s_package = malloc(sizeof(struct ServerPackage) + size);
    s_package->type = type;
    s_package->package_identifier = package_identifier;
    s_package->size = size;
    if(buffer!=0 && size!=0)
        memcpy(&s_package->buffer, buffer, size);
    send_message(s_package, sizeof(struct ServerPackage) + size);
}
 
void send_reliable_package(void *buffer, unsigned int size){
    send_package(TRUSTED_DATA, last_package_id++, size, buffer);
}

void send_init_package(){
    send_package(INIT_CONNECTION, 1, 0, 0);
}