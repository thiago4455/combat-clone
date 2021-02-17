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

#define PORT     4242 


int sockfd; 
float buffer; 
struct sockaddr_in     servaddr; 

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
    setsockopt(sockfd,SOL_SOCKET,SO_RCVTIMEO,(char*)&timeout,sizeof(struct timeval));  
    return; 
}

void send_message(float *msg){
    sendto(sockfd, (const float *)msg, 4, 
        MSG_CONFIRM, (const struct sockaddr *) &servaddr,  
            sizeof(servaddr)); 
    printf("Message sent.\n"); 
}

float recive_message(){
    int n, len; 
          
    n = recvfrom(sockfd, &buffer, 4,  
                MSG_WAITALL, (struct sockaddr *) &servaddr, 
                &len); 
            
    if(n>=0)
        printf("Server : %f\n", buffer); 
    else
        printf("Timeout"); 
    return buffer;
}

void close_server(){
    close(sockfd); 
}