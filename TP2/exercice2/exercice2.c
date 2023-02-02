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

/*
int inet_aton(const char *cp, struct in_addr *pin);
int inet_pton(int af, const char * restrict src, void * restrict dst);
int socket(int domaine, int type, int protocole);
int connect(int socket, const struct sockaddr *adresse, socklen_t longueur);
int close(int socket);
ssize_t send(int socket,const void *tampon, size_t longueur,int options);
ssize_t recv(int socket,void *tampon, size_t longueur,int options);

adresse ipv4 of lampe host : 192.168.70.237
*/

int main()
{
    //Question2 
    /*
    etapes de création d'un client : 
    1. socket (socket(), struct sockaddr_in)
    2. connect
    3. send-recev 
    4. close 
    */
   /*
   1. Pourquoi si on fait la meme chose avec l'adresse de ma machine cela ne fonctionne pas.
   2. 
   */
    int commucationType = socket(PF_INET, SOCK_STREAM, 0);
    struct sockaddr_in serveurInformations;
    memset(&serveurInformations, 0, sizeof(serveurInformations));
    serveurInformations.sin_family = AF_INET;
    serveurInformations.sin_port = htons(4242);
    char hostAdress[] = "192.168.70.237";

    int r1 = inet_pton(AF_INET, hostAdress, &serveurInformations.sin_addr);
    if(r1 == -1) {perror("inet_pton");}

    int r2 = connect(commucationType, (struct sockaddr*) &serveurInformations, sizeof(serveurInformations));
    if(r2 == -1) {perror("connect ");}
    char buf[MAX_SIZE];
    memset(buf, 0, MAX_SIZE);
    ssize_t nombreCaractereRecu = recv(commucationType, buf, MAX_SIZE, 0); 
    //ssize_t nombreCaractereRecu = read(commucationType, buf, MAX_SIZE);
    if(nombreCaractereRecu ==-1){perror("recv ");}
    if(nombreCaractereRecu == 0){printf("Le serveur a tterminer proprement la connexion \n");}

    printf("Le message reçu du serveur est le suivant : %s\n", buf);

    close(commucationType);
}