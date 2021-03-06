// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include <sys/socket.h>
#include <stdio.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <math.h>
#include <unistd.h>
#include "random.h"

static void checkError(int res, char *function);
static void setupAndInitializeServer(int *serverFd, int *opt, struct sockaddr_in *address, int *addrlen, int *socketFd, FILE **socketFile);
static void runChallenges(FILE *socketFile);
static void clearScreen();
static int isCorrectAns(char const *correctAns,char const *userAns);
static void printHintMessage();
static void printQMessage();
static void challenge4();
static void challenge7();
static void challenge8();
static void challenge10();
static void challenge11();
static void challenge12();

#define STDOUT 1
#define STDERR 2
#define PORT 8080
#define CHALLENGES 12
#define LOWER 180
#define UPPER 220
#define MIN_CHAR 33
#define MAX_CHAR 126
#define PI 3.14159265358979323846

typedef struct {
    char *answer;
    char *hint;
    char *question;
    void (*func)(void);
}Challenge;

static Challenge allChallenges[CHALLENGES] = {
    {
        "entendido\n",
        "Bienvenidos al TP3 y felicitaciones, ya resolvieron el primer acertijo.\n\nEn este TP deberán finalizar el juego que ya comenzaron resolviendo los desafíos de cada nivel.\nAdemás tendrán que investigar otras preguntas para responder durante la defensa.\nEl desafío final consiste en crear un programa que se comporte igual que yo, es decir, que provea los mismos desafíos y que sea necesario hacer lo mismo para resolverlos. No basta con esperar la respuesta.\nAdemás, deberán implementar otro programa para comunicarse conmigo.\n\nDeberán estar atentos a los easter eggs.\nPara verificar que sus respuestas tienen el formato correcto respondan a este desafío con la palabra 'entendido\\n'",
        "¿Cómo descubrieron el protocolo, la dirección y el puerto para conectarse?",
        NULL
    },
    {
        "itba\n",
        "The Wire S1E5\n5295 888 6288\n",
        "¿Qué diferencias hay entre TCP y UDP y en qué casos conviene usar cada uno?",
        NULL
    },
    {
        "M4GFKZ289aku\n",
        "https://ibb.co/tc0Hb6w\n",
        "¿El puerto que usaron para conectarse al server es el mismo que usan para mandar las respuestas? ¿Por qué?",
        NULL
    },
    {
        "fk3wfLCm3QvS\n",
        "EBADF... \n",
        "¿Qué útil abstracción es utilizada para comunicarse con sockets? ¿se puede utilizar read(2) y write(2) para operar?",
        challenge4
    },
    {
        "too_easy\n",
        "respuesta = strings:105 \n",
        "¿Cómo garantiza TCP que los paquetes llegan en orden y no se pierden?",
        NULL
    },
    {
        ".RUN_ME\n",
        ".rela.plt .init .text ? .fini .rodata .eh_frame_hdr",
        "Un servidor suele crear un nuevo proceso o thread para atender las conexiones entrantes. ¿Qué conviene más?",
        NULL
    },
    {
        "K5n2UFfpFMUN\n",
        "Filter error",
        "¿Cómo se puede implementar un servidor que atienda muchas conexiones sin usar procesos ni threads?",
        challenge7
    },
    {
        "BUmyYq5XxXGt\n",
        "¿? \n",
        "¿Qué aplicaciones se pueden utilizar para ver el tráfico por la red?",
        challenge8
    },
    {
        "u^v\n",
        "Latexme\n\nSi\n \\mathrm{d}y = u^v{\\cdot}(v'{\\cdot}\\ln{(u)}+v{\\cdot}\\frac{u'}{u})\nentonces\ny = ",
        "Sockets es un mecanismo de IPC. ¿Qué es más eficiente entre sockets y pipes?",
        NULL
    },
    {
        "chin_chu_lan_cha\n",
        "quine\n",
        "¿Cuáles son las características del protocolo SCTP?",
        challenge10
    },
    {
        "gdb_rules\n",
        "b gdbme y encontrá el valor mágico\n",
        "¿Qué es un RFC?",
        challenge11
    },
    {
        "normal\n",
        "Me conoces\n",
        "¿Fue divertido?",
        challenge12
    }
};


//data hidden in binary
static const char* tooez = "too_easy";

static const char* easter_egg =  
"_______________________\n"
"< ESTO ES UN EASTER_EGG >\n"
 "-----------------------\n"
    "    \\   ^__^\n"
    "    \\  (oo)\\_______\n"
    "        (__)\\       )\\/\\\n"
    "           ||----w |\n"
    "           ||     ||\n";

void foo() __attribute__((section(".RUN_ME")));

void foo() {return;};

int main(int argc, char const *argv[]) {
    int serverFd, socketFd, opt = 1;
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    FILE *socketFile;

    setupAndInitializeServer(&serverFd, &opt, &address, &addrlen, &socketFd, &socketFile);
    runChallenges(socketFile);

    fclose(socketFile);
    close(serverFd);
    close(socketFd);

    return 0;
}

static void setupAndInitializeServer(int *serverFd, int *opt, struct sockaddr_in *address, int *addrlen, int *socketFd, FILE **socketFile){
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

static void runChallenges(FILE *socketFile){
    int  current = 0 ;
    char *buffer = NULL;
    size_t bufferSize = 0;
    randomize();

    while (current < CHALLENGES ) {
        clearScreen();
        printHintMessage();
        printf("%s\n",allChallenges[current].hint);

        if (allChallenges[current].func!=NULL) allChallenges[current].func();

        printQMessage();
        printf("%s\n\n",allChallenges[current].question);

        if (getline(&buffer,&bufferSize,socketFile) > 0){
            current += isCorrectAns(allChallenges[current].answer,buffer);
        } else {
            perror("getline");
            return;
        }
    }

    clearScreen();
    printf("Felicitaciones, finalizaron el juego. Ahora deberán implementar el servidor que se comporte como el servidor provisto\n\n");
    free(buffer);
}

static void challenge4() {
    if(write(13, "La respuesta es fk3wfLCm3QvS\n",30) == -1)
        perror("write");
}

static void challenge7() {
    char *theAnsIs = "La respuesta es K5n2UFfpFMUN";
    int ansLen = strlen(theAnsIs);
    int num = randInt(LOWER, UPPER);
    char aux[2];
    aux[1] = '\0';
    int i,j;
    putchar('\n');
    for (i=0, j=0 ; i < num || j < ansLen ; ) {
        if (randInt(0, 4) == 0 && j < ansLen) {
            aux[0] = theAnsIs[j++];
            write(STDOUT, aux, 1);
        } else {
            aux[0] = randInt(MIN_CHAR, MAX_CHAR);
            write(STDERR, aux, 1);
            i++;
        }
    }
    putchar('\n');
}

static void challenge8() {
    printf("\033[30;40mLa respuesta es BUmyYq5XxXGt\033[0m\n");
}

static void challenge10() {
    if (system("gcc quine.c -o quine") != 0){
        puts("\n\nENTER para reintentar.\n");
        return;
    }
    puts("¡Genial!, ya lograron meter un programa en quine.c, veamos si hace lo que corresponde.");

    if (system("./quine | diff - quine.c") != 0) {
        printf("\n%s\n", "diff encontró diferencias.");
        puts("\n\nENTER para reintentar.\n");
    } else
        printf("La respuesta es chin_chu_lan_cha\n");
}

static void gdbme(){
    if (getpid() == 0x12345678) {
        printf("La respuesta es gdb_rules\n");
    }
}

static void challenge11() {
    gdbme();
}

static void challenge12() {
    double x, y, aux;
    int i;
    for ( i = 0; i < 1000; i++) {
        x = rand() / ((double)RAND_MAX + 1);
        y = rand() / ((double)RAND_MAX + 1);
        aux = sqrt(-2 * log(x)) * cos(2 * PI * y);
        printf("%.6f ", aux);
    }
    printf("\n");
}

static int isCorrectAns(char const *correctAns,char const *userAns) {
    if (strcmp(correctAns,userAns)!=0) {
        printf("\nWrong answer: %s\n",userAns);
        sleep(2);
        return 0;
    }
    return 1;
}

static void printHintMessage() {
    printf("\n------------- DESAFIO -------------\n");
}

static void printQMessage() {
    printf("\n----- PREGUNTA PARA INVESTIGAR -----\n");
}

static void clearScreen() {
    printf("\033[1;1H\033[2J");
}

static void checkError(int res, char *functionName) {
    if (res == -1){
        perror(functionName);
        exit(-1);
    }
}
