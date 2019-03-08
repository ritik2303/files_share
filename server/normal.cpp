#include "html.h"
#include "wsHandshake.h"
#include <string.h>
#include <sys/socket.h>
#include <iostream>

using namespace std;

void handleRequest(string request);


char *server_message2 =
"Hello client terminal";

void normal(int soc, char buf[1024], int valend){
    printf("\nhello for client terminal\n");
    printf("buffer  %s\n",buf);
    stringstream requestStream;
    printf("valend %d\n",valend);

    while (valend > 0){
        buf[valend] = '\0';
        requestStream.write(buf, valend);
        valend = recv(soc , buf, 256, 0);
    }
    
    if (valend == 0) {
        cout << "Errno: " << errno << "\n";
        // cout << "Client " << i << " quit. Status: 0\n";
    }
    
    if (requestStream.str().length() > 0) {
        // cout << "Client " << i << ": Status: " << valend << "\n";
        cout << "Data: " << requestStream.str() << "\n";
    
        handleRequest(requestStream.str());
    }
    
    // string request = "GET";

    // wsHandshake handshake(request);
    // handshake.generateResponse();
        
    // if (!handshake.isSuccess()) {
    //     printf("handshake failed\n");
    // }
    // else
    // {
    //     printf("handshake SUCCESS\n");
    // }
     

    // send(soc, server_message2, strlen(server_message2), 0);
}

void handleRequest(string request) {
    if (request[0] == 'G' &&
        request[1] == 'E' &&
        request[2] == 'T') {
        
        // handshake!
        printf("handshake");
        
        wsHandshake handshake(request);
        handshake.generateResponse();
        
        if (!handshake.isSuccess()) {
            printf("handshake failed");
            return;
        }
    }
}