///**************************************************\\\
||| Dernière modification : Le 06/02/08 par Gregouar |||
///**************************************************///




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
	void Pose::setPose(coordonnee positionDansLImage,int animation,int son,int image,int attaque);
	coordonnee getCoordonnee();
	int getAnimation();
	int getSon();
	int getImage();
	int getAttaque();

	private:
	coordonnee m_positionDansLImage; // Position du tile dans le tileset
	int m_son,m_animation,m_image,m_attaque;
};

#endif


