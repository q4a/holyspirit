


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

c_Miracles::c_Miracles()
{
    m_decalage = -600;
    m_fenetre = 0;
}

void c_Miracles::Utiliser(Jeu *jeu)
{
    if (m_decalage<=-600)
        m_afficher=1;

    temps_ecoule=jeu->Clock.GetElapsedTime();
    jeu->m_display=true;
    jeu->Clock.Reset();

    jeu->map->Afficher(&jeu->hero,0,jeu->m_jeu->alpha_map);
    jeu->menu.Afficher(2,jeu->m_jeu->alpha_map,&jeu->hero.m_classe);

    if (m_afficher)
        m_decalage+=temps_ecoule*2000;
    else
        m_decalage-=temps_ecoule*2000;

    if (m_decalage>0)
        m_decalage=0;
    if (m_decalage<-600)
    {
        m_decalage=-600;
        jeu->m_contexte=jeu->m_jeu,eventManager->StopEvenement(Key::Q,"ET");
    }

    jeu->menu.AfficherMiracles(m_decalage, &jeu->hero.m_classe, m_fenetre);

    eventManager->AfficherCurseur(jeu->hero.AfficherMiracles(m_decalage, m_fenetre));

    jeu->menu.Afficher(1,255,&jeu->hero.m_classe);
    jeu->menu.AfficherDynamique(jeu->hero.m_caracteristiques,-1,jeu->hero.m_caracteristiques,&jeu->hero.m_classe);

    if (eventManager->getEvenement(Key::T,"ET")||eventManager->getEvenement(Key::Escape,"ET"))
    {
        m_afficher=0;
        jeu->Clock.Reset();
        eventManager->StopEvenement(Key::T,"ET");
    }

    for(int i = 0;i < (int)jeu->hero.m_classe.boutons_miracles.size(); ++i)
        if(eventManager->getPositionSouris().x > jeu->hero.m_classe.boutons_miracles[i].position.x
         &&eventManager->getPositionSouris().x < jeu->hero.m_classe.boutons_miracles[i].position.x + jeu->hero.m_classe.boutons_miracles[i].position.w
         &&eventManager->getPositionSouris().y > jeu->hero.m_classe.boutons_miracles[i].position.y
         &&eventManager->getPositionSouris().y < jeu->hero.m_classe.boutons_miracles[i].position.y + jeu->hero.m_classe.boutons_miracles[i].position.h)
        {
            coordonnee buf = eventManager->getPositionSouris();
            buf.y -= 20;
            moteurGraphique->AjouterTexte(jeu->hero.m_classe.boutons_miracles[i].nom, buf, 17,0,14);

            if (eventManager->getEvenement(Mouse::Left,"C"))
            {
                m_fenetre = jeu->hero.m_classe.boutons_miracles[i].lien;
                eventManager->StopEvenement(Mouse::Left,"C");
            }
        }
}

