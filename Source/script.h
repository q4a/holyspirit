
#ifndef SCRIPTH
#define SCRIPTH

#include <iostream>
#include "constantes.h"

struct Instruction
{
    std::string nom;
    std::vector < int > valeurs;
};

class Script
{
	public:
    Script();
    Script(std::string chemin);
	void Charger(std::string chemin);

	void AjouterCondition(std::ifstream *fichier);
	int Lire(std::ifstream *fichier);

	std::vector<Instruction> m_instructions;
};

#endif




