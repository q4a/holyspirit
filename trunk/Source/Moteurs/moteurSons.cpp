

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
    change_volume = 100;
    sonEnCours=0;

    for (int i=0;i<NOMBRE_SONS;i++)
    {
        m_IDSons[i]=-1;
        m_sons_preserv[i]=false;
    }
}
MoteurSons::~MoteurSons()
{
    for (int i=0;i<(int)m_buffers.size();i++)
        delete m_buffers[i];
    m_buffers.clear();
    m_cheminsSons.clear();
}

void MoteurSons::Gerer(float temps)
{
    m_music.setPosition(sf::Listener::getPosition());
    m_music.setVolume(configuration->music_volume * change_volume * m_music_volume * 0.0001);

    if(!m_nextMusic.empty() && (m_nextMusic != m_curMusic || GetMusicStatus() == 0))
    {
        change_volume -= temps * 50;
        if(change_volume <= 0 || m_curMusic.empty() || GetMusicStatus() == 0)
        {
            m_music_volume = 100;

            if(change_volume <= 0)
                change_volume = 0;

            m_music.stop();

            m_music.setLoop(false);

            if (!m_music.openFromFile(m_nextMusic.c_str()))
                console->Ajouter("Unable to load : "+m_nextMusic,1);
            else
                console->Ajouter("Loading : "+m_nextMusic,0),m_music.play();

            std::string conf_path = m_nextMusic.substr(0,m_nextMusic.size()-3) + "conf";
            std::ifstream conf;
            conf.open(conf_path.c_str(), std::ios::in);

            if (conf)
            {
                char caractere;
                do
                {
                    conf.get(caractere);
                    if (caractere=='*')
                    {
                        do
                        {
                            conf.get(caractere);
                            switch (caractere)
                            {
                            case 'v' :
                                conf>>m_music_volume;
                                break;
                            }

                            if (conf.eof())
                                caractere='$';
                        }
                        while (caractere!='$');
                        conf.get(caractere);
                    }
                    if (conf.eof())
                        caractere='$';
                }
                while (caractere!='$');
                conf.close();
            }

            m_curMusic = m_nextMusic;
            m_nextMusic.clear();
        }
    }
    else if(change_volume < 100)
    {
        change_volume += temps * 50;
        if(change_volume >= 100)
            change_volume = 100;
    }
}

void MoteurSons::Vider()
{

}


int MoteurSons::AjouterBuffer(std::string chemin)
{
    for (int i=0;i<(int)m_cheminsSons.size();i++)
        if (m_cheminsSons[i]==chemin)
            return i;

    m_buffers.push_back(new Infos_buffer ());
    m_cheminsSons.push_back(chemin);

    if (!m_buffers.back()->buffer.loadFromFile(chemin.c_str()))
    {
        console->Ajouter("Unable to load : "+chemin,1);
        return -1;
    }
    else
        console->Ajouter("Loading : "+chemin,0);

    std::string conf_path = chemin.substr(0,chemin.size()-3) + "conf";
    std::ifstream conf;
    conf.open(conf_path.c_str(), std::ios::in);

    if (conf)
    {
        char caractere;
        do
        {
            conf.get(caractere);
            if (caractere=='*')
            {
                do
                {
                    conf.get(caractere);
                    switch (caractere)
                    {
                    case 'v' :
                        conf>>m_buffers.back()->volume;
                        break;
                    case 'p' :
                        conf>>m_buffers.back()->pitchable;
                        break;
                    }

                    if (conf.eof())
                        caractere='$';
                }
                while (caractere!='$');
                conf.get(caractere);
            }
            if (conf.eof())
                caractere='$';
        }
        while (caractere!='$');
        conf.close();
    }

    return m_buffers.size()-1;
}

void MoteurSons::StopAllSounds()
{
    for (int i=0;i<NOMBRE_SONS;i++)
        if(!m_sons_preserv[i])
            m_sons[i].stop();
}

bool MoteurSons::IsPlayingSound(int ID)
{
    if (ID>=0&&ID<(int)m_buffers.size())
        for (int i=0;i<NOMBRE_SONS;i++)
            if (m_IDSons[i]==ID && m_sons[i].getStatus() != 0)
                return true;
    return false;
}

bool MoteurSons::JouerSon(int ID,coordonnee position,bool unique,bool preserv,int volume)
{
    coordonnee positionHero;
    positionHero.x = (int)(sf::Listener::getPosition().x);
    positionHero.y = (int)(sf::Listener::getPosition().y);

    if (volume>100)
        volume=100;
    if (volume<0)
        volume=0;
    if (ID>=0&&ID<(int)m_buffers.size())
    {
        bool creerNouveauSon=true;
        if (unique)
        {
            for (int i=0;i<NOMBRE_SONS && creerNouveauSon;i++)
                if (m_IDSons[i]==ID && m_sons[i].getStatus() != 0)
                {

                    //m_sons[i].SetVolume(volume);
                    float x,y;//,z;
                    x=m_sons[i].getPosition().x;
                    y=m_sons[i].getPosition().y;
                    //z=m_sons[i].getPosition().z;

                    m_sons[i].setVolume(volume * m_buffers[ID]->volume / 100);

                    // Je test voir si le nouveau son du même type est plus près du perso que l'ancien, si oui, je mets la position du nouveau à la place de l'ancien
                    if ((double)(((positionHero.x-x)*(positionHero.x-x)+(positionHero.y-y)*(positionHero.y-y)))>
                        (double)(((positionHero.x-position.x)*(positionHero.x-position.x)+(positionHero.y-position.y)*(positionHero.y-position.y))))
                            m_sons[i].setPosition(position.x,0,position.y);



                   /* float attenuation = 100 - ((positionHero.x-position.x)*(positionHero.x-position.x)+(positionHero.y-position.y)*(positionHero.y-position.y))/50;
                    if(attenuation < 0)
                        attenuation = 0;

                    if(m_buffers[ID]->buffer.GetChannelsCount() == 2)
                        m_sons[i].SetVolume(volume * m_buffers[ID]->volume / 100 * attenuation / 100);*/
                        //m_sons[i].SetPosition(sf::Listener::GetPosition());

                   // if(volume > m_sons[i].GetVolume())


                    if((position.x==-1&&position.y==-1)
                    || (position.x==0&&position.y==0))
                        m_sons[i].setPosition(sf::Listener::getPosition());

                    creerNouveauSon=false;
                }
        }

        if (creerNouveauSon/* && volume > 0*/)
        {
            int temp = sonEnCours++;
            if (sonEnCours>=NOMBRE_SONS)
                sonEnCours=0;

            while(m_sons[sonEnCours].getStatus() != 0 && sonEnCours != temp)
            {
                sonEnCours ++;
                if (sonEnCours>=NOMBRE_SONS)
                    sonEnCours=0;
            }

            m_sons[sonEnCours].setVolume(volume * m_buffers[ID]->volume / 100);
            m_IDSons[sonEnCours]=ID;
            m_sons[sonEnCours].setBuffer(m_buffers[ID]->buffer);

            //sf::Sound::Status Status = m_sons[sonEnCours].GetStatus();
            //if (Status==0)
                m_sons[sonEnCours].play();

            m_sons[sonEnCours].setPosition(position.x,0,position.y);


            /*float attenuation = 100 - ((positionHero.x-position.x)*(positionHero.x-position.x)+(positionHero.y-position.y)*(positionHero.y-position.y));
            if(attenuation < 0)
                attenuation = 0;

            if(m_buffers[ID]->buffer.GetChannelsCount() == 2)
                m_sons[sonEnCours].SetVolume(volume * m_buffers[ID]->volume / 100 * attenuation / 100);*/

            if((position.x==-1&&position.y==-1)
            || (position.x==0&&position.y==0))
                m_sons[sonEnCours].setPosition(sf::Listener::getPosition());

            m_sons[sonEnCours].setAttenuation(0.5f);
            //m_sons[sonEnCours].SetMinDistance(16.f);

            m_sons[sonEnCours].setLoop(false);

            if(m_buffers[ID]->pitchable)
                m_sons[sonEnCours].setPitch(0.92 + (rand()/(float)RAND_MAX)*0.20);
            else
                m_sons[sonEnCours].setPitch(1);


            m_sons_preserv[sonEnCours] = preserv;
                      //  m_sons[sonEnCours].SetPosition(sf::Listener::GetPosition());
        }

        return creerNouveauSon;
    }

    return false;
}

void MoteurSons::PlayNewMusic(const std::string &chemin)
{
    m_nextMusic = chemin;
}

sf::Sound::Status MoteurSons::GetMusicStatus()
{
    return m_music.getStatus();
}

void MoteurSons::setVolumeMusique(int volume)
{
    m_music.setVolume(volume);
}


void MoteurSons::DebugRefreshSound()
{
    for(unsigned i = 0 ; i < m_cheminsSons.size() ; ++i)
    {
        std::string conf_path = m_cheminsSons[i].substr(0,m_cheminsSons[i].size()-3) + "conf";
        std::ifstream conf;
        conf.open(conf_path.c_str(), std::ios::in);

        if (conf)
        {
            char caractere;
            do
            {
                conf.get(caractere);
                if (caractere=='*')
                {
                    do
                    {
                        conf.get(caractere);
                        switch (caractere)
                        {
                        case 'v' :
                            conf>>m_buffers[i]->volume;
                            break;
                        }

                        if (conf.eof())
                            caractere='$';
                    }
                    while (caractere!='$');
                    conf.get(caractere);
                }
                if (conf.eof())
                    caractere='$';
            }
            while (caractere!='$');
            conf.close();
        }

    }
}
