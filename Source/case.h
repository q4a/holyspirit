///**************************************************\\\
||| Dernière modification : Le 06/02/08 par Gregouar |||
///**************************************************///


#ifndef CASEH
#define CASEH

#include "constantes.h"
#include "Decor.h"

class Case
{
	public:
	coordonnee getPosition();
	void setCoordonnee(coordonnee nouvelleCoordonnee,int distance);
	int getDistance();

	Case operator=(const Case &caseCopiee);

	private:
	coordonnee m_coordonnee;
	int m_distance;
};

#endif


