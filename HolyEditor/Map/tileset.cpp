

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
}
Tileset::Tileset(string chemin)
{
    Charger(chemin);
}
Tileset::~Tileset()
{
    /* if(m_sonAZero.size()>0)
     {
         m_sonAZero.clear();
         m_buffer.clear();
         //m_sons.clear();
     }*/
    m_sons.clear();
    m_tile.clear();

    m_image.clear();
}

Tileset Tileset::operator=(const Tileset &tileset)
{
    m_image = tileset.m_image;
    m_tile=tileset.m_tile;

    return *this;
}

bool Tileset::Charger(std::string chemin)
{
    string cheminFinal,temp;
    m_chemin=chemin;
    /*cheminFinal=chemin+".png";
    m_image=moteurGraphique->AjouterImage(cheminFinal);*/

    /*int nombreTiles=0;
    cheminFinal=chemin+".ts.hs";*/


    ifstream fichier;
    fichier.open(m_chemin.c_str(), ios::in);
    if (fichier)
    {
        /*fichier>>temp;
        if(temp=="*NoAntiCrenelage")
            m_image.SetSmooth(false);
        else
            m_image.SetSmooth(true);*/

        char caractere;

        do
        {
            fichier.get(caractere);
            if (caractere=='*')
            {
                string cheminImage;
                fichier>>cheminImage;
                m_image.push_back(moteurGraphique->AjouterImage(cheminImage));
            }
            if (fichier.eof())
            {
                console->Ajouter("Erreur : Tileset \" "+cheminFinal+" \" Invalide",1);
                caractere='$';
            }
        }
        while (caractere!='$');

        do
        {
            fichier.get(caractere);
            if (caractere=='*')
            {
                string cheminDuSon;
                fichier>>cheminDuSon;
                m_sons.push_back(moteurSons->AjouterBuffer(cheminDuSon));
            }
            if (fichier.eof())
            {
                console->Ajouter("Erreur : Tileset \" "+cheminFinal+" \" Invalide",1);
                caractere='$';
            }
        }
        while (caractere!='$');

        do
        {
            fichier.get(caractere);
            if (caractere=='*')
            {
                coordonnee position,centre(-100,-100,-1,-1), coordMinimap(0,0,0,0);
                int animation=m_tile.size(),son=-1,image=0,imageMM = 0;
                Lumiere lumiere;
                lumiere.intensite=0;
                lumiere.hauteur=0;
                bool collision=0,ombre=0,transparent=0;
                char orientation=' ';
                float tempsAnimation=0.075;
                int opacity = 255;

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
                    case 't':
                        fichier>>transparent;
                        break;
                    case 'n':
                        fichier>>tempsAnimation;
                        break;
                    case 'p':
                        fichier>>opacity;
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
                    }
                    if (fichier.eof())
                    {
                        console->Ajouter("Erreur : Tileset \" "+cheminFinal+" \" Invalide",1);
                        caractere='$';
                    }
                }
                while (caractere!='$');
                //AjouterTile(position,collision,animation,son,lumiere,ombre,orientation);

                if (centre.x==-100)
                    centre.x=position.w/2;
                if (centre.y==-100)
                    centre.y=position.h-32;

                Tile tileTemp;
                m_tile.push_back(tileTemp);
                m_tile.back().setTile(position,image,collision,animation,son,lumiere,ombre,orientation,transparent,centre,tempsAnimation,opacity);

                fichier.get(caractere);
            }
            if (fichier.eof())
            {
                console->Ajouter("Erreur : Tileset \" "+cheminFinal+" \" Invalide",1);
                caractere='$';
            }
        }
        while (caractere!='$');
    }
    else
        console->Ajouter("Impossible d'ouvrir le fichier : "+cheminFinal,1);

    fichier.close();
    return 1;
}

int Tileset::getImage(int tile)
{
    if (tile>=0&&tile<(int)m_tile.size())
        if (m_tile[tile].getImage()>=0&&m_tile[tile].getImage()<(int)m_image.size())
            return m_image[m_tile[tile].getImage()];

    return 0;
}

const coordonnee &Tileset::getPositionDuTile(int tile)
{
    if (tile>=0&&tile<(int)m_tile.size())
        return m_tile[tile].getCoordonnee();
    else
        return m_tile[0].getCoordonnee();
}

bool Tileset::getCollisionTile(int tile)
{
    if (tile>=0&&tile<(int)m_tile.size())
        return m_tile[tile].getCollision();

    return 0;
}

int Tileset::getAnimationTile(int tile)
{
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

int Tileset::getTaille()
{
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

bool Tileset::getTransparentDuTile(int tile)
{
    if (tile>=0&&tile<(int)m_tile.size())
        return m_tile[tile].getTransparent();

    return 0;
}
float Tileset::getTempsDuTile(int tile)
{
    if (tile>=0&&tile<(int)m_tile.size())
        return m_tile[tile].getTemps();

    return 0;
}

int Tileset::getOpacityDuTile(int tile)
{
    if (tile>=0&&tile<(int)m_tile.size())
        return m_tile[tile].getOpacity();

    return 255;
}




char Tileset::getOrientationDuTile(int tile)
{
    if (tile>=0&&tile<(int)m_tile.size())
        return m_tile[tile].getOrientation();
    return 0;
}

const coordonnee &Tileset::getCentreDuTile(int tile)
{
    if (tile>=0&&tile<(int)m_tile.size())
        return m_tile[tile].getCentre();
    else
        return m_tile[0].getCentre();;
}

void Tileset::JouerSon(int numeroSon,coordonnee position,coordonnee positionHero)
{
    if (numeroSon>=0&&numeroSon<(int)m_sons.size())
    {
        coordonnee pos;
        pos.x=-position.x;
        pos.y=position.y;

        moteurSons->JouerSon(m_sons[numeroSon],pos,positionHero,1);
    }
}

void Tileset::DeleteTiles()
{
    m_tile.clear();
    m_sons.clear();
}



