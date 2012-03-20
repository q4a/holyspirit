
#include "Climate.h"

#include <iostream>
#include <fstream>

Climate::Climate(std::string chemin)
{
    m_actif = false;
    m_cur_time = 0;
    m_max_time = 0;

    m_lumiereModificater.rouge      = 0;
    m_lumiereModificater.vert       = 0;
    m_lumiereModificater.bleu       = 0;
    m_lumiereModificater.intensite  = 0;
    m_lumiereModificater.hauteur    = 0;

    if(!chemin.empty())
        Charger(chemin);
}

Climate::~Climate()
{
}

void Climate::Charger(std::string chemin)
{
    m_chemin = chemin;

    std::ifstream fichier;
    fichier.open(chemin.c_str(), std::ios::in);
    if(fichier)
    {
        char caractere;


        do
        {
            //Chargement de la lumière ambiante
            fichier.get(caractere);
            if (caractere=='*')
            {
                std::string temp;
                fichier>>temp;
                m_tileset.push_back(moteurGraphique->AjouterTileset(temp,3));
            }
            if (fichier.eof())
            {
                console->Ajouter("Error : Climate \" "+chemin+" \" is invalid",1);
                caractere = '$';
            }
        }
        while (caractere!='$');

        do
        {
            fichier.get(caractere);
            if (caractere=='*')
            {
                m_noAnimationStart.push_back(0);
                m_entities.push_back(Entite_graphique ());
                m_entities.back().m_couche = 11;
                m_entities.back().m_fixed = 1;

                coordonneeDecimal pos;

                do
                {
                    fichier.get(caractere);

                    if (caractere=='t')
                    {
                        fichier>>m_entities.back().m_noAnimation;
                        m_noAnimationStart.back() = m_entities.back().m_noAnimation;
                    }
                    else if (caractere=='s')
                    {
                        int temp;
                        fichier>>temp;

                        if(temp >= 0 && temp < (int)m_tileset.size())
                            m_entities.back().m_tileset = moteurGraphique->getTileset(m_tileset[temp]);
                    }
                    else if (caractere=='x')
                        fichier>>pos.x;
                    else if (caractere=='y')
                        fichier>>pos.y;

                    else if (caractere=='f')
                        fichier>>m_entities.back().m_fixed;


                    if (fichier.eof())
                    {
                        console->Ajouter("Error : Climate \" "+m_chemin+" \" is invalid",1);
                        caractere='$';
                    }
                }
                while (caractere!='$');

                pos.x = pos.x * configuration->Resolution.x/800;
                pos.y = pos.y * configuration->Resolution.y/600;
                m_entities.back().setPosition(pos.x, pos.y);
                m_entities.back().Initialiser(pos);

                fichier.get(caractere);
            }
            if (fichier.eof())
            {
                console->Ajouter("Error : Climate \" "+m_chemin+" \" is invalid",1);
                caractere='$';
            }
        }
        while (caractere!='$');

        do
        {

            //Chargement de la lumière ambiante
            fichier.get(caractere);
            if (caractere=='*')
            {
                fichier>>m_lumiereModificater.rouge;
                fichier>>m_lumiereModificater.vert;
                fichier>>m_lumiereModificater.bleu;
                fichier>>m_lumiereModificater.intensite;
                fichier>>m_lumiereModificater.hauteur;
            }
            if (fichier.eof())
            {
                console->Ajouter("Error : Climate \" "+chemin+" \" is invalid",1);
                caractere = '$';
            }
        }
        while (caractere!='$');

        do
        {

            //Chargement de la lumière ambiante
            fichier.get(caractere);
            if (caractere=='*')
            {
                fichier>>m_max_time;
            }
            if (fichier.eof())
            {
                console->Ajouter("Error : Climate \" "+chemin+" \" is invalid",1);
                caractere = '$';
            }
        }
        while (caractere!='$');
        do
        {

            //Chargement de la lumière ambiante
            fichier.get(caractere);
            if (caractere=='*')
            {
                fichier>>m_transition_time;
            }
            if (fichier.eof())
            {
                console->Ajouter("Error : Climate \" "+chemin+" \" is invalid",1);
                caractere = '$';
            }
        }
        while (caractere!='$');

        fichier.close();
    }
}


void Climate::Update(float time)
{
    for(unsigned i = 0 ; i < m_entities.size() ; ++i)
    {
        if(GetState() == 0)
        {
            m_entities[i].m_noAnimation = m_noAnimationStart[i];
        }
        else
        {
            m_entities[i].Animer(time);
            m_entities[i].Generer();

            m_entities[i].setScale((float)configuration->Resolution.x/800,
                                    (float)configuration->Resolution.y/600);

            if(!m_entities[i].m_fixed)
            {
                sf::Vector2f pos = m_entities[i].getPosition();

                if(pos.x < GetViewRect(moteurGraphique->m_camera).left)
                    pos.x += (int)((GetViewRect(moteurGraphique->m_camera).left - pos.x)/GetViewRect(moteurGraphique->m_camera).width + 1)*GetViewRect(moteurGraphique->m_camera).width;
                if(pos.y < GetViewRect(moteurGraphique->m_camera).top)
                    pos.y += (int)((GetViewRect(moteurGraphique->m_camera).top - pos.y)/GetViewRect(moteurGraphique->m_camera).height + 1)*GetViewRect(moteurGraphique->m_camera).height;

                if(pos.x > GetViewRect(moteurGraphique->m_camera).left + GetViewRect(moteurGraphique->m_camera).width)
                    pos.x -= (int)((pos.x - GetViewRect(moteurGraphique->m_camera).left)/GetViewRect(moteurGraphique->m_camera).width)*GetViewRect(moteurGraphique->m_camera).width;
                if(pos.y > GetViewRect(moteurGraphique->m_camera).top + GetViewRect(moteurGraphique->m_camera).height)
                    pos.y -= (int)((pos.y - GetViewRect(moteurGraphique->m_camera).top)/GetViewRect(moteurGraphique->m_camera).height)*GetViewRect(moteurGraphique->m_camera).height;

                m_entities[i].setPosition(pos.x, pos.y);
            }
        }

        sf::Color color = m_entities[i].getColor();
        float temp = (float)color.a * GetState();
        color.a = (unsigned char)temp;
        m_entities[i].setColor(color);
        m_entities[i].m_sound_volume = GetState() * 100;
    }

    if(m_actif)
    {
        m_cur_time += time;
        if(m_cur_time > m_max_time)
            m_actif = 0;
    }
    else
        m_cur_time = 0;
}

void Climate::Draw()
{
    if(m_actif)
        for(unsigned i = 0 ; i < m_entities.size() ; ++i)
            moteurGraphique->AjouterEntiteGraphique(&m_entities[i]);
}

float Climate::GetState()
{
    if(m_cur_time < m_transition_time)
        return m_cur_time / m_transition_time;
    else if(m_cur_time > m_max_time - m_transition_time)
        return (m_max_time - m_cur_time) / m_transition_time;
    else
        return 1.0;
}

void Climate::Stop()
{
    if(m_cur_time < m_max_time - m_transition_time && m_actif)
        m_cur_time = m_max_time - m_transition_time;
}
void Climate::Continue()
{
    m_cur_time = m_transition_time;
}

