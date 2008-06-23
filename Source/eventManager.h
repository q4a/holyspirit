///**************************************************\\\
||| Dernière modification : Le 06/02/08 par Gregouar |||
///**************************************************///




#ifndef EVENTMANAGERH
#define EVENTMANAGERH

#include <iostream>
#include "prototypes.h"
#include "hero.h"

class EventManager
{
	public:
	EventManager();
	void GererLesEvenements(sf::RenderWindow *ecran,sf::View *camera,bool *continuer,float temps,coordonnee tailleMap);
	void AfficherCurseur(sf::RenderWindow *ecran);

	bool getEvenement(int numeroEvenement,std::string evenement);
	coordonnee getCasePointee();
	coordonnee getPositionSouris();

	void arreterClique();

	private:
	bool m_EventTableau[500],m_Clic[5],m_ClicAncien[5];
	coordonnee m_positionSouris,m_casePointee;
	sf::Image m_curseur;

};

#endif

