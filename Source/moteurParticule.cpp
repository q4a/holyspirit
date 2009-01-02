
#include "moteurParticule.h"
#include "globale.h"
#include <fstream>



ParticuleSysteme::ParticuleSysteme()
{

}
ParticuleSysteme::ParticuleSysteme(int modeleInt, ModeleParticuleSysteme *modele, coordonnee position, sf::Color color,float force,float angle)
{
     m_modele=modeleInt;
     Generer(force,modele,position,angle);
     m_color=color;
     m_son=modele->m_son;
}

ParticuleSysteme::~ParticuleSysteme()
{
    m_particules.clear();
}
void ParticuleSysteme::Afficher(sf::RenderWindow *ecran, ModeleParticuleSysteme *modele)
{
    for(int i=0;i<m_particules.size();i++)
    {
        sf::Sprite sprite;
        if(m_particules[i].numero>=0&&m_particules[i].numero<modele->m_particules.size())
        {
            sprite.SetImage(*moteurGraphique.getImage(modele->m_image));
            sprite.SetSubRect(sf::IntRect(modele->m_particules[m_particules[i].numero].positionImage.x,modele->m_particules[m_particules[i].numero].positionImage.y,modele->m_particules[m_particules[i].numero].positionImage.x+modele->m_particules[m_particules[i].numero].positionImage.w,modele->m_particules[m_particules[i].numero].positionImage.y+modele->m_particules[m_particules[i].numero].positionImage.h));
            sprite.SetCenter(modele->m_particules[m_particules[i].numero].positionImage.w/2,modele->m_particules[m_particules[i].numero].positionImage.h/2);
            sprite.SetRotation(m_particules[i].rotation);
            float scale=m_particules[i].position.z/200+1;
            sprite.SetScale(scale,scale);
            sprite.SetX(m_particules[i].position.x);
            sprite.SetY(m_particules[i].position.y-m_particules[i].position.z);
            sprite.SetColor(sf::Color(m_particules[i].color.r,m_particules[i].color.g,m_particules[i].color.b,(int)m_particules[i].alpha));
            if(m_particules[i].position.z>32)
                moteurGraphique.AjouterCommande(&sprite,12,1);
            else
                moteurGraphique.AjouterCommande(&sprite,10,1);
        }
    }
}
void ParticuleSysteme::Generer(float force, ModeleParticuleSysteme *modele,coordonnee position,float angle)
{
    for(int i=0;i<modele->m_particules.size();i++)
    {
        int nombre=(rand() % (modele->m_particules[i].max - modele->m_particules[i].min + 1)) + modele->m_particules[i].min;
        for(int j=0;j<nombre;j++)
        {
            angle+= (180-(rand() % 360))*force/100;
            m_particules.push_back(Particule ());
            m_particules[m_particules.size()-1].vie=100;
            m_particules[m_particules.size()-1].position.x=position.x;
            m_particules[m_particules.size()-1].position.y=position.y;
            m_particules[m_particules.size()-1].position.z=(rand() % (64 - 0 + 1)) + 0;
            m_particules[m_particules.size()-1].vecteur.x=cos(angle*M_PI/180);
            m_particules[m_particules.size()-1].vecteur.y=sin(angle*M_PI/180)/2;
            m_particules[m_particules.size()-1].vecteur.z=(rand() % (int)(force*0.4 - force*0.6 + 1)) + force*0.4;
            m_particules[m_particules.size()-1].vitesse=(rand() % (int)(force*1.25 - force*0.75 + 1)) + force*0.75;
            m_particules[m_particules.size()-1].vitesse_rotation=((rand() % (int)(force*1 - force*0.5 + 1)) + force*0.5)*5;
            if(m_particules[m_particules.size()-1].vitesse_rotation>100)
                m_particules[m_particules.size()-1].vitesse_rotation=100;
            m_particules[m_particules.size()-1].rotation=rand() % 360;
            m_particules[m_particules.size()-1].alpha=255;
            m_particules[m_particules.size()-1].numero=i;
        }
    }
}
bool ParticuleSysteme::Gerer(float temps,int tailleMapY)
{
    int efface=0;
    for(int i=0;i<m_particules.size();i++)
    {
        if(m_particules[i].vie==100)
        {
            m_particules[i].position.x+=m_particules[i].vecteur.x*m_particules[i].vitesse*temps*25;
            m_particules[i].position.y+=m_particules[i].vecteur.y*m_particules[i].vitesse*temps*25;
            m_particules[i].position.z+=m_particules[i].vecteur.z*temps*25;

            m_particules[i].vecteur.z-=temps*25;

            if(m_particules[i].position.z<=1&&m_particules[i].vie==100&&i%3==0)
            {
                coordonnee positionHero={0,0,0,0},position,positionCase;

                if((float)((m_particules[i].position.y*2-m_particules[i].position.x)/2)/64+tailleMapY/2<(float)tailleMapY/2)
                    positionCase.y=(int)((m_particules[i].position.y*2-m_particules[i].position.x)/2)/64+tailleMapY/2-1;
                else
                    positionCase.y=(int)((m_particules[i].position.y*2-m_particules[i].position.x)/2)/64+tailleMapY/2;

                positionCase.x=(int)(m_particules[i].position.x+((m_particules[i].position.y*2-m_particules[i].position.x)/2))/64-tailleMapY/2;

                position.x=-(positionCase.x-positionCase.y-1+tailleMapY)/5;
                position.y=(positionCase.x+positionCase.y)/5;

                if((int)(m_particules[i].vecteur.z*100)>10)
                    moteurSons.JouerSon(m_son,position,positionHero,0,(int)(m_particules[i].vecteur.z*100));
            }

            if(m_particules[i].position.z<0)
                m_particules[i].position.z=0,m_particules[i].vecteur.z=fabs(m_particules[i].vecteur.z)/4;

            m_particules[i].vitesse-=temps*10;
            m_particules[i].vitesse_rotation-=temps*50;
            if(m_particules[i].vitesse_rotation<0)
                m_particules[i].vitesse_rotation=0;

             m_particules[i].rotation+=m_particules[i].vitesse_rotation*temps*10;
        }

        if(m_particules[i].vitesse<=0)
            m_particules[i].vitesse=0,m_particules[i].vie-=temps*20;
        if(m_particules[i].vie<=0)
             m_particules[i].alpha-=temps*100;
        if(m_particules[i].alpha<=0)
            m_particules[i].alpha=0,efface++;


    }

    if(efface==m_particules.size())
        return 0;

    return 1;
}
