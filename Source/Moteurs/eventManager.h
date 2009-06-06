

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
#include "../constantes.h"

class EventManager : public CSingleton<EventManager>
{
	public:

	friend EventManager* CSingleton<EventManager>::GetInstance();
    friend void CSingleton<EventManager>::Kill();

	EventManager();
	void GererLesEvenements(bool *continuer,float temps,coordonnee tailleMap);
	void AfficherCurseur();

	void StopEvenement(int numeroEvenement,std::string evenement);

	bool getEvenement(int numeroEvenement,std::string evenement);
	coordonnee getCasePointee();
	coordonnee getPositionSouris();
	int getMolette();

	void arreterClique();

	private:
	bool m_EventTableau[500],m_Clic[5],m_ClicAncien[5];
	coordonnee m_positionSouris,m_casePointee;
	int idcurseur;
	int m_molette;

};

#endif


