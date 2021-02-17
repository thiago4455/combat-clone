#include "Client.h"
#include <stdio.h> 
#include <stdlib.h> 
// #include <winsock2.h>

// #pragma comment(lib,"ws2_32.lib") //Winsock Library

// #define PORT     4242 

// WSADATA wsa;
// SOCKET s;
// struct sockaddr_in server;


void create_connection(){  
    // Creating socket file descriptor 
    // WSAStartup(MAKEWORD(2,2),&wsa);
    // s = socket(AF_INET , SOCK_STREAM , 0 );

    // // Filling server information 
	// server.sin_family = AF_INET;
	// server.sin_port = htons(PORT);
    // server.sin_addr.s_addr = inet_addr("35.247.224.237");
    // if (connect(s , (struct sockaddr *)&server , sizeof(server)) < 0){
    //     printf("Error connecting");
    // }else{
    //     printf("Connected");
    // }
    
    return; 
}

void send_message(float *msg){
    printf("Message sent.\n"); 
}

float recive_message(){
    return 10;
}

void close_server(){
    return;
}