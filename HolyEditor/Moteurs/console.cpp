

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

#include "console.h"
#include "../constantes.h"
#include "../globale.h"
#include <iostream>
#include <sstream>

using namespace std;

Console::Console()
{
    m_defilement=0;
    fichier.open("Log.txt", ios::out | ios::trunc);
}

Console::~Console()
{
    m_textes.clear();
    m_erreur.clear();
    fichier.close();
}

void Console::Afficher()
{
    if (!configuration->desactivate_console)
    {
        int min=m_textes.size()-27+m_defilement;
        if (min<0)
            min=0;
        for (int i=min;i<(int)m_textes.size()+m_defilement;i++)
        {
            if (i>=0&&i<(int)m_erreur.size())
            {
                sf::Text texte;
                texte.SetString(m_textes[i].c_str());
                texte.SetSize(12.f);
                texte.SetY((i-m_textes.size()+27-m_defilement)*15+60);

                if (m_erreur[i])
                    texte.SetColor(sf::Color(255,0,0,255));

                moteurGraphique->AjouterTexte(&texte,19);
            }

        }
    }
}
void Console::Ajouter(std::string texte,bool erreur)
{
    if (!configuration->desactivate_console)
    {
        m_textes.push_back(texte);
        m_erreur.push_back(erreur);
        Rapport();

        std::cout<<m_textes.back()<<std::endl;
    }
}

void Console::Ajouter(int nombre,bool erreur)
{
    if (!configuration->desactivate_console)
    {
        std::ostringstream buf;
        buf<<nombre;
        m_textes.push_back(buf.str());
        m_erreur.push_back(erreur);
        Rapport();

        std::cout<<m_textes.back()<<std::endl;
    }
}

void Console::Ajouter(char caractere,bool erreur)
{
    if (!configuration->desactivate_console)
    {
        std::ostringstream buf;
        buf<<caractere;
        m_textes.push_back(buf.str());
        m_erreur.push_back(erreur);
        Rapport();

        std::cout<<m_textes.back()<<std::endl;
    }
}


int Console::getTaille()
{
    return m_textes.size();
}

void Console::Defiler(bool direction)
{
    if (direction==0)
    {
        m_defilement--;
        if (m_defilement<(int)-m_textes.size())
            m_defilement=-m_textes.size()+1;
    }
    if (direction==1)
    {
        m_defilement++;
        if (m_defilement>0)
            m_defilement=0;
    }
}

void Console::Rapport()
{
    if (fichier) // si l'ouverture a réussi
    {
        if (m_erreur.back())
            fichier<<"ERREUR : ";
        else
            fichier<<"         ";

        fichier<<m_textes.back()<<'\n';
    }
}
