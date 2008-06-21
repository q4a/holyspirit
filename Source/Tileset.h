///**************************************************\\\
||| Dernière modification : Le 06/02/08 par Gregouar |||
///**************************************************///




#ifndef TILESETH
#define TILESETH

#include <iostream>
#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

#include "tile.h"
#include "Hero.h"

class Tileset
{
	public:
	Tileset();
	Tileset(std::string chemin);
	~Tileset();
	Tileset operator=(const Tileset &tileset);
	bool Charger(std::string chemin);
	sf::Image* getImage();
	coordonnee getPositionDuTile(int tile);
	bool getCollisionTile(int tile);
	int getAnimationTile(int tile);
	int getSonTile(int tile);
	Lumiere Tileset::getLumiereDuTile(int tile);
	bool getOmbreDuTile(int tile);
	char getOrientationDuTile(int tile);

	int getTaille();

	void jouerSon(int numeroSon,double distance,coordonnee position,coordonnee positionHero);
	void remiseAZeroDesSons();

	void deleteTiles();

	private:
	std::vector <bool> m_sonAZero;
	sf::Image m_image;
	std::vector <Tile> m_tile;
	std::vector <sf::SoundBuffer> m_buffer;
	std::vector <sf::Sound> m_sons;
};

#endif

