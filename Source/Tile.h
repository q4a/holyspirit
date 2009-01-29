


#ifndef TILEH
#define TILEH

#include <iostream>
#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include "constantes.h"

class Tile
{
	public:
	Tile();
	~Tile();
	Tile operator=(const Tile &tile);
	void Tile::setTile(coordonnee positionDansLImage,int image,bool collision,int animation,int son,Lumiere lumiere,bool ombre,char orientation,bool transparent,coordonnee centre,float temps);
	coordonnee getCoordonnee();
	bool getCollision();
	int getAnimation();
	int getImage();
	int getSon();
	Lumiere getLumiere();
	bool getOmbre();
	bool getTransparent();
	char getOrientation();
	coordonnee getCentre();
	float getTemps();

	private:
	coordonnee m_positionDansLImage,m_centre;
	bool m_collision,m_ombre,m_transparent;
	int m_animation,m_son,m_image;
	char m_orientation; // Orientation du tile, utilisé pour le calcul des lumières
	Lumiere m_lumiere;

	float m_temps;

};

#endif


