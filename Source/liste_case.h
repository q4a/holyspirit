///**************************************************\\\
||| Dernière modification : Le 06/02/08 par Gregouar |||
///**************************************************///


///Liste de cases utilisée pour le pathfinding

#ifndef LISTE_CASEH
#define LISTE_CASEH

#include "constantes.h"
#include "case.h"

class liste_case
{
	public:
	~liste_case();
	void liste_case::ajouterCase(coordonnee coordonneeAjoutable);
	void setTailleListe(int taille);
	int getTailleListe();

	bool ajouterCasesAdjacentes(std::vector<std::vector<bool> > grille,coordonnee *arrivee,coordonnee depart,coordonnee exception);
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




