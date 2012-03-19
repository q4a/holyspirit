

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
#include "c_potales.h"

#include "../globale.h"

#include <iostream>
#include <fstream>

#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

using namespace sf;
using namespace std;

int GestionBoutons(Jeu *jeu, bool = false, bool = false, bool = false);

c_Potales::c_Potales()
{
    m_decalage=-600;
}
c_Potales::~c_Potales()
{
}

void c_Potales::Utiliser(Jeu *jeu)
{
    int temp = GestionBoutons(jeu);

    temps_ecoule=jeu->Clock.getElapsedTime().asSeconds();
    jeu->m_display=true;
    jeu->Clock.restart();

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

    jeu->menu.AfficherQuetes(m_decalage,&jeu->hero.m_classe);

    jeu->hero.AfficherPotales(m_decalage,jeu->map->getNom());

    if(jeu->hero.m_potale_selectionnee >= 0)
    {
        string nomMap=jeu->hero.m_potales[jeu->hero.m_potale_selectionnee].chemin;

        console->Ajouter("",0);
        console->Ajouter("---------------------------------------------------------------------------------",0);
        console->Ajouter("EVENEMENT : Changement de map",0);
        console->Ajouter("---------------------------------------------------------------------------------",0);

        coordonnee coordonneePerso;
        coordonneePerso.x=jeu->hero.m_potales[jeu->hero.m_potale_selectionnee].position.x;
        coordonneePerso.y=jeu->hero.m_potales[jeu->hero.m_potale_selectionnee].position.y;

        sf::Clock Clock;
        Clock.restart();

        jeu->m_chargement->setC_Chargement(nomMap,coordonneePerso);
        net->SendChangeMap(nomMap,coordonneePerso);
        temp = 8;
    }

    jeu->menu.AfficherHUD(&jeu->hero.m_classe);
    jeu->menu.AfficherDynamique(jeu->hero.m_caracteristiques,-1,jeu->hero.m_caracteristiques,&jeu->hero.m_classe);
    jeu->map->GererAmbiance(temps_ecoule);

    eventManager->AfficherCurseur();


    if(temp >= 0)
    {
        jeu->next_screen = temp;
        m_afficher=0;
        jeu->Clock.restart();

        if(jeu->next_screen == 6 || jeu->next_screen == 4)
            jeu->next_screen = 3;
    }
}

