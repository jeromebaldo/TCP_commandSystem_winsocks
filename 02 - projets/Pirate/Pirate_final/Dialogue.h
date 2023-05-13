#include <iostream>
#include <filesystem>


using namespace std;


class Dialogue
{

private: 
    string repertoireChargement;
    string repertoireTelechargement;
public :
	Dialogue() 
    {
        this->repertoireChargement = "";
        this->repertoireTelechargement = "";
    }
	~Dialogue() {}
	
	int ChoixOption()
	{
		cout << "CHOIX du menu " << endl;
		cout << "1. Realiser une commande systeme" << endl;
		cout << "2. Telecharger un fichier" << endl;
		cout << "3. Charger un fichier sur le serveur" << endl;
		cout << "4. Se deconnecter du serveur" << endl;
		cout << "(tapez entre 1 et 4 ) Choix: ";
		int choix;
		cin >> choix;

		return choix;
	}

    string OptionEnvoi(int choix)
    {
        string texte = "";
        string enteteInfo = "";

        switch (choix)
        {
        case(1):
            
            cout << "Commande a executer: ";
            //https://en.cppreference.com/w/cpp/io/basic_istream/ignore
            cin.ignore(); // Pour supprimer le caractère de retour à la ligne dans le buffer d'entrée
            getline(cin, texte);
            enteteInfo = "COMMANDE\\" + texte + " \\" ;

            break;

        case(2):

            cout << "exemple de fichier a recuperer : adressage_ip.docx" << endl;
            cout << "Nom du fichier a telecharger: ";
            cin.ignore();
            getline(cin, texte);
            //texte doit comprendre le nom du fichier et son extension
            enteteInfo = "TELECHARGER\\" + texte + "\\";

            break;

        case(3):

            cout << "exemple de fichier a charger : GRASP_1.pdf" << endl;
            cout << "Nom du fichier a charger: ";
            cin.ignore();
            getline(cin, texte);
            enteteInfo = texte;

            break;

        case(4):

            enteteInfo = "DECONNECTION\\";
            break;

        default:
            cout << "Erreur de saisie" << endl;
            break;
        }

        return enteteInfo;
    }

    void InitRepertoires()
    {
        // Obtenir le chemin absolu du répertoire courant en appelant la fonction current_path()
        // et en stockant le résultat dans un objet de type path
        // https://en.cppreference.com/w/cpp/filesystem/current_path
        filesystem::path chemin_absolu = filesystem::current_path();

        // Convertir le chemin absolu en une chaîne de caractères
        string repertoireAbsolu = chemin_absolu.string();

        // Définir le répertoire de chargement en concaténant le chemin absolu avec le nom du répertoire de chargement
        this->repertoireChargement = repertoireAbsolu + "\\ChargementPirate";

        // Définir le répertoire de chargement en concaténant le chemin absolu avec le nom du répertoire de telechargement
        this->repertoireTelechargement = repertoireAbsolu + "\\TelechargementPirate";
    }

    //methode pour retourner le repertoire de chargement
    string GetRepertoireChargement()
    {
		return this->repertoireChargement;
	}

    //methode pour retourner le repertoire de telechargement
    string GetRepertoireTelechargement()
    {
        return this->repertoireTelechargement;
    }
};

