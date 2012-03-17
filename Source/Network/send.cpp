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

void Network::SendPacket(sf::Packet &packet)
{
    if(configuration->multi)
    {
        if(configuration->hote)
        {
            for (std::list<sf::TcpSocket*>::iterator it = m_clientsTCP.begin(); it != m_clientsTCP.end(); ++it)
            {
                sf::TcpSocket& client = **it;
                client.send(packet);

                //m_udp.Send(packet,m_host->GetRemoteAddress(),NET_PORT);
            }
        }
        else
        {
            if(m_host)
                m_host->send(packet);
        }
    }
}

void Network::SendSkin()
{
    if(configuration->multi)
    {
        sf::Packet packet;
        packet<<(sf::Int8)P_PLAYERSKIN;

        if(configuration->hote)
            packet<<(sf::Int8)0;

        packet<<(sf::Int8)jeu->hero.m_cas;

        for (int i=0; i<NOMBRE_MORCEAU_PERSONNAGE; ++i)
            packet<<jeu->hero.m_cheminModele[i]<<jeu->hero.m_pasEquipe[i];

        if(configuration->hote)
        {
            for (std::list<sf::TcpSocket*>::iterator it = m_clientsTCP.begin(); it != m_clientsTCP.end(); ++it)
            {
                sf::TcpSocket& client = **it;
                client.send(packet);
            }
        }
        else
            m_host->send(packet);
    }
}

void Network::SendChangeMap(const std::string &prochaineMap,const coordonnee &coordonneePerso)
{
    if(configuration->multi)
    {
        sf::Packet packet;
        packet<<(sf::Int8)P_CHANGEMAP<<prochaineMap<<coordonneePerso<<(sf::Int8)configuration->heure<<(sf::Int8)configuration->minute;

        if(configuration->hote)
        {
            for (std::list<sf::TcpSocket*>::iterator it = m_clientsTCP.begin(); it != m_clientsTCP.end(); ++it)
            {
                sf::TcpSocket& client = **it;
                client.send(packet);
            }
        }
        else
            m_host->send(packet);
    }
}

void Network::SendDegats(Hero *hero2, float degats, int type, float temps)
{
    if(configuration->multi && configuration->hote)
    {
        sf::Packet packet;
        packet<<(sf::Int8)P_DEGATS<<(sf::Int16)degats<<(sf::Int8)type<<(sf::Int16)temps;

        int no = 0;
        for (std::list<Hero>::iterator p = jeu->m_personnageClients.begin();
                p != jeu->m_personnageClients.end() && &*p != hero2; ++p, ++no) {}

        int no2 = 0;
        for (std::list<sf::TcpSocket*>::iterator it = m_clientsTCP.begin(); it != m_clientsTCP.end() && no2 < no; ++it, ++no2)
            if(no == no2 + 1)
            {
                sf::TcpSocket& client = **it;
                client.send(packet);
            }
    }
}

void Network::SendDegats(int no, float degats, int type, float temps)
{
    if(configuration->multi && !configuration->hote)
    {
        sf::Packet packet;
        packet<<(sf::Int8)P_DEGATS<<(sf::Int16)no<<(sf::Int16)degats<<(sf::Int8)type<<(sf::Int16)temps;
        m_host->send(packet);
       // m_udp.Send(packet,m_host->GetRemoteAddress(),NET_PORT);
    }
}

void Network::SendKillMonstre(int no, int angle, float degats)
{
    if(configuration->multi && configuration->hote)
    {
        sf::Packet packet;
        packet<<(sf::Int8)P_KILLMONSTRE<<(sf::Int16)no<<(sf::Int16)angle<<(sf::Int16)degats;

        for (std::list<sf::TcpSocket*>::iterator it = m_clientsTCP.begin(); it != m_clientsTCP.end(); ++it)
        {
            sf::TcpSocket& client = **it;
            client.send(packet);
           // m_udp.Send(packet,client.GetRemoteAddress(),NET_PORT);
        }
    }
}

void Network::SendUseMiracle(int monstre, int no)
{
    if(configuration->multi)
    {
        sf::Packet packet;
        packet<<(sf::Int8)P_MIRACLE<<(bool)false<<(sf::Int16)monstre<<(sf::Int8)no;

        if(configuration->hote)
        {
            for (std::list<sf::TcpSocket*>::iterator it = m_clientsTCP.begin(); it != m_clientsTCP.end(); ++it)
            {
                sf::TcpSocket& client = **it;
                client.send(packet);
            }
        }
    }
}

void Network::SendUseMiracle(int no, int monstre, coordonnee cible)
{
    if(configuration->multi)
    {
        sf::Packet packet;

        if(configuration->hote)
        {
            packet<<(sf::Int8)P_MIRACLE<<(bool)true<<(sf::Int8)0<<(sf::Int8)no<<(sf::Int16)monstre<<cible;

            for (std::list<sf::TcpSocket*>::iterator it = m_clientsTCP.begin(); it != m_clientsTCP.end(); ++it)
            {
                sf::TcpSocket& client = **it;
                client.send(packet);
            }
        }
        else
        {
            packet<<(sf::Int8)P_MIRACLE<<(bool)true<<(sf::Int8)no<<(sf::Int16)monstre<<cible;
            m_host->send(packet);
        }
    }
}

void Network::SendEraseFriend(int no)
{
    if(configuration->multi)
    {
        sf::Packet packet;
        packet<<(sf::Int8)P_ERASEFRIEND;

        packet<<(sf::Int16)no;

        if(configuration->hote)
        {
            for (std::list<sf::TcpSocket*>::iterator it = m_clientsTCP.begin(); it != m_clientsTCP.end(); ++it)
            {
                sf::TcpSocket& client = **it;
                client.send(packet);
            }
        }
        else
            m_host->send(packet);
    }
}


void Network::SendInteract()
{
    if(configuration->multi)
    {
        sf::Packet packet;
        packet<<(sf::Int8)P_INTERACT;

        if(configuration->hote)
            packet<<(sf::Int8)0;

        packet<<(sf::Int16)jeu->hero.m_cibleInt;

        if(configuration->hote)
        {
            for (std::list<sf::TcpSocket*>::iterator it = m_clientsTCP.begin(); it != m_clientsTCP.end(); ++it)
            {
                sf::TcpSocket& client = **it;
                client.send(packet);
            }
        }
        else
            m_host->send(packet);
    }
}

void Network::SendClimate(int no, bool actif)
{
    if(configuration->multi && configuration->hote)
    {
        sf::Packet packet;
        packet<<(sf::Int8)P_CLIMATE;

        packet<<(sf::Int8)no<<(bool)actif;

        for (std::list<sf::TcpSocket*>::iterator it = m_clientsTCP.begin(); it != m_clientsTCP.end(); ++it)
        {
            sf::TcpSocket& client = **it;
            client.send(packet);

            /*sf::IpAddress ip = client.GetRemoteAddress();
            short unsigned int port = NET_PORT;
            m_udp.Send(packet, ip, port);*/
        }
    }
}

void Network::SendQuest(int id, int type, int info1, int info2)
{
    if(configuration->multi)
    {
        sf::Packet packet;
        packet<<(sf::Int8)P_QUEST;

        packet<<(sf::Int16)id<<(sf::Int8)type<<(sf::Int16)info1<<(sf::Int16)info2;

        if(configuration->hote)
        {
            for (std::list<sf::TcpSocket*>::iterator it = m_clientsTCP.begin(); it != m_clientsTCP.end(); ++it)
            {
                sf::TcpSocket& client = **it;
                client.send(packet);
            }
        }
        else
            m_host->send(packet);
    }
}

void Network::SendMessage(std::string msg)
{
    if(!msg.empty())
    if(configuration->multi)
    {
        sf::Packet packet;
        packet<<(sf::Int8)P_MSGCHAT;

        if(configuration->hote)
            packet<<(sf::Int8)0;

        packet<<msg;

        if(configuration->hote)
        {
            for (std::list<sf::TcpSocket*>::iterator it = m_clientsTCP.begin(); it != m_clientsTCP.end(); ++it)
            {
                sf::TcpSocket& client = **it;
                client.send(packet);
            }
        }
        else
            m_host->send(packet);
    }
}


void Network::SendReady()
{
    if(configuration->multi)
    {
        sf::Packet packet;
        packet<<(sf::Int8)P_READY;

        if(configuration->hote)
            packet<<(sf::Int8)0;

        if(configuration->hote)
        {
            for (std::list<sf::TcpSocket*>::iterator it = m_clientsTCP.begin(); it != m_clientsTCP.end(); ++it)
            {
                sf::TcpSocket& client = **it;
                client.send(packet);
            }
        }
        else
            m_host->send(packet);
    }
}
