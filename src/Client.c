#include "Client.h"
#include <time.h>
#include <stdio.h> 
#include <stdlib.h> 
#include <string.h>


void send_reliable_package();
void handle_reliable_package(struct ServerMessage *package);
void send_init_package();

unsigned int uuid = 0;
unsigned char connection_id = 0;

unsigned int last_package_id = 0;
struct ServerPackage awaiting_packages[1000];
unsigned int packages_numer = 0;

void server_loop(){
    struct ServerMessage *package = malloc(sizeof(struct ServerMessage) + RECV_PACKAGE_SIZE);
    float buffer = (float) 10;

    if(connection_id==0)
        send_init_package();
    //     send_reliable_package(&buffer, sizeof(float));
    int n = recive_message(package, sizeof(struct ServerMessage) + RECV_PACKAGE_SIZE);
    if(n > 0){
        switch (package->type)
        {
        case INIT_CONNECTION:
            connection_id = * (char *) &package->buffer;
            printf("%d\n",connection_id);
            break;

        case TRUSTED_DATA:
            handle_reliable_package(package);
            break;

        case UNTRUSTED_DATA:
            printf("%d\n", *(int *) &package->buffer);
            unsigned int type = package->package_identifier & 0x00ffffff;
            unsigned char pid = package->package_identifier >> 24;
            if(type==1 && pid!=connection_id){
                HandleMultiplayerInput(pid, *(struct InputPackage *) &package->buffer);
            }
            break;

        default:
            break;
        }
    }
    free(package);
}

void send_package(struct ServerPackage b_package){
    struct ServerMessage *s_package = malloc(sizeof(struct ServerMessage) + b_package.size);
    s_package->type = b_package.type;
    s_package->package_identifier = b_package.package_identifier;
    s_package->timestamp = getTimeMs();
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
    struct ServerPackage package = create_package(TRUSTED_DATA, (connection_id<<24) + last_package_id++, size, buffer);
    awaiting_packages[packages_numer++] = package;
    send_package(package);
}

void handle_reliable_package(struct ServerMessage *package){
    printf("Server : %f\n", * (float *) &package->buffer);
    for (unsigned int i = 0; i < packages_numer; i++)
    {
        if(awaiting_packages[i].package_identifier == package->package_identifier){
            free(awaiting_packages[i].buffer);
            packages_numer--;
            memmove(&awaiting_packages[i], &awaiting_packages[i+1], (packages_numer-i)*sizeof(struct ServerPackage));
            break;
        }
    }
    
}

void send_init_package(){
    if(uuid == 0){
        srand(time(NULL));
        uuid = rand();
    }
    struct ServerPackage package;
    package.type = INIT_CONNECTION;
    package.package_identifier = 1;
    package.size = sizeof(int);
    package.buffer = &uuid;
    send_package(package);
}

void send_input_data(struct InputPackage input) {
    struct ServerPackage package = create_package(UNTRUSTED_DATA, (connection_id<<24)+1, sizeof(struct InputPackage), &input);
    send_package(package);
}