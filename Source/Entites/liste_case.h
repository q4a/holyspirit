

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




///Liste de cases utilisée pour le pathfinding

#ifndef LISTE_CASEH
#define LISTE_CASEH

#include "../constantes.h"
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
	void ajouterCase(coordonnee coordonneeAjoutable);
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
	std::vector<Case>::iterator iter;
};

#endif




