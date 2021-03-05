#include "Manager.h"
#include "Client.h"

int main() {
    create_connection();
    M_Init();
    M_Run();
    M_Cleanup();
    return 0;
}