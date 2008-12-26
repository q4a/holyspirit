
#include "MoteurSons.h"
#include "Globale.h"

#include <iostream>


MoteurSons::MoteurSons()
{
    sonEnCours=0;

    for(int i=0;i<NOMBRE_SONS;i++)
        m_IDSons[i]=-1;
}
MoteurSons::~MoteurSons()
{
    //m_sons.clear();
	//m_sonAZero.clear();
	//m_sonIDBuffer.clear();
	m_buffers.clear();
	m_cheminsSons.clear();
}

void MoteurSons::Gerer()
{
}

void MoteurSons::Vider()
{
//    for(int i=0;i<m_sonAZero.size();i++)
 //       m_sonAZero[i]=1;
}

/*int MoteurSons::AjouterSon(int ID)
{
    if(ID>=0&&ID<m_buffers.size())
    {
        int nbrSonsIdentiques=0,num=-1;

        for(int i=0;i<m_sonIDBuffer.size();i++)
            if(m_sonIDBuffer[i]==ID)
                nbrSonsIdentiques++,num=i;

        if(nbrSonsIdentiques<5)
        {
            m_sons.push_back(sf::Sound ());
            m_sonAZero.push_back(1);

            m_sons[m_sons.size()-1].SetBuffer(m_buffers[ID]);
            m_sons[m_sons.size()-1].SetVolume(100);
        }
        else
            return num;

        return m_sons.size()-1;
    }
    else
        return 0;
}*/

int MoteurSons::AjouterBuffer(std::string chemin)
{
   for(int i=0;i<m_cheminsSons.size();i++)
        if(m_cheminsSons[i]==chemin)
            return i;

    m_buffers.push_back(sf::SoundBuffer ());
    m_cheminsSons.push_back(chemin);

    if(!m_buffers[m_buffers.size()-1].LoadFromFile(chemin.c_str()))
    {
        console.Ajouter("Impossible de charger : "+chemin,1);
        return -1;
    }
    else
        console.Ajouter("Chargement de : "+chemin,0);

    return m_buffers.size()-1;
}

void MoteurSons::JouerSon(int ID,coordonnee position,coordonnee positionHero,bool unique)
{
   /* if(ID>=0&&ID<m_sons.size())
    {
        if(m_sonAZero[ID])
        {
            m_sons[ID].SetVolume(100);
            m_sonAZero[ID]=false;
            m_sons[ID].SetPosition(position.x,0,position.y);
        }
        else
        {
            m_sons[ID].SetVolume(100);
            float x,y,z;
            x=m_sons[ID].GetPosition().x;
            y=m_sons[ID].GetPosition().y;
            z=m_sons[ID].GetPosition().z;
            // Je test voir si le nouveau son du même type est plus près du perso que l'ancien, si oui, je mets la position du nouveau à la place de l'ancien
            if((double)(gpl::sqrt((positionHero.x+x)*(positionHero.x+x)+(positionHero.y-y)*(positionHero.y-y)))>(double)(gpl::sqrt((positionHero.x-position.x)*(positionHero.x-position.x)+(positionHero.y-position.y)*(positionHero.y-position.y))))
                m_sons[ID].SetPosition(position.x,0,position.y);
        }

        sf::Sound::Status Status = m_sons[ID].GetStatus();

        if(Status==0)
            m_sons[ID].Play();
    }*/

    if(ID>=0&&ID<m_buffers.size())
    {
        bool creerNouveauSon=true;
        if(unique)
        {
            for(int i=0;i<NOMBRE_SONS;i++)
                if(m_IDSons[i]==ID)
                {
                    m_sons[i].SetVolume(100);
                    float x,y,z;
                    x=m_sons[i].GetPosition().x;
                    y=m_sons[i].GetPosition().y;
                    z=m_sons[i].GetPosition().z;
                    // Je test voir si le nouveau son du même type est plus près du perso que l'ancien, si oui, je mets la position du nouveau à la place de l'ancien
                    if((double)(gpl::sqrt((positionHero.x+x)*(positionHero.x+x)+(positionHero.y-y)*(positionHero.y-y)))>(double)(gpl::sqrt((positionHero.x-position.x)*(positionHero.x-position.x)+(positionHero.y-position.y)*(positionHero.y-position.y))))
                        m_sons[i].SetPosition(position.x,0,position.y);

                    sf::Sound::Status Status = m_sons[i].GetStatus();
                    if(Status==0)
                        m_sons[i].Play();

                    creerNouveauSon=false;
                }
        }

        if(creerNouveauSon)
        {
            m_IDSons[sonEnCours]=ID;
            m_sons[sonEnCours].SetBuffer(m_buffers[ID]);

            sf::Sound::Status Status = m_sons[sonEnCours].GetStatus();
            if(Status==0)
                m_sons[sonEnCours].Play();

            m_sons[sonEnCours].SetPosition(position.x,0,position.y);

            sonEnCours++;
            if(sonEnCours>=NOMBRE_SONS)
                sonEnCours=0;
        }
    }
}




