#ifndef CLIENT_H
#define CLIENT_H

typedef enum {
    INIT_CONNECTION = 0,
    TRUSTED_DATA = 1,
    UNTRUSTED_DATA = 2
} PackageType;

typedef enum {
    POSITION = 0,
} EventType;

struct ServerPackage{
    PackageType type;
    unsigned int package_identifier; //Package id or type
    unsigned int size;
    void *buffer;
};

void create_connection();
void send_message(void *buffer, unsigned int size);
int recive_message(void *buffer, unsigned int size);
void close_server();

void server_loop();

// struct InitPackage{
//     unsigned int game_type;
// }

typedef enum {
    PLAYER_WON = 0
} TrustedEventType;

struct TrustedPackage
{
    unsigned int player_id;
    TrustedEventType event_type;
};


#endif //CLIENT_H