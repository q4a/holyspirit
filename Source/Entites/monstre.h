

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


#ifndef MONSTREH
#define MONSTREH

#include "personnage.h"
#include "../constantes.h"

class Modele_Monstre : public Modele_Personnage
{
public:
    Modele_Monstre();
    ~Modele_Monstre();

    bool Charger(const std::string &chemin);
    std::vector <std::vector<Objet> > getObjets();

    std::string m_chemin;

    Script m_scriptAI;
    int m_particules;
    bool m_minimap;
    bool m_friendly;

private:
    std::vector <std::vector<Objet> > m_objets;
};


class Monstre : public Personnage
{
public:
    Monstre();
    Monstre(Personnage &);
    ~Monstre();
    void Charger(int numero,Modele_Monstre *modele);
    void GenererInventaire(Modele_Monstre *modele);
    void TesterVision(coordonnee positionHero);


    bool getVu();
    void setVu(bool);


    float m_attente;
    int m_compteur;

private:
    bool m_vu;
};


#endif

