

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



#include "personnage.h"
#include "../globale.h"

#include <iostream>
#include <math.h>

#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>


using namespace std;
using namespace sf;

int calculerAngle(int x, int y)
{
    // Faudrait refaire plus proprement avec des fonctions trigonométrique, mais j'avais pas envie de réfléchir ^^'
    if (x>0)
    {
        if (y>0)
            return 270;
        else if (y<0)
            return 0;
        else
            return 315;
    }
    else if (x<0)
    {
        if (y>0)
            return 180;
        else if (y<0)
            return 90;
        else
            return 135;
    }
    else
    {
        if (y>0)
            return 225;
        else
            return 45;
    }
}

Personnage::Personnage()
{
    m_animation                         = 0;
    m_angle                             = 45;
    m_monstre                           = false;
    frappeEnCours                       = false;

    m_modele                            = -1;

    m_erreurPathfinding                 = false;

    m_caracteristique.pointAme          = 0;

    m_porteeLumineuseBasique.intensite  = -1;
    m_porteeLumineuseBasique.rouge      = 255;
    m_porteeLumineuseBasique.vert       = 255;
    m_porteeLumineuseBasique.bleu       = 255;

    m_positionPixel.h                   = 0;
    m_cheminFinal.h                     = 0;
    m_positionCase.h                    = 0;

    m_nombreInvocation                  = 0;

    m_shooter                           = false;

    m_cible                             = NULL;

    m_angle                             = 0;
    m_poseEnCours                       = 0;

    m_pousse.x                          = 0;
    m_pousse.y                          = 0;
    m_pousse.w                          = 0;

    m_miracleALancer                    = -1;

    m_positionPixel.h                   = 0;
    m_cheminFinal.h                     = 0;
    m_positionCase.h                    = 0;

    m_caracteristique.rang              = 0;

    m_impenetrable                      = 0;
    m_impoussable                       = 0;
    m_doitMourir                        = 0;

    m_vientDeFrapper                    = NULL;
    m_vientDetreTouche                  = NULL;
}
Modele_Personnage::Modele_Personnage()
{
    m_ombre=1;

    m_caracteristique.vie                = 1;
    m_caracteristique.maxVie             = 1;
    m_caracteristique.reserveVie         = 0;
    m_caracteristique.regenVie           = 0;

    m_caracteristique.foi                = 1;
    m_caracteristique.maxFoi             = 1;
    m_caracteristique.reserveFoi         = 0;
    m_caracteristique.regenFoi           = 0;

    m_caracteristique.volVie             = 0;
    m_caracteristique.volFoi             = 0;

    m_caracteristique.vitesse            = 1;
    m_caracteristique.pointAme           = 0;
    m_caracteristique.ancienPointAme     = 0;
    m_caracteristique.positionAncienAme  = 0;
    m_caracteristique.niveau             = 1;
    m_caracteristique.nom                = "Entitie";

    m_caracteristique.force              = 0;
    m_caracteristique.dexterite          = 0;
    m_caracteristique.vitalite           = 0;
    m_caracteristique.piete              = 0;
    m_caracteristique.charisme           = 0;
    m_caracteristique.pts_restant        = 0;
    m_caracteristique.miracles_restant   = 0;

    m_caracteristique.modificateurTaille = 1;

    m_caracteristique.degatsMin          = 0;
    m_caracteristique.degatsMax          = 0;
    m_caracteristique.armure             = 0;

    m_impenetrable                       = 0;
    m_impoussable                        = 0;
}


void Modele_Personnage::Reinitialiser()
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
}

Modele_Personnage::~Modele_Personnage()
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
}


bool Modele_Personnage::Charger(string chemin)
{
    for (unsigned i=0;i<m_pose.size();i++)
    {
        for (unsigned j=0;j<m_pose[i].size();j++)
            m_pose[i][j].clear();
        m_pose[i].clear();
    }

    m_pose.clear();

    m_image.clear();
    m_sons.clear();

    console->Ajouter("",0);
    console->Ajouter("Chargement du personnage : "+chemin,0);

    cDAT reader;

    if(reader.Read(chemin))
    {
        ifstream *fichier   = NULL;
        fichier             = reader.GetInfos("infos.txt");
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
                    console->Ajouter("Erreur : Personnage \" "+chemin+" \" Invalide",1);
                    caractere='$';
                }
            }
            while (caractere!='$');

            m_sons.clear();
            do
            {
                fichier->get(caractere);
                if (caractere=='*')
                {
                    do
                    {
                        fichier->get(caractere);
                        if (caractere=='m')
                        {
                            string cheminSon;
                            *fichier>>cheminSon;
                            m_sons.push_back(moteurSons->AjouterBuffer(cheminSon));
                        }
                        else if (caractere=='t')
                        {
                            int temp;
                            *fichier>>temp;
                            if(temp == 0)
                                m_sons_touche.push_back(m_sons.size()-1);
                        }
                        if (fichier->eof())
                        {
                            console->Ajouter("Erreur : Monstre \" "+chemin+" \" Invalide",1);
                            caractere='$';
                            m_caracteristique.maxVie=0;
                        }
                    }
                    while (caractere!='$');

                    fichier->get(caractere);
                }

                if (fichier->eof())
                {
                    console->Ajouter("Erreur : Monstre \" "+chemin+" \" Invalide",1);
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
                    do
                    {
                        fichier->get(caractere);
                        switch (caractere)
                        {
                        case 'r' :
                            *fichier>>m_porteeLumineuse.rouge;
                            break;
                        case 'v' :
                            *fichier>>m_porteeLumineuse.vert;
                            break;
                        case 'b' :
                            *fichier>>m_porteeLumineuse.bleu;
                            break;
                        case 'i' :
                            *fichier>>m_porteeLumineuse.intensite;
                            break;
                        }

                        if (fichier->eof())
                        {
                            console->Ajouter("Erreur : Personnage \" "+chemin+" \" Invalide",1);
                            caractere='$';
                        }

                    }
                    while (caractere!='$');
                    fichier->get(caractere);
                }
                if (fichier->eof())
                {
                    console->Ajouter("Erreur : Personnage \" "+chemin+" \" Invalide",1);
                    caractere='$';
                }

            }
            while (caractere!='$');

            ChargerPose(fichier);

            fichier->close();
        }
        else
        {
            console->Ajouter("Impossible d'ouvrir : "+chemin,1);
            return 0;
        }

        if(fichier != NULL)
            delete fichier;
    }
    else
    {
        console->Ajouter("Impossible d'ouvrir : "+chemin,1);
        return 0;
    }



    return true;
}

void Modele_Personnage::ChargerPose(ifstream *fichier)
{
    int etat=-1;
    char caractere=' ';

    do
    {
        fichier->get(caractere);

        if (caractere=='*')
        {
            fichier->putback(caractere);

            etat++;
            m_pose.push_back(vector<vector<Pose> >(0,vector<Pose>(0,Pose ())));

            fichier->get(caractere);

            for (int j=0;j<8;j++)
            {
                m_pose[etat].push_back(vector<Pose> (0,Pose ()));
                do
                {
                    if (caractere=='*')
                    {
                        coordonnee position(-1,-1,0,0),centre(-1000,-1000,-1,-1);
                        int animation=0,son=-1,image=0,attaque=-1,lumiere=m_porteeLumineuse.intensite,ordre=0, couche = 0;
                        float tempsAnimation=0.075;

                        do
                        {
                            fichier->get(caractere);
                            switch (caractere)
                            {
                            case 'x':
                                *fichier>>position.x;
                                break;
                            case 'y':
                                *fichier>>position.y;
                                break;
                            case 'w':
                                *fichier>>position.w;
                                break;
                            case 'h':
                                *fichier>>position.h;
                                break;
                            case 'a':
                                *fichier>>animation;
                                break;
                            case 't':
                                *fichier>>tempsAnimation;
                                break;
                            case 's':
                                *fichier>>son;
                                break;
                            case 'i':
                                *fichier>>image;
                                break;
                            case 'd':
                                *fichier>>attaque;
                                break;
                            case 'l':
                                *fichier>>lumiere;
                                break;

                            case 'o':
                                *fichier>>ordre;
                                break;

                            case 'c':
                                fichier->get(caractere);
                                if (caractere=='x') *fichier>>centre.x;
                                else *fichier>>centre.y;
                                break;

                            case 'u':
                                *fichier>>couche;
                                break;
                            }
                            if (fichier->eof())
                            {
                                console->Ajouter("Erreur : Entité Invalide",1);
                                caractere='$';
                                m_caracteristique.maxVie=0;
                            }
                        }
                        while (caractere!='$');

                        if (centre.x==-1000)
                            centre.x=position.w/2;
                        if (centre.y==-1000)
                            centre.y=position.h-32;

                        m_pose[etat][j].push_back(Pose ());
                        m_pose[etat][j].back().setPose(position,centre,animation,son,image,attaque,lumiere,tempsAnimation,ordre,couche);
                        fichier->get(caractere);
                        if (fichier->eof())
                        {
                            console->Ajouter("Erreur : Entité Invalide",1);
                            caractere='$';
                            m_caracteristique.maxVie=0;
                        }
                    }
                    fichier->get(caractere);
                    if (fichier->eof())
                    {
                        console->Ajouter("Erreur : Entité Invalide",1);
                        caractere='$';
                        m_caracteristique.maxVie=0;
                    }
                }
                while (caractere!='$');
            }
            fichier->get(caractere);
        }
    }
    while (caractere!='$');
}

void Personnage::Sauvegarder(ofstream &fichier)
{
    for(unsigned i = 0; i < m_effets.size(); ++i)
    {
        if(m_effets[i].m_effet.m_actif)
        {
            if(m_effets[i].m_type == AURA_CARACTERISTIQUES)
            {
                if(m_effets[i].m_info1 == 0)
                {
                    m_caracteristique.maxVie -= m_effets[i].m_info2;
                    m_caracteristique.vie -= m_effets[i].m_info2;
                }
            }
        }
    }

    fichier <<"* m" <<m_modele
            <<" vi" <<m_caracteristique.vie
            <<" va" <<m_caracteristique.maxVie
            <<" di" <<m_caracteristique.degatsMin
            <<" da" <<m_caracteristique.degatsMax
            <<" r"  <<m_caracteristique.rang
            <<" a"  <<m_caracteristique.pointAme
            <<" t"  <<m_caracteristique.modificateurTaille
            <<" p"  <<m_poseEnCours
            <<" e"  <<m_etat
            <<" g"  <<m_angle
            <<" lr" <<m_porteeLumineuse.rouge
            <<" lv" <<m_porteeLumineuse.vert
            <<" lb" <<m_porteeLumineuse.bleu
            <<" li" <<m_porteeLumineuse.intensite;

    for (unsigned o=0;o < m_objets.size();o++)
            m_objets[o].SauvegarderTexte(&fichier);

    fichier<<" $\n";

    for(unsigned i = 0; i < m_effets.size(); ++i)
    {
        if(m_effets[i].m_effet.m_actif)
        {
            if(m_effets[i].m_type == AURA_CARACTERISTIQUES)
            {
                if(m_effets[i].m_info1 == 0)
                {
                    m_caracteristique.maxVie += m_effets[i].m_info2;
                    m_caracteristique.vie    += m_effets[i].m_info2;
                }
            }
        }
    }
}

int Personnage::getOrdre(Modele_Personnage *modele)
{
    if (modele->m_pose.size()>0)
        if (m_etat>=0&&m_etat<NOMBRE_ETAT)
            if ((int)(m_angle/45)>=0&&(int)(m_angle/45)<(int)modele->m_pose[m_etat].size())
                if (m_poseEnCours>=0&&m_poseEnCours<(int)modele->m_pose[m_etat][(int)(m_angle/45)].size())
                    return modele->m_pose[m_etat][(int)(m_angle/45)][m_poseEnCours].getOrdre();

    return -10;
}

void Personnage::Afficher(coordonnee dimensionsMap,Modele_Personnage *modele,bool surbrillance, bool sansEffet)
{
    if(!sansEffet)
        for(int i = 0; i < (int)m_effets.size(); ++i)
        {
            m_effets[i].m_effet.m_position = m_positionPixel;
            m_effets[i].m_effet.Afficher();
        }

    if (modele!=NULL)
        if (modele->m_pose.size()>0)
            if ((int)(m_angle/45)>=0&&(int)(m_angle/45)<8)
            {
                Sprite sprite;

                if (configuration->Ombre&&modele->m_ombre)
                {
                    int angleOmbre=(int)((m_angle-moteurGraphique->m_angleOmbreSoleil)+22.5);

                    while (angleOmbre<0)
                        angleOmbre=360+angleOmbre;
                    while (angleOmbre>=360)
                        angleOmbre=angleOmbre-360;

                    if ((int)(angleOmbre/45)>=0&&(int)(angleOmbre/45)<(int)modele->m_pose[m_etat].size())
                        if (m_poseEnCours>=0&&m_poseEnCours<(int)modele->m_pose[m_etat][(int)(angleOmbre/45)].size())
                            if (modele->m_pose[m_etat][(int)(angleOmbre/45)][m_poseEnCours].getImage()>=0&&modele->m_pose[m_etat][(int)(angleOmbre/45)][m_poseEnCours].getImage()<(int)modele->m_image.size())
                            {
                                sprite.SetImage(*moteurGraphique->getImage(modele->m_image[modele->m_pose[m_etat][(int)(angleOmbre/45)][m_poseEnCours].getImage()]));
                                sprite.SetSubRect(IntRect(modele->m_pose[m_etat][(int)(angleOmbre/45)][m_poseEnCours].getCoordonnee().x, modele->m_pose[m_etat][(int)(angleOmbre/45)][m_poseEnCours].getCoordonnee().y, modele->m_pose[m_etat][(int)(angleOmbre/45)][m_poseEnCours].getCoordonnee().x+modele->m_pose[m_etat][(int)(angleOmbre/45)][m_poseEnCours].getCoordonnee().w, modele->m_pose[m_etat][(int)(angleOmbre/45)][m_poseEnCours].getCoordonnee().y+modele->m_pose[m_etat][(int)(angleOmbre/45)][m_poseEnCours].getCoordonnee().h));

                                sprite.SetScale(m_caracteristique.modificateurTaille,m_caracteristique.modificateurTaille);

                                sprite.SetOrigin(modele->m_pose[m_etat][(int)(angleOmbre/45)][m_poseEnCours].getCentre().x,modele->m_pose[m_etat][(int)(angleOmbre/45)][m_poseEnCours].getCentre().y);

                                sprite.SetX(((m_positionPixel.x-m_positionPixel.y)*64/COTE_TILE));
                                sprite.SetY(((m_positionPixel.x+m_positionPixel.y)*64/COTE_TILE)/2+32 -m_positionPixel.h);

                                sprite.SetScale(m_caracteristique.modificateurTaille, m_caracteristique.modificateurTaille*(100-(float)moteurGraphique->m_soleil.hauteur)/50);
                                sprite.SetRotation(moteurGraphique->m_angleOmbreSoleil);
                                moteurGraphique->AjouterCommande(&sprite,9,1);
                                sprite.SetOrigin(0,0);
                                sprite.SetScale(1, 1);
                                sprite.SetRotation(0);
                            }
                }

                if ((int)(m_angle/45)>=0&&(int)(m_angle/45)<(int)modele->m_pose[m_etat].size())
                    if (m_poseEnCours>=0&&m_poseEnCours<(int)modele->m_pose[m_etat][(int)(m_angle/45)].size())
                        if (modele->m_pose[m_etat][(int)(m_angle/45)][m_poseEnCours].getImage()>=0&&modele->m_pose[m_etat][(int)(m_angle/45)][m_poseEnCours].getImage()<(int)modele->m_image.size())
                        {
                            sprite.SetImage(*moteurGraphique->getImage(modele->m_image[modele->m_pose[m_etat][(int)(m_angle/45)][m_poseEnCours].getImage()]));

                            sprite.SetOrigin(modele->m_pose[m_etat][(int)(m_angle/45)][m_poseEnCours].getCentre().x,modele->m_pose[m_etat][(int)(m_angle/45)][m_poseEnCours].getCentre().y);

                            sprite.SetSubRect(IntRect(modele->m_pose[m_etat][(int)(m_angle/45)][m_poseEnCours].getCoordonnee().x, modele->m_pose[m_etat][(int)(m_angle/45)][m_poseEnCours].getCoordonnee().y, modele->m_pose[m_etat][(int)(m_angle/45)][m_poseEnCours].getCoordonnee().x+modele->m_pose[m_etat][(int)(m_angle/45)][m_poseEnCours].getCoordonnee().w, modele->m_pose[m_etat][(int)(m_angle/45)][m_poseEnCours].getCoordonnee().y+modele->m_pose[m_etat][(int)(m_angle/45)][m_poseEnCours].getCoordonnee().h));

                            sprite.FlipX(false);


                            sprite.SetX(((m_positionPixel.x-m_positionPixel.y)*64/COTE_TILE/*+dimensionsMap.y*64*/));
                            sprite.SetY(((m_positionPixel.x+m_positionPixel.y)*32/COTE_TILE)+32 -m_positionPixel.h);


                            sprite.SetScale(m_caracteristique.modificateurTaille,m_caracteristique.modificateurTaille);


                            if (m_porteeLumineuse.intensite>0)
                                sprite.SetColor(sf::Color(m_porteeLumineuse.rouge,m_porteeLumineuse.vert,m_porteeLumineuse.bleu, 255));
                            else
                                sprite.SetColor(sf::Color(255,255,255, 255));

                            if(sprite.GetPosition().x+sprite.GetSize().x                                                    >= moteurGraphique->m_camera.GetRect().Left
                            && sprite.GetPosition().x-modele->m_pose[m_etat][(int)(m_angle/45)][m_poseEnCours].getCentre().x<  moteurGraphique->m_camera.GetRect().Right
                            && sprite.GetPosition().y+sprite.GetSize().y                                                    >= moteurGraphique->m_camera.GetRect().Top
                            && sprite.GetPosition().y                                                                       <  moteurGraphique->m_camera.GetRect().Bottom)
                            {
                                moteurGraphique->AjouterCommande(&sprite,10 + modele->m_pose[m_etat][(int)(m_angle/45)][m_poseEnCours].getCouche(),1);

                                if (surbrillance)
                                {
                                    sprite.SetBlendMode(sf::Blend::Add);
                                    moteurGraphique->AjouterCommande(&sprite,10,1);
                                }

                                sprite.FlipY(true);
                                sprite.Move(0,modele->m_pose[m_etat][(int)(m_angle/45)][m_poseEnCours].getCentre().y-sprite.GetSize().y*0.5);
                                sprite.Scale(1,0.5);
                                moteurGraphique->AjouterCommande(&sprite,0,1);
                            }
                        }
            }
}
void Personnage::regenererVie(float vie)
{
    m_caracteristique.vie+=vie;
    if (m_caracteristique.vie>m_caracteristique.maxVie)
        m_caracteristique.vie=m_caracteristique.maxVie;
}

int Personnage::Pathfinding(casePathfinding** map,coordonnee exception, bool noDelete)
{
    //if(!(m_arrivee.x==m_mauvaiseArrivee.x&&m_arrivee.y==m_mauvaiseArrivee.y))
    if (!(m_arrivee.x==m_positionCase.x&&m_arrivee.y==m_positionCase.y))
    {
        bool retest = false;
        m_erreurPathfinding=false;
        liste_case casesVisitee;
        coordonnee depart,arrivee,decalage;

        decalage.x=m_positionCase.x-5;
        decalage.y=m_positionCase.y-5;

        depart.x=m_positionCase.x-decalage.x;
        depart.y=m_positionCase.y-decalage.y;
        arrivee.x=m_arrivee.x-decalage.x;
        arrivee.y=m_arrivee.y-decalage.y;
        exception.x=exception.x-decalage.x;
        exception.y=exception.y-decalage.y;

        if (exception.x>=0&&exception.x<10&&exception.y>=0&&exception.y<10)
            map[exception.y][exception.x].collision=1;

        casesVisitee.setTailleListe(0);

        casesVisitee.AjouterCase(depart, -1);

        if (arrivee.y>=0&&arrivee.x>=0&&arrivee.y<10&&arrivee.x<10)
            if (map[arrivee.y][arrivee.x].collision)
            {
                coordonnee temp(-100,-100,-1,-1),enCours;

                enCours.y=arrivee.y-1;
                enCours.x=arrivee.x-1;
                if (enCours.y>=0&&enCours.y<10&&enCours.x>=0&&enCours.x<10)
                    if (!map[enCours.y][enCours.x].collision&&!(enCours.x==depart.x&&enCours.y==depart.y))
                        if (((enCours.y-depart.y)*(enCours.y-depart.y)+(enCours.x-depart.x)*(enCours.x-depart.x)) < ((temp.y-depart.y)*(temp.y-depart.y)+(temp.x-depart.x)*(temp.x-depart.x)))
                            temp.y=enCours.y,temp.x=enCours.x;
                enCours.y=arrivee.y;
                enCours.x=arrivee.x-1;
                if (enCours.y>=0&&enCours.y<10&&enCours.x>=0&&enCours.x<10)
                    if (!map[enCours.y][enCours.x].collision&&!(enCours.x==depart.x&&enCours.y==depart.y))
                        if (((enCours.y-depart.y)*(enCours.y-depart.y)+(enCours.x-depart.x)*(enCours.x-depart.x)) < ((temp.y-depart.y)*(temp.y-depart.y)+(temp.x-depart.x)*(temp.x-depart.x)))
                            temp.y=enCours.y,temp.x=enCours.x;
                enCours.y=arrivee.y-1;
                enCours.x=arrivee.x;
                if (enCours.y>=0&&enCours.y<10&&enCours.x>=0&&enCours.x<10)
                    if (!map[enCours.y][enCours.x].collision&&!(enCours.x==depart.x&&enCours.y==depart.y))
                        if (((enCours.y-depart.y)*(enCours.y-depart.y)+(enCours.x-depart.x)*(enCours.x-depart.x)) < ((temp.y-depart.y)*(temp.y-depart.y)+(temp.x-depart.x)*(temp.x-depart.x)))
                            temp.y=enCours.y,temp.x=enCours.x;

                enCours.y=arrivee.y+1;
                enCours.x=arrivee.x+1;
                if (enCours.y>=0&&enCours.y<10&&enCours.x>=0&&enCours.x<10)
                    if (!map[enCours.y][enCours.x].collision&&!(enCours.x==depart.x&&enCours.y==depart.y))
                        if (((enCours.y-depart.y)*(enCours.y-depart.y)+(enCours.x-depart.x)*(enCours.x-depart.x)) < ((temp.y-depart.y)*(temp.y-depart.y)+(temp.x-depart.x)*(temp.x-depart.x)))
                            temp.y=enCours.y,temp.x=enCours.x;
                enCours.y=arrivee.y+1;
                enCours.x=arrivee.x;
                if (enCours.y>=0&&enCours.y<10&&enCours.x>=0&&enCours.x<10)
                    if (!map[enCours.y][enCours.x].collision&&!(enCours.x==depart.x&&enCours.y==depart.y))
                        if (((enCours.y-depart.y)*(enCours.y-depart.y)+(enCours.x-depart.x)*(enCours.x-depart.x)) < ((temp.y-depart.y)*(temp.y-depart.y)+(temp.x-depart.x)*(temp.x-depart.x)))
                            temp.y=enCours.y,temp.x=enCours.x;
                enCours.y=arrivee.y;
                enCours.x=arrivee.x+1;
                if (enCours.y>=0&&enCours.y<10&&enCours.x>=0&&enCours.x<10)
                    if (!map[enCours.y][enCours.x].collision&&!(enCours.x==depart.x&&enCours.y==depart.y))
                        if (((enCours.y-depart.y)*(enCours.y-depart.y)+(enCours.x-depart.x)*(enCours.x-depart.x)) < ((temp.y-depart.y)*(temp.y-depart.y)+(temp.x-depart.x)*(temp.x-depart.x)))
                            temp.y=enCours.y,temp.x=enCours.x;


                enCours.y=arrivee.y+1;
                enCours.x=arrivee.x-1;
                if (enCours.y>=0&&enCours.y<10&&enCours.x>=0&&enCours.x<10)
                    if (!map[enCours.y][enCours.x].collision&&!(enCours.x==depart.x&&enCours.y==depart.y))
                        if (((enCours.y-depart.y)*(enCours.y-depart.y)+(enCours.x-depart.x)*(enCours.x-depart.x)) < ((temp.y-depart.y)*(temp.y-depart.y)+(temp.x-depart.x)*(temp.x-depart.x)))
                            temp.y=enCours.y,temp.x=enCours.x;
                enCours.y=arrivee.y-1;
                enCours.x=arrivee.x+1;
                if (enCours.y>=0&&enCours.y<10&&enCours.x>=0&&enCours.x<10)
                    if (!map[enCours.y][enCours.x].collision&&!(enCours.x==depart.x&&enCours.y==depart.y))
                        if (((enCours.y-depart.y)*(enCours.y-depart.y)+(enCours.x-depart.x)*(enCours.x-depart.x)) < ((temp.y-depart.y)*(temp.y-depart.y)+(temp.x-depart.x)*(temp.x-depart.x)))
                            temp.y=enCours.y,temp.x=enCours.x;

                if (temp.y!=-100&&temp.x!=-100)
                    arrivee=temp, m_arrivee.x = temp.x + decalage.x,  m_arrivee.y = temp.y + decalage.y;
                else
                    m_erreurPathfinding=true;
            }

        casesVisitee.IncrementerDistanceEnCours();
        while (!casesVisitee.AjouterCasesAdjacentes(map,&arrivee,depart)&&!m_erreurPathfinding)
        {
            casesVisitee.IncrementerDistanceEnCours();
            if (casesVisitee.getDistance()>10)
                m_erreurPathfinding=true, retest = true;
        }

        if (!m_erreurPathfinding)
        {
            m_cheminFinal=arrivee;

            Case temp = casesVisitee.getCase(casesVisitee.getTailleListe() - 1);

            if(temp.getParent() == -1)
                temp = casesVisitee.getCase(temp.getParent());
            else
                while (casesVisitee.getCase(temp.getParent()).getParent() != -1)
                    temp = casesVisitee.getCase(temp.getParent());

            m_cheminFinal = temp.getPosition();

            m_arrivee.x=arrivee.x+decalage.x;
            m_arrivee.y=arrivee.y+decalage.y;

            m_cheminFinal.h=map[m_cheminFinal.y][m_cheminFinal.x].hauteur;

            m_cheminFinal.x+=decalage.x,m_cheminFinal.y+=decalage.y;

            m_ancienneArrivee=m_arrivee;
        }
        else
        {
            if(!noDelete && retest)
            {
                if(m_arrivee.x - m_positionCase.x > 0)
                    m_arrivee.x = m_positionCase.x + 1;
                else if(m_arrivee.x - m_positionCase.x < 0)
                    m_arrivee.x = m_positionCase.x - 1;
                else
                    m_arrivee.x = m_positionCase.x;

                if(m_arrivee.y - m_positionCase.y > 0)
                    m_arrivee.y = m_positionCase.y + 1;
                else if(m_arrivee.y - m_positionCase.y < 0)
                    m_arrivee.y = m_positionCase.y - 1;
                else
                    m_arrivee.y = m_positionCase.y;

                /*float m = atan2(m_arrivee.y - m_positionCase.y, m_arrivee.x - m_positionCase.x);

                if( m <= 3*M_PI/8 && m >= -3*M_PI/8 )
                    m_arrivee.x = m_positionCase.x + 1;

                if( m >= 5*M_PI/8 && m <= M_PI  ||  m <= -5*M_PI/8 && m >= -M_PI )
                    m_arrivee.x = m_positionCase.x - 1;

                if( m >= M_PI/8 && m <= 7*M_PI/8 )
                    m_arrivee.y = m_positionCase.y + 1;

                if( m <= -M_PI/8 && m >= -7*M_PI/8 )
                    m_arrivee.y = m_positionCase.y - 1;*/

                for(int i = 0 ; i < 10 ; ++i)
                for(int j = 0 ; j < 10 ; ++j)
                    map[i][j].valeur = -1, map[i][j].dist = 3, map[i][j].cases = -1;

                Pathfinding(map, exception, true);
            }
            else
            {
                m_arrivee=m_positionCase;
                m_cheminFinal=m_positionCase;
                if (m_etat!=0)
                    m_etat=0,m_poseEnCours=0,frappeEnCours=0;
            }
        }

        if(!noDelete)
        {
            for (int i=0;i<10;i++)
                delete[] map[i];
            delete[] map;
        }


        if (m_etat==0)
            return 0;
        else
            return 1;
    }

    if(!noDelete)
    {
        for (int i=0;i<10;i++)
            delete[] map[i];
        delete[] map;
    }


    return 2;
}


bool Personnage::SeDeplacer(float tempsEcoule,coordonnee dimensionsMap, bool pousserPossible)
{
    //int buf=(int)(tempsEcoule*1000);
    //tempsEcoule=(float)buf/1000;

    sf::Vector2f pos;
    pos.x=((m_positionPixel.x-m_positionPixel.y)*64/COTE_TILE);
    pos.y=((m_positionPixel.x+m_positionPixel.y)*64/COTE_TILE)+64;

    moteurGraphique->LightManager->SetPosition(m_light,pos);

    if(!pousserPossible)
    {
        m_pousse.x = 0;
        m_pousse.y = 0;
    }

    if((m_pousse.x != 0 || m_pousse.y != 0) && pousserPossible)
    {
        if(pousserPossible)
        {
            m_positionPixel.x += m_pousse.x * tempsEcoule * COTE_TILE * 0.05;
            m_positionPixel.y += m_pousse.y * tempsEcoule * COTE_TILE * 0.05;

            bool xgrand = (m_pousse.x > 0);
            bool ygrand = (m_pousse.y > 0);

            m_pousse.x -= tempsEcoule * (-1 + xgrand * 2) * 0.025;
            m_pousse.y -= tempsEcoule * (-1 + ygrand * 2) * 0.025;

            if(xgrand && m_pousse.x < 0)
                m_pousse.x = 0;
            else if(!xgrand && m_pousse.x > 0)
                m_pousse.x = 0;

            if(ygrand && m_pousse.y < 0)
                m_pousse.y = 0;
            else if(!ygrand && m_pousse.y > 0)
                m_pousse.y = 0;
        }
        else
            m_pousse.y = 0, m_pousse.x = 0;

        m_positionCase.x    = (int)((m_positionPixel.x+COTE_TILE*0.5)/COTE_TILE);
        m_positionCase.y    = (int)((m_positionPixel.y+COTE_TILE*0.5)/COTE_TILE);

        m_arrivee.x         = m_positionCase.x;
        m_arrivee.y         = m_positionCase.y;

        m_cheminFinal.x     = m_positionCase.x;
        m_cheminFinal.y     = m_positionCase.y;

        frappeEnCours       = 0;

        return 1;
    }
    else if (m_caracteristique.vie > 0)
    {
        if (!frappeEnCours)
        {
            if (m_positionCase.y!=m_cheminFinal.y||m_positionCase.x!=m_cheminFinal.x)
            {
                if (m_etat!=1)
                    m_etat=1,m_poseEnCours=0;

                m_positionPixelPrecedente.x=(int)m_positionPixel.x;
                m_positionPixelPrecedente.y=(int)m_positionPixel.y;

                if (m_positionCase.x<m_cheminFinal.x)
                {
                    if (m_positionCase.h!=m_cheminFinal.h && m_positionPixel.h!=m_cheminFinal.h)
                        m_positionPixel.h=m_positionCase.h*( COTE_TILE - fabs(m_positionPixel.x-m_positionCase.x*COTE_TILE))/COTE_TILE + m_cheminFinal.h*( fabs(m_positionPixel.x-m_positionCase.x*COTE_TILE))/COTE_TILE ;
                    if (m_positionCase.y>m_cheminFinal.y)
                        m_positionPixel.x+=(float)2*tempsEcoule*m_caracteristique.vitesse;
                    else
                        m_positionPixel.x+=(float)4*tempsEcoule*m_caracteristique.vitesse;
                }
                else if (m_positionCase.x>m_cheminFinal.x)
                {
                    if (m_positionCase.h!=m_cheminFinal.h && m_positionPixel.h!=m_cheminFinal.h)
                        m_positionPixel.h=m_positionCase.h*( COTE_TILE - fabs(m_positionPixel.x-m_positionCase.x*COTE_TILE))/COTE_TILE + m_cheminFinal.h*( fabs(m_positionPixel.x-m_positionCase.x*COTE_TILE))/COTE_TILE ;
                    if (m_positionCase.y<m_cheminFinal.y)
                        m_positionPixel.x-=(float)2*tempsEcoule*m_caracteristique.vitesse;
                    else
                        m_positionPixel.x-=(float)4*tempsEcoule*m_caracteristique.vitesse;
                }
                if (m_positionCase.y<m_cheminFinal.y)
                {
                    if (m_positionCase.h!=m_cheminFinal.h && m_positionPixel.h!=m_cheminFinal.h)
                        m_positionPixel.h=m_positionCase.h*( COTE_TILE - fabs(m_positionPixel.y-m_positionCase.y*COTE_TILE))/COTE_TILE + m_cheminFinal.h*( fabs(m_positionPixel.y-m_positionCase.y*COTE_TILE))/COTE_TILE ;
                    if (m_positionCase.x>m_cheminFinal.x)
                        m_positionPixel.y+=(float)2*tempsEcoule*m_caracteristique.vitesse;
                    else
                        m_positionPixel.y+=(float)4*tempsEcoule*m_caracteristique.vitesse;
                }
                else if (m_positionCase.y>m_cheminFinal.y)
                {
                    if (m_positionCase.h!=m_cheminFinal.h && m_positionPixel.h!=m_cheminFinal.h)
                        m_positionPixel.h=m_positionCase.h*( COTE_TILE - fabs(m_positionPixel.y-m_positionCase.y*COTE_TILE))/COTE_TILE + m_cheminFinal.h*( fabs(m_positionPixel.y-m_positionCase.y*COTE_TILE))/COTE_TILE ;
                    if (m_positionCase.x<m_cheminFinal.x)
                        m_positionPixel.y-=(float)2*tempsEcoule*m_caracteristique.vitesse;
                    else
                        m_positionPixel.y-=(float)4*tempsEcoule*m_caracteristique.vitesse;
                }

                m_angle=calculerAngle(m_cheminFinal.x-m_positionCase.x,m_cheminFinal.y-m_positionCase.y);

                if ((m_positionCase.x<m_cheminFinal.x&&m_positionPixel.x>=m_cheminFinal.x*COTE_TILE)
                        ||(m_positionCase.x>m_cheminFinal.x&&m_positionPixel.x<=m_cheminFinal.x*COTE_TILE)
                        ||m_positionCase.x==m_cheminFinal.x)
                    if ((m_positionCase.y<m_cheminFinal.y&&m_positionPixel.y>=m_cheminFinal.y*COTE_TILE)
                            ||(m_positionCase.y>m_cheminFinal.y&&m_positionPixel.y<=m_cheminFinal.y*COTE_TILE)
                            ||m_positionCase.y==m_cheminFinal.y)
                    {
                        m_positionPixel.x=(m_cheminFinal.x*COTE_TILE);
                        m_positionPixel.y=(m_cheminFinal.y*COTE_TILE);

                        m_positionPixel.h=m_cheminFinal.h;

                        m_positionCase.y=m_cheminFinal.y;
                        m_positionCase.x=m_cheminFinal.x;
                        m_positionCase.h=m_cheminFinal.h;

                        return 1;
                    }
            }
            else if (m_arrivee.x!=m_positionCase.x||m_arrivee.y!=m_positionCase.y)
                return 1;
            else
            {
                if (m_etat!=0)
                    m_poseEnCours=0;

                m_etat=0,frappeEnCours=0;
            }
        }
        if (m_etat==2||m_arrivee.x==m_positionCase.x&&m_arrivee.y==m_positionCase.y)
            return 1;

        if(m_caracteristique.vitesse == 0)
            m_cheminFinal = m_positionCase;
    }
    else
        return 1;

    return 0;
}

void Personnage::InfligerDegats(float degats, Modele_Personnage *modele)
{
    float temp = degats;
    degats -= (float)m_caracteristique.armure/50;

    if (degats < 0)
        degats = 0;
    if (degats > temp)
        degats = temp;

    m_caracteristique.vie-=degats;

    m_cible = NULL;

    if(modele != NULL)
        if(modele->getNombreSonsTouche() > 0)
        {
            int random = rand()%modele->getNombreSonsTouche();
            coordonnee position;
            position.x=(m_positionCase.x-m_positionCase.y-1)/5;
            position.y=(m_positionCase.x+m_positionCase.y)/5;

            modele->JouerSon(modele->getSonTouche(random),position,coordonnee (1,1), true);
        }

    if (m_caracteristique.vie<=0&&m_etat!=3)
        m_poseEnCours=0,m_etat=3;

    m_touche = true;
}

int Personnage::Animer(Modele_Personnage *modele,float temps,coordonnee positionHero)
{
    int retour=-2;

    if(m_monstre)
        retour = 0;

    int nombreInactif = 0;
    for(int i = 0; i < (int)m_effets.size(); ++i)
    {
        if(m_caracteristique.vie <= 0)
            m_effets[i].m_effet.m_actif = false;

        bool actif = m_effets[i].m_effet.m_actif;
        m_effets[i].m_effet.Animer(temps);

        if(actif && !m_effets[i].m_effet.m_actif)
        {
            if(m_effets[i].m_type == AURA_CARACTERISTIQUES)
            {
                if(m_effets[i].m_info1 == 0)
                {
                    m_caracteristique.maxVie -= m_effets[i].m_info2;
                    m_caracteristique.vie -= m_effets[i].m_info2;
                }
            }
        }

        if(!m_effets[i].m_effet.m_actif)
            ++nombreInactif;
        else if(!m_doitMourir)
        {
            float viePrecedente = m_caracteristique.vie;

            if(m_effets[i].m_type == AURA_REGENERATION)
            {
                if(m_effets[i].m_info3)
                {
                    if(m_effets[i].m_info1 == 0)
                        m_caracteristique.vie += m_effets[i].m_info2 * temps;
                    if(m_effets[i].m_info1 == 1)
                        m_caracteristique.foi += m_effets[i].m_info2 * temps;
                }
            }

            if(m_monstre)
                if(m_caracteristique.vie > m_caracteristique.maxVie)
                    m_caracteristique.vie = m_caracteristique.maxVie;


            if(m_caracteristique.vie <= 0 && viePrecedente >= 0)
                m_caracteristique.vie = viePrecedente, m_doitMourir = true;
        }
    }


    if(nombreInactif == (int)m_effets.size() || !EnVie())
        m_effets.clear();

    m_animation+=temps;

    float tempsAnimation=0.075;

    if(m_etat >= 0 && m_etat < (int)modele->m_pose.size())
    if((int)(m_angle/45) >= 0 && (int)(m_angle/45) < (int)modele->m_pose[m_etat].size())
    if(m_poseEnCours >= 0 && m_poseEnCours < (int)modele->m_pose[m_etat][(int)(m_angle/45)].size())
    {
        if (modele->m_pose.size()>0)
            tempsAnimation = modele->m_pose[m_etat][(int)(m_angle/45)][m_poseEnCours].getTempsAnimation();

        if (modele->m_pose.size()>0&&tempsAnimation>0)
            while (m_animation>=tempsAnimation)
            {
                coordonnee position;
                position.x=(m_positionCase.x-m_positionCase.y-1)/5;
                position.y=(m_positionCase.x+m_positionCase.y)/5;

                m_poseEnCours=modele->m_pose[m_etat][(int)(m_angle/45)][m_poseEnCours].getAnimation();

                if (m_poseEnCours>=(int)modele->m_pose[m_etat][(int)(m_angle/45)].size())
                    m_poseEnCours=0;

                modele->JouerSon(modele->m_pose[m_etat][(int)(m_angle/45)][m_poseEnCours].getSon(),position,positionHero);

                m_animation-=tempsAnimation;
                tempsAnimation = modele->m_pose[m_etat][(int)(m_angle/45)][m_poseEnCours].getTempsAnimation();

                if (m_monstre)
                {
                    if (modele->m_pose[m_etat][(int)(m_angle/45)][m_poseEnCours].getAttaque()==0)
                    {
                        if(m_miracleALancer >= 0)
                            retour = 1;
                        else
                            retour+=(rand()%(m_caracteristique.degatsMax-m_caracteristique.degatsMin+1)+m_caracteristique.degatsMin);
                    }
                }
                else
                {
                    if (modele->m_pose[m_etat][(int)(m_angle/45)][m_poseEnCours].getAttaque()>retour)
                        retour=modele->m_pose[m_etat][(int)(m_angle/45)][m_poseEnCours].getAttaque();
                }

                if (modele->m_pose[m_etat][(int)(m_angle/45)][m_poseEnCours].getAttaque()==1 && m_monstre)
                    frappeEnCours=false,m_miracleALancer=-1;

                if (m_monstre)
                {
                    if (modele->m_pose[m_etat][(int)(m_angle/45)][m_poseEnCours].getLumiereIntensite()!=-1&&m_caracteristique.rang==0)
                        m_porteeLumineuse.intensite=modele->m_pose[m_etat][(int)(m_angle/45)][m_poseEnCours].getLumiereIntensite();

                    float inte=m_porteeLumineuse.intensite;
                    if (inte>255)
                        inte=255;

                    moteurGraphique->LightManager->SetIntensity(m_light,(int)inte);
                    moteurGraphique->LightManager->SetRadius(m_light,(int)m_porteeLumineuse.intensite*2);
                }
            }
    }
    return retour;
}

void Personnage::Frappe(coordonnee direction,coordonnee position)
{
    if (m_etat<2)
    {
        m_etat=2;
        m_poseEnCours=0;
    }

    if(!frappeEnCours)
        m_poseEnCours=0;

    frappeEnCours=1;

    float m=atan2((double)(direction.x-position.x),(double)(direction.y-position.y));
    m+=M_PI/3;

    m_angle=(int)(m*180/M_PI);
    if (m_angle>=360)
        m_angle=0;
    if (m_angle<0)
        m_angle=360+m_angle;

    m_cheminFinal=m_positionCase;
    m_arrivee=m_cheminFinal;
}

void Personnage::DetruireEffets()
{
    for(unsigned i = 0; i < m_effets.size(); ++i)
    {
        if(m_effets[i].m_effet.m_actif)
        {
            if(m_effets[i].m_type == AURA_CARACTERISTIQUES)
            {
                if(m_effets[i].m_info1 == 0)
                {
                    m_caracteristique.maxVie -= m_effets[i].m_info2;
                    m_caracteristique.vie -= m_effets[i].m_info2;
                }
            }
        }
    }

    m_effets.clear();
}

int Personnage::AjouterEffet(std::vector<Tile> &tiles, int type, int compteur, int info1, int info2, int info3)
{
    m_effets.push_back(EffetPersonnage());

    m_effets.back().m_effet.m_tiles     = tiles;
    m_effets.back().m_effet.m_compteur  = compteur;
    m_effets.back().m_effet.m_position  = m_positionPixel;

    m_effets.back().m_type              = type;
    m_effets.back().m_info1             = info1;
    m_effets.back().m_info2             = info2;
    m_effets.back().m_info3             = info3;

    if(m_effets.back().m_type == AURA_CARACTERISTIQUES)
    {
        if(m_effets.back().m_info1 == 0)
        {
            m_caracteristique.maxVie += m_effets.back().m_info2;
            m_caracteristique.vie += m_effets.back().m_info2;
        }
    }

    return m_effets.size() - 1;
}


void Modele_Personnage::JouerSon(int numeroSon,coordonnee position,coordonnee positionHero,bool uniqueSound)
{
    if (numeroSon>=0 && numeroSon<(int)m_sons.size())
    {
        coordonnee pos;
        pos.x=-position.x;
        pos.y=position.y;
        moteurSons->JouerSon(m_sons[numeroSon],pos,positionHero,uniqueSound);
    }
}



void Modele_Personnage::setPorteeLumineuse(Lumiere  lumiere)
{
    m_porteeLumineuse=lumiere;
}
void Personnage::setPorteeLumineuse(Lumiere  lumiere)
{
    m_porteeLumineuse=lumiere;
}
void Personnage::setCaracteristique(Caracteristique caracteristique)
{
    m_caracteristique=caracteristique;
}
void Personnage::setProchaineCase(coordonnee position)
{
    m_cheminFinal=position;
}
void Personnage::setVitesse(float vitesse)
{
    m_caracteristique.vitesse=vitesse;
}

void Personnage::setEtat(int etat)
{
    m_etat=etat,m_poseEnCours=0,frappeEnCours=false;
}
void Personnage::setJustEtat(int etat)
{
    m_etat=etat/*,m_poseEnCours=0*/;

    if(etat == 2)
        frappeEnCours = 1;
}
void Personnage::addAngle(int angle)
{
    m_angle += angle;
    if(m_angle < 0)
        m_angle += 360;
    if(m_angle > 360)
        m_angle -= 360;
}


void Personnage::setPose(int pose)
{
    m_poseEnCours=pose;
}
void Personnage::setAngle(int angle)
{
    m_angle=angle;
}
void Personnage::setErreurPathfinding(bool erreur)
{
    m_erreurPathfinding=erreur;
}
void Personnage::setCoordonnee(coordonnee nouvellesCoordonnees)
{
    m_positionCase=nouvellesCoordonnees;
    m_etat=ARRET;

    m_positionPixel.x=(float)nouvellesCoordonnees.x*COTE_TILE;
    m_positionPixel.y=(float)nouvellesCoordonnees.y*COTE_TILE;

    m_arrivee.x=m_positionCase.x;
    m_arrivee.y=m_positionCase.y;

    m_cheminFinal.x=m_positionCase.x;
    m_cheminFinal.y=m_positionCase.y;

    //m_angle=0;
    m_poseEnCours=0;

    //moteurGraphique->LightManager->SetPosition(m_light,sf::Vector2f(m_positionPixel.x,m_positionPixel.y));

    m_cible = NULL;
}
void Personnage::setArrivee(coordonnee arrivee)
{
    if (!(m_mauvaiseArrivee.x==arrivee.x&&m_mauvaiseArrivee.y==arrivee.y))
    {
        m_arrivee=arrivee;
    }
}
void Personnage::setMauvaiseArrivee(coordonnee arrivee)
{
    m_mauvaiseArrivee=arrivee;
}

void Personnage::setCoordonneePixel(coordonnee position)
{
    m_positionPixel.x=position.x*COTE_TILE;
    m_positionPixel.y=position.y*COTE_TILE;
}

void Personnage::setDepart()
{
    m_depart=m_positionCase;
}

void Personnage::setModele(int modele)
{
    m_modele = modele;
}


void Personnage::setPousse(coordonneeDecimal pousse)
{
    if(!m_impoussable)
    {
        m_pousse.x = pousse.x;
        m_pousse.y = pousse.y;
    }
}

void Personnage::Pousser(coordonneeDecimal vecteur)
{
    if(!m_impoussable)
    {
        if(fabs(vecteur.x) > fabs(m_pousse.x))
            m_pousse.x = vecteur.x;

        if(fabs(vecteur.y) > fabs(m_pousse.y))
            m_pousse.y = vecteur.y;
    }
}



void Personnage::AjouterPointAme(int pointAme)
{
    m_caracteristique.pointAme+=pointAme;
}

bool Personnage::EnVie()
{
    if (m_caracteristique.vie>0) return 1;
    else return 0;
}

const coordonnee &Personnage::getDepart()
{
    return m_depart;
}

int Modele_Personnage::getNombreSons()
{
    return m_sons.size();
}

int Modele_Personnage::getNombreSonsTouche()
{
    return m_sons_touche.size();
}

int Modele_Personnage::getSonTouche(int son)
{
    if(son >= 0 && son < (int)m_sons_touche.size())
        return m_sons_touche[son];

    return -1;
}

const coordonnee &Personnage::getCoordonnee()
{
    return m_positionCase;
}
const coordonnee &Personnage::getArrivee()
{
    return m_arrivee;
}

const Caracteristique &Personnage::getCaracteristique()
{
    return m_caracteristique;
}
const Caracteristique &Modele_Personnage::getCaracteristique()
{
    return m_caracteristique;
}
const std::string &Personnage::getNom()
{
    return m_caracteristique.nom;
}

const Lumiere &Modele_Personnage::getPorteeLumineuse()
{
    return m_porteeLumineuse;
}
const Lumiere &Personnage::getPorteeLumineuse()
{
    return m_porteeLumineuse;
}
int Personnage::getEtat()
{
    return m_etat;
}
int Personnage::getAngle()
{
    return m_angle;
}
int Personnage::getPose()
{
    return m_poseEnCours;
}
bool Personnage::getErreurPathfinding()
{
    return m_erreurPathfinding;
}
const coordonneeDecimal &Personnage::getCoordonneePixel()
{
    return m_positionPixel;
}
const coordonneeDecimal &Personnage::getPousse()
{
    return m_pousse;
}
const coordonnee &Personnage::getProchaineCase()
{
    if (m_cheminFinal.x!=m_positionCase.x||m_cheminFinal.y!=m_positionCase.y)
        return m_cheminFinal;
    else
        return m_positionCase;
}

const std::vector<Objet> &Personnage::getObjets()
{
    return m_objets;
}
std::vector<Objet>* Personnage::getPointeurObjets()
{
    return &m_objets;
}

void Personnage::setObjets(std::vector<Objet> objets)
{
    m_objets=objets;
}

int Personnage::getModele()
{
    return m_modele;
}

