#define WIN32_LEAN_AND_MEAN
#include "Client.h"
#include <stdio.h> 
#include <stdlib.h> 
#include <stdint.h>
#include <winsock2.h>
#include <windows.h>

#pragma comment(lib,"ws2_32.lib") //Winsock Library

#define PORT     4242 
const int MS_PER_TICK = 1000 / 60;

WSADATA wsa;
int sockfd;
float buffer;
struct sockaddr_in servaddr;

DWORD WINAPI connection_thread_handler(void* data);


void create_connection(){  
    // Creating socket file descriptor 
    WSAStartup(MAKEWORD(2,2),&wsa);
    if ( (sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ) { 
        perror("socket creation failed"); 
    }

    // Filling server information 
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(PORT);
    servaddr.sin_addr.s_addr = inet_addr("35.247.224.237");
    // servaddr.sin_addr.s_addr = inet_addr("0.0.0.0");

    HANDLE thread = CreateThread(NULL, 0, connection_thread_handler, NULL, 0, NULL);

    if (!thread) {
        perror("Error creating thread");
        return;
    }

    return; 
}

void send_message(void *buffer, unsigned int size){
    int r = sendto(sockfd, buffer, size, 
            0, (const struct sockaddr *) &servaddr, sizeof(servaddr)); 
    if (r == SOCKET_ERROR) {
        printf("Error: %d\n", WSAGetLastError());
    } 
    // else
    //     // printf("Message sent.\n"); 
}

int recive_message(void *buffer, unsigned int size){
    int n, len; 
          
    n = recvfrom(sockfd, buffer, size,  
                0, NULL, NULL); 
            
    if (n == SOCKET_ERROR) {
        WSAGetLastError();
    } 
    // else
    //     printf("Server : %f\n", buffer);
        
    return n;
}

void close_server(){
    close(sockfd);
    return;

}

unsigned int getTimeMs(){
    static const uint64_t EPOCH = ((uint64_t) 116444736000000000ULL);

    SYSTEMTIME  system_time;
    FILETIME    file_time;
    uint64_t    time;

    GetSystemTime( &system_time );
    SystemTimeToFileTime( &system_time, &file_time );
    time =  ((uint64_t)file_time.dwLowDateTime )      ;
    time += ((uint64_t)file_time.dwHighDateTime) << 32;

    return ((time - EPOCH) / 10000000L) * 1000000 + (system_time.wMilliseconds * 1000);
}

DWORD WINAPI connection_thread_handler(void* data){
    unsigned int currentTime = getTimeMs() + MS_PER_TICK;
    while (1)    
        while (currentTime < getTimeMs())
        {
            server_loop();
            currentTime += MS_PER_TICK;
        }
}