
#include<iostream>
#include "constantes.h"
#include <fstream>

#ifndef OBJETH
#define OBJETH

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

    std::string getNom();
    int getRarete();
    int getImage();
    int getChanceTrouver();
    coordonnee getPosition();
    coordonnee getTaille();
    coordonnee getPositionImage();

    void setRarete(int);
    void setPosition(int, int);

    protected:
    std::string m_nom,m_chemin;
    int m_rarete,m_image,m_chanceTrouver;
    coordonnee m_positionImage,m_taille,m_position;
};


class Arme : public Objet
{
    public:
    void AfficherCaracteristiques(sf::RenderWindow *,coordonnee);
    void ChargerCaracteristiques(std::ifstream *fichier);


    private:
    int m_degatsMin, m_degatsMax;
};

#endif
