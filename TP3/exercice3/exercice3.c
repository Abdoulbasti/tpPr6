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
#define MAX_CLIENT 2

/*
Les étapes de création d'un serveur : 
    1. socket sockaddr_in 
    2. bind 
    3. listen
    4. accept
    5. read/write
    6. close
*/


//Serveur avec un unique client qui se connecte
/*
int main() 
{
    int commucationType = socket(PF_INET, SOCK_STREAM, 0);
    struct sockaddr_in clientInformations;
    memset(&clientInformations, 0, sizeof(clientInformations));
    clientInformations.sin_family = AF_INET;
    clientInformations.sin_port = htons(2020);
    clientInformations.sin_addr.s_addr=htonl(INADDR_ANY); // Pour accepter n'importe quelle machines client
    char nomFonction[10];
    int r1 = bind(commucationType, (struct sockaddr *)&clientInformations,
                sizeof(clientInformations));
    if(r1 == -1) 
    {
        strcpy(nomFonction, "bind ");
        goto endProgram;
    }
    
    int r2=listen(commucationType, MAX_CLIENT);
    if(r2 == -1) {strcpy(nomFonction, "listen ");    goto endProgram;}

    socklen_t sockLenght = sizeof(clientInformations);

    int r3 = accept(commucationType, (struct sockaddr *) &clientInformations,
    &sockLenght);
    if(r3 == -1) 
    {
        strcpy(nomFonction, "accept ");
        goto endProgram;
    }

    while(1)
    {
        char buffSend[MAX_SIZE];
        char buffRecev[MAX_SIZE];
        memset(buffSend, 0, sizeof(buffSend));
        memset(buffRecev, 0, sizeof(buffRecev));

        int r4 = read(r3, buffRecev, MAX_SIZE);
        if(r4 == -1) 
        {
            strcpy(nomFonction, "read ");
            goto endProgram;
        }

        int r5 = write(r3 , buffRecev, MAX_SIZE);
        if(r5 == -1) 
        {
            strcpy(nomFonction, "write ");
            goto endProgram;
        }
    }
    endProgram :
        write(1, nomFonction, strlen(nomFonction));
        perror("error ");
        close(r3);
}*/


//Un serveur avec plusieurs client qui se connecte.

int main() 
{
    int commucationType = socket(PF_INET, SOCK_STREAM, 0);
    struct sockaddr_in clientInformations;
    memset(&clientInformations, 0, sizeof(clientInformations));
    clientInformations.sin_family = AF_INET;
    clientInformations.sin_port = htons(9999);
    clientInformations.sin_addr.s_addr=htonl(INADDR_ANY); // Pour accepter n'importe quelle machines client
    char nomFonction[10];
    int r1 = bind(commucationType, (struct sockaddr *)&clientInformations,
                sizeof(clientInformations));
    if(r1 == -1) perror("bind ");


    int r2=listen(commucationType, MAX_CLIENT);
    if(r2 == -1) perror("listen ");
    socklen_t sockLenght = sizeof(clientInformations);


    while(1)
    {
        int r3 = accept(commucationType, (struct sockaddr *) &clientInformations,
        &sockLenght);
        if(r3 == -1) perror("accept ");

        int retourFork = fork();
        if(retourFork == -1) perror("fork ");
        else if(retourFork == 0)
        {
            close(commucationType);
            //Protocole de communication
            close(r3);
            exit(0);
        }
        else 
        {
            close(r3);
        }
    }
} 