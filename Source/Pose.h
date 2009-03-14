

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




#ifndef POSEH
#define POSEH

#include <iostream>
#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include "constantes.h"

class Pose
{
	public:
	Pose operator=(const Pose &Pose);
	void Pose::setPose(coordonnee positionDansLImage,coordonnee centre,int animation,int son,int image,int attaque,int lum_intensite,float tempsAnimation,int ordre);
	coordonnee getCoordonnee();
	coordonnee getCentre();
	int getAnimation();
	int getSon();
	int getImage();
	int getAttaque();
	int getLumiereIntensite();
	int getOrdre();

	float getTempsAnimation();

	private:
	coordonnee m_positionDansLImage,m_centre; // Position du tile dans le tileset
	int m_son,m_animation,m_image,m_attaque,m_lumiere_intensite;
	float m_tempsAnimation;
	int m_ordre;
};

#endif



