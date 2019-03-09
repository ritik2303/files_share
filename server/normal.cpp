#include <string.h>
#include <sys/socket.h>
#include <iostream>

using namespace std;

char *server_message2 =
"Hello client terminal";

void normal(int soc, char buf[1024], int valend){
    printf("\nhello for client terminal\n");
    // send(soc, server_message2, strlen(server_message2), 0);
}

