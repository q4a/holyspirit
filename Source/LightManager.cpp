
#include "LightManager.h"

Light_Manager::Light_Manager()
{

}
Light_Manager::~Light_Manager()
{
    m_wall.clear();
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
Light_Entity Light_Manager::Add_Dynamic_Light(sf::Vector2f position, float intensity, float radius, int quality, sf::Color color)
{
    m_DynamicLight.push_back(Light (position,intensity,radius,quality,color));
    return Light_Entity((int)m_DynamicLight.size()-1,true);
}


// Notez bien qu'on calcule les lumières statiques lors de leur ajout.

Light_Entity Light_Manager::Add_Static_Light(Light light)
{
    m_StaticLight.push_back(light);
    m_StaticLight.back().Generate(m_wall);
    return Light_Entity((int)m_StaticLight.size()-1,false);
}
Light_Entity Light_Manager::Add_Static_Light(sf::Vector2f position, float intensity, float radius, int quality, sf::Color color)
{
    m_StaticLight.push_back(Light (position,intensity,radius,quality,color));
    m_StaticLight.back().Generate(m_wall);
    return Light_Entity((int)m_StaticLight.size()-1,false);
}

// Ajouter un mur

void Light_Manager::AddWall(sf::Vector2f pt1,sf::Vector2f pt2)
{
    // On teste voir si un autre mur n'a pas comme point d'extrémité celui du nouveau, si c'est le cas, on décale celui-ci.
    // Pour le moment, plusieurs murs ayant une extrémité en un même point fait buger la lumières. Je compte régler ce problème dans la prochaine version.
    for(int i=0;i<(int) m_wall.size();i++)
    {
        if(m_wall[i].pt1==pt1)
            pt1.x+=0.0001;
        if(m_wall[i].pt2==pt1)
            pt1.x+=0.0001;
        if(m_wall[i].pt1==pt2)
            pt2.x+=0.0001;
        if(m_wall[i].pt1==pt2)
            pt2.x+=0.0001;
    }
    m_wall.push_back(Wall (pt1,pt2));
}

// On désactive une lumière
bool Light_Manager::Delete_Light(Light_Entity e)
{
    if(e.Dynamic())
        if(e.ID()>=0&&e.ID()<(int)m_DynamicLight.size())
            m_DynamicLight[e.ID()].m_actif=false;
    else
        if(e.ID()>=0&&e.ID()<(int)m_StaticLight.size())
            m_StaticLight[e.ID()].m_actif=false;
}

// On supprime un mur
bool Light_Manager::Delete_Wall(Wall_Entity e)
{
    m_wall.erase(m_wall.begin()+e.ID());
}

// On calcule toutes les lumières dynamiques actives
void Light_Manager::Generate()
{
    for(int i=0;i<(int)m_DynamicLight.size();i++)
        if(m_DynamicLight[i].m_actif)
            m_DynamicLight[i].Generate(m_wall);
}


// On affiche toutes les lumières actives
void Light_Manager::Draw(sf::RenderWindow *App)
{
    for(int i=0;i<(int)m_DynamicLight.size();i++)
        if(m_DynamicLight[i].m_actif)
            m_DynamicLight[i].Draw(App);
    for(int i=0;i<(int)m_StaticLight.size();i++)
        if(m_StaticLight[i].m_actif)
            m_StaticLight[i].Draw(App);
}

// On modifie ou récupère les attributs de lumières dynamiques

void Light_Manager::SetPosition(Light_Entity e, sf::Vector2f p)
{
    if(e.Dynamic())
        if(e.ID()>=0&&e.ID()<(int)m_DynamicLight.size())
            m_DynamicLight[e.ID()].SetPosition(p);
}
void Light_Manager::SetQuality(Light_Entity e, int q)
{
    if(e.Dynamic())
        if(e.ID()>=0&&e.ID()<(int)m_DynamicLight.size())
            m_DynamicLight[e.ID()].SetQuality(q);
}
void Light_Manager::SetRadius(Light_Entity e, int r)
{
    if(e.Dynamic())
        if(e.ID()>=0&&e.ID()<(int)m_DynamicLight.size())
            m_DynamicLight[e.ID()].SetRadius(r);
}
void Light_Manager::SetColor(Light_Entity e, sf::Color c)
{
    if(e.Dynamic())
        if(e.ID()>=0&&e.ID()<(int)m_DynamicLight.size())
            m_DynamicLight[e.ID()].SetColor(c);
}
void Light_Manager::SetIntensity(Light_Entity e, int i)
{
    if(e.Dynamic())
        if(e.ID()>=0&&e.ID()<(int)m_DynamicLight.size())
            m_DynamicLight[e.ID()].SetIntensity(i);
}


float Light_Manager::GetIntensity(Light_Entity e)
{
    if(e.Dynamic())
        if(e.ID()>=0&&e.ID()<(int)m_DynamicLight.size())
            return m_DynamicLight[e.ID()].GetIntensity();
}
float Light_Manager::GetRadius(Light_Entity e)
{
    if(e.Dynamic())
        if(e.ID()>=0&&e.ID()<(int)m_DynamicLight.size())
            return m_DynamicLight[e.ID()].GetRadius();
}
int Light_Manager::GetQuality(Light_Entity e)
{
    if(e.Dynamic())
        if(e.ID()>=0&&e.ID()<(int)m_DynamicLight.size())
            return m_DynamicLight[e.ID()].GetQuality();
}
sf::Color Light_Manager::GetColor(Light_Entity e)
{
    if(e.Dynamic())
        if(e.ID()>=0&&e.ID()<(int)m_DynamicLight.size())
            return m_DynamicLight[e.ID()].GetColor();
}
sf::Vector2f Light_Manager::GetPosition(Light_Entity e)
{
    if(e.Dynamic())
        if(e.ID()>=0&&e.ID()<(int)m_DynamicLight.size())
            return m_DynamicLight[e.ID()].GetPosition();
}
