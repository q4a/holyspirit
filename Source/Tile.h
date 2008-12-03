///**************************************************\\\
||| Dernière modification : Le 06/02/08 par Gregouar |||
///**************************************************///




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
	void Tile::setTile(coordonnee positionDansLImage,bool collision,int animation,int son,Lumiere lumiere,bool ombre,char orientation,bool transparent);
	coordonnee getCoordonnee();
	bool getCollision();
	int getAnimation();
	int getSon();
	Lumiere getLumiere();
	bool getOmbre();
	bool getTransparent();
	char getOrientation();

	private:
	coordonnee m_positionDansLImage;
	bool m_collision,m_ombre,m_transparent;
	int m_animation,m_son;
	char m_orientation; // Orientation du tile, utilisé pour le calcul des lumières
	Lumiere m_lumiere;

};

#endif


