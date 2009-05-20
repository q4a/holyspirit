
#ifndef CLASSEH
#define CLASSEH

#include "miracle.h"

struct Image_interface
{
    coordonnee position;
    int image;
};

struct Emplacement_inventaire
{
    coordonnee position;
    int emplacement;
};

struct Classe
{
    void Charger(std::string chemin);

    int ID;

    std::string nom;
    Caracteristique caracteristique;


    std::string chemin_modele;

    std::vector<std::string> equipementParDefaut;
    std::string modeleNu[3];

    Image_interface inventaire;
    Image_interface menu_marchand;
    Image_interface hud;
    Image_interface orbe_vie;
    Image_interface orbe_foi;
    Image_interface cache_vie;
    Image_interface cache_foi;

    coordonnee position_sac_inventaire;
    coordonnee position_contenu_inventaire;
    coordonnee position_contenu_marchand;

    std::vector <Emplacement_inventaire> emplacements;

    std::vector<Miracle> miracle;
};
#endif
