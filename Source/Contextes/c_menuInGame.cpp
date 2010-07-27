

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

c_MenuInGame::c_MenuInGame()
{
    m_alpha=0;
    retour=false;
}

void c_MenuInGame::Utiliser(Jeu *jeu)
{
    moteurGraphique->m_blur=0.0025*m_alpha/255;
    configuration->effetNoir = 1 - m_alpha/255*0.5;

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

   // if (eventManager->getEvenement(Key::Escape,EventKey)&&m_alpha==255)
        //eventManager->StopEvenement(Key::I,EventKey),retour=1;

 //   if (eventManager->getEvenement(Key::Space,EventKey))
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

    moteurGraphique->Gerer(0,jeu->map->getDimensions().y);

    jeu->map->Animer(&jeu->hero,0,&jeu->menu);
    jeu->map->Afficher(&jeu->hero,0,jeu->m_jeu->alpha_map);
    jeu->hero.AfficherAmisEtCraft();
  //  jeu->menu.Afficher(2,jeu->m_jeu->alpha_map,&jeu->hero.m_classe);

   /* if (jeu->hero.getChercherSac().x!=-1&&jeu->map->getNombreObjets(jeu->hero.getChercherSac())>0)
    {
        jeu->menu.Afficher(3,255,&jeu->hero.m_classe);
        jeu->map->AfficherSac(jeu->hero.getChercherSac(),0,
                              coordonnee (600,(int)((float)configuration->Resolution.w*0.265),200,10),
                              jeu->hero.m_caracteristiques);
    }*/
    jeu->menu.AfficherHUD(&jeu->hero.m_classe);
    jeu->menu.AfficherDynamique(jeu->hero.m_caracteristiques,0,jeu->hero.m_caracteristiques,&jeu->hero.m_classe);


    texte.SetCharacterSize(56);

    texte.SetString(configuration->getText(0,0));
    texte.SetY(configuration->Resolution.h/2-texte.GetRect().Height);
    texte.SetX(configuration->Resolution.w/2-texte.GetRect().Width/2);
    if (eventManager->getPositionSouris().y>configuration->Resolution.y/2-texte.GetRect().Height&&eventManager->getPositionSouris().y<configuration->Resolution.y/2)
    {
        texte.SetColor(Color(100,50,0,(int)m_alpha));
        if (eventManager->getEvenement(Mouse::Left,EventClic))
            retour=1, jeu->next_screen = 3;

    }
    else
        texte.SetColor(Color(150,100,50,(int)m_alpha));
    moteurGraphique->AjouterTexte(&texte,19,1);

    texte.SetString(configuration->getText(0,1));
    texte.SetY(configuration->Resolution.h/2);
    texte.SetX(configuration->Resolution.w/2-texte.GetRect().Width/2);
    if (eventManager->getPositionSouris().y>configuration->Resolution.y/2&&eventManager->getPositionSouris().y<configuration->Resolution.y/2+texte.GetRect().Height)
    {
        texte.SetColor(Color(100,50,0,(int)m_alpha));
        if (eventManager->getEvenement(Mouse::Left,EventClic))
            jeu->m_mainMenu->m_save=true, jeu->m_contexte = jeu->m_mainMenu;
    }
    else
        texte.SetColor(Color(150,100,50,(int)m_alpha));
    moteurGraphique->AjouterTexte(&texte,19,1);

    texte.SetString(configuration->getText(0,2));
    texte.SetY(configuration->Resolution.h/2+texte.GetRect().Height);
    texte.SetX(configuration->Resolution.w/2-texte.GetRect().Width/2);
    if (eventManager->getPositionSouris().y>configuration->Resolution.y/2+texte.GetRect().Height&&eventManager->getPositionSouris().y<configuration->Resolution.y/2+texte.GetRect().Height*2)
    {
        texte.SetColor(Color(100,50,0,(int)m_alpha));
        if (eventManager->getEvenement(Mouse::Left,EventClic))
            jeu->m_mainMenu->m_reset=true,jeu->m_mainMenu->m_save=true, jeu->m_contexte = jeu->m_mainMenu;
    }
    else
        texte.SetColor(Color(150,100,50,(int)m_alpha));
    moteurGraphique->AjouterTexte(&texte,19,1);

    eventManager->AfficherCurseur();
}
