

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





#ifndef DECORH
#define DECORH

#include <iostream>
#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "../constantes.h"

#include "../Entites/objet.h"
#include "../Moteurs/Entite_graphique.h"


class Decor
{
public:
    Decor(){}
    Decor(int tileset,int tile,const std::vector<int> &monstre,int herbe, int couche,int hauteur);
    Decor(int tileset,int tile,const std::vector<int> &monstre,int herbe, int couche,int hauteur,std::vector <Objet> objets);
    ~Decor();
    Decor operator=(const Decor &Decor);

    void AjouterObjet(Objet);

    int AfficherTexteObjets(Border &border, coordonnee positionn, bool selectable);
    bool AfficherTexteObjet(Border &border, coordonnee position,int, float *decalage = NULL, bool selectable = true);
    void AlphaObjets(int alpha);

    void AnimerObjets(float temps);

    int getTile(); // Prendre le num�ro du tile du d�cor
    int getTileset(); // Prendre le num�ro du tileset du d�cor
    int getPosition(char type); // Prendre la position du d�cor
    const std::vector<int> &getMonstre(); // Retourne l'ID du monstre sur la case
    const std::vector<int> &getProjectile();
    const std::vector<int> &getEffetGraphique();

    int getCouche();
    int getHerbe(); // Retourne le numero de l'herbe du d�cor
    int getNumeroHerbe();
    int getTailleHerbe();
    int getHauteur();
    const sf::Color &getCouleurHerbe();
    const coordonnee &getDecalageHerbe();
    Objet *getObjet(int numero);
    void supprimerObjet(int numero);
    const std::vector<Objet> &getObjets();
    std::vector<Objet> *getPointeurObjets();
    int getNombreObjets();

    void setTileset( int tileset);
    void setTile(int tile);
    void setCouche(int couche);
    void setDecor(int tileset,int tile,const std::vector<int> &monstre,int herbe,int couche,int hauteur);
    void setDecor(int tileset,int tile,const std::vector<int> &monstre,int herbe,int couche,int hauteur,std::vector <Objet> objets); //D�finir une valeur au d�cor
    void setNumeroHerbe(int numero);

    void setMonstre(int monstre);
    void delMonstre(int monstre);

    void setProjectile(int projectile);
    void delProjectile(int monstre);

    void setEffetGraphique(int effet);
    void delEffetGraphique(int monstre);

    sf::Color color;

    sf::Sprite /*m_sprite,m_spriteHerbe,m_spriteOmbre,*/m_spriteMinimap/*, m_spriteReflect*/;
    bool added_minimap;

    Entite_graphique m_entite_graphique;
    Entite_graphique m_entite_herbe;

private:
    int m_tileset,m_tile,m_couche;

    std::vector <int> m_monstre;
    std::vector <int> m_projectile;
    std::vector <int> m_effet;

    coordonnee m_position;

    std::vector<Objet> m_objets;

    int m_herbe,m_numeroHerbe;
    int m_herbe_taille;
    int m_hauteur;
    coordonnee m_herbe_decalage;
    sf::Color m_herbe_couleur;
};

#endif


