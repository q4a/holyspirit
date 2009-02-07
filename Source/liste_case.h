

///Liste de cases utilisée pour le pathfinding

#ifndef LISTE_CASEH
#define LISTE_CASEH

#include "constantes.h"
#include "case.h"

struct casePathfinding
{
    bool collision;
    int hauteur;
};

class liste_case
{
	public:
	~liste_case();
	void liste_case::ajouterCase(coordonnee coordonneeAjoutable);
	void setTailleListe(int taille);
	int getTailleListe();

	bool ajouterCasesAdjacentes(casePathfinding ** grille,coordonnee *arrivee,coordonnee depart);
	bool testerCasesEnCours(coordonnee caseCherchee);
	void incrementerDistanceEnCours();
	void decrementerDistanceEnCours();
	void supprimer();
	int getDistance();
	coordonnee trouverLeChemin(coordonnee caseEnCours);

	private:
	int m_distanceEnCours;
	std::vector <Case> m_liste;
};

#endif




