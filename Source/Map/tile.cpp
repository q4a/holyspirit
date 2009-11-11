

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

Tile Tile::operator=(const Tile &tile)
{
    m_positionDansLImage=tile.m_positionDansLImage;
    m_image=tile.m_image;
    m_collision=tile.m_collision;
    m_animation=tile.m_animation;
    m_son=tile.m_son;
    m_lumiere=tile.m_lumiere;
    m_ombre=tile.m_ombre;
    m_orientation=tile.m_orientation;
    m_transparent=tile.m_transparent;
    m_centre=tile.m_centre;
    m_temps=tile.m_temps;
    return *this;
}

void Tile::setTile(coordonnee positionDansLImage,int image,bool collision,int animation,int son,Lumiere lumiere,bool ombre,char orientation,bool transparent,coordonnee centre,float temps, int opacity)
{
    m_positionDansLImage    = positionDansLImage;
    m_image                 = image;
    m_collision             = collision;
    m_animation             = animation;
    m_son                   = son;
    m_lumiere               = lumiere;
    m_ombre                 = ombre;
    m_orientation           = orientation;
    m_transparent           = transparent;
    m_opacity               = opacity;

    m_temps                 = temps;

    m_centre                = centre;

    m_tileMinimap           = -1;

    if (m_centre.x==-1)
        m_centre.x=positionDansLImage.w/2;
    if (m_centre.y==-1)
        m_centre.y=positionDansLImage.h-32;
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
int Tile::getSon()
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
float Tile::getTemps()
{
    return m_temps;
}
int Tile::getOpacity()
{
    return m_opacity;
}

void Tile::setImage(int image)
{
    m_image=image;
}
void Tile::setSon(int son)
{
    m_son=son;
}

