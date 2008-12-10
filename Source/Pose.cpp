#include "Pose.h"

#include <iostream>
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
    return *this;
}

void Pose::setPose(coordonnee positionDansLImage,coordonnee centre,int animation,int son,int image,int attaque,int lum_intensite)
{
	m_positionDansLImage=positionDansLImage;
	m_centre=centre;
	m_animation=animation;
	m_son=son;
	m_image=image;
	m_attaque=attaque;
	m_lumiere_intensite=lum_intensite;
}

coordonnee Pose::getCoordonnee(){return m_positionDansLImage;}
coordonnee Pose::getCentre(){return m_centre;}
int Pose::getAnimation(){return m_animation;}
int Pose::getSon(){return m_son;}
int Pose::getImage(){return m_image;}
int Pose::getAttaque(){return m_attaque;}
int Pose::getLumiereIntensite(){return m_lumiere_intensite;}

