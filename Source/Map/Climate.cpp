
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
                console->Ajouter("Erreur : Climat \" "+chemin+" \" Invalide",1);
                caractere = '$';
            }
        }
        while (caractere!='$');

        do
        {
            fichier.get(caractere);
            if (caractere=='*')
            {
                m_entities.push_back(Entite_graphique ());
                m_entities.back().m_couche = 11;
                m_entities.back().m_fixed = 1;

                coordonneeDecimal pos;

                do
                {
                    fichier.get(caractere);

                    if (caractere=='t')
                        fichier>>m_entities.back().m_noAnimation;
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


                    if (fichier.eof())
                    {
                        console->Ajouter("Erreur : Climat \" "+m_chemin+" \" Invalide",1);
                        caractere='$';
                    }
                }
                while (caractere!='$');

                pos.x = pos.x * configuration->Resolution.x/800;
                pos.y = pos.y * configuration->Resolution.y/600;
                m_entities.back().m_sprite.SetPosition(pos.x, pos.y);
                m_entities.back().Initialiser(pos);

                fichier.get(caractere);
            }
            if (fichier.eof())
            {
                console->Ajouter("Erreur : Climat \" "+m_chemin+" \" Invalide",1);
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
                console->Ajouter("Erreur : Climat \" "+chemin+" \" Invalide",1);
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
                console->Ajouter("Erreur : Climat \" "+chemin+" \" Invalide",1);
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
                console->Ajouter("Erreur : Climat \" "+chemin+" \" Invalide",1);
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
        m_entities[i].Animer(time);

        m_entities[i].m_sprite.SetScale((float)configuration->Resolution.x/800,
                                        (float)configuration->Resolution.y/600);

        sf::Color color = m_entities[i].m_sprite.GetColor();
        float temp = (float)color.a * GetState();
        color.a = (unsigned char)temp;
        m_entities[i].m_sprite.SetColor(color);
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


