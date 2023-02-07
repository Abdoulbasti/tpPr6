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
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>


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
int main() 
{
    int commucationType = socket(PF_INET, SOCK_STREAM, 0);
    struct sockaddr_in clientInformations;
    memset(&clientInformations, 0, sizeof(clientInformations));
    clientInformations.sin_family = AF_INET;
    clientInformations.sin_port = htons(1288);
    clientInformations.sin_addr.s_addr=htonl(INADDR_ANY); // Pour accepter n'importe quelle machines client
    char nomFonction[10];
    int r1 = bind(commucationType, (struct sockaddr *)&clientInformations,
                sizeof(clientInformations));
    if(r1 == -1) 
    {
        strcpy(nomFonction, "bind ");
        perror(nomFonction);
        exit(1);
    }
    
    int r2=listen(commucationType, MAX_CLIENT);
    if(r2 == -1) 
    {
        strcpy(nomFonction, "listen ");   
        perror(nomFonction);
        exit(1);
    }

    socklen_t sockLenght = sizeof(clientInformations);

    int r3 = accept(commucationType, (struct sockaddr *) &clientInformations,
    &sockLenght);
    if(r3 == -1) 
    {
        strcpy(nomFonction, "accept ");
        perror(nomFonction);
        close(r3);
    }
    
    char buff[100];
    memset(buff, 0, 100);

    //Protocle de communication...
    write(r3, "DEBUT\n", strlen("DEBUT\n"));

    read(r3, buff, 100);


    char* fileName = "test";
    int fd = open(fileName, O_APPEND | O_WRONLY| O_CREAT, 0666);
    if(fd == -1)
    {
        strcpy(nomFonction, "open ");
        perror(nomFonction);
        close(r3);
    }
    int n = atoi(buff);
    for(int i=0; i<n; i++)
    {
        char buff2[100];
        memset(buff2, 0, 100);
        read(r3, buff2, 100);
        
        int nb = write(fd, buff2, strlen(buff2));
        if(nb == -1)
        {
            exit(1);
        }
    }
    write(r3, "BIEN REÇU\n", strlen("BIEN REÇU\n"));
    close(r3);
    close(fd);
}