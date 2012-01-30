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



#include "Entite_graphique.h"
#include "../globale.h"

Entite_graphique::Entite_graphique() : sf::Sprite()
{
    m_reflect       = false;
    m_shadow        = false;
    m_distort       = false;
    m_fixed         = false;

    attaque_touche  = false;
    attaque_stop    = false;
    attaque_pause   = false;

    m_tileset       = NULL;

    m_couche        = 0;
    m_decalCouche   = 0;
    m_noAnimation   = 0;

    SetSubRect(sf::IntRect(0,0,0,0));

    m_animation = 0;
    m_nextAnimation = 0;

    m_old_lightIntensity = 0;

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

    m_ambientShadow     = -1;

    m_sound_volume = 100;
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
        float tempsAnimation = m_tileset->getTempsDuTile(m_noAnimation) + m_nextAnimation;
        m_animation += temps;

        if(configuration->Lumiere > 0)
        if(m_old_lightIntensity != m_tileset->getLumiereDuTile(m_noAnimation).intensite
        && m_tileset->getLumiereDuTile(m_noAnimation).intensite >= 0)// && !option_forcedLight)
        {
            float intensite = m_old_lightIntensity;
            intensite += m_animation/tempsAnimation * (m_tileset->getLumiereDuTile(m_noAnimation).intensite - m_old_lightIntensity);
            moteurGraphique->LightManager->SetRadius(m_light,(int)intensite*3);

            if (m_animation >= tempsAnimation)
                m_old_lightIntensity = m_tileset->getLumiereDuTile(m_noAnimation).intensite;

            moteurGraphique->LightManager->Generate(m_light);
        }

        if (m_tileset->getAnimationTile(m_noAnimation) >=0)//!= m_noAnimation)
            while (m_animation >= tempsAnimation && tempsAnimation > 0)
            {
                //if(m_tileset->getAnimationTile(m_noAnimation) != m_noAnimation)
                NextTile();
                Generer();

                m_animation -= tempsAnimation;
                tempsAnimation = m_tileset->getTempsDuTile(m_noAnimation) + m_nextAnimation;
            }
    }
}

void Entite_graphique::NextTile(bool cur,bool no_sound)
{
    if(m_tileset != NULL)
    {
        if(m_tileset->getAttaqueDuTile(m_noAnimation) >= 0)
        {
            if(m_tileset->getAttaqueDuTile(m_noAnimation) == 0)
                attaque_touche  = true;
            else if(m_tileset->getAttaqueDuTile(m_noAnimation) == 1)
                attaque_stop    = true;
            else if(m_tileset->getAttaqueDuTile(m_noAnimation) == 2)
                attaque_pause   = true;
        }

        m_old_lightIntensity = m_tileset->getLumiereDuTile(m_noAnimation).intensite;

        if(!cur)
            m_noAnimation = m_tileset->getAnimationTile(m_noAnimation);

        coordonnee position;
        position.x = (int)(GetPosition().x/64/5);
        position.y = (int)(GetPosition().y/32/5);

        if(m_fixed)
        {
            position.x = (int)moteurGraphique->m_camera.GetCenter().x;
            position.y = (int)moteurGraphique->m_camera.GetCenter().y;
        }

        m_nextAnimation = ((float)rand()/RAND_MAX)*m_tileset->getTempsRandDuTile(m_noAnimation);

        if(!m_tileset->getSonTile(m_noAnimation).empty() && !no_sound)
        {
            const int n = m_tileset->getSonTile(m_noAnimation).size();
            int no = rand()%n, nbr = 0;

            while(!m_tileset->JouerSon(m_tileset->getSonTile(m_noAnimation)[no++],position,option_sonUnique,m_sound_volume) && nbr++ <= n)
                if(no >= n)
                    no = 0;
        }


        if(configuration->Lumiere && m_light.ID() == -1 && m_tileset->getLumiereDuTile(m_noAnimation).intensite > 0)
        {
            sf::Vector2f position2;
            position2.x = (int)(GetPosition().x);
            position2.y = (int)(GetPosition().y*2);

            m_light = moteurGraphique->LightManager->Add_Dynamic_Light(position2,255,m_tileset->getLumiereDuTile(m_noAnimation).intensite*3,12,
                                                                       sf::Color(m_tileset->getLumiereDuTile(m_noAnimation).rouge,
                                                                                 m_tileset->getLumiereDuTile(m_noAnimation).vert,
                                                                                 m_tileset->getLumiereDuTile(m_noAnimation).bleu));
        }

        if (configuration->Lumiere > 0 && m_light.ID() != -1)
        {
            bool modif = false;
            if(m_tileset->getLumiereDuTile(m_noAnimation).intensite >= 0)
            {
               // if(m_tileset->getLumiereDuTile(m_noAnimation).intensite != moteurGraphique->LightManager->GetIntensity(m_light))
                 //   modif = true;

                if(m_tileset->getLumiereDuTile(m_noAnimation).intensite > 0)
                    moteurGraphique->LightManager->SetIntensity(m_light,255);
                else
                    moteurGraphique->LightManager->SetIntensity(m_light,0);
            }
            if(m_tileset->getLumiereDuTile(m_noAnimation).rouge >= 0
            || m_tileset->getLumiereDuTile(m_noAnimation).vert >= 0
            || m_tileset->getLumiereDuTile(m_noAnimation).bleu >= 0)
            {
                if(m_tileset->getLumiereDuTile(m_noAnimation).rouge != moteurGraphique->LightManager->GetColor(m_light).r
                || m_tileset->getLumiereDuTile(m_noAnimation).vert != moteurGraphique->LightManager->GetColor(m_light).g
                || m_tileset->getLumiereDuTile(m_noAnimation).bleu != moteurGraphique->LightManager->GetColor(m_light).b)
                    modif = true;
                moteurGraphique->LightManager->SetColor(m_light,sf::Color(m_tileset->getLumiereDuTile(m_noAnimation).rouge,
                                                                          m_tileset->getLumiereDuTile(m_noAnimation).vert,
                                                                          m_tileset->getLumiereDuTile(m_noAnimation).bleu));
            }

            if(modif)
                moteurGraphique->LightManager->Generate(m_light);
        }
        if(configuration->Lumiere > 0 && m_light_wall.ID() != -1)
            moteurGraphique->LightManager->SetIntensity(m_light_wall, m_tileset->getLumiereDuTile(m_noAnimation).intensite);

        if(configuration->Lumiere > 0 && m_light_wall.ID() == -1
        && m_tileset->getLumiereDuTile(m_noAnimation).intensite < 0
        && m_tileset->getOrientationDuTile(m_noAnimation) != ' ')
        {
            coordonnee pos;
            pos.x = GetPosition().x;
            pos.y = GetPosition().y * 2;

            AddWallLight(pos);
        }
    }
}

void Entite_graphique::AddWallLight(const coordonnee &pos2)
{
    coordonnee pos = pos2;
    pos.x += m_tileset->getLumiereDecalDuTile(m_noAnimation).x;
    pos.y += m_tileset->getLumiereDecalDuTile(m_noAnimation).y;

    if(m_light_wall.ID() == -1
    && m_tileset->getLumiereDuTile(m_noAnimation).intensite < 0
    && m_tileset->getOrientationDuTile(m_noAnimation) != ' ')// && !option_forcedLight)
    {
        if (m_tileset->getOrientationDuTile(m_noAnimation)=='x'||m_tileset->getOrientationDuTile(m_noAnimation)=='c'||m_tileset->getOrientationDuTile(m_noAnimation)=='o'||m_tileset->getOrientationDuTile(m_noAnimation)=='q')
            m_light_wall = moteurGraphique->LightManager->Add_Wall(sf::Vector2f(pos.x-32, pos.y-32), sf::Vector2f(pos.x+32 , pos.y+32),(int)m_tileset->getLumiereDuTile(m_noAnimation).hauteur);

        if (m_tileset->getOrientationDuTile(m_noAnimation)=='y'||m_tileset->getOrientationDuTile(m_noAnimation)=='c'||m_tileset->getOrientationDuTile(m_noAnimation)=='p'||m_tileset->getOrientationDuTile(m_noAnimation)=='r')
            m_light_wall = moteurGraphique->LightManager->Add_Wall(sf::Vector2f(pos.x-32, pos.y+32), sf::Vector2f(pos.x+32 , pos.y-32),(int)m_tileset->getLumiereDuTile(m_noAnimation).hauteur);

        if (m_tileset->getOrientationDuTile(m_noAnimation)=='b'||m_tileset->getOrientationDuTile(m_noAnimation)=='g'||m_tileset->getOrientationDuTile(m_noAnimation)=='q')
            m_light_wall = moteurGraphique->LightManager->Add_Wall(sf::Vector2f(pos.x, pos.y), sf::Vector2f(pos.x+32 , pos.y-32),(int)m_tileset->getLumiereDuTile(m_noAnimation).hauteur);

        if (m_tileset->getOrientationDuTile(m_noAnimation)=='b'||m_tileset->getOrientationDuTile(m_noAnimation)=='p')
            m_light_wall = moteurGraphique->LightManager->Add_Wall(sf::Vector2f(pos.x-32, pos.y-32), sf::Vector2f(pos.x , pos.y),(int)m_tileset->getLumiereDuTile(m_noAnimation).hauteur);

        if (m_tileset->getOrientationDuTile(m_noAnimation)=='d')
            m_light_wall = moteurGraphique->LightManager->Add_Wall(sf::Vector2f(pos.x-32, pos.y-32), sf::Vector2f(pos.x , pos.y),(int)m_tileset->getLumiereDuTile(m_noAnimation).hauteur);

        if (m_tileset->getOrientationDuTile(m_noAnimation)=='h'||m_tileset->getOrientationDuTile(m_noAnimation)=='g'||m_tileset->getOrientationDuTile(m_noAnimation)=='r')
            m_light_wall = moteurGraphique->LightManager->Add_Wall(sf::Vector2f(pos.x, pos.y), sf::Vector2f(pos.x+32 , pos.y+32),(int)m_tileset->getLumiereDuTile(m_noAnimation).hauteur);

        if (m_tileset->getOrientationDuTile(m_noAnimation)=='h'||m_tileset->getOrientationDuTile(m_noAnimation)=='d')
            m_light_wall = moteurGraphique->LightManager->Add_Wall(sf::Vector2f(pos.x-32, pos.y+32), sf::Vector2f(pos.x , pos.y),(int)m_tileset->getLumiereDuTile(m_noAnimation).hauteur);
        if (m_tileset->getOrientationDuTile(m_noAnimation)=='o')
            m_light_wall = moteurGraphique->LightManager->Add_Wall(sf::Vector2f(pos.x-32, pos.y+32), sf::Vector2f(pos.x , pos.y),(int)m_tileset->getLumiereDuTile(m_noAnimation).hauteur);


        /*if (m_tileset->getOrientationDuTile(m_noAnimation)=='a')
        {
            m_light_wall = moteurGraphique->LightManager->Add_Wall(sf::Vector2f(pos.x-16, pos.y-16), sf::Vector2f(pos.x-16 , pos.y+16),(int)m_tileset->getLumiereDuTile(m_noAnimation).hauteur);
            m_light_wall = moteurGraphique->LightManager->Add_Wall(sf::Vector2f(pos.x-16, pos.y+16), sf::Vector2f(pos.x+16 , pos.y+16),(int)m_tileset->getLumiereDuTile(m_noAnimation).hauteur);
            m_light_wall = moteurGraphique->LightManager->Add_Wall(sf::Vector2f(pos.x+16, pos.y-16), sf::Vector2f(pos.x+16 , pos.y+16),(int)m_tileset->getLumiereDuTile(m_noAnimation).hauteur);
            m_light_wall = moteurGraphique->LightManager->Add_Wall(sf::Vector2f(pos.x-16, pos.y-16), sf::Vector2f(pos.x+16 , pos.y-16),(int)m_tileset->getLumiereDuTile(m_noAnimation).hauteur);
        }*/

        if (m_tileset->getOrientationDuTile(m_noAnimation)=='a')
        {
            m_light_wall = moteurGraphique->LightManager->Add_Wall(sf::Vector2f(pos.x-16, pos.y), sf::Vector2f(pos.x+16 , pos.y),(int)m_tileset->getLumiereDuTile(m_noAnimation).hauteur);
            m_light_wall = moteurGraphique->LightManager->Add_Wall(sf::Vector2f(pos.x, pos.y-16), sf::Vector2f(pos.x , pos.y+16),(int)m_tileset->getLumiereDuTile(m_noAnimation).hauteur);
        }

    }
}

void Entite_graphique::Initialiser(const coordonnee &pos)
{
    SetBlendMode(sf::Blend::Alpha);

    if(m_tileset != NULL)
    {
        NextTile(true, true);
        Generer();

        if(configuration->Lumiere)
        if(m_tileset->getTaille() > 0)
        {
            AddWallLight(pos);
            if(option_forcedLight)
            {
                sf::Vector2f position;
                position.x = pos.x + m_decalage.x;
                position.y = pos.y + m_decalage.y;

                m_light = moteurGraphique->LightManager->Add_Dynamic_Light(position,m_tileset->getLumiereDuTile(m_noAnimation).intensite > 0 ? 255 : 0,m_tileset->getLumiereDuTile(m_noAnimation).intensite*3,12,
                                                                            sf::Color(m_tileset->getLumiereDuTile(m_noAnimation).rouge,
                                                                                      m_tileset->getLumiereDuTile(m_noAnimation).vert,
                                                                                      m_tileset->getLumiereDuTile(m_noAnimation).bleu));
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

            SetTexture(*moteurGraphique->getImage(m_tileset->getImage(m_noAnimation)));
            SetSubRect(sf::IntRect(positionPartieDecor.x, positionPartieDecor.y,
                                   positionPartieDecor.w, positionPartieDecor.h - 1));

            SetOrigin(m_tileset->getCentreDuTile(m_noAnimation).x,m_tileset->getCentreDuTile(m_noAnimation).y);

            SetColor(sf::Color(m_color.r,
                                        m_color.g,
                                        m_color.b,
                                        m_tileset->getOpacityDuTile(m_noAnimation) * m_color.a / 255));

            SetScale(fabs((float)m_scale.x*0.01), fabs((float)m_scale.y*0.01));
            FlipX(m_scale.x < 0);
            FlipY(m_scale.y < 0);

            if(m_scale.x < 0)
            SetOrigin(positionPartieDecor.w - m_tileset->getCentreDuTile(m_noAnimation).x,
                      m_tileset->getCentreDuTile(m_noAnimation).y);

           SetRotation(-m_rotation);

            m_decalCouche = m_tileset->getLayerDuTile(m_noAnimation);

            m_ambientShadow = m_tileset->getAmbientShadow(m_noAnimation);

            m_shadow = false;
            if (configuration->Ombre)
                if (m_tileset->getOmbreDuTile(m_noAnimation)    || option_forcedShadow)
                    m_shadow = true;

            m_reflect = false;
            if (configuration->Reflection)
                if (m_tileset->getReflectionDuTile(m_noAnimation)|| option_forcedReflect)
                    m_reflect = true;

            if(m_tileset->getDistortionDuTile(m_noAnimation) >= 0 && configuration->Distortion)
            {
                coordonnee positionPartieDecor2 = m_tileset->getPositionDuTile(m_tileset->getDistortionDuTile(m_noAnimation), 1);

                m_sprite_distortion.SetTexture(*moteurGraphique->getImage(m_tileset->getImage(m_tileset->getDistortionDuTile(m_noAnimation), 1)));
                m_sprite_distortion.SetSubRect(sf::IntRect(positionPartieDecor2.x, positionPartieDecor2.y,
                                                           positionPartieDecor2.w, positionPartieDecor2.h));

                m_sprite_distortion.SetOrigin(m_tileset->getCentreDuTile(m_tileset->getDistortionDuTile(m_noAnimation), 1).x,m_tileset->getCentreDuTile(m_tileset->getDistortionDuTile(m_noAnimation), 1).y);

                m_sprite_distortion.SetColor(sf::Color(m_sprite_distortion.GetColor().r,
                                                       m_sprite_distortion.GetColor().g,
                                                       m_sprite_distortion.GetColor().b,
                                                       m_tileset->getOpacityDuTile(m_tileset->getDistortionDuTile(m_noAnimation), 1)));

                m_sprite_distortion.SetPosition(GetPosition());

                m_distort = true;
            }
            else
                m_distort = false;

            m_sprite_shadowmap.clear();
            m_angle_shadowmap.clear();

            for(unsigned i = 0 ; i < m_tileset->getShadowmapDuTile(m_noAnimation).size() ; ++i)
            if((int)m_tileset->m_tile_shadowmap.size() > m_tileset->getShadowmapDuTile(m_noAnimation)[i]
            && m_tileset->getShadowmapDuTile(m_noAnimation)[i] >= 0)
            {
                m_sprite_shadowmap.push_back(sf::Sprite ());
                coordonnee positionPartieDecor2 = m_tileset->m_tile_shadowmap[m_tileset->getShadowmapDuTile(m_noAnimation)[i]].getCoordonnee();


                m_sprite_shadowmap.back().SetTexture(*moteurGraphique->getImage(m_tileset->getImageShadowmap(m_noAnimation,i)));
                m_sprite_shadowmap.back().SetSubRect(sf::IntRect(positionPartieDecor2.x,
                                                                 positionPartieDecor2.y,
                                                                 positionPartieDecor2.w,
                                                                 positionPartieDecor2.h));

                m_sprite_shadowmap.back().SetOrigin(m_tileset->m_tile_shadowmap[m_tileset->getShadowmapDuTile(m_noAnimation)[i]].getCentre().x,
                                                    m_tileset->m_tile_shadowmap[m_tileset->getShadowmapDuTile(m_noAnimation)[i]].getCentre().y);

                m_sprite_shadowmap.back().SetPosition(GetPosition());

                m_angle_shadowmap.push_back(m_tileset->m_tile_shadowmap[m_tileset->getShadowmapDuTile(m_noAnimation)[i]].getAngle());
            }
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
void Entite_graphique::SetParameters (Entite_graphique &entite)
{
    m_decalage  = entite.m_decalage;
    m_scale     = entite.m_scale;
    m_rotation  = entite.m_rotation;
    m_color     = entite.m_color;
}


int Entite_graphique::getOrdre()
{
    if(m_tileset != NULL)
        return m_tileset->getOrdreDuTile(m_noAnimation);
    else
        return 0;
}
