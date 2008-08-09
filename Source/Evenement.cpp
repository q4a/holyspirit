#include "Evenement.h"

#include <iostream>

Evenement::Evenement()
{
}

Evenement::Evenement(int numero)
{
    m_numero=numero;
}

Evenement::~Evenement()
{
	m_information.clear();
}

void Evenement::deleteInformations()
{
    m_information.clear();
}

Evenement Evenement::operator=(const Evenement &evenement)
{
    //Copie des variables
	m_numero=evenement.m_numero;
    m_information = evenement.m_information;

    return *this;
}


void Evenement::setEvenement(int numeroEvenement)
{
    m_numero=numeroEvenement;
}


int Evenement::getType()
{
    return m_numero;
}

int Evenement::getInformation(int numeroInfo)
{
    if(m_information.at(numeroInfo))
        return m_information.at(numeroInfo);
    else
        return 0;
}


void Evenement::AjouterInformation(int information)
{
    m_information.push_back(information);
}
