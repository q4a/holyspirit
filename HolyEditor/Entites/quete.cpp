

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


#include <iostream>
#include <fstream>

#include "quete.h"
#include "../constantes.h"
#include "../globale.h"

using namespace std;

Quete::     Quete()
{
    m_nom           = " ";
    m_description   = " ";
    m_id            = 0;
    m_statut        = 0;
}

Quete::     Quete(int id)
{
    m_nom           = " ";
    m_description   = " ";
    m_id            = id;
    m_statut        = 0;
}

Quete::    ~Quete()
{

}


void    Quete::    SauvegarderTexte(std::ofstream *fichier)
{
    std::string description = m_description;
    std::string nom = m_nom;

    for (int i=0;i<(int)description.size();i++)
    {
        if (description[i]=='\n')
                description[i]='\\';
        if (description[i]==' ')
                description[i]='_';
    }
    for (int i=0;i<(int)nom.size();i++)
    {
        if (nom[i]=='\n')
                nom[i]='\\';
        if (nom[i]==' ')
                nom[i]='_';
    }

    *fichier<<" q ";

    *fichier<<" n"<<nom;
    *fichier<<" d"<<description;
    *fichier<<" i"<<m_id;
    *fichier<<" s"<<m_statut;

    *fichier<<" $ ";

    if (configuration->debug)
        console->Ajouter("/Sauvegarde de "+m_nom);
}

void    Quete::    ChargerTexte(std::ifstream *fichier)
{
    char caractere;
    do
    {
        fichier->get(caractere);
        if (caractere=='n')
            *fichier>>m_nom;

        else if (caractere=='d')
            *fichier>>m_description;

        else if (caractere=='i')
            *fichier>>m_id;
        else if (caractere=='s')
            *fichier>>m_statut;


        if (fichier->eof())
        {
            throw "Impossible de charger la qu�te";
        }
    }
    while (caractere!='$');




    for (int i=0;i<(int)m_description.size();i++)
    {
        if (m_description[i]=='\\')
                m_description[i]='\n';
        if (m_description[i]=='_')
                m_description[i]=' ';
    }
    for (int i=0;i<(int)m_nom.size();i++)
    {
        if (m_nom[i]=='\\')
                m_nom[i]='\n';
        if (m_nom[i]=='_')
                m_nom[i]=' ';
    }

    cout<<m_nom<<endl;
}
