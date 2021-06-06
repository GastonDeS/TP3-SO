#include <sys/socket.h>
#include <stdio.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

void checkError(int res, char *message);
void setupAndInitializeServer(int *serverFd, int *opt, struct sockaddr_in *address, int *addrlen, int *socketFd, FILE **socketFile);
void runChallenges(FILE *socketFile);
void clearScreen();
int isCorrectAns(char const *correctAns,char const *userAns);
void printInitialInstructions();

#define PORT 8080
#define CHALLENGES 12
#define ANSWER_SIZE 25
#define HINT_SIZE 256
#define QUESTION_SIZE 1024

typedef struct {
    char answer[ANSWER_SIZE];
    char hint[HINT_SIZE];
    char question[QUESTION_SIZE];
    void (*func)(void);
}Challenge;

Challenge allChallenges[CHALLENGES] = {
    {
        "entendido\n",
        "Para verificar que sus respuestas tienen el formato correcto respondan a este desafío con la palabra 'entendido\\\n'", 
        "¿Cómo descubrieron el protocolo, la dirección y el puerto para conectarse?",
        NULL
    },
    {
        "itba\n",
        "The Wire S1E5 5295 888 6288", 
        "¿Qué diferencias hay entre TCP y UDP y en qué casos conviene usar cada uno?",
        NULL
    },
    {
        "M4GFKZ289aku\n", 
        "https://ibb.co/tc0Hb6w", 
        "¿El puerto que usaron para conectarse al server es el mismo que usan para mandar las respuestas? ¿Por qué?", 
        NULL
    },
    {
        "fk3wfLCm3QvS\n", 
        "EBADF... \n", 
        "¿Qué útil abstracción es utilizada para comunicarse con sockets? ¿se puede utilizar read(2) y write(2) para operar?", 
        NULL //CHALLENGE 4
    },
    {
        "too_easy\n", 
        "respuesta = strings:277", 
        "¿Cómo garantiza TCP que los paquetes llegan en orden y no se pierden?",
        NULL
    },
    {
        ".RUN_ME\n", 
        ".data .bss .comment ? .shstrtab .symtab .strtab", 
        "Un servidor suele crear un nuevo proceso o thread para atender las conexiones entrantes. ¿Qué conviene más?",
        NULL
    },
    {
        "K5n2UFfpFMUN\n", 
        "Filter error", 
        "¿Cómo se puede implementar un servidor que atienda muchas conexiones sin usar procesos ni threads?",
        NULL //CHALLENGE 7
    },
    {
        "BUmyYq5XxXGt\n", 
        "¿? \n", 
        "¿Qué aplicaciones se pueden utilizar para ver el tráfico por la red?",
        NULL
    },
    {
        "u^v\n",
        "Latexme\n\nSi\n\\mathrm{d}y = u^v{\\cdot}(v'{\\cdot}\\ln{(u)}+v{\\cdot}\\frac{u'}{u})\nentonces\ny = ", 
        "Sockets es un mecanismo de IPC. ¿Qué es más eficiente entre sockets y pipes?",
        NULL
    },
    {
        "chin_chu_lan_cha\n", 
        "quine\n", 
        "¿Cuáles son las características del protocolo SCTP?",
        NULL //CHALLENGE10
    },
    {
        "gdb_rules\n", 
        "b gdbme y encontrá el valor mágico\n", 
        "¿Qué es un RFC?",
        NULL //CHALLENGE11
    },
    {
        "normal\n", 
        "Me conoces",
        "¿Se divirtieron?",
        NULL //CHALLENGE12
    }
};

char const *start = "Bienvenidos al TP3 y felicitaciones, ya resolvieron el primer acertijo.\nEn este TP deberán finalizar el juego que ya comenzaron resolviendo los desafíos de cada nivel.\nAdemás tendrán que investigar otras preguntas para responder durante la defensa.\nEl desafío final consiste en crear un programa que se comporte igual que yo, es decir, que provea los mismos desafíos y que sea necesario hacer lo mismo para resolverlos. No basta con esperar la respuesta.\nAdemás, deberán implementar otro programa para comunicarse conmigo.\nDeberán estar atentos a los easter eggs.\n";

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

    printInitialInstructions();

    while (current < CHALLENGES ) {
        printf("%s",allChallenges[current].hint);
        printf("%s",allChallenges[current].question);

        if (getline(&buffer,&bufferSize,socketFile) > 0){ // TODO Maybe free buffer
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

void printInitialInstructions(){
    printf("%s",start);
}

void checkError(int res, char *message) {
    if (res == -1){
        fprintf(stderr, "Error in function: %s",message);
        exit(-1);
    }
}