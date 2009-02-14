
#include <iostream>
#include <math.h>
#include "light.h"

Light::Light()
{
    m_intensity=0;
    m_radius=0;
    m_quality=0;
    m_actif=true;
}

Light::Light(sf::Vector2f position, float intensity, float radius, int quality, sf::Color color)
{
    m_position=position;
    m_intensity=intensity;
    m_radius=radius;
    m_color=color;
    m_quality=quality;
}

Light::~Light()
{
    m_shape.clear();
}


void Light::Draw(sf::RenderWindow *App)
{
    // On boucle sur m_shape pour afficher tous les triangles.
    for(int i=0;i<(int)m_shape.size();i++)
        App->Draw(m_shape[i]);
}


bool Light::CollisionWithPoint(sf::Vector2f p,sf::Vector2f pt1,sf::Vector2f pt2)
{
    // D'abord, on calcul les extrémités du rectangle dans lequel est inscrit le triangle
    sf::Vector2f min(0,0),max(0,0);
    if(pt1.x<min.x)
        min.x=pt1.x;
    if(pt1.x>max.x)
        max.x=pt1.x;
    if(pt1.y<min.y)
        min.y=pt1.y;
    if(pt1.y>max.y)
        max.y=pt1.y;

    if(pt2.x<min.x)
        min.x=pt2.x;
    if(pt2.x>max.x)
        max.x=pt2.x;
    if(pt2.y<min.y)
        min.y=pt2.y;
    if(pt2.y>max.y)
        max.y=pt2.y;

    // Si le point se trouve en dehors de ce rectangle, ça ne sert à rien d'aller plus loin, on quitte
    if(p.x<min.x || p.x>max.x || p.y<min.y || p.y>max.y)
        return 0;

    // Variable qui contiendra le coéficient angulaire
    float rapport;

    // Petit test pour éviter une division par 0
    if(pt1.x!=0)
    {
        // Le coéficient est égal au rapport entre pt1.y et p1.x, la formule étant DY/DX, mais on prend le point 0 et le point pt.
        // Donc ça donne (pt1.y-0)/(pt1.x-0), on peut retirer le 0 il ne sert à rien.
        rapport=(pt1.y/pt1.x);

        // Suivant si on se trouve à gauche ou à droite, on doit être plus grand ou plus petit.
        if( (pt1.x<0 && (p.x)*rapport<(p.y)) || (pt1.x>0 && (p.x)*rapport>(p.y)) )
            return 0;
    }

    //Idem
    if(pt2.x!=0)
    {
        rapport=(pt2.y/pt2.x);

        if( (pt2.x<=0 && (p.x)*rapport>(p.y)) || (pt2.x>0 && (p.x)*rapport<(p.y)) )
            return 0;
    }

    //Idem, mais avec le segment ["pt1","pt2"]
    if(pt1.x-pt2.x!=0)
    {
        rapport=(pt1.y-pt2.y)/(pt1.x-pt2.x);
        if( ( pt1.x>pt2.x && (p.x-pt2.x)*rapport<(p.y-pt2.y)) || ( pt1.x<pt2.x && (p.x-pt1.x)*rapport>(p.y-pt1.y)) )
            return 0;
    }
    else
    {
        if(pt1.x<0)
            if(p.x<pt1.x)
                return 0;

        if(pt1.x>0)
            if(p.x>pt1.x)
                return 0;
    }

    // On retourne 1 pour dire que le point se trouve dans le triangle
    return 1;
}

bool Light::CollisionWithLine(sf::Vector2f l1, sf::Vector2f l2,sf::Vector2f pt1,sf::Vector2f pt2)
{
    // On regarde si le mur traverse le triangle, j'ai laissé les 0 pour pouvoir mieux cerner la formule.


    float r = ((l1.y-pt1.y)*(0-pt1.x)-(l1.x-pt1.x)*(0-pt1.y))/((l2.x-l1.x)*(0-pt1.y)-(l2.y-l1.y)*(0-pt1.x));
    float s = ((l1.y-pt1.y)*(l2.x-l1.x)-(l1.x-pt1.x)*(l2.y-l1.y))/((l2.x-l1.x)*(0-pt1.y)-(l2.y-l1.y)*(0-pt1.x));

    // r et s représente ou l'on se trouve dans les deux segment à l'intersection. 0 est une extrémité du segment, 1 l'autre, r et s doivent donc être tout deux entre
    // 0 et 1 pour que le point d'intersection des deux droites soit un point des deux segments.
    if (0<r && r<1 && 0<s && s<1)
        return 1;

    r = ((l1.y-0)*(pt2.x-0)-(l1.x-0)*(pt2.y-0))/((l2.x-l1.x)*(pt2.y-0)-(l2.y-l1.y)*(pt2.x-0));
    s = ((l1.y-0)*(l2.x-l1.x)-(l1.x-0)*(l2.y-l1.y))/((l2.x-l1.x)*(pt2.y-0)-(l2.y-l1.y)*(pt2.x-0));

    if (0<r && r<1 && 0<s && s<1)
        return 1;


    r = ((l1.y-pt1.y)*(pt2.x-pt1.x)-(l1.x-pt1.x)*(pt2.y-pt1.y))/((l2.x-l1.x)*(pt2.y-pt1.y)-(l2.y-l1.y)*(pt2.x-pt1.x));
    s = ((l1.y-pt1.y)*(l2.x-l1.x)-(l1.x-pt1.x)*(l2.y-l1.y))/((l2.x-l1.x)*(pt2.y-pt1.y)-(l2.y-l1.y)*(pt2.x-pt1.x));

    if (0<r && r<1 && 0<s && s<1)
        return 1;

    return 0;
}

void Light::AddTriangle(sf::Vector2f pt1,sf::Vector2f pt2, int minimum_wall,std::vector <Wall> m_wall)
{
    // On boucle sur tous les murs
    for(int m=minimum_wall;m<(int)m_wall.size();m++)
    {
        // l1 et l2 sont les positions relative au centre de la lumière des deux extrémités du mur
        sf::Vector2f l1(m_wall[m].pt1.x-m_position.x,m_wall[m].pt1.y-m_position.y);
        sf::Vector2f l2(m_wall[m].pt2.x-m_position.x,m_wall[m].pt2.y-m_position.y);

        // Deux bool, elles contiendront "true" si l1 ou l2 se trouve dans le triangle
        bool Collision1 = false;
        bool Collision2 = false;

        // Si il n'y a pas encore eut d'intersection entre ce triangle et une des extrémité du mur.
        if(!m_wall[m].collision)
        {
            // Collision 1 et 2 prennent la veleur true si l1 ou l2 se trouve dans le triangle
            Collision1 = CollisionWithPoint(sf::Vector2f(l1.x,l1.y),pt1,pt2);
            Collision2 = CollisionWithPoint(sf::Vector2f(l2.x,l2.y),pt1,pt2);


            // Si l1 est dans le triangle
            if(Collision1)
            {
                // On donne true comme valeur pour qu'on ne s'occupe plus du fait qu'il y ait une extrémité du mur.
                m_wall[m].collision=true;


                // On calcule l'angle pour couper en deux le triangle, au niveau de l1.
                float angle;
                if(l1.x!=0)
                    angle=atan((l1.y)/(l1.x));
                else
                {
                    if(l1.y<0)
                        angle=M_PI/2;
                    else
                        angle=-M_PI/2;
                }

                if(l1.x<=0)
                    angle+=M_PI;

                // ptP est l'extrémité du segment qui coupe le triangle en deux au niveau de l1.
                sf::Vector2f ptP(cos(angle)*m_radius,sin(angle)*m_radius);

                // On calcul le point d'intersection entre le segment [(0,0),'ptP'].
                float s = ((pt1.y-0)*(ptP.x-0)-(pt1.x-0)*(ptP.y-0))/((pt2.x-pt1.x)*(ptP.y-0)-(pt2.y-pt1.y)*(ptP.x-0));

                sf::Vector2f pt;
                if(0<s && s<1)
                {
                    pt.x=pt1.x+s*(pt2.x-pt1.x);
                    pt.y=pt1.y+s*(pt2.y-pt1.y);
                }

                // On regarde si l'on est au dessus ou en dessous du mur, et si l1/l2 est à gauche/droite

                if((l1.x) < (l2.x) && 0>(l1.y) - (l1.x)*(((l1.y)-(l2.y))/((l1.x)-(l2.x)))
                    ||(l1.x) >= (l2.x) && 0<(l1.y) - (l1.x)*(((l1.y)-(l2.y))/((l1.x)-(l2.x))))
                {
                    // Si il n'y a pas les deux extrémités du murs dans ce même et unique triangle
                    if(!Collision2)
                    {
                        // On ajoute un triangle vers le mur
                        AddTriangle(pt,pt2,m,m_wall);

                        // On donne comme valeur à pt2 pt pour que le triangle soit maintenant plus que celui qui va vers l'extérieur du mur
                        pt2=pt;
                    }
                    else // Sinon, on ajoute un triangle vers l'extérieur du mur
                        AddTriangle(pt1,pt,m+1,m_wall);

                }
                else if((l1.x) < (l2.x) && 0<(l1.y) - (l1.x)*(((l1.y)-(l2.y))/((l1.x)-(l2.x)))
                    ||(l1.x) >= (l2.x) && 0>(l1.y) - (l1.x)*(((l1.y)-(l2.y))/((l1.x)-(l2.x))))
                {
                    // Idem qu'en haut
                    if(!Collision2)
                    {
                        AddTriangle(pt1,pt,m,m_wall);
                        pt1=pt;
                    }
                    else
                        AddTriangle(pt,pt2,m+1,m_wall);
                }

            }

            // Quasi idem que pour Collision 1
            if(Collision2)
            {
                m_wall[m].collision=true;

                float angle;
                if(l2.x!=0)
                    angle=atan((l2.y)/(l2.x));
                else
                {
                    if(l2.y<0)
                        angle=M_PI/2;
                    else
                        angle=-M_PI/2;
                }

                if(l2.x<=0)
                    angle+=M_PI;

                sf::Vector2f ptP(cos(angle)*m_radius,sin(angle)*m_radius);

                float s = ((pt1.y-0)*(ptP.x-0)-(pt1.x-0)*(ptP.y-0))/((pt2.x-pt1.x)*(ptP.y-0)-(pt2.y-pt1.y)*(ptP.x-0));

                sf::Vector2f pt=pt1;
                if(0<s && s<1)
                {
                    pt.x=pt1.x+s*(pt2.x-pt1.x);
                    pt.y=pt1.y+s*(pt2.y-pt1.y);
                }


                if((l1.x) < (l2.x) && 0<(l1.y) - (l1.x)*(((l1.y)-(l2.y))/((l1.x)-(l2.x)))
                    ||(l1.x) >= (l2.x) && 0>(l1.y) - (l1.x)*(((l1.y)-(l2.y))/((l1.x)-(l2.x))))
                {
                    if(!Collision1)
                    {
                        AddTriangle(pt,pt2,m,m_wall);
                        pt2=pt;
                    }
                    else
                    {
                        // Juste ici, qu'en plus de dire que l'on rajoute un triangle vers l'extérieur, on dit que le triangle actuel à pour extrémités celles du mur
                        AddTriangle(pt1,pt,m+1,m_wall);
                        pt1=l2,pt2=l1;
                    }
                }
                else if((l1.x) < (l2.x) && 0>(l1.y) - (l1.x)*(((l1.y)-(l2.y))/((l1.x)-(l2.x)))
                    ||(l1.x) >= (l2.x) && 0<(l1.y) - (l1.x)*(((l1.y)-(l2.y))/((l1.x)-(l2.x))))
                {
                    if(!Collision1)
                    {
                        AddTriangle(pt1,pt,m,m_wall);
                        pt1=pt;
                    }
                    else
                    {
                        // Idem
                        AddTriangle(pt,pt2,m+1,m_wall);
                        pt1=l1,pt2=l2;
                    }
                }

            }
        }

        // Si il n'y a pas d'extrémité du mur dans ce triangle et si il y a une collision avec le mur
        if(!Collision1 && !Collision2)
            if(CollisionWithLine(sf::Vector2f(m_wall[m].pt1.x-m_position.x,m_wall[m].pt1.y-m_position.y),sf::Vector2f(m_wall[m].pt2.x-m_position.x,m_wall[m].pt2.y-m_position.y),pt1,pt2))
            {
                // p1 et p2 vont représenter pt1 et pt2 une fois coupé au niveau du mur
                sf::Vector2f p1=pt1;
                sf::Vector2f p2=pt2;


                // On calcul l'intersection entre [(0,0) , 'pt1' ] et le mur
                float s = ((0-l1.y)*(l2.x-l1.x)-(0-l1.x)*(l2.y-l1.y))/((pt1.x-0)*(l2.y-l1.y)-(pt1.y-0)*(l2.x-l1.x));

                // Si elle se trouve sur le segment [(0,0) , 'pt1' ], p1 prend comme valeur ce point d'intersection, sinon, p1 garde comme valeur pt1
                if(0<s && s<1)
                {
                    p1.x=s*pt1.x;
                    p1.y=s*pt1.y;
                }

                // Idem, mais avec p2 et pt2
                s = ((0-l1.y)*(l2.x-l1.x)-(0-l1.x)*(l2.y-l1.y))/((pt2.x-0)*(l2.y-l1.y)-(pt2.y-0)*(l2.x-l1.x));

                if(0<s && s<1)
                {
                    p2.x=s*pt2.x;
                    p2.y=s*pt2.y;
                }

                // On calcul l'intersection entre ['pt1' , 'pt2' ] et le mur
                s = ((pt1.y-l1.y)*(l2.x-l1.x)-(pt1.x-l1.x)*(l2.y-l1.y))/((pt2.x-pt1.x)*(l2.y-l1.y)-(pt2.y-pt1.y)*(l2.x-l1.x));

                // Si il y a intersection
                if(0<s && s<1)
                {
                    sf::Vector2f p3;
                    p3.x=pt1.x+s*(pt2.x-pt1.x);
                    p3.y=pt1.y+s*(pt2.y-pt1.y);

                    // On coupe le triangle en deux au niveau de p3
                    AddTriangle(p1,p3,m+1,m_wall);
                    pt1=p3;
                    pt2=p2;
                }
                else
                {
                    // Sinon, le triangle actuel prend pour coordonner les intersection calculées plus haut
                    pt1=p1;
                    pt2=p2;
                }
            }
    }

    // Variable qui contiendra l'intensité calculée, pour le dégradé
    float intensity;

    // On ajoute un shape
    m_shape.push_back(sf::Shape ());

    // On lui donne comme point de départ (0,0), le centre de la lumière, avec la couleur et intensité maximal
    m_shape.back().AddPoint(0, 0,  sf::Color((int)(m_intensity*m_color.r/255),(int)(m_intensity*m_color.g/255),(int)(m_intensity*m_color.b/255)));

    // On calcul ou l'on se trouve par rapport au centre, pour savoir à quel intensité on est
    intensity=m_intensity-sqrt(pt1.x*pt1.x + pt1.y*pt1.y)*m_intensity/m_radius;
    // Et on ajoute un  point au shape
    m_shape.back().AddPoint(pt1.x, pt1.y,  sf::Color((int)(intensity*m_color.r/255),(int)(intensity*m_color.g/255),(int)(intensity*m_color.b/255)));

    // Idem
    intensity=m_intensity-sqrt(pt2.x*pt2.x + pt2.y*pt2.y)*m_intensity/m_radius;
    m_shape.back().AddPoint(pt2.x, pt2.y,  sf::Color((int)(intensity*m_color.r/255),(int)(intensity*m_color.g/255),(int)(intensity*m_color.b/255)));

    // On met que le shape soit en Add et on lui donne sa position
    m_shape.back().SetBlendMode(sf::Blend::Add);
    m_shape.back().SetPosition(m_position);

    /* m_shape.back().SetPointOutlineColor(0, sf::Color(255, 255, 255));
    m_shape.back().SetPointOutlineColor(1, sf::Color(255, 255, 255));
    m_shape.back().SetPointOutlineColor(2, sf::Color(255, 255, 255));
    m_shape.back().SetOutlineWidth(1);*/
}

void Light::Generate(std::vector <Wall> m_wall)
{
    // On vide la mémoire
    m_shape.clear();

    // buf est l'angle de chaque triangle, c'est donc 2pi divisé par le nombre de triangles
    float buf=(M_PI*2)/(float)m_quality;

    // On ajoute tous les triangles qui composent la lumière
    for(int i=0;i<m_quality;i++)
    {
        for(int m=0;m<(int)m_wall.size();m++)
            m_wall[m].collision=false;

        AddTriangle(sf::Vector2f((int)((float)m_radius*cos((float)i*buf)),(int)((float)m_radius*sin((float)i*buf))) , sf::Vector2f((int)((float)m_radius*cos((float)(i+1)*buf)),(int)((float)m_radius*sin((float)(i+1)*buf))),0,m_wall);
    }
}


// Différentes fonctions pour modifier les attributs de la lumière, et pour les récupérer

void Light::SetIntensity(float intensity) { m_intensity=intensity; }
void Light::SetRadius(float radius) { m_radius=radius; }
void Light::SetQuality(int quality) { m_quality=quality; }
void Light::SetColor(sf::Color color) { m_color=color; }
void Light::SetPosition(sf::Vector2f position) { m_position=position; }



float Light::GetIntensity(){ return m_intensity; }
float Light::GetRadius(){ return m_radius; }
int Light::GetQuality(){ return m_quality; }
sf::Vector2f Light::GetPosition(){ return m_position;}
sf::Color Light::GetColor(){ return m_color; }


