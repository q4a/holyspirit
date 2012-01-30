

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

c_MenuInGame::c_MenuInGame()
{
    m_alpha     = 0;
    retour      = false;
    options     = false;
}
c_MenuInGame::~c_MenuInGame()
{
}

void c_MenuInGame::Utiliser(Jeu *jeu)
{
    moteurGraphique->m_blur=0.0025*m_alpha/255;
    configuration->effetNoir = 1 - m_alpha/255*0.75;

    jeu->m_display=true;
    temps_ecoule=jeu->Clock.GetElapsedTime()*0.001;
    jeu->Clock.Reset();

    if (configuration->Lumiere)
        jeu->map->CalculerOmbresEtLumieres();


    jeu->map->GererAmbiance(temps_ecoule);

    if (retour)
    {
        m_alpha-=temps_ecoule*1000;
        if (m_alpha<0)
            m_alpha=0,retour=false,jeu->Next(),moteurGraphique->m_blur=0;
    }
    else
    {
        m_alpha+=temps_ecoule*500;
        if (m_alpha>255)
            m_alpha=255;
    }


    int temp = GestionBoutons(jeu);
    if(temp >= 0)
    {
        jeu->next_screen = temp;
        retour=1;
        jeu->Clock.Reset();

        if(jeu->next_screen == 4)
            jeu->next_screen = 3;
    }

    moteurGraphique->Gerer(0);

    //jeu->map->Animer(&jeu->hero,0);
    jeu->map->Afficher(jeu->m_listHeroes,0,jeu->m_jeu->alpha_map);
    jeu->hero.AfficherAmisEtCraft();
    jeu->menu.AfficherHUD(&jeu->hero.m_classe);
    jeu->menu.AfficherDynamique(jeu->hero.m_caracteristiques,0,jeu->hero.m_caracteristiques,&jeu->hero.m_classe);

    if(!options)
    {
        texte.SetCharacterSize(56);


        buttons_principal[0].no_opacity = true;
        buttons_principal[0].position.w = moteurGraphique->special_typo.getSize(configuration->getText(0,0), 72);
        buttons_principal[0].position.h = 64;
        buttons_principal[0].position.y = configuration->Resolution.h/2 - 140;
        buttons_principal[0].position.x = configuration->Resolution.w/2-buttons_principal[0].position.w/2;
        buttons_principal[0].Afficher(0);

        if(buttons_principal[0].m_press)
            moteurGraphique->special_typo_p.Draw(configuration->getText(0,0), sf::Vector2f(configuration->Resolution.w/2,
                                                                                            buttons_principal[0].position.y), 72, 19, true, sf::Color(255,255,255,(int)m_alpha));
        else if(buttons_principal[0].m_hover)
            moteurGraphique->special_typo_h.Draw(configuration->getText(0,0), sf::Vector2f(configuration->Resolution.w/2,
                                                                                            buttons_principal[0].position.y), 72, 19, true, sf::Color(255,255,255,(int)m_alpha));
        else
            moteurGraphique->special_typo.Draw(configuration->getText(0,0), sf::Vector2f(configuration->Resolution.w/2,
                                                                                          buttons_principal[0].position.y), 72, 19, true, sf::Color(255,255,255,(int)m_alpha));

        if(buttons_principal[0].m_action)
        {
            buttons_principal[0].m_action = false;
            retour=1;
            jeu->next_screen = 3;
        }



        buttons_principal[1].no_opacity = true;
        buttons_principal[1].position.w = moteurGraphique->special_typo.getSize(configuration->getText(0,55), 72);
        buttons_principal[1].position.h = 64;
        buttons_principal[1].position.y = configuration->Resolution.h/2 - 70;
        buttons_principal[1].position.x = configuration->Resolution.w/2-buttons_principal[1].position.w/2;
        buttons_principal[1].Afficher(0);

        if(buttons_principal[1].m_press)
            moteurGraphique->special_typo_p.Draw(configuration->getText(0,55), sf::Vector2f(configuration->Resolution.w/2,
                                                                                            buttons_principal[1].position.y), 72, 19, true, sf::Color(255,255,255,(int)m_alpha));
        else if(buttons_principal[1].m_hover)
            moteurGraphique->special_typo_h.Draw(configuration->getText(0,55), sf::Vector2f(configuration->Resolution.w/2,
                                                                                            buttons_principal[1].position.y), 72, 19, true, sf::Color(255,255,255,(int)m_alpha));
        else
            moteurGraphique->special_typo.Draw(configuration->getText(0,55), sf::Vector2f(configuration->Resolution.w/2,
                                                                                          buttons_principal[1].position.y), 72, 19, true, sf::Color(255,255,255,(int)m_alpha));

        if(buttons_principal[1].m_action)
        {
            buttons_principal[1].m_action = false;
            options = true;
            configuration->no_menu_option = 0;
        }


        buttons_principal[2].no_opacity = true;
        buttons_principal[2].position.w = moteurGraphique->special_typo.getSize(configuration->getText(0,1), 72);
        buttons_principal[2].position.h = 64;
        buttons_principal[2].position.y = configuration->Resolution.h/2;
        buttons_principal[2].position.x = configuration->Resolution.w/2-buttons_principal[2].position.w/2;
        buttons_principal[2].Afficher(0);

        if(buttons_principal[2].m_press)
            moteurGraphique->special_typo_p.Draw(configuration->getText(0,1), sf::Vector2f(configuration->Resolution.w/2,
                                                                                            buttons_principal[2].position.y), 72, 19, true, sf::Color(255,255,255,(int)m_alpha));
        else if(buttons_principal[2].m_hover)
            moteurGraphique->special_typo_h.Draw(configuration->getText(0,1), sf::Vector2f(configuration->Resolution.w/2,
                                                                                            buttons_principal[2].position.y), 72, 19, true, sf::Color(255,255,255,(int)m_alpha));
        else
            moteurGraphique->special_typo.Draw(configuration->getText(0,1), sf::Vector2f(configuration->Resolution.w/2,
                                                                                          buttons_principal[2].position.y), 72, 19, true, sf::Color(255,255,255,(int)m_alpha));

        if(buttons_principal[2].m_action)
        {
            buttons_principal[2].m_action = false;
            jeu->m_mainMenu->m_save=true;
            jeu->m_mainMenu->m_reset=false;
            jeu->m_contexte = jeu->m_mainMenu;
            net->Disconnect();
            net->CloseServer();
        }


        buttons_principal[3].no_opacity = true;
        buttons_principal[3].position.w = moteurGraphique->special_typo.getSize(configuration->getText(0,2), 72);
        buttons_principal[3].position.h = 64;
        buttons_principal[3].position.y = configuration->Resolution.h/2+70;
        buttons_principal[3].position.x = configuration->Resolution.w/2-buttons_principal[3].position.w/2;
        buttons_principal[3].Afficher(0);

        if(buttons_principal[3].m_press)
            moteurGraphique->special_typo_p.Draw(configuration->getText(0,2), sf::Vector2f(configuration->Resolution.w/2,
                                                                                            buttons_principal[3].position.y), 72, 19, true, sf::Color(255,255,255,(int)m_alpha));
        else if(buttons_principal[3].m_hover)
            moteurGraphique->special_typo_h.Draw(configuration->getText(0,2), sf::Vector2f(configuration->Resolution.w/2,
                                                                                            buttons_principal[3].position.y), 72, 19, true, sf::Color(255,255,255,(int)m_alpha));
        else
            moteurGraphique->special_typo.Draw(configuration->getText(0,2), sf::Vector2f(configuration->Resolution.w/2,
                                                                                          buttons_principal[3].position.y), 72, 19, true, sf::Color(255,255,255,(int)m_alpha));

        if(buttons_principal[3].m_action)
        {
            buttons_principal[3].m_action = false;
            jeu->m_mainMenu->m_save=true;
            jeu->m_mainMenu->m_reset=true;
            jeu->m_contexte = jeu->m_mainMenu;
            net->Disconnect();
            net->CloseServer();
        }


    }
    else if(configuration->Options())
        options = false;



    eventManager->AfficherCurseur();
}
