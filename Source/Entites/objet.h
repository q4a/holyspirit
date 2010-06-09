

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



#include<iostream>
#include "../constantes.h"
#include <fstream>

#ifndef OBJETH
#define OBJETH

#include "miracle.h"

sf::Color GetItemColor(int rarete);

enum {L_TYPE,L_EMPLACEMENT,L_NOM};

struct benediction
{
    int type;
    int info1;
    int info2;
    int info3;
};

struct Requirement
{
    int force;
    int dexterite;
    int vitalite;
    int piete;
    int charisme;
};

struct Ingredient
{
    Ingredient()
    {
        nom = "";
        nombre = 0;
        text_nom = "";
    }
    std::string nom;
    std::string text_nom;
    int nombre;
};

struct ConditionLitanie
{
    int type;
    int valeur;
    std::string valeur_string;
};

void ChargerMiracleBenediction(benediction &bene, Miracle &miracle, bool &m_useMiracle);


class Set
{
    public:
    void Charger(std::string chemin, Caracteristique caract);

    std::string m_nom;
    std::string m_chemin;

    std::vector<std::vector<benediction> > m_benedictions;
    std::vector<std::string> m_items;

    std::vector<Miracle> m_miracle;
    std::vector<bool> m_useMiracle;
    std::vector< std::vector<std::string> > m_chemin_miracles;

    int m_nombre;
};

class Objet
{
public:
    Objet();
    Objet(std::string nom,int rarete);
    ~Objet();

    void Afficher(coordonnee position);
    int AfficherCaracteristiques(coordonnee position,Caracteristique caract, std::vector<Objet> *items, std::string nom_classe,float modPrix = 1, bool compare = false, bool = false, bool = false, bool = false, bool = false);
    void Charger(const std::string &chemin, const Caracteristique &caract,bool NePasAjouterBenedictions=false);
    void ChargerCaracteristiques(std::ifstream *fichier);

    void Sauvegarder(std::ofstream *fichier);
    void SauvegarderTexte(std::ofstream *fichier);
    void ChargerTexte(std::ifstream *fichier, const Caracteristique &caract ,bool NePasAjouterBenedictions=false);
    void ChargerChemin(std::ifstream *fichier);

    void ChargerMiracle(const Caracteristique &caract);
    int AddBenediction(benediction );

    void Generer(int bonus);

    void CalculerGolem();

    bool Utilisable(const Caracteristique &caract,std::string IDClasse);

    void JouerSon();

    const std::string &getChemin();
    const std::string &getNom();
    int getRarete();
    int getImage();
    float getChanceTrouver();
    int getPrix();
    coordonnee getPosition();
    coordonnee getTaille();
    coordonnee getPositionImage();

    void setRarete(int);
    void setPosition(int, int);
    void setChanceTrouver(float);

    int m_type,m_equipe;
    std::vector <int> m_emplacement;
    std::vector <int> m_emplacementImpossible;
    std::vector <std::string> m_IDClasse;
    std::vector <int> m_nom_IDClasse;

    int m_degatsMin, m_degatsMax,m_armure, m_vie;
    int m_capaciteBenediction;
    int m_prix;
    bool m_shoot_weapon,m_useMiracle;
    bool m_dejaTrie;
    float m_hauteur;
    bool m_monter;
    float m_rotation;

    Requirement m_requirement;

    std::vector <int> m_emplacementImageHero;
    std::vector <std::string> m_cheminImageHero;

    std::vector <benediction> m_benedictions;

    Miracle m_miracle;
    std::vector<std::string> m_chemin_miracles;

    std::vector<Miracle> m_miracles_benedictions;
    std::vector<ConditionLitanie> m_conditions;

    float m_alpha;

    std::vector<Ingredient> m_craft_ingredients;
    std::string             m_craft_result;

    std::string             m_chemin_set;
    Set                     m_set;

    int                     m_nbr_bless;

    Caracteristique         m_gol_caract;

private:
    sf::Text AjouterCaracteristiqueAfficher(coordonnee position,coordonnee *decalage,coordonnee *tailleCadran, const char *chaine,sf::Color color=sf::Color(255,255,255));

    std::string m_nom,m_chemin;
    std::vector <std::string> m_description;
    int m_rarete,m_image,m_son;
    float m_chanceTrouver;
    coordonnee m_positionImage,m_taille,m_position;
    int ai,aa,dii,dia,dai,daa,bi,ba,vi,va;
};

#endif
