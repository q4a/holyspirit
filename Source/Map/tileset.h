

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





#ifndef TILESETH
#define TILESETH

#include <iostream>
#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

#include "tile.h"

class Tileset
{
	public:
	Tileset();
	Tileset(std::string chemin);
	~Tileset();
	Tileset operator=(const Tileset &tileset);

	bool Charger(std::string chemin);

	void JouerSon(int numeroSon,coordonnee position,coordonnee positionHero);
	void DeleteTiles();

	int getImage(int tile);
	const coordonnee &getPositionDuTile(int tile);
	bool getCollisionTile(int tile);
	int getAnimationTile(int tile);
	int getSonTile(int tile);
	const Lumiere &getLumiereDuTile(int tile);
	bool getOmbreDuTile(int tile);
	bool getReflectionDuTile(int tile);
	bool getTransparentDuTile(int tile);
	char getOrientationDuTile(int tile);
	const coordonnee &getCentreDuTile(int tile);
	const std::string &getChemin(){return m_chemin;}
	float getTempsDuTile(int tile);
	int getOpacityDuTile(int tile);
	int getTaille();

	int getMinimap(int tile);
	const coordonnee &getPositionMinimap(int tile);

	private:
	std::vector <int> m_image;
	std::vector <Tile> m_tile;
	std::vector <int> m_sons;
	std::string m_chemin;
};

class Herbe : public Tileset
{
    private:

};

#endif


