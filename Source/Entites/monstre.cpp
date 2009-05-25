

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


#include <iostream>
#include <fstream>

#include "monstre.h"
#include "../constantes.h"
#include "../globale.h"

using namespace std;

Modele_Monstre::Modele_Monstre()
{
    m_porteeLumineuse.rouge=0;
    m_porteeLumineuse.vert=0;
    m_porteeLumineuse.bleu=0;
    m_porteeLumineuse.intensite=0;
}

Modele_Monstre::~Modele_Monstre()
{
    for (int i=0;i<(int)m_pose.size();i++)
    {
        for (int j=0;j<(int)m_pose[i].size();j++)
            m_pose[i][j].clear();
        m_pose[i].clear();
    }
    m_pose.clear();

    m_image.clear();
    m_sons.clear();

    m_miracles.clear();
    m_objets.clear();
}

Monstre::~Monstre()
{
    m_miracleEnCours.clear();
}

Monstre::Monstre()
{
    Personnage::Personnage();
    m_vu=0;
    m_modele=-1;
    m_monstre=true;

    m_attente=0;
    m_compteur=0;

    m_miracleALancer=-1;

    m_positionPixel.h=0;
    m_cheminFinal.h=0;
    m_positionCase.h=0;

    m_porteeLumineuse.rouge=0;
    m_porteeLumineuse.vert=0;
    m_porteeLumineuse.bleu=0;
    m_porteeLumineuse.intensite=0;
    m_caracteristique.rang=0;
}

void Monstre::Charger(int numero,Modele_Monstre *modele)
{
    // Personnage::Charger(modele);

    m_modele=numero;
    m_caracteristique=modele->getCaracteristique();
    m_porteeLumineuse=modele->getPorteeLumineuse();
    m_porteeLumineuseBasique=m_porteeLumineuse;

    m_friendly=modele->m_friendly;

    if (m_caracteristique.rang==0&&m_caracteristique.pointAme>0)
    {
        int temp=rand()%(1000);
        if (temp<50)
            m_caracteristique.rang=1;
        if (temp<15)
            m_caracteristique.rang=2;

        if (m_caracteristique.rang==1)
        {
            m_porteeLumineuse.intensite=128;
            m_caracteristique.maxVie*=3;
            m_caracteristique.vie*=3;
            m_caracteristique.degatsMin*=1;
            m_caracteristique.degatsMax*=1;
            m_caracteristique.pointAme*=2;

            m_caracteristique.modificateurTaille*=1.1;
        }
        if (m_caracteristique.rang==2)
        {
            m_porteeLumineuse.intensite=255;
            m_caracteristique.maxVie*=5;
            m_caracteristique.vie*=5;
            m_caracteristique.degatsMin*=2;
            m_caracteristique.degatsMax*=2;
            m_caracteristique.pointAme*=4;

            m_caracteristique.modificateurTaille*=1.2;
        }

        if (m_caracteristique.rang>0)
        {
            m_porteeLumineuse.intensite=255;
            m_porteeLumineuse.rouge=rand()%255;
            m_porteeLumineuse.vert=rand()%255;
            m_porteeLumineuse.bleu=rand()%255;

            m_porteeLumineuseBasique=m_porteeLumineuse;
        }
    }


    for (int i=0;i<(int)modele->getObjets().size();i++)
        if (rand()%100000/(m_caracteristique.rang*3+1)<=modele->getObjets()[i].getChanceTrouver())
        {
            m_objets.push_back(Objet ());
            m_objets.back()=modele->getObjets()[i];
            m_objets.back().Generer((m_caracteristique.rang*5+1));
        }
}


bool Modele_Monstre::Charger(string chemin)
{
    console->Ajouter("",0);
    console->Ajouter("Chargement du monstre : "+chemin,0);

    m_caracteristique.vitesse=0;
    m_caracteristique.vie=0;
    m_caracteristique.degatsMin=0;
    m_caracteristique.degatsMax=0;
    m_caracteristique.rang=0;

    m_chemin=chemin;
    m_particules=-1;

    cDAT reader;

    reader.Read(chemin);

    ifstream *fichier;
    fichier = reader.GetInfos("infos.txt");

    if (fichier)
    {
        char caractere;
        do
        {
            fichier->get(caractere);
            if (caractere=='*')
            {

                string cheminImage;
                *fichier>>cheminImage;

                m_image.push_back(moteurGraphique->AjouterImage(reader.GetFile(cheminImage), reader.GetFileSize(cheminImage), cheminImage));
            }
            if (fichier->eof())
            {
                char temp[1000];
                sprintf(temp,"Erreur : Monstre \" %s \" Invalide",chemin.c_str());
                console->Ajouter(temp,1);
                caractere='$';
                m_caracteristique.maxVie=0;
            }
        }
        while (caractere!='$');

        //m_buffer.clear();
        do
        {
            fichier->get(caractere);
            if (caractere=='*')
            {
                string cheminSon;
                *fichier>>cheminSon;
                m_sons.push_back(moteurSons->AjouterBuffer(cheminSon));
            }
            if (fichier->eof())
            {
                char temp[1000];
                sprintf(temp,"Erreur : Monstre \" %s \" Invalide",chemin.c_str());
                console->Ajouter(temp,1);
                caractere='$';
                m_caracteristique.maxVie=0;
            }
        }
        while (caractere!='$');

        do
        {
            //Chargement de la lumière ambiante
            fichier->get(caractere);
            if (caractere=='*')
            {
                do
                {
                    fichier->get(caractere);
                    switch (caractere)
                    {
                    case 'r':
                        *fichier>>m_porteeLumineuse.rouge;
                        break;
                    case 'v':
                        *fichier>>m_porteeLumineuse.vert;
                        break;
                    case 'b':
                        *fichier>>m_porteeLumineuse.bleu;
                        break;
                    case 'i':
                        *fichier>>m_porteeLumineuse.intensite;
                        break;
                    }
                    if (fichier->eof())
                    {
                        char temp[1000];
                        sprintf(temp,"Erreur : Monstre \" %s \" Invalide",chemin.c_str());
                        console->Ajouter(temp,1);
                        caractere='$';
                        m_caracteristique.maxVie=0;
                    }
                }
                while (caractere!='$');
                fichier->get(caractere);
                if (fichier->eof())
                {
                    char temp[1000];
                    sprintf(temp,"Erreur : Monstre \" %s \" Invalide",chemin.c_str());
                    console->Ajouter(temp,1);
                    caractere='$';
                    m_caracteristique.maxVie=0;
                }
            }
            if (fichier->eof())
            {
                char temp[1000];
                sprintf(temp,"Erreur : Monstre \" %s \" Invalide",chemin.c_str());
                console->Ajouter(temp,1);
                caractere='$';
            }

        }
        while (caractere!='$');

        do
        {
            fichier->get(caractere);
            if (caractere=='*')
            {
                getline(*fichier,m_caracteristique.nom);
                //*fichier>>m_caracteristique.nom;
            }
            if (fichier->eof())
            {
                char temp[1000];
                sprintf(temp,"Erreur : Monstre \" %s \" Invalide",chemin.c_str());
                console->Ajouter(temp,1);
                caractere='$';
            }

        }
        while (caractere!='$');

        do
        {
            fichier->get(caractere);
            if (caractere=='*')
            {
                *fichier>>m_caracteristique.rang;
            }
            if (fichier->eof())
            {
                char temp[1000];
                sprintf(temp,"Erreur : Monstre \" %s \" Invalide",chemin.c_str());
                console->Ajouter(temp,1);
                caractere='$';
            }

        }
        while (caractere!='$');

        do
        {
            if (caractere=='*')
            {
                m_caracteristique.modificateurTaille=1;
                m_caracteristique.sang=0;
                m_explosif=false;
                m_minimap=true;
                m_friendly=false;
                do
                {
                    fichier->get(caractere);
                    switch (caractere)
                    {
                    case 'v':
                        *fichier>>m_caracteristique.maxVie;
                        break;
                    case 'd':
                        fichier->get(caractere);
                        if (caractere=='i') *fichier>>m_caracteristique.degatsMin;
                        else *fichier>>m_caracteristique.degatsMax ;
                        break;
                    case 'm':
                        *fichier>>m_caracteristique.vitesse;
                        break;
                    case 'o':
                        *fichier>>m_ombre;
                        break;
                    case 'a':
                        *fichier>>m_caracteristique.pointAme;
                        break;
                    case 'n':
                        *fichier>>m_caracteristique.niveau;
                        break;

                    case 's':
                        *fichier>>m_caracteristique.sang;
                        break;

                    case 't':
                        *fichier>>m_caracteristique.modificateurTaille;
                        break;

                    case 'e':
                        *fichier>>m_explosif;
                        break;

                    case 'i':
                        *fichier>>m_minimap;
                        break;

                    case 'f':
                        *fichier>>m_friendly;
                        break;

                    }
                    if (fichier->eof())
                    {
                        char temp[1000];
                        sprintf(temp,"Erreur : Monstre \" %s \" Invalide",chemin.c_str());
                        console->Ajouter(temp,1);
                        caractere='$';
                        m_caracteristique.maxVie=0;
                    }
                }
                while (caractere!='$');


                m_caracteristique.vie=m_caracteristique.maxVie;

                fichier->get(caractere);
                if (fichier->eof())
                {
                    char temp[1000];
                    sprintf(temp,"Erreur : Monstre \" %s \" Invalide",chemin.c_str());
                    console->Ajouter(temp,1);
                    caractere='$';
                    m_caracteristique.maxVie=0;
                }
            }
            fichier->get(caractere);
            if (fichier->eof())
            {
                char temp[1000];
                sprintf(temp,"Erreur : Monstre \" %s \" Invalide",chemin.c_str());
                console->Ajouter(temp,1);
                caractere='$';
                m_caracteristique.maxVie=0;
            }
        }
        while (caractere!='$');

        do
        {
            fichier->get(caractere);
            if (caractere=='*')
            {
                string temp;
                *fichier>>temp;
                //getline(fichier, temp);

                m_scriptAI.Charger(temp);
            }

            if (fichier->eof())
            {
                char temp[1000];
                sprintf(temp,"Erreur : Monstre \" %s \" Invalide",chemin.c_str());
                console->Ajouter(temp,1);
                caractere='$';
                m_caracteristique.maxVie=0;
            }
        }
        while (caractere!='$');


        do
        {
            fichier->get(caractere);
            if (caractere=='*')
            {
                Objet tempModeleObjet;
                do
                {
                    fichier->get(caractere);
                    if (caractere=='r')
                    {
                        int temp2;
                        *fichier>>temp2;
                        tempModeleObjet.setChanceTrouver(temp2);
                    }

                    if (caractere=='*')
                    {
                        string temp2;
                        //getline(fichier, temp2);
                        *fichier>>temp2;
                        tempModeleObjet.Charger(temp2);
                    }

                    if (fichier->eof())
                    {
                        char temp[1000];
                        sprintf(temp,"Erreur : Monstre \" %s \" Invalide",chemin.c_str());
                        console->Ajouter(temp,1);
                        caractere='$';
                    }
                }
                while (caractere!='$');
                m_objets.push_back(tempModeleObjet);
                fichier->get(caractere);
            }
            if (fichier->eof())
            {
                char temp[1000];
                sprintf(temp,"Erreur : Monstre \" %s \" Invalide",chemin.c_str());
                console->Ajouter(temp,1);
                caractere='$';
            }

        }
        while (caractere!='$');

        do
        {
            fichier->get(caractere);
            if (caractere=='*')
            {
                string temp;
                //getline(fichier, temp);
                *fichier>>temp;
                m_particules=moteurGraphique->AjouterModeleSystemeParticules(temp);
            }

            if (fichier->eof())
            {
                char temp[1000];
                sprintf(temp,"Erreur : Monstre \" %s \" Invalide",chemin.c_str());
                console->Ajouter(temp,1);
                caractere='$';
                m_caracteristique.maxVie=0;
            }
        }
        while (caractere!='$');


        do
        {
            fichier->get(caractere);
            if (caractere=='*')
            {
                string temp;
                *fichier>>temp;
                m_miracles.push_back(Miracle (temp));
            }

            if (fichier->eof())
            {
                char temp[1000];
                sprintf(temp,"Erreur : Monstre \" %s \" Invalide",chemin.c_str());
                console->Ajouter(temp,1);
                caractere='$';
                m_caracteristique.maxVie=0;
            }
        }
        while (caractere!='$');

        ChargerPose(fichier);

        fichier->close();
    }
    else
        console->Ajouter("Impossible d'ouvrir : "+chemin,1);

    delete fichier;

    return 1;
}

std::vector<Objet> Modele_Monstre::getObjets()
{
    return m_objets;
}

void Monstre::TesterVision(coordonnee positionHero)
{
    if (m_caracteristique.vie>0)
    {
        if (fabs(positionHero.x-m_positionCase.x)<5&&fabs(positionHero.y-m_positionCase.y)<5)
        {
            m_vu=1;
            // if(m_etat==0) m_poseEnCours=0;
        }
        if (fabs(positionHero.x-m_positionCase.x)>10||fabs(positionHero.y-m_positionCase.y)>10)
        {
            if (m_vu)
                setDepart();
            m_vu=0,m_etat=0,m_poseEnCours=0;

        }
    }
}

coordonnee Monstre::getDepart()
{
    return m_depart;
}

std::vector<Objet> Monstre::getObjets()
{
    return m_objets;
}
void Monstre::setObjets(std::vector<Objet> objets)
{
    m_objets=objets;
}

int Monstre::getModele()
{
    return m_modele;
}

bool Monstre::getVu()
{
    return m_vu;
}
void Monstre::setVu(bool vu)
{
    m_vu=vu;
}

void Monstre::setDepart()
{
    m_depart=m_positionCase;
}

