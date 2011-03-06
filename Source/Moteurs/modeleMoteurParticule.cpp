

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



#include "modeleMoteurParticule.h"
#include "../globale.h"
#include <fstream>



ModeleParticuleSysteme::ModeleParticuleSysteme()
{

}
ModeleParticuleSysteme::ModeleParticuleSysteme(std::string chemin)
{
    Charger(chemin);
}
ModeleParticuleSysteme::~ModeleParticuleSysteme()
{
    m_particules.clear();
}
void ModeleParticuleSysteme::Charger(std::string chemin)
{
    console->Ajouter("",0);
    console->Ajouter("Chargement du systeme de particules : "+chemin,0);

    std::ifstream fichier;
    fichier.open(chemin.c_str(), std::ios::in);
    if (fichier)
    {
        char caractere;
        do
        {
            fichier.get(caractere);
            if (caractere=='*')
            {
                std::string m_cheminImage;
                fichier>>m_cheminImage;
                m_image=moteurGraphique->AjouterImage(m_cheminImage,-1);
            }
            if (fichier.eof())
            {
                console->Ajouter("Erreur : Système de particules \" "+chemin+" \" Invalide",1);
                caractere='$';
            }

        }
        while (caractere!='$');


        do
        {
            fichier.get(caractere);
            if (caractere=='*')
            {
                do
                {
                    fichier.get(caractere);
                    if (caractere=='m')
                    {
                        std::string cheminSon;
                        fichier>>cheminSon;
                        m_son.push_back(moteurSons->AjouterBuffer(cheminSon));
                    }
                    else if (caractere=='u')
                    {
                        bool temp;
                        fichier>>temp;
                        if(!m_son.empty())
                            m_son.back().unique = temp;
                    }
                    if (fichier.eof())
                    {
                        console->Ajouter("Erreur : Monstre \" "+m_chemin+" \" Invalide",1);
                        caractere='$';
                    }
                }
                while (caractere!='$');

                fichier.get(caractere);
            }
            if (fichier.eof())
            {
                console->Ajouter("Erreur : Tileset \" "+m_chemin+" \" Invalide",1);
                caractere='$';
            }
        }
        while (caractere!='$');


        m_chemin=chemin;

        do
        {
            fichier.get(caractere);
            if (caractere=='*')
            {
                m_particules.push_back(ModeleParticule ());

                m_particules.back().min         = 0;
                m_particules.back().max         = 0;

                m_particules.back().rebond      = 0;
                m_particules.back().frottement  = 0;
                m_particules.back().poids       = 1;
                m_particules.back().sang        = 0;
                m_particules.back().rotation    = 1;

                do
                {
                    fichier.get(caractere);
                    switch (caractere)
                    {
                    case 'x' :
                        fichier>>m_particules.back().positionImage.x;
                        break;
                    case 'y' :
                        fichier>>m_particules.back().positionImage.y;
                        break;
                    case 'w' :
                        fichier>>m_particules.back().positionImage.w;
                        break;
                    case 'h' :
                        fichier>>m_particules.back().positionImage.h;
                        break;

                    case 'i' :
                        fichier>>m_particules.back().min;
                        break;
                    case 'a' :
                        fichier>>m_particules.back().max;
                        break;

                    case 'r' :
                        fichier>>m_particules.back().rebond;
                        break;
                    case 'f' :
                        fichier>>m_particules.back().frottement;
                        break;
                    case 'p' :
                        fichier>>m_particules.back().poids;
                        break;
                    case 'b' :
                        fichier>>m_particules.back().sang;
                        break;


                    }

                    if (fichier.eof())
                    {
                        console->Ajouter("Erreur : Système de particules \" "+chemin+" \" Invalide",1);
                        caractere='$';
                    }

                }
                while (caractere!='$');

                if(m_particules.back().poids == 0)
                    m_particules.back().poids = 1;
                fichier.get(caractere);
            }
            if (fichier.eof())
            {
                console->Ajouter("Erreur : Système de particules \" "+chemin+" \" Invalide",1);
                caractere='$';
            }

        }
        while (caractere!='$');

        fichier.close();
    }
    else
        console->Ajouter("Impossible d'ouvrir : "+chemin,1);
}
