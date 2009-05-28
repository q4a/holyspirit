

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





#ifndef DECORH
#define DECORH

#include <iostream>
#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "../constantes.h"

#include "../Entites/objet.h"
#include "../Moteurs/light.h"


class Decor
{
public:
    Decor(){}
    Decor(int tileset,int tile,std::vector<int> evenement,int monstre,int herbe, int couche,int hauteur);
    Decor(int tileset,int tile,std::vector<int> evenement,int monstre,int herbe, int couche,int hauteur,std::vector <Objet> objets);
    ~Decor();
    Decor operator=(const Decor &Decor);

    void AjouterObjet(Objet);

    int AfficherTexteObjets(coordonnee positionn,int);
    bool AfficherTexteObjet(coordonnee position,int);

    void DecrementerAnimation(float nombre);
    void AugmenterAnimation(float temps);

    int getTile(); // Prendre le numéro du tile du décor
    int getTileset(); // Prendre le numéro du tileset du décor
    int getPosition(char type); // Prendre la position du décor
    std::vector<int> getEvenement(); // Prendre le numéro de l'événement du décor
    int getMonstre(); // Retourne l'ID du monstre sur la case
    int getProjectile();
    int getEffetGraphique();
    int getCouche();
    int getHerbe(); // Retourne le numero de l'herbe du décor
    int getNumeroHerbe();
    int getTailleHerbe();
    int getHauteur();
    sf::Color getCouleurHerbe();
    coordonnee getDecalageHerbe();
    Objet *getObjet(int numero);
    void supprimerObjet(int numero);
    std::vector<Objet> getObjets();
    std::vector<Objet> *getPointeurObjets();
    int getNombreObjets();
    float getAnimation();


    void setEvenement(int evenement,int numero);
    void ajouterEvenement(int evenement);
    void setTileset( int tileset);
    void setTile(int tile);
    void setCouche(int couche);
    void setDecor(int tileset,int tile,std::vector<int> evenement,int monstre,int herbe,int couche,int hauteur);
    void setDecor(int tileset,int tile,std::vector<int> evenement,int monstre,int herbe,int couche,int hauteur,std::vector <Objet> objets); //Définir une valeur au décor
    void setNumeroHerbe(int numero);
    void setMonstre(int monstre);
    void setProjectile(int projectile);
    void setEffetGraphique(int effet);


    Light_Entity m_light;

    sf::Color color;

    sf::Sprite m_sprite,m_spriteHerbe,m_spriteOmbre;

private:
    int m_tileset,m_tile,m_monstre,m_projectile,m_effet,m_couche;
    std::vector<int> m_evenement;
    float m_animation;
    coordonnee m_position;

    std::vector<Objet> m_objets;

    int m_herbe,m_numeroHerbe;
    int m_herbe_taille;
    int m_hauteur;
    coordonnee m_herbe_decalage;
    sf::Color m_herbe_couleur;
};

#endif


