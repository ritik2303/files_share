#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <errno.h>  
#include <arpa/inet.h>    //close 
#include <string>
#include <sys/stat.h> //for getting file stats
#include <unordered_map> 
#include <iostream>
using namespace std;
#include "html.h"
#include "normal.h"
#include "logic.h"
#define PORT 8550

char *server_message =
"HTTP/1.1 200 OK \n\
\n\
<html>\n\
<head>\n\
	<title>Title: Fserver</title>\n\
<head>\n\
<body>\n\
	<center>\n\
		<h1>\n\
			Hello from server!\n\
		</h1>\n\
	</center>\n\
</body>\n\
</html>";

void closeConn(int soc, struct sockaddr_in address,int* list_soc);

int main(int argc, char const *argv[]) {

    struct stat obj; //to use stat.h
    char buf[100], command[5], filename[20];
    int k, i, len, c;

    
    int master_socket , new_socket , client_socket[30] ,  
          max_clients = 6 , activity , valread , sd;   struct sockaddr_in address;
    int opt = 1;
    int max_sd;
    int addrlen = sizeof(address);
    char buffer[1024] = {0};

    //set of socket descriptors  
    fd_set readfds;   
    
    //initialise all client_socket[] to 0 so not checked  
    for (i = 0; i < max_clients; i++)   
    {   
        client_socket[i] = 0;   
    }

    // Creating socket file descriptor 
    if ((master_socket = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    // Forcefully attaching socket to the port 8000
    if (setsockopt(master_socket, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    // Forcefully attaching socket to the port 8000
    if (bind(master_socket, (struct sockaddr *)&address, sizeof(address))<0) {
        perror("Binding error");
        exit(EXIT_FAILURE);
    }
    if (listen(master_socket, 5) < 0) {
        perror("Listen failed");
        exit(EXIT_FAILURE);
    }

    //accept the incoming connection  
    puts("Waiting for connections ...");

    while(true){
     //clear the socket set  
        FD_ZERO(&readfds);   
     
        //add master socket to set  
        FD_SET(master_socket, &readfds);   
        max_sd = master_socket;  
             
        //add child sockets to set  
        for ( i = 0 ; i < max_clients ; i++)   
        {
            //socket descriptor  
            sd = client_socket[i];   
                 
            //if valid socket descriptor then add to read list  
            if(sd > 0)   
                FD_SET( sd , &readfds);   
                 
            //highest file descriptor number, need it for the select function  
            if(sd > max_sd)   
                max_sd = sd;   
        }   
     
        //wait for an activity on one of the sockets , timeout is NULL ,  
        //so wait indefinitely  
        activity = select( max_sd + 1 , &readfds , NULL , NULL , NULL);   
       
        printf("activity: %d\n",activity); 
        if ((activity < 0) && (errno!=EINTR))   
        {   
            printf("select error");   
        }
 
        //If something happened on the master socket ,  
        //then its an incoming connection  
        printf("%c",FD_ISSET(master_socket, &readfds));

        if (FD_ISSET(master_socket, &readfds))   
        {   
            if ((new_socket = accept(master_socket,  
                    (struct sockaddr *)&address, (socklen_t*)&addrlen))<0)   
            {   
                perror("accept");   
                exit(EXIT_FAILURE);   
            }   
             
            //inform user of socket number - used in send and receive commands  
            printf("New connection , socket fd is %d , ip is : %s , port : %d \n" , new_socket , inet_ntoa(address.sin_addr) , ntohs 
                  (address.sin_port));   
           
            // send new connection greeting message  
            // if( send(new_socket, server_message, strlen(server_message), 0) != strlen(server_message) )   
            // {   
            //     perror("send");   
            // }   
                 
            // puts("Welcome message sent successfully");   

            //add new socket to array of sockets  
            for (i = 0; i < max_clients; i++)   
            {
                //if position is empty  
                if( client_socket[i] == 0 )   
                {   
                    client_socket[i] = new_socket;   
                    printf("Adding to list of sockets as %d\n" , i);   
                         
                    break;   
                }   
            }   
        }   
             
        //else its some IO operation on some other socket 
        for (i = 0; i < max_clients; i++)   
        {   
            sd = client_socket[i]; 

            if (FD_ISSET( sd , &readfds))   
            {   
                valread = recv(sd, buf, 100, 0);
                printf("169  %d\n",valread);

                if (valread <= 0){
                    printf("error 178 %d\n",errno);   
                    //Somebody disconnected , get his details and print  
                    closeConn(sd, address, &client_socket[i]);

                }else
                {
                    buf[valread] = '\0';   

                    string work(buf);
                    string str1 = "text/html"; 
                    size_t found = work.find(str1); 
                    if (found != string::npos) 
                    { // html output

                        html(sd, buf, valread);
                    }
                    else
                    {
                        // non html
                        normal(sd, buf, valread);
                    }
                    
                    if(logic(&sd, buf, valread) == -1 ){
                        closeConn(sd, address, &client_socket[i]);                    
                    }
                    
                }
            }   
        }   
    }   
    printf("202\n");    
         
    return 0;   
}

void closeConn(int soc, struct sockaddr_in address,int* list_soc){
    int addrlen = sizeof(address);
    getpeername(soc , (struct sockaddr*)&address , \ 
            (socklen_t*)&addrlen);   
        printf("Host disconnected , ip %s , port %d \n" ,  
            inet_ntoa(address.sin_addr) , ntohs(address.sin_port));   
            
        //Close the socket and mark as 0 in list for reuse  
        close( soc );   
        *list_soc = 0;   
}
