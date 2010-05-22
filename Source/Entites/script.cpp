

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
}


Script::Script(const std::string &chemin)
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
        {
            m_instructions[noCondition].m_valeurs.push_back(temp);
            m_instructions[noCondition].m_var_valeurs.push_back(-1);
        }
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
    else if (temp=="*" || temp=="$")
    {
        int valeur;

        *fichier>>valeur;

        if(temp=="*")
        {
            m_instructions.back().m_valeurs.push_back(valeur);
            m_instructions.back().m_var_valeurs.push_back(-1);
        }
        else
        {
            m_instructions.back().m_valeurs.push_back(-1);
            m_instructions.back().m_var_valeurs.push_back(valeur);
        }

        retour = -4;
    }
    else if (temp=="\"")
    {
        m_instructions.back().valeurString = "";
        char caractere;

        fichier->get(caractere);

        do
        {
            fichier->get(caractere);
            m_instructions.back().valeurString += caractere;
        }
        while(caractere!= '"');
        m_instructions.back().valeurString.resize(m_instructions.back().valeurString.size() - 1);

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

void Script::Sauvegarder_instruction(ofstream &fichier , int no)
{
    if(no == -2)
        fichier<<"then"<<endl;
    else if(no == -3)
        fichier<<"else"<<endl;
    else if(no >= 0 && no < (int)m_instructions.size())
    {
        fichier<<m_instructions[no].nom<<" ";
        if(m_instructions[no].nom == "if" || m_instructions[no].nom == "main")
        {
            /*if(m_instructions[no].nom == "main")
            {
                for(int i = 0 ; i < m_variables.size() ; ++i)
                    fichier<<endl<<"variable * "<<i<<" * "<<m_variables[i]<<endl;
            }*/
            for(unsigned i = 0 ; i < m_instructions[no].m_valeurs.size() ; ++i)
                Sauvegarder_instruction(fichier ,m_instructions[no].m_valeurs[i]);

            fichier<<"end"<<endl;
        }
        else
        {
            for(unsigned i = 0 ; i < m_instructions[no].m_valeurs.size() ; ++i)
                if(m_instructions[no].m_var_valeurs[i] >= 0)
                    fichier<<"$ "<<m_instructions[no].m_var_valeurs[i]<<" ";
                else
                    fichier<<"* "<<m_instructions[no].m_valeurs[i]<<" ";
        }
        fichier<<endl;
    }
}

void Script::Sauvegarder(ofstream &fichier)
{
    for(int i = 0 ; i < m_variables.size() ; ++i)
        fichier<<endl<<"variable "<<i<<" "<<m_variables[i]<<endl;
    Sauvegarder_instruction(fichier , 0);
}

void Script::Charger(ifstream &fichier)
{
    if (fichier)
    {
        string temp;
        while (!fichier.eof()&&temp!="main")
        {
            fichier>>temp;
            if(temp == "main")
            {
                Instruction instructionBuffer;
                instructionBuffer.nom="main";
                m_instructions.push_back(instructionBuffer);
            }
            else if(temp == "variable")
            {
                int no = 0, val = 0;
                fichier>>no>>val;
                 if(no >= m_variables.size())
                    m_variables.resize(no + 1);
                m_variables[no] = val;
            }

        }

        bool OK=true;
        while (!fichier.eof()&&OK==true)
        {
            int temp=Lire(&fichier);

            if (temp==-1)
                OK=false;
            else
            {
                m_instructions[0].m_valeurs.push_back(temp);
                m_instructions[0].m_var_valeurs.push_back(-1);
            }

        }
        fichier.close();
    }

}

void Script::Charger(const std::string &chemin)
{
    ifstream fichier;
    fichier.open(chemin.c_str(), ios::in);

    Charger(fichier);

    console->Ajouter("Chargement du script : \" "+chemin+" \"");
}

void Script::setVariable(int i, int val)
{
    if(i >= m_variables.size())
        m_variables.resize(i + 1);

    m_variables[i] = val;
}

int Script::getVariable(int i)
{
    if(i >= m_variables.size())
        m_variables.resize(i + 1, 0);

    return m_variables[i];
}

int Script::getNbrVariable()
{
    return m_variables.size();
}

void Script::setValeur(int no, int i, int val)
{
    if( no >= 0 && no < m_instructions.size())
    {
        if(i >= m_instructions[no].m_valeurs.size())
            m_instructions[no].m_valeurs.resize(i + 1, 0), m_instructions[no].m_var_valeurs.resize(i + 1, -1);

        m_instructions[no].m_valeurs[i] = val;
    }
}

int Script::getValeur(int no, int i)
{
    if( no >= 0 && no < m_instructions.size())
    {
        if(i >= m_instructions[no].m_valeurs.size())
            m_instructions[no].m_valeurs.resize(i + 1, 0), m_instructions[no].m_var_valeurs.resize(i + 1, -1);

        if(m_instructions[no].m_var_valeurs[i] >= 0)
        {
            if(m_instructions[no].m_var_valeurs[i] < m_variables.size())
                return m_variables[m_instructions[no].m_var_valeurs[i]];
            else
                return (0);
        }
        else
            return m_instructions[no].m_valeurs[i];
    }

    return (0);
}
