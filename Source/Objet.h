
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

    void Sauvegarder(std::ofstream *fichier);

    std::string getNom();
    int getRarete();
    int getImage();
    coordonnee getPosition();
    coordonnee getTaille();
    coordonnee getPositionImage();

    void setRarete(int);
    void setPosition(int, int);

    protected:
    std::string m_nom,m_chemin;
    int m_rarete,m_image;
    coordonnee m_positionImage,m_taille,m_position;
};

class ModeleObjet : public Objet
{
    public:
    void Charger(std::string chemin);
    void setChanceTrouver(int);

    int getChanceTrouver();

    private:
    int m_chanceTrouver;
};

#endif
