

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
enum  {AURA_REGENERATION, AURA_DEGATS};
enum  {PHYSIQUE, FEU, CORROSION, FOI};

class EffetGraphique
{
public:
    EffetGraphique();
    void Afficher(float rotation = 0);
    void Animer(float temps);

    bool m_actif;
    coordonneeDecimal m_position;

    std::vector <Tile> m_tiles;

    int m_compteur;
    float m_animation;
    int m_couche;
    int m_tileEnCours;

    Light_Entity m_light;
};

class EffetPersonnage
{
public:
    void Afficher();

    EffetGraphique  m_effet;
    int             m_type;
    float           m_info1;
    float           m_info2;
    float           m_info3;
};


class Projectile
{
public:
    void Afficher();
    void Deplacer(float temp);

    bool m_monstre;
    bool m_actif;
    bool m_supprime;
    int m_degats;
    coordonneeDecimal m_position,m_vecteur;
    coordonnee m_positionCase;
    float m_rotation,m_rotationReelle;

    EffetGraphique m_effet;
    bool m_dejaDeplace;


    coordonnee m_cible ; //Pour savoir si le projectile doit s'arreter à une case précise ou continuer jusqu'a rencontrer un obstacle, vaut -1 -1 si il doit continuer
    coordonnee m_depart;
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

    void AfficherDescription(coordonnee position);
    sf::String AjouterCaracteristiqueAfficher(coordonnee position,coordonnee *decalage,coordonnee *tailleCadran, const char *chaine,sf::Color color=sf::Color(255,255,255));



    std::vector < std::vector <Tile> > m_tile;

    std::vector <Effet>         m_effets;

    std::vector <int>           m_image;
    std::vector <int>           m_sons;
    std::string                 m_chemin;

    int                         m_coutFoi;
    int                         m_reserveFoi;

    int                         m_coutVie;
    int                         m_reserveVie;

    std::string                 m_nom;
    std::vector<std::string>    m_description;

    bool                        m_max;
    int                         m_cas;
};

struct InfosEntiteMiracle
{
    InfosEntiteMiracle()
    {
        m_effetEnCours=0;
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

    int        m_effetEnCours;
    int        m_IDObjet;
    float               m_informations[5];

    coordonneeDecimal   m_position;
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
