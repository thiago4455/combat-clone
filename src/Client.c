#include "Client.h"
#include <stdio.h> 
#include <stdlib.h> 
#include <string.h>


void send_reliable_package();

struct ServerPackage *package_buffer_index[100];
unsigned int last_package_id = 0;

void server_loop(){
    struct ServerPackage package;
    float buffer = (float) 10;

    send_reliable_package(&buffer, sizeof(float));
    if(recive_message(&package, sizeof(struct ServerPackage)) > 0){
        printf("Server : %f\n", package.buffer);
    }
}

void send_package(struct ServerPackage b_package){
    struct ServerMessage{
        PackageType type;
        unsigned int package_identifier;
        unsigned int size;
        char buffer[];
    };
    struct ServerMessage *s_package = malloc(sizeof(struct ServerMessage) + b_package.size);
    s_package->type = b_package.type;
    s_package->package_identifier = b_package.package_identifier;
    s_package->size = b_package.size;
    if(b_package.buffer!=0 && b_package.size!=0)
        memcpy(&s_package->buffer, b_package.buffer, b_package.size);
    send_message(s_package, sizeof(struct ServerMessage) + b_package.size);
    free(s_package);
}

struct ServerPackage create_package(PackageType type, unsigned int package_identifier, unsigned int size, void *buffer){
    struct ServerPackage package;
    package.type = type;
    package.package_identifier = package_identifier;
    package.size = size;
    package.buffer = malloc(size);
    memcpy(package.buffer, buffer, size);
    return package;
}
 
void send_reliable_package(void *buffer, unsigned int size){
    struct ServerPackage package = create_package(TRUSTED_DATA, last_package_id++, size, buffer);
    send_package(package);
}

void send_init_package(){
    struct ServerPackage package;
    package.type = INIT_CONNECTION;
    package.package_identifier = 1;
    package.size = 0;
    package.buffer = 0;
    send_package(package);
}