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
#include <pthread.h>

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

/*Nombre aleatoire entre 0 et 65535*/

 int aleatoire();
 void* protocole(void* s);

 int aleatoire()
 {
    srand(time(NULL));
    return rand() % 100;
 }

 void* protocole(void* s)
 {
    int sockClient = *((int *)s);
    int r = 20;
    int maxOccurrent = 20;
    int taille = 10;
    int n = aleatoire();
    for(int i =0; i<maxOccurrent; i++)
    {
        char buff[taille];
        memset(buff, 0, taille);
        int charReaded = read(sockClient, buff, taille);
        if(charReaded == -1) perror(" read ");
        int k = atoi(buff);

        if(k > n)
        {
            r--;
            char stockerProtocole[15];
            memset(stockerProtocole, 0, 15);            
            sprintf(stockerProtocole, "PLUS %d\n", r);  
            write(1, stockerProtocole, strlen(stockerProtocole)); 
        }
        if(k < n)
        {
            r--;
            char stockerProtocole[15];
            memset(stockerProtocole, 0, 15);
            sprintf(stockerProtocole, "MOINS %d\n", r);
            write(1, stockerProtocole, strlen(stockerProtocole));
        }
        if(k == n)
        {
            write(1, "GAGNE \n", strlen("GAGNE \n"));
            break;         
        }
        if (r == 0 && k != n)
        {
            write(1, "PERDU \n", taille);
            break;
        }
    }
    close(sockClient);
    return NULL;
 }


/*
//MULTI-CLIENT AVEC LES THREADS
int main() 
{
    int commucationType = socket(PF_INET, SOCK_STREAM, 0);
    struct sockaddr_in clientInformations;
    memset(&clientInformations, 0, sizeof(clientInformations));
    clientInformations.sin_family = AF_INET;
    clientInformations.sin_port = htons(3356);
    clientInformations.sin_addr.s_addr=htonl(INADDR_ANY); // Pour accepter n'importe quelle machines client
    char nomFonction[10];
    int r1 = bind(commucationType, (struct sockaddr *)&clientInformations,
                sizeof(clientInformations));
    if(r1 == -1) perror("bind ");

    int r2=listen(commucationType, MAX_CLIENT);
    if(r2 == -1) { perror("listen ");}
    socklen_t sockLenght = sizeof(clientInformations);


    //PROTOCOLE DE COMMUNICATION....
    while(1)
    {
        int r3 = accept(commucationType, (struct sockaddr *) &clientInformations,
        &sockLenght);
        if(r3 == -1) 
        {
            strcpy(nomFonction, "accept ");
            perror("accept ");
        }
        pthread_t thread;
        int threadReturn = pthread_create(&thread, NULL, protocole, &r3);
        if(threadReturn != 0)   perror("pthread_create ");
    }
    return 0;
}*/




/* MULTI-CLIENTS AVEC LES FORK*/
int main() 
{
    int commucationType = socket(PF_INET, SOCK_STREAM, 0);
    struct sockaddr_in clientInformations;
    memset(&clientInformations, 0, sizeof(clientInformations));
    clientInformations.sin_family = AF_INET;
    clientInformations.sin_port = htons(5127);
    clientInformations.sin_addr.s_addr=htonl(INADDR_ANY); // Pour accepter n'importe quelle machines client
    char nomFonction[10];
    int r1 = bind(commucationType, (struct sockaddr *)&clientInformations,
                sizeof(clientInformations));
    if(r1 == -1) perror("bind ");

    int r2=listen(commucationType, MAX_CLIENT);
    if(r2 == -1) { perror("listen ");}
    socklen_t sockLenght = sizeof(clientInformations);


    //PROTOCOLE DE COMMUNICATION....
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
            int r = 20;
            int maxOccurrent = 20;
            int taille = 10;
            int n = aleatoire();
            for(int i =0; i<maxOccurrent; i++)
            {
                char buff[taille];
                memset(buff, 0, taille);
                int charReaded = read(r3, buff, taille);
                if(charReaded == -1) perror(" read ");
                int k = atoi(buff);

                if(k > n)
                {
                    r--;
                    char stockerProtocole[15];
                    memset(stockerProtocole, 0, 15);            
                    sprintf(stockerProtocole, "PLUS %d\n", r);  
                    write(1, stockerProtocole, strlen(stockerProtocole)); 
                }
                if(k < n)
                {
                    r--;
                    char stockerProtocole[15];
                    memset(stockerProtocole, 0, 15);
                    sprintf(stockerProtocole, "MOINS %d\n", r);
                    write(1, stockerProtocole, strlen(stockerProtocole));
                }
                if(k == n)
                {
                    write(1, "GAGNE \n", strlen("GAGNE \n"));
                    break;         
                }
                if (r == 0 && k != n)
                {
                    write(1, "PERDU \n", taille);
                    break;
                }
            }


            close(r3);
            exit(0);
        }
        else 
        {
            close(r3);
        }
    }

    return 0;
}

/*
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
*/