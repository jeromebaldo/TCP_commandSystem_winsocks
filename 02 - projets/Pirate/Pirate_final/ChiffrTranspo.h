#include <iostream>

using namespace std;

class ChiffrTranspo
{

private:

	string cle;
	int nbrcolonnes;

public:

	ChiffrTranspo(string cle)
	{
		this->cle = cle;
		this->nbrcolonnes = cle.length();
	}

	char** CreerMatrice(int nbrlignes)
	{
		char** matrice = new char* [nbrlignes];
		for (int i = 0; i < nbrlignes; i++)
		{
			matrice[i] = new char[this->nbrcolonnes];
		}

		for (int i = 0; i < nbrlignes; i++)
		{
			matrice[i] = new char[this->nbrcolonnes];
			for (int j = 0; j < this->nbrcolonnes; j++)
			{
				matrice[i][j] = '¬'; 
			}
		}
		return matrice;
	}

	//////////////////////////////////////////////////
	// phase de chiffrement avec les fonctions suivantes 
	// RemplirMatriceChiff + Chiffrer + TexteChiffre

	char** RemplirMatriceChiff(char** matrice, string texte)
	{
		int colonne = 0;
		int ligne = 0;
		for (int i = 0; i < texte.length(); i++)
		{

			matrice[ligne][colonne] = texte[i];

			colonne++;
			if (colonne == this->nbrcolonnes)
			{
				colonne = 0;
				ligne++;
			}
		}
		return matrice;
	}

	string Chiffrer(string texte)
	{
		int nbrlignes = ceil(texte.length() / static_cast<double>(cle.length()));
		char** matriceClair = CreerMatrice(nbrlignes);
		matriceClair = RemplirMatriceChiff(matriceClair, texte);

		char** matriceChiffre = CreerMatrice(nbrlignes);

		int nbrCase = this->nbrcolonnes * nbrlignes;
		int i = 0;
		int w = 0;

		for (int colonne = 0; colonne < this->nbrcolonnes; colonne++)
		{
			int cle = this->cle[colonne] - '0';
			for (int ligne = 0; ligne < nbrlignes; ligne++)
			{
				matriceChiffre[ligne][cle - 1] = matriceClair[ligne][colonne];
			}
		}

		string texteChiffre = "";
		texteChiffre = TexteChiffre(matriceChiffre, nbrlignes);

		delete matriceClair;
		delete matriceChiffre;

		return texteChiffre;
	}

	string TexteChiffre(char** matriceChiffre, int nbrlignes)
	{
		string texteChiffre = "";

		for (int colonne = 0; colonne < this->nbrcolonnes; colonne++)
		{
			for (int ligne = 0; ligne < nbrlignes; ligne++)
			{
				texteChiffre += matriceChiffre[ligne][colonne];
			}
		}

		return texteChiffre;
	}

	//////////////////////////////////////////////////
	// phase de dechiffrement avec les fonctions suivantes
	// RemplirMatriceDechiff + Dechiffrer + TexteDechiffre

	char** RemplirMatriceDechiff(char** matrice, string texte, int nbrlignes)
	{
		int ligne = 0;
		int colonne = 0;
		for (int i = 0; i < texte.length(); i++)
		{
			matrice[ligne][colonne] = texte[i];
			ligne++;
			if (ligne == nbrlignes)
			{
				ligne = 0;
				colonne++;
			}
		}

		return matrice;
	}

	string Dechiffrer(string texte)
	{
		int nbrlignes = ceil(texte.length() / static_cast<double>(cle.length()));
		char** matriceChiffre = CreerMatrice(nbrlignes);
		matriceChiffre = RemplirMatriceDechiff(matriceChiffre, texte, nbrlignes);

		char** matriceDechiffre = CreerMatrice(nbrlignes);

		int colonne = 0;

		for (int i = 0; i < this->cle.length(); i++)
		{
			int cle = this->cle[i] - '0';
			for (int ligne = 0; ligne < nbrlignes; ligne++)
			{
				matriceDechiffre[ligne][colonne] = matriceChiffre[ligne][cle - 1];
			}
			colonne++;
		}

		string texteDechiffre = "";
		texteDechiffre = TexteDechiffre(matriceDechiffre, texte);
		//texteDechiffre = TexteDechiffre(matriceDechiffre, nbrlignes);

		return texteDechiffre;
	}

	string TexteDechiffre(char** matriceDechiffre, string texte)
	{
		int ligne = 0;
		int colonne = 0;
		string texteDechiffre = "";
		texte.erase(remove(texte.begin(), texte.end(), '¬'), texte.end());
		for (int i = 0; i < texte.length(); i++)
		{
			//l'objectif est de ne pas afficher les caract�res null
			if (matriceDechiffre[ligne][colonne] != '\0')
			{
				texteDechiffre += matriceDechiffre[ligne][colonne];
				colonne++;
				if (colonne == this->nbrcolonnes)
				{
					colonne = 0;
					ligne++;
				}
			}
		}

		

		return texteDechiffre;
	}
};
