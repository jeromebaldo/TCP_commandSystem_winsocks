#include <winsock2.h>
#include <ws2tcpip.h> // Pour inet_pton()
#include <iostream>
#include <string>

//https://stackoverflow.com/questions/5971332/redefinition-errors-in-winsock2-h

#pragma comment(lib, "ws2_32.lib")

using namespace std;

class SocketPirate
{

private:

    string m_ip_address;
    int m_port;
    SOCKET m_socket;
    WSADATA m_wsa_data;

public:
    
    SocketPirate()
    {
        this->m_ip_address = "";
        this->m_port = 0;
        this->m_socket = INVALID_SOCKET;
    }
    //à la destruction de l'objet , on ferme le socket et on libère la mémoire
    ~SocketPirate()
    {
        closesocket(m_socket);
        WSACleanup();
    }

    bool InitialisPirate()
    {
        //pour le côté pratique
        cout << "Initialisation du client avec les elements suivants : 127.0.0.1 | port : 8080" << endl;
        RecupIP();
        RecupPort();

        //creation du socket client et connexion au serveur
        if (!Connect())
        {
            return false;
        }

        return true;
    }

    //methode pour recuperer l'adresse IP du serveur
    void RecupIP()
    {
        string ip_address;
        cout << "Entrez l'adresse IP du serveur: ";
        getline(cin, ip_address); // getline() est utilisé pour lire une ligne complète
        this->m_ip_address = ip_address;
    }
    
    //methode pour recuperer le port du serveur
    void RecupPort()
    {
        int port;
        cout << "Entrez le numero de port du serveur: ";
        cin >> port;
        this->m_port = port;
    }

    bool Connect()
    {
        // Initialize Winsock
        if (WSAStartup(MAKEWORD(2, 2), &m_wsa_data) != 0)
        {
            cerr << "Echec pour initialiser winsock" << endl;
            return false;
        }

        // Create a socket
        m_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
        if (m_socket == INVALID_SOCKET)
        {
            cerr << "Echec pour creer un socket" << endl;
            return false;
        }

        // Resolve the server address and port
        sockaddr_in server_address;
        server_address.sin_family = AF_INET;
        if (inet_pton(AF_INET, m_ip_address.c_str(), &server_address.sin_addr) != 1)
        {
            cerr << "Echec pour convertir une adresse IP" << endl;
            return false;
        }
        server_address.sin_port = htons(m_port);

        // pour se connecter au serveur
        //https://www.geeksforgeeks.org/socket-programming-cc/
        // reinterpret_cast est utilisé pour convertir un pointeur de type void* en un pointeur de type sockaddr*
        
        if (connect(m_socket, reinterpret_cast<sockaddr*>(&server_address), sizeof(server_address)) != 0)
        {
            cerr << "Echec pour connecter au serveur" << endl;
            return false;
        }

        return true;
    }

    // methode pour envoyer des données au serveur de maniere simple. 
    //pas besoin d'envoyer en deux envois
    bool Send(const string& message)
    {
        if (send(m_socket, message.c_str(), message.size(), 0) == SOCKET_ERROR) // Utilise la fonction d'envoi du socket pour envoyer les données du tampon
        {
            cerr << "Echec de l'envoie des donn�es" << endl;
            return false;
        }
        return true;
    }

    //methode pour recevoir des données du serveur de maniere simple. 
    //pas besoin de recevoir en deux envois 
    string Receive()
    {
        char buffer[1024]; // Déclare un tableau de caractères de taille 1024 pour stocker les données reçues
        int bytes_received = recv(m_socket, buffer, sizeof(buffer), 0); // Utilise la fonction de réception du socket pour récupérer les données du tampon et les stocker dans "buffer"
        if (bytes_received == SOCKET_ERROR) // Si la fonction de réception a échoué
        {
            cerr << "Echec dans la reception des donnees" << endl;
            return "";
        }

        string reponse = "";
        for (int i = 0; i < bytes_received; i++) // Ajoute les données reçues dans la chaîne de caractères "reponse"
        {
            reponse += buffer[i];
        }
        return reponse;
    }
    
    //accesseur permettant d'obtenir l'adresse IP
    string GetIp()
    {
        return this->m_ip_address;
    }

    //accesseur permettant d'obtenir le numéro de port
    int GetPort()
    {
        return this->m_port;
    }

    // Fonction pour recevoir un fichier en utilisant un tampon de réception, 
    //le nombre de bytes reçus et la taille du fichier à recevoir
    int ReceivFichier(char* buffer, int bytesRecus, int tailleFichierRecp)
    {
        // Utiliser la fonction de réception du socket pour récupérer les données du tampon et les stocker dans "buffer"
        // "bytesRecus" est utilisé pour indiquer à la fonction où les nouvelles données doivent être stockées dans "buffer"
        // "tailleFichierRecp-bytesRecus" est utilisé pour indiquer combien d'octets sont encore nécessaires pour recevoir le fichier complet
        int result = recv(m_socket, buffer + bytesRecus, tailleFichierRecp-bytesRecus, 0);
        if (result == SOCKET_ERROR)
        {
            return -1;
        }

        // Retourner le nombre d'octets qui ont été reçus avec succès
        return result;
    }
};

