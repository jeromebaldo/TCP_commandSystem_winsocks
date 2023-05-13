#include <iostream>    // Inclure la biblioth�que standard d'entr�es/sorties en C++
#include <string>      // Inclure la biblioth�que pour manipuler les cha�nes de caract�res en C++
#include <filesystem> // Inclure la biblioth�que pour manipuler les fichiers et les r�pertoires en C++17
#include <vector>      // Inclure la biblioth�que pour manipuler les tableaux dynamiques en C++
#include <fstream>     // Inclure la biblioth�que pour lire et �crire des fichiers en C++
#include <iomanip>     // Inclure la biblioth�que pour manipuler le formatage des donn�es en C++

#include "SocketPirate.h"
#include "ChiffrTranspo.h"
#include "Dialogue.h"

using namespace std;
namespace fs = std::filesystem; //pour la gestion des systemes de fichiers


int main()
{
    //////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////
    //INITIALISATION DES VARIABLES + CONNEXION AU SERVEUR 

    //creation du module de dialogue
    Dialogue dialogue;

    //initialisation des variables pour la connexion au serveur
    SocketPirate client;
    if (client.InitialisPirate() == false) // V�rifier si la tentative de connexion au serveur a �chou�
    {
        cerr << "Echec pour connecter au serveur" << endl; // Afficher un message d'erreur dans la sortie d'erreur standard
        throw runtime_error("Impossible de se connecter au serveur"); // Lancer une exception indiquant qu'il est impossible de se connecter au serveur

        return 1; // Retourner un code d'erreur pour indiquer que la connexion a �chou�
    }


    //initialisation des r�pertoires servant pour le t�l�chargement et le chargement 
    //pour les telechargements du serveur, ceux-ci seront mis dans le dossier telechargement
    //pour les chargements sur le serveur, ceux-ci devront �tre mis dans le dossier de chargement 
    dialogue.InitRepertoires();
    
    //creation des dossiers si ils n'existent pas gr�ce � la fonction exists()
    if (!fs::exists(dialogue.GetRepertoireChargement()))
    {
        //prend le chemin d'acces pour cr�er le dossier 
        fs::create_directory(dialogue.GetRepertoireChargement());
    }

    //creation des dossiers si ils n'existent pas gr�ce � la fonction exists()
    if (!fs::exists(dialogue.GetRepertoireTelechargement()))
    {
        //prend le chemin d'acces pour cr�er le dossier
        fs::create_directory(dialogue.GetRepertoireTelechargement());
    }

    //////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////

    cout << "---------------------------------------------------" << endl;
    cout << "Connexion OK serveur " << client.GetIp() << " | " << client.GetPort() << endl;
    cout << "Chemin du dossier de chargement  : " << dialogue.GetRepertoireChargement() << endl;
    cout << "Chemin du dossier de telechargement  : " << dialogue.GetRepertoireTelechargement() << endl;
    cout << "---------------------------------------------------" << endl;
    
    bool deconnexion = false;
    while (!deconnexion)
    {
        cout << "---------------------------------------------------" << endl;
        
        string response = "";
        string enteteInfo = "";
        
        
        //////////////////////////////////////////////////
        // PARTIE ENVOI DE L'ENTETE AU SERVEUR
        
        //selection de l'option et preparation de l'entete
        int choix = dialogue.ChoixOption();
        enteteInfo = dialogue.OptionEnvoi(choix);
        
        //si chargement sur le serveur alors cela necessite deux envois 
        //pour le reste un seul envoi suffit
        if (choix == 1 || choix == 2 || choix == 4)
        {         
            if (!client.Send(enteteInfo))
            {
                cerr << "Echec pour envoyer le message" << endl;
                return 1;
            }
        }
        else if (choix == 3)
        {
            //envoi pour le chargement d'un fichier sur le serveur
            string cheminFichier = dialogue.GetRepertoireChargement() + '\\' + enteteInfo;
            
            //ouverture en mode binaire 
            ifstream fileIn(cheminFichier, ios::binary);

            if (!fileIn.is_open())
            {
                cerr << "Erreur: impossible d'ouvrir le fichier " << enteteInfo << endl;
            }

            // Creer un vecteur contenant le contenu du fichier � partir des it�rateurs
            vector<unsigned char>fichierBytes((istreambuf_iterator<char>(fileIn)), istreambuf_iterator<char>());
            
            fileIn.close();

            //conversion du vecteur en string
            string stringifBinaire(fichierBytes.begin(), fichierBytes.end());
            //recuperation de la taille du fichier
            string tailleFichier = to_string(stringifBinaire.size());
            //creation de l'entete pour le serveur 
            string reponseServeur = "CHARGEMENT\\" + enteteInfo + "\\" + tailleFichier + "\\";

            cout << "reponseServeur" << reponseServeur << endl;

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
        
        //////////////////////////////////////////////////
        //REPONSE DU SERVEUR

        //determination de la reponse du serveur
        //si telechargement, il y aura deux envois a receptionner  
        //pour deconnection , le serveur envoie une reponse notifiant qu'il l'a pris en compte 
        //pour les commandes, il revoit le detail de reponse de commandes 
        //pour le chargement il renvoit simplement une notification qu'il a bien re�u le fichier 
        cout << "choix : " << choix << endl;
        if (choix == 1 || choix == 3 || choix == 4)
        {
            response = client.Receive();
            if (response.empty())
            {
                cerr << "Echec pour recevoir la reponse" << endl;
                return 1;
            }

            cout << response << endl;
            if (choix == 4)
            {
                deconnexion = true;
                
            }
            response = "";
        }
        else
        {
                     
            //reception de la taille du fichier ainsi que de son nom. 
            response = client.Receive();
            if (response.empty())
            {
                cerr << "Echec pour recevoir la reponse" << endl;
                return 1;
            }

            //extraction du nom ainsi que de la taille du fichier 
            string membre = "";
            vector <string> detailFichier;
            for (auto it : response)
            {
                if (it == '\\')
                {
                    detailFichier.push_back(membre);
                    membre = "";
                }
                else
                {
                    membre += it;
                }
            }
            //ajout du fichier pour creer le chemin d'acces 
            string cheminFichier = dialogue.GetRepertoireTelechargement() + "\\" + detailFichier[0];
            cout << "Chemin du fichier : " << cheminFichier << endl;
            
            //utilisation de stoi pour convertir la taille du fichier en int et recup la taille du fichier
            int tailleFichierRecp = stoi(detailFichier[1]);
            
            //configuration d'un buffer de la taille du fichier qui sera receptionne
            char* buffer = new char[tailleFichierRecp];
            int bytesRecus = 0;
            while (bytesRecus < tailleFichierRecp)//incremente des bytes jusqua la taille total du fichier
            {
                //decrementation de la taille totale restant a receptionner 
                //incrementation de la position sur le buffer. cela permet de le remplir  
                int result = client.ReceivFichier(buffer, bytesRecus, tailleFichierRecp);
                if (result == -1)
                {
                    cerr << "Erreur lors de la reception du fichier : " << WSAGetLastError() << endl;
                    client.~SocketPirate();
                    
                    return 1;
                }
                bytesRecus += result;//incrementation
                
            }
            cout << "Bytes recus : " << bytesRecus << endl;
            
            //ouverture du fichier en mode binaire 
            // ecriture du buffer dans le fichier
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
				client.~SocketPirate();
				delete[] buffer;
				return 1;
			}
            delete[] buffer;
			
            cout << "Reussite du telechargement du fichier " << detailFichier[0] << endl;
            
        }

        cout << "---------------------------------------------------" << endl;
        
        
		
    }

    return 0;
}
