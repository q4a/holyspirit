

/*Copyright (C) 2009 Naisse Gr�goire

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



#ifndef C_DEMARRAGE
#define C_DEMARRAGE

#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>

#include "contexte.h"

//! Contexte qui g�re l'affichage du logo d'Alpha-Arts avec une fondu vers le noir
/**
Ce contexte s'occupe d'afficher le beau logo AA au d�marrage du jeu.
m_alpha : Le taux vers le noir ( 0 = tout noir )
m_image : ID de l'image du logo d'AA, � envoyer au moteur graphique poour recevoir l'image en retour
m_augmenter : boolean qui permet de dire si on doit augmenter ou diminuer m_alpha
*/
class c_Demarrage : public Contexte
{
public:
    //! Constructeur
    /**
    Met m_alpha � 0, m_augmenter � true et charge l'image du logo
    */
    c_Demarrage();

    //! Utilise le contexte.
    /**
    C'est le coeur du contexte. C'est dans cette m�thode qu'il fait tout ce qu'il a � faire.
    Afficher le logo
    In/decrementer m_alpha
    Redirige vers le contexte m_chargement quand il a fini, afin de charger la premi�re map
    \param jeu : class qui contient le jeu, donc le h�ro, la map en cours, ...
    */
    void Utiliser(Jeu *jeu);

private:
    float temps_ecoule,m_alpha;
    int m_image;
    bool m_augmenter;
};

#endif

