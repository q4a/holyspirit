

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


#include "liste_case.h"
#include "../globale.h"

#include <iostream>

liste_case::~liste_case()
{
    m_liste.clear();
}

void liste_case::setTailleListe(int taille)
{
    Case caseTemp;
	m_liste.resize(taille,caseTemp);
	m_distanceEnCours=0;
}
int liste_case::getTailleListe(){return m_liste.size();}

void liste_case::ajouterCase(coordonnee coordonneeAjoutable)
{
    Case caseTemp;
	m_liste.push_back(caseTemp);
	m_liste.back().setCoordonnee(coordonneeAjoutable,m_distanceEnCours);
}

void liste_case::supprimer(){m_liste.clear();}

bool liste_case::testerCasesEnCours(coordonnee caseCherchee)
{
    // Je regarde si je suis sur ma case d'arrivee
	for(iter=m_liste.begin();iter!=m_liste.end();++iter)
			if(iter->getPosition().x==caseCherchee.x&&iter->getPosition().y==caseCherchee.y)
				return 1;

	return 0;
}

bool liste_case::ajouterCasesAdjacentes(casePathfinding **grille,coordonnee *arrivee,coordonnee depart)
{
	coordonnee enCours;

	//J'ajoute toutes les cases adjacentes à celle que j'ai déjà dans ma liste
	for(int i=m_liste.size()-1;i>=0;i--)
		if(m_liste[i].getDistance()==m_distanceEnCours-1)
		{
			enCours.x=m_liste[i].getPosition().x+1;
			enCours.y=m_liste[i].getPosition().y;
			if(enCours.x>=0&&enCours.y>=0&&enCours.x<20&&enCours.y<20)
				if(!testerCasesEnCours(enCours)&&!grille[enCours.y][enCours.x].collision&&fabs(grille[enCours.y][enCours.x].hauteur- grille[m_liste[i].getPosition().y][m_liste[i].getPosition().x].hauteur)<=32)
					ajouterCase(enCours);
                /*else if(enCours.x==arrivee->x&&enCours.y==arrivee->y&&grille[enCours.y][enCours.x]&&!(exception.x==m_liste[i].getPosition().x&&exception.y==m_liste[i].getPosition().y))
                {
                    arrivee->x=m_liste[i].getPosition().x,arrivee->y=m_liste[i].getPosition().y;
                    return 0;
                }*/

			enCours.x=m_liste[i].getPosition().x-1;
			enCours.y=m_liste[i].getPosition().y;
			if(enCours.x>=0&&enCours.y>=0&&enCours.x<20&&enCours.y<20)
				if(!testerCasesEnCours(enCours)&&!grille[enCours.y][enCours.x].collision&&fabs(grille[enCours.y][enCours.x].hauteur- grille[m_liste[i].getPosition().y][m_liste[i].getPosition().x].hauteur)<=32)
					ajouterCase(enCours);



			enCours.x=m_liste[i].getPosition().x;
			enCours.y=m_liste[i].getPosition().y+1;
			if(enCours.x>=0&&enCours.y>=0&&enCours.x<20&&enCours.y<20)
				if(!testerCasesEnCours(enCours)&&!grille[enCours.y][enCours.x].collision&&fabs(grille[enCours.y][enCours.x].hauteur- grille[m_liste[i].getPosition().y][m_liste[i].getPosition().x].hauteur)<=32)
					ajouterCase(enCours);


			enCours.x=m_liste[i].getPosition().x;
			enCours.y=m_liste[i].getPosition().y-1;
			if(enCours.x>=0&&enCours.y>=0&&enCours.x<20&&enCours.y<20)
				if(!testerCasesEnCours(enCours)&&!grille[enCours.y][enCours.x].collision&&fabs(grille[enCours.y][enCours.x].hauteur- grille[m_liste[i].getPosition().y][m_liste[i].getPosition().x].hauteur)<=32)
					ajouterCase(enCours);


			enCours.x=m_liste[i].getPosition().x+1;
			enCours.y=m_liste[i].getPosition().y+1;
			if(enCours.x>=0&&enCours.y>=0&&enCours.x<20&&enCours.y<20)
				if(!testerCasesEnCours(enCours)&&!grille[enCours.y][enCours.x].collision&&fabs(grille[enCours.y][enCours.x].hauteur- grille[m_liste[i].getPosition().y][m_liste[i].getPosition().x].hauteur)<=32)
					ajouterCase(enCours);


			enCours.x=m_liste[i].getPosition().x-1;
			enCours.y=m_liste[i].getPosition().y-1;
			if(enCours.x>=0&&enCours.y>=0&&enCours.x<20&&enCours.y<20)
				if(!testerCasesEnCours(enCours)&&!grille[enCours.y][enCours.x].collision&&fabs(grille[enCours.y][enCours.x].hauteur- grille[m_liste[i].getPosition().y][m_liste[i].getPosition().x].hauteur)<=32)
					ajouterCase(enCours);


			enCours.x=m_liste[i].getPosition().x-1;
			enCours.y=m_liste[i].getPosition().y+1;
			if(enCours.x>=0&&enCours.y>=0&&enCours.x<20&&enCours.y<20)
				if(!testerCasesEnCours(enCours)&&!grille[enCours.y][enCours.x].collision&&fabs(grille[enCours.y][enCours.x].hauteur- grille[m_liste[i].getPosition().y][m_liste[i].getPosition().x].hauteur)<=32)
					ajouterCase(enCours);


			enCours.x=m_liste[i].getPosition().x+1;
			enCours.y=m_liste[i].getPosition().y-1;
			if(enCours.x>=0&&enCours.y>=0&&enCours.x<20&&enCours.y<20)
				if(!testerCasesEnCours(enCours)&&!grille[enCours.y][enCours.x].collision&&fabs(grille[enCours.y][enCours.x].hauteur- grille[m_liste[i].getPosition().y][m_liste[i].getPosition().x].hauteur)<=32)
					ajouterCase(enCours);

		}
		else
            return 0;
        return 1;
}

void liste_case::incrementerDistanceEnCours(){m_distanceEnCours++;}

void liste_case::decrementerDistanceEnCours(){m_distanceEnCours--;}

int liste_case::getDistance(){return m_distanceEnCours;}

coordonnee liste_case::trouverLeChemin(coordonnee caseEnCours)
{
    // Je repart de mon arrivee et suis les cases dans l'ordre décroisant jusqu'à me retrouver à ma case départ
	coordonnee enCours;
	for(int i=0;i<(int)m_liste.size();i++)
		if(m_liste[i].getPosition().x==caseEnCours.x&&m_liste[i].getPosition().y==caseEnCours.y)
		{
			enCours.y=m_liste[i].getPosition().y;
			enCours.x=m_liste[i].getPosition().x+1;
			for(int j=0;j<(int)m_liste.size();j++)
				if(m_liste[j].getDistance()==m_distanceEnCours-1&&m_liste[j].getPosition().x==enCours.x&&m_liste[j].getPosition().y==enCours.y)
					return m_liste[j].getPosition();

			enCours.y=m_liste[i].getPosition().y;
			enCours.x=m_liste[i].getPosition().x-1;
			for(int j=0;j<(int)m_liste.size();j++)
				if(m_liste[j].getDistance()==m_distanceEnCours-1&&m_liste[j].getPosition().x==enCours.x&&m_liste[j].getPosition().y==enCours.y)
					return m_liste[j].getPosition();

			enCours.y=m_liste[i].getPosition().y+1;
			enCours.x=m_liste[i].getPosition().x;
			for(int j=0;j<(int)m_liste.size();j++)
				if(m_liste[j].getDistance()==m_distanceEnCours-1&&m_liste[j].getPosition().x==enCours.x&&m_liste[j].getPosition().y==enCours.y)
					return m_liste[j].getPosition();

			enCours.y=m_liste[i].getPosition().y-1;
			enCours.x=m_liste[i].getPosition().x;
			for(int j=0;j<(int)m_liste.size();j++)
				if(m_liste[j].getDistance()==m_distanceEnCours-1&&m_liste[j].getPosition().x==enCours.x&&m_liste[j].getPosition().y==enCours.y)
					return m_liste[j].getPosition();

			enCours.y=m_liste[i].getPosition().y-1;
			enCours.x=m_liste[i].getPosition().x+1;
			for(int j=0;j<(int)m_liste.size();j++)
				if(m_liste[j].getDistance()==m_distanceEnCours-1&&m_liste[j].getPosition().x==enCours.x&&m_liste[j].getPosition().y==enCours.y)
					return m_liste[j].getPosition();

			enCours.y=m_liste[i].getPosition().y+1;
			enCours.x=m_liste[i].getPosition().x-1;
			for(int j=0;j<(int)m_liste.size();j++)
				if(m_liste[j].getDistance()==m_distanceEnCours-1&&m_liste[j].getPosition().x==enCours.x&&m_liste[j].getPosition().y==enCours.y)
					return m_liste[j].getPosition();

			enCours.y=m_liste[i].getPosition().y+1;
			enCours.x=m_liste[i].getPosition().x+1;
			for(int j=0;j<(int)m_liste.size();j++)
				if(m_liste[j].getDistance()==m_distanceEnCours-1&&m_liste[j].getPosition().x==enCours.x&&m_liste[j].getPosition().y==enCours.y)
					return m_liste[j].getPosition();

			enCours.y=m_liste[i].getPosition().y-1;
			enCours.x=m_liste[i].getPosition().x-1;
			for(int j=0;j<(int)m_liste.size();j++)
				if(m_liste[j].getDistance()==m_distanceEnCours-1&&m_liste[j].getPosition().x==enCours.x&&m_liste[j].getPosition().y==enCours.y)
					return m_liste[j].getPosition();

            enCours.y=m_liste[i].getPosition().y;
			enCours.x=m_liste[i].getPosition().x;
			for(int j=0;j<(int)m_liste.size();j++)
				if(m_liste[j].getDistance()==m_distanceEnCours-1&&m_liste[j].getPosition().x==enCours.x&&m_liste[j].getPosition().y==enCours.y)
					return m_liste[j].getPosition();
		}
		return enCours;
}

