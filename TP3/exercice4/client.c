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

#define MAX_SIZE 100

int main() 
{

    int commucationType = socket(PF_INET, SOCK_STREAM, 0);
    struct sockaddr_in serveurInformations;
    memset(&serveurInformations, 0, sizeof(serveurInformations));
    serveurInformations.sin_family = AF_INET;
    serveurInformations.sin_port = htons(1288);
    //Lampe adresse : 192.168.70.237
    //Localhost : 127.0.0.1    10.245.218.186
    char hostAdress[] = "10.245.218.186";

    int r1 = inet_pton(AF_INET, hostAdress, &serveurInformations.sin_addr);
    if(r1 == -1) {perror("inet_pton");  exit(1);}

    int r2 = connect(commucationType, (struct sockaddr*) &serveurInformations, sizeof(serveurInformations));
    if(r2 == -1) {perror("connect ");   exit(1);}
    printf("Connecté\n");

    //Protocole de communciation

    char buff[MAX_SIZE];
    memset(buff, 0, MAX_SIZE);

    char n[MAX_SIZE];
    memset(n, 0, MAX_SIZE);

    read(commucationType, buff, MAX_SIZE);
    write(1, buff, MAX_SIZE);

    read(0, n, MAX_SIZE);
    write(commucationType, n, MAX_SIZE);

    
    int nInt = atoi(n); 
    for(int i=0; i<nInt; i++)
    {
        char buff2[MAX_SIZE];   memset(buff2, 0, MAX_SIZE);
        read(0, buff2, MAX_SIZE);
        write(commucationType, buff2, MAX_SIZE);
    }

    char buff3[MAX_SIZE]; memset(buff3, 0, MAX_SIZE);
    read(commucationType, buff3, MAX_SIZE);
    write(1, buff3, MAX_SIZE);
    close(commucationType);
}