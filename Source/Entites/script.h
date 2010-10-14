

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



#ifndef SCRIPTH
#define SCRIPTH

#include <iostream>
#include "../constantes.h"

class Instruction
{
    public:

    std::string nom;
    std::string valeurString;

    std::vector < float > m_valeurs;
    std::vector < std::string > m_string_valeurs;
};

class Script
{
public:
    Script();
    Script(const std::string &chemin);
    void Charger(const std::string &chemin);
    void Charger(std::ifstream &fichier);

    void Sauvegarder(std::ofstream &fichier);
    void Sauvegarder_instruction(std::ofstream &fichier , int no);

    void   setVariable(int i, float val);
    float  getVariable(int i);
    int    getNbrVariable();

    void   setValeur(int no, int i, float val);
    float  getValeur(int no, int i);

    void   AjouterCondition(std::ifstream *fichier);
    int    Lire(std::ifstream *fichier);

    std::vector<Instruction> m_instructions;

    std::vector<std::string> m_text;

protected:
    std::vector <float> m_variables;
};

#endif




