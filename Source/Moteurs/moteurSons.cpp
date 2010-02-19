

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



#include "moteurSons.h"
#include "../globale.h"

#include <iostream>


MoteurSons::MoteurSons()
{
    sonEnCours=0;

    for (int i=0;i<NOMBRE_SONS;i++)
        m_IDSons[i]=-1;
}
MoteurSons::~MoteurSons()
{
    m_buffers.clear();
    m_cheminsSons.clear();
}

void MoteurSons::Gerer()
{
}

void MoteurSons::Vider()
{

}


int MoteurSons::AjouterBuffer(std::string chemin)
{
    for (int i=0;i<(int)m_cheminsSons.size();i++)
        if (m_cheminsSons[i]==chemin)
            return i;

    m_buffers.push_back(sf::SoundBuffer ());
    m_cheminsSons.push_back(chemin);

    if (!m_buffers.back().LoadFromFile(chemin.c_str()))
    {
        console->Ajouter("Impossible de charger : "+chemin,1);
        return -1;
    }
    else
        console->Ajouter("Chargement de : "+chemin,0);

    return m_buffers.size()-1;
}

void MoteurSons::StopAllSounds()
{
    for (int i=0;i<NOMBRE_SONS;i++)
        m_sons[i].Stop();
}

void MoteurSons::JouerSon(int ID,coordonnee position,bool unique,int volume)
{
    m_music.SetPosition(sf::Listener::GetPosition());

    if (volume>100)
        volume=100;
    if (volume<0)
        volume=0;
    if (ID>=0&&ID<(int)m_buffers.size())
    {
        bool creerNouveauSon=true;
        if (unique)
        {
            for (int i=0;i<NOMBRE_SONS;i++)
                if (m_IDSons[i]==ID)
                {
                    coordonnee positionHero;
                    positionHero.x = (int)(sf::Listener::GetPosition().x);
                    positionHero.y = (int)(sf::Listener::GetPosition().y);

                    //m_sons[i].SetVolume(volume);
                    float x,y,z;
                    x=m_sons[i].GetPosition().x;
                    y=m_sons[i].GetPosition().y;
                    z=m_sons[i].GetPosition().z;
                    // Je test voir si le nouveau son du même type est plus près du perso que l'ancien, si oui, je mets la position du nouveau à la place de l'ancien
                    if ((double)(((positionHero.x+x)*(positionHero.x+x)+(positionHero.y-y)*(positionHero.y-y)))>(double)(((positionHero.x-position.x)*(positionHero.x-position.x)+(positionHero.y-position.y)*(positionHero.y-position.y))))
                        m_sons[i].SetPosition(position.x,0,position.y);

                    if(volume > m_sons[i].GetVolume())
                        m_sons[i].SetVolume(volume);

                    if(position.x==-1&&position.y==-1)
                        m_sons[i].SetPosition(sf::Listener::GetPosition());

                    sf::Sound::Status Status = m_sons[i].GetStatus();
                    if (Status==0)
                        m_sons[i].Play();

                    creerNouveauSon=false;
                }
        }

        if (creerNouveauSon)
        {
            m_sons[sonEnCours].SetVolume(volume);
            m_IDSons[sonEnCours]=ID;
            m_sons[sonEnCours].SetBuffer(m_buffers[ID]);

            sf::Sound::Status Status = m_sons[sonEnCours].GetStatus();
            if (Status==0)
                m_sons[sonEnCours].Play();

            m_sons[sonEnCours].SetPosition(position.x,0,position.y);

            sonEnCours++;
            if (sonEnCours>=NOMBRE_SONS)
                sonEnCours=0;
        }
    }
}

void MoteurSons::PlayNewMusic(std::string chemin)
{
    m_music.Stop();

    m_music.SetLoop(false);
    m_music.SetVolume(100);

    if (!m_music.OpenFromFile(chemin.c_str()))
        console->Ajouter("Impossible de charger : "+chemin,1);
    else
        console->Ajouter("Chargement de : "+chemin,0);

    m_music.Play();
}

sf::Sound::Status MoteurSons::GetMusicStatus()
{
    return m_music.GetStatus();
}

void MoteurSons::setVolumeMusique(int volume)
{
    m_music.SetVolume(volume);
}

