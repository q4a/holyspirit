

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


#include "../constantes.h"
#include "../globale.h"
#include "../jeu.h"

sf::Packet& operator <<(sf::Packet& Packet, const Caracteristique& C)
{
    return Packet << (sf::Int32)C.vie << (sf::Int32)C.maxVie << (sf::Int16)C.niveau;
}
sf::Packet& operator >>(sf::Packet& Packet, Caracteristique& C)
{
    sf::Int32 vie, maxVie;
    sf::Int16 niveau;

    if((Packet >> vie >> maxVie >> niveau))
    {
        C.vie = vie;
        C.maxVie = maxVie;
        C.niveau = niveau;
    }

    return Packet;
}

sf::Packet& operator <<(sf::Packet& Packet, const coordonneeDecimal& C)
{
    return Packet << (sf::Int32)C.x << (sf::Int32)C.y << (sf::Int32)C.h << (sf::Int32)C.w;
}
sf::Packet& operator >>(sf::Packet& Packet, coordonneeDecimal& C)
{
    sf::Int32 x,y,h,w;
    if((Packet >> x >> y >> h >> w))
    {
         C.x = x;
        C.y = y;
        C.h = h;
        C.w = w;
    }

    return Packet;
}

sf::Packet& operator <<(sf::Packet& Packet, const coordonnee& C)
{
    return Packet << (sf::Int32)C.x << (sf::Int32)C.y << (sf::Int32)C.h << (sf::Int32)C.w;
}
sf::Packet& operator >>(sf::Packet& Packet, coordonnee& C)
{
    sf::Int32 x,y,h,w;
    if((Packet >> x >> y >> h >> w))
    {
        C.x = x;
        C.y = y;
        C.h = h;
        C.w = w;
    }

    return Packet;
}


sf::Packet& operator <<(sf::Packet& Packet,  Personnage& C)
{
    return Packet << C.getCaracteristique() << C.getCoordonneePixel() << (sf::Int32)C.getTime() << (sf::Int8)C.getEtat()  << (sf::Int16)C.m_entite_graphique.m_noAnimation /*<< (sf::Int32)(C.m_entite_graphique.m_animation*1000)*/ << (sf::Int16)C.getAngle();
}
sf::Packet& operator >>(sf::Packet& Packet, Personnage& C)
{
    coordonneeDecimal pos;
    sf::Int8 etat;
    sf::Int16 angle,pose;
    sf::Int32 anim, time;

    Caracteristique caract = C.getCaracteristique();

    if((Packet >> caract >> pos >> time >> etat >> pose /*>> anim*/ >> angle))
    {
        C.setEmulatePos(pos,(float)(time)/1000);

        if(fabs(pose - C.m_entite_graphique.m_noAnimation) <= 2
        && etat == C.getEtat())
            pose = C.m_entite_graphique.m_noAnimation;
       // else
         //   C.m_entite_graphique.m_animation = (float)anim/1000;

        C.setEtat(etat,pose);
        C.setForcedAngle(angle);
        C.setCaracteristique(caract);
    }

    return Packet;
}

Network::Network()
{
    jeu = NULL;

    m_host = NULL;
    m_thread_clientTCP = NULL;
    m_thread_clientUDP = NULL;
    m_thread_host = NULL;
}

Network::~Network()
{

}

int  Network::GetNoClient(const sf::IpAddress &address)
{
    int no = 0;
    for (std::list<sf::TcpSocket*>::iterator it = m_clientsTCP.begin(); it != m_clientsTCP.end(); ++it, ++no)
        if((*it)->GetRemoteAddress() == address)
            return no;

    return -1;
}


void GererReseauClientTCP(void* UserData)
{
    sf::Context context;
    Jeu *jeu = static_cast<Jeu*>(UserData);

    while(net->m_host)
    {
        sf::Packet packet;
        if (net->m_host->Receive(packet) == sf::Socket::Done)
           net->CheckPacketClient(packet);
    }
}
void GererReseauClientUDP(void* UserData)
{
    Jeu *jeu = static_cast<Jeu*>(UserData);

    sf::IpAddress ip;
    short unsigned int port;

    while(net->m_host)
    {

      //  sf::Sleep(0.001);

        sf::Packet packet;
        if(net->m_udp.Receive(packet, ip, port) == sf::Socket::Done)
            net->CheckPacketClient(packet);
    }
}

void GererReseauHost(void* UserData)
{
    sf::Context context;
    Jeu *jeu = static_cast<Jeu*>(UserData);

    while(net->m_runTCPHost)
    {
        if (net->m_selector.Wait(500))
        {
            if (net->m_selector.IsReady(net->m_listener))
            {
                sf::TcpSocket* clientTCP = new sf::TcpSocket;

                if (net->m_listener.Accept(*clientTCP) == sf::Socket::Done)
                    net->AddClient(clientTCP);
                else
                    delete clientTCP;
            }
            else
            {
                int no = 0;
                for (std::list<sf::TcpSocket*>::iterator it = net->m_clientsTCP.begin(); it != net->m_clientsTCP.end(); ++it, ++no)
                {
                    sf::TcpSocket& client = **it;
                    if (net->m_selector.IsReady(client))
                    {
                        sf::Packet packet;
                        if (client.Receive(packet) == sf::Socket::Done)
                            net->CheckPacketHost(packet, no, &client, &client);
                    }
                }

                /*if (net->m_selector.IsReady(net->m_udp))
                {
                    sf::IpAddress ip;
                    short unsigned int port;
                    sf::Packet packet;
                    if (net->m_udp.Receive(packet,ip,port) == sf::Socket::Done) {
                        net->CheckPacketHost(packet, net->GetNoClient(ip), &net->m_udp, NULL);
                    }
                }*/
            }
        }
    }
}



void Network::GererMultijoueur(Jeu* p_jeu)
{
    jeu = p_jeu;
   // m_net_send = false;
    if(configuration->multi)
    if(m_net_clock.GetElapsedTime() > (float)configuration->net_rate)
    {
        jeu->m_net_send = true;
        m_net_clock.Reset();

        if(configuration->hote)
        {
            sf::Packet packet2;
            packet2<<(sf::Int8)P_PLAYERCARACT<<(sf::Int8)0<<jeu->hero.m_personnage;

            int no = 0;
            for (std::list<sf::TcpSocket*>::iterator it = m_clientsTCP.begin(); it != m_clientsTCP.end(); ++it, ++no)
            {
                sf::TcpSocket& client = **it;

                sf::Packet ping;
                ping<<(sf::Int8)(-1);
                if(client.Send(ping) != sf::TcpSocket::Done)
                {
                    m_selector.Remove(*(*it));
                    m_clientsTCP.erase(it);
                    //delete *&*it;

                    if(!DeletePersonnageClient(no))
                        std::cout<<"Un joueur est partit"<<std::endl;
                    int no2 = 1;
                    for (std::list<sf::TcpSocket*>::iterator it2 = m_clientsTCP.begin(); it2 != m_clientsTCP.end(); ++it2,++no2)
                    {
                        sf::Packet packet3;
                        packet3<<(sf::Int8)P_DELETEPLAYER<<(sf::Int8)(no+(no>no2));
                        sf::TcpSocket& client2 = **it2;
                        client2.Send(packet3);
                    }

                    it = m_clientsTCP.begin();
                    no = 0;
                }
                else
                {
                    /*sf::IpAddress ip = client.GetRemoteAddress();
                    short unsigned int port = NET_PORT;
                    m_udp.Send(packet2, ip, port);*/
                    client.Send(packet2);
                }
            }
        }
        else if(m_host)
        {
            sf::Packet ping;
            ping<<(sf::Int8)(-1);
            if(!(m_host->Send(ping) == sf::TcpSocket::Done))
            {
                jeu->m_mainMenu->m_save  = true;
                jeu->m_mainMenu->m_reset = true;
                jeu->m_contexte = jeu->m_mainMenu;
                std::cout<<"L'hôte à quitté la partie"<<std::endl;

                Disconnect();
                CloseServer();
            }
            else
            {
                sf::Packet packet;
                //sf::UdpSocket upd;
                packet<<(sf::Int8)P_PLAYERCARACT<<jeu->hero.m_personnage;
               /* sf::IpAddress ip = m_host->GetRemoteAddress();
                short unsigned int port = NET_PORT;
                m_udp.Send(packet, ip, port);*/
                m_host->Send(packet);
            }
        }
    }
}

void Network::LaunchServer()
{
    CloseServer();


    //m_udp.Bind(NET_PORT);
    m_listener.Listen(NET_PORT);

   // m_selector.Add(m_udp);
    m_selector.Add(m_listener);

    m_runTCPHost = true;
    m_thread_host = new sf::Thread(&GererReseauHost, this);
    m_thread_host->Launch();
}

void Network::CloseServer()
{
    m_runTCPHost = false;
    m_listener.Close();
    //m_udp.Unbind();

    if(m_thread_host)
    {
        m_thread_host->Wait();
        delete m_thread_host;
    }
    m_thread_host = NULL;

    jeu->m_personnageClients.clear();

    for (std::list<sf::TcpSocket*>::iterator it = m_clientsTCP.begin(); it != m_clientsTCP.end(); ++it)
        delete *it;
    m_clientsTCP.clear();

    m_selector.Clear();
}

bool Network::Connect(sf::IpAddress ServerAddress)
{
    m_host = new sf::TcpSocket();

    if (m_host->Connect(ServerAddress, NET_PORT, 3000) != sf::TcpSocket::Done)
    {
        delete m_host;
        m_host = NULL;
        return false;
    }

    //m_udp.Bind(NET_PORT);

    m_thread_clientTCP = new sf::Thread(&GererReseauClientTCP, this);
    m_thread_clientTCP->Launch();

    /*m_thread_clientUDP = new sf::Thread(&GererReseauClientUDP, this);
    m_thread_clientUDP->Launch();*/

    return true;
}

void Network::Disconnect()
{
    std::cout<<"Vous avez ete deconnecte"<<std::endl;

    GlobalMutex.Lock();
    if(m_host)
    m_host->Disconnect();
    if(m_host)
        delete m_host;
    m_host = NULL;
    //m_udp.Unbind();
    GlobalMutex.Unlock();

    std::cout<<"Vous avez ete deconnecte 2"<<std::endl;

    if(m_thread_clientTCP)
    {
        m_thread_clientTCP->Wait();
        delete m_thread_clientTCP;
    }
    std::cout<<"Vous avez ete deconnecte 3"<<std::endl;

    if(m_thread_clientUDP)
    {
        m_thread_clientUDP->Wait();
        delete m_thread_clientUDP;
    }

    std::cout<<"Vous avez ete deconnecte 4"<<std::endl;

    m_thread_clientTCP = NULL;
    m_thread_clientUDP = NULL;
}


void Network::AddClient(sf::TcpSocket* clientTCP)
{
    sf::Packet packet;

    m_clientsTCP.push_back(clientTCP);

    std::cout<<"Un joueur arrive !"<<std::endl;

    m_selector.Add(*clientTCP);

    packet<<(sf::Int8)P_NEWPLAYER<<jeu->hero.m_caracteristiques.nom<<jeu->hero.m_caracteristiques
          <<jeu->hero.m_cheminClasse<<(sf::Int8)jeu->hero.m_lvl_miracles.size();
    for(unsigned i = 0 ; i < jeu->hero.m_lvl_miracles.size() ; ++i)
        packet<<(sf::Int8)jeu->hero.m_lvl_miracles[i];
    clientTCP->Send(packet);

    packet.Clear();
    packet<<(sf::Int8)P_PLAYERSKIN<<(sf::Int8)0<<(sf::Int8)jeu->hero.m_cas;
    for (int i=0; i<NOMBRE_MORCEAU_PERSONNAGE; ++i)
        packet<<jeu->hero.m_cheminModele[i]<<jeu->hero.m_pasEquipe[i];
    clientTCP->Send(packet);

    int no2 = 1;
    for (std::list<Hero>::iterator p = jeu->m_personnageClients.begin();
            p != jeu->m_personnageClients.end(); ++p, ++no2)
    {
        packet.Clear();
        packet<<(sf::Int8)P_NEWPLAYER<<p->m_caracteristiques.nom<<p->m_caracteristiques<<p->m_cheminClasse<<(sf::Int8)p->m_lvl_miracles.size();
        for(unsigned i = 0 ; i < p->m_lvl_miracles.size() ; ++i)
            packet<<(sf::Int8)p->m_lvl_miracles[i];
        clientTCP->Send(packet);

        packet.Clear();
        packet<<(sf::Int8)P_PLAYERSKIN<<(sf::Int8)no2<<(sf::Int8)p->m_cas;
        for (int i=0; i<NOMBRE_MORCEAU_PERSONNAGE; ++i)
            packet<<p->m_cheminModele[i]<<p->m_pasEquipe[i];
        clientTCP->Send(packet);
    }
}

bool Network::DeletePersonnageClient(int no)
{
    int i = 0;
    for (std::list<Hero>::iterator p = jeu->m_personnageClients.begin();
            p != jeu->m_personnageClients.end() && i <= no; ++p, ++i)
        if(i == no)
        {
            std::cout<<p->m_caracteristiques.nom<<" est partit"<<std::endl;

            GlobalMutex.Lock();
            for(unsigned j = 0 ; j < jeu->hero.m_amis.size() ; ++j)
                if(jeu->hero.m_amis[j] == &p->m_personnage)
                    jeu->hero.m_amis.erase(jeu->hero.m_amis.begin() + j);
            jeu->m_personnageClients.erase(p);
            GlobalMutex.Unlock();

            return true;
        }
    return false;
}



