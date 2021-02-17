#include "Client.h"
#include <stdio.h> 
#include <stdlib.h> 
#include <winsock2.h>

#pragma comment(lib,"ws2_32.lib") //Winsock Library

#define PORT     4242 

WSADATA wsa;
int sockfd;
float buffer;
struct sockaddr_in servaddr;


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
    
    return; 
}

void send_message(float *msg){
    int r = sendto(sockfd, (char*)msg, 4, 
        0, (const struct sockaddr *) &servaddr,  
            sizeof(servaddr)); 
    if (r == SOCKET_ERROR) {
        printf("Error: %d\n", WSAGetLastError());
    } else
    printf("Message sent.\n"); 
}

float recive_message(){
    int n, len; 
          
    n = recvfrom(sockfd, (char*)&buffer, 4,  
                0, (struct sockaddr *) &servaddr, 
                &len); 
            
    if (n == SOCKET_ERROR) {
        WSAGetLastError();
    } else
        printf("Server : %f\n", buffer);
        
    return buffer;
}

void close_server(){
    return;
}