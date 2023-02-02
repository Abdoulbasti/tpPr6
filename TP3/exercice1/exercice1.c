#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>

#define MAX_SIZE 1000 


int main() 
{

    int commucationType = socket(PF_INET, SOCK_STREAM, 0);
    struct sockaddr_in6 serveurInformations;
    memset(&serveurInformations, 0, sizeof(serveurInformations));
    serveurInformations.sin6_family = AF_INET;
    serveurInformations.sin6_port = htons(4242);
    //Lampe adresse : 192.168.70.237
    //Localhost : 127.0.1.1, ::1

    char hostAdress[] = "::1"; //Adresse ipv6

    int r1 = inet_pton(AF_INET6, hostAdress, &serveurInformations.sin6_addr);
    if(r1 == -1) {goto endProgram;}

    int r2 = connect(commucationType, (struct sockaddr*) &serveurInformations, sizeof(serveurInformations));
    if(r2 == -1) {goto endProgram;}
    printf("Connecté\n");

    while(1)
    {
        char buffSend[MAX_SIZE];
        char buffRecev[MAX_SIZE];
        memset(buffSend, 0, sizeof(buffSend));
        memset(buffRecev, 0, sizeof(buffRecev));

        int r1 = read(0, buffSend,MAX_SIZE);
        if(r1 == -1) {goto endProgram;}


        int r2 = write(commucationType , buffSend, MAX_SIZE);
        if(r2 == -1) {goto endProgram;}
        int r3 = read(commucationType, buffRecev, MAX_SIZE);
        if(r3 == -1){goto endProgram;}

        int r4 = write(1, buffRecev, MAX_SIZE);
        if(r4 == -1){goto endProgram;}
    }
    endProgram :
        perror("error ");
        close(commucationType);
}