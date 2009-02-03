


#ifndef HEROH
#define HEROH

#include <iostream>
#include <string>

#include "Personnage.h"
#include "Monstre.h"

struct Image_interface
{
    coordonnee position;
    int image;
};

struct Emplacement_inventaire
{
    coordonnee position;
    int emplacement;
};

struct Classe
{
    void Charger(std::string chemin);

    std::string nom;
    Caracteristique caracteristique;

    std::vector<std::string> equipementParDefaut;
    std::string modeleNu[3];

    Image_interface inventaire;
    Image_interface hud;
    Image_interface orbe_vie;
    Image_interface orbe_foi;

    coordonnee position_sac_inventaire;
    coordonnee position_contenu_inventaire;

    std::vector <Emplacement_inventaire> emplacements;
};

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

	void Afficher(sf::RenderWindow* ecran,sf::View *camera,coordonnee position,coordonnee dimensionsMap);
	void CalculerOrdreAffichage();

	void afficherInventaire(sf::RenderWindow *,coordonnee,float);
	void afficherCaracteristiques(sf::RenderWindow *ecran,coordonnee positionSouris,float decalage);
	void GenererGrille();
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
	//void LibererCases(int numero);

	void setMonstreVise(int monstre);
	void setChercherSac(coordonnee);
	void setSacVise(coordonnee);

	int getMonstreVise();
	coordonnee getChercherSac();
	coordonnee getSacVise();

	Lumiere getPorteeLumineuse();

	std::string m_cheminClasse;
	Classe m_classe;

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
	std::vector< std::vector<bool> > m_caseInventaire;
};

#endif


