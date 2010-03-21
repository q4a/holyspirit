

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

c_Demarrage::c_Demarrage()
{
    m_alpha=0;
    m_augmenter=true;
    m_image=moteurGraphique->AjouterImage(configuration->chemin_aa);
}

void c_Demarrage::Utiliser(Jeu *jeu)
{
    temps_ecoule=jeu->Clock.GetElapsedTime();
    jeu->Clock.Reset();

    Sprite sprite;

    sprite.SetImage(*moteurGraphique->getImage(m_image));

    configuration->effetNoir=1;

    if (m_alpha<255)
        sprite.SetColor(Color(255,255,255,(int)m_alpha));
    else
        sprite.SetColor(Color(255,255,255,255));
    sprite.Resize(configuration->Resolution.x,configuration->Resolution.y);

    moteurGraphique->AjouterCommande(&sprite,15,0);

    if (m_augmenter)
        m_alpha+=temps_ecoule*200;
    else
        m_alpha-=temps_ecoule*200;

    if (m_alpha>384)
        m_augmenter=false;

    if (eventManager->getEvenement(Mouse::Left,EventClic))
        m_augmenter=false;

    if (m_alpha<0)
        jeu->m_contexte=jeu->m_mainMenu;

    jeu->m_display=true;
}
