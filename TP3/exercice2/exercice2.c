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


/*Nombre aleatoire entre 0 et 65535*/
 int aleatoire()
 {
    srand(time(NULL));
    return rand() % 65535;
 }

int main() 
{
    int commucationType = socket(PF_INET, SOCK_STREAM, 0);
    struct sockaddr_in clientInformations;
    memset(&clientInformations, 0, sizeof(clientInformations));
    clientInformations.sin_family = AF_INET;
    clientInformations.sin_port = htons(7979);
    clientInformations.sin_addr.s_addr=htonl(INADDR_ANY); // Pour accepter n'importe quelle machines client
    char nomFonction[10];
    int r1 = bind(commucationType, (struct sockaddr *)&clientInformations,
                sizeof(clientInformations));
    if(r1 == -1) perror("bind ");



    int r2=listen(commucationType, MAX_CLIENT);
    if(r2 == -1) { perror("listen ");}
    socklen_t sockLenght = sizeof(clientInformations);

    int r = 20;
    while(1)
    {
        int r3 = accept(commucationType, (struct sockaddr *) &clientInformations,
        &sockLenght);
        if(r3 == -1) 
        {
            strcpy(nomFonction, "accept ");
            perror("accept ");
        }

        char stockerProtocole[15];
        int retourFork = fork();
        int taille = 10;
        if(retourFork == -1) perror(" fork ");
        else if(retourFork == 0)
        {
            close(commucationType);

            //PROTOCOLE DE COMMUNICATION....
            int n = aleatoire();
            for(int i =0; i<r; i++)
            {
                char buff[taille];
                int charReaded = read(r3, buff, taille);
                if(charReaded == -1) perror(" read ");
                int k = atoi(buff);


                if(k < n)
                {
                    r--;
                    memset(stockerProtocole, 0, 15);
                    sprintf(stockerProtocole, "PLUS %d\n", r);
                    write(1, stockerProtocole, taille);
                    continue;
                }
                else if(k > n)
                {
                    r--;
                    memset(stockerProtocole, 0, 15);
                    sprintf(stockerProtocole, "MOINS %d\n", r);
                    write(1, stockerProtocole, taille);
                    continue;
                }
                else if (k == n)
                {
                    r--;
                    write(1, "GAGNE \n", taille);
                    close(r3);
                    exit(0);
                }
                if( k != n && r == 0)
                {
                    write(1, "PERDU \n", taille);
                    close(r3);
                    exit(0);
                }
            }
        }
        else 
        {
            close(r3);
        }
    }
} 
