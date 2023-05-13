#include <winsock2.h> // Pour SOCKET, SOCKADDR_IN, ...
#include <ws2tcpip.h> // Pour inet_pton()
#include <iostream>
#include <string>

#pragma comment(lib, "ws2_32.lib") // Pour utiliser la librairie ws2_32.lib

using namespace std;

class SocketServeur
{
private:
    string m_ip_address;
    int m_port;
    SOCKET m_socket;
    WSADATA m_wsa_data;

public:

    // Constructeur qui initialise l'adresse IP et le port et cree le socket
    SocketServeur(const string& ip_address, int port)
    {
        this->m_ip_address = ip_address;
        this->m_port = port;
        this->m_socket = INVALID_SOCKET;
    }

    // Destructeur qui ferme le socket
    ~SocketServeur()
    {
        closesocket(m_socket);
        WSACleanup();
    }

    bool Listen()
    {
        // initialise winsock
        if (WSAStartup(MAKEWORD(2, 2), &m_wsa_data) != 0)
        {
            cerr << "Echec de l'initialisation winsock" << endl;
            return false;
        }

        // cree le socket
        m_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP); // AF_INET = IPv4, SOCK_STREAM = TCP, IPPROTO_TCP = TCP
        if (m_socket == INVALID_SOCKET) // INVALID_SOCKET = -1
        {
            cerr << "Echec de la creation du socket" << endl;
            return false;
        }

        // Bind le socket avec l'adresse IP et le port 
        sockaddr_in server_address; // SOCKADDR_IN = sockaddr_in
        server_address.sin_family = AF_INET; // AF_INET = IPv4
        server_address.sin_addr.s_addr = INADDR_ANY; // INADDR_ANY signifie que le socket va ecouter sur toutes les interfaces reseau disponibles sur la machine
        server_address.sin_port = htons(m_port); // htons() est une fonction qui convertit l'ordre des octets des entiers (pour s'assurer que la representation binaire de l'entier est compatible avec les sockets)

        /*
            sockaddr_in est une structure specifique aux sockets Internet (IP), et AF_INET est la famille d'adresses utilis�e pour les adresses IP.
            server_address.sin_family = AF_INET;
            initialise la famille d'adresses de server_address AF_INET.
            server_address.sin_addr.s_addr = INADDR_ANY;
            initialise l'adresse IP de server_address INADDR_ANY.
            Cette adresse signifie que le socket va �couter sur toutes les interfaces r�seau disponibles sur la machine
            (c'est-�-dire qu'il peut accepter des connexions entrantes sur n'importe quelle adresse IP associ�e � la machine).
            server_address.sin_port = htons(m_port);
            initialise le port de server_address � m_port, qui est le num�ro de port pass� en param�tre au constructeur de la classe SocketServeur.
            htons() est une fonction qui convertit l'ordre des octets des entiers
            (pour s'assurer que la repr�sentation binaire de l'entier est compatible avec les sockets).
        */
        //reinterpret_cast est utile car on ne peut pas convertir un pointeur de type sockaddr_in* en pointeur de type sockaddr* directement
        //https://stackoverflow.com/questions/573294/when-to-use-reinterpret-cast
        if (bind(m_socket, reinterpret_cast<sockaddr*>(&server_address), sizeof(server_address)) != 0)
        {
            cerr << "Echec pour binder le socket sur l'adresse IP et le port" << endl;
            return false;
        }

        // ecoute les connections entrantes
        if (listen(m_socket, SOMAXCONN) != 0) // SOMAXCONN spécifie la taille de la file d'attente des connexions en attente pour une socket.
        {
            cerr << "Echec pour l'�coute des connections entrantes" << endl;
            return false;
        }

        return true;
    }


    //methode pour accepter les connections entrantes
    SOCKET Accept()
    {
        sockaddr_in client_address; //structure de données avec TCP/IP
        int client_address_size = sizeof(client_address); //taille de la structure de données client
        // castage de la structure de données client en sockaddr* pour la fonction accept
        SOCKET client_socket = accept(m_socket, reinterpret_cast<sockaddr*>(&client_address), &client_address_size);
        if (client_socket == INVALID_SOCKET)
        {
            cerr << "Echec pour l'acceptation de la connection du client" << endl;
            return INVALID_SOCKET;
        }

        return client_socket;
    }
};



