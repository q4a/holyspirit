

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

Modele_Monstre::Modele_Monstre() : Modele_Personnage()
{

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

Monstre::Monstre():
    Personnage()
{
    m_vu=0;

    m_monstre=true;

    m_attente=0;
    m_compteur=0;

    m_ID = -1;
}

Monstre::Monstre(Personnage &personnage):
    Personnage()
{
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
    m_depart                    = personnage.getDepart();
    m_miracleEnCours            = personnage.m_miracleEnCours;

    m_heroic                    = personnage.m_heroic;

    m_angle                     = 0;
    m_etat                      = 0;
}


void Monstre::Charger(int numero,Modele_Monstre *modele)
{
    // Personnage::Charger(modele);

    m_modele=numero;
    m_caracteristique=modele->getCaracteristique();
    m_impenetrable = modele->m_impenetrable;
    m_impoussable = modele->m_impoussable;
    m_noDistanceRestriction = modele->m_noDistanceRestriction;
    m_selectable = modele->m_selectable;
    m_collision = modele->m_collision;

    m_scriptAI = modele->m_scriptAI;

    m_friendly=modele->m_friendly;

    m_materiau=modele->m_materiau;

    m_entite_graphique.option_sonUnique = false;

    /*if (m_caracteristique.rang==0&&m_caracteristique.pointAme>0)
    {
        int temp=rand()%(1000);
        if (temp<20)
            m_caracteristique.rang=1;
        if (temp<5)
            m_caracteristique.rang=2;

        if (m_caracteristique.rang==1)
        {
            m_caracteristique.maxVie*=5;
            m_caracteristique.vie*=5;
            m_caracteristique.degatsMin[PHYSIQUE]=(int)((float)m_caracteristique.degatsMin[PHYSIQUE]*1.25);
            m_caracteristique.degatsMax[PHYSIQUE]=(int)((float)m_caracteristique.degatsMax[PHYSIQUE]*1.25);
            m_caracteristique.pointAme*=2;

            m_caracteristique.modificateurTaille*=1.1;
        }
        if (m_caracteristique.rang==2)
        {
            m_caracteristique.maxVie*=10;
            m_caracteristique.vie*=10;
            m_caracteristique.degatsMin[PHYSIQUE]=(int)((float)m_caracteristique.degatsMin[PHYSIQUE]*1.5);
            m_caracteristique.degatsMax[PHYSIQUE]=(int)((float)m_caracteristique.degatsMax[PHYSIQUE]*1.5);
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
    }*/

    GenererInventaire(modele);

    m_caracteristique.maxVie = m_caracteristique.vie;
}

void Monstre::GenererInventaire(Modele_Monstre *modele)
{
    //srand(time(NULL));
    m_objets.clear();

    for (unsigned k=0; k<modele->getObjets().size(); k++)
        for (unsigned i=0; i<modele->getObjets()[k].size(); i++)
            if ( ( rand()/(float)RAND_MAX) * 40000<=(float)(modele->getObjets()[k][i].getChanceTrouver()*(m_caracteristique.rang*3+1))


               // (float)(rand()%100000)<=(float)(modele->getObjets()[k][i].getChanceTrouver()*0.5*(m_caracteristique.rang*3+1))
                || modele->getObjets()[k][i].getChanceTrouver() == -1)
            {
                m_objets.push_back(modele->getObjets()[k][i]);
                m_objets.back().Generer((m_caracteristique.rang*10+1));
                i = modele->getObjets()[k].size();
            }
}

bool Modele_Monstre::Charger(const std::string &chemin)
{
    m_chemin = chemin;

    configuration->SetLanguage();

    console->Ajouter("",0);
    console->Ajouter("Loading the monster : "+chemin,0);

    m_caracteristique.vitesse=0;
    m_caracteristique.vie=0;

    for(int i = 0 ; i < 4 ; ++i)
    {
        m_caracteristique.degatsMin[i]=0;
        m_caracteristique.degatsMax[i]=0;
        m_caracteristique.armure[i]=0;
    }

    m_caracteristique.rang=0;

    m_caracteristique.dexterite=0;

    m_chemin=chemin;
    m_particules=-1;

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
                console->Ajouter("Error : Monster \" "+chemin+" \" is invalid",1);
                caractere='$';
                m_caracteristique.maxVie=0;
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
						textdomain ("data");
                        m_caracteristique.nom = gettext(configuration->getText(3, no).c_str());
						textdomain ("menus");
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
                console->Ajouter("Error : Monster \" "+chemin+" \" is invalid",1);
                caractere='$';
            }

        }
        while (caractere!='$');

        do
        {
            if (caractere=='*')
            {
                m_caracteristique.modificateurTaille=1;
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
                        if (caractere=='i') fichier>>m_caracteristique.degatsMin[PHYSIQUE];
                        else fichier>>m_caracteristique.degatsMax[PHYSIQUE] ;
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

                    case 'e':
                        fichier>>m_selectable;
                        break;

                    case 'c':
                        fichier>>m_collision;
                        break;

                    case 'g':
                        fichier>>m_noDistanceRestriction;
                        break;

                    case 's':
                        fichier>>m_materiau;
                        break;

                    case 'r':
                        fichier.get(caractere);
                        float temp;
                        fichier>>temp;

                        if(caractere == '0')
                            m_caracteristique.armure[0] = temp;
                        if(caractere == '1')
                            m_caracteristique.armure[1] = temp;
                        if(caractere == '2')
                            m_caracteristique.armure[2] = temp;
                        if(caractere == '3')
                            m_caracteristique.armure[3] = temp;

                        break;

                    }
                    if (fichier.eof())
                    {
                        console->Ajouter("Error : Monster \" "+chemin+" \" is invalid",1);
                        caractere='$';
                        m_caracteristique.maxVie=0;
                    }
                }
                while (caractere!='$');


                m_caracteristique.vie=m_caracteristique.maxVie;

                fichier.get(caractere);
                if (fichier.eof())
                {
                    console->Ajouter("Error : Monster \" "+chemin+" \" is invalid",1);
                    caractere='$';
                    m_caracteristique.maxVie=0;
                }
            }
            fichier.get(caractere);
            if (fichier.eof())
            {
                console->Ajouter("Error : Monster \" "+chemin+" \" is invalid",1);
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
                console->Ajouter("Error : Monster \" "+chemin+" \" is invalid",1);
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
                m_objets.push_back(std::vector<Objet> ());

                do
                {
                    fichier.get(caractere);
                    if (caractere=='r')
                    {
                        int temp2;
                        fichier>>temp2;
                        m_objets.back().back().setChanceTrouver(temp2);
                    }
                    if (caractere=='i')
                    {
                        int temp2;
                        fichier>>temp2;
                        m_objets.back().back().setMin(temp2);
                    }
                    if (caractere=='a')
                    {
                        int temp2;
                        fichier>>temp2;
                        m_objets.back().back().setMax(temp2);
                    }

                    if (caractere=='*')
                    {
                        m_objets.back().push_back(Objet ());
                        string temp2;
                        fichier>>temp2;
                        m_objets.back().back().Charger(temp2,m_caracteristique);
                    }

                    if (fichier.eof())
                    {
                        console->Ajouter("Error : Monster \" "+chemin+" \" is invalid",1);
                        caractere='$';
                    }
                }
                while (caractere!='$');
                fichier.get(caractere);
            }
            if (fichier.eof())
            {
                console->Ajouter("Error : Monster \" "+chemin+" \" is invalid",1);
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
                console->Ajouter("Error : Monster \" "+chemin+" \" is invalid",1);
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
                console->Ajouter("Error : Monster \" "+chemin+" \" is invalid",1);
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

std::vector <std::vector<Objet> > Modele_Monstre::getObjets()
{
    return m_objets;
}

void Monstre::TesterVision(coordonnee positionHero)
{
    if (m_caracteristique.vie>0)
    {
        if (abs(positionHero.x-m_positionCase.x)<5&&abs(positionHero.y-m_positionCase.y)<5)
        {
            m_vu=1;
            // if(m_etat==0) m_poseEnCours=0;
        }
        if (abs(positionHero.x-m_positionCase.x)>=15||abs(positionHero.y-m_positionCase.y)>=15)
        {
            if (m_vu)
                setDepart();
            m_vu=0/*,m_etat=0,m_entite_graphique.m_noAnimation=0*/;

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



