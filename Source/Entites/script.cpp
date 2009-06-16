

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


#include <fstream>
#include "script.h"

#include "../globale.h"

using namespace std;


Script::Script()
{
    for (int i=0;i<10;i++)
        variables[i]=0;
}


Script::Script(std::string chemin)
{
    Charger(chemin);
}

void Script::AjouterCondition(ifstream *fichier)
{
    int noCondition=0;

    Instruction instructionBuffer;
    instructionBuffer.nom="if";
    m_instructions.push_back(instructionBuffer);

    noCondition=m_instructions.size()-1;

    bool OK=true;

    while (!fichier->eof()&&OK)
    {
        int temp;
        temp=Lire(fichier);

        if (temp==-1)
            OK=false;
        else
            m_instructions.at(noCondition).valeurs.push_back(temp);
    }
}

int Script::Lire(ifstream *fichier)
{
    int retour=0;

    string temp;

    *fichier>>temp;
    if (temp=="if")
        retour=m_instructions.size(),AjouterCondition(fichier);
    else if (temp=="end")
        retour=-1;
    else if (temp=="then")
        retour=-2;
    else if (temp=="else")
        retour=-3;
    else if (temp=="*")
    {
        int valeur;
        *fichier>>valeur;
        m_instructions.back().valeurs.push_back(valeur);
        retour = -4;
    }
    else if (temp=="\"")
    {
        m_instructions.back().valeurString = "\"";
        char caractere;

        do
        {
            fichier->get(caractere);
            m_instructions.back().valeurString += caractere;
        }
        while(caractere!= '"');

        retour = -4;
    }
    else
    {
        retour=m_instructions.size();

        Instruction instructionBuffer;
        instructionBuffer.nom=temp;

        m_instructions.push_back(instructionBuffer);
    }

    return retour;
}

void Script::Charger(std::string chemin)
{
    ifstream fichier;
    fichier.open(chemin.c_str(), ios::in);

    if (fichier)
    {
        string temp;
        while (!fichier.eof()&&temp!="main")
        {
            fichier>>temp;
            Instruction instructionBuffer;
            instructionBuffer.nom="main";
            m_instructions.push_back(instructionBuffer);
        }

        bool OK=true;
        while (!fichier.eof()&&OK==true)
        {
            int temp=Lire(&fichier);

            if (temp==-1)
                OK=false;
            else
                m_instructions[0].valeurs.push_back(temp);

        }

        fichier.close();
    }

    console->Ajouter("Chargement du script : \" "+chemin+" \"");
}
