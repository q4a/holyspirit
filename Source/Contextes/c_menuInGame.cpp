

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

int GestionBoutons(Jeu *jeu);

c_MenuInGame::c_MenuInGame()
{
    m_alpha=0;
    retour=false;
}

void c_MenuInGame::Utiliser(Jeu *jeu)
{
    moteurGraphique->m_blur=0.005*m_alpha/255;

    jeu->m_display=true;

    if (configuration->Lumiere)
        jeu->map->CalculerOmbresEtLumieres();

    temps_ecoule=0;
    temps_ecoule=jeu->Clock.GetElapsedTime();
    jeu->Clock.Reset();

    if (retour)
    {
        if (m_alpha>0)
            m_alpha-=temps_ecoule*1000;
        if (m_alpha<0)
            m_alpha=0,retour=false,jeu->Next(),moteurGraphique->m_blur=0;
    }
    else
    {
        if (m_alpha<255)
            m_alpha+=temps_ecoule*500;
        if (m_alpha>255)
            m_alpha=255;
    }

   // if (eventManager->getEvenement(Key::Escape,"ET")&&m_alpha==255)
        //eventManager->StopEvenement(Key::I,"ET"),retour=1;

 //   if (eventManager->getEvenement(Key::Space,"ET"))
     //   jeu->m_run=false;

    int temp = GestionBoutons(jeu);
    if(temp >= 0)
    {
        jeu->next_screen = temp;
        retour=1;
        jeu->Clock.Reset();

        if(jeu->next_screen == 4)
            jeu->next_screen = 3;
    }

    jeu->map->Afficher(&jeu->hero,0,jeu->m_jeu->alpha_map);
    jeu->menu.Afficher(2,jeu->m_jeu->alpha_map,&jeu->hero.m_classe);

    if (jeu->hero.getChercherSac().x!=-1&&jeu->map->getNombreObjets(jeu->hero.getChercherSac())>0)
    {
        jeu->menu.Afficher(3,255,&jeu->hero.m_classe);
        jeu->map->AfficherSac(jeu->hero.getChercherSac(),0,
                              coordonnee (600,(int)((float)configuration->Resolution.w*0.265),200,10),
                              jeu->hero.m_caracteristiques);
    }
    jeu->menu.Afficher(1,255,&jeu->hero.m_classe);
    jeu->menu.AfficherDynamique(jeu->hero.m_caracteristiques,0,jeu->hero.m_caracteristiques,&jeu->hero.m_classe);


    texte.SetSize(56.f*configuration->Resolution.h/600);

    texte.SetText(configuration->getText(0,0));
    texte.SetY(configuration->Resolution.h/2-texte.GetRect().GetSize().y);
    texte.SetX(configuration->Resolution.w/2-texte.GetRect().GetSize().x/2);
    if (eventManager->getPositionSouris().y>configuration->Resolution.y/2-texte.GetRect().GetSize().y&&eventManager->getPositionSouris().y<configuration->Resolution.y/2)
    {
        texte.SetColor(Color(100,50,0,(int)m_alpha));
        if (eventManager->getEvenement(Mouse::Left,"C"))
            retour=1, jeu->next_screen = 3;

    }
    else
        texte.SetColor(Color(150,100,50,(int)m_alpha));
    moteurGraphique->AjouterTexte(&texte,19,1);

    texte.SetText(configuration->getText(0,1));
    texte.SetY(configuration->Resolution.h/2);
    texte.SetX(configuration->Resolution.w/2-texte.GetRect().GetSize().x/2);
    if (eventManager->getPositionSouris().y>configuration->Resolution.y/2&&eventManager->getPositionSouris().y<configuration->Resolution.y/2+texte.GetRect().GetSize().y)
    {
        texte.SetColor(Color(100,50,0,(int)m_alpha));
        if (eventManager->getEvenement(Mouse::Left,"C"))
            jeu->m_run=false;
    }
    else
        texte.SetColor(Color(150,100,50,(int)m_alpha));
    moteurGraphique->AjouterTexte(&texte,19,1);

    texte.SetText(configuration->getText(0,2));
    texte.SetY(configuration->Resolution.h/2+texte.GetRect().GetSize().y);
    texte.SetX(configuration->Resolution.w/2-texte.GetRect().GetSize().x/2);
    if (eventManager->getPositionSouris().y>configuration->Resolution.y/2+texte.GetRect().GetSize().y&&eventManager->getPositionSouris().y<configuration->Resolution.y/2+texte.GetRect().GetSize().y*2)
    {
        texte.SetColor(Color(100,50,0,(int)m_alpha));
        if (eventManager->getEvenement(Mouse::Left,"C"))
            jeu->m_reset=true,jeu->m_run=false;
    }
    else
        texte.SetColor(Color(150,100,50,(int)m_alpha));
    moteurGraphique->AjouterTexte(&texte,19,1);

    eventManager->AfficherCurseur();
}
