///**************************************************\\\
||| Dernière modification : Le 06/02/08 par Gregouar |||
///**************************************************///




#ifndef HEROH
#define HEROH

#include <iostream>
#include <string>

#include "Personnage.h"
#include "Monstre.h"
class Hero
{
	public:
	Hero();
	void placerCamera(sf::View *camera,coordonnee dimensionsMap);

	void testMontreVise(Monstre *monstre,int hauteurMap);

	void setMonstreVise(int monstre);

	int getEnemiVise();

	Personnage m_personnage;
	Modele_Personnage m_modelePersonnage;

	private:
	coordonnee m_positionAffichage;
	int m_enemiVise;
};

#endif

