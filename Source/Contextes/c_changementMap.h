

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





#ifndef C_CHARGEMENT
#define C_CHARGEMENT

#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>

#include "contexte.h"

//! Classe contexte qui s'occupe de gérer l'écran de chargement
/**
c_Chargement est un contexte qui va s'occuper d'interchanger la map précédente avec la map suivante. Elle s'occupe de faire le fondu vers
le noir du chargement.
*/
class c_Chargement : public Contexte
{
public:

    //! Constructeur.
    /**
    Le constructeur de cette classe est obsolète. ^^'
    */
    c_Chargement();
    virtual ~c_Chargement();

    //! Utilise le contexte.
    /**
    C'est le coeur du contexte. C'est dans cette méthode qu'il fait tout ce qu'il a à faire.
    \param jeu : class qui contient le jeu, donc le héro, la map en cours, ...
    */
    void Utiliser(Jeu *jeu);

    //! Permet de dire quelle sera la prochaine map à charger suivant le paramètre prochaineMap.
    /**
    Cette méthode va réinitialiser certains membres et va aller charger l'en-tête de la prochaine map pour récupérer son nom et le fond de chargement utilisé.
    \param prochaineMap : chemin relatif de la prochaine map à charger.
    \param coordonneePerso : nouvelle position du héro.
    \param debut : ce bool permet juste de dire si c'est la première map du jeu, la map d'initialisation car celle-ci ne doit pas être sauvegardée.
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
    bool mort;

};

#endif



