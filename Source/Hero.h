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

	void ajouterObjet(Objet objet);

	void setMonstreVise(int monstre);
	void setChercherSac(coordonnee);

	int getMonstreVise();
	coordonnee getChercherSac();

	Personnage m_personnage;
	Modele_Personnage m_modelePersonnage;

	private:
	coordonneeDecimal m_positionAffichage;
	int m_monstreVise;
	coordonnee m_chercherSac;

	std::vector<Objet> m_inventaire;
	bool m_caseInventaire[5][8];
};

#endif


