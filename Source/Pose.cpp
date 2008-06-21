#include "Pose.h"

#include <iostream>
#include <fstream>

#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>


Pose Pose::operator=(const Pose &pose)
{
	m_positionDansLImage=pose.m_positionDansLImage;
	m_animation=pose.m_animation;
	m_son=pose.m_son;
	m_image=pose.m_image;
	m_attaque=pose.m_attaque;
    return *this;
}

void Pose::setPose(coordonnee positionDansLImage,int animation,int son,int image,int attaque)
{
	m_positionDansLImage=positionDansLImage;
	m_animation=animation;
	m_son=son;
	m_image=image;
	m_attaque=attaque;
}

coordonnee Pose::getCoordonnee(){return m_positionDansLImage;}
int Pose::getAnimation(){return m_animation;}
int Pose::getSon(){return m_son;}
int Pose::getImage(){return m_image;}
int Pose::getAttaque(){return m_attaque;}

