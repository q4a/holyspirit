

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

int GestionBoutons(Jeu *jeu, bool = false);

c_Quetes::c_Quetes()
{
    m_decalage=-600;
}
c_Quetes::~c_Quetes()
{
}

void c_Quetes::Utiliser(Jeu *jeu)
{
    if (m_decalage<=-600)
        m_afficher=1;

    temps_ecoule=jeu->Clock.GetElapsedTime();
    jeu->m_display=true;
    jeu->Clock.Reset();

    moteurGraphique->Gerer(0);
    jeu->map->Animer(&jeu->hero,0);
    jeu->map->Afficher(&jeu->hero,0,jeu->m_jeu->alpha_map);
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
    }

    int temp = GestionBoutons(jeu);

    jeu->menu.AfficherQuetes(m_decalage,&jeu->hero.m_classe);

    jeu->hero.AfficherQuetes(m_decalage);

    jeu->menu.AfficherHUD(&jeu->hero.m_classe);
    jeu->menu.AfficherDynamique(jeu->hero.m_caracteristiques,-1,jeu->hero.m_caracteristiques,&jeu->hero.m_classe);

    eventManager->AfficherCurseur();

    if(temp >= 0)
    {
        jeu->next_screen = temp;
        m_afficher=0;
        jeu->Clock.Reset();

        if(jeu->next_screen == 6 || jeu->next_screen == 4)
            jeu->next_screen = 3;
    }

    if (eventManager->getEvenement(Mouse::Left,EventClic))
    {
        eventManager->StopEvenement(Mouse::Left,EventClic);
        jeu->hero.m_queteSelectionnee = jeu->hero.m_quetePointee;
    }
}

