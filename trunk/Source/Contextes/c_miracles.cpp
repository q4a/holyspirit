


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


#include "c_jeu.h"
#include "../jeu.h"

#include "../globale.h"

#include <iostream>
#include <fstream>

#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

using namespace sf;
using namespace std;

int GestionBoutons(Jeu *jeu, bool = false, bool = false, bool = false);

inline sf::Vector2f AutoScreenAdjust(float x, float y, float decalage = 0)
{
    sf::Vector2f temp;
    temp.x = x + (configuration->Resolution.x - 800) * 0.5;
    temp.y = y + (configuration->Resolution.y - 600) - decalage * configuration->Resolution.h/600;
    return temp;
}


c_Miracles::c_Miracles()
{
    m_decalage = -600;
    m_fenetre = 0;
}
c_Miracles::~c_Miracles()
{
}


void c_Miracles::Utiliser(Jeu *jeu)
{
    temps_ecoule=jeu->Clock.getElapsedTime().asMilliseconds();
    jeu->m_display=true;
    jeu->Clock.restart();

    moteurGraphique->Gerer(0);
    jeu->map->GererAmbiance(temps_ecoule);
    //jeu->map->Animer(&jeu->hero,0);
    jeu->map->Afficher(jeu->m_listHeroes,0,jeu->m_jeu->alpha_map);
    jeu->hero.AfficherAmisEtCraft();

    if (m_afficher)
        m_decalage+=temps_ecoule*2000;
    else
        m_decalage-=temps_ecoule*2000;

    if (m_decalage>0)
        m_decalage=0;
    if (m_decalage<-600)
    {
        m_decalage=-600;
        jeu->Next();
        m_afficher = 1;
    }

    int temp = GestionBoutons(jeu);
    if(temp >= 0)
    {
        jeu->next_screen = temp;
        m_afficher=0;
        jeu->Clock.restart();

        if(jeu->next_screen == 5 || jeu->next_screen == 4)
            jeu->next_screen = 3;
    }

    jeu->menu.AfficherMiracles(m_decalage, &jeu->hero.m_classe);

    if(jeu->hero.m_miracleEnMain < 0)
        eventManager->AfficherCurseur();

    jeu->hero.AfficherMiracles(m_decalage, m_fenetre);

    jeu->menu.AfficherHUD(&jeu->hero.m_classe);
    jeu->menu.AfficherDynamique(jeu->hero.m_caracteristiques,-1,jeu->hero.m_caracteristiques,&jeu->hero.m_classe);

    for(int i = 0;i < (int)jeu->hero.m_classe.boutons_miracles.size(); ++i)
        if(eventManager->getPositionSouris().x >  AutoScreenAdjust(jeu->hero.m_classe.boutons_miracles[i].position.x,0).x
         &&eventManager->getPositionSouris().x <  AutoScreenAdjust(jeu->hero.m_classe.boutons_miracles[i].position.x,0).x + jeu->hero.m_classe.boutons_miracles[i].position.w
         &&eventManager->getPositionSouris().y >  AutoScreenAdjust(0,jeu->hero.m_classe.boutons_miracles[i].position.y).y
         &&eventManager->getPositionSouris().y <  AutoScreenAdjust(0,jeu->hero.m_classe.boutons_miracles[i].position.y).y + jeu->hero.m_classe.boutons_miracles[i].position.h)
        {
            coordonnee buf = eventManager->getPositionSouris();
            buf.y -= 20;
            moteurGraphique->AjouterTexte(jeu->hero.m_classe.boutons_miracles[i].nom, buf,jeu->hero.m_classe.border, 17,0,14,sf::Color(224,224,224));

            if (eventManager->getEvenement(Mouse::Left,EventClic))
            {
                m_fenetre = jeu->hero.m_classe.boutons_miracles[i].lien;
                eventManager->StopEvenement(Mouse::Left,EventClic);
            }
        }

    coordonnee position;
    position.x=(jeu->hero.m_personnage.getCoordonnee().x-jeu->hero.m_personnage.getCoordonnee().y-1)/5;
    position.y=(jeu->hero.m_personnage.getCoordonnee().x+jeu->hero.m_personnage.getCoordonnee().y)/5;

    Listener::setGlobalVolume((float)configuration->volume);
    Listener::setPosition(-position.x, 0, position.y);
    Listener::setDirection(0, 0, 1);
    jeu->map->MusiquePlay();
    jeu->sonMort.setPosition(position.x,0,position.y);
}

