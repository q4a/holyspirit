

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

void Network::CheckPacketClient(sf::Packet &packet)
{
    sf::Int8 type;
    if((packet>>type))
    {
        if(type == P_PLAYERCARACT)
        {
            sf::Int8 no;
            if((packet>>no))
            {
                int i = 0;
                for (std::list<Hero>::iterator p = jeu->m_personnageClients.begin();
                        p != jeu->m_personnageClients.end() && i <= no; ++p, ++i)
                    if(i == no)
                    {
                        packet>>p->m_personnage;
                        p->m_caracteristiques.vie = p->m_personnage.getCaracteristique().vie;
                    }
            }
        }
        else if(type == P_INFOSMONSTRE && jeu->map)
        {
            do
            {
                type = (sf::Int8)-1;
                sf::Int16 no;
                if((packet>>no))
                {
                    if(jeu->map->m_loaded)
                    if(jeu->map->getEntiteMonstre(no))
                    {
                        GlobalMutex.Lock();

                        bool enVie = jeu->map->getEntiteMonstre(no)->EnVie();
                        int x = (int)((jeu->map->getEntiteMonstre(no))->getCoordonneePixel().x/COTE_TILE + 0.5);
                        int y = (int)((jeu->map->getEntiteMonstre(no))->getCoordonneePixel().y/COTE_TILE + 0.5);
                        packet>>*(jeu->map->getEntiteMonstre(no));
                        jeu->map->DeplacerMonstreCase(no,x,y);

                        if(enVie != jeu->map->getEntiteMonstre(no)->EnVie())
                            jeu->map->KillMonstre(jeu->map->getEntiteMonstre(no),0,0,jeu);

                        GlobalMutex.Unlock();

                        if(!packet.EndOfPacket())
                            if(!(packet>>type))
                                type = (sf::Int8)-1;
                    }
                }
            }while(type == sf::Int8(P_INFOSMONSTRE));
        }
        else if(type == P_NEWPLAYER)
        {
            std::string cheminclasse, nom;
            Caracteristique caract;
            sf::Int8 nbr_mir;
            if((packet>>caract.nom>>caract>>cheminclasse>>nbr_mir))
            {
                jeu->m_personnageClients.push_back(Hero ());
                jeu->m_personnageClients.back().m_cheminClasse = cheminclasse;
                jeu->m_personnageClients.back().m_caracteristiques = caract;
                jeu->m_personnageClients.back().m_personnage.setCaracteristique(caract);
                jeu->m_personnageClients.back().m_personnage.m_heroic = true;

                GlobalMutex.Lock();

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
                std::cout<<"Joueur " << caract.nom << std::endl;

                GlobalMutex.Unlock();
            }
        }
        else if(type == P_DELETEPLAYER)
        {
            sf::Int8 no;
            if((packet>>no))
                DeletePersonnageClient((int)no);
        }
        else if(type == P_PLAYERSKIN)
        {
            int i = 0;
            sf::Int8 no;

            if((packet>>no))
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

                        GlobalMutex.Lock();
                        p->ChargerGraphics();
                        GlobalMutex.Unlock();
                    }
                }
        }
        else if(type == P_CHANGEMAP)
        {
            sf::Int8 h,m;
            std::string prochaineMap;
            coordonnee coordonneePerso;
            if((packet>>prochaineMap>>coordonneePerso>>h>>m))
            {
                GlobalMutex.Lock();
                jeu->m_chargement->setC_Chargement(prochaineMap,coordonneePerso);
                jeu->m_contexte = jeu->m_chargement;
                for (std::list<Hero>::iterator p = jeu->m_personnageClients.begin(); p != jeu->m_personnageClients.end(); ++p)
                    p->m_ready = false;
                configuration->heure = h;
                configuration->minute = m;
                GlobalMutex.Unlock();
            }
        }
        else if(type == P_DEGATS)
        {
            sf::Int16 degats;
            sf::Int8  typeD;
            sf::Int16 temps;
            if((packet>>degats>>typeD>>temps))
                jeu->hero.m_personnage.InfligerDegats(degats, typeD, temps);
        }
        else if(type == P_KILLMONSTRE && jeu->map)
        {
            sf::Int16 no,angle;
            sf::Int16 degats;

            if((packet>>no>>angle>>degats))
            if(jeu->map->m_loaded)
                jeu->map->KillMonstre(jeu->map->getEntiteMonstre(no),angle,degats,jeu);
        }
        else if(type == P_MIRACLE && jeu->map)
        {
            sf::Int16 monstre;
            sf::Int8 no,noMiracle;
            coordonnee cible;

            bool heroic;
            packet>>heroic;

            if(!heroic)
            if((packet>>monstre>>noMiracle))
                if(jeu->map->m_loaded)
                {
                    Monstre *m = jeu->map->getEntiteMonstre(monstre);
                    if(m)
                        m->m_miracleALancer = noMiracle;
                }

            if(heroic)
            if((packet>>no>>noMiracle>>monstre>>cible))
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
            }
        }
        else if(type == P_MSGCHAT)
        {
            int i = 0;
            sf::Int8 no;

            GlobalMutex.Lock();

            if((packet>>no))
            for (std::list<Hero>::iterator p = jeu->m_personnageClients.begin();
                    p != jeu->m_personnageClients.end() && i <= no; ++p, ++i)
                if(i == no)
                {
                    packet>>p->m_personnage.m_speak;
                    p->m_personnage.m_speak_time = 5000;
                    console->Ajouter(p->m_caracteristiques.nom + " : "+p->m_personnage.m_speak);
                }

            GlobalMutex.Unlock();
        }
        else if(type == P_INTERACT)
        {
            int i = 0;
            sf::Int8 no;

            GlobalMutex.Lock();

            if((packet>>no))
            for (std::list<Hero>::iterator p = jeu->m_personnageClients.begin();
                    p != jeu->m_personnageClients.end() && i <= no; ++p, ++i)
                if(i == no)
                {
                    sf::Int16 n;
                    packet>>n;
                    p->m_cibleInt = n;
                }

            GlobalMutex.Unlock();
        }
        else if(type == P_QUEST)
        {
            GlobalMutex.Lock();

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
            }

            GlobalMutex.Unlock();
        }
        else if(type == P_CLIMATE && jeu->map)
        {
            sf::Int8 no;
            bool actif;
            if(packet>>no>>actif)
                jeu->map->SetClimate(no, actif);
        }
        else if(type == P_READY)
        {
            int i = 0;
            sf::Int8 no;

            GlobalMutex.Lock();

            if((packet>>no))
            for (std::list<Hero>::iterator p = jeu->m_personnageClients.begin();
                    p != jeu->m_personnageClients.end() && i <= no; ++p, ++i)
                if(i == no)
                    p->m_ready = true;

            GlobalMutex.Unlock();
        }

    }
}
