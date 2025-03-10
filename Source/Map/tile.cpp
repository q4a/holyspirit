

/*Copyright (C) 2009 Naisse Gr�goire

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


#include "tile.h"

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

void Tile::setTile( coordonnee positionDansLImage,int image,bool collision,int animation,std::vector<int> son,Lumiere lumiere, coordonnee lumiere_decal,bool ombre,
                    bool reflection,char orientation,bool transparent,coordonnee centre,float temps, float tempsRand, int opacity, int layer,
                    int attaque, int ordre, int angle, int ambientShadow)
{
    m_positionDansLImage    = positionDansLImage;
    m_image                 = image;
    m_collision             = collision;
    m_animation             = animation;
    m_son                   = son;
    m_lumiere               = lumiere;
    m_ombre                 = ombre;
    m_reflection            = reflection;
    m_orientation           = orientation;
    m_transparent           = transparent;
    m_opacity               = opacity;

    m_lumiere_decal         = lumiere_decal;

    m_temps                 = temps;
    m_tempsRand             = tempsRand;

    m_centre                = centre;

    m_layer                 = layer;
    m_ordre                 = ordre;
    m_attaque               = attaque;

    m_tileMinimap           = -1;

    m_angle                 = angle;
    m_ambientShadow         = ambientShadow;
}

const coordonnee &Tile::getCoordonnee()
{
    return m_positionDansLImage;
}
int Tile::getImage()
{
    return m_image;
}
bool Tile::getCollision()
{
    return m_collision;
}
int Tile::getAnimation()
{
    return m_animation;
}
std::vector<int> Tile::getSon()
{
    return m_son;
}
const Lumiere &Tile::getLumiere()
{
    return m_lumiere;
}
bool Tile::getOmbre()
{
    return m_ombre;
}
bool Tile::getReflection()
{
    return m_reflection;
}
bool Tile::getTransparent()
{
    return m_transparent;
}
char Tile::getOrientation()
{
    return m_orientation;
}
const coordonnee &Tile::getCentre()
{
    return m_centre;
}
const coordonnee &Tile::getLumiereDecal()
{
    return m_lumiere_decal;
}
float Tile::getTemps()
{
    return m_temps;
}
float Tile::getTempsRand()
{
    return m_tempsRand;
}
int Tile::getOpacity()
{
    return m_opacity;
}
int Tile::getLayer()
{
    return m_layer;
}
int Tile::getOrdre()
{
    return m_ordre;
}
int Tile::getAttaque()
{
    return m_attaque;
}
int Tile::getAngle()
{
    return m_angle;
}
int Tile::getAmbientShadow()
{
    return m_ambientShadow;
}

void Tile::setImage(int image)
{
    m_image=image;
}
void Tile::setSon(int son)
{
    m_son.push_back(son);//son;
}

