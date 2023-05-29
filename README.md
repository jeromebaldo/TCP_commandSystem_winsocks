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
## V - Changelog

### V1 => version initiale
