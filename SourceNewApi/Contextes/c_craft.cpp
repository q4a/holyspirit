

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

int GestionBoutons(Jeu *jeu, bool = false, bool = true, bool = true);
void GestionRaccourcis(Jeu *jeu);

inline sf::Vector2f AutoScreenAdjust(float x, float y, float decalage = 0)
{
    sf::Vector2f temp;
    temp.x = x + (configuration->Resolution.x - 800) * 0.5;
    temp.y = y + (configuration->Resolution.y - 600) - decalage * configuration->Resolution.h/600;
    return temp;
}
c_Craft::c_Craft()
{
    m_decalage=-600;
}
c_Craft::~c_Craft()
{
}

void c_Craft::Utiliser(Jeu *jeu)
{
    temps_ecoule=jeu->Clock.GetElapsedTime()*0.001;
    jeu->m_display=true;
    jeu->Clock.Reset();

    jeu->map->GererAmbiance(temps_ecoule);

    //jeu->map->Animer(&jeu->hero,0);
    jeu->map->Afficher(&jeu->hero,jeu->m_personnageClients,0,jeu->m_jeu->alpha_map);
    jeu->hero.AfficherAmisEtCraft();

    if (m_afficher)
        m_decalage+=temps_ecoule*2000;
    else
        m_decalage-=temps_ecoule*2000;

    if (m_decalage>0)
        m_decalage=0;
    if (m_decalage<-600)
    {
        for(unsigned i = 0 ; i < m_trader.size() ; ++i)
        {
            m_trader[i].setPosition(jeu->hero.m_personnage.getCoordonnee().x,
                                    jeu->hero.m_personnage.getCoordonnee().y);
            jeu->map->AjouterObjet(m_trader[i]);
        }
        m_trader.clear();
        m_decalage=-600;
        jeu->Next();
        m_afficher = 1;
    }

    int temp = GestionBoutons(jeu);
    if(temp >= 0)
    {
        jeu->next_screen = temp;
        jeu->hero.m_defilement_trader=0;
        jeu->hero.m_max_defilement_trader=0;
        m_afficher=0;
        jeu->Clock.Reset();
        eventManager->StopEvenement(Keyboard::I,EventKey);

        if (jeu->hero.m_objetEnMain>=0)
        {
            jeu->hero.m_objetADeposer=jeu->hero.m_objetEnMain;
            jeu->map->AjouterObjet(jeu->hero.DeposerObjet());
        }

        if(jeu->next_screen == 2 || jeu->next_screen == 4)
            jeu->next_screen = 3;
    }

    jeu->menu.AfficherInventaire(m_decalage,&jeu->hero.m_classe,false);
    jeu->menu.AfficherCraft(m_decalage,&jeu->hero.m_classe);

    jeu->hero.AfficherInventaire(m_decalage,&m_trader,true,false,true);

    if(jeu->hero.m_objetEnMain == -1)
        eventManager->AfficherCurseur();

    jeu->map->AfficherSac(jeu->hero.m_personnage.getCoordonnee(),m_decalage,jeu->hero.m_classe.position_sac_inventaire,jeu->hero.m_caracteristiques, jeu->hero.m_cheminClasse, jeu->hero.m_classe.border);

    jeu->menu.AfficherHUD(&jeu->hero.m_classe);
    jeu->menu.AfficherDynamique(jeu->hero.m_caracteristiques,-1,jeu->hero.m_caracteristiques,&jeu->hero.m_classe);

    jeu->hero.GererCraft(&m_trader);

    if (eventManager->getEvenement(Mouse::Left,EventClic))
    {
        if (jeu->hero.m_objetEnMain==-1&&jeu->map->getObjetPointe()==-1&&eventManager->getPositionSouris().x>jeu->hero.m_classe.position_sac_inventaire.x*configuration->Resolution.x/800&&eventManager->getPositionSouris().x<(jeu->hero.m_classe.position_sac_inventaire.x+jeu->hero.m_classe.position_sac_inventaire.w)*configuration->Resolution.x/800&&eventManager->getPositionSouris().y>jeu->hero.m_classe.position_sac_inventaire.y*configuration->Resolution.y/600&&eventManager->getPositionSouris().y<jeu->hero.m_classe.position_sac_inventaire.y*configuration->Resolution.y/600+20*configuration->Resolution.x/800)
            jeu->map->m_defilerObjets--,eventManager->StopEvenement(Mouse::Left,EventClic);

        if (jeu->hero.m_objetEnMain==-1&&jeu->map->getObjetPointe()==-1&&eventManager->getPositionSouris().x>jeu->hero.m_classe.position_sac_inventaire.x*configuration->Resolution.x/800&&eventManager->getPositionSouris().x<(jeu->hero.m_classe.position_sac_inventaire.x+jeu->hero.m_classe.position_sac_inventaire.w)*configuration->Resolution.x/800&&eventManager->getPositionSouris().y>jeu->hero.m_classe.position_sac_inventaire.y*configuration->Resolution.y/600+(jeu->hero.m_classe.position_sac_inventaire.h-1)*20*configuration->Resolution.x/800&&eventManager->getPositionSouris().y<jeu->hero.m_classe.position_sac_inventaire.y*configuration->Resolution.y/600+jeu->hero.m_classe.position_sac_inventaire.h*20*configuration->Resolution.x/800)
            jeu->map->m_defilerObjets++,eventManager->StopEvenement(Mouse::Left,EventClic);
    }

    if (eventManager->getEvenement(Mouse::Left,EventClic))
    {
        if (jeu->map->RamasserObjet(&jeu->hero,1))
            eventManager->StopEvenement(Mouse::Left,EventClic);
    }

    if (eventManager->getEvenement(Mouse::Left,EventClic)
     && !jeu->hero.m_classe.sort_inventory.m_hover)
    {
        if (jeu->hero.PrendreEnMain(&m_trader, true))
            if (jeu->hero.m_objetADeposer>=0)
                jeu->map->AjouterObjet(jeu->hero.DeposerObjet());
        eventManager->StopEvenement(Mouse::Left,EventClic);
    }



     jeu->hero.m_defilement_trader -= eventManager->getMolette();

    if(eventManager->getEvenement(Mouse::Left,EventClicA))
    if(jeu->hero.m_max_defilement_trader-jeu->hero.m_classe.position_contenu_marchand.h != 0)
      if (eventManager->getPositionSouris().x > AutoScreenAdjust(jeu->hero.m_classe.scroll_button.position.x,0).x
        &&eventManager->getPositionSouris().x < AutoScreenAdjust(jeu->hero.m_classe.scroll_button.position.x+jeu->hero.m_classe.scroll_button.position.w,0).x
        &&eventManager->getPositionSouris().y > AutoScreenAdjust(0,jeu->hero.m_classe.scroll_button.position.y).y
        &&eventManager->getPositionSouris().y < AutoScreenAdjust(0,jeu->hero.m_classe.scroll_button.position.y+jeu->hero.m_classe.position_contenu_marchand.h*32).y)
            jeu->hero.m_defilement_trader = (int)((((float)eventManager->getPositionSouris().y-jeu->hero.m_classe.scroll_button.position.h*0.5-(float)AutoScreenAdjust(0,jeu->hero.m_classe.scroll_button.position.y).y)/((float)jeu->hero.m_classe.position_contenu_marchand.h*28))*(float)(jeu->hero.m_max_defilement_trader-jeu->hero.m_classe.position_contenu_marchand.h));

    if (jeu->hero.m_defilement_trader<0)
        jeu->hero.m_defilement_trader=0;
    if (jeu->hero.m_defilement_trader>jeu->hero.m_max_defilement_trader-jeu->hero.m_classe.position_contenu_marchand.h)
        jeu->hero.m_defilement_trader=jeu->hero.m_max_defilement_trader-jeu->hero.m_classe.position_contenu_marchand.h;

   // GestionRaccourcis(jeu);
}
