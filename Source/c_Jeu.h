

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



#ifndef _JEUH
#define _JEUH

#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>

#include "Contexte.h"
#include "Globale.h"

class c_Jeu : public Contexte
{
    public:
    c_Jeu(Jeu *jeu);
    void Utiliser(Jeu *jeu);

   // sf::View *camera;

    private:

	bool continuer,lumiere,augmenter;
	int nbrTourBoucle;
	char chaine[10];
	sf::String variableQuiNeSertARien,Version,Temps,fps,TourBoucle;

	float tempsActuel,tempsPrecedent,tempsDepuisDerniereAnimation,tempsEcoule,tempsNbrTourBoucle,tempsEcouleDepuisDernierCalculLumiere,tempsEcouleDepuisDernierDeplacement,tempsEcouleDepuisDernierIA,tempsEcouleDepuisDernierAffichage,tempsEcouleDepuisFPS,tempsEffetMort,tempsSauvergarde;
	float alpha_map,alpha_sac;

};

#endif


