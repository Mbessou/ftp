// SERVEUR

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <dirent.h>
#include <fcntl.h>

#define MAX_CLIENTS 15 // Limite à 10 clients.

typedef struct sockaddr_in SOCKADDR_IN;
typedef struct sockaddr SOCKADDR;

void *client_handler(void *);
void *get(int);
void *put(int);
void quit(void);

// Fonction d'envoie de fichier du client au serveur.
void *put(int sock) {
  char buffer[100], str[128];
  int fdin, fdout, nread;

  recv(sock, buffer, 100, 0);
  if (strlen(buffer) > 80 || strlen(buffer) == 1) {
    puts("Erreur: nom du fichier invalide.\n");
    exit(87);
  }
  printf("Recherche de : %s\n", buffer);
  buffer[strlen(buffer) - 1] = '\0';

  char pathf[100] = "/Users/cesar/Dest/"; // Répertoire client.
  char pathd[100] = "/Users/cesar/File/"; // Répertoire serveur.

  strcat(pathf, buffer);
  strcat(pathd, buffer);

  fdin = open(pathf, O_RDONLY);

  if (fdin == -1) {
    perror("open");
    exit(87);
  }

  fdout = open(pathd, O_WRONLY | O_CREAT | O_TRUNC, S_IRWXU);

  if (fdout == -1) {
    perror("open");
    exit(87);
  }

  do {
    nread = read(fdin, str, 128);
    write(fdout, str, nread);
  } while (nread == 128);

  close(fdin);
  close(fdout);

  puts("Fichier envoyé.\n");

  return (NULL);
}

// Fonction de réception de fichier du serveur au client.
void *get(int sock) {
  char buffer[100], str[128];
  int fdin, fdout, nread;

  recv(sock, buffer, 100, 0);
  if (strlen(buffer) > 80 || strlen(buffer) == 0) {
    puts("Erreur: nom du fichier invalide.\n");
    exit(87);
  }
  printf("Recherche de : %s\n", buffer);
  buffer[strlen(buffer) - 1] = '\0';

  char pathf[100] = "/Users/cesar/File/"; // Répertoire serveur.
  char pathd[100] = "/Users/cesar/Dest/"; // Répertoire client.

  strcat(pathf, buffer);
  strcat(pathd, buffer);

  fdin = open(pathf, O_RDONLY);

  if (fdin == -1) {
    perror("open");
    exit(87);
  }

  fdout = open(pathd, O_WRONLY | O_CREAT | O_TRUNC, S_IRWXU);

  if (fdout == -1) {
    perror("open");
    exit(87);
  }

  do {
    nread = read(fdin, str, 128);
    write(fdout, str, nread);
  } while (nread == 128);

  close(fdin);
  close(fdout);

  puts("Fichier reçu.\n");

  return (NULL);
}

// Quitte le programme.
void quit(void) {
  puts("Fin du programme.\n");
  exit(0);
}

void ls(int sock) {
  puts("Le client a fait un list.\n");
}

void need_help(int sock) {
  puts("Le client a fait un help.\n");
}

// Controleur des commandes client.
void *client_handler(void *sock) {
  int rsock = *(int*) sock;
  char buffer[100];

  while (1) {
    recv(rsock, buffer, 100, 0);

    if (buffer[0] == 'l' && buffer[1] == 'i' && buffer[2] == 's' && buffer[3] == 't') // Le client fait un list.
      ls(rsock);
    else if (buffer[0] == 'p' && buffer[1] == 'u' && buffer[2] == 't') // Le client fait un put.
      put(rsock);
    else if (buffer[0] == 'g' && buffer[1] == 'e' && buffer[2] == 't') // Le client fait un get.
      get(rsock);
    else if (buffer[0] == 'q' && buffer[1] == 'u' && buffer[2] == 'i' && buffer[3] == 't') // Le client fait un quit.
      quit();
    else if (buffer[0] == 'h' && buffer[1] == 'e' && buffer[2] == 'l' && buffer[3] == 'p') // Le client fait un help.
      need_help(rsock);
    else {
      puts("Erreur: commande invalide.\n");
      client_handler(sock);
    }
  }
  return (NULL);
}

int main(void) {
  int sock = 0;
  struct sockaddr_in sin;
  socklen_t recsize = sizeof(sin);

  int csock = 0;
  struct sockaddr_in csin;
  socklen_t crecsize = sizeof(csin);

  int sock_err;

  sock = socket(AF_INET, SOCK_STREAM, 0);

  int buf = 1;

  setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &buf, sizeof (int));
  // Permet de relancer le serveur directement.

  sin.sin_addr.s_addr = htonl(INADDR_ANY); // Adresse IP automatique.
  sin.sin_family = AF_INET;
  sin.sin_port = htons(1025);

  sock_err = bind(sock, (SOCKADDR*) &sin, recsize);

  pthread_t thread;
  pthread_detach(thread);

  if (sock_err != -1) {
    sock_err = listen(sock, 10);
    if (sock_err != -1) {
      while ( (csock = accept(sock, (SOCKADDR*) &csin, &crecsize)) < MAX_CLIENTS) {
        printf("\nClient %d connecté avec la socket %s:%d.\n\n", csock - 3, inet_ntoa(csin.sin_addr), htons(csin.sin_port));
        pthread_create(&thread, NULL, client_handler, (void*) &csock);
      }
    }
  }
  close(csock);
  close(sock);
  return (0);
}
