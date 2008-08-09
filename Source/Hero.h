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

	void Sauvegarder();
	void Charger();

	void placerCamera(sf::View *camera,coordonnee dimensionsMap);

	void testMontreVise(Monstre *monstre,int hauteurMap);

	void augmenterAme(float temps);

	void setMonstreVise(int monstre);

	int getMonstreVise();

	Personnage m_personnage;
	Modele_Personnage m_modelePersonnage;

	private:
	coordonnee m_positionAffichage;
	int m_monstreVise;
};

#endif


