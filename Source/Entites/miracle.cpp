

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

Projectile::Projectile()
{
    m_entite_cible  = NULL;
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

        if(sprite.GetPosition().x + sprite.GetSize().x  >= moteurGraphique->m_camera.GetRect().Left
        && sprite.GetPosition().x                       <  moteurGraphique->m_camera.GetRect().Right
        && sprite.GetPosition().y + sprite.GetSize().y  >= moteurGraphique->m_camera.GetRect().Top
        && sprite.GetPosition().y                       <  moteurGraphique->m_camera.GetRect().Bottom)
            moteurGraphique->AjouterCommande(&sprite,10,1);
    }
}

void EffetGraphique::Animer(float temps)
{
    if (m_actif)
    {
        float tempsAnimation = m_tiles[m_tileEnCours].getTemps();
        m_animation += temps;

        while (m_animation >= tempsAnimation && m_actif)
        {
            m_tileEnCours = m_tiles[m_tileEnCours].getAnimation();

            m_animation-=tempsAnimation;

            tempsAnimation = m_tiles[m_tileEnCours].getTemps();

            if (m_compteur > 0 || m_compteur == -100)
            {
                if (m_compteur != -100)
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

Miracle::Miracle(std::string chemin, const Caracteristique &caract, int level)
{
    Charger(chemin, caract, level);
}

Miracle::~Miracle()
{
    m_sons.clear();
    for (int i=0;i<(int)m_tile.size();i++)
        m_tile[i].clear();
    m_tile.clear();
    m_image.clear();
}

float ChargerEquation(ifstream &fichier, const Caracteristique &caract, int level, char priorite, bool *cont);

float getValeur(ifstream &fichier, char caractere)
{
    int temp = 0;
    std::vector<char> buf;
    do
    {
        buf.push_back(caractere);
        fichier.get(caractere);
    }
    while (caractere >= '0' && caractere <= '9');

    for (unsigned i = 0 ; i < buf.size() ; i++ )
    {
        int val = 0;
        if (buf[i] == '0')
            val = 0;
        else if (buf[i] == '1')
            val = 1;
        else if (buf[i] == '2')
            val = 2;
        else if (buf[i] == '3')
            val = 3;
        else if (buf[i] == '4')
            val = 4;
        else if (buf[i] == '5')
            val = 5;
        else if (buf[i] == '6')
            val = 6;
        else if (buf[i] == '7')
            val = 7;
        else if (buf[i] == '8')
            val = 8;
        else if (buf[i] == '9')
            val = 9;

        int exposant = 1;
        for (unsigned j = 0 ; j < (buf.size() - i - 1) ; ++j)
            exposant *= 10;

        temp += val * exposant;
    }
    return temp;
}

float lireValeur(ifstream &fichier, std::vector<float> &valeurs)
{
    char    caractere;
    float   val;

    fichier.get(caractere);
    if(caractere == '%')
    {
        fichier.get(caractere);
        int nbr = (int)getValeur(fichier, caractere);

        if(nbr >= 0 && nbr < (int)valeurs.size())
            val = (int)valeurs[nbr];
    }
    else
    {
        fichier.putback(caractere);
        fichier>>val;
    }

    return val;
}

float ChargerEquation(ifstream &fichier, const Caracteristique &caract, int level, char priorite, bool *continuer)
{
    float valeur = 0;
    char caractere;

    do
    {
        fichier.get(caractere);

        if (caractere == '+')
        {
             if(priorite == '+')
             {
                valeur += ChargerEquation(fichier, caract, level, '+', continuer);
                if(!*continuer)
                    return valeur;
                else
                    *continuer = false;
             }
             else
             {
                 fichier.putback(caractere);
                *continuer = true;
                 return valeur;
             }
        }
        else if (caractere == '-')
        {
             if(priorite == '+')
             {
                valeur -= ChargerEquation(fichier, caract, level, '+', continuer);
                if(!*continuer)
                    return valeur;
                else
                    *continuer = false;
             }
             else
             {
                 fichier.putback(caractere);
                *continuer = true;
                 return valeur;
             }
        }
        else if (caractere == '*')
        {
             if(priorite == '*' || priorite == '+')
             {
                valeur *= ChargerEquation(fichier, caract, level, '*', continuer);
                if(!*continuer)
                    return valeur;
                else
                    *continuer = false;
             }
             else
             {
                 fichier.putback(caractere);
                *continuer = true;
             }
        }
        else if (caractere == '/')
        {
             if(priorite == '*' || priorite == '+')
             {
                valeur /= ChargerEquation(fichier, caract, level, '*', continuer);
                if(!*continuer)
                    return valeur;
                else
                    *continuer = false;
             }
             else
             {
                 fichier.putback(caractere);
                *continuer = true;
             }
        }
        else if (caractere == '^')
        {
             if( priorite == '^' || priorite == '*' || priorite == '+')
             {
                float temp = ChargerEquation(fichier, caract, level, '^', continuer);
                float buf = valeur;

                for(int i = 1 ; i < (int)temp ; ++i )
                    valeur *= buf;

                if(!*continuer)
                    return valeur;
                else
                    *continuer = false;
             }
             else
             {
                 fichier.putback(caractere);
                *continuer = true;
             }
        }

        else if (caractere == 'l')
            valeur = level;

        else if (caractere == 'i')
            valeur = caract.degatsMin;
        else if (caractere == 'a')
            valeur = caract.degatsMax;

        else if (caractere == 'b')
            valeur = caract.armure;

        else if (caractere == '(')
            valeur = ChargerEquation(fichier, caract, level, '+', continuer);

        else if (caractere >= '0' && caractere <= '9')
            valeur = getValeur(fichier, caractere);

    }
    while (caractere!='$' && caractere!=')');
    return valeur;
}

void Miracle::Charger(std::string chemin, const Caracteristique &caract, int level)
{
    console->Ajouter("Chargement du miracle : "+chemin);

    m_unique     =  false;
    m_chemin     =  chemin;
    m_cas        =  -1;
    m_reserveFoi =  0;
    m_coutFoi    =  0;
    m_reserveVie =  0;
    m_coutVie    =  0;

    m_level      = level;

    std::vector<float> valeurs;

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
                int no;
                fichier>>no;
                m_nom = configuration->getText(6, no);
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
                int no;
                fichier>>no;
                description = configuration->getText(6, no);
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


        do
        {
            fichier.get(caractere);
            if (caractere=='*')
            {
                bool continuer = false;
                valeurs.push_back(ChargerEquation(fichier, caract, level, '+', &continuer));
            }

            if (fichier.eof())
            {
                console->Ajouter("Erreur : Miracle \" "+chemin+" \" Invalide",1);
                caractere='$';
            }
        }
        while (caractere!='$');

        description = "";

        do
        {
            fichier.get(caractere);
            if (caractere=='*')
            {
                m_description_effets.push_back(string ());

                int no;
                fichier>>no;
                m_description_effets.back() = configuration->getText(6, no);

                for (int i=0;i<(int)m_description_effets.back().size();i++)
                {
                    if (m_description_effets.back()[i]=='_')
                        m_description_effets.back()[i]=' ';
                    else if (m_description_effets.back()[i]=='\\')
                    {
                        std::ostringstream buf2;

                        for(int j = 0 ; j < i ; ++j)
                            buf2 << m_description_effets.back()[j];
                        for(int j = i + 1 ; j < (int)m_description_effets.back().size() ; ++j)
                            buf2 << m_description_effets.back()[j];

                        m_description_effets.back() = buf2.str();
                    }
                    else if (m_description_effets.back()[i]=='%')
                    {
                        ++i;
                        int buf = 0;
                        int nbr = 0;
                        while(i + buf < (int)m_description_effets.back().size())
                        {
                            if(m_description_effets.back()[i + buf] >= '0' && m_description_effets.back()[i + buf] <= '9')
                                buf++;
                            else
                                break;
                        }

                        for(int j = 0 ; j < buf ; ++j)
                        {
                            int val = 0;
                            if (m_description_effets.back()[i + j] == '0')
                                val = 0;
                            else if (m_description_effets.back()[i + j] == '1')
                                val = 1;
                            else if (m_description_effets.back()[i + j] == '2')
                                val = 2;
                            else if (m_description_effets.back()[i + j] == '3')
                                val = 3;
                            else if (m_description_effets.back()[i + j] == '4')
                                val = 4;
                            else if (m_description_effets.back()[i + j] == '5')
                                val = 5;
                            else if (m_description_effets.back()[i + j] == '6')
                                val = 6;
                            else if (m_description_effets.back()[i + j] == '7')
                                val = 7;
                            else if (m_description_effets.back()[i + j] == '8')
                                val = 8;
                            else if (m_description_effets.back()[i + j] == '9')
                                val = 9;
                            for (int k = 0 ; k < (buf-j-1) ; ++k)
                                val *= 10;

                            nbr  += val;
                        }

                        if(nbr >= 0 && nbr < (int)valeurs.size())
                        {
                            std::ostringstream buf2;

                            for(int j = 0 ; j < i-1 ; ++j)
                                buf2 << m_description_effets.back()[j];
                            buf2 << (int)valeurs[nbr];
                            for(int j = i+buf ; j < (int)m_description_effets.back().size() ; ++j)
                                buf2 << m_description_effets.back()[j];

                            m_description_effets.back() = buf2.str();
                            i = -1;
                        }
                    }
                }
            }

            if (fichier.eof())
            {
                console->Ajouter("Erreur : Miracle \" "+chemin+" \" Invalide",1);
                caractere='$';
            }
        }
        while (caractere!='$');

        if(level == 0)
            m_description_effets.clear();

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
                        m_coutFoi = (int)lireValeur(fichier, valeurs);
                        break;

                    case 'F':
                        m_reserveFoi = (int)lireValeur(fichier, valeurs);
                        break;

                    case 'v':
                        m_coutVie = (int)lireValeur(fichier, valeurs);
                        break;

                    case 'V':
                        m_reserveVie = (int)lireValeur(fichier, valeurs);
                        break;

                    case 'u':
                        fichier>>m_unique;
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
                        m_effets.back().m_type = (int)lireValeur(fichier, valeurs);
                        break;

                    case 'l':
                        int buffer;
                        buffer = (int)lireValeur(fichier, valeurs);
                        m_effets.back().m_lien.push_back(buffer);
                        break;

                    case 'm':
                        fichier>>m_effets.back().m_chaine;
                        break;

                    case 's':
                        m_effets.back().m_sequence = (int)lireValeur(fichier, valeurs);
                        break;

                    case 'i':
                        fichier.get(caractere);
                        if (caractere=='a')
                            m_effets.back().m_informations[0] = (int)lireValeur(fichier, valeurs);
                        if (caractere=='b')
                            m_effets.back().m_informations[1] = (int)lireValeur(fichier, valeurs);
                        if (caractere=='c')
                            m_effets.back().m_informations[2] = (int)lireValeur(fichier, valeurs);
                        if (caractere=='d')
                            m_effets.back().m_informations[3] = (int)lireValeur(fichier, valeurs);
                        if (caractere=='e')
                            m_effets.back().m_informations[4] = (int)lireValeur(fichier, valeurs);
                        if (caractere=='f')
                            m_effets.back().m_informations[5] = (int)lireValeur(fichier, valeurs);
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
                                position.x = (int)lireValeur(fichier, valeurs);
                                break;
                            case 'y':
                                position.y = (int)lireValeur(fichier, valeurs);
                                break;
                            case 'w':
                                position.w = (int)lireValeur(fichier, valeurs);
                                break;
                            case 'h':
                                position.h = (int)lireValeur(fichier, valeurs);
                                break;
                            case 'i':
                                image = (int)lireValeur(fichier, valeurs);
                                break;
                            case 'c':
                                collision = (int)lireValeur(fichier, valeurs);
                                break;
                            case 'a':
                                animation = (int)lireValeur(fichier, valeurs);
                                break;
                            case 's':
                                son = (int)lireValeur(fichier, valeurs);
                                break;
                            case 'o':
                                ombre = (int)lireValeur(fichier, valeurs);
                                break;
                            case 't':
                                transparent = (int)lireValeur(fichier, valeurs);
                                break;
                            case 'n':
                                tempsAnimation = lireValeur(fichier, valeurs);
                                break;
                            case 'p':
                                opacity = (int)lireValeur(fichier, valeurs);
                                break;

                            case 'e':
                                fichier.get(caractere);
                                if (caractere=='y')
                                    centre.y = (int)lireValeur(fichier, valeurs);
                                if (caractere=='x')
                                    centre.x = (int)lireValeur(fichier, valeurs);
                                break;

                            case 'l':
                                fichier.get(caractere);
                                if (caractere=='r')
                                    lumiere.rouge = (int)lireValeur(fichier, valeurs);
                                if (caractere=='v')
                                    lumiere.vert = (int)lireValeur(fichier, valeurs);
                                if (caractere=='b')
                                    lumiere.bleu = (int)lireValeur(fichier, valeurs);
                                if (caractere=='i')
                                    lumiere.intensite = (int)lireValeur(fichier, valeurs);
                                if (caractere=='h')
                                    lumiere.hauteur = (int)lireValeur(fichier, valeurs);
                                break;
                            case 'r':
                                orientation = (int)lireValeur(fichier, valeurs);
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
                        m_tile.back().back().setTile(position,image,collision,animation,son,lumiere,ombre,0,orientation,transparent,centre,tempsAnimation,opacity);

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

void Miracle::Concatenencer(std::string chemin, const Caracteristique &caract, int level)
{
    Miracle miracle(chemin, caract, level) ;
    m_effets.back().m_lien.push_back((int)m_effets.size());

    int tailleEffets    = m_effets.size();
    int tailleSon       = m_sons.size();
    int tailleTile      = m_tile.size();

    for (int i=0;i<(int)miracle.m_tile.size();i++)
    {
        m_tile.push_back(miracle.m_tile[i]);
        for (int j=0;j<(int)m_tile.back().size();j++)
        {
            m_tile.back()[j].setImage(m_tile.back()[j].getImage());
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
        if(m_effets.back().m_type == AURA || m_effets.back().m_type == ZONE)
            m_effets.back().m_informations[0] += tailleEffets;
        if(m_effets.back().m_type == DECLENCHEUR)
            m_effets.back().m_informations[3] += tailleEffets;
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

    for (int i = 0 ; i < (int)miracle.m_description_effets.size() ; ++i)
        m_description_effets.push_back(miracle.m_description_effets[i]);
}



sf::Text Miracle::AjouterCaracteristiqueAfficher(coordonnee position,coordonnee *decalage,coordonnee *tailleCadran, const char *chaine,sf::Color color)
{
    sf::Text string;

    string.SetFont(moteurGraphique->m_font);

    string.SetColor(color);

    string.SetSize(12.f*(int)(configuration->Resolution.h/600));
    string.SetString(chaine);

    if (tailleCadran->x<((int)string.GetRect().Right-(int)string.GetRect().Left))
        tailleCadran->x=((int)string.GetRect().Right-(int)string.GetRect().Left);

    decalage->y+=(int)string.GetRect().Bottom-(int)string.GetRect().Top+5;

    return string;
}


void Miracle::AfficherDescription(coordonnee position, bool suivant)
{
    std::vector <sf::Text> temp;

    sf::Sprite sprite;
    sf::Text string;

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

    if(m_level > 0)
    {
        if (m_coutFoi > 0)
        {
            std::ostringstream buf;
            buf<<configuration->getText(0,29)<<m_coutFoi;
            temp.push_back(AjouterCaracteristiqueAfficher(position,&decalage,&tailleCadran,buf.str().c_str()));
        }
        if (m_reserveFoi > 0)
        {
            std::ostringstream buf;
            buf<<configuration->getText(0,30)<<m_reserveFoi;
            temp.push_back(AjouterCaracteristiqueAfficher(position,&decalage,&tailleCadran,buf.str().c_str()));
        }
        if (m_coutVie > 0)
        {
            std::ostringstream buf;
            buf<<configuration->getText(0,31)<<m_coutVie;
            temp.push_back(AjouterCaracteristiqueAfficher(position,&decalage,&tailleCadran,buf.str().c_str()));
        }
        if (m_reserveVie > 0)
        {
            std::ostringstream buf;
            buf<<configuration->getText(0,32)<<m_reserveVie;
            temp.push_back(AjouterCaracteristiqueAfficher(position,&decalage,&tailleCadran,buf.str().c_str()));
        }
    }

    temp.push_back(AjouterCaracteristiqueAfficher(position,&decalage,&tailleCadran,""));
    temp.push_back(AjouterCaracteristiqueAfficher(position,&decalage,&tailleCadran,""));

    if (suivant)
    {
        temp.push_back(AjouterCaracteristiqueAfficher(position,&decalage,&tailleCadran,"Niveau suivant :"));
        temp.push_back(AjouterCaracteristiqueAfficher(position,&decalage,&tailleCadran,""));

        for (int i=0;i<(int)m_description_effets_suivant.size();i++)
            temp.push_back(AjouterCaracteristiqueAfficher(position,&decalage,&tailleCadran,m_description_effets_suivant[i].c_str()));

        if (m_coutFoi_suivant > 0)
        {
            std::ostringstream buf;
            buf<<configuration->getText(0,29)<<m_coutFoi_suivant;
            temp.push_back(AjouterCaracteristiqueAfficher(position,&decalage,&tailleCadran,buf.str().c_str()));
        }
        if (m_reserveFoi_suivant > 0)
        {
            std::ostringstream buf;
            buf<<configuration->getText(0,30)<<m_reserveFoi_suivant;
            temp.push_back(AjouterCaracteristiqueAfficher(position,&decalage,&tailleCadran,buf.str().c_str()));
        }
        if (m_coutVie_suivant > 0)
        {
            std::ostringstream buf;
            buf<<configuration->getText(0,31)<<m_coutVie_suivant;
            temp.push_back(AjouterCaracteristiqueAfficher(position,&decalage,&tailleCadran,buf.str().c_str()));
        }
        if (m_reserveVie_suivant > 0)
        {
            std::ostringstream buf;
            buf<<configuration->getText(0,32)<<m_reserveVie_suivant;
            temp.push_back(AjouterCaracteristiqueAfficher(position,&decalage,&tailleCadran,buf.str().c_str()));
        }
    }

    if (position.x-10<0)
        position.x=10;

    if (position.y+decalage.y+20>configuration->Resolution.h)
        position.y=configuration->Resolution.h-decalage.y-20;

    if (position.x+tailleCadran.x+20>configuration->Resolution.w)
        position.x=configuration->Resolution.w-tailleCadran.x-20;

    int decalY=0;
    for (int i=0;i<(int)temp.size();i++)
    {
        temp[i].SetY((position.y+decalY+10));
        temp[i].SetX(position.x+20+(tailleCadran.x/2-((int)temp[i].GetRect().Right-(int)temp[i].GetRect().Left)/2)/*-tailleCadran.x*/);

        decalY+=(int)temp[i].GetRect().Bottom-(int)temp[i].GetRect().Top+5;

        moteurGraphique->AjouterTexte(&temp[i],19);
    }

    tailleCadran.y=decalage.y;

    tailleCadran.y+=20;
    tailleCadran.x+=20;

    sprite.SetImage(*moteurGraphique->getImage(0));
    sprite.SetColor(sf::Color(0,0,0,248));
    sprite.SetY(position.y);
    sprite.SetX(position.x/*-tailleCadran.x*/+10);
    sprite.Resize(tailleCadran.x,tailleCadran.y);
    moteurGraphique->AjouterCommande(&sprite,19,0);

    sf::Sprite sprite2;

    sprite2.SetImage(*moteurGraphique->getImage(moteurGraphique->m_img_corner));
    sprite2.Resize(16,16);
    sprite2.SetColor(sf::Color(255,255,255,255));
    sprite2.SetY(position.y-2);
    sprite2.SetX(position.x/*-tailleCadran.x*/+10-2);
    moteurGraphique->AjouterCommande(&sprite2,19,0);


    sprite2.SetY(position.y-2);
    sprite2.SetX(position.x+tailleCadran.x+10+2);
    sprite2.SetRotation(270);
    moteurGraphique->AjouterCommande(&sprite2,19,0);

    sprite2.SetY(position.y+tailleCadran.y+2);
    sprite2.SetX(position.x+tailleCadran.x+10+2);
    sprite2.SetRotation(180);
    moteurGraphique->AjouterCommande(&sprite2,19,0);

    sprite2.SetY(position.y+tailleCadran.y+2);
    sprite2.SetX(position.x/*-tailleCadran.x*/+10-2);
    sprite2.SetRotation(90);
    moteurGraphique->AjouterCommande(&sprite2,19,0);


    temp.clear();
}



