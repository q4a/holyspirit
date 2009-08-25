

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
#include <sstream>

using namespace std;
using namespace sf;



void Projectile::Afficher()
{
    if (m_actif)
        m_effet.Afficher((-(m_rotation)*180/M_PI));
}

EffetGraphique::EffetGraphique()
{
    m_animation     = 0;
    m_tileEnCours   = 0;
    m_actif         = true;
    m_compteur      = -100;
    m_light         = moteurGraphique->LightManager->Add_Dynamic_Light(sf::Vector2f(0,0),0,0,4,sf::Color(0,0,0));
}

void EffetGraphique::Afficher(float rotation)
{
    if (m_actif && m_tileEnCours >= 0 && m_tileEnCours < (int)m_tiles.size())
    {
        Sprite sprite;

        sprite.SetImage(*moteurGraphique->getImage(m_tiles[m_tileEnCours].getImage()));
        sprite.SetSubRect(IntRect(  m_tiles[m_tileEnCours].getCoordonnee().x,
                                    m_tiles[m_tileEnCours].getCoordonnee().y,
                                    m_tiles[m_tileEnCours].getCoordonnee().x + m_tiles[m_tileEnCours].getCoordonnee().w,
                                    m_tiles[m_tileEnCours].getCoordonnee().y + m_tiles[m_tileEnCours].getCoordonnee().h));

        sprite.FlipX(false);

        sprite.SetX(((m_position.x - m_position.y) * 64 / COTE_TILE));
        sprite.SetY(((m_position.x + m_position.y) * 64 / COTE_TILE) * 0.5);

        sprite.SetOrigin(m_tiles[m_tileEnCours].getCentre().x,m_tiles[m_tileEnCours].getCentre().y);

        sprite.Rotate(rotation);

        sprite.SetColor(sf::Color(255,255,255,m_tiles[m_tileEnCours].getOpacity()));

        if (sprite.GetPosition().x + sprite.GetSize().x  >= moteurGraphique->m_camera.GetRect().Left
                && sprite.GetPosition().x                       <  moteurGraphique->m_camera.GetRect().Right
                && sprite.GetPosition().y + sprite.GetSize().y  >= moteurGraphique->m_camera.GetRect().Top
                && sprite.GetPosition().y                       <  moteurGraphique->m_camera.GetRect().Bottom)
            moteurGraphique->AjouterCommande(&sprite,10,1);
    }
}

void EffetGraphique::Animer(float temps)
{
    if(m_actif)
    {
        float tempsAnimation = m_tiles[m_tileEnCours].getTemps();
        m_animation += temps;

        if (m_animation >= tempsAnimation)
        {
            m_tileEnCours = m_tiles[m_tileEnCours].getAnimation();

            m_animation-=tempsAnimation;

            if (m_compteur > 0 || m_compteur == -100)
            {
                if(m_compteur != -100)
                    m_compteur--;

                sf::Vector2f pos;
                pos.x = (m_position.x - m_position.y) * 64 / COTE_TILE;
                pos.y = (m_position.x + m_position.y) * 64 / COTE_TILE;

                moteurGraphique->LightManager->SetPosition  (m_light,pos);
                moteurGraphique->LightManager->SetIntensity (m_light,m_tiles[m_tileEnCours].getLumiere().intensite);
                moteurGraphique->LightManager->SetRadius    (m_light,m_tiles[m_tileEnCours].getLumiere().intensite*5);
                moteurGraphique->LightManager->SetColor     (m_light,
                        sf::Color(  m_tiles[m_tileEnCours].getLumiere().rouge,
                                    m_tiles[m_tileEnCours].getLumiere().vert,
                                    m_tiles[m_tileEnCours].getLumiere().bleu));


                if (m_tiles[m_tileEnCours].getSon() >= 0)
                {
                    coordonnee positionHero;
                    coordonnee position;
                    position.x=-(int)(m_position.x/COTE_TILE - m_position.y/COTE_TILE-1)/5;
                    position.y= (int)(m_position.x/COTE_TILE + m_position.y/COTE_TILE)/5;

                    moteurSons->JouerSon(m_tiles[m_tileEnCours].getSon(), position, positionHero,0);
                }
            }

            moteurGraphique->LightManager->Generate(m_light);

            if (m_compteur <= 0 && m_compteur != -100)
                m_actif=false;
        }
    }
    else
        moteurGraphique->LightManager->Delete_Light(m_light);
}

void Projectile::Deplacer(float temps)
{
    m_position.x += m_vecteur.x*temps*500;
    m_position.y += m_vecteur.y*temps*500;

    sf::Vector2f pos;
    pos.x=(((m_position.x-m_position.y)*64/COTE_TILE));
    pos.y=(((m_position.x+m_position.y)*64/COTE_TILE)/2+32)*2;

    moteurGraphique->LightManager->SetPosition(m_effet.m_light,pos);

    m_effet.m_position = m_position;
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
    console->Ajouter("Chargement du miracle : "+chemin);

    m_max        =  false;
    m_chemin     =  chemin;
    m_cas        =  -1;
    m_reserveFoi =  0;
    m_coutFoi    =  0;
    m_reserveVie =  0;
    m_coutVie    =  0;

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
                fichier>>m_nom;
                for (int i=0;i<(int)m_nom.size();i++)
                    if (m_nom[i]=='_')
                        m_nom[i]=' ';
            }

            if (fichier.eof())
            {
                console->Ajouter("Erreur : Miracle \" "+chemin+" \" Invalide",1);
                caractere='$';
            }
        }
        while (caractere!='$');

        std::string description;

        do
        {
            fichier.get(caractere);
            if (caractere=='*')
            {
                fichier>>description;
                for (int i=0;i<(int)description.size();i++)
                    if (description[i]=='_')
                        description[i]=' ';
            }

            if (fichier.eof())
            {
                console->Ajouter("Erreur : Miracle \" "+chemin+" \" Invalide",1);
                caractere='$';
            }
        }
        while (caractere!='$');

        std::string::size_type stTemp = description.find('\\');

        while (stTemp !=  std::string::npos)
        {
            m_description.push_back(description.substr(0, stTemp)+'\n');
            description = description.substr(stTemp + 1);
            stTemp = description.find('\\');
        }
        m_description.push_back(description.substr(0, stTemp)+'\n');

        description = "";

        do
        {
            fichier.get(caractere);
            if (caractere=='*')
            {
                m_description_effets.push_back(string ());

                fichier>>m_description_effets.back();
                for (int i=0;i<(int)m_description_effets.back().size();i++)
                    if (m_description_effets.back()[i]=='_')
                        m_description_effets.back()[i]=' ';
            }

            if (fichier.eof())
            {
                console->Ajouter("Erreur : Miracle \" "+chemin+" \" Invalide",1);
                caractere='$';
            }
        }
        while (caractere!='$');

        /*if(description.size() > 0)
        {
            stTemp = description.find('\\');

            while (stTemp !=  std::string::npos)
            {
                m_description.push_back(description.substr(0, stTemp)+'\n');
                description = description.substr(stTemp + 1);
                stTemp = description.find('\\');
            }
            m_description_effets.push_back(description.substr(0, stTemp)+'\n');
        }

        description = "";*/

        do
        {
            fichier.get(caractere);
            if (caractere=='*')
            {
                do
                {
                    fichier.get(caractere);
                    switch (caractere)
                    {
                    case 'f':
                        fichier>>m_coutFoi;
                        break;

                    case 'F':
                        fichier>>m_reserveFoi;
                        break;

                    case 'v':
                        fichier>>m_coutVie;
                        break;

                    case 'V':
                        fichier>>m_reserveVie;
                        break;

                    case 'm':
                        fichier>>m_max;
                        break;

                    case 'e':
                        fichier>>m_cas;
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
                        if (caractere=='f')
                            fichier>>m_effets.back().m_informations[5];
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
                            }
                            if (fichier.eof())
                            {
                                console->Ajouter("Erreur : Miracle \" "+chemin+" \" Invalide",1);
                                caractere='$';
                            }
                        }
                        while (caractere!='$');

                        if (centre.x == -100)
                            centre.x = position.w / 2;
                        if (centre.y == -100)
                            centre.y = position.h - 32;

                        if (image >= 0 && image < (int)m_image.size())
                            image = m_image[image];
                        else
                            image = -1;

                        if (son >= 0 && son < (int)m_sons.size())
                            son = m_sons[son];
                        else
                            son = -1;

                        //AjouterTile(position,collision,animation,son,lumiere,ombre,orientation);
                        m_tile.back().push_back(Tile());
                        m_tile.back().back().setTile(position,image,collision,animation,son,lumiere,ombre,orientation,transparent,centre,tempsAnimation,opacity);

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

    int tailleEffets    = m_effets.size();
    int tailleImage     = m_image.size();
    int tailleSon       = m_sons.size();
    int tailleTile      = m_tile.size();

    for (int i=0;i<(int)miracle.m_tile.size();i++)
    {
        m_tile.push_back(miracle.m_tile[i]);
        for (int j=0;j<(int)m_tile.back().size();j++)
        {
            m_tile.back()[j].setImage(m_tile.back()[j].getImage()+tailleImage);
            if (m_tile.back()[j].getSon()!=-1)
                m_tile.back()[j].setSon(m_tile.back()[j].getSon()+tailleSon);
        }
    }

    for (int i=0;i<(int)miracle.m_image.size();i++)
        m_image.push_back(miracle.m_image[i]);
    for (int i=0;i<(int)miracle.m_sons.size();i++)
        m_sons.push_back(miracle.m_sons[i]);

    for (int i=0;i<(int)miracle.m_effets.size();i++)
    {
        m_effets.push_back(miracle.m_effets[i]);
        for (int j=0;j<(int)m_effets.back().m_lien.size();j++)
            m_effets.back().m_lien[j]+=tailleEffets;
        m_effets.back().m_sequence+=tailleTile;
    }

    m_coutFoi               += miracle.m_coutFoi;
    m_reserveFoi            += miracle.m_reserveFoi;
    m_coutFoi_suivant       += miracle.m_coutFoi;
    m_reserveFoi_suivant    += miracle.m_reserveFoi;

    m_coutVie               += miracle.m_coutVie;
    m_reserveVie            += miracle.m_reserveVie;
    m_coutVie_suivant       += miracle.m_coutVie;
    m_reserveVie_suivant    += miracle.m_reserveVie;

    /*m_description.push_back(std::string ());
    m_description.push_back(std::string ("__________________________"));
    m_description.push_back(std::string ());*/

    for(int i = 0 ; i < (int)miracle.m_description_effets.size() ; ++i)
        m_description_effets.push_back(miracle.m_description_effets[i]);
}



sf::String Miracle::AjouterCaracteristiqueAfficher(coordonnee position,coordonnee *decalage,coordonnee *tailleCadran, const char *chaine,sf::Color color)
{
    sf::String string;

    string.SetFont(moteurGraphique->m_font);

    string.SetColor(color);

    string.SetSize(12.f*(int)(configuration->Resolution.h/600));
    string.SetText(chaine);

    if (tailleCadran->x<((int)string.GetRect().Right-(int)string.GetRect().Left))
        tailleCadran->x=((int)string.GetRect().Right-(int)string.GetRect().Left);

    decalage->y+=(int)string.GetRect().Bottom-(int)string.GetRect().Top+5;

    return string;
}


void Miracle::AfficherDescription(coordonnee position, bool suivant)
{
    std::vector <sf::String> temp;

    sf::Sprite sprite;
    sf::String string;

    coordonnee tailleCadran,decalage(-10,0);

    temp.push_back(AjouterCaracteristiqueAfficher(position,&decalage,&tailleCadran,m_nom.c_str()));
    temp.push_back(AjouterCaracteristiqueAfficher(position,&decalage,&tailleCadran,"---------------"));
    temp.push_back(AjouterCaracteristiqueAfficher(position,&decalage,&tailleCadran,""));

    for (int i=0;i<(int)m_description.size() && suivant;i++)
        temp.push_back(AjouterCaracteristiqueAfficher(position,&decalage,&tailleCadran,m_description[i].c_str()));

    temp.push_back(AjouterCaracteristiqueAfficher(position,&decalage,&tailleCadran,""));
    temp.push_back(AjouterCaracteristiqueAfficher(position,&decalage,&tailleCadran,""));

    for (int i=0;i<(int)m_description_effets.size();i++)
        temp.push_back(AjouterCaracteristiqueAfficher(position,&decalage,&tailleCadran,m_description_effets[i].c_str()));

    if(m_coutFoi > 0)
    {
        std::ostringstream buf;
        buf<<"Cout en foi : "<<m_coutFoi;
        temp.push_back(AjouterCaracteristiqueAfficher(position,&decalage,&tailleCadran,buf.str().c_str()));
    }
    if(m_reserveFoi > 0)
    {
        std::ostringstream buf;
        buf<<"Réserve en foi : "<<m_reserveFoi;
        temp.push_back(AjouterCaracteristiqueAfficher(position,&decalage,&tailleCadran,buf.str().c_str()));
    }
    if(m_coutVie > 0)
    {
        std::ostringstream buf;
        buf<<"Cout en vie : "<<m_coutVie;
        temp.push_back(AjouterCaracteristiqueAfficher(position,&decalage,&tailleCadran,buf.str().c_str()));
    }
    if(m_reserveVie > 0)
    {
        std::ostringstream buf;
        buf<<"Réserve en vie : "<<m_reserveVie;
        temp.push_back(AjouterCaracteristiqueAfficher(position,&decalage,&tailleCadran,buf.str().c_str()));
    }

    temp.push_back(AjouterCaracteristiqueAfficher(position,&decalage,&tailleCadran,""));
    temp.push_back(AjouterCaracteristiqueAfficher(position,&decalage,&tailleCadran,""));

    if(!m_max && suivant)
    {
        temp.push_back(AjouterCaracteristiqueAfficher(position,&decalage,&tailleCadran,"Niveau suivant :"));
        temp.push_back(AjouterCaracteristiqueAfficher(position,&decalage,&tailleCadran,""));

        for (int i=0;i<(int)m_description_effets_suivant.size();i++)
            temp.push_back(AjouterCaracteristiqueAfficher(position,&decalage,&tailleCadran,m_description_effets_suivant[i].c_str()));

        if(m_coutFoi_suivant > 0)
        {
            std::ostringstream buf;
            buf<<"Cout en foi : "<<m_coutFoi_suivant;
            temp.push_back(AjouterCaracteristiqueAfficher(position,&decalage,&tailleCadran,buf.str().c_str()));
        }
        if(m_reserveFoi_suivant > 0)
        {
            std::ostringstream buf;
            buf<<"Réserve en foi : "<<m_reserveFoi_suivant;
            temp.push_back(AjouterCaracteristiqueAfficher(position,&decalage,&tailleCadran,buf.str().c_str()));
        }
        if(m_coutVie_suivant > 0)
        {
            std::ostringstream buf;
            buf<<"Cout en vie : "<<m_coutVie_suivant;
            temp.push_back(AjouterCaracteristiqueAfficher(position,&decalage,&tailleCadran,buf.str().c_str()));
        }
        if(m_reserveVie_suivant > 0)
        {
            std::ostringstream buf;
            buf<<"Réserve en vie : "<<m_reserveVie_suivant;
            temp.push_back(AjouterCaracteristiqueAfficher(position,&decalage,&tailleCadran,buf.str().c_str()));
        }
    }

    if (position.x-tailleCadran.x-10<0)
        position.x=tailleCadran.x+10;

    if (position.y+decalage.y+20>configuration->Resolution.h)
        position.y=configuration->Resolution.h-decalage.y-20;

    if (position.x+decalage.x+20>configuration->Resolution.w)
        position.x=configuration->Resolution.w-decalage.x-20;

    int decalY=0;
    for (int i=0;i<(int)temp.size();i++)
    {
        temp[i].SetY((position.y+decalY+10));
        temp[i].SetX(position.x+(tailleCadran.x/2-((int)temp[i].GetRect().Right-(int)temp[i].GetRect().Left)/2)-tailleCadran.x);

        decalY+=(int)temp[i].GetRect().Bottom-(int)temp[i].GetRect().Top+5;

        moteurGraphique->AjouterTexte(&temp[i],19);
    }

    tailleCadran.y=decalage.y;

    tailleCadran.y+=20;
    tailleCadran.x+=20;

    sprite.SetImage(*moteurGraphique->getImage(0));
    sprite.SetColor(sf::Color(0,0,0,248));
    sprite.SetY(position.y);
    sprite.SetX(position.x-tailleCadran.x+10);
    sprite.Resize(tailleCadran.x,tailleCadran.y);
    moteurGraphique->AjouterCommande(&sprite,19,0);

    sf::Sprite sprite2;

    sprite2.SetImage(*moteurGraphique->getImage(moteurGraphique->m_img_corner));
    sprite2.Resize(16,16);
    sprite2.SetColor(sf::Color(255,255,255,255));
    sprite2.SetY(position.y-2);
    sprite2.SetX(position.x-tailleCadran.x+10-2);
    moteurGraphique->AjouterCommande(&sprite2,19,0);


    sprite2.SetY(position.y-2);
    sprite2.SetX(position.x+10+2);
    sprite2.SetRotation(270);
    moteurGraphique->AjouterCommande(&sprite2,19,0);

    sprite2.SetY(position.y+tailleCadran.y+2);
    sprite2.SetX(position.x+10+2);
    sprite2.SetRotation(180);
    moteurGraphique->AjouterCommande(&sprite2,19,0);

    sprite2.SetY(position.y+tailleCadran.y+2);
    sprite2.SetX(position.x+-tailleCadran.x+10-2);
    sprite2.SetRotation(90);
    moteurGraphique->AjouterCommande(&sprite2,19,0);


    temp.clear();
}



