///**************************************************\\\
||| Dernière modification : Le 06/02/08 par Gregouar |||
///**************************************************///




#ifndef HEROH
#define HEROH

#include <iostream>
#include <string>

#include "Personnage.h"
class Hero
{
	public:
	Hero();
	void placerCamera(sf::View *camera,coordonnee dimensionsMap);

	Personnage m_personnage;
	Modele_Personnage m_modelePersonnage;

	private:
	coordonnee m_positionAffichage;
};

#endif

