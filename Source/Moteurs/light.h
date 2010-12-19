

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


#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

#include "../constantes.h"

#ifndef LIGHTH
#define LIGHTH

struct Wall
{
    Wall(sf::Vector2f p1 , sf::Vector2f p2 )
    {
        pt1=p1;
        pt2=p2;
        hauteur=96;
        actif = true;
    }
    Wall(sf::Vector2f p1,sf::Vector2f p2,int newhauteur)
    {
        pt1=p1;
        pt2=p2;
        hauteur=newhauteur;
        actif = true;
    }

    // Pt1 et Pt2 sont les deux extrémités du mur
    sf::Vector2f pt1;
    sf::Vector2f pt2;
    int hauteur;

    bool actif;

    // Position du mur
    sf::Vector2f position;

    // Ombre du mur
    sf::Shape m_shadow;
};

// Wall_Entity est une variable qui permet de représenter dans le programme un mur
struct Wall_Entity
{
    Wall_Entity ()
    {
        m_ID=-1;
    }
    Wall_Entity (int id)
    {
        m_ID=id;
    }

    int ID()
    {
        return m_ID;
    }

private:

    int m_ID;
};

// Light_Entity est une variable qui permet de représenter dans le programme une lumière
struct Light_Entity
{
    Light_Entity ()
    {
        m_Dynamic=false,m_ID=-1;
    }
    Light_Entity (int id,bool d)
    {
        m_ID=id;
        m_Dynamic=d;
    }

    int ID()
    {
        return m_ID;
    }
    bool Dynamic()
    {
        return m_Dynamic;
    }

private:

    int m_ID;
    bool m_Dynamic;
};

class Light
{
public :

    // Constructeur et destructeur
    Light();
    Light(sf::Vector2f position, float intensity, float radius, int quality, sf::Color color);
    ~Light();

    // Afficher la lumière
    void Draw(sf::RenderTarget *App);

    // Calculer la lumière
    void Generate( std::vector<Wall>& m_wall);

    // Ajouter un triangle à la lumière, en effet, les lumières sont composée de triangles
    void AddTriangle(sf::Vector2f pt1,sf::Vector2f pt2, int minimum_wall, std::vector<Wall>& m_wall, int hauteur = 96);

    // Changer différents attributs de la lumière
    void SetIntensity(float);
    void SetRadius(float);
    void SetQuality(int);
    void SetColor(sf::Color);
    void SetPosition(sf::Vector2f);



    // Retourner différents attributs de la lumière
    float GetIntensity();
    float GetRadius();
    int GetQuality();
    sf::Color GetColor();
    sf::Vector2f GetPosition();

    // Une petite bool pour savoir si la lumière est allumée ou éteinte
    bool m_actif;

private :
    //Position à l'écran
    sf::Vector2f m_position;
    //Intensité, gère la transparence ( entre 0 et 255 )
    float m_intensity;
    //Rayon de la lumière
    float m_radius;
    //Couleur de la lumière
    sf::Color m_color;
    //Qualité de la lumière, c'est à dire le nombre de triangles par défaut qui la compose.
    int m_quality;

    //Tableau dynamique de Shape, ce sont ces shapes de type triangle qui compose la lumière
    std::vector <sf::Shape> m_shape;
};

#endif

