

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


#include "tileset.h"

#include "../globale.h"

#include <iostream>
#include <fstream>

#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>

using namespace std;
using namespace sf;

Tileset::Tileset()
{
    option_forcedShadow     = false;
    option_forcedReflect    = false;
}
Tileset::Tileset(const std::string &chemin)
{
    option_forcedShadow     = false;
    option_forcedReflect    = false;
    Charger(chemin);
}
Tileset::Tileset(ifstream &fichier)
{
    option_forcedShadow     = false;
    option_forcedReflect    = false;
    Charger(fichier);
}
Tileset::~Tileset()
{
    m_sons.clear();
    m_tile.clear();

    m_image.clear();
}

/*Tileset Tileset::operator=(const Tileset &tileset)
{
    m_image = tileset.m_image;
    m_tile=tileset.m_tile;

    return *this;
}*/

void Tileset::ChargerTiles(ifstream &fichier, int lumiere_base)
{
    if (fichier)
    {
        char caractere;

        do
        {
            fichier.get(caractere);
            if (caractere=='*')
            {
                ChargerInfosTile(fichier,lumiere_base);
            }
            if (fichier.eof())
            {
                console->Ajouter("Erreur : Tileset \" "+m_chemin+" \" Invalide",1);
                caractere='$';
            }
        }
        while (caractere!='$');
    }
    else
        console->Ajouter("Impossible d'ouvrir le fichier : "+m_chemin,1);
}

void Tileset::ChargerInfosTile(ifstream &fichier, int lumiere_base,int type)
{
    coordonnee position,centre(-100,-100,-100,-100), coordMinimap(0,0,0,0);
    int animation=m_tile.size(),son=-1,image=0,imageMM = 0;
    Lumiere lumiere;
    lumiere.intensite=lumiere_base;
    lumiere.rouge = -1;
    lumiere.vert = -1;
    lumiere.bleu = -1;
    lumiere.hauteur=0;
    bool collision=0,ombre=option_forcedShadow, reflection = option_forcedReflect,transparent=0;
    char orientation=' ';
    float tempsAnimation=0.075;
    int opacity = 255;
    int layer = 0;
    int attaque = -1;
    int ordre = 0;
    int angle = 0;
    std::string nom;
    int distortionTile = -1;
    std::vector <int> shadowmapTile;
    int ambientShadow = -1;


    char caractere;
    do
    {
        fichier.get(caractere);
        switch (caractere)
        {
        case 'x':
            fichier>>position.x;
            break;
        case 'y':
            fichier>>position.y;
            break;
        case 'w':
            fichier>>position.w;
            break;
        case 'h':
            fichier>>position.h;
            break;
        case 'i':
            fichier>>image;
            break;
        case 'c':
            fichier>>collision;
            break;
        case 'a':
            fichier>>animation;
            break;
        case 's':
            fichier>>son;
            break;
        case 'o':
            fichier>>ombre;
            break;
        case 'f':
            fichier>>reflection;
            break;
        case 't':
            fichier>>transparent;
            break;
        case 'n':
            fichier>>tempsAnimation;
            break;
        case 'p':
            fichier>>opacity;
            break;
        case 'd':
            fichier>>attaque;
            break;
        case 'g':
            fichier>>ordre;
            break;
        case 'q':
            fichier>>ambientShadow;
            break;

        case 'v':

            fichier>>nom;
            break;

        case 'e':
            fichier.get(caractere);
            if (caractere=='y')
                fichier>>centre.y;
            if (caractere=='x')
                fichier>>centre.x;
            break;

        case 'l':
            fichier.get(caractere);
            if (caractere=='r')
                fichier>>lumiere.rouge;
            if (caractere=='v')
                fichier>>lumiere.vert;
            if (caractere=='b')
                fichier>>lumiere.bleu;
            if (caractere=='i')
                fichier>>lumiere.intensite;
            if (caractere=='h')
                fichier>>lumiere.hauteur;
            break;
        case 'r':
            fichier>>orientation;
            break;
        case 'u':
            fichier>>layer;
            break;
        case 'b':
            fichier>>angle;
            break;

        case 'm':
            do
            {
                fichier.get(caractere);
                switch (caractere)
                {
                    case 'x':
                        fichier>>coordMinimap.x;
                        break;
                    case 'y':
                        fichier>>coordMinimap.y;
                        break;
                    case 'w':
                        fichier>>coordMinimap.w;
                        break;
                    case 'h':
                        fichier>>coordMinimap.h;
                        break;
                    case 'i':
                        fichier>>imageMM;
                        break;
                }
            }
            while (caractere!='$');
            fichier.get(caractere);
            break;

        case 'j':
            ChargerInfosTile(fichier, lumiere_base, 1);
            distortionTile = m_tile_distortion.size() - 1;
            break;

        case 'k':
            ChargerInfosTile(fichier, lumiere_base, 2);
            shadowmapTile.push_back(m_tile_shadowmap.size() - 1);
            break;

        }
        if (fichier.eof())
        {
            console->Ajouter("Erreur : Tileset \" "+m_chemin+" \" Invalide",1);
            caractere='$';
        }
    }
    while (caractere!='$');
    //AjouterTile(position,collision,animation,son,lumiere,ombre,orientation);

    if (centre.x==-100)
        centre.x=position.w/2;
    if (centre.y==-100)
        centre.y=position.h-32;

    if(type == 1)
    {
        m_tile_distortion.push_back(Tile ());
        m_tile_distortion.back().setTile(position,image,collision,animation,son,lumiere,ombre, reflection,orientation,transparent,centre,tempsAnimation,opacity, layer, attaque, ordre, angle,ambientShadow);
    }
    else if(type == 2)
    {
        m_tile_shadowmap.push_back(Tile ());
        m_tile_shadowmap.back().setTile(position,image,collision,animation,son,lumiere,ombre, reflection,orientation,transparent,centre,tempsAnimation,opacity, layer, attaque, ordre, angle,ambientShadow);
    }
    else
    {
        m_tile.push_back(Tile ());
        m_tile.back().setTile(position,image,collision,animation,son,lumiere,ombre, reflection,orientation,transparent,centre,tempsAnimation,opacity, layer, attaque, ordre, angle,ambientShadow);
        m_tile.back().m_tileMinimap = imageMM;
        m_tile.back().m_coordMinimap = coordMinimap;
        m_tile.back().m_distortion = distortionTile;
        m_tile.back().m_shadowMap = shadowmapTile;
    }
}

void Tileset::Charger(ifstream &fichier, int lumiere_base, cDAT *reader)
{
    if (fichier)
    {
        char caractere;

        do
        {
            fichier.get(caractere);
            if (caractere=='*')
            {
                string cheminImage;
                fichier>>cheminImage;
                if(reader == NULL)
                    m_image.push_back(moteurGraphique->AjouterImage(cheminImage));
                else
                    m_image.push_back(moteurGraphique->AjouterImage(reader->GetFile(cheminImage), reader->GetFileSize(cheminImage), cheminImage));

                m_image_chemin.push_back(cheminImage);
            }
            if (fichier.eof())
            {
                console->Ajouter("Erreur : Tileset \" "+m_chemin+" \" Invalide",1);
                caractere='$';
            }
        }
        while (caractere!='$');

        do
        {
            fichier.get(caractere);
            if (caractere=='*')
            {
                do
                {
                    fichier.get(caractere);
                    if (caractere=='m')
                    {
                        string cheminSon;
                        fichier>>cheminSon;
                        m_sons.push_back(moteurSons->AjouterBuffer(cheminSon));
                    }
                    else if (caractere=='n')
                    {
                        int temp;
                        fichier>>temp;
                        if(temp >= (int)m_sonsSpecial.size())
                            m_sonsSpecial.resize(temp + 1);

                        m_sonsSpecial[temp].push_back(m_sons.size()-1);
                    }
                    else if (caractere=='u')
                    {
                        bool temp;
                        fichier>>temp;
                        if(!m_sons.empty())
                            m_sons.back().unique = temp;
                    }
                    if (fichier.eof())
                    {
                        console->Ajouter("Erreur : Monstre \" "+m_chemin+" \" Invalide",1);
                        caractere='$';
                    }
                }
                while (caractere!='$');

                fichier.get(caractere);
            }
            if (fichier.eof())
            {
                console->Ajouter("Erreur : Tileset \" "+m_chemin+" \" Invalide",1);
                caractere='$';
            }
        }
        while (caractere!='$');


        ChargerTiles(fichier,lumiere_base);
    }
    else
        console->Ajouter("Impossible d'ouvrir le fichier : "+m_chemin,1);
}

void Tileset::Charger(const std::string &chemin)
{
    m_chemin=chemin;

    ifstream fichier;
    fichier.open(m_chemin.c_str(), ios::in);
    Charger(fichier);
    fichier.close();
}

int Tileset::getImage(int tile,int type)
{
    if(type == 1)
    {
        if (tile>=0&&tile<(int)m_tile_distortion.size())
            if (m_tile_distortion[tile].getImage()>=0&&m_tile_distortion[tile].getImage()<(int)m_image.size())
                return m_image[m_tile_distortion[tile].getImage()];
    }
    else
    {
        if (tile>=0&&tile<(int)m_tile.size())
            if (m_tile[tile].getImage()>=0&&m_tile[tile].getImage()<(int)m_image.size())
                return m_image[m_tile[tile].getImage()];
    }

    return 0;
}

int Tileset::getImageShadowmap(int tile,int no)
{
    if (tile>=0&&tile<(int)m_tile.size())
    {
        if (no>=0&&no<(int)m_tile[tile].m_shadowMap.size())
        if (m_tile_shadowmap[m_tile[tile].m_shadowMap[no]].getImage()>=0
          &&m_tile_shadowmap[m_tile[tile].m_shadowMap[no]].getImage()<(int)m_image.size())
            return m_image[m_tile_shadowmap[m_tile[tile].m_shadowMap[no]].getImage()];
    }


    return 0;
}

const coordonnee &Tileset::getPositionDuTile(int tile,int type)
{
    if(type == 1)
    {
        if (tile>=0&&tile<(int)m_tile_distortion.size())
            return m_tile_distortion[tile].getCoordonnee();
        else
            return m_tile_distortion[0].getCoordonnee();
    }
    else
    {
        if (tile>=0&&tile<(int)m_tile.size())
            return m_tile[tile].getCoordonnee();
        else
            return m_tile[0].getCoordonnee();
    }
}

bool Tileset::getCollisionTile(int tile)
{
    if (tile>=0&&tile<(int)m_tile.size())
        return m_tile[tile].getCollision();

    return 0;
}

int Tileset::getAnimationTile(int tile,int type)
{
    if(type == 1)
    {
        if (tile>=0&&tile<(int)m_tile_distortion.size())
            return m_tile_distortion[tile].getAnimation();
    }
    else
        if (tile>=0&&tile<(int)m_tile.size())
            return m_tile[tile].getAnimation();

    return 0;
}


int Tileset::getSonTile(int tile)
{
    if (tile>=0&&tile<(int)m_tile.size())
        return m_tile[tile].getSon();
    return 0;
}

int Tileset::getTaille(int type)
{
    if(type == 1)
        return m_tile_distortion.size();
    else
        return m_tile.size();
}

const Lumiere &Tileset::getLumiereDuTile(int tile)
{
    if (tile>=0&&tile<(int)m_tile.size())
        return m_tile[tile].getLumiere();

    return m_tile[0].getLumiere();
}

bool Tileset::getOmbreDuTile(int tile)
{
    if (tile>=0&&tile<(int)m_tile.size())
        return m_tile[tile].getOmbre();

    return 0;
}

bool Tileset::getReflectionDuTile(int tile)
{
    if (tile>=0&&tile<(int)m_tile.size())
        return m_tile[tile].getReflection();

    return 0;
}

bool Tileset::getTransparentDuTile(int tile)
{
    if (tile>=0&&tile<(int)m_tile.size())
        return m_tile[tile].getTransparent();

    return 0;
}
float Tileset::getTempsDuTile(int tile,int type)
{
    if(type == 1)
    {
        if (tile>=0&&tile<(int)m_tile_distortion.size())
            return m_tile_distortion[tile].getTemps();
    }
    else
    {
        if (tile>=0&&tile<(int)m_tile.size())
            return m_tile[tile].getTemps();
    }

    return 0;
}

int Tileset::getOpacityDuTile(int tile,int type)
{
    if(type == 1)
    {
        if (tile>=0&&tile<(int)m_tile_distortion.size())
            return m_tile_distortion[tile].getOpacity();
    }
    else
    {
        if (tile>=0&&tile<(int)m_tile.size())
            return m_tile[tile].getOpacity();
    }

    return 255;
}

int Tileset::getLayerDuTile(int tile)
{
    if (tile>=0&&tile<(int)m_tile.size())
        return m_tile[tile].getLayer();

    return 0;
}

int Tileset::getOrdreDuTile(int tile)
{
    if (tile>=0&&tile<(int)m_tile.size())
        return m_tile[tile].getOrdre();

    return 0;
}

int Tileset::getAttaqueDuTile(int tile)
{
    if (tile>=0&&tile<(int)m_tile.size())
        return m_tile[tile].getAttaque();

    return 0;
}

int Tileset::getDistortionDuTile(int tile)
{
    if (tile>=0&&tile<(int)m_tile.size())
        return m_tile[tile].m_distortion;

    return 0;
}

std::vector <int> Tileset::getShadowmapDuTile(int tile)
{
    if (tile>=0&&tile<(int)m_tile.size())
        return m_tile[tile].m_shadowMap;
}


char Tileset::getOrientationDuTile(int tile)
{
    if (tile>=0&&tile<(int)m_tile.size())
        return m_tile[tile].getOrientation();
    return 0;
}

const coordonnee &Tileset::getCentreDuTile(int tile,int type)
{
    if(type == 1)
    {
        if (tile>=0&&tile<(int)m_tile_distortion.size())
            return m_tile_distortion[tile].getCentre();
        else
            return m_tile_distortion[0].getCentre();
    }
    else
    {
        if (tile>=0&&tile<(int)m_tile.size())
            return m_tile[tile].getCentre();
        else
            return m_tile[0].getCentre();
    }
}

int Tileset::getAmbientShadow(int tile,int type)
{
    if(type == 1)
    {
        if (tile>=0&&tile<(int)m_tile_distortion.size())
            return m_tile_distortion[tile].getAmbientShadow();
        else
            return m_tile_distortion[0].getAmbientShadow();
    }
    else
    {
        if (tile>=0&&tile<(int)m_tile.size())
            return m_tile[tile].getAmbientShadow();
        else
            return m_tile[0].getAmbientShadow();
    }
}

int Tileset::getMinimap(int tile)
{
    if (tile>=0&&tile<(int)m_tile.size())
        if (m_tile[tile].m_tileMinimap>=0&&m_tile[tile].m_tileMinimap<(int)m_image.size())
            return m_image[m_tile[tile].m_tileMinimap];

    return 0;
}

const coordonnee &Tileset::getPositionMinimap(int tile)
{
    if (tile>=0&&tile<(int)m_tile.size())
        return m_tile[tile].m_coordMinimap;
    else
        return m_tile[0].m_coordMinimap;
}

void Tileset::JouerSon(int numeroSon,coordonnee position, bool unique, float volume)
{
    if (numeroSon>=0&&numeroSon<(int)m_sons.size())
    {
        coordonnee pos;
        pos.x=-position.x;
        pos.y=position.y;

        if(m_sons[numeroSon].unique == true)
            unique = true;

        moteurSons->JouerSon(m_sons[numeroSon].no,pos,unique,(int)volume);
    }
}

int getNombreSons();
int getNombreSonsSpecial(int);
int getSonSpecial(int, int);


int Tileset::getNombreSons()
{
    return m_sons.size();
}
int Tileset::getNombreSonsSpecial(int no)
{
    if(m_sonsSpecial.size() > (unsigned)no)
        return m_sonsSpecial[(unsigned)no].size();
    return 0;
}
int Tileset::getSonSpecial(int no, int son)
{
    if(m_sonsSpecial.size() > (unsigned)no)
        if(m_sonsSpecial[(unsigned)no].size() > (unsigned)son)
            return m_sonsSpecial[(unsigned)no][(unsigned)son];
    return -1;
}

void Tileset::DeleteTiles()
{
    m_tile.clear();
    //m_sons.clear();
}

void Tileset::ChargerImages()
{
    for(unsigned i = 0 ; i < m_image_chemin.size() ; ++i)
        moteurGraphique->AjouterImage(m_image_chemin[i]);
}



