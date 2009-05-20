

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



#ifndef JEUH
#define JEUH


#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>

#include "constantes.h"
#include "Map/map.h"
#include "Moteurs/eventManager.h"
#include "menu.h"

#include "Contextes/c_jeu.h"
#include "Contextes/c_changementMap.h"
#include "Contextes/c_inventaire.h"
#include "Contextes/c_menuInGame.h"
#include "Contextes/c_demarrage.h"

class Jeu
{
    public:

    Jeu();
    void Demarrer();
    void Reset();

    Menu menu;
    EventManager eventManager;
    Map *map;
    Hero hero;
    sf::Clock Clock;
    sf::SoundBuffer bufferSonMort;
	sf::Sound  sonMort;

	bool m_run,m_display,m_reset;

	c_Demarrage *m_demarrage;
    c_Jeu *m_jeu;
    c_Chargement *m_chargement;
    c_Inventaire *m_inventaire;
    c_MenuInGame *m_menuInGame;
    Contexte *m_contexte;

    private:
};

#endif



