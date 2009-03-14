

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
    if(m_information.size()>0)
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

void Evenement::setInformation(int information,int numero)
{
    if(numero>=0&&numero<(int)m_information.size())
        m_information[numero]=information;
}

void Evenement::AjouterInformation(int information)
{
    m_information.push_back(information);
}
