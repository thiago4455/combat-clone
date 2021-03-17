#include "Client.h"
#include <stdio.h> 
#include <stdlib.h> 
#include <unistd.h> 
#include <string.h> 
#include <sys/types.h> 
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <netinet/in.h> 
#include <sys/time.h>
#include <pthread.h>

#define PORT     4242 
const int MS_PER_TICK = 1000 / 60;

int sockfd; 
struct sockaddr_in     servaddr; 

void *connection_thread_handler();

void create_connection(){  
    // Creating socket file descriptor 
    if ( (sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ) { 
        perror("socket creation failed"); 
        exit(EXIT_FAILURE); 
    } 
  
    memset(&servaddr, 0, sizeof(servaddr)); 
    struct timeval timeout={1,0};
      
    // Filling server information 
    servaddr.sin_family = AF_INET; 
    servaddr.sin_port = htons(PORT); 
    servaddr.sin_addr.s_addr = inet_addr("35.247.224.237"); // INADDR_ANY;   
    // servaddr.sin_addr.s_addr = INADDR_ANY;   
    setsockopt(sockfd,SOL_SOCKET,SO_RCVTIMEO,(char*)&timeout,sizeof(struct timeval));  

    pthread_t thread;

    if( pthread_create(&thread , NULL ,  connection_thread_handler , NULL) < 0)
    {
        perror("Error creating thread");
        return;
    }

    return; 
}

void send_message(void *buffer, unsigned int size){
    sendto(sockfd, buffer, size, 
        MSG_CONFIRM, (const struct sockaddr *) &servaddr,  
            sizeof(servaddr));
    // printf("Message sent.\n"); 
}

int recive_message(void *buffer, unsigned int size){
    int n, len; 
          
    n = recvfrom(sockfd, buffer, size,  
                0, NULL, NULL); 
    if(n<0){
        // printf("Error\n:%d", * (int *) buffer);
    }
    return n;
}

void close_server(){
    close(sockfd); 
}

unsigned int getTimeMs(){
    struct timeval time;
    gettimeofday(&time, NULL);
    return time.tv_sec * 1000000 + time.tv_usec;
}

void *connection_thread_handler(){
    unsigned int currentTime = getTimeMs() + MS_PER_TICK;
    while (1)    
        while (currentTime < getTimeMs())
        {
            server_loop();
            currentTime += MS_PER_TICK;
        }
}