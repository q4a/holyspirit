

/*Copyright (C) 2009 Naisse Gr�goire

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




#ifndef EVENEMENTH
#define EVENEMENTH

#include "../constantes.h"

class Evenement
{
public:
    Evenement();
    Evenement(int numero);
    ~Evenement();
    Evenement operator=(const Evenement &evenement);

    void AjouterInformation(int information); // Ajouter une information � l'�v�nement ( ex : pour un changement de map, le num�ro de la prochaine map, la position du h�ro, etc... )
    void DeleteInformations();

    int getType();
    int getNombreInformation()
    {
        return m_information.size();
    }
    int getInformation(int numeroInfo);
    const std::string &getString();

    void setEvenement(int numeroEvenement);
    void setInformation(int information,int numero);
    void setString(std::string);

protected:
    int m_numero;
    std::string m_string;
    std::vector <int> m_information;
};

#endif


