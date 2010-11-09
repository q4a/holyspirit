

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



#include "../constantes.h"

#ifndef QUETEH
#define QUETEH


class Document
{
public:
    Document();
    virtual ~Document();

    virtual void SauvegarderTexte(std::ofstream *fichier);
    virtual void ChargerTexte(std::ifstream *fichier);

    void GenerateDescription();

    std::string m_chemin;
    std::string m_nom;
    std::string m_description;
    std::vector<std::string> m_ldescription;
};

class Quete : public Document
{
public:
    Quete();
    Quete(int);
    virtual ~Quete();

    void SauvegarderTexte(std::ofstream *fichier);
    void ChargerTexte(std::ifstream *fichier);

    int m_id;
    int m_statut;
    std::string m_map;
};

#endif

