#include "html.h"
#include <string.h>
#include <sys/socket.h>
#include <iostream>
#include <sys/stat.h> //for getting file stats
#include <sys/fcntl.h> //for 0_RDONLY
#include <sys/sendfile.h> //for sendfile() function
#include <unistd.h>
#include <dirent.h>

using namespace std;
char command[5], filename[20];

int logic(int *soc, char buf[100], int valend){
    struct stat obj; //to use stat.h
    int filehandle, size;
    
    sscanf(buf, "%s", command);

    if(!strcmp(command,"get"))
    {
        sscanf(buf, "%s%s", filename, filename);
        stat(filename, &obj);
        filehandle = open(filename, O_RDONLY);
        size = obj.st_size;
        if(filehandle == -1)
            size = 0;
        send(*soc, &size, sizeof(int), 0);
        if(size)
            sendfile(*soc, filehandle, NULL, size);
        return 0;
    }
    else if(!strcmp(command, "post"))
    {
        int c = 0, len;
        char *f;
        sscanf(buf+strlen(command), "%s", filename);
        recv(*soc, &size, sizeof(int), 0);
        int i = 1;
        while(1)
            {
            filehandle = open(filename, O_CREAT | O_EXCL | O_WRONLY, 0666);
            if(filehandle == -1)
            {
                sprintf(filename + strlen(filename), "%d", i);
            }
            else
                break;
            }
        f = (char*)malloc(size);
        recv(*soc, f, size, 0);
            c = write(filehandle, f, size);
        close(filehandle);
        send(*soc, &c, sizeof(int), 0);
        return 0;
    }

    else if (!strcmp(command,"ls")) {
        char str[1024] = ":: ";
        DIR *dir;
        struct dirent *ent;
        if ((dir = opendir ("./")) != NULL) {
            while ((ent = readdir (dir)) != NULL) {
            strcat(str, " | ");
            strcat(str,ent->d_name);
            }
        closedir(dir);
        }
        size = strlen(str);
        send(*soc, &str, size, 0);
    }
    

    else if(!strcmp(command, "bye") || !strcmp(command, "quit"))
    {
        printf("quit %d\n",errno);   
        int a = 1;
        send(*soc, &a, sizeof(int), 0);
        return -1;
    }
}