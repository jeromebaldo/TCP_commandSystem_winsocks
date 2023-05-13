#include <iostream>
#include <string>
#include <cstdio>
#include <vector>
#include <sstream>

using namespace std;

class Commande
{
public:

    Commande() {};

    ~Commande() {};

    //methode qui permet d'executer une commande et de retourner le resultat
    string ExecCommand(string command, string adresse)
    {
        vector<string> resultatCommande;
        string commandeFinale = "";

        // execution d'une navigation dans les r�pertoires
        // puis execution de la commande 
        // enfin execution d'un cd pour obtenir la derniere localisation du serveur
        commandeFinale += "cd " + adresse + " && " + command + " && cd";

        // execution de la commande en utilisant le pipe
        // et recuperation du resultat
        // _popen et _pclose sont des fonctions de la librairie cstdio
        // ils permettent d'executer une commande et de recuperer le resultat

        FILE* stream = _popen(commandeFinale.c_str(), "r"); // r est le mode de lecture et _popen permet d'executer une commande et de recuperer le resultat
        if (!stream)// si le stream est vide
        {
            return "";
        }

        char buffer[2048];
        string result;
        while (!feof(stream)) // tant que le stream n'est pas vide feof est une fonction de la librairie cstdio qui permet de savoir si on est a la fin du fichier
        {
            if (fgets(buffer, 2048, stream) != NULL) // fgets est une fonction de la librairie cstdio qui permet de lire une ligne dans un fichier
            {
                result += buffer; // on ajoute le buffer a la chaine de caractere result
            }
        }
        _pclose(stream); // on ferme le stream

        return result; // on retourne le resultat
    }
};


