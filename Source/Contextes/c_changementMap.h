

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

//! Classe contexte qui s'occupe de g�rer l'�cran de chargement
/**
c_Chargement est un contexte qui va s'occuper d'interchanger la map pr�c�dente avec la map suivante. Elle s'occupe de faire le fondu vers
le noir du chargement.
*/



#ifndef C_CHARGEMENT
#define C_CHARGEMENT

#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>

#include "contexte.h"

class c_Chargement : public Contexte
{
public:

    //! Constructeur.
    /**
    Il ne fait pas grand chose � part initialiser les membres de la classe.
    */
    c_Chargement(Jeu *jeu);

    //! Permet d'utiliser le contexte.
    /**
    C'est le coeur du contexte. C'est dans cette m�thode qu'il fait tout ce qu'il a � faire.
    */
    void Utiliser(Jeu *jeu);

    //! Permet de dire quelle sera la prochaine map � charger.
    /**
    Cette m�thode va r�initialiser certains membres et va aller charger l'en-t�te de la prochaine map pour r�cup�rer son nom et le fond de chargement utilis�.
    \param prochaineMap : chemin relatif de la prochaine map � charger.
    \param coordonneePerso : nouvelle position du h�ro.
    \param debut : ce bool permet juste de dire si c'est la premi�re map du jeu, la map d'initialisation car celle-ci ne doit pas �tre sauvegard�e.
    */
    void setC_Chargement(std::string prochaineMap,coordonnee coordonneePerso,bool debut=false);

private:

    std::string nomMap,m_nomProchaineMap;


    float tempsActuel,tempsPrecedent,temps_ecoule,tempsEcouleDepuisDernierAffichage;
    float z;
    bool augmenterNoir,allerVersImageChargement,m_debut;
    coordonnee m_coordonneePerso;
    int m_fond;
    float m_tempsChargement;

};

#endif



