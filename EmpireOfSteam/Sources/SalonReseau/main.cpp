/*#include <iostream>

#include <SFML/Network.hpp>


using namespace std;

void ThreadConnectFunction(void* UserData)
{
    std::vector <sf::IPAddress>* ClientAddress = static_cast<std::vector <sf::IPAddress>*>(UserData);

    sf::SocketTCP Client;

    sf::SocketTCP Listener;
    if (!Listener.Listen(4567))
    {
        // Erreur...
    }

    while(1)
    {
        sf::IPAddress CurClientIP;

        if (Listener.Accept(Client, &CurClientIP) != sf::Socket::Done)
            cout << "Error" << endl;
        else
            ClientAddress->push_back(CurClientIP);
    }

    Client.Close();
}



int main()
{
    bool            host    = false;
    bool            lan     = false;

    sf::IPAddress   IPHost;
    sf::IPAddress   YourIP;

    cout << "Internet(0) or lan(1) ?" << endl;
    cin>>lan;

    cout << "Client(0) or host(1) ?" << endl;
    cin>>host;

    if(!host)
    {
        cout << "Host's IP :" << endl;
        cin  >> IPHost;

        if (!IPHost.IsValid())
        {
            cout << "Invalid IP" << endl;
            exit(0);
        }

    }

    if(lan)
        YourIP = sf::IPAddress::GetLocalAddress();
    else
        YourIP = sf::IPAddress::GetPublicAddress();

    cout << "Your IP : " << YourIP <<endl;


    if(host)
    {
        std::vector <sf::IPAddress> ClientAddress;

        // Créons un thread avec notre fonction
        sf::Thread ThreadConnect(&ThreadConnectFunction, &ClientAddress);


        // Démarrons le !
        ThreadConnect.Launch();

        int action = -1;

        do
        {
            cout << "Your IP : " << YourIP <<endl;

            unsigned i = 0;
            while(i < ClientAddress.size())
            {
                sf::SocketTCP Client;

                char Buffer[] = "Ok";
                if (Client.Send(Buffer, sizeof(Buffer)) == sf::Socket::Disconnected)
                    ClientAddress.erase(ClientAddress.begin() + i), i = -1;
                ++i;

                Client.Close();
            }

            i = 0;
            while(i < ClientAddress.size())
                cout << "Client : " << ClientAddress[i++] << endl;

            cout << "Exit(0) ; Refresh(1) ; Chat(2) ; Kick(3)" << endl;
            cin >> action;
        }
        while(action != 0);

        // Pour une raison X, nous voulons le stopper immédiatement
        ThreadConnect.Terminate();
    }
    else
    {
        sf::SocketTCP Client;
        if (Client.Connect(4567, IPHost) != sf::Socket::Done)
            cout << "Cannot connecting" << endl;
        else
            cout << "Connection estalished" << endl;

        int action = -1;
        do
        {
            cout << "Your IP : " << YourIP <<endl;

            cout << "Exit(0) ; Refresh(1) ; Chat(2)" << endl;
            cin >> action;
        }
        while(action != 0);

        Client.Close();
    }

    system("PAUSE");

    return 0;
}*/


#include <iostream>

#include <SFML/Network.hpp>


using namespace std;

void ThreadConnectFunction(void* UserData)
{
    std::vector <sf::IPAddress>* ClientAddress = static_cast<std::vector <sf::IPAddress>*>(UserData);

    sf::SocketTCP Client;

    sf::SocketTCP Listener;
    if (!Listener.Listen(4567))
    {
        // Erreur...
    }

    while(1)
    {
        sf::IPAddress CurClientIP;

        if (Listener.Accept(Client, &CurClientIP) != sf::Socket::Done)
            cout << "Error" << endl;
        else
            ClientAddress->push_back(CurClientIP);
    }

    Client.Close();
}



int main()
{
    bool            host    = false;
    bool            lan     = false;

    sf::IPAddress   IPHost;
    sf::IPAddress   YourIP;

    cout << "Internet(0) or lan(1) ?" << endl;
    cin>>lan;

    cout << "Client(0) or host(1) ?" << endl;
    cin>>host;

    if(!host)
    {
        cout << "Host's IP :" << endl;
        cin  >> IPHost;

        if (!IPHost.IsValid())
        {
            cout << "Invalid IP" << endl;
            exit(0);
        }

    }

    if(lan)
        YourIP = sf::IPAddress::GetLocalAddress();
    else
        YourIP = sf::IPAddress::GetPublicAddress();

    cout << "Your IP : " << YourIP <<endl;


    if(host)
    {
        sf::SelectorTCP Selector;

        sf::SocketTCP Listener;
        if (!Listener.Listen(4567))
        {
            // Erreur...
        }

        Selector.Add(Listener);

        int action = -1;

        do
        {
            unsigned int NbSockets = Selector.Wait(1);

            for (unsigned int i = 0; i < NbSockets; ++i)
            {
                sf::SocketTCP Socket = Selector.GetSocketReady(i);

                if (Socket == Listener)
                {
                    // Si le socket écouteur est prêt, cela signifie que nous pouvons accepter une nouvelle connexion
                    sf::IPAddress Address;
                    sf::SocketTCP Client;
                    Listener.Accept(Client, &Address);
                    std::cout << "Client connected ! (" << Address << ")" << std::endl;

                    // On l'ajoute au sélecteur
                    Selector.Add(Client);
                }
                /*else
                {
                    // Sinon, il s'agit d'un socket de client et nous pouvons lire les données qu'il nous envoie
                    sf::Packet Packet;
                    if (Socket.Receive(Packet) == sf::Socket::Done)
                    {
                        // On extrait le message et on l'affiche
                        std::string Message;
                        Packet >> Message;
                        std::cout << "A client says : \"" << Message << "\"" << std::endl;
                    }
                    else
                    {
                        // Erreur : on ferait mieux de retirer le socket du sélecteur
                        Selector.Remove(Socket);
                    }
                }*/
            }

            cout << "Your IP : " << YourIP <<endl;

            cout << "Exit(0) ; Refresh(1) ; Chat(2) ; Kick(3)" << endl;
            cin >> action;
        }
        while(action != 0);


    }
    else
    {
        sf::SocketTCP Client;
        if (Client.Connect(4567, IPHost) != sf::Socket::Done)
            cout << "Cannot connecting" << endl;
        else
            cout << "Connection estalished" << endl;

        int action = -1;
        do
        {
            cout << "Your IP : " << YourIP <<endl;

            cout << "Exit(0) ; Refresh(1) ; Chat(2)" << endl;
            cin >> action;
        }
        while(action != 0);

        Client.Close();
    }

    system("PAUSE");

    return 0;
}

