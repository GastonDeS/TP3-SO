#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>
#define PORT 8080
#define QCOUNT 12

static char *answers[40] = {
    "entendido\n","itba\n","M4GFKZ289aku\n",
    "fk3wfLCm3QvS\n","too_easy\n",".RUN_ME\n",
    "K5n2UFfpFMUN\n","BUmyYq5XxXGt\n","u^v\n",
    "chin_chu_lan_cha\n","gdb_rules\n","normal\n"}; 

int main(int argc, char **argv) {
    struct sockaddr_in serv_addr;
    int sock ;

    if ((sock = socket(PF_INET, SOCK_STREAM, IPPROTO_IP)) < 0) {
        printf("\n Socket creation error \n");
        return -1;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    if(inet_pton(AF_INET, "0.0.0.0", &serv_addr.sin_addr)<=0) {
        printf("\nInvalid address/ Address not supported \n");
        return -1;
    }
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        printf("\nConnection Failed \n");
        return -1;
    }

    int i;
    for (i=0; i < QCOUNT; i++)
        send(sock , answers[i] , strlen(answers[i]) , 0 );

    return 0;
}