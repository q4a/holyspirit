

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



#ifndef C_MAINMENU
#define C_MAINMENU

#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>

#include "contexte.h"

enum {E_PRINCIPAL, E_CONTINUER, E_NOUVEAU};

class c_MainMenu : public Contexte
{
public:
    c_MainMenu();
    void Utiliser(Jeu *jeu);
    void Reset(Jeu *jeu);

    bool m_save;
    bool m_reset;

private:
    float temps_ecoule;
    int no_ecran;
    int defilement_saves;
    int classe_choisie;

    std::vector <int> m_nom_classes;
    std::vector <std::string> m_chemin_saves;
    std::vector <sf::Sprite> m_images_saves;
    std::string nom_hero;

    sf::Text texte;

    sf::Sprite m_mainscreen;
    Light_Entity m_light;

    float time;
};

#endif

