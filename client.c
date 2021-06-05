#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#define PORT 8080
#define QCOUNT 12

static char *answers[40] = {
    "entendido\n","itba\n","M4GFKZ289aku\n",
    "fk3wfLCm3QvS\n","too_easy\n",".RUN_ME\n",
    "K5n2UFfpFMUN\n","BUmyYq5XxXGt\n","u^v\n",
    "chin_chu_lan_cha\n","gdb_rules\n","normal\n"}; 

int main(int argc, char **argv) {
    struct sockaddr_in serv_addr;
    char *hello = "entendido\n";
    char buff[1024];
    int sock = socket(PF_INET, SOCK_STREAM, IPPROTO_IP);

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);
    inet_pton(AF_INET, "0.0.0.0", &serv_addr.sin_addr);
    connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr));
    int i;
    for (i=0; i < QCOUNT; i++)
        send(sock , answers[i] , strlen(answers[i]) , 0 );
}