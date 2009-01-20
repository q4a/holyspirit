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
	~Hero()
	{
	    m_inventaire.clear();
	}

	void Sauvegarder();
	void Charger();

	void afficherInventaire(sf::RenderWindow *,coordonnee,float);
	void afficherCaracteristiques(sf::RenderWindow *ecran,coordonnee positionSouris,float decalage);

	void placerCamera(sf::View *camera,coordonnee dimensionsMap);

	bool testMonstreVise(Monstre *monstre,int hauteurMap);

	void augmenterAme(float temps);
	void recalculerCaracteristiques();

	void infligerDegats(float degats);
	void regenererVie(float vie);
	void regenererFoi(float foi);

	int utiliserClicDroit(coordonnee positionSouris, int monstreVise);

	bool ajouterObjet(Objet objet);

	Objet prendreEnMain(coordonnee positionSouris);
	bool equiper(int numero, int emplacement);
	void AttribuerPositionObjet(coordonnee position,int numero);
	void LibererCases(int numero);

	void setMonstreVise(int monstre);
	void setChercherSac(coordonnee);
	void setSacVise(coordonnee);

	int getMonstreVise();
	coordonnee getChercherSac();
	coordonnee getSacVise();

	Personnage m_personnage;
	Modele_Personnage m_modelePersonnage;

    Caracteristique m_caracteristiques;

    int miracleEnCours,frappeEnCours;

	private:
	coordonneeDecimal m_positionAffichage;
	int m_monstreVise,m_objetEnMain;
	coordonnee m_chercherSac,m_sacVise;


	std::vector<Objet> m_inventaire;
	bool m_caseInventaire[5][8];
};

#endif


