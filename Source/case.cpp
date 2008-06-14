#include "case.h"

#include <iostream>


coordonnee Case::getPosition()
{
	return m_coordonnee;
}

void Case::setCoordonnee(coordonnee nouvelleCoordonnee,int distance)
{
	m_coordonnee=nouvelleCoordonnee;
	m_distance=distance;
}

int Case::getDistance(){return m_distance;}

Case Case::operator=(const Case &caseCopiee)
{
	m_coordonnee=caseCopiee.m_coordonnee;
	m_distance=caseCopiee.m_distance;
    return *this;
}


