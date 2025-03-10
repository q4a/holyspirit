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



#include "Entite_graphique.h"
#include "../globale.h"

Entite_graphique::Entite_graphique()
{
    m_reflect       = false;
    m_shadow        = false;
    m_distort       = false;

    m_tileset       = NULL;

    m_couche        = 0;
    m_decalCouche   = 0;
    m_noAnimation   = 0;

    m_sprite.SetSubRect(sf::IntRect(0,0,0,0));

    m_animation = 0;

    option_sonUnique        = true;
    option_forcedLight      = false;
    option_forcedShadow     = false;
    option_forcedReflect    = false;

    m_decalage.x = 0;
    m_decalage.y = 0;
    m_scale.x = 100;
    m_scale.y = 100;
    m_rotation = 0;
    m_color.r = 255;
    m_color.g = 255;
    m_color.b = 255;
    m_color.a = 255;
}

Entite_graphique::~Entite_graphique()
{
    //dtor
}

void Entite_graphique::Animer(float temps)
{
    attaque_touche  = false;
    attaque_stop    = false;
    attaque_pause   = false;

    if(m_tileset != NULL)
    {
        float tempsAnimation = m_tileset->getTempsDuTile(m_noAnimation);
        m_animation += temps;

        if (m_tileset->getAnimationTile(m_noAnimation) >= 0)
            while (m_animation >= tempsAnimation && tempsAnimation > 0)
            {
                NextTile();
                Generer();

                m_animation -= tempsAnimation;
                tempsAnimation = m_tileset->getTempsDuTile(m_noAnimation);
            }
    }
}

void Entite_graphique::NextTile()
{
    if(m_tileset != NULL)
    {
        m_noAnimation = m_tileset->getAnimationTile(m_noAnimation);

        coordonnee position;
        position.x = (int)(m_sprite.GetPosition().x/64/5);
        position.y = (int)(m_sprite.GetPosition().y/32/5);

        if(m_tileset->getSonTile(m_noAnimation) >= 0)
            m_tileset->JouerSon(m_tileset->getSonTile(m_noAnimation),position,option_sonUnique);

        if(m_tileset->getAttaqueDuTile(m_noAnimation) >= 0)
        {
            if(m_tileset->getAttaqueDuTile(m_noAnimation) == 0)
                attaque_touche  = true;
            else if(m_tileset->getAttaqueDuTile(m_noAnimation) == 1)
                attaque_stop    = true;
            else if(m_tileset->getAttaqueDuTile(m_noAnimation) == 2)
                attaque_pause   = true;
        }

        if (configuration->Lumiere && m_light.ID() != -1)
        {
            bool modif = false;
            if(m_tileset->getLumiereDuTile(m_noAnimation).intensite >= 0)
            {
                if(m_tileset->getLumiereDuTile(m_noAnimation).intensite != moteurGraphique->LightManager->GetIntensity(m_light))
                    modif = true;

                moteurGraphique->LightManager->SetIntensity(m_light,m_tileset->getLumiereDuTile(m_noAnimation).intensite);
                moteurGraphique->LightManager->SetRadius(m_light,m_tileset->getLumiereDuTile(m_noAnimation).intensite*3);
            }
            if(m_tileset->getLumiereDuTile(m_noAnimation).rouge >= 0)
            {
                if(m_tileset->getLumiereDuTile(m_noAnimation).rouge != moteurGraphique->LightManager->GetColor(m_light).r)
                    modif = true;
                if(m_tileset->getLumiereDuTile(m_noAnimation).vert != moteurGraphique->LightManager->GetColor(m_light).g)
                    modif = true;
                if(m_tileset->getLumiereDuTile(m_noAnimation).bleu != moteurGraphique->LightManager->GetColor(m_light).b)
                    modif = true;
                moteurGraphique->LightManager->SetColor(m_light,sf::Color(m_tileset->getLumiereDuTile(m_noAnimation).rouge,m_tileset->getLumiereDuTile(m_noAnimation).vert,m_tileset->getLumiereDuTile(m_noAnimation).bleu));
            }

            if(modif)
                moteurGraphique->LightManager->Generate(m_light);

            //moteurGraphique->LightManager->SetIntensity(m_light_wall,m_tileset->getLumiereDuTile(m_noAnimation).intensite);
        }
        if(configuration->Lumiere && m_light.ID() == -1 && m_tileset->getLumiereDuTile(m_noAnimation).intensite >= 0)
        {
            sf::Vector2f position;
            position.x = (int)(m_sprite.GetPosition().x);
            position.y = (int)(m_sprite.GetPosition().y*2);

            m_light = moteurGraphique->LightManager->Add_Dynamic_Light(position,m_tileset->getLumiereDuTile(m_noAnimation).intensite,m_tileset->getLumiereDuTile(m_noAnimation).intensite*3,6,
                                                                       sf::Color(m_tileset->getLumiereDuTile(m_noAnimation).rouge,m_tileset->getLumiereDuTile(m_noAnimation).vert,m_tileset->getLumiereDuTile(m_noAnimation).bleu));
        }
    }
}

void Entite_graphique::Initialiser(coordonnee pos)
{
    if(m_tileset != NULL)
    {
        Generer();

        if(configuration->Lumiere)
        if(m_tileset->getTaille() > 0)
        {
            if (m_tileset->getLumiereDuTile(m_noAnimation).intensite < 0 && !option_forcedLight)
            {
                if (m_tileset->getOrientationDuTile(m_noAnimation)=='x'||m_tileset->getOrientationDuTile(m_noAnimation)=='c'||m_tileset->getOrientationDuTile(m_noAnimation)=='o'||m_tileset->getOrientationDuTile(m_noAnimation)=='q')
                    m_light_wall = moteurGraphique->LightManager->Add_Wall(sf::Vector2f(pos.x-32, pos.y-32), sf::Vector2f(pos.x+32 , pos.y+32),(int)m_tileset->getLumiereDuTile(m_noAnimation).hauteur);

                if (m_tileset->getOrientationDuTile(m_noAnimation)=='y'||m_tileset->getOrientationDuTile(m_noAnimation)=='c'||m_tileset->getOrientationDuTile(m_noAnimation)=='p'||m_tileset->getOrientationDuTile(m_noAnimation)=='r')
                    m_light_wall = moteurGraphique->LightManager->Add_Wall(sf::Vector2f(pos.x-32, pos.y+32), sf::Vector2f(pos.x+32 , pos.y-32),(int)m_tileset->getLumiereDuTile(m_noAnimation).hauteur);

                if (m_tileset->getOrientationDuTile(m_noAnimation)=='b'||m_tileset->getOrientationDuTile(m_noAnimation)=='g'||m_tileset->getOrientationDuTile(m_noAnimation)=='q')
                    m_light_wall = moteurGraphique->LightManager->Add_Wall(sf::Vector2f(pos.x, pos.y), sf::Vector2f(pos.x+32 , pos.y-32),(int)m_tileset->getLumiereDuTile(m_noAnimation).hauteur);

                if (m_tileset->getOrientationDuTile(m_noAnimation)=='b'||m_tileset->getOrientationDuTile(m_noAnimation)=='p')
                    m_light_wall = moteurGraphique->LightManager->Add_Wall(sf::Vector2f(pos.x-32, pos.y-32), sf::Vector2f(pos.x-0.1 , pos.y-0.1),(int)m_tileset->getLumiereDuTile(m_noAnimation).hauteur);

                if (m_tileset->getOrientationDuTile(m_noAnimation)=='d')
                    m_light_wall = moteurGraphique->LightManager->Add_Wall(sf::Vector2f(pos.x-32, pos.y-32), sf::Vector2f(pos.x-0.1 , pos.y+0.1),(int)m_tileset->getLumiereDuTile(m_noAnimation).hauteur);

                if (m_tileset->getOrientationDuTile(m_noAnimation)=='h'||m_tileset->getOrientationDuTile(m_noAnimation)=='g'||m_tileset->getOrientationDuTile(m_noAnimation)=='r')
                    m_light_wall = moteurGraphique->LightManager->Add_Wall(sf::Vector2f(pos.x+0.1, pos.y+0.1), sf::Vector2f(pos.x+32 , pos.y+32),(int)m_tileset->getLumiereDuTile(m_noAnimation).hauteur);

                if (m_tileset->getOrientationDuTile(m_noAnimation)=='h'||m_tileset->getOrientationDuTile(m_noAnimation)=='d')
                    m_light_wall = moteurGraphique->LightManager->Add_Wall(sf::Vector2f(pos.x-32, pos.y+32), sf::Vector2f(pos.x , pos.y),(int)m_tileset->getLumiereDuTile(m_noAnimation).hauteur);
                if (m_tileset->getOrientationDuTile(m_noAnimation)=='o')
                    m_light_wall = moteurGraphique->LightManager->Add_Wall(sf::Vector2f(pos.x-32, pos.y+32), sf::Vector2f(pos.x-0.1 , pos.y+0.1),(int)m_tileset->getLumiereDuTile(m_noAnimation).hauteur);
            }
            else/* if (m_tileset->getLumiereDuTile(m_noAnimation).intensite > 0)*/
            {

                bool light = option_forcedLight;

                for(int i = 0 ; i < m_tileset->getTaille(); ++i)
                    if(m_tileset->getLumiereDuTile(i).intensite > 0)
                        light = true;

                if(light)
                {

                    sf::Vector2f position;
                    position.x = pos.x;
                    position.y = pos.y;

                    //if (m_tileset->getAnimationTile(m_noAnimation) != -1)
                        m_light = moteurGraphique->LightManager->Add_Dynamic_Light(position,m_tileset->getLumiereDuTile(m_noAnimation).intensite,m_tileset->getLumiereDuTile(m_noAnimation).intensite*3,6,
                                                                                   sf::Color(m_tileset->getLumiereDuTile(m_noAnimation).rouge,m_tileset->getLumiereDuTile(m_noAnimation).vert,m_tileset->getLumiereDuTile(m_noAnimation).bleu));
                    //else
                      //  m_light = moteurGraphique->LightManager->Add_Static_Light (position,m_tileset->getLumiereDuTile(m_noAnimation).intensite,m_tileset->getLumiereDuTile(m_noAnimation).intensite*3,6,
                        //                                                            sf::Color(m_tileset->getLumiereDuTile(m_noAnimation).rouge,m_tileset->getLumiereDuTile(m_noAnimation).vert,m_tileset->getLumiereDuTile(m_noAnimation).bleu));
                }
            }
        }
    }
}

void Entite_graphique::Generer()
{
    if(m_tileset != NULL)
    {
        if(m_tileset->getTaille() > 0)
        {
            coordonnee positionPartieDecor = m_tileset->getPositionDuTile(m_noAnimation);

            m_sprite.SetImage(*moteurGraphique->getImage(m_tileset->getImage(m_noAnimation)));
            m_sprite.SetSubRect(sf::IntRect(positionPartieDecor.x, positionPartieDecor.y,
                                            positionPartieDecor.w, positionPartieDecor.h - 1));

            m_sprite.SetOrigin(m_tileset->getCentreDuTile(m_noAnimation).x,m_tileset->getCentreDuTile(m_noAnimation).y);

            m_sprite.SetColor(sf::Color(m_sprite.GetColor().r,m_sprite.GetColor().g,m_sprite.GetColor().b,m_tileset->getOpacityDuTile(m_noAnimation)));

            m_decalCouche = m_tileset->getLayerDuTile(m_noAnimation);

            m_shadow = false;
            if (configuration->Ombre)
                if (m_tileset->getOmbreDuTile(m_noAnimation)    || option_forcedShadow)
                    m_shadow = true;

            /*m_reflect = false;
            if (configuration->Reflection)
                if (m_tileset->getReflectionDuTile(m_noAnimation)|| option_forcedReflect)
                    m_reflect = true;*/
        }
    }
}

void Entite_graphique::SaveParameters(std::ofstream &fichier)
{
     if(m_decalage.x != 0 || m_decalage.y != 0
     ||m_scale.x != 100 || m_scale.y != 100
     ||m_rotation != 0
     ||m_color.r != 255 || m_color.g != 255 || m_color.b != 255 || m_color.a != 255)
     {
         fichier<<"p";
         if(m_decalage.x != 0)
            fichier<<" tx"<<m_decalage.x;
         if(m_decalage.y != 0)
            fichier<<" ty"<<m_decalage.y;

         if(m_scale.x != 100)
            fichier<<" sx"<<m_scale.x;
         if(m_scale.y != 100)
            fichier<<" sy"<<m_scale.y;

        if(m_rotation != 0)
            fichier<<" r"<<m_rotation;

         if(m_color.r != 255)
            fichier<<" cr"<<m_color.r;
         if(m_color.g != 255)
            fichier<<" cg"<<m_color.g;
         if(m_color.b != 255)
            fichier<<" cb"<<m_color.b;
         if(m_color.a != 255)
            fichier<<" ca"<<m_color.a;

        fichier<<" $ ";
     }

}
void Entite_graphique::LoadParameters(std::ifstream &fichier)
{
    char caractere;
    do
    {
        fichier.get(caractere);
        if(caractere == 't')
        {
            fichier.get(caractere);
            if(caractere == 'x')
                fichier>>m_decalage.x;
            if(caractere == 'y')
                fichier>>m_decalage.y;
        }
        if(caractere == 's')
        {
            fichier.get(caractere);
            if(caractere == 'x')
                fichier>>m_scale.x;
            if(caractere == 'y')
                fichier>>m_scale.y;
        }
        if(caractere == 'r')
        {
            fichier>>m_rotation;
        }
        if(caractere == 'c')
        {
            fichier.get(caractere);
            if(caractere == 'r')
                fichier>>m_color.r;
            if(caractere == 'g')
                fichier>>m_color.g;
            if(caractere == 'b')
                fichier>>m_color.b;
            if(caractere == 'a')
                fichier>>m_color.a;
        }
    }while(caractere != '$');
}
