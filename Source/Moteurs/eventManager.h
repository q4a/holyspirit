

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





#ifndef EVENTMANAGERH
#define EVENTMANAGERH

#include <iostream>
#include "../Entites/hero.h"

class EventManager
{
	public:
	EventManager();
	void GererLesEvenements(sf::RenderWindow *ecran,sf::View *camera,bool *continuer,float temps,coordonnee tailleMap);
	void AfficherCurseur(sf::RenderWindow *ecran);

	void StopEvenement(int numeroEvenement,std::string evenement);

	bool getEvenement(int numeroEvenement,std::string evenement);
	coordonnee getCasePointee();
	coordonnee getPositionSouris();

	void arreterClique();

	private:
	bool m_EventTableau[500],m_Clic[5],m_ClicAncien[5];
	coordonnee m_positionSouris,m_casePointee;
	int idcurseur;

};

#endif


