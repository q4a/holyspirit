

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
#include "../Moteurs/Entite_graphique.h"


#ifndef MIRACLEH
#define MIRACLEH

class Objet;
class Personnage;

enum  {PROJECTILE = 0,CORPS_A_CORPS = 1,DEGATS = 2,EFFET_GRAPHIQUE = 3,INVOCATION = 4,AURA = 5,EFFET = 6,ZONE = 7,
       REPETITION = 8,CHARME = 9,CHANGEMENT_POSE = 10,EFFET_ECRAN = 11,CHARGE = 12, SOUFFLE = 13, DECLENCHEUR = 14,
       CONDITION = 15, BLOQUER = 16, VARIABLE_MIRACLE = 17};
enum  {AURA_REGENERATION, AURA_DEGATS, AURA_VOL, AURA_CARACTERISTIQUES, AURA_STUNNED, AURA_ARMURE};
enum  {D_PERMANENT, D_FRAPPE, D_TOUCHE, D_ATTAQUE, D_MORT,
                    D_CIBLE_FRAPPE, D_CIBLE_TOUCHE, D_CIBLE_ATTAQUE, D_CIBLE_MORT};
enum  {C_ANGLE,C_CIBLELIFE,C_RANDOM,C_VARIABLE, C_NO_VARIABLE, C_CIBLE};

class EffetGraphique : public Entite_graphique
{
public:
    EffetGraphique();
    void Afficher();
    void Animer(float temps);

    coordonnee m_position;
    coordonnee m_position_case;
    bool m_actif;
    bool m_old_actif;

    int m_compteur;
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
    Projectile();

    void Afficher();
    void Deplacer(float temp);

    bool m_monstre;
    bool m_actif;
    bool m_supprime;
    int  m_transperce;
    //int m_degats;
    coordonneeDecimal m_position,m_vecteur;
    coordonnee m_positionCase;
    float m_rotation,m_rotationReelle;

    EffetGraphique m_effet;
    bool m_dejaDeplace;


    coordonnee m_cible ; //Pour savoir si le projectile doit s'arreter à une case précise ou continuer jusqu'a rencontrer un obstacle, vaut -1 -1 si il doit continuer
    coordonnee m_depart;

    Personnage *m_entite_cible;
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
        m_informations[5]=0;
        m_informations[6]=0;
        m_informations[7]=0;
        m_informations[8]=0;
        m_informations[9]=0;
    }

    ~Effet()
    {
        m_lien.clear();
    }

    std::vector <int> m_lien;
    int m_type;
    int m_sequence;

    int m_informations[10];
    std::string m_chaine;
};

class Miracle
{
public:
    Miracle();
    Miracle(const std::string &chemin, const Caracteristique &caract, int level);
    ~Miracle();

    void Charger(const std::string &chemin, const Caracteristique &caract, int level);
    void Concatenencer(const std::string &chemin, const Caracteristique &caract, int level);
    void RechargerTileset();

    void AfficherDescription(coordonnee position, bool suivant = true);
    sf::Text AjouterCaracteristiqueAfficher(coordonnee position,coordonnee *decalage,coordonnee *tailleCadran, const char *chaine,sf::Color color=sf::Color(255,255,255));

    std::vector <int>           m_tileset;

    std::vector <Effet>         m_effets;

    std::string                 m_chemin;
    std::vector<std::string>    m_chemin_concatene;

    int                         m_coutFoi;
    int                         m_reserveFoi;
    int                         m_coutFoi_suivant;
    int                         m_reserveFoi_suivant;

    int                         m_coutVie;
    int                         m_reserveVie;
    int                         m_coutVie_suivant;
    int                         m_reserveVie_suivant;

    std::string                 m_nom;
    std::vector<std::string>    m_description;
    std::vector<std::string>    m_description_effets;
    std::vector<std::string>    m_description_effets_suivant;

    bool                        m_unique;
    bool                        m_direct;
    bool                        m_golem;
    bool                        m_miracleArme;
    int                         m_cas;
    float                       m_cooldown;
    float                       m_cur_time;
    int                         m_consommer;


    int                         m_buf;
    int                         m_level;
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
        m_informations[5]=0;

        m_cible = NULL;
    }

    int                 m_effetEnCours;
    int                 m_IDObjet;
    float               m_informations[6];

    coordonneeDecimal   m_position;
    Personnage         *m_cible;
};

class EntiteMiracle
{
public:
    EntiteMiracle();

    std::vector<InfosEntiteMiracle*>    m_infos;
    int                                 m_modele;
    coordonnee                          m_coordonneeCible;
    coordonnee                          m_coordonneeDepart;
    bool                                m_dejaConsommeFoi;
    Objet                               *m_source;

    bool                                m_forced_maj;
};

#endif
