

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
c_Documents::c_Documents()
{
    m_decalage=-600;
}
c_Documents::~c_Documents()
{
}

void c_Documents::Utiliser(Jeu *jeu)
{
    temps_ecoule=jeu->Clock.getElapsedTime().asSeconds();
    jeu->m_display=true;
    jeu->Clock.restart();

    jeu->map->GererAmbiance(temps_ecoule);

    moteurGraphique->Gerer(0);
    jeu->map->MusiquePlay();
    //jeu->map->Animer(&jeu->hero,0);
    jeu->map->Afficher(jeu->m_listHeroes,0,jeu->m_jeu->alpha_map);
    jeu->hero.AfficherAmisEtCraft();
    //jeu->menu.Afficher(2,jeu->m_jeu->alpha_map,&jeu->hero.m_classe);

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

    jeu->menu.AfficherDocs(m_decalage,&jeu->hero.m_classe);

    jeu->hero.AfficherDocs(m_decalage);

    jeu->menu.AfficherHUD(&jeu->hero.m_classe);
    jeu->menu.AfficherDynamique(jeu->hero.m_caracteristiques,-1,jeu->hero.m_caracteristiques,&jeu->hero.m_classe);

    eventManager->AfficherCurseur();

    if(temp >= 0)
    {
        jeu->next_screen = temp;
        m_afficher=0;
        jeu->Clock.restart();

        if(jeu->next_screen == 9 || jeu->next_screen == 4)
            jeu->next_screen = 3;
    }

    if (eventManager->getEvenement(Mouse::Left,EventClic))
    {
        eventManager->StopEvenement(Mouse::Left,EventClic);
        if(jeu->hero.m_docPointe >= 0)
        {
            jeu->hero.m_docSelectionne = jeu->hero.m_docPointe;
            moteurSons->JouerSon(configuration->sound_book,coordonnee (0,0),0);
        }
        if(jeu->hero.m_docPointe >= 0)
            jeu->hero.m_defil_cdoc = 0;
    }

    jeu->hero.m_defil_cdoc -= eventManager->getMolette();

    if(eventManager->getEvenement(Mouse::Left,EventClicA))
        if(jeu->hero.m_max_defil_cdoc-jeu->hero.m_classe.position_contenu_docs.h != 0)
          if (eventManager->getPositionSouris().x > AutoScreenAdjust(jeu->hero.m_classe.scroll_button_cdocs.position.x,0).x
            &&eventManager->getPositionSouris().x < AutoScreenAdjust(jeu->hero.m_classe.scroll_button_cdocs.position.x+jeu->hero.m_classe.scroll_button_cdocs.position.w,0).x
            &&eventManager->getPositionSouris().y - jeu->hero.m_classe.scroll_button_cdocs.position.h/2 > AutoScreenAdjust(0,jeu->hero.m_classe.scroll_button_cdocs.position.y).y
            &&eventManager->getPositionSouris().y - jeu->hero.m_classe.scroll_button_cdocs.position.h/2 < AutoScreenAdjust(0,jeu->hero.m_classe.scroll_button_cdocs.position.y+jeu->hero.m_classe.position_contenu_docs.h*19+jeu->hero.m_classe.scroll_button_cdocs.position.h).y)
                jeu->hero.m_defil_cdoc = (int)((float)(eventManager->getPositionSouris().y - jeu->hero.m_classe.scroll_button_cdocs.position.h/2 - AutoScreenAdjust(0,jeu->hero.m_classe.scroll_button_cdocs.position.y).y)/(jeu->hero.m_classe.position_contenu_docs.h*19+jeu->hero.m_classe.scroll_button_cdocs.position.h)*(jeu->hero.m_max_defil_cdoc - jeu->hero.m_classe.position_contenu_docs.h + 1));


    if (jeu->hero.m_defil_cdoc>jeu->hero.m_max_defil_cdoc-jeu->hero.m_classe.position_contenu_docs.h)
        jeu->hero.m_defil_cdoc=jeu->hero.m_max_defil_cdoc-jeu->hero.m_classe.position_contenu_docs.h;
    if (jeu->hero.m_defil_cdoc<0)
        jeu->hero.m_defil_cdoc=0;
}


