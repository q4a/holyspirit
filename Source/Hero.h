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

	void afficherInventaire(sf::RenderWindow *,coordonnee,float);

	void placerCamera(sf::View *camera,coordonnee dimensionsMap);

	void testMontreVise(Monstre *monstre,int hauteurMap);

	void augmenterAme(float temps);

	bool ajouterObjet(Objet objet);

	Objet prendreEnMain(coordonnee positionSouris);
	void AttribuerPositionObjet(coordonnee position,int numero);
	void LibererCases(int numero);

	void setMonstreVise(int monstre);
	void setChercherSac(coordonnee);

	int getMonstreVise();
	coordonnee getChercherSac();

	Personnage m_personnage;
	Modele_Personnage m_modelePersonnage;

	private:
	coordonneeDecimal m_positionAffichage;
	int m_monstreVise,m_objetEnMain;
	coordonnee m_chercherSac;

	std::vector<Objet> m_inventaire;
	bool m_caseInventaire[5][8];
};

#endif


