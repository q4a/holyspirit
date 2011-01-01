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



#ifndef ENTITE_GRAPHIQUE_H
#define ENTITE_GRAPHIQUE_H

#include "../Map/tileset.h"
#include "light.h"

class Entite_graphique
{
    public:
        Entite_graphique();
        virtual ~Entite_graphique();
        void Animer(float temps);
        void NextTile(bool cur = false,bool no_sound = false);
        void Initialiser(coordonnee position);
        void Generer();

        void SaveParameters(std::ofstream &fichier);
        void LoadParameters(std::ifstream &fichier);
        void SetParameters (Entite_graphique &entite);

        int  getOrdre();
   // protected:
    //private:

    sf::Sprite m_sprite;
    sf::Sprite m_sprite_distortion;
    std::vector <sf::Sprite> m_sprite_shadowmap;
    std::vector <int> m_angle_shadowmap;

    bool m_reflect;
    bool m_shadow;
    bool m_distort;
    bool m_fixed;

    Tileset *m_tileset;

    int m_couche;
    int m_decalCouche;
    int m_noAnimation;
    int m_ambientShadow;

    float m_animation;

    float m_old_lightIntensity;

    Light_Entity m_light;
    Wall_Entity  m_light_wall;

    bool attaque_touche;
    bool attaque_stop;
    bool attaque_pause;

    bool option_sonUnique;
    bool option_forcedLight;
    bool option_forcedShadow;
    bool option_forcedReflect;

    float m_sound_volume;

    sf::Vector2f m_decalage;
    sf::Vector2f m_scale;
    float m_rotation;
    sf::Color  m_color;
};

#endif // ENTITE_GRAPHIQUE_H
