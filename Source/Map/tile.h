

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
	void setTile(   coordonnee positionDansLImage,int image,bool collision,int animation,int son,Lumiere lumiere, coordonnee lumiere_decal,
                    bool ombre,bool reflection,char orientation,bool transparent,coordonnee centre,float temps, int opacity, int layer,
                    int attaque, int ordre, int angle, int ambientShadow);

	const coordonnee &getCoordonnee();
	bool getCollision();
	int getAnimation();
	int getImage();
	int getSon();
	const Lumiere &getLumiere();
	bool getOmbre();
	bool getReflection();
	bool getTransparent();
	char getOrientation();
	const coordonnee &getCentre();
	const coordonnee &getLumiereDecal();
	float getTemps();
	int getOpacity();
	int getLayer();
	int getAttaque();
    int getOrdre();
    int getAngle();
    int getAmbientShadow();

	void setImage(int);
	void setSon(int);

    int m_tileMinimap;
	coordonnee m_coordMinimap;
	int m_distortion;
	std::vector<int> m_shadowMap;

	private:
	coordonnee m_positionDansLImage,m_centre,m_lumiere_decal;
	bool m_collision,m_ombre,m_reflection,m_transparent;
	int m_animation,m_son,m_image;
	char m_orientation; // Orientation du tile, utilisé pour le calcul des lumières
	Lumiere m_lumiere;
	int m_opacity;

	float m_temps;
	int m_layer;

    int m_ordre;
    int m_attaque;
    int m_angle;
    int m_ambientShadow;

};

#endif


