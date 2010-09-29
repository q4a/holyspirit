

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



#include "light.h"

#ifndef LIGHTMANAGERH
#define LIGHTMANAGERH

#include "../singleton.h"

class Light_Manager : public CSingleton<Light_Manager>
{
    private :

    Light_Manager();
    ~Light_Manager();

     // Les tableaux de murs, lumières statiques et dynamiques
    std::vector <Wall> m_wall;
    std::vector <Light> m_StaticLight;
    std::vector <Light> m_DynamicLight;

    public :
    // Constructeur et destructeur
    friend Light_Manager* CSingleton<Light_Manager>::GetInstance();
    friend void CSingleton<Light_Manager>::Kill();


    //Différents moyen d'ajouter une lumière dynamique, soit on l'ajoute sans aucune valeur par défaut, soit on lui donne une lumière par défaut, soit on lui donne ses valeurs "à la main"
    Light_Entity Add_Dynamic_Light();
    Light_Entity Add_Dynamic_Light(Light);
    Light_Entity Add_Dynamic_Light(sf::Vector2f position, float intensity, float radius, int quality, sf::Color color);

    //Différents moyen d'ajouter une lumière dynamique, soit on lui donne une lumière par défaut, soit on lui donne ses valeurs "à la main"
    //On ne peut ajouter une lumière static sans rien, ça ne servirait à rien car elle ne peut être modifiée par la suite
    Light_Entity Add_Static_Light(Light);
    Light_Entity Add_Static_Light(sf::Vector2f position, float intensity, float radius, int quality, sf::Color color);

    // Ajouter un mur
    Wall_Entity Add_Wall(sf::Vector2f pt1,sf::Vector2f pt2);
    Wall_Entity Add_Wall(sf::Vector2f pt1,sf::Vector2f pt2,int hauteur);

    // Désactiver une lumière ou supprimer un mur
    void Delete_Light(Light_Entity);
    void Delete_Wall(Wall_Entity);

    void Delete_All_Wall();
    void Delete_All_Light(bool justDynamic=false);

    // Calculer toutes les lumières dynamiques1
    void Generate();
    //void Generate(sf::View *camera);
    void Generate(Light_Entity &e);

    // Afficher toutes les lumières à l'écran
    void Draw(sf::RenderTarget *App,sf::View *camera);
    void Draw(sf::RenderTarget *App,Light_Entity);

    void DrawWallShadow(sf::RenderTarget *App,sf::View *camera,float angle,Lumiere soleil);

    // Différentes méthodes pour modifier les attributs d'une lumière, ou les récupérer. Il faut à chaque fois envoyer une Light_Entity en paramètre pour
    // savoir de quelle lumière on parle/

    void SetPosition(Light_Entity &, sf::Vector2f );
    void SetQuality(Light_Entity &, int );
    void SetRadius(Light_Entity &, int );
    void SetColor(Light_Entity &, sf::Color );
    void SetIntensity(Light_Entity &, int);

    void SetIntensity(Wall_Entity &, int);

    float GetIntensity(Light_Entity);
    float GetRadius(Light_Entity);
    int GetQuality(Light_Entity);
    sf::Color GetColor(Light_Entity);
    sf::Vector2f GetPosition(Light_Entity);


    sf::Vector2f GetPt(Wall_Entity, int );
    void SetPosition(Wall_Entity, sf::Vector2f );

    private :
    std::vector<Light>::iterator Iter;

};
#endif

