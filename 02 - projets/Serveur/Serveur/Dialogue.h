#include <iostream>
#include <vector>

using namespace std;

class Dialogue
{
public:
	Dialogue() {}
	~Dialogue() {}

	//methode pour traiter les entetes
	vector<string> DecodageEntete(string reponse)
	{
		vector<string> entete;
		string membre = "";
		int iteration = 0;
		for (auto it : reponse)
		{

			if (it == '\\' || reponse.size() == 0) // si on a un \ alors on a fini de lire un membre
			{
				entete.push_back(membre); // on ajoute le membre a la liste
				membre = ""; // on vide le membre
			}
			else
			{
				membre += it; // on ajoute le caractere au membre
			}
		}

		return entete; // on retourne la liste des membres
	}
};

