
#include<iostream>
#include "constantes.h"
#include <fstream>

#ifndef OBJETH
#define OBJETH

struct benediction
{
    int type;
    int info1;
    int info2;
};

class Objet
{
    public:
    Objet();
    Objet(std::string nom,int rarete);
    ~Objet();

    void AfficherCaracteristiques(sf::RenderWindow *ecran,coordonnee position);
    void Charger(std::string chemin);
    void ChargerCaracteristiques(std::ifstream *fichier);
    void setChanceTrouver(int);
    void Sauvegarder(std::ofstream *fichier);

    void Generer();

    std::string getNom();
    int getRarete();
    int getImage();
    int getChanceTrouver();
    coordonnee getPosition();
    coordonnee getTaille();
    coordonnee getPositionImage();

    void setRarete(int);
    void setPosition(int, int);

    int m_type,m_equipe;
    std::vector <int> m_emplacement;

    int m_degatsMin, m_degatsMax,m_armure;
    int m_capaciteBenediction;

    std::vector <benediction> m_benedictions;

    sf::Color m_color;

    private:
    sf::String AjouterCaracteristiqueAfficher(coordonnee position,coordonnee *decalage,coordonnee *tailleCadran, char *chaine);

    std::string m_nom,m_chemin;
    std::vector <std::string> m_description;
    int m_rarete,m_image,m_chanceTrouver;
    coordonnee m_positionImage,m_taille,m_position;
    int ai,aa,dii,dia,dai,daa;


};

#endif
