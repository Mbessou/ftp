// CLIENT

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

typedef struct sockaddr_in SOCKADDR_IN;
typedef struct sockaddr SOCKADDR;

// Fonction qui récupère le nom du fichier à recevoir et l'envoie au serveur.
void get(char *buffer, int sock) {
  char file[100];

  if (buffer[0]== 'g' && buffer[1] == 'e' && buffer[2] == 't') {
    puts("\nNom du fichier à recevoir :\n");
    memset(file, 0, sizeof(file));
    read(STDIN_FILENO, file, sizeof(file));
    if (strlen(file) > 50 || strlen(file) == 1) {
      puts("Erreur: nom du fichier invalide.\n");
      exit(87);
    }
    send(sock, file, 100, 0);
  }
}

// Fonction qui récupère le nom du fichier à envoyer et l'envoie au serveur.
void put(char *buffer, int sock) {
  char file[100];

  if (buffer[0]== 'p' && buffer[1] == 'u' && buffer[2] == 't') {
    puts("\nNom du fichier à envoyer :\n");
    memset(file, 0, sizeof(file));
    read(STDIN_FILENO, file, sizeof(file));
    if (strlen(file) > 50 || strlen(file) == 1) {
      puts("Erreur: nom du fichier invalide.\n");
      exit(87);
    }
    send(sock, file, 100, 0);
  }
}

// Quitte le programme.
void quit(char *buffer) {
  if (buffer[0]== 'q' && buffer[1] == 'u' && buffer[2] == 'i' && buffer[3] == 't') {
    puts("\nFin du programme.\n");
    exit(0);
  }
}

// Fonction qui liste tous les fichiers du répertoire serveur.
void ls(char *buffer) {
  struct dirent *lecture;
  DIR *rep;

  if (buffer[0] == 'l' && buffer[1] == 'i' && buffer[2] == 's' && buffer[3] == 't') {
    rep = opendir("/Users/cesar/Stage/File/"); // Repertoire serveur.
    
    puts("\nListe des fichiers :\n");
    while ((lecture = readdir(rep)))
      if (lecture->d_name[0] != '.')
	printf("|-  %s\n", lecture->d_name);
    closedir(rep);
  putchar('\n');
  }
}

// Fonction qui affiche l'aide du programme.
void need_help(char *buffer) {
  if (buffer[0] == 'h' && buffer[1] == 'e' && buffer[2] == 'l' && buffer[3] == 'p') {
    puts("\nUSAGE :\n\n\tlist : Liste les fichiers du répertoire serveur.\n");
    puts("\tget : Recupère un fichier sur le répertoire serveur.\n");
    puts("\tput : Envoie un fichier sur le répertoire serveur.\n");
    puts("\tquit : Quitte le programme.\n");
  }
}

int main(void) {
  int sock;
  char buffer[100];
  struct sockaddr_in sin;

  sock = socket(AF_INET, SOCK_STREAM, 0);

  sin.sin_addr.s_addr = inet_addr("127.0.0.1");
  sin.sin_family = AF_INET;
  sin.sin_port = htons(1025);

  if (connect(sock, (SOCKADDR*) &sin, sizeof(sin)) != -1) {
    printf("\nConnexion à %s sur le port %d\n\n", inet_ntoa(sin.sin_addr), htons(sin.sin_port));
    while (1) { 
      puts("Entrez une commande :\n");
      memset(buffer, 0, sizeof(buffer));
      read(STDIN_FILENO, buffer, sizeof(buffer));
      need_help(buffer);
      if (strlen(buffer) < 3 || strlen(buffer) > 5)
	puts("\nErreur: cammande invalide.");
      send(sock, buffer, 100, 0);
      ls(buffer);
      quit(buffer);
      get(buffer, sock);
      put(buffer, sock);
      putchar('\n');
    }
  } else {
    printf("Erreur: Connexion impossible.\n");
    exit (87);
  }
  return (0);
}
