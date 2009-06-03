

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

struct benediction
{
    int type;
    int info1;
    int info2;
};

struct Requirement
{
    int force;
    int dexterite;
    int vitalite;
    int piete;
    int charisme;
};

class Objet
{
public:
    Objet();
    Objet(std::string nom,int rarete);
    ~Objet();

     Objet operator=(const Objet &objet);

    int AfficherCaracteristiques(coordonnee position,Caracteristique caract,float modPrix = 1, bool compare = false, bool = false, bool = false);
    void Charger(std::string chemin,bool NePasAjouterBenedictions=false);
    void ChargerCaracteristiques(std::ifstream *fichier);

    void Sauvegarder(std::ofstream *fichier);
    void SauvegarderTexte(std::ofstream *fichier);
    void ChargerTexte(std::ifstream *fichier,bool NePasAjouterBenedictions=false);
    void ChargerChemin(std::ifstream *fichier);

    void Generer(int bonus);

    bool Utilisable(Caracteristique caract,int IDClasse);

    void JouerSon();

    std::string getChemin();
    std::string getNom();
    int getRarete();
    int getImage();
    int getChanceTrouver();
    int getPrix();
    coordonnee getPosition();
    coordonnee getTaille();
    coordonnee getPositionImage();

    void setRarete(int);
    void setPosition(int, int);
    void setChanceTrouver(int);

    int m_type,m_equipe;
    std::vector <int> m_emplacement;
    std::vector <int> m_emplacementImpossible;
    std::vector <int> m_IDClasse;

    int m_degatsMin, m_degatsMax,m_armure;
    int m_capaciteBenediction;
    int m_prix;
    bool m_shoot_weapon,m_useMiracle;

    Requirement m_requirement;

    std::vector <int> m_emplacementImageHero;
    std::vector <std::string> m_cheminImageHero;

    std::vector <benediction> m_benedictions;

    sf::Color m_color;

    Miracle m_miracle;

private:
    sf::String AjouterCaracteristiqueAfficher(coordonnee position,coordonnee *decalage,coordonnee *tailleCadran, const char *chaine,sf::Color color=sf::Color(255,255,255));

    std::string m_nom,m_chemin;
    std::vector <std::string> m_description;
    int m_rarete,m_image,m_son,m_chanceTrouver;
    coordonnee m_positionImage,m_taille,m_position;
    int ai,aa,dii,dia,dai,daa;
    int m_img_corner;
};

#endif
