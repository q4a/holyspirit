

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


#include "lightManager.h"
#include "../globale.h"

Light_Manager::Light_Manager()
{
    m_origin_sector.x = 0;
    m_origin_sector.y = 0;
}
Light_Manager::~Light_Manager()
{
    m_wall.clear();

    for (Iter=m_StaticLight.begin();Iter!=m_StaticLight.end();++Iter)
        Iter->Clear();
    for (Iter=m_DynamicLight.begin();Iter!=m_DynamicLight.end();++Iter)
        Iter->Clear();

    m_StaticLight.clear();
    m_DynamicLight.clear();


}

// Les différents moyens d'ajouter des lumières

Light_Entity Light_Manager::Add_Dynamic_Light()
{
    m_DynamicLight.push_back(Light ());
    return Light_Entity((int)m_DynamicLight.size()-1,true);
}
Light_Entity Light_Manager::Add_Dynamic_Light(Light light)
{
    m_DynamicLight.push_back(light);
    return Light_Entity((int)m_DynamicLight.size()-1,true);
}
Light_Entity Light_Manager::Add_Dynamic_Light(sf::Vector2f position, float intensity, float radius, int quality, sf::Color color, bool m)
{
    m_DynamicLight.push_back(Light (position,intensity,radius,quality,color,m));
    return Light_Entity((int)m_DynamicLight.size()-1,true);
}


// Notez bien qu'on calcule les lumières statiques lors de leur ajout.

Light_Entity Light_Manager::Add_Static_Light(Light light)
{
    m_StaticLight.push_back(light);
    m_StaticLight.back().Generate(m_wall, m_sectors, m_origin_sector);
    return Light_Entity((int)m_StaticLight.size()-1,false);
}
Light_Entity Light_Manager::Add_Static_Light(sf::Vector2f position, float intensity, float radius, int quality, sf::Color color)
{
    m_StaticLight.push_back(Light (position,intensity,radius,quality,color));
    m_StaticLight.back().Generate(m_wall, m_sectors, m_origin_sector);
    return Light_Entity((int)m_StaticLight.size()-1,false);
}

// Ajouter un mur

Wall_Entity Light_Manager::Add_Wall(sf::Vector2f pt1,sf::Vector2f pt2)
{
    for (int i=0;i<(int)m_wall.size();i++)
        if ((pt1.y-pt2.y)/(pt1.x-pt2.y)==(m_wall[i].pt1.y-m_wall[i].pt2.y)/(m_wall[i].pt1.x-m_wall[i].pt2.y))
            if (pt1==m_wall[i].pt1 || pt2==m_wall[i].pt1 || pt1==m_wall[i].pt2 || pt2==m_wall[i].pt2)
            {
                sf::Vector2f min=pt1,max=pt2;

                if (pt2.x<min.x)
                    min.x=pt2.x;
                if (pt1.x>max.x)
                    max.x=pt2.x;

                if (m_wall[i].pt1.x<min.x)
                    min.x=m_wall[i].pt1.x;
                if (m_wall[i].pt2.x<min.x)
                    min.x=m_wall[i].pt2.x;

                if (m_wall[i].pt1.x>max.x)
                    max.x=m_wall[i].pt1.x;
                if (m_wall[i].pt2.x>max.x)
                    max.x=m_wall[i].pt2.x;


                if (pt2.y<min.y)
                    min.y=pt2.y;
                if (pt1.y>max.y)
                    max.y=pt2.y;

                if (m_wall[i].pt1.y<min.y)
                    min.y=m_wall[i].pt1.y;
                if (m_wall[i].pt2.y<min.y)
                    min.y=m_wall[i].pt2.y;

                if (m_wall[i].pt1.y>max.y)
                    max.y=m_wall[i].pt1.y;
                if (m_wall[i].pt2.y>max.y)
                    max.y=m_wall[i].pt2.y;

                m_wall[i].pt1=min;
                m_wall[i].pt2=max;

                return Wall_Entity(i);

            }


    sf::Vector2f min=pt1,max=pt2;

    if (pt2.x<min.x)
        min.x=pt2.x;
    if (pt1.x>max.x)
        max.x=pt2.x;
    if (pt2.y<min.y)
        min.y=pt2.y;
    if (pt1.y>max.y)
        max.y=pt2.y;

    pt1=min;
    pt2=max;

    m_wall.push_back(Wall (pt1,pt2));

    if(pt1.y + m_origin_sector.y * SECTOR_SIZE >= m_sectors.size() * SECTOR_SIZE)
        m_sectors.resize(pt1.y/SECTOR_SIZE + m_origin_sector.y + 1, std::vector< std::vector<int> > ());

    while(pt1.y + m_origin_sector.y * SECTOR_SIZE < 0)
    {
        m_sectors.insert(m_sectors.begin(), std::vector< std::vector<int> > ());
        m_origin_sector.y++;
    }

    if(pt1.x + m_origin_sector.x * SECTOR_SIZE >= m_sectors[pt1.y/SECTOR_SIZE + m_origin_sector.y].size() * SECTOR_SIZE)
        m_sectors[pt1.y/SECTOR_SIZE + m_origin_sector.y].resize(pt1.x/SECTOR_SIZE + m_origin_sector.x + 1, std::vector<int> ());


    while(pt1.x + m_origin_sector.x * SECTOR_SIZE < 0)
    {
        for(unsigned i = 0 ; i  < m_sectors.size() ; ++i)
            m_sectors[i].insert(m_sectors[i].begin(), std::vector<int> ());
        m_origin_sector.x++;
    }

    m_sectors[pt1.y/SECTOR_SIZE + m_origin_sector.y][pt1.x/SECTOR_SIZE + m_origin_sector.x].push_back(m_wall.size()-1);

    return Wall_Entity(m_wall.size()-1);
}

Wall_Entity Light_Manager::Add_Wall(sf::Vector2f pt1,sf::Vector2f pt2,int hauteur)
{
    m_wall.push_back(Wall (pt1,pt2,hauteur));

    if(pt1.y + m_origin_sector.y * SECTOR_SIZE >= m_sectors.size() * SECTOR_SIZE)
        m_sectors.resize(pt1.y/SECTOR_SIZE + m_origin_sector.y + 1, std::vector< std::vector<int> > ());

    while(pt1.y + m_origin_sector.y * SECTOR_SIZE < 0)
    {
        m_sectors.insert(m_sectors.begin(), std::vector< std::vector<int> > ());
        m_origin_sector.y++;
    }

    if(pt1.x + m_origin_sector.x * SECTOR_SIZE >= m_sectors[pt1.y/SECTOR_SIZE + m_origin_sector.y].size() * SECTOR_SIZE)
        m_sectors[pt1.y/SECTOR_SIZE + m_origin_sector.y].resize(pt1.x/SECTOR_SIZE + m_origin_sector.x + 1, std::vector<int> ());


    while(pt1.x + m_origin_sector.x * SECTOR_SIZE < 0)
    {
        for(unsigned i = 0 ; i  < m_sectors.size() ; ++i)
            m_sectors[i].insert(m_sectors[i].begin(), std::vector<int> ());
        m_origin_sector.x++;
    }

    int x = pt1.x/SECTOR_SIZE + m_origin_sector.x;
    int y = pt1.y/SECTOR_SIZE + m_origin_sector.y;

    m_sectors[y][x].push_back(m_wall.size()-1);

    for(int w = 0 ; w < (int)m_sectors[y][x].size() ; ++w)
    {
        if(fabs(m_wall[m_sectors[y][x][w]].pt1.x - m_wall.back().pt1.x) < 5
        && fabs(m_wall[m_sectors[y][x][w]].pt1.y - m_wall.back().pt1.y) < 5)
        {
            int m = m_wall.back().hauteur1 + m_wall[m_sectors[y][x][w]].hauteur1;
            m_wall.back().hauteur1 = m/2;
            m_wall[m_sectors[y][x][w]].hauteur1 = m/2;
            m_wall[m_sectors[y][x][w]].pt1 = m_wall.back().pt1;
        }
        if(fabs(m_wall[m_sectors[y][x][w]].pt1.x - m_wall.back().pt2.x) < 5
        && fabs(m_wall[m_sectors[y][x][w]].pt1.y - m_wall.back().pt2.y) < 5)
        {
            int m = m_wall.back().hauteur2 + m_wall[m_sectors[y][x][w]].hauteur1;
            m_wall.back().hauteur2 = m/2;
            m_wall[m_sectors[y][x][w]].hauteur1 = m/2;
            m_wall[m_sectors[y][x][w]].pt1 = m_wall.back().pt2;
        }
        if(fabs(m_wall[m_sectors[y][x][w]].pt2.x - m_wall.back().pt1.x) < 5
        && fabs(m_wall[m_sectors[y][x][w]].pt2.y - m_wall.back().pt1.y) < 5)
        {
            int m = m_wall.back().hauteur1 + m_wall[m_sectors[y][x][w]].hauteur2;
            m_wall.back().hauteur1 = m/2;
            m_wall[m_sectors[y][x][w]].hauteur2 = m/2;
            m_wall[m_sectors[y][x][w]].pt2 = m_wall.back().pt1;
        }
        if(fabs(m_wall[m_sectors[y][x][w]].pt2.x - m_wall.back().pt2.x) < 5
        && fabs(m_wall[m_sectors[y][x][w]].pt2.y - m_wall.back().pt2.y) < 5)
        {
            int m = m_wall.back().hauteur2 + m_wall[m_sectors[y][x][w]].hauteur2;
            m_wall.back().hauteur2 = m/2;
            m_wall[m_sectors[y][x][w]].hauteur2 = m/2;
            m_wall[m_sectors[y][x][w]].pt2 = m_wall.back().pt2;
        }
    }

    return Wall_Entity(m_wall.size()-1);
}

// On désactive une lumière
void Light_Manager::Delete_Light(Light_Entity e)
{
    if (e.Dynamic())
    {
        if (e.ID()>=0&&e.ID()<(int)m_DynamicLight.size())
            m_DynamicLight[e.ID()].m_actif=false;
    }
    else
        if (e.ID()>=0&&e.ID()<(int)m_StaticLight.size())
            m_StaticLight[e.ID()].m_actif=false;
}

// On supprime un mur
void Light_Manager::Delete_Wall(Wall_Entity e)
{
    m_wall.erase(m_wall.begin()+e.ID());
}


void Light_Manager::Delete_All_Wall()
{
    m_wall.clear();
    m_sectors.clear();

    m_origin_sector.x = 0;
    m_origin_sector.y = 0;
}
void Light_Manager::Delete_All_Light(bool justDynamic)
{
    if (!justDynamic)
    {
        for (Iter=m_StaticLight.begin();Iter!=m_StaticLight.end();++Iter)
            Iter->Clear();
        m_StaticLight.clear();
    }

    for (Iter=m_DynamicLight.begin();Iter!=m_DynamicLight.end();++Iter)
        Iter->Clear();
    m_DynamicLight.clear();
}

// On calcule toutes les lumières dynamiques actives

void Light_Manager::Generate()
{
    if (configuration->Lumiere>0)
        for (Iter=m_DynamicLight.begin();Iter!=m_DynamicLight.end();++Iter)
            if (Iter->m_actif)
                Iter->Generate(m_wall, m_sectors, m_origin_sector);
}

void Light_Manager::Generate(Light_Entity &e)
{
    if (configuration->Lumiere>0)
        if (e.Dynamic())
            if (e.ID()>=0&&e.ID()<(int)m_DynamicLight.size())
                if (m_DynamicLight[e.ID()].m_actif)
                    m_DynamicLight[e.ID()].Generate(m_wall, m_sectors, m_origin_sector);
}

/*void Light_Manager::GenerateWallShadow(float angle,Lumiere soleil)
{
    angle-=90;

    sf::Vector2f vect(cos(angle*M_PI/180) * (100-soleil.hauteur) * 0.02,
                      sin(angle*M_PI/180) * (100-soleil.hauteur) * 0.01);

    for (std::vector<Wall>::iterator IterWall=m_wall.begin();IterWall!=m_wall.end();++IterWall)
    {
        IterWall->m_shadow = sf::Shape();

        IterWall->m_shadow.AddPoint(sf::Vector2f(IterWall->pt2.x,IterWall->pt2.y*0.5),sf::Color(0,0,0,(int)(255)));
        IterWall->m_shadow.AddPoint(sf::Vector2f(IterWall->pt1.x,IterWall->pt1.y*0.5),sf::Color(0,0,0,(int)(255)));

        IterWall->m_shadow.AddPoint(sf::Vector2f(IterWall->pt1.x-IterWall->hauteur * vect.x,
                                                 IterWall->pt1.y*0.5+IterWall->hauteur * vect.y),sf::Color(0,0,0,(int)(255)));
        IterWall->m_shadow.AddPoint(sf::Vector2f(IterWall->pt2.x-IterWall->hauteur * vect.x,
                                                 IterWall->pt2.y*0.5+IterWall->hauteur * vect.y),sf::Color(0,0,0,(int)(255)));
    }
}*/

// On affiche toutes les lumières actives
void Light_Manager::Draw(sf::RenderTarget *App,sf::View *camera)
{
    for (Iter=m_DynamicLight.begin();Iter!=m_DynamicLight.end();++Iter)
        if (Iter->m_actif)
            if (Iter->GetPosition().x + Iter->GetRadius() > GetViewRect(*camera).Left
             && Iter->GetPosition().x - Iter->GetRadius() < GetViewRect(*camera).Left + GetViewRect(*camera).Width
             && Iter->GetPosition().y + Iter->GetRadius() > 2 *  GetViewRect(*camera).Top
             && Iter->GetPosition().y - Iter->GetRadius() < 2 * (GetViewRect(*camera).Top + GetViewRect(*camera).Height))
                Iter->Draw(App);

    for (Iter=m_StaticLight.begin();Iter!=m_StaticLight.end();++Iter)
        if (Iter->m_actif)
            if (Iter->GetPosition().x + Iter->GetRadius() > GetViewRect(*camera).Left
             && Iter->GetPosition().x - Iter->GetRadius() < GetViewRect(*camera).Left + GetViewRect(*camera).Width
             && Iter->GetPosition().y + Iter->GetRadius() > 2 *  GetViewRect(*camera).Top
             && Iter->GetPosition().y - Iter->GetRadius() < 2 * (GetViewRect(*camera).Top + GetViewRect(*camera).Height))
                Iter->Draw(App);
}

void Light_Manager::DrawWallShadow(sf::RenderTarget *App,sf::View *camera,float angle,Lumiere soleil)
{
    angle-=90;

    sf::Vector2f vect(cos(angle*M_PI/180) * (100-soleil.hauteur) * 0.02,
                      sin(angle*M_PI/180) * (100-soleil.hauteur) * 0.01);

    for(int y =  GetViewRect(*camera).Top * 2 / SECTOR_SIZE + m_origin_sector.y - 1;
            y < (GetViewRect(*camera).Top * 2 + GetViewRect(*camera).Height * 2) / SECTOR_SIZE + 1 + m_origin_sector.y;
          ++y)
    if(y >= 0 && y < (int)m_sectors.size())
    for(int x =  GetViewRect(*camera).Left / SECTOR_SIZE + m_origin_sector.x - 1;
            x < (GetViewRect(*camera).Left + GetViewRect(*camera).Width) / SECTOR_SIZE + 1 + m_origin_sector.x;
          ++x)
    if(x >= 0 && x < (int)m_sectors[y].size())
    for (std::vector<int>::iterator IterWall=m_sectors[y][x].begin();
                                    IterWall!=m_sectors[y][x].end();++IterWall)
     {
        m_wall[*IterWall].m_shadow = sf::Shape();

        m_wall[*IterWall].m_shadow.AddPoint(sf::Vector2f(m_wall[*IterWall].pt2.x,m_wall[*IterWall].pt2.y*0.5),sf::Color(0,0,0,(int)(255)));
        m_wall[*IterWall].m_shadow.AddPoint(sf::Vector2f(m_wall[*IterWall].pt1.x,m_wall[*IterWall].pt1.y*0.5),sf::Color(0,0,0,(int)(255)));

        m_wall[*IterWall].m_shadow.AddPoint(sf::Vector2f(m_wall[*IterWall].pt1.x-m_wall[*IterWall].hauteur1 * vect.x,
                                                 m_wall[*IterWall].pt1.y*0.5+m_wall[*IterWall].hauteur1 * vect.y),sf::Color(0,0,0,(int)(255)));
        m_wall[*IterWall].m_shadow.AddPoint(sf::Vector2f(m_wall[*IterWall].pt2.x-m_wall[*IterWall].hauteur2 * vect.x,
                                                 m_wall[*IterWall].pt2.y*0.5+m_wall[*IterWall].hauteur2 * vect.y),sf::Color(0,0,0,(int)(255)));
        App->Draw(m_wall[*IterWall].m_shadow);
     }
}

void Light_Manager::Draw(sf::RenderTarget *App,Light_Entity e)
{
    if (e.Dynamic())
        if (e.ID()>=0&&e.ID()<(int)m_DynamicLight.size())
            if (m_DynamicLight[e.ID()].m_actif)
                m_DynamicLight[e.ID()].Draw(App);
}



// On modifie ou récupère les attributs de lumières dynamiques

void Light_Manager::SetPosition(Light_Entity &e, sf::Vector2f p)
{
    if (e.Dynamic())
        if (e.ID()>=0&&e.ID()<(int)m_DynamicLight.size())
            m_DynamicLight[e.ID()].SetPosition(p);
}
void Light_Manager::SetQuality(Light_Entity &e, int q)
{
    if (e.Dynamic())
        if (e.ID()>=0&&e.ID()<(int)m_DynamicLight.size())
            m_DynamicLight[e.ID()].SetQuality(q);
}
void Light_Manager::SetRadius(Light_Entity &e, int r)
{
    if (e.Dynamic())
        if (e.ID()>=0&&e.ID()<(int)m_DynamicLight.size())
            m_DynamicLight[e.ID()].SetRadius(r);
}
void Light_Manager::SetColor(Light_Entity &e, sf::Color c)
{
    if (e.Dynamic())
        if (e.ID()>=0&&e.ID()<(int)m_DynamicLight.size())
            m_DynamicLight[e.ID()].SetColor(c);
}
void Light_Manager::SetIntensity(Light_Entity &e, int i)
{
    if (e.Dynamic())
        if (e.ID()>=0&&e.ID()<(int)m_DynamicLight.size())
            m_DynamicLight[e.ID()].SetIntensity(i);
}
void Light_Manager::SetMovingLight(Light_Entity &e, bool m)
{
    if (e.Dynamic())
        if (e.ID()>=0&&e.ID()<(int)m_DynamicLight.size())
            m_DynamicLight[e.ID()].SetMovingLight(m);
}

void Light_Manager::SetIntensity(Wall_Entity &e, int i)
{
    if (e.ID()>=0&&e.ID()<(int)m_wall.size())
    {
        if(i < 0)
            m_wall[e.ID()].actif = true;
        else
            m_wall[e.ID()].actif = false;
    }

}



float Light_Manager::GetIntensity(Light_Entity e)
{
    if (e.Dynamic())
        if (e.ID()>=0&&e.ID()<(int)m_DynamicLight.size())
            return m_DynamicLight[e.ID()].GetIntensity();
    return 0;
}
float Light_Manager::GetRadius(Light_Entity e)
{
    if (e.Dynamic())
        if (e.ID()>=0&&e.ID()<(int)m_DynamicLight.size())
            return m_DynamicLight[e.ID()].GetRadius();
    return 0;
}
int Light_Manager::GetQuality(Light_Entity e)
{
    if (e.Dynamic())
        if (e.ID()>=0&&e.ID()<(int)m_DynamicLight.size())
            return m_DynamicLight[e.ID()].GetQuality();

    return 0;
}
sf::Color Light_Manager::GetColor(Light_Entity e)
{
    if (e.Dynamic())
        if (e.ID()>=0&&e.ID()<(int)m_DynamicLight.size())
            return m_DynamicLight[e.ID()].GetColor();

    return sf::Color (0,0,0);
}
sf::Vector2f Light_Manager::GetPosition(Light_Entity e)
{
    if (e.Dynamic())
        if (e.ID()>=0&&e.ID()<(int)m_DynamicLight.size())
            return m_DynamicLight[e.ID()].GetPosition();

    return sf::Vector2f (0,0);
}



// On modifie ou récupère différents attributs des murs

void Light_Manager::SetPosition(Wall_Entity e, sf::Vector2f p)
{
    if (e.ID()>=0&&e.ID()<(int)m_wall.size())
    {
        sf::Vector2f buffer=m_wall[e.ID()].pt1;
        m_wall[e.ID()].pt1=p;
        m_wall[e.ID()].pt2.x= m_wall[e.ID()].pt2.x + (p.x-buffer.x);
        m_wall[e.ID()].pt2.y= m_wall[e.ID()].pt2.y + (p.y-buffer.y);
    }
}

sf::Vector2f Light_Manager::GetPt(Wall_Entity e, int nbr)
{
    if (e.ID()>=0&&e.ID()<(int)m_wall.size())
    {
        if(nbr == 2)
            return m_wall[e.ID()].pt2;
        else
            return m_wall[e.ID()].pt1;
    }

    return sf::Vector2f (0,0);
}



