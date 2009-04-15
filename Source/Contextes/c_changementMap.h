

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

class c_Chargement : public Contexte
{
    public:
    c_Chargement(Jeu *jeu);
    void Utiliser(Jeu *jeu);
    void setC_Chargement(int numeroMap,coordonnee coordonneePerso,bool debut=false);

    //sf::View *camera;

    private:

	std::string nomMap;


	float tempsActuel,tempsPrecedent,temps_ecoule,tempsEcouleDepuisDernierAffichage;
	float z;
	bool augmenterNoir,allerVersImageChargement,m_debut;
	coordonnee m_coordonneePerso;
	int numeroProchaineMap,m_fond;
	float m_tempsChargement;

};

#endif



