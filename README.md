# TCP_commandSystem_winsocks

## I - Contexte et objectifs de l'application 

Laboratoire réalisé dans le cadre de mon cours "Réseaux d'ordinateurs" délivré par l'Université du Québec à Chicoutimi.
Il consiste pour un client à réaliser des commandes systèmes sur un serveur ainsi qu'à déclencher le chargement et le téléchargement de fichiers.
L'ensemble des opérations se basent sur le protocole TCP et  les sockets. 

## II - Point de situation

Actuellement le système permet les fonctionnalités suivantes :
- téléchargement de fichiers sur le serveur 
- chargement de fichiers depuis le serveur sur le client 
- déclenchement des commandes systèmes sur l'ordinateur. 

L'ensemble fonctionne sous windows avec les winsocks pour  gèrer les sockets. Le laboratoire se réalise en localhost sur un système d'exploitation windows. 

Le téléchargement et le chargement possèdent un fonctionnement particulier. Pour télécharger, il faut vous situer dans le répertoire courant souhaité du serveur. En tapant 2, vous donnerez le nom du fichier avec son extension (ex : test.txt). Celui-ci sera automatiquement télécharger dans le dossier de téléchargement pirate situé du côté client.

Pour charger un fichier spécifique sur le serveur, vous devez vous positionner sur le répertoire courant souhaité. Par la suite vous devez réaliser un copier-coller du fichier souhaité dans le dossier chargement pirate côté client. Enfin vous devez 3 en option pour charger le fichier en indiquant le nom et son extension (ex : test.txt). 
Le client et le serveur feront le reste pour charger le fichier. Pensez bien à donner votre option dans le menu contextuel avant de réaliser votre action (commande, fichier). 

## III - Installation et exécution de l'application

### Composition du repository
Le repository est composé de la façon suivante : 
- dossier des exécutables avec des fichiers à charger ou télécharger pour le client et le serveur
- dossier des projets complets tournant sous Visual studio (Windows) pour le client et le serveur
- dossier du rapport d'analyse du projet 
- énoncé du projet 

### Lancement du projet
Vous pouvez directement lancer le client/pirate et le serveur sur votre ordinateur situés dans le dossiers "01_executables". Veuillez double-cliquer d'abord sur le serveur puis sur le client. Sans l'ouverture du serveur, le client s'arrêtera automatiquement.

Pour les projets, il suffit de naviguer dans le dossier "02 - projets" et de cliquer sur le fichier de type .sln.

## IV - Sources
### Cacher la console 
- https://stackoverflow.com/questions/18260508/c-how-do-i-hide-a-console-window-on-startup
### Exécution et récupération de la commande système
- https://stackoverflow.com/questions/34074112/when-to-call-pclose
- https://stackoverflow.com/questions/38876218/execute-a-command-and-get-output-popen-and-pclose-undefined
- https://stackoverflow.com/questions/34074112/when-to-call-pclose
### Liens pour les sockets
- https://stackoverflow.com/questions/5971332/redefinition-errors-in-winsock2-h
- https://learn.microsoft.com/en-us/windows/win32/api/ws2tcpip/
- https://learn.microsoft.com/en-us/windows/win32/api/winsock2/
- https://learn.microsoft.com/en-us/windows/win32/api/ws2tcpip/nf-ws2tcpip-inet_pton
- https://learn.microsoft.com/en-us/windows/win32/api/ws2def/ns-ws2def-sockaddr_in
### Liens sur les sockets
- https://learn.microsoft.com/en-us/windows/win32/winsock/getting-started-with-winsock
- https://stackoverflow.com/questions/5971332/redefinition-errors-in-winsock2-h
- https://www.geeksforgeeks.org/socket-programming-cc/
- https://stackoverflow.com/questions/8480640/how-to-throw-a-c-exception
### Liens sur l’ouverture, l’écriture de fichiers
- https://en.cppreference.com/w/cpp/filesystem/create_directory
- https://en.cppreference.com/w/cpp/iterator/istreambuf_iterator
- https://cplusplus.com/doc/tutorial/files/
## V - Changelog

### V1 => version initiale
