

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



#ifndef JEUH
#define JEUH


#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Network.hpp>

#include "constantes.h"
#include "Map/map.h"
#include "Moteurs/eventManager.h"
#include "menu.h"

#include "Contextes/c_jeu.h"
#include "Contextes/c_changementMap.h"
#include "Contextes/c_inventaire.h"
#include "Contextes/c_quetes.h"
#include "Contextes/c_documents.h"
#include "Contextes/c_potales.h"
#include "Contextes/c_craft.h"
#include "Contextes/c_bless.h"
#include "Contextes/c_miracles.h"
#include "Contextes/c_menuInGame.h"
#include "Contextes/c_demarrage.h"
#include "Contextes/c_mainMenu.h"

class Jeu
{
    public:

    Jeu();
    void Demarrer();
    void Next();


    void GererMultijoueur();
    bool Connect(sf::IpAddress);
    void Disconnect();
    void LaunchServer();
    void CloseServer();
    bool DeletePersonnageClient(int no);

    void AddClient(sf::TcpSocket* clientTCP);
    void CheckPacketHost(sf::Packet &packet, int no, sf::Socket* it, sf::TcpSocket* tcp);
    void CheckPacketClient(sf::Packet &packet);

    int  GetNoClient(const sf::IpAddress &address);

    void SendSkin();
    void SendChangeMap(const std::string &prochaineMap,const coordonnee &coordonneePerso);
    void SendInfosMonstre(sf::Packet &packet);
    void SendDegats(Hero *hero, float degats, int type, float temps = 0);
    void SendDegats(int no, float degats, int type, float temps = 0);
    void SendKillMonstre(int no, int angle, float degats);
    void SendInteract();
    void SendQuest(int id, int type, int info1 = -1, int info2 = -1);
    void SendMessage(std::string msg);
    void SendReady();

    Menu menu;
    Map *map;
    Hero hero;
    sf::Clock Clock, MusicClock;
    sf::SoundBuffer bufferSonMort;
	sf::Sound  sonMort;

	sf::SocketSelector  m_selector;
    sf::TcpListener     m_listener;
    std::list<sf::TcpSocket*> m_clientsTCP;
    std::list<Hero> m_personnageClients;

    sf::TcpSocket *m_host;
    sf::UdpSocket m_udp;

    bool m_runTCPHost;

	bool m_run,m_display;

	c_Demarrage     *m_demarrage;
    c_Jeu           *m_jeu;
    c_Chargement    *m_chargement;
    c_Inventaire    *m_inventaire;
    c_Quetes        *m_quetes;
    c_Potales       *m_potales;
    c_Craft         *m_craft;
    c_Documents     *m_docs;
    c_Bless         *m_bless;
    c_Miracles      *m_miracles;
    c_MenuInGame    *m_menuInGame;
    c_MainMenu      *m_mainMenu;
    Contexte        *m_contexte;

    int next_screen;

    sf::Mutex GlobalMutex;

    bool m_net_send;

    private:
    sf::Thread *m_thread_clientTCP;
    sf::Thread *m_thread_clientUDP;
    sf::Thread *m_thread_host;

    sf::Clock m_net_clock;
};

#endif



