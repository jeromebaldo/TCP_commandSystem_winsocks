#include <winsock2.h> // Pour les sockets
#include <ws2tcpip.h> // Pour inet_pton()
#include <iostream>
#include <string>

#pragma comment(lib, "ws2_32.lib") // Pour utiliser les fonctions de Winsock

using namespace std;

class SocketClient
{
private:
	SOCKET client;
public:

	//constructeur qui initialise la socket client
	SocketClient(SOCKET client)
	{
		this->client = client;
	}

	//destructeur qui ferme la socket client
	~SocketClient()
	{
		closesocket(client);
	}

	string Receive()
	{
		char buffer[1024];
		int bytes_received = recv(client, buffer, 1024, 0);
		if (bytes_received == SOCKET_ERROR)
		{
			cerr << "Echec de la r�ception des donn�es." << endl;
			return "";
		}
		return string(buffer, 0, bytes_received);
	}

	// methode qui envoie un message au client sans double envoi
	bool Send(const string& message)
	{
		// +1 pour le caractere de fin de chaine
		// 0 pour indiquer que le message est envoye en une seule fois
		// c_str() pour convertir la chaine en tableau de char
		int bytes_sent = send(client, message.c_str(), message.size() + 1, 0);
		if (bytes_sent == SOCKET_ERROR)
		{
			cerr << "Echec de l'envoie des donn�es." << endl;
			return false;
		}
		return true;
	}

	// methode qui envoie un message au client pour l'envoi de fichier car ceux-ci sont gros
	int ReceivFichier(char* buffer, int bytesRecus, int tailleFichierRecp)
	{

		int result = recv(client, buffer + bytesRecus, tailleFichierRecp - bytesRecus, 0);
		if (result == SOCKET_ERROR)
		{
			return -1;
		}

		return result;
	}
};


