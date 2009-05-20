

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




#ifndef CONSOLEH
#define CONSOLEH

#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "../singleton.h"

class Console : public CSingleton<Console>
{
	public:

	friend Console* CSingleton<Console>::GetInstance();
    friend void CSingleton<Console>::Kill();

	void Afficher();
	void Ajouter(std::string texte,bool erreur=0);
	void Ajouter(int texte,bool erreur=0);

    void Defiler(bool direction);

    int getTaille();



	protected:

	Console(){m_defilement=0;}
	~Console();

	void Rapport();

	std::vector <std::string> m_textes;
	std::vector <bool> m_erreur;
	int m_defilement;
};


#endif





