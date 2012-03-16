

/*Copyright (C) 2009 Naisse Grégoire

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License along
with this program; if not, write to the Free Software Foundation, Inc.,
51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.*/



#ifndef NETH
#define NETH

#include <SFML/System.hpp>
#include <SFML/Network.hpp>

class Jeu;
class Hero;

class Network : public CSingleton<Network>
{

public:
	friend Network* CSingleton<Network>::GetInstance();
    friend void CSingleton<Network>::Kill();

    bool Connect(sf::IpAddress);
    void Disconnect();
    void LaunchServer();
    void CloseServer();
    bool DeletePersonnageClient(int no);

    void AddClient(sf::TcpSocket* clientTCP);
    void CheckPacketHost(sf::Packet &packet, int no, sf::Socket* it, sf::TcpSocket* tcp);
    void CheckPacketClient(sf::Packet &packet);

    int  GetNoClient(const sf::IpAddress &address);

    void SendPacket(sf::Packet &packet);
    void SendSkin();
    void SendChangeMap(const std::string &prochaineMap,const coordonnee &coordonneePerso);
    void SendDegats(Hero *hero, float degats, int type, float temps = 0);
    void SendDegats(int no, float degats, int type, float temps = 0);
    void SendKillMonstre(int no, int angle, float degats);
    void SendUseMiracle(int monstre, int no);
    void SendUseMiracle(int no, int monstre, coordonnee cible);
    void SendEraseFriend(int no);
    void SendInteract();
    void SendQuest(int id, int type, int info1 = -1, int info2 = -1);
    void SendClimate(int no, bool actif);
    void SendMessage(std::string msg);
    void SendReady();

    void GererMultijoueur(Jeu *jeu);

	sf::SocketSelector  m_selector;
    sf::TcpListener     m_listener;
    std::list<sf::TcpSocket*> m_clientsTCP;
    sf::TcpSocket *m_host;
    sf::UdpSocket m_udp;

    bool m_runTCPHost;

    sf::Mutex GlobalMutex;

private:
    Network();
	~Network();

    sf::Thread *m_thread_clientTCP;
    sf::Thread *m_thread_clientUDP;
    sf::Thread *m_thread_host;

    sf::Clock m_net_clock;

    Jeu *jeu;
};

#endif
