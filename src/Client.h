#ifndef CLIENT_H
#define CLIENT_H

void create_connection();
void send_message(float *msg);
float recive_message();
void close_server();

#endif //CLIENT_H