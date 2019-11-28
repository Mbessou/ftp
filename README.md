# File Transfer Protocol

## Description

*Création d'un ftp (client(s)/serveur) afin de s'échanger toutes sortes de fichiers grâce à l'utilisation de socket qui utilise le protocol TCP et de thread.*

*Le nombre de client pouvant se connecter est limité à 10.*

### Compilation

Serveur : `gcc -o server.c server -O0`

Client : `gcc -o client.c client -O0`

Client 1 : `gcc -o client.c client1 -O0`

[...]

### Utilisation Serveur

	./server

`Port : 1025`

### Utilisation Clients

	./client

	./client1

	[...]

`Port : 1025`

### Commandes

Voici la listes des commandes :

	help : Affiche l'aide.

	list : Liste tous les fichiers du répertoire serveur.

	get : Récupère un fichier sur le répertoire serveur.

	put : Envoie un fichier sur le répertoire serveur.

	quit : Quitte le programme.

**Remarques**

Le contenu des fichiers est envoyé 128 par 128 octets.


Par défaut,

- le répertoire serveur à pour chemin `./File/`

- le répertoire client à pour chemin `./Dest/`

### Conclusion

Personnellement, j'ai trouvé ce projet intéressant car j'ai appris de nouvelles notions tels que les sockets et les threads. De plus, je trouve le résultat final du projet assez aboutit malgré un commencement difficile avec peu de connaissances en réseau. J'ai déjà l'impression de m'être bien améliorer après une semaine sur ce projet.


Auteur : *César VENZAC*

Tuteurs : *Nicolas VILMAIN et Vincent KHERBACHE*