#include <arpa/inet.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

int server_1p();
int server_1p_fork();
int server_1p_threads();


int main(int argn,char** args) {
  if(argn<=1|| strcmp(args[1],"-1p")==0){
    server_1p();
  }
}

/***************LES FONCTIONS DE JEU ******************/

/**
 * fait tourner une partie pour un joueur dont le socket est passé en argument
 */
void game_1p(int sock) {
  srand(time(NULL) + sock);
  
  // une valeur aléatoire mystere entre 0 et 65536
  unsigned short int n = rand() % (1 << 16); 
  printf("nb mystere pour partie socket %d = %d\n", sock, n);

  /*uint16_t*/
  unsigned short int guess;  // le nb proposé par le joueur, sur 2 octets
  int taille = 0;
  int tentatives = 20;
  int gagne = 0;
  char buff_in[100];
  while ((taille = recv(sock, buff_in, 100, 0)) > 0) {
    sscanf(buff_in, "%hu", &guess);
    printf("Joueur courant a envoyé : %d\n", guess);
    char reponse[20];
    if (n < guess || n > guess) {
      tentatives--;
    }
    if (tentatives == 0)
      sprintf(reponse, "PERDU\n");
    else if (n < guess)
      sprintf(reponse, "MOINS %d\n", tentatives);
    else if (n > guess)
      sprintf(reponse, "PLUS %d\n", tentatives);
    else {
      sprintf(reponse, "GAGNE\n");
      gagne = 1;
    }
    send(sock, reponse, strlen(reponse), 0);
    if (gagne || !tentatives) break;
  }
  printf("Fin de partie\n");

  close(sock);
}

/**
 * serveur pour jeu 1 player avec 1 connexion à la fois
 */
int server_1p() {
  int serv_sock = socket(PF_INET, SOCK_STREAM, 0);

  struct sockaddr_in serv_addr;
  serv_addr.sin_family = AF_INET;
  serv_addr.sin_port = htons(6632);
  serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);

  int r = bind(serv_sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr));
  if (r != 0) {
    fprintf(stderr, "Échec de bind");
    exit(-1);
  }

  r = listen(serv_sock, 0);
  if (r != 0) {
    fprintf(stderr, "Échec de listen");
    exit(-1);
  }

  int client_sock = 0;
  while (1) {
    client_sock = accept(serv_sock, NULL, NULL);
    if (client_sock < 0) {
      fprintf(stderr, "Échec de accept");
      exit(-1);
    } 
    else {
      printf("Connexion acceptee, nouvelle partie lancée.\n");
      game_1p(client_sock);
      close(client_sock);
    }
  }
  close(serv_sock);
  return 0;
}


/*
int server_1p_fork()
{
  int serv_sock = socket(PF_INET, SOCK_STREAM, 0);

  struct sockaddr_in serv_addr;
  serv_addr.sin_family = AF_INET;
  serv_addr.sin_port = htons(1245);
  serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);

  int r = bind(serv_sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr));
  if (r != 0) {
    fprintf(stderr, "Échec de bind");
    exit(-1);
  }

  r = listen(serv_sock, 0);
  if (r != 0) {
    fprintf(stderr, "Échec de listen");
    exit(-1);
  }
  socklen_t sockLenght = sizeof(serv_addr);

  while(1)
  {
      int r3 = accept(serv_sock, (struct sockaddr *) &serv_addr,
      &sockLenght);
      if(r3 == -1) perror("accept ");
      printf("test \n");

      int retourFork = fork();
      if(retourFork == -1) perror("fork ");
      else if(retourFork == 0)
      {
          close(serv_sock);
          //Protocole de communication

          printf("Connexion acceptee, nouvelle partie lancée.\n");
          game_1p(r3);
          close(r3);
          exit(0);
      }
      else 
      {
        close(r3);
      }
  }
}


int server_1p_threads()
{

}*/