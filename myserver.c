#include <sys/socket.h>
#include <stdio.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#define PORT 8080
#define CHALLENGES 12

void checkError(int res, char *message);
void setupAndInitializeServer(int *serverFd, int *opt, struct sockaddr_in *address, int *addrlen, int *socketFd, FILE **socketFile);
void runChallenges(FILE *socketFile);
void clearScreen();
int isCorrectAns(char const *correctAns,char const *userAns);

int main(int argc, char const *argv[]) {
    int serverFd, socketFd, opt = 1;
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    FILE *socketFile;

    setupAndInitializeServer(&serverFd, &opt, &address, &addrlen, &socketFd, &socketFile);

    runChallenges(socketFile);


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

void runChallenges(FILE *socketFile){
    int  current = 0 ;
    char *buffer = NULL;
    size_t bufferSize = 0;

    while (current < CHALLENGES ) {

        if (getline(&buffer,&bufferSize,socketFile) > 0){ // TODO Maybe free buffer
            printf("%s\n",buffer);
            current += isCorrectAns("hola","hola");
        } else
            return;
    }

    printf("Felicitaciones, finalizaron el juego. Ahora deberán implementar el servidor que se comporte como el servidor provisto\n");

}

int isCorrectAns(char const *correctAns,char const *userAns) {
    if (strcmp(correctAns,userAns)) {
        printf("\nWrong answer: %s\n");
        sleep(2);
        return 0;
    }
    clearScreen();
    return 1;
}

void clearScreen() {
    printf("\033[1;1H\033[2J");
}

void checkError(int res, char *message) {
    if (res == -1){
        fprintf(stderr, "Error in function: %s",message);
        exit(-1);
    }
}