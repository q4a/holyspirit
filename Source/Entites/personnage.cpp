

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



#include "Personnage.h"
#include "../Globale.h"

#include <iostream.h>
#include <fstream>
#include <math.h>

#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>


using namespace std;
using namespace sf;

int calculerAngle(int x, int y)
{
    // Faudrait refaire plus proprement avec des fonctions trigonométrique, mais j'avais pas envie de réfléchir ^^'
    if(x>0)
    {
        if(y>0)
            return 270;
        else if(y<0)
            return 0;
        else
            return 315;
    }
    else if(x<0)
    {
        if(y>0)
            return 180;
        else if(y<0)
            return 90;
        else
            return 135;
    }
    else
    {
        if(y>0)
            return 225;
        else
            return 45;
    }
}

Personnage::Personnage()
{
    m_animation=0;
    m_angle=45;
    m_monstre=false;
    frappeEnCours=false;

    m_erreurPathfinding=false;

    m_porteeLumineuseBasique.intensite=-1;
    m_porteeLumineuseBasique.rouge=255;
    m_porteeLumineuseBasique.vert=255;
    m_porteeLumineuseBasique.bleu=255;

    m_positionPixel.h=0;
    m_cheminFinal.h=0;
    m_positionCase.h=0;

    m_nombreInvocation=0;
}
Modele_Personnage::Modele_Personnage()
{
    m_ombre=1;
}


void Modele_Personnage::Reinitialiser()
{
    for(int i=0;i<(int)m_pose.size();i++)
    {
         for(int j=0;j<(int)m_pose[i].size();j++)
            m_pose[i][j].clear();
         m_pose[i].clear();
    }
    m_pose.clear();

    m_image.clear();
    m_sons.clear();
}

Modele_Personnage::~Modele_Personnage()
{
    for(int i=0;i<(int)m_pose.size();i++)
    {
         for(int j=0;j<(int)m_pose[i].size();j++)
            m_pose[i][j].clear();
         m_pose[i].clear();
    }
    m_pose.clear();

    m_image.clear();
    m_sons.clear();
}


bool Modele_Personnage::Charger(string chemin)
{
    for(int i=0;i<(int)m_pose.size();i++)
    {
         for(int j=0;j<(int)m_pose[i].size();j++)
            m_pose[i][j].clear();
         m_pose[i].clear();
    }
    m_pose.clear();

    m_image.clear();
    m_sons.clear();

    console->Ajouter("",0);
	console->Ajouter("Chargement du personnage : "+chemin,0);

	cDAT reader;

    reader.Read(chemin);

    ifstream *fichier;
    fichier = reader.GetInfos("infos.txt");
    if(fichier)
    {
    	char caractere;
    	do
    	{
    		fichier->get(caractere);
    		if(caractere=='*')
    		{
    			string cheminImage;
    			*fichier>>cheminImage;

                m_image.push_back(moteurGraphique->AjouterImage(reader.GetFile(cheminImage), reader.GetFileSize(cheminImage), cheminImage));
    		}
    		if(fichier->eof()){ char temp[1000]; sprintf(temp,"Erreur : Personnage \" %s \" Invalide",chemin.c_str());console->Ajouter(temp,1); caractere='$'; }
    	}while(caractere!='$');

    	m_sons.clear();
    	do
    	{
    		fichier->get(caractere);
    		if(caractere=='*')
    		{
    			string cheminSon;
    			*fichier>>cheminSon;
                m_sons.push_back(moteurSons->AjouterBuffer(cheminSon));
    		}
    		if(fichier->eof()){ char temp[1000]; sprintf(temp,"Erreur : Personnage \" %s \" Invalide",chemin.c_str());console->Ajouter(temp,1); caractere='$'; }
    	}while(caractere!='$');


    	do
    	{
    		fichier->get(caractere);
    		if(caractere=='*')
            {
                do
                {
                    fichier->get(caractere);
                    switch (caractere)
                    {
                        case 'r' : *fichier>>m_porteeLumineuse.rouge; break;
                        case 'v' : *fichier>>m_porteeLumineuse.vert; break;
                        case 'b' : *fichier>>m_porteeLumineuse.bleu; break;
                        case 'i' : *fichier>>m_porteeLumineuse.intensite; break;
                    }

                    if(fichier->eof()){ char temp[255]; sprintf(temp,"Erreur : Objet \" %s \" Invalide",chemin.c_str());console->Ajouter(temp,1); caractere='$'; }

                }while(caractere!='$');
                fichier->get(caractere);
            }
    		if(fichier->eof()){ char temp[255]; sprintf(temp,"Erreur : Objet \" %s \" Invalide",chemin.c_str());console->Ajouter(temp,1); caractere='$'; }

    	}while(caractere!='$');

    	 Pose poseTemp;
    	 m_pose.resize(NOMBRE_ETAT,vector<vector<Pose> >(0,vector<Pose>(0,poseTemp)));

    	for(int i=0;i<NOMBRE_ETAT;i++)
    	{
    	    for(int j=0;j<8;j++)
    	    {
    	        m_pose[i].push_back(vector<Pose> (0,poseTemp));
                do
                {
                    if(caractere=='*')
                    {
                        coordonnee position,centre={-1000,-1000,-1,-1};
                        int animation=0,son=-1,image=0,attaque=-1,lumiere=m_porteeLumineuse.intensite,ordre=0;
                        float tempsAnimation=0.075;

                        do
                        {
                            fichier->get(caractere);
                            switch (caractere)
                            {
                                case 'x': *fichier>>position.x; break;
                                case 'y': *fichier>>position.y; break;
                                case 'w': *fichier>>position.w; break;
                                case 'h': *fichier>>position.h; break;
                                case 'a': *fichier>>animation; break;
                                case 't': *fichier>>tempsAnimation; break;
                                case 's': *fichier>>son; break;
                                case 'i': *fichier>>image; break;
                                case 'd': *fichier>>attaque; break;
                                case 'l': *fichier>>lumiere; break;

                                case 'o': *fichier>>ordre; break;

                                case 'c': fichier->get(caractere); if(caractere=='x') *fichier>>centre.x; else *fichier>>centre.y; break;
                            }
                            if(fichier->eof()){ char temp[1000]; sprintf(temp,"Erreur : Monstre \" %s \" Invalide",chemin.c_str());console->Ajouter(temp,1); caractere='$'; m_caracteristique.maxVie=0;}
                        }while(caractere!='$');

                        if(centre.x==-1000)
                            centre.x=position.w/2;
                        if(centre.y==-1000)
                            centre.y=position.h-32;

                        m_pose[i][j].push_back(poseTemp);
                        m_pose[i][j].back().setPose(position,centre,animation,son,image,attaque,lumiere,tempsAnimation,ordre);
                        fichier->get(caractere);
                        if(fichier->eof()){ char temp[1000]; sprintf(temp,"Erreur : Monstre \" %s \" Invalide",chemin.c_str());console->Ajouter(temp,1); caractere='$'; m_caracteristique.maxVie=0;  }
                    }
                    fichier->get(caractere);
                    if(fichier->eof()){ char temp[1000]; sprintf(temp,"Erreur : Monstre \" %s \" Invalide",chemin.c_str());console->Ajouter(temp,1); caractere='$'; m_caracteristique.maxVie=0; }
                }while(caractere!='$');
    	    }
    	}
    	fichier->close();
    }
    else
    {
        console->Ajouter("Impossible d'ouvrir : "+chemin,1);

        return 0;
    }


    delete fichier;

    return true;
}

int Personnage::getOrdre(Modele_Personnage *modele)
{
    if(modele->m_pose.size()>0)
        if(m_etat>=0&&m_etat<NOMBRE_ETAT)
            if((int)(m_angle/45)>=0&&(int)(m_angle/45)<(int)modele->m_pose[m_etat].size())
                if(m_poseEnCours>=0&&m_poseEnCours<(int)modele->m_pose[m_etat][(int)(m_angle/45)].size())
                     return modele->m_pose[m_etat][(int)(m_angle/45)][m_poseEnCours].getOrdre();

    return -10;
}

void Personnage::Afficher(coordonnee dimensionsMap,Modele_Personnage *modele,bool surbrillance)
{
    if(modele->m_pose.size()>0)
    if((int)(m_angle/45)>=0&&(int)(m_angle/45)<8)
    {
        Sprite sprite;

        if(configuration->Ombre&&modele->m_ombre)
        {
                int angleOmbre=(int)((m_angle-moteurGraphique->m_angleOmbreSoleil)+22.5);

                while(angleOmbre<0)
                    angleOmbre=360+angleOmbre;
                while(angleOmbre>=360)
                    angleOmbre=angleOmbre-360;

                if((int)(angleOmbre/45)>=0&&(int)(angleOmbre/45)<(int)modele->m_pose[m_etat].size())
                    if(m_poseEnCours>=0&&m_poseEnCours<(int)modele->m_pose[m_etat][(int)(angleOmbre/45)].size())
                    {
                        sprite.SetImage(*moteurGraphique->getImage(modele->m_image[modele->m_pose[m_etat][(int)(angleOmbre/45)][m_poseEnCours].getImage()]));
                        sprite.SetSubRect(IntRect(modele->m_pose[m_etat][(int)(angleOmbre/45)][m_poseEnCours].getCoordonnee().x, modele->m_pose[m_etat][(int)(angleOmbre/45)][m_poseEnCours].getCoordonnee().y, modele->m_pose[m_etat][(int)(angleOmbre/45)][m_poseEnCours].getCoordonnee().x+modele->m_pose[m_etat][(int)(angleOmbre/45)][m_poseEnCours].getCoordonnee().w, modele->m_pose[m_etat][(int)(angleOmbre/45)][m_poseEnCours].getCoordonnee().y+modele->m_pose[m_etat][(int)(angleOmbre/45)][m_poseEnCours].getCoordonnee().h));

                        sprite.SetScale(m_caracteristique.modificateurTaille,m_caracteristique.modificateurTaille);

                        sprite.SetCenter(modele->m_pose[m_etat][(int)(angleOmbre/45)][m_poseEnCours].getCentre().x,modele->m_pose[m_etat][(int)(angleOmbre/45)][m_poseEnCours].getCentre().y);

                        sprite.SetX(((m_positionPixel.x-m_positionPixel.y)*64/COTE_TILE/*+dimensionsMap.y*64*/));
                        sprite.SetY(((m_positionPixel.x+m_positionPixel.y)*64/COTE_TILE)/2+32 -m_positionPixel.h);

                        sprite.SetScale(m_caracteristique.modificateurTaille, m_caracteristique.modificateurTaille*(100-(float)moteurGraphique->m_soleil.hauteur)/50);
                        sprite.SetRotation(moteurGraphique->m_angleOmbreSoleil);
                        moteurGraphique->AjouterCommande(&sprite,9,1);
                        sprite.SetCenter(0,0);
                        sprite.SetScale(1, 1);
                        sprite.SetRotation(0);
                    }
        }

        if((int)(m_angle/45)>=0&&(int)(m_angle/45)<(int)modele->m_pose[m_etat].size())
            if(m_poseEnCours>=0&&m_poseEnCours<(int)modele->m_pose[m_etat][(int)(m_angle/45)].size())
            {
                sprite.SetImage(*moteurGraphique->getImage(modele->m_image[modele->m_pose[m_etat][(int)(m_angle/45)][m_poseEnCours].getImage()]));

                sprite.SetCenter(modele->m_pose[m_etat][(int)(m_angle/45)][m_poseEnCours].getCentre().x,modele->m_pose[m_etat][(int)(m_angle/45)][m_poseEnCours].getCentre().y);

                sprite.SetSubRect(IntRect(modele->m_pose[m_etat][(int)(m_angle/45)][m_poseEnCours].getCoordonnee().x, modele->m_pose[m_etat][(int)(m_angle/45)][m_poseEnCours].getCoordonnee().y, modele->m_pose[m_etat][(int)(m_angle/45)][m_poseEnCours].getCoordonnee().x+modele->m_pose[m_etat][(int)(m_angle/45)][m_poseEnCours].getCoordonnee().w, modele->m_pose[m_etat][(int)(m_angle/45)][m_poseEnCours].getCoordonnee().y+modele->m_pose[m_etat][(int)(m_angle/45)][m_poseEnCours].getCoordonnee().h));

                sprite.FlipX(false);


                sprite.SetX(((m_positionPixel.x-m_positionPixel.y)*64/COTE_TILE/*+dimensionsMap.y*64*/));
                sprite.SetY(((m_positionPixel.x+m_positionPixel.y)*32/COTE_TILE)+32 -m_positionPixel.h);


                sprite.SetScale(m_caracteristique.modificateurTaille,m_caracteristique.modificateurTaille);


                 if(m_porteeLumineuse.intensite>0)
                    sprite.SetColor(sf::Color(m_porteeLumineuse.rouge,m_porteeLumineuse.vert,m_porteeLumineuse.bleu, 255));
                else
                    sprite.SetColor(sf::Color(255,255,255, 255));

                if(sprite.GetPosition().x+sprite.GetSize().x>=moteurGraphique->m_camera.GetRect().Left
                &&sprite.GetPosition().x-modele->m_pose[m_etat][(int)(m_angle/45)][m_poseEnCours].getCentre().x<moteurGraphique->m_camera.GetRect().Right
                &&sprite.GetPosition().y+sprite.GetSize().y>=moteurGraphique->m_camera.GetRect().Top
                &&sprite.GetPosition().y<moteurGraphique->m_camera.GetRect().Bottom)
                {
                    moteurGraphique->AjouterCommande(&sprite,10,1);
                    if(surbrillance)
                    {
                        sprite.SetBlendMode(sf::Blend::Add);
                        moteurGraphique->AjouterCommande(&sprite,10,1);
                    }
                }
            }
    }
}
void Personnage::regenererVie(float vie)
{
    m_caracteristique.vie+=vie;
    if(m_caracteristique.vie>m_caracteristique.maxVie)
        m_caracteristique.vie=m_caracteristique.maxVie;
}

int Personnage::pathfinding(casePathfinding** map,coordonnee exception)
{


    //if(!(m_arrivee.x==m_mauvaiseArrivee.x&&m_arrivee.y==m_mauvaiseArrivee.y))
        if(!(m_arrivee.x==m_positionCase.x&&m_arrivee.y==m_positionCase.y))
        {
            m_erreurPathfinding=false;
            liste_case casesVisitee;
            coordonnee depart,arrivee,decalage;

            decalage.x=m_positionCase.x-10;
            decalage.y=m_positionCase.y-10;

            depart.x=m_positionCase.x-decalage.x;
            depart.y=m_positionCase.y-decalage.y;
            arrivee.x=m_arrivee.x-decalage.x;
            arrivee.y=m_arrivee.y-decalage.y;
            exception.x=exception.x-decalage.x;
            exception.y=exception.y-decalage.y;

            if(exception.x>=0&&exception.x<20&&exception.y>=0&&exception.y<20)
                map[exception.y][exception.x].collision=1;

            casesVisitee.setTailleListe(0);

            casesVisitee.ajouterCase(depart);

            if(arrivee.y>=0&&arrivee.x>=0&&arrivee.y<20&&arrivee.x<20)
                if(map[arrivee.y][arrivee.x].collision)
                {
                    coordonnee temp={-100,-100,-1,-1},enCours;

                    enCours.y=arrivee.y-1;
                    enCours.x=arrivee.x-1;
                    if(enCours.y>=0&&enCours.y<20&&enCours.x>=0&&enCours.x<20)
                        if(!map[enCours.y][enCours.x].collision&&!(enCours.x==depart.x&&enCours.y==depart.y))
                            if(sqrt((enCours.y-depart.y)*(enCours.y-depart.y)+(enCours.x-depart.x)*(enCours.x-depart.x)) < sqrt((temp.y-depart.y)*(temp.y-depart.y)+(temp.x-depart.x)*(temp.x-depart.x)))
                                temp.y=enCours.y,temp.x=enCours.x;
                    enCours.y=arrivee.y;
                    enCours.x=arrivee.x-1;
                    if(enCours.y>=0&&enCours.y<20&&enCours.x>=0&&enCours.x<20)
                        if(!map[enCours.y][enCours.x].collision&&!(enCours.x==depart.x&&enCours.y==depart.y))
                            if(sqrt((enCours.y-depart.y)*(enCours.y-depart.y)+(enCours.x-depart.x)*(enCours.x-depart.x)) < sqrt((temp.y-depart.y)*(temp.y-depart.y)+(temp.x-depart.x)*(temp.x-depart.x)))
                                temp.y=enCours.y,temp.x=enCours.x;
                    enCours.y=arrivee.y-1;
                    enCours.x=arrivee.x;
                    if(enCours.y>=0&&enCours.y<20&&enCours.x>=0&&enCours.x<20)
                        if(!map[enCours.y][enCours.x].collision&&!(enCours.x==depart.x&&enCours.y==depart.y))
                            if(sqrt((enCours.y-depart.y)*(enCours.y-depart.y)+(enCours.x-depart.x)*(enCours.x-depart.x)) < sqrt((temp.y-depart.y)*(temp.y-depart.y)+(temp.x-depart.x)*(temp.x-depart.x)))
                                temp.y=enCours.y,temp.x=enCours.x;

                    enCours.y=arrivee.y+1;
                    enCours.x=arrivee.x+1;
                    if(enCours.y>=0&&enCours.y<20&&enCours.x>=0&&enCours.x<20)
                        if(!map[enCours.y][enCours.x].collision&&!(enCours.x==depart.x&&enCours.y==depart.y))
                            if(sqrt((enCours.y-depart.y)*(enCours.y-depart.y)+(enCours.x-depart.x)*(enCours.x-depart.x)) < sqrt((temp.y-depart.y)*(temp.y-depart.y)+(temp.x-depart.x)*(temp.x-depart.x)))
                                temp.y=enCours.y,temp.x=enCours.x;
                    enCours.y=arrivee.y+1;
                    enCours.x=arrivee.x;
                    if(enCours.y>=0&&enCours.y<20&&enCours.x>=0&&enCours.x<20)
                        if(!map[enCours.y][enCours.x].collision&&!(enCours.x==depart.x&&enCours.y==depart.y))
                            if(sqrt((enCours.y-depart.y)*(enCours.y-depart.y)+(enCours.x-depart.x)*(enCours.x-depart.x)) < sqrt((temp.y-depart.y)*(temp.y-depart.y)+(temp.x-depart.x)*(temp.x-depart.x)))
                                temp.y=enCours.y,temp.x=enCours.x;
                    enCours.y=arrivee.y;
                    enCours.x=arrivee.x+1;
                    if(enCours.y>=0&&enCours.y<20&&enCours.x>=0&&enCours.x<20)
                        if(!map[enCours.y][enCours.x].collision&&!(enCours.x==depart.x&&enCours.y==depart.y))
                            if(sqrt((enCours.y-depart.y)*(enCours.y-depart.y)+(enCours.x-depart.x)*(enCours.x-depart.x)) < sqrt((temp.y-depart.y)*(temp.y-depart.y)+(temp.x-depart.x)*(temp.x-depart.x)))
                                temp.y=enCours.y,temp.x=enCours.x;


                    enCours.y=arrivee.y+1;
                    enCours.x=arrivee.x-1;
                    if(enCours.y>=0&&enCours.y<20&&enCours.x>=0&&enCours.x<20)
                        if(!map[enCours.y][enCours.x].collision&&!(enCours.x==depart.x&&enCours.y==depart.y))
                            if(sqrt((enCours.y-depart.y)*(enCours.y-depart.y)+(enCours.x-depart.x)*(enCours.x-depart.x)) < sqrt((temp.y-depart.y)*(temp.y-depart.y)+(temp.x-depart.x)*(temp.x-depart.x)))
                                temp.y=enCours.y,temp.x=enCours.x;
                    enCours.y=arrivee.y-1;
                    enCours.x=arrivee.x+1;
                    if(enCours.y>=0&&enCours.y<20&&enCours.x>=0&&enCours.x<20)
                        if(!map[enCours.y][enCours.x].collision&&!(enCours.x==depart.x&&enCours.y==depart.y))
                            if(sqrt((enCours.y-depart.y)*(enCours.y-depart.y)+(enCours.x-depart.x)*(enCours.x-depart.x)) < sqrt((temp.y-depart.y)*(temp.y-depart.y)+(temp.x-depart.x)*(temp.x-depart.x)))
                                temp.y=enCours.y,temp.x=enCours.x;

                    if(temp.y!=-100&&temp.x!=-100)
                        arrivee=temp;
                    else
                        m_erreurPathfinding=true;
                }

            while(!casesVisitee.testerCasesEnCours(arrivee)&&!m_erreurPathfinding)
            {
                casesVisitee.incrementerDistanceEnCours();
                casesVisitee.ajouterCasesAdjacentes(map,&arrivee,depart);
                if(casesVisitee.getDistance()>10)
                    m_erreurPathfinding=true;
            }

            if(!m_erreurPathfinding)
            {
                m_cheminFinal=arrivee;

                while(casesVisitee.getDistance()>1)
                {
                    m_cheminFinal=casesVisitee.trouverLeChemin(m_cheminFinal);
                    casesVisitee.decrementerDistanceEnCours();
                }

                m_arrivee.x=arrivee.x+decalage.x;
                m_arrivee.y=arrivee.y+decalage.y;

                m_cheminFinal.h=map[m_cheminFinal.y][m_cheminFinal.x].hauteur;

                m_cheminFinal.x+=decalage.x,m_cheminFinal.y+=decalage.y;

                m_ancienneArrivee=m_arrivee;
            }
            else
            {
                m_arrivee=m_positionCase;
                m_cheminFinal=m_positionCase;
                if(m_etat!=0)
                    m_etat=0,m_poseEnCours=0,frappeEnCours=0;
            }

            for(int i=0;i<20;i++)
                delete[] map[i];
            delete[] map;

            if(m_etat==0)
                return 0;
            else
                return 1;
        }

        for(int i=0;i<20;i++)
                delete[] map[i];
            delete[] map;

    return 2;
}


bool Personnage::seDeplacer(float tempsEcoule,coordonnee dimensionsMap)
{
    int buf=(int)(tempsEcoule*1000);
    tempsEcoule=(float)buf/1000;

    if(m_caracteristique.vie>0)
    {
        if(!frappeEnCours)
        {
            if(m_positionCase.y!=m_cheminFinal.y||m_positionCase.x!=m_cheminFinal.x)
            {
                if(m_etat!=1)
                    m_etat=1,m_poseEnCours=0;

                m_positionPixelPrecedente.x=(int)m_positionPixel.x;
                m_positionPixelPrecedente.y=(int)m_positionPixel.y;

                if(m_positionCase.x<m_cheminFinal.x)
                {
                    if(m_positionCase.h!=m_cheminFinal.h && m_positionPixel.h!=m_cheminFinal.h)
                        m_positionPixel.h=m_positionCase.h*( COTE_TILE - fabs(m_positionPixel.x-m_positionCase.x*COTE_TILE))/COTE_TILE + m_cheminFinal.h*( fabs(m_positionPixel.x-m_positionCase.x*COTE_TILE))/COTE_TILE ;
                    if(m_positionCase.y>m_cheminFinal.y)
                        m_positionPixel.x+=(float)2*tempsEcoule*m_caracteristique.vitesse;
                    else
                        m_positionPixel.x+=(float)4*tempsEcoule*m_caracteristique.vitesse;
                }
                else if(m_positionCase.x>m_cheminFinal.x)
                {
                    if(m_positionCase.h!=m_cheminFinal.h && m_positionPixel.h!=m_cheminFinal.h)
                        m_positionPixel.h=m_positionCase.h*( COTE_TILE - fabs(m_positionPixel.x-m_positionCase.x*COTE_TILE))/COTE_TILE + m_cheminFinal.h*( fabs(m_positionPixel.x-m_positionCase.x*COTE_TILE))/COTE_TILE ;
                    if(m_positionCase.y<m_cheminFinal.y)
                        m_positionPixel.x-=(float)2*tempsEcoule*m_caracteristique.vitesse;
                    else
                        m_positionPixel.x-=(float)4*tempsEcoule*m_caracteristique.vitesse;
                }
                if(m_positionCase.y<m_cheminFinal.y)
                {
                    if(m_positionCase.h!=m_cheminFinal.h && m_positionPixel.h!=m_cheminFinal.h)
                        m_positionPixel.h=m_positionCase.h*( COTE_TILE - fabs(m_positionPixel.y-m_positionCase.y*COTE_TILE))/COTE_TILE + m_cheminFinal.h*( fabs(m_positionPixel.y-m_positionCase.y*COTE_TILE))/COTE_TILE ;
                    if(m_positionCase.x>m_cheminFinal.x)
                        m_positionPixel.y+=(float)2*tempsEcoule*m_caracteristique.vitesse;
                    else
                        m_positionPixel.y+=(float)4*tempsEcoule*m_caracteristique.vitesse;
                }
                else if(m_positionCase.y>m_cheminFinal.y)
                {
                    if(m_positionCase.h!=m_cheminFinal.h && m_positionPixel.h!=m_cheminFinal.h)
                        m_positionPixel.h=m_positionCase.h*( COTE_TILE - fabs(m_positionPixel.y-m_positionCase.y*COTE_TILE))/COTE_TILE + m_cheminFinal.h*( fabs(m_positionPixel.y-m_positionCase.y*COTE_TILE))/COTE_TILE ;
                    if(m_positionCase.x<m_cheminFinal.x)
                        m_positionPixel.y-=(float)2*tempsEcoule*m_caracteristique.vitesse;
                    else
                        m_positionPixel.y-=(float)4*tempsEcoule*m_caracteristique.vitesse;
                }

                sf::Vector2f pos;
                pos.x=((m_positionPixel.x-m_positionPixel.y)*64/COTE_TILE);
                pos.y=((m_positionPixel.x+m_positionPixel.y)*64/COTE_TILE)+64;

                moteurGraphique->LightManager->SetPosition(m_light,pos);

                m_angle=calculerAngle(m_cheminFinal.x-m_positionCase.x,m_cheminFinal.y-m_positionCase.y);

                if((m_positionCase.x<m_cheminFinal.x&&m_positionPixel.x>=m_cheminFinal.x*COTE_TILE)
                 ||(m_positionCase.x>m_cheminFinal.x&&m_positionPixel.x<=m_cheminFinal.x*COTE_TILE)
                  ||m_positionCase.x==m_cheminFinal.x)
                if((m_positionCase.y<m_cheminFinal.y&&m_positionPixel.y>=m_cheminFinal.y*COTE_TILE)
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
            else if(m_arrivee.x!=m_positionCase.x||m_arrivee.y!=m_positionCase.y)
                return 1;
            else
            {
                if(m_etat!=0)
                    m_poseEnCours=0;

                m_etat=0,frappeEnCours=0;
            }
        }
        if(m_etat==2||m_arrivee.x==m_positionCase.x&&m_arrivee.y==m_positionCase.y)
            return 1;
    }

    return 0;
}

void Personnage::infligerDegats(float degats)
{
    m_caracteristique.vie-=degats;
    if(m_caracteristique.vie<=0&&m_etat!=3)
        m_poseEnCours=0,m_etat=3;
}

int Personnage::animer(Modele_Personnage *modele,float temps,bool *explosif,coordonnee positionHero)
{
    int retour=-2;

    m_animation+=temps;

    float tempsAnimation=0.075;

    if(modele->m_pose.size()>0)
        tempsAnimation = modele->m_pose[m_etat][(int)(m_angle/45)][m_poseEnCours].getTempsAnimation();

    if(modele->m_pose.size()>0&&tempsAnimation>0)
    while(m_animation>=tempsAnimation)
    {
        coordonnee position;
        position.x=(m_positionCase.x-m_positionCase.y-1)/5;
        position.y=(m_positionCase.x+m_positionCase.y)/5;

        m_poseEnCours=modele->m_pose[m_etat][(int)(m_angle/45)][m_poseEnCours].getAnimation();

        if(m_poseEnCours>=(int)modele->m_pose[m_etat][(int)(m_angle/45)].size())
        m_poseEnCours=0;

        //if(modele->m_pose[m_etat][(int)(m_angle/45)][m_poseEnCours].getSon()>=0)
        modele->jouerSon(modele->m_pose[m_etat][(int)(m_angle/45)][m_poseEnCours].getSon(),position,positionHero);

        m_animation-=tempsAnimation;
        tempsAnimation = modele->m_pose[m_etat][(int)(m_angle/45)][m_poseEnCours].getTempsAnimation();

        if(m_monstre)
        {
            if(modele->m_pose[m_etat][(int)(m_angle/45)][m_poseEnCours].getAttaque()==0)
                retour+=(rand()%(m_caracteristique.degatsMax-m_caracteristique.degatsMin+1)+m_caracteristique.degatsMin);
        }
        else
        {
            //if(modele->m_pose[m_etat][(int)(m_angle/45)][m_poseEnCours].getAttaque()==0)
            if(modele->m_pose[m_etat][(int)(m_angle/45)][m_poseEnCours].getAttaque()>retour)
                retour=modele->m_pose[m_etat][(int)(m_angle/45)][m_poseEnCours].getAttaque();
        }

        if(modele->m_pose[m_etat][(int)(m_angle/45)][m_poseEnCours].getAttaque()==1)
            frappeEnCours=false;

        *explosif=modele->m_explosif;

        if(m_monstre)
        {
            if(modele->m_pose[m_etat][(int)(m_angle/45)][m_poseEnCours].getLumiereIntensite()!=-1)
                m_porteeLumineuse.intensite=modele->m_pose[m_etat][(int)(m_angle/45)][m_poseEnCours].getLumiereIntensite();
            else if(m_porteeLumineuseBasique.intensite>=0)
                m_porteeLumineuse=m_porteeLumineuseBasique;

            float inte=m_porteeLumineuse.intensite;
            if(inte>255)
                inte=255;

            moteurGraphique->LightManager->SetIntensity(m_light,(int)inte);
            moteurGraphique->LightManager->SetRadius(m_light,(int)m_porteeLumineuse.intensite*2);
        }
    }
    return retour;
}

void Personnage::frappe(coordonnee direction,coordonnee position)
{

    if(!frappeEnCours)
    {
        m_etat=2;
        m_poseEnCours=0;
    }

    frappeEnCours=1;

    float m=atan2((double)(direction.x-position.x),(double)(direction.y-position.y));
    m+=M_PI/3;


    m_angle=(int)(m*180/M_PI);
    if(m_angle>=360)
        m_angle=0;
    if(m_angle<0)
        m_angle=360+m_angle;

    m_cheminFinal=m_positionCase;
    m_arrivee=m_cheminFinal;

}


void Modele_Personnage::jouerSon(int numeroSon,coordonnee position,coordonnee positionHero,bool uniqueSound)
{
    if(numeroSon>=0&&numeroSon<(int)m_sons.size())
    {
        coordonnee pos;
        pos.x=-position.x;
        pos.y=position.y;
        moteurSons->JouerSon(m_sons[numeroSon],pos,positionHero,uniqueSound);
    }
}



void Modele_Personnage::setPorteeLumineuse(Lumiere  lumiere){m_porteeLumineuse=lumiere;}
void Personnage::setPorteeLumineuse(Lumiere  lumiere){m_porteeLumineuse=lumiere;}
void Personnage::setCaracteristique(Caracteristique caracteristique){m_caracteristique=caracteristique;}
void Personnage::setProchaineCase(coordonnee position){m_cheminFinal=position;}
void Personnage::setVitesse(float vitesse){m_caracteristique.vitesse=vitesse;}
void Personnage::setEtat(int etat){m_etat=etat,m_poseEnCours=0,frappeEnCours=false;}
void Personnage::setPose(int pose){m_poseEnCours=pose;}
void Personnage::setAngle(int angle){m_angle=angle;}
void Personnage::setErreurPathfinding(bool erreur){m_erreurPathfinding=erreur;}
void Personnage::setCoordonnee(coordonnee nouvellesCoordonnees)
{
	m_positionCase=nouvellesCoordonnees;
    m_etat=ARRET;

	m_positionPixel.x=(int)(m_positionCase.x*sqrt(64*64+32*32));
	m_positionPixel.y=(int)(m_positionCase.y*sqrt(64*64+32*32));

	m_arrivee.x=m_positionCase.x;
	m_arrivee.y=m_positionCase.y;

	m_cheminFinal.x=m_positionCase.x;
	m_cheminFinal.y=m_positionCase.y;

	m_angle=0;
	m_poseEnCours=0;

	moteurGraphique->LightManager->SetPosition(m_light,sf::Vector2f(m_positionPixel.x,m_positionPixel.y));
}
void Personnage::setArrivee(coordonnee arrivee)
{
    if(!(m_mauvaiseArrivee.x==arrivee.x&&m_mauvaiseArrivee.y==arrivee.y))
    {
        m_arrivee=arrivee;
    }
}
void Personnage::setMauvaiseArrivee(coordonnee arrivee){m_mauvaiseArrivee=arrivee;}

void Personnage::setCoordonneePixel(coordonnee position)
{
    m_positionPixel.x=position.x*COTE_TILE;
    m_positionPixel.y=position.y*COTE_TILE;
}

void Personnage::PousserCase(coordonnee vecteur)
{
    m_positionCase.x+=vecteur.x;
    m_positionCase.y+=vecteur.y;

    m_positionPixel.x=(int)(m_positionCase.x*sqrt(64*64+32*32));
    m_positionPixel.y=(int)(m_positionCase.y*sqrt(64*64+32*32));

    m_arrivee.x=m_positionCase.x;
	m_arrivee.y=m_positionCase.y;

	m_cheminFinal.x=m_positionCase.x;
	m_cheminFinal.y=m_positionCase.y;
}

void Personnage::Pousser(coordonnee vecteur)
{
    m_positionPixel.x+=vecteur.x;
    m_positionPixel.y+=vecteur.y;

    m_positionCase.x=(int)(m_positionPixel.x/sqrt(64*64+32*32));
    m_positionCase.y=(int)(m_positionPixel.y/sqrt(64*64+32*32));

    m_arrivee.x=m_positionCase.x;
	m_arrivee.y=m_positionCase.y;

	m_cheminFinal.x=m_positionCase.x;
	m_cheminFinal.y=m_positionCase.y;
}

void Personnage::AjouterPointAme(int pointAme) { m_caracteristique.pointAme+=pointAme; }

bool Personnage::enVie() { if(m_caracteristique.vie>0) return 1; else return 0; }

int Modele_Personnage::getNombreSons(){return m_sons.size();}
coordonnee Personnage::getCoordonnee(){return m_positionCase;}
coordonnee Personnage::getArrivee(){return m_arrivee;}

Caracteristique Personnage::getCaracteristique(){return m_caracteristique;}
Caracteristique Modele_Personnage::getCaracteristique(){return m_caracteristique;}
Lumiere Modele_Personnage::getPorteeLumineuse(){return m_porteeLumineuse;}
Lumiere Personnage::getPorteeLumineuse(){return m_porteeLumineuse;}
int Personnage::getEtat(){return m_etat;}
int Personnage::getAngle(){return m_angle;}
int Personnage::getPose(){return m_poseEnCours;}
bool Personnage::getErreurPathfinding(){return m_erreurPathfinding;}
coordonnee Personnage::getCoordonneePixel()
{
    coordonnee position;
    position.x=(int)m_positionPixel.x;
    position.y=(int)m_positionPixel.y;
    position.h=(int)m_positionPixel.h;
	return position;
}
coordonnee Personnage::getProchaineCase()
{
    if(m_cheminFinal.x!=m_positionCase.x||m_cheminFinal.y!=m_positionCase.y)
        return m_cheminFinal;
	else
        return m_positionCase;
}
