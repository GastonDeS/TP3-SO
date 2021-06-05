#include <sys/socket.h>
#include <stdio.h>
#include <netinet/in.h>
#include <stdlib.h>

#define PORT 8080

void checkError(int res, char *message);
void setupAndInitializeServer(int *serverFd, int *opt, struct sockaddr_in *address, int *addrlen, int *socketFd, FILE **socketFile);

int main(int argc, char const *argv[]) {
    int serverFd, socketFd, opt = 1;
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    FILE *socketFile;

    setupAndInitializeServer(&serverFd, &opt, &address, &addrlen, &socketFd, &socketFile);

    

    return 0;
}

void setupAndInitializeServer(int *serverFd, int *opt, struct sockaddr_in *address, int *addrlen, int *socketFd, FILE **socketFile){
    checkError(*serverFd = socket(AF_INET, SOCK_STREAM, 0),"socket");
    checkError(setsockopt(*serverFd, SOL_SOCKET, SO_REUSEADDR, opt, sizeof(*opt)),"setsockopt");

    address->sin_family = AF_INET;
    address->sin_addr.s_addr = INADDR_ANY;
    address->sin_port = htons(PORT);

    checkError(bind(*serverFd, (struct sockaddr *)address, sizeof(*address)), "bind"); 
    checkError(listen(*serverFd, 1),"listen");
    checkError(*socketFd = accept(*serverFd, (struct sockaddr *)address, (socklen_t *) addrlen),"accept");

    if ((*socketFile = fdopen(*socketFd, "r")) == NULL) {
        checkError(-1,"fdopen");
    }
}

void checkError(int res, char *message) {
    if (res == -1){
        fprintf(stderr, "Error in function: %s",message);
        exit(-1);
    }
}