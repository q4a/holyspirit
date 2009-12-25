

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

    Modele_Personnage();
}

Modele_Monstre::~Modele_Monstre()
{
    m_tileset.clear();

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

    m_monstre=true;

    m_attente=0;
    m_compteur=0;
}

Monstre::Monstre(Personnage &personnage)
{
    Personnage::Personnage();

    m_attente   = 0;
    m_compteur  = 0;
    m_vu        = 0;

    m_cible = NULL;

    m_lumiere                   = personnage.m_lumiere;
    frappeEnCours               = personnage.frappeEnCours;
    m_lancementMiracleEnCours   = personnage.m_lancementMiracleEnCours;
    m_shooter                   = personnage.m_shooter;
    m_monstre                   = personnage.m_monstre;
    m_touche                    = personnage.m_touche;
    m_nombreInvocation          = personnage.m_nombreInvocation;
    m_miracleALancer            = personnage.m_miracleALancer;
    m_effets                    = personnage.m_effets;
    m_scriptAI                  = personnage.m_scriptAI;
    m_friendly                  = personnage.m_friendly;
    m_caracteristique           = personnage.getCaracteristique();
    m_positionCase              = personnage.getCoordonnee();
    m_positionPixel             = personnage.getCoordonneePixel();
    m_modele                    = personnage.getModele();
    m_objets                    = personnage.getObjets();
    m_porteeLumineuse           = personnage.getPorteeLumineuse();
    m_porteeLumineuseBasique    = personnage.getPorteeLumineuse();
    m_depart                    = personnage.getDepart();

    m_angle                     = 0;
    m_etat                      = 0;
}

void Monstre::Charger(int numero,Modele_Monstre *modele)
{
    // Personnage::Charger(modele);

    m_modele=numero;
    m_caracteristique=modele->getCaracteristique();
    m_porteeLumineuse=modele->getPorteeLumineuse();
    m_porteeLumineuseBasique=m_porteeLumineuse;

    m_impenetrable = modele->m_impenetrable;
    m_impoussable = modele->m_impoussable;
    m_collision = modele->m_collision;

    m_scriptAI = modele->m_scriptAI;

    m_friendly=modele->m_friendly;

    for (unsigned i=0; i<modele->getObjets().size(); i++)
        if ((float)(rand()%1000000000)<=(float)(modele->getObjets()[i].getChanceTrouver()*0.5*(m_caracteristique.rang*3+1)))
        {
            m_objets.push_back(modele->getObjets()[i]);
            m_objets.back().Generer((m_caracteristique.rang*5+1));
        }

    m_caracteristique.maxVie = m_caracteristique.vie;
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

    m_caracteristique.dexterite=0;

    m_chemin=chemin;
    m_particules=-1;

    //cDAT reader;

    // reader.Read(chemin);

    ifstream fichier;
    fichier.open(chemin.c_str(), ios::in);

    string cheminRessources;


    if (fichier)
    {
        char caractere;
        do
        {
            fichier.get(caractere);
            if (caractere=='*')
                fichier>>cheminRessources;

            if (fichier.eof())
            {
                console->Ajouter("Erreur : Monstre \" "+chemin+" \" Invalide",1);
                caractere='$';
                m_caracteristique.maxVie=0;
            }
        }
        while (caractere!='$');

        do
        {
            //Chargement de la lumière ambiante
            fichier.get(caractere);
            if (caractere=='*')
            {
                do
                {
                    fichier.get(caractere);
                    switch (caractere)
                    {
                    case 'r':
                        fichier>>m_porteeLumineuse.rouge;
                        break;
                    case 'v':
                        fichier>>m_porteeLumineuse.vert;
                        break;
                    case 'b':
                        fichier>>m_porteeLumineuse.bleu;
                        break;
                    case 'i':
                        fichier>>m_porteeLumineuse.intensite;
                        break;
                    }
                    if (fichier.eof())
                    {
                        console->Ajouter("Erreur : Monstre \" "+chemin+" \" Invalide",1);
                        caractere='$';
                        m_caracteristique.maxVie=0;
                    }
                }
                while (caractere!='$');
                fichier.get(caractere);
                if (fichier.eof())
                {
                    console->Ajouter("Erreur : Monstre \" "+chemin+" \" Invalide",1);
                    caractere='$';
                    m_caracteristique.maxVie=0;
                }
            }
            if (fichier.eof())
            {
                console->Ajouter("Erreur : Monstre \" "+chemin+" \" Invalide",1);
                caractere='$';
            }

        }
        while (caractere!='$');


        do
        {
            //Chargement du nom
            fichier.get(caractere);
            if (caractere=='*')
            {
                do
                {
                    //Chargement du nom
                    fichier.get(caractere);
                    if (caractere=='n')
                    {
                        int no;
                        fichier>>no;
                        m_caracteristique.nom = configuration->getText(3, no);
                    }
                }
                while (caractere!='$');
                fichier.get(caractere);
            }
        }
        while (caractere!='$');

        do
        {
            fichier.get(caractere);
            if (caractere=='*')
            {
                fichier>>m_caracteristique.rang;
            }
            if (fichier.eof())
            {
                console->Ajouter("Erreur : Monstre \" "+chemin+" \" Invalide",1);
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
                m_minimap=true;
                m_friendly=false;
                m_collision = true;
                do
                {
                    fichier.get(caractere);
                    switch (caractere)
                    {
                    case 'v':
                        fichier>>m_caracteristique.maxVie;
                        break;
                    case 'd':
                        fichier.get(caractere);
                        if (caractere=='i') fichier>>m_caracteristique.degatsMin;
                        else fichier>>m_caracteristique.degatsMax ;
                        break;
                    case 'm':
                        fichier>>m_caracteristique.vitesse;
                        break;
                    case 'o':
                        fichier>>m_ombre;
                        break;
                    case 'a':
                        fichier>>m_caracteristique.pointAme;
                        break;
                    case 'n':
                        fichier>>m_caracteristique.niveau;
                        break;

                    case 's':
                        fichier>>m_caracteristique.sang;
                        break;

                    case 't':
                        fichier>>m_caracteristique.modificateurTaille;
                        break;

                    case 'i':
                        fichier>>m_minimap;
                        break;

                    case 'f':
                        fichier>>m_friendly;
                        break;

                    case 'x':
                        fichier>>m_caracteristique.dexterite;
                        break;

                    case 'p':
                        fichier>>m_impenetrable;
                        break;

                    case 'u':
                        fichier>>m_impoussable;
                        break;

                    case 'c':
                        fichier>>m_collision;
                        break;

                    }
                    if (fichier.eof())
                    {
                        console->Ajouter("Erreur : Monstre \" "+chemin+" \" Invalide",1);
                        caractere='$';
                        m_caracteristique.maxVie=0;
                    }
                }
                while (caractere!='$');


                m_caracteristique.vie=m_caracteristique.maxVie;

                fichier.get(caractere);
                if (fichier.eof())
                {
                    console->Ajouter("Erreur : Monstre \" "+chemin+" \" Invalide",1);
                    caractere='$';
                    m_caracteristique.maxVie=0;
                }
            }
            fichier.get(caractere);
            if (fichier.eof())
            {
                console->Ajouter("Erreur : Monstre \" "+chemin+" \" Invalide",1);
                caractere='$';
                m_caracteristique.maxVie=0;
            }
        }
        while (caractere!='$');

        do
        {
            fichier.get(caractere);
            if (caractere=='*')
            {
                string temp;
                fichier>>temp;
                //getline(fichier, temp);

                m_scriptAI.Charger(temp);
            }

            if (fichier.eof())
            {
                console->Ajouter("Erreur : Monstre \" "+chemin+" \" Invalide",1);
                caractere='$';
                m_caracteristique.maxVie=0;
            }
        }
        while (caractere!='$');


        do
        {
            fichier.get(caractere);
            if (caractere=='*')
            {
                Objet tempModeleObjet;
                do
                {
                    fichier.get(caractere);
                    if (caractere=='r')
                    {
                        float temp2;
                        fichier>>temp2;
                        tempModeleObjet.setChanceTrouver(temp2);
                    }

                    if (caractere=='*')
                    {
                        string temp2;
                        //getline(fichier, temp2);
                        fichier>>temp2;
                        tempModeleObjet.Charger(temp2,m_caracteristique);
                    }

                    if (fichier.eof())
                    {
                        console->Ajouter("Erreur : Monstre \" "+chemin+" \" Invalide",1);
                        caractere='$';
                    }
                }
                while (caractere!='$');
                m_objets.push_back(tempModeleObjet);
                fichier.get(caractere);
            }
            if (fichier.eof())
            {
                console->Ajouter("Erreur : Monstre \" "+chemin+" \" Invalide",1);
                caractere='$';
            }

        }
        while (caractere!='$');

        do
        {
            fichier.get(caractere);
            if (caractere=='*')
            {
                string temp;
                //getline(fichier, temp);
                fichier>>temp;
                m_particules=moteurGraphique->AjouterModeleSystemeParticules(temp);
            }

            if (fichier.eof())
            {
                console->Ajouter("Erreur : Monstre \" "+chemin+" \" Invalide",1);
                caractere='$';
                m_caracteristique.maxVie=0;
            }
        }
        while (caractere!='$');


        do
        {
            fichier.get(caractere);
            if (caractere=='*')
            {
                string temp;
                fichier>>temp;
                m_miracles.push_back(Miracle (temp, m_caracteristique, 1));
            }

            if (fichier.eof())
            {
                console->Ajouter("Erreur : Monstre \" "+chemin+" \" Invalide",1);
                caractere='$';
                m_caracteristique.maxVie=0;
            }
        }
        while (caractere!='$');

        if(!cheminRessources.empty())
            Modele_Personnage::Charger(cheminRessources);

        fichier.close();
    }
    else
        console->Ajouter("Impossible d'ouvrir : "+chemin,1);


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
        if (fabs(positionHero.x-m_positionCase.x)>=12||fabs(positionHero.y-m_positionCase.y)>=12)
        {
            if (m_vu)
                setDepart();
            m_vu=0,m_etat=0,m_entite_graphique.m_noAnimation=0;

            m_cible = NULL;
        }
    }
}

bool Monstre::getVu()
{
    return m_vu;
}
void Monstre::setVu(bool vu)
{
    m_vu=vu;
    if(m_vu == 0)
        m_cible = NULL;
}



