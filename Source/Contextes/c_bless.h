

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




#ifndef C_BLESS
#define C_BLESS

#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>

#include "contexte.h"

//! Contexte qui gère l'affichage et la gestion de l'inventaire
/**
Ce contexte s'occupe d'afficher le beau logo AA au démarrage du jeu.
m_decalage : nombre de pixel de decalage de l'écran inventaire, per
met de faire défiler l'inventaire vers le haut quand il apparait
             et vers le bas quand il disparait.
m_afficher : boolean qui permet de dire si on doit faire défiler vers le haut ou vers le bas l'écran d'inventaire
m_trader : Tableau dynamique d'objets, c'est l'inventaire du marchand. S'il ne vaut pas empty, c'est que le joueur à cliqué sur un marchand
           et qu'il faut donc afficher le menu marchand.
*/
class c_Bless : public Contexte
{
public:
    //! Constructeur
    /**
    Met m_decalage à -600
    */
    c_Bless();

    //! Utilise le contexte.
    /**
    C'est le coeur du contexte. C'est dans cette méthode qu'il fait tout ce qu'il a à faire.
    Afficher l'inventaire
    Afficher l'inventaire du marchand si m_trader ne vaut pas 0
    In/decrementer m_decalage
    Redirige vers le contexte m_jeu si on appuie sur i ou echappe.
    Gerer les objets dans l'inventaire
    \param jeu : class qui contient le jeu, donc le héro, la map en cours, ...
    */
    void Utiliser(Jeu *jeu);

private:
    float temps_ecoule,m_decalage;
    bool m_afficher;

    std::vector<Objet> m_trader;
    Objet m_schema;

};

#endif



