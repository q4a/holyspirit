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
	void Pose::setPose(coordonnee positionDansLImage,coordonnee centre,int animation,int son,int image,int attaque,int lum_intensite,float tempsAnimation);
	coordonnee getCoordonnee();
	coordonnee getCentre();
	int getAnimation();
	int getSon();
	int getImage();
	int getAttaque();
	int getLumiereIntensite();

	float getTempsAnimation();

	private:
	coordonnee m_positionDansLImage,m_centre; // Position du tile dans le tileset
	int m_son,m_animation,m_image,m_attaque,m_lumiere_intensite;
	float m_tempsAnimation;
};

#endif



