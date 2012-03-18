

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

#include <fstream>

#include "tile.h"
#include "../Moteurs/moteurSons.h"


class Tileset
{
	public:
	Tileset();
	Tileset(const std::string &chemin);
	Tileset(std::ifstream &fichier);
	~Tileset();

	void Charger(const std::string &chemin);
	void Charger(std::ifstream &fichier, int lumiere_base = 0, cDAT *reader = NULL);
	void ChargerTiles(std::ifstream &fichier, int lumiere_base = 0);
	void ChargerInfosTile(std::ifstream &fichier, int lumiere_base = 0, int type = 0);
	void ChargerImages();

	bool JouerSon(int numeroSon,coordonnee position, bool unique = true, float volume = 100);
	bool IsPlayingSound(int numeroSon);
	void DeleteTiles();

	int     getImage(int tile, int type = 0);
	int     getImageShadowmap(int tile, int no);
	const   coordonnee &getPositionDuTile(int tile, int type = 0);
	bool    getCollisionTile(int tile);
	int     getAnimationTile(int tile, int type = 0);
	std::vector<int>     getSonTile(int tile);
	const   Lumiere &getLumiereDuTile(int tile);
	bool    getOmbreDuTile(int tile);
	bool    getReflectionDuTile(int tile);
	bool    getTransparentDuTile(int tile);
	char    getOrientationDuTile(int tile);
	const   coordonnee &getCentreDuTile(int tile, int type = 0);
            coordonnee getLumiereDecalDuTile(int tile);
	const   std::string &getChemin(){return m_chemin;}
	float   getTempsDuTile(int tile, int type = 0);
	float   getTempsRandDuTile(int tile, int type = 0);
	int     getOpacityDuTile(int tile, int type = 0);
	int     getLayerDuTile(int tile);
	int     getOrdreDuTile(int tile);
	int     getAttaqueDuTile(int tile);
	int     getAmbientShadow(int tile, int type = 0);
	int     getTaille(int type = 0);
	int     getAngle(int type = 0);
	int     getDistortionDuTile(int tile);
	const  std::vector<int> &getShadowmapDuTile(int tile);

	int getMinimap(int tile);
	const coordonnee &getPositionMinimap(int tile);

	int getNombreSons();
    int getNombreSonsSpecial(int);
    int getSonSpecial(int, int);

	std::string m_chemin;

	bool option_forcedShadow;
    bool option_forcedReflect;

    std::vector <Tile>                  m_tile_shadowmap;

	private:
	std::vector <int>                   m_image;
	std::vector <std::string>           m_image_chemin;
	std::vector <Tile>                  m_tile;
	std::vector <Tile>                  m_tile_distortion;
	std::vector <Infos_son>                   m_sons;
	std::vector <std::vector <int> >    m_sonsSpecial;

	Lumiere             defaultLight;
	std::vector<int>    defaultShadowMap;
	std::vector<int>    defaultSounds;
	coordonnee          defaultCoordonnee;
};

class Herbe : public Tileset
{
    private:

};

#endif


