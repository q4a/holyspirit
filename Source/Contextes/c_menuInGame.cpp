

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

c_MenuInGame::c_MenuInGame()
{
    m_alpha=0;
    retour=false;
}

void c_MenuInGame::Utiliser(Jeu *jeu)
{
    moteurGraphique->m_blur=0.0025*m_alpha/255;

    jeu->m_display=true;
    jeu->hero.PlacerCamera(jeu->map->getDimensions());

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
            m_alpha=0,retour=false,jeu->m_contexte=jeu->m_jeu,moteurGraphique->m_blur=0;

    }
    else
    {
        if (m_alpha<255)
            m_alpha+=temps_ecoule*500;
        if (m_alpha>255)
            m_alpha=255;
    }

    jeu->eventManager.GererLesEvenements(&jeu->m_run,temps_ecoule,jeu->map->getDimensions());

    if (jeu->eventManager.getEvenement(Key::Escape,"ET")&&m_alpha==255)
        jeu->eventManager.StopEvenement(Key::I,"ET"),retour=1;
    if (jeu->eventManager.getEvenement(Key::Space,"ET"))
        jeu->m_run=false;

    coordonnee temp;

    jeu->map->Afficher(&jeu->hero,temp,0);
    if (configuration->Minimap)
        jeu->menu.Afficher(2,255,&jeu->hero.m_classe);
    if (jeu->hero.getChercherSac().x!=-1&&jeu->map->getNombreObjets(jeu->hero.getChercherSac())>0)
    {
        coordonnee temp={600,(int)((float)configuration->Resolution.w*0.265),200,10};
        jeu->menu.Afficher(3,255,&jeu->hero.m_classe);
        jeu->map->AfficherSac(jeu->hero.getChercherSac(),0,jeu->eventManager.getPositionSouris(),temp,jeu->hero.m_caracteristiques);
    }
    jeu->menu.Afficher(1,255,&jeu->hero.m_classe);
    jeu->menu.AfficherDynamique(jeu->hero.m_caracteristiques,0,jeu->hero.m_caracteristiques,&jeu->hero.m_classe);


    texte.SetSize(56.f*configuration->Resolution.h/600);
    texte.SetText(configuration->text_menus[0]);

    texte.SetY(configuration->Resolution.h/2-texte.GetRect().GetHeight());
    texte.SetX(configuration->Resolution.w/2-texte.GetRect().GetWidth()/2);
    if (jeu->eventManager.getPositionSouris().y>configuration->Resolution.y/2-texte.GetRect().GetHeight()&&jeu->eventManager.getPositionSouris().y<configuration->Resolution.y/2)
    {
        texte.SetColor(Color(100,50,0,(int)m_alpha));
        if (jeu->eventManager.getEvenement(Mouse::Left,"C"))
            retour=1;

    }
    else
        texte.SetColor(Color(150,100,50,(int)m_alpha));
    moteurGraphique->AjouterTexte(&texte,19,1);

    texte.SetText(configuration->text_menus[1]);

    texte.SetY(configuration->Resolution.h/2);
    texte.SetX(configuration->Resolution.w/2-texte.GetRect().GetWidth()/2);
    if (jeu->eventManager.getPositionSouris().y>configuration->Resolution.y/2&&jeu->eventManager.getPositionSouris().y<configuration->Resolution.y/2+texte.GetRect().GetHeight())
    {
        texte.SetColor(Color(100,50,0,(int)m_alpha));
        if (jeu->eventManager.getEvenement(Mouse::Left,"C"))
            jeu->m_run=false;
    }
    else
        texte.SetColor(Color(150,100,50,(int)m_alpha));
    moteurGraphique->AjouterTexte(&texte,19,1);

    texte.SetText(configuration->text_menus[2]);

    texte.SetY(configuration->Resolution.h/2+texte.GetRect().GetHeight());
    texte.SetX(configuration->Resolution.w/2-texte.GetRect().GetWidth()/2);
    if (jeu->eventManager.getPositionSouris().y>configuration->Resolution.y/2+texte.GetRect().GetHeight()&&jeu->eventManager.getPositionSouris().y<configuration->Resolution.y/2+texte.GetRect().GetHeight()*2)
    {
        texte.SetColor(Color(100,50,0,(int)m_alpha));
        if (jeu->eventManager.getEvenement(Mouse::Left,"C"))
            jeu->m_reset=true,jeu->m_run=false;
    }
    else
        texte.SetColor(Color(150,100,50,(int)m_alpha));
    moteurGraphique->AjouterTexte(&texte,19,1);

    jeu->eventManager.AfficherCurseur();
}
