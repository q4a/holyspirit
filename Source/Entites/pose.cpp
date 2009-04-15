

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


#include "Pose.h"

#include <iostream.h>
#include <fstream>

#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>

Pose Pose::operator=(const Pose &pose)
{
	m_positionDansLImage=pose.m_positionDansLImage;
	m_centre=pose.m_centre;
	m_animation=pose.m_animation;
	m_son=pose.m_son;
	m_image=pose.m_image;
	m_attaque=pose.m_attaque;
	m_lumiere_intensite=pose.m_lumiere_intensite;
	m_tempsAnimation=pose.m_tempsAnimation;
	m_ordre=pose.m_ordre;
    return *this;
}

void Pose::setPose(coordonnee positionDansLImage,coordonnee centre,int animation,int son,int image,int attaque,int lum_intensite,float tempsAnimation,int ordre)
{
	m_positionDansLImage=positionDansLImage;
	m_centre=centre;
	m_animation=animation;
	m_son=son;
	m_image=image;
	m_attaque=attaque;
	m_lumiere_intensite=lum_intensite;

	m_tempsAnimation=tempsAnimation;

	m_ordre=ordre;
}

coordonnee Pose::getCoordonnee(){return m_positionDansLImage;}
coordonnee Pose::getCentre(){return m_centre;}
int Pose::getAnimation(){return m_animation;}
int Pose::getSon(){return m_son;}
int Pose::getImage(){return m_image;}
int Pose::getAttaque(){return m_attaque;}
int Pose::getLumiereIntensite(){return m_lumiere_intensite;}
float Pose::getTempsAnimation(){return m_tempsAnimation;}
int Pose::getOrdre() {return m_ordre;}

