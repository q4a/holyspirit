

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





#ifndef HEROH
#define HEROH

#include <iostream.h>
#include <string>

#include "personnage.h"
#include "monstre.h"

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

    int ID;

    std::string nom;
    Caracteristique caracteristique;


    std::string chemin_modele;

    std::vector<std::string> equipementParDefaut;
    std::string modeleNu[3];

    Image_interface inventaire;
    Image_interface hud;
    Image_interface orbe_vie;
    Image_interface orbe_foi;
    Image_interface cache_vie;
    Image_interface cache_foi;

    coordonnee position_sac_inventaire;
    coordonnee position_contenu_inventaire;

    std::vector <Emplacement_inventaire> emplacements;
};

class Hero
{
	public:
	Hero();
	~Hero();

	void Sauvegarder();
	void Charger();
	void ChargerModele(bool tout=false);

	void Afficher(coordonnee dimensionsMap);
	void CalculerOrdreAffichage();

	void afficherInventaire(coordonnee,float);
	void afficherCaracteristiques(coordonnee positionSouris,float decalage);
	void GenererGrille();
	void placerCamera(coordonnee dimensionsMap);

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
    int m_argent;

    //cDAT m_fichierSave;
    std::vector<std::string> m_contenuSave;

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


