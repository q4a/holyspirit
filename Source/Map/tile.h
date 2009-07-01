

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





#ifndef TILEH
#define TILEH

#include <iostream>
#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include "../constantes.h"

class Tile
{
	public:
	Tile();
	~Tile();
	Tile operator=(const Tile &tile);
	void setTile(coordonnee positionDansLImage,int image,bool collision,int animation,int son,Lumiere lumiere,bool ombre,char orientation,bool transparent,coordonnee centre,float temps);

	const coordonnee &getCoordonnee();
	bool getCollision();
	int getAnimation();
	int getImage();
	int getSon();
	const Lumiere &getLumiere();
	bool getOmbre();
	bool getTransparent();
	char getOrientation();
	const coordonnee &getCentre();
	float getTemps();

	void setImage(int);
	void setSon(int);

	private:
	coordonnee m_positionDansLImage,m_centre;
	bool m_collision,m_ombre,m_transparent;
	int m_animation,m_son,m_image;
	char m_orientation; // Orientation du tile, utilisé pour le calcul des lumières
	Lumiere m_lumiere;

	float m_temps;

};

#endif


