

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



#include "miracle.h"
#include "../globale.h"

#include <iostream>
#include <fstream>

using namespace std;
using namespace sf;



void Projectile::Afficher(coordonnee position)
{
    if (m_actif)
    {
        Sprite sprite;

        sprite.SetImage(*moteurGraphique->getImage(m_image));
        sprite.SetSubRect(IntRect(m_positionImage.x, m_positionImage.y, m_positionImage.x+m_positionImage.w, m_positionImage.y+m_positionImage.h));

        sprite.FlipX(false);

        sprite.SetX(((m_position.x-m_position.y)*64/COTE_TILE) - (m_centre.x - m_positionImage.w/2) * cos(-m_rotationReelle-M_PI/4));
        sprite.SetY(((m_position.x+m_position.y)*64/COTE_TILE)/2+(64-sprite.GetSize().y)+16);

        sprite.SetOrigin(m_centre.x,m_centre.y);

        sprite.Rotate((-(m_rotation)*180/M_PI));

        if (sprite.GetPosition().x+sprite.GetSize().x>=moteurGraphique->m_camera.GetRect().Left
                &&sprite.GetPosition().x<moteurGraphique->m_camera.GetRect().Right
                &&sprite.GetPosition().y+sprite.GetSize().y>=moteurGraphique->m_camera.GetRect().Top
                &&sprite.GetPosition().y<moteurGraphique->m_camera.GetRect().Bottom)
            moteurGraphique->AjouterCommande(&sprite,10,1);
    }
}

void EffetGraphique::Afficher(coordonnee position)
{
    if (m_actif)
    {
        Sprite sprite;

        sprite.SetImage(*moteurGraphique->getImage(m_image));
        sprite.SetSubRect(IntRect(m_positionImage.x, m_positionImage.y, m_positionImage.x+m_positionImage.w, m_positionImage.y+m_positionImage.h));

        sprite.FlipX(false);

        sprite.SetX(((m_position.x-m_position.y)*64/COTE_TILE)/*-64+(64-sprite.GetSize().x/2)*/);
        sprite.SetY(((m_position.x+m_position.y)*64/COTE_TILE)/2/*+(64-sprite.GetSize().y)+16*/);

        sprite.SetOrigin(m_centre.x,m_centre.y);

        if (sprite.GetPosition().x+sprite.GetSize().x>=moteurGraphique->m_camera.GetRect().Left
                &&sprite.GetPosition().x<moteurGraphique->m_camera.GetRect().Right
                &&sprite.GetPosition().y+sprite.GetSize().y>=moteurGraphique->m_camera.GetRect().Top
                &&sprite.GetPosition().y<moteurGraphique->m_camera.GetRect().Bottom)
            moteurGraphique->AjouterCommande(&sprite,10,1);
    }
}

void Projectile::Deplacer(float temps)
{
    m_position.x+=m_vecteur.x*temps*500;
    m_position.y+=m_vecteur.y*temps*500;

    sf::Vector2f pos;
    pos.x=(((m_position.x-m_position.y)*64/COTE_TILE));
    pos.y=(((m_position.x+m_position.y)*64/COTE_TILE)/2+32)*2;

    moteurGraphique->LightManager->SetPosition(m_light,pos);
}


Miracle::Miracle()
{
}

Miracle::Miracle(std::string chemin)
{
    Charger(chemin);
}

Miracle::~Miracle()
{
    m_sons.clear();
    for (int i=0;i<(int)m_tile.size();i++)
        m_tile[i].clear();
    m_tile.clear();
    m_image.clear();
}


void Miracle::Charger(std::string chemin)
{
    m_chemin=chemin;

    ifstream fichier;
    fichier.open(m_chemin.c_str(), ios::in);
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
                m_image.push_back(moteurGraphique->AjouterImage(cheminImage));
            }
            if (fichier.eof())
            {
                console->Ajouter("Erreur : Miracle \" "+chemin+" \" Invalide",1);
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
                console->Ajouter("Erreur : Miracle \" "+chemin+" \" Invalide",1);
                caractere='$';
            }
        }
        while (caractere!='$');


        do
        {
            fichier.get(caractere);
            if (caractere=='*')
            {
                m_effets.push_back(Effet ());
                do
                {
                    fichier.get(caractere);
                    switch (caractere)
                    {
                    case 't':
                        fichier>>m_effets.back().m_type;
                        break;

                    case 'l':
                        int buffer;
                        fichier>>buffer;
                        m_effets.back().m_lien.push_back(buffer);
                        break;

                    case 'm':
                        fichier>>m_effets.back().m_chaine;
                        break;



                    case 's':
                        fichier>>m_effets.back().m_sequence;
                        break;

                    case 'i':
                        fichier.get(caractere);
                        if (caractere=='a')
                            fichier>>m_effets.back().m_informations[0];
                        if (caractere=='b')
                            fichier>>m_effets.back().m_informations[1];
                        if (caractere=='c')
                            fichier>>m_effets.back().m_informations[2];
                        if (caractere=='d')
                            fichier>>m_effets.back().m_informations[3];
                        if (caractere=='e')
                            fichier>>m_effets.back().m_informations[4];
                        break;
                    }
                    if (fichier.eof())
                    {
                        console->Ajouter("Erreur : Miracle \" "+chemin+" \" Invalide",1);
                        caractere='$';
                    }

                }
                while (caractere!='$');

                fichier.get(caractere);
            }

            if (fichier.eof())
            {
                console->Ajouter("Erreur : Miracle \" "+chemin+" \" Invalide",1);
                caractere='$';
            }
        }
        while (caractere!='$');

        do
        {
            fichier.get(caractere);
            if (caractere=='*')
            {

                m_tile.push_back(vector <Tile> () );

                do
                {
                    fichier.get(caractere);
                    if (caractere=='*')
                    {
                        coordonnee position(-1,-1,-1,-1),centre(-100,-100,-1,-1);
                        int animation=m_tile.size(),son=-1,image=0;
                        Lumiere lumiere;
                        lumiere.intensite=0;
                        lumiere.rouge=0;
                        lumiere.vert=0;
                        lumiere.bleu=0;
                        bool collision=0,ombre=0,transparent=0;
                        char orientation=' ';
                        float tempsAnimation=0.075;

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
                            }
                            if (fichier.eof())
                            {
                                console->Ajouter("Erreur : Miracle \" "+chemin+" \" Invalide",1);
                                caractere='$';
                            }
                        }
                        while (caractere!='$');

                        if(centre.x == -100)
                            centre.x = position.w / 2;
                        if(centre.y == -100)
                            centre.y = position.h - 32;

                        //AjouterTile(position,collision,animation,son,lumiere,ombre,orientation);
                        m_tile.back().push_back(Tile());
                        m_tile.back().back().setTile(position,image,collision,animation,son,lumiere,ombre,orientation,transparent,centre,tempsAnimation);

                        fichier.get(caractere);
                    }
                    if (fichier.eof())
                    {
                        console->Ajouter("Erreur : Miracle \" "+chemin+" \" Invalide",1);
                        caractere='$';
                    }
                }
                while (caractere!='$');
                fichier.get(caractere);
            }
            if (fichier.eof())
            {
                console->Ajouter("Erreur : Miracle \" "+chemin+" \" Invalide",1);
                caractere='$';
            }
        }
        while (caractere!='$');
        fichier.close();
    }
    else
        console->Ajouter("Impossible d'ouvrir le fichier : "+chemin,1);
}


void Miracle::JouerSon(int numeroSon,coordonnee position,coordonnee positionHero)
{
    if (numeroSon>=0&&numeroSon<(int)m_sons.size())
    {
        coordonnee pos;
        pos.x=-position.x;
        pos.y=position.y;

        moteurSons->JouerSon(m_sons[numeroSon],pos,positionHero,0);
    }
}

void Miracle::Concatenencer(std::string chemin)
{
    Miracle miracle(chemin);
    m_effets.back().m_lien.push_back((int)m_effets.size());

    int tailleEffets=m_effets.size();
    int tailleImage=m_image.size();
    int tailleSon=m_sons.size();
    int tailleTile=m_tile.size();

    for(int i=0;i<(int)miracle.m_tile.size();i++)
    {
        m_tile.push_back(miracle.m_tile[i]);
        for(int j=0;j<(int)m_tile.back().size();j++)
        {
            m_tile.back()[j].setImage(m_tile.back()[j].getImage()+tailleImage);
            if(m_tile.back()[j].getSon()!=-1)
                m_tile.back()[j].setSon(m_tile.back()[j].getSon()+tailleSon);
        }
    }

    for(int i=0;i<(int)miracle.m_image.size();i++)
        m_image.push_back(miracle.m_image[i]);
    for(int i=0;i<(int)miracle.m_sons.size();i++)
        m_sons.push_back(miracle.m_sons[i]);

    for(int i=0;i<(int)miracle.m_effets.size();i++)
    {
        m_effets.push_back(miracle.m_effets[i]);
        for(int j=0;j<(int)m_effets.back().m_lien.size();j++)
            m_effets.back().m_lien[j]+=tailleEffets;
        m_effets.back().m_sequence+=tailleTile;
    }

}

