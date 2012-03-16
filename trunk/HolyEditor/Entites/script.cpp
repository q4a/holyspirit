

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
51 Franklin Street, Fifth Floor, Boston, MA 02110301 USA.*/


#include <fstream>
#include <sstream>
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
            if(temp != -4)
            {
                m_instructions[noCondition].m_valeurs.push_back(temp);
                m_instructions[noCondition].m_string_valeurs.push_back("");
            }
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
    else if (temp=="*")
    {
        std::string string;
        *fichier>>string;

        bool NaN = false;
        for(unsigned i = 0 ; i < string.size() - 1 ; ++i)
            if((string[i] < '0' || string[i] > '9') && string[i] != '-')
                NaN = true;

        if(NaN)
        {
            m_instructions.back().m_valeurs.push_back(-1);
            m_instructions.back().m_string_valeurs.push_back(string);

            if(string == "variable")
                *fichier>>m_instructions.back().m_valeurs.back();
        }
        else
        {
            int valeur;

            istringstream buf(string);
            buf>>valeur;

            m_instructions.back().m_valeurs.push_back(valeur);
            m_instructions.back().m_string_valeurs.push_back("");
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

void Script::Sauvegarder_instruction(ostringstream &fichier , int no, int indent, bool plus3)
{
    //if(no == -3 || no == -2 || no == -1 || !m_instructions[no].nom.empty())
    for(int i = 0 ; i < indent - (no == -3 || no == -2 || no == -1); ++i)
        fichier<<'\t';
    if(plus3)
        fichier<<"   ";

    if(no == -2)
        fichier<<"then"<<endl;
    else if(no == -3)
        fichier<<"else"<<endl;
    else if(no >= 0 && no < (int)m_instructions.size())
    {
        fichier<<m_instructions[no].nom<<" ";
        if(m_instructions[no].nom == "if" || m_instructions[no].nom == "main")
        {
           // cout<<indent<<endl;
            int id = 0;

            if(m_instructions[no].nom == "main")
                fichier<<endl,id = 1;

          //  cout<<m_instructions[no].m_valeurs.size()<<endl;

            for(unsigned i = 0 ; i < m_instructions[no].m_valeurs.size() && m_instructions[no].m_valeurs[i] != -1 ; ++i)
            {
                if(m_instructions[no].nom == "main")
                    id = 1;
                else
                {
                    if(i != 0 && id == 0)
                        id = indent;
                    if(m_instructions[no].m_valeurs[i] == -2)
                        id = indent + 1;
                }

                Sauvegarder_instruction(fichier ,m_instructions[no].m_valeurs[i],id,(indent == id && m_instructions[no].nom == "if"));
            }

            for(int i = 0 ; i < indent; ++i)
                fichier<<'\t';

            fichier<<"end"<<endl;
        }
        else
        {
            for(unsigned i = 0 ; i < m_instructions[no].m_valeurs.size() ; ++i)
                if(m_instructions[no].m_string_valeurs[i] != "")
                {
                    fichier<<"* "<<m_instructions[no].m_string_valeurs[i]<<" ";
                    if(m_instructions[no].m_string_valeurs[i] == "variable")
                        fichier<<m_instructions[no].m_valeurs[i]<<" ";
                }
                else
                    fichier<<"* "<<m_instructions[no].m_valeurs[i]<<" ";

            if(!m_instructions[no].valeurString.empty())
                fichier<<"\" "<<m_instructions[no].valeurString<<"\"";
        }
        fichier<<endl;
    }
}

void Script::Sauvegarder(ostringstream &fichier)
{
    for(int i = 0 ; i < m_variables.size() ; ++i)
        fichier<<endl<<"variable "<<i<<" "<<m_variables[i]<<endl;
    Sauvegarder_instruction(fichier , 0);
}

void Script::Sauvegarder_instruction(ofstream &fichier , int no, int indent)
{
    for(int i = 0 ; i < indent - (no == -2 || no == -1); ++i)
        fichier<<'\t';

    if(no == -2)
        fichier<<"then"<<endl;
    else if(no == -3)
        fichier<<"else"<<endl;
    else if(no >= 0 && no < (int)m_instructions.size())
    {
        fichier<<m_instructions[no].nom<<" ";
        if(m_instructions[no].nom == "if" || m_instructions[no].nom == "main")
        {
            for(unsigned i = 0 ; i < m_instructions[no].m_valeurs.size() ; ++i)
                Sauvegarder_instruction(fichier ,m_instructions[no].m_valeurs[i],indent+1);

            fichier<<"end"<<endl;
        }
        else
        {
            for(unsigned i = 0 ; i < m_instructions[no].m_valeurs.size() ; ++i)
                if(m_instructions[no].m_string_valeurs[i] != "")
                {
                    fichier<<"* "<<m_instructions[no].m_string_valeurs[i]<<" ";
                    if(m_instructions[no].m_string_valeurs[i] == "variable")
                        fichier<<m_instructions[no].m_valeurs[i]<<" ";
                }
                else
                    fichier<<"* "<<m_instructions[no].m_valeurs[i]<<" ";

            if(!m_instructions[no].valeurString.empty())
                fichier<<"\" "<<m_instructions[no].valeurString<<"\"";
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
                m_instructions[0].m_string_valeurs.push_back("");
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

void Script::setVariable(int i, float val)
{
    if(i >= m_variables.size())
        m_variables.resize(i + 1);

    m_variables[i] = val;
}

float Script::getVariable(int i)
{
    if(i >= m_variables.size())
        m_variables.resize(i + 1, 0);

    return m_variables[i];
}

int Script::getNbrVariable()
{
    return m_variables.size();
}

void Script::setValeur(int no, int i, float val)
{
    if( no >= 0 && no < m_instructions.size())
    {
        if(i >= m_instructions[no].m_valeurs.size())
            m_instructions[no].m_valeurs.resize(i + 1, 0), m_instructions[no].m_string_valeurs.resize(i + 1, "");

        m_instructions[no].m_valeurs[i] = val;
    }
}

float Script::getValeur(int no, int i)
{
    if( no >= 0 && no < m_instructions.size())
    {
        if(i >= m_instructions[no].m_valeurs.size())
        {
            m_instructions[no].m_valeurs.resize(i + 1, 0);
            m_instructions[no].m_string_valeurs.resize(i + 1, "");
        }

        if(m_instructions[no].m_string_valeurs[i] != "")
        {
           /*if(m_instructions[no].m_string_valeurs[i] == "day")
                return configuration->jour;
            else if(m_instructions[no].m_string_valeurs[i] == "time")
                return configuration->elapsed_time;
            else if(m_instructions[no].m_string_valeurs[i] == "variable")
            {
                if(m_instructions[no].m_valeurs[i] < m_variables.size())
                    return m_variables[m_instructions[no].m_valeurs[i]];
                else
                    return (0);
            }*/

        }
        else
            return m_instructions[no].m_valeurs[i];
    }

    return (0);
}
