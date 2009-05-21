

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
    void AjouterCase(coordonnee coordonneeAjoutable);

    bool AjouterCasesAdjacentes(casePathfinding ** grille,coordonnee *arrivee,coordonnee depart);
    bool TesterCasesEnCours(coordonnee caseCherchee);
    void IncrementerDistanceEnCours();
    void DecrementerDistanceEnCours();
    void Supprimer();

    coordonnee TrouverLeChemin(coordonnee caseEnCours);

    int getDistance();
    int getTailleListe();
    void setTailleListe(int taille);

private:
    int m_distanceEnCours;
    std::vector <Case> m_liste;
    std::vector<Case>::iterator iter;
};

#endif




