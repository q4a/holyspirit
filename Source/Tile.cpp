#include "Tile.h"

#include <iostream>
#include <fstream>

#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>

using namespace std;

Tile::Tile()
{
}
Tile::~Tile()
{
}

Tile Tile::operator=(const Tile &tile)
{
	m_positionDansLImage=tile.m_positionDansLImage;
	m_collision=tile.m_collision;
	m_animation=tile.m_animation;
	m_son=tile.m_son;
	m_lumiere=tile.m_lumiere;
	m_ombre=tile.m_ombre;
	m_orientation=tile.m_orientation;
    return *this;
}

void Tile::setTile(coordonnee positionDansLImage,bool collision,int animation,int son,Lumiere lumiere,bool ombre,char orientation)
{
	m_positionDansLImage=positionDansLImage;
	m_collision=collision;
	m_animation=animation;
	m_son=son;
	m_lumiere=lumiere;
	m_ombre=ombre;
	m_orientation=orientation;
}

coordonnee Tile::getCoordonnee(){return m_positionDansLImage;}
bool Tile::getCollision(){return m_collision;}
int Tile::getAnimation(){return m_animation;}
int Tile::getSon(){return m_son;}
Lumiere Tile::getLumiere(){return m_lumiere;}
bool Tile::getOmbre(){return m_ombre;}
char Tile::getOrientation(){return m_orientation;}

