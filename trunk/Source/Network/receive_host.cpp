

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

sf::Packet& operator <<(sf::Packet& Packet,  Personnage& C);
sf::Packet& operator >>(sf::Packet& Packet, Personnage& C);


void Network::CheckPacketHost(sf::Packet &packet, int no, sf::Socket* it, sf::TcpSocket* tcp)
{
    sf::Int8 type;
    if((packet>>type))
    {
        if(type == P_NEWPLAYER)
        {
            Caracteristique caract;
            std::string cheminclasse;
            sf::Int8 nbr_mir;
            if((packet>>caract.nom>>caract>>cheminclasse>>nbr_mir))
            {

                std::cout<<caract.nom<<" was connected to the party"<<std::endl;
                jeu->m_personnageClients.push_back(Hero ());
                jeu->m_listHeroes.push_back(&jeu->m_personnageClients.back());
                jeu->m_personnageClients.back().m_cheminClasse = cheminclasse;
                jeu->m_personnageClients.back().m_caracteristiques = caract;
                jeu->m_personnageClients.back().m_personnage.setCaracteristique(caract);
                jeu->m_personnageClients.back().m_personnage.m_heroic = true;

                while(--nbr_mir >= 0)
                {
                    sf::Int8 v;
                    packet>>v;
                    jeu->m_personnageClients.back().m_lvl_miracles.push_back(v);
                    jeu->m_personnageClients.back().m_lvl_miracles_new.push_back(v);
                }

                jeu->m_personnageClients.back().m_classe.Charger(cheminclasse,jeu->m_personnageClients.back().m_lvl_miracles,caract);
                jeu->m_personnageClients.back().ChargerModele();

                jeu->hero.m_amis.push_back(&jeu->m_personnageClients.back().m_personnage);

                sf::Packet packet2;
                packet2<<(sf::Int8)P_NEWPLAYER<<caract.nom<<caract<<cheminclasse<<(sf::Int8)jeu->m_personnageClients.back().m_lvl_miracles.size();
                for(unsigned i = 0 ; i < jeu->m_personnageClients.back().m_lvl_miracles.size() ; ++i)
                    packet2<<(sf::Int8)jeu->m_personnageClients.back().m_lvl_miracles[i];

                for (std::list<sf::TcpSocket*>::iterator it2 = m_clientsTCP.begin(); it2 != m_clientsTCP.end(); ++it2)
                    if(*it2 != it)
                    {
                        sf::TcpSocket& client2 = **it2;
                        client2.send(packet2);
                    }

                sf::Packet packet3;
                packet3<<(sf::Int8)P_CHANGEMAP<<jeu->map->getNom()<<jeu->hero.m_personnage.getCoordonnee()<<(sf::Int8)configuration->heure<<(sf::Int8)configuration->minute;
                tcp->send(packet3);

                int no2 = 0;
                for (std::list<sf::TcpSocket*>::iterator it2 = m_clientsTCP.begin(); it2 != m_clientsTCP.end(); ++it2, ++no2)
                {
                    packet3.clear();
                    packet3<<(sf::Int8)P_READY<<(sf::Int8)no2;
                    tcp->send(packet3);
                }


            }
        }
        else if(type == P_PLAYERSKIN)
        {
            int i = 0;
            for (std::list<Hero>::iterator p = jeu->m_personnageClients.begin();
                    p != jeu->m_personnageClients.end() && i <= no; ++p, ++i)
                if(i == no)
                {
                    sf::Int8 cas;
                    if((packet>>cas))
                    {
                        p->m_cas = cas;
                        for (int k=0; k<NOMBRE_MORCEAU_PERSONNAGE; ++k)
                            packet>>p->m_cheminModeleNouveau[k]>>p->m_pasEquipe[k];
                        p->ChargerGraphics();
                    }
                }
        }
        else if(type == P_CHANGEMAP)
        {
            sf::Int8 h, m;
            std::string prochaineMap;
            coordonnee coordonneePerso;
            if((packet>>prochaineMap>>coordonneePerso>>h>>m))
            {
                SendChangeMap(prochaineMap, coordonneePerso);

                GlobalMutex.lock();
                jeu->m_chargement->setC_Chargement(prochaineMap,coordonneePerso);
                jeu->m_contexte = jeu->m_chargement;
                for (std::list<Hero>::iterator p = jeu->m_personnageClients.begin(); p != jeu->m_personnageClients.end(); ++p)
                    p->m_ready = false;

                configuration->heure = h;
                configuration->minute = m;
                GlobalMutex.unlock();
            }
        }
        else if(type == P_DEGATS && jeu->map && jeu->map->m_loaded)
        {
            sf::Int16  no2;
            sf::Int16 degats;
            sf::Int8  typeD;
            sf::Int16 temps;
            if((packet>>no2>>degats>>typeD>>temps))
            {
                int i = 0;
                for (std::list<Hero>::iterator p = jeu->m_personnageClients.begin();
                        p != jeu->m_personnageClients.end() && i <= no; ++p, ++i)
                    if(i == no)
                        jeu->map->InfligerDegats((int)no2,&p->m_personnage,(float)degats,(int)typeD,jeu,(float)temps);
            }
        }
        else if(type == P_PLAYERCARACT)
        {
            Personnage *perso = NULL;

            int i = 0;
            for (std::list<Hero>::iterator p = jeu->m_personnageClients.begin();
                    p != jeu->m_personnageClients.end() && i <= no; ++p, ++i)
                if(i == no)
                {
                    packet>>p->m_personnage;
                    perso = &p->m_personnage;

                    p->m_caracteristiques.vie =  p->m_personnage.getCaracteristique().vie;
                }

            int no2 = 0;

            if(perso)
                for (std::list<sf::TcpSocket*>::iterator it2 = m_clientsTCP.begin(); it2 != m_clientsTCP.end(); ++it2,++no2)
                    if(*it2 != it)
                    {
                        sf::TcpSocket& client2 = **it2;
                        sf::Packet packet2;
                        packet2<<(sf::Int8)P_PLAYERCARACT<<(sf::Int8)(no+(no<no2))<<*perso;//<<n;

                        //sf::IpAddress ip2 = client2.GetRemoteAddress();
                        //m_udp.Send(packet, ip2, NET_PORT);
                        client2.send(packet2);
                    }
        }
        else if(type == P_MIRACLE)
        {
            sf::Int8 noMiracle;
            sf::Int16 monstre;
            coordonnee cible;

            bool heroic;
            packet>>heroic;

            if(heroic)
            if((packet>>noMiracle>>monstre>>cible))
            {
                int i = 0;
                for (std::list<Hero>::iterator p = jeu->m_personnageClients.begin();
                        p != jeu->m_personnageClients.end() && i <= no; ++p, ++i)
                    if(i == no)
                    {
                        if (p->UtiliserMiracle(noMiracle, jeu->map->getEntiteMonstre(monstre), cible, jeu, true))
                        {
                            p->m_personnage.m_miracleEnCours.back().m_miracle_client = true;
                            p->m_personnage.m_miracleEnCours.back().m_infos.back()->m_cible = jeu->map->getEntiteMonstre(monstre);
                        }
                    }

                int no2 = 0;
                for (std::list<sf::TcpSocket*>::iterator it2 = m_clientsTCP.begin(); it2 != m_clientsTCP.end(); ++it2,++no2)
                    if(*it2 != it)
                    {
                        sf::TcpSocket& client2 = **it2;

                        sf::Packet packet2;
                        packet2<<(sf::Int8)P_MIRACLE<<(bool)true<<(sf::Int8)(no+(no<no2))<<(sf::Int8)noMiracle<<(sf::Int16)monstre<<cible;

                        client2.send(packet2);
                    }
            }
        }
        else if(type == P_INTERACT)
        {
            sf::Int16 n;

            GlobalMutex.lock();

            int i = 0;
            for (std::list<Hero>::iterator p = jeu->m_personnageClients.begin();
                    p != jeu->m_personnageClients.end() && i <= no; ++p, ++i)
                if(i == no)
                {
                    packet>>n;
                    p->m_cibleInt =  n;
                }

            int no2 = 0;

            for (std::list<sf::TcpSocket*>::iterator it2 = m_clientsTCP.begin(); it2 != m_clientsTCP.end(); ++it2,++no2)
                if(*it2 != it)
                {
                    sf::TcpSocket& client2 = **it2;
                    sf::Packet packet2;
                    packet2<<(sf::Int8)P_INTERACT<<(sf::Int8)(no+(no<no2))<<n;

                    client2.send(packet2);
                }

            GlobalMutex.unlock();
        }
        else if(type == P_ERASEFRIEND)
        {
            sf::Int16 no;

            GlobalMutex.lock();

            if((packet>>no))
            {
                jeu->hero.EraseFriend(no);

                for (std::list<sf::TcpSocket*>::iterator it2 = m_clientsTCP.begin(); it2 != m_clientsTCP.end(); ++it2)
                if(*it2 != it)
                {
                    sf::TcpSocket& client2 = **it2;
                    sf::Packet packet2;
                    packet2<<(sf::Int8)P_ERASEFRIEND<<no;

                    client2.send(packet2);
                }
            }

            GlobalMutex.unlock();
        }
        else if(type == P_QUEST)
        {
            GlobalMutex.lock();

            sf::Int16 id, i1, i2;
            sf::Int8  t;
            if(packet>>id>>t>>i1>>i2)
            {
                if(t == 0)
                    jeu->hero.NewQuest(id);
                if(t == 1)
                    jeu->hero.SetQuestName(id, i1);
                if(t == 2)
                    jeu->hero.SetQuestState(id, i1, i2);
                if(t == 3)
                    jeu->hero.SetQuestActif(id, i1);
                SendQuest(id,t,i1,i2);
            }

            GlobalMutex.unlock();
        }
        else if(type == P_MSGCHAT)
        {
            std::string msg;

            GlobalMutex.lock();

            int i = 0;
            for (std::list<Hero>::iterator p = jeu->m_personnageClients.begin();
                    p != jeu->m_personnageClients.end() && i <= no; ++p, ++i)
                if(i == no)
                {
                    packet>>msg;
                    p->m_personnage.m_speak = msg;
                    p->m_personnage.m_speak_time = 5000;

                    console->Ajouter(p->m_caracteristiques.nom + " : "+msg);
                }

            int no2 = 0;

            for (std::list<sf::TcpSocket*>::iterator it2 = m_clientsTCP.begin(); it2 != m_clientsTCP.end(); ++it2,++no2)
                if(*it2 != it)
                {
                    sf::TcpSocket& client2 = **it2;
                    sf::Packet packet2;
                    packet2<<(sf::Int8)P_MSGCHAT<<(sf::Int8)(no+(no<no2))<<msg;

                    client2.send(packet2);
                }

            GlobalMutex.unlock();
        }
        else if(type == P_READY)
        {
            GlobalMutex.lock();

            int i = 0;
            for (std::list<Hero>::iterator p = jeu->m_personnageClients.begin();
                    p != jeu->m_personnageClients.end() && i <= no; ++p, ++i)
                if(i == no)
                    p->m_ready = true;

            int no2 = 0;

            for (std::list<sf::TcpSocket*>::iterator it2 = m_clientsTCP.begin(); it2 != m_clientsTCP.end(); ++it2,++no2)
                if(*it2 != it)
                {
                    sf::TcpSocket& client2 = **it2;
                    sf::Packet packet2;
                    packet2<<(sf::Int8)P_READY<<(sf::Int8)(no+(no<no2));

                    client2.send(packet2);
                }

            GlobalMutex.unlock();
        }
    }
}
