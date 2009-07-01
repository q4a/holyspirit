

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



#include "../constantes.h"
#include "../Map/tile.h"
#include "../Moteurs/light.h"


#ifndef MIRACLEH
#define MIRACLEH

class Personnage;

enum  {PROJECTILE,CORPS_A_CORPS,DEGATS,EFFET_GRAPHIQUE,INVOCATION,AURA,SOIN,M_EXPLOSION,REPETITION};

class Projectile
{
public:
    void Afficher(coordonnee position);
    void Deplacer(float temp);

    bool m_monstre,m_actif;
    int m_degats;
    coordonneeDecimal m_position,m_vecteur;
    coordonnee m_positionCase;
    float m_rotation,m_rotationReelle;

    int m_image;
    coordonnee m_positionImage;
    coordonnee m_centre;

    Lumiere m_lumiere;

    float m_animation;
    int m_couche;

    bool m_dejaDeplace;

    Light_Entity m_light;
};

class EffetGraphique
{
public:
    void Afficher(coordonnee position);

    bool m_actif;
    coordonneeDecimal m_position;

    int m_image;
    int m_compteur;
    coordonnee m_positionImage;
    coordonnee m_centre;

    Lumiere m_lumiere;

    float m_animation;
    int m_couche;

    Light_Entity m_light;
};

struct Effet
{
    Effet()
    {
        m_type=0;
        m_sequence=0;

        m_informations[0]=0;
        m_informations[1]=0;
        m_informations[2]=0;
        m_informations[3]=0;
        m_informations[4]=0;
    }

    ~Effet()
    {
        m_lien.clear();
    }

    std::vector <int> m_lien;
    int m_type;
    int m_sequence;

    int m_informations[5];
    std::string m_chaine;
};

class Miracle
{
public:
    Miracle();
    Miracle(std::string chemin);
    ~Miracle();

    void Charger(std::string chemin);
    void Concatenencer(std::string chemin);
    void JouerSon(int numeroSon,coordonnee position,coordonnee positionHero);


    std::vector < std::vector <Tile> > m_tile;
    std::vector <Effet> m_effets;

    std::vector <int> m_image;
    std::vector <int> m_sons;
    std::string m_chemin;

    int m_coutFoi;
};

struct InfosEntiteMiracle
{
    InfosEntiteMiracle()
    {
        m_effetEnCours=0;
        m_imageEnCours=0;
        m_IDObjet=-1;

        m_position.x=0;
        m_position.y=0;
        m_position.w=0;
        m_position.h=0;

        m_informations[0]=0;
        m_informations[1]=0;
        m_informations[2]=0;
        m_informations[3]=0;
        m_informations[4]=0;
    }

    int m_effetEnCours;
    int m_imageEnCours;
    int m_IDObjet;
    float m_informations[5];

    coordonneeDecimal m_position;
};

class EntiteMiracle
{
public:
    EntiteMiracle(){ m_cible = NULL; }
    std::vector<InfosEntiteMiracle> m_infos;
    int m_modele;
    Personnage *m_cible;
};

#endif
