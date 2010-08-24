

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




#ifndef MOTEURPARTICULES
#define MOTEURPARTICULES

#include "../constantes.h"
#include "modeleMoteurParticule.h"

struct Particule
{
    coordonnee3D position;
    coordonnee3D vecteur;
    float vitesse;
    float vitesse_rotation;
    float rotation;
    float alpha;
    float vie;
    sf::Color color;
    float taille;

    bool sang;
    float poids;

    int numero;
    int seed;
};

class ParticuleSysteme
{
    public:
    ParticuleSysteme();
    ParticuleSysteme(int , ModeleParticuleSysteme *, coordonnee , sf::Color,float ,float);
    ~ParticuleSysteme();
    bool Afficher( ModeleParticuleSysteme *modele,float temps);
    void Envoler(sf::Vector2f pos,int force, int type);
    void Generer(float force, ModeleParticuleSysteme *modele,coordonnee position,float);

    int m_modele;
    std::vector <Particule> m_particules;
    std::vector <Particule>::iterator Iter;

    private:
    sf::Color m_color;
    int m_son;
};

#endif

