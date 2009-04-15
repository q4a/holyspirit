

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


#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

#include "../constantes.h"

#ifndef MOTEURSONSH
#define MOTEURSONSH


class MoteurSons : public CSingleton<MoteurSons>
{
	public:
	friend MoteurSons* CSingleton<MoteurSons>::GetInstance();
    friend void CSingleton<MoteurSons>::Kill();


	void Gerer();
	void Vider();
	int AjouterBuffer(std::string);
	void JouerSon(int ID,coordonnee position,coordonnee positionHero,bool unique=0,int volume=100);

	std::string getCheminSon(int IDimage);


	protected:

	MoteurSons();
	~MoteurSons();

	int sonEnCours;
	sf::Sound m_sons[NOMBRE_SONS];
	int m_IDSons[NOMBRE_SONS];
	std::vector <sf::SoundBuffer> m_buffers;
	std::vector <std::string> m_cheminsSons;//Permet de s'assurer d'avoir des singletons
};

#endif




