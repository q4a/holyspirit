

/*Copyright (C) 2009 Naisse Gr�goire

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

class Objet
{
public:
    Objet();
    Objet(std::string nom,int rarete);
    ~Objet();

    int AfficherCaracteristiques(coordonnee position,Caracteristique caract,float modPrix = 1, bool compare = false, bool = false, bool = false);
    void Charger(const std::string &chemin, const Caracteristique &caract,bool NePasAjouterBenedictions=false);
    void ChargerCaracteristiques(std::ifstream *fichier);

    void Sauvegarder(std::ofstream *fichier);
    void SauvegarderTexte(std::ofstream *fichier);
    void ChargerTexte(std::ifstream *fichier, const Caracteristique &caract ,bool NePasAjouterBenedictions=false);
    void ChargerChemin(std::ifstream *fichier);

    void ChargerMiracle(const Caracteristique &caract);

    void Generer(int bonus);

    bool Utilisable(const Caracteristique &caract,int IDClasse);

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
    std::vector <int> m_IDClasse;

    int m_degatsMin, m_degatsMax,m_armure;
    int m_capaciteBenediction;
    int m_prix;
    bool m_shoot_weapon,m_useMiracle;
    bool m_dejaTrie;

    Requirement m_requirement;

    std::vector <int> m_emplacementImageHero;
    std::vector <std::string> m_cheminImageHero;

    std::vector <benediction> m_benedictions;

    sf::Color m_color;

    Miracle m_miracle;
    std::vector<std::string> m_chemin_miracles;

    float m_alpha;

    std::vector<Ingredient> m_craft_ingredients;
    std::string             m_craft_result;

private:
    sf::Text AjouterCaracteristiqueAfficher(coordonnee position,coordonnee *decalage,coordonnee *tailleCadran, const char *chaine,sf::Color color=sf::Color(255,255,255));

    std::string m_nom,m_chemin;
    std::vector <std::string> m_description;
    int m_rarete,m_image,m_son;
    float m_chanceTrouver;
    coordonnee m_positionImage,m_taille,m_position;
    int ai,aa,dii,dia,dai,daa;
};

#endif
