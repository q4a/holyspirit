
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



#include <iostream>
#include <math.h>
#include "light.h"
#include "../globale.h"


Light::Light()
{
    m_intensity=0;
    m_radius=0;
    m_quality=0;
    m_actif=false;
}

Light::Light(sf::Vector2f position, float intensity, float radius, int quality, sf::Color color)
{
    m_position=position;

    m_position=position;
    m_position.x+=0.34752;
    m_position.y+=0.52863;

    m_intensity=intensity;
    m_radius=radius;
    m_color=color;
    m_quality=quality;
    if (intensity>0)
        m_actif=true;
    else
        m_actif=false;

}

Light::~Light()
{
    m_shape.clear();
}


void Light::Draw(sf::RenderTarget *App)
{
    // On boucle sur m_shape pour afficher tous les triangles.
    for (int i=0;i<(int)m_shape.size();i++)
        App->Draw(m_shape[i]);
}


sf::Vector2f Intersect(sf::Vector2f p1, sf::Vector2f p2, sf::Vector2f q1, sf::Vector2f q2)
{
    sf::Vector2f j;

    if((p2.x - p1.x) == 0 && (q2.x - q1.x) == 0)
        j.x = 0, j.y = 0;
    else if((p2.x - p1.x) == 0)
    {
        j.x = p1.x;

        float c = (q2.y - q1.y) / (q2.x - q1.x);
        float d = q1.y - q1.x * c;

        j.y = c * j.x + d;
    }

    else if((q2.x - q1.x) == 0)
    {
        j.x = q1.x;

        float a = (p2.y - p1.y) / (p2.x - p1.x);
        float b = p1.y - p1.x * a;

        j.y = a * j.x + b;
    }
    else
    {
        float a = (p2.y - p1.y) / (p2.x - p1.x);
        float b = p1.y - p1.x * a;

        float c = (q2.y - q1.y) / (q2.x - q1.x);
        float d = q1.y - q1.x * c;

        j.x = (d-b)/(a-c);
        j.y = a * j.x + b;
    }

    return j;
}

sf::Vector2f Collision(sf::Vector2f p1, sf::Vector2f p2, sf::Vector2f q1, sf::Vector2f q2)
{
    sf::Vector2f i;
    i = Intersect(p1, p2, q1, q2);

    if(((i.x >= p1.x - 0.1 && i.x <= p2.x + 0.1)
        || (i.x >= p2.x - 0.1 && i.x <= p1.x + 0.1))
    && ((i.x >= q1.x - 0.1 && i.x <= q2.x + 0.1)
        || (i.x >= q2.x - 0.1 && i.x <= q1.x + 0.1))
    && ((i.y >= p1.y - 0.1 && i.y <= p2.y + 0.1)
        || (i.y >= p2.y - 0.1 && i.y <= p1.y + 0.1))
    && ((i.y >= q1.y - 0.1 && i.y <= q2.y + 0.1)
        || (i.y >= q2.y - 0.1 && i.y <= q1.y + 0.1)))
        return i;
    else
        return sf::Vector2f (0,0);

}


void Light::AddTriangle(sf::Vector2f pt1,sf::Vector2f pt2, std::list<int> deja_wall,std::vector<Wall>& m_wall, std::vector <std::vector <std::vector <int> > > &m_sectors, sf::Vector2i &m_origin_sector, int hauteur)
{
    // On bo€ucle sur tous les murs
    if (configuration->Lumiere==2)

    for(int y =  (m_position.y - m_radius) / SECTOR_SIZE + m_origin_sector.y;
            y <  (m_position.y + m_radius) / SECTOR_SIZE + 1 + m_origin_sector.y;
          ++y)
    if(y >= 0 && y < m_sectors.size())
    for(int x =  (m_position.x - m_radius) / SECTOR_SIZE + m_origin_sector.x;
            x <  (m_position.x + m_radius) / SECTOR_SIZE + 1 + m_origin_sector.x;
          ++x)
    if(x >= 0 && x < m_sectors[y].size())
    for (std::vector<int>::iterator IterWall=m_sectors[y][x].begin();
                                    IterWall!=m_sectors[y][x].end();++IterWall)
        if(m_wall[*IterWall].actif)
        {
            bool ok = true;

            for(std::list<int>::iterator i = deja_wall.begin() ; i != deja_wall.end() ; ++i)
                if((*i) == (*IterWall))
                {
                    ok = false;
                    break;
                }


            if(ok)
            if ( m_wall[*IterWall].pt1.x-m_position.x>-m_radius && m_wall[*IterWall].pt1.y-m_position.y>-m_radius
              && m_wall[*IterWall].pt2.x-m_position.x<m_radius && m_wall[*IterWall].pt2.y-m_position.y<m_radius )
            {
                // l1 et l2 sont les positions relatives au centre de la lumière des deux extrémités du mur
                sf::Vector2f l1(m_wall[*IterWall].pt1.x-m_position.x, m_wall[*IterWall].pt1.y-m_position.y);
                sf::Vector2f l2(m_wall[*IterWall].pt2.x-m_position.x, m_wall[*IterWall].pt2.y-m_position.y);

                if(l1.x * l1.x + l1.y * l1.y < m_radius * m_radius)
                {
                    sf::Vector2f i = Intersect(pt1,pt2,sf::Vector2f (0,0),l1);

                    if((pt1.x > i.x + 0.01 && pt2.x < i.x - 0.01) || (pt1.x < i.x - 0.01 && pt2.x > i.x + 0.01))
                    if((pt1.y > i.y + 0.01 && pt2.y < i.y - 0.01) || (pt1.y < i.y - 0.01 && pt2.y > i.y + 0.01))
                        if(l1.y > 0 && i.y > 0 || l1.y < 0 && i.y < 0)
                        if(l1.x > 0 && i.x > 0 || l1.x < 0 && i.x < 0)
                        AddTriangle(i, pt2, deja_wall, m_wall, m_sectors, m_origin_sector,m_wall[*IterWall].hauteur), pt2 = i, hauteur = m_wall[*IterWall].hauteur;
                }
                if(l2.x * l2.x + l2.y * l2.y < m_radius * m_radius)
                {
                    sf::Vector2f i = Intersect(pt1,pt2,sf::Vector2f (0,0),l2);

                    if((pt1.x > i.x + 0.01 && pt2.x < i.x - 0.01) || (pt1.x < i.x - 0.01 && pt2.x > i.x + 0.01))
                    if((pt1.y > i.y + 0.01 && pt2.y < i.y - 0.01) || (pt1.y < i.y - 0.01 && pt2.y > i.y + 0.01))
                        if(l2.y > 0 && i.y > 0 || l2.y < 0 && i.y < 0)
                        if(l2.x > 0 && i.x > 0 || l2.x < 0 && i.x < 0)
                        AddTriangle(pt1, i, deja_wall, m_wall, m_sectors, m_origin_sector,m_wall[*IterWall].hauteur), pt1 = i, hauteur = m_wall[*IterWall].hauteur;
                }

                deja_wall.push_back(*IterWall);

                sf::Vector2f m = Collision(l1, l2, sf::Vector2f(0,0), pt1);
                sf::Vector2f n = Collision(l1, l2, sf::Vector2f(0,0), pt2);
                sf::Vector2f o = Collision(l1, l2, pt1, pt2);

                if((m.x != 0 || m.y != 0) && (n.x != 0 || n.y != 0))
                    pt1 = m, pt2 = n, hauteur = m_wall[*IterWall].hauteur;
                else
                {
                    if((m.x != 0 || m.y != 0) && (o.x != 0 || o.y != 0))
                        AddTriangle(m ,o , deja_wall, m_wall, m_sectors, m_origin_sector,m_wall[*IterWall].hauteur), pt1 = o;

                    if((n.x != 0 || n.y != 0) && (o.x != 0 || o.y != 0))
                        AddTriangle(o ,n , deja_wall, m_wall, m_sectors, m_origin_sector,m_wall[*IterWall].hauteur), pt2 = o;
                }
            }
        }

    // Variable qui contiendra l'intensité calculée, pour le dégradé
    float intensity,intensity2;


    // On ajoute un shape
    m_shape.push_back(sf::Shape ());

    // On lui donne comme point de départ (0,0), le centre de la lumière, avec la couleur et intensité maximal
    m_shape.back().AddPoint(0, 0,  sf::Color((int)(m_intensity*m_color.r/255),(int)(m_intensity*m_color.g/255),(int)(m_intensity*m_color.b/255)));

    // On calcul ou l'on se trouve par rapport au centre, pour savoir à quel intensité on est
    intensity=m_intensity-gpl::sqrt(pt1.x*pt1.x + pt1.y*pt1.y)*m_intensity/m_radius;
    // Et on ajoute un  point au shape
    m_shape.back().AddPoint(pt1.x,pt1.y/2,  sf::Color((int)(intensity*m_color.r/255),(int)(intensity*m_color.g/255),(int)(intensity*m_color.b/255)));

    // Idem
    intensity2=m_intensity-gpl::sqrt(pt2.x*pt2.x + pt2.y*pt2.y)*m_intensity/m_radius;

    m_shape.back().AddPoint(pt2.x,pt2.y/2,  sf::Color((int)(intensity2*m_color.r/255),(int)(intensity2*m_color.g/255),(int)(intensity2*m_color.b/255)));

    // On met que le shape soit en Add et on lui donne sa position
    m_shape.back().SetBlendMode(sf::Blend::Add);

    if(hauteur > 160)
        hauteur = 160;
    if(hauteur == -1)
        hauteur = 256;


    m_shape.back().SetPosition(m_position.x,m_position.y/2);

    if (intensity>1||intensity2>1)
        if ( 0>=(pt1.y) - (pt1.x)*(((pt1.y)-(pt2.y))/((pt1.x)-(pt2.x))))
        {
            m_shape.push_back(sf::Shape ());

            m_shape.back().AddPoint(pt1.x,pt1.y/2,  sf::Color((int)(intensity*m_color.r/255),(int)(intensity*m_color.g/255),(int)(intensity*m_color.b/255)));
            m_shape.back().AddPoint(pt1.x,pt1.y/2-hauteur * sin(intensity /m_intensity*M_PI_2),  sf::Color(0,0,0));
            m_shape.back().AddPoint(pt2.x,pt2.y/2-hauteur * sin(intensity2/m_intensity*M_PI_2),  sf::Color(0,0,0));
            m_shape.back().AddPoint(pt2.x,pt2.y/2,  sf::Color((int)(intensity2*m_color.r/255),(int)(intensity2*m_color.g/255),(int)(intensity2*m_color.b/255)));

            m_shape.back().SetBlendMode(sf::Blend::Add);

            m_shape.back().SetPosition(m_position.x,m_position.y/2);
        }
}

void Light::Generate(std::vector<Wall>& m_wall, std::vector <std::vector <std::vector <int> > > &m_sectors, sf::Vector2i &m_origin_sector)
{
    // On vide la mémoire
    m_shape.clear();

    // buf est l'angle de chaque triangle, c'est donc 2pi divisé par le nombre de triangles
    float buf=(M_PI*2)/(float)m_quality;



    // On ajoute tous les triangles qui composent la lumière
    std::list <int> list;
    for (int i=0;i<m_quality;i++)
    {
        list.clear();
        AddTriangle(sf::Vector2f((int)((float)m_radius*cos((float)i*buf)),
                                 (int)((float)m_radius*sin((float)i*buf))) ,
                    sf::Vector2f((int)((float)m_radius*cos((float)(i+1)*buf)),
                                 (int)((float)m_radius*sin((float)(i+1)*buf))),list,m_wall, m_sectors, m_origin_sector);
    }

}


// Différentes fonctions pour modifier les attributs de la lumière, et pour les récupérer

void Light::SetIntensity(float intensity)
{
    m_intensity=intensity;
    if (m_intensity<=0) m_actif=false;
    else m_actif=true;
}
void Light::SetRadius(float radius)
{
    m_radius=radius;
}
void Light::SetQuality(int quality)
{
    m_quality=quality;
}
void Light::SetColor(sf::Color color)
{
    m_color=color;
}
void Light::SetPosition(sf::Vector2f position)
{
    m_position=position;
    m_position.y += 2.11;
    //m_position.x += 0.01;
}



float Light::GetIntensity()
{
    return m_intensity;
}
float Light::GetRadius()
{
    return m_radius;
}
int Light::GetQuality()
{
    return m_quality;
}
sf::Vector2f Light::GetPosition()
{
    return m_position;
}
sf::Color Light::GetColor()
{
    return m_color;
}









