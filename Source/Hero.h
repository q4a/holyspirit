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
	void ChargerModele();
	void ReChargerModele();

	void Afficher(sf::RenderWindow* ecran,sf::View *camera,coordonnee position,coordonnee dimensionsMap);
	void CalculerOrdreAffichage();

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

	bool ajouterObjet(Objet objet,bool enMain=false);

	Objet DeposerObjet();
	bool prendreEnMain(coordonnee positionSouris);
	bool equiper(int numero, int emplacement);
	void AttribuerPositionObjet(coordonnee position,int numero);
	void LibererCases(int numero);

	void setMonstreVise(int monstre);
	void setChercherSac(coordonnee);
	void setSacVise(coordonnee);

	int getMonstreVise();
	coordonnee getChercherSac();
	coordonnee getSacVise();

	Lumiere getPorteeLumineuse();

	Personnage m_personnage;
	Modele_Personnage m_modelePersonnage[NOMBRE_MORCEAU_PERSONNAGE];
	std::string m_cheminModele[NOMBRE_MORCEAU_PERSONNAGE],m_cheminModeleNouveau[NOMBRE_MORCEAU_PERSONNAGE];

    Caracteristique m_caracteristiques;

    int miracleEnCours,frappeEnCours,m_objetEnMain,m_objetADeposer;

	private:
	coordonneeDecimal m_positionAffichage;
	int m_monstreVise;
	coordonnee m_chercherSac,m_sacVise;

	int m_cas;

	int ordreAffichage[NOMBRE_MORCEAU_PERSONNAGE];


	std::vector<Objet> m_inventaire;
	bool m_caseInventaire[10][10];
};

#endif


