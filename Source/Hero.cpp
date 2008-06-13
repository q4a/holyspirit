#ifndef HEROCPP
#define HEROCPP

#include "Hero.h"
#include "Globale.h"

#include <iostream>
#include <fstream>

#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include <math.h>

using namespace std;
using namespace sf;

Hero::Hero()
{
   ///Je donnes des valeur à mes variables juste pour les tests
	m_personnage.setEtat(ARRET);

	coordonnee position;
	position.x=0;
	position.y=0;

	m_personnage.setCoordonnee(position);

	Lumiere lumiere;
	lumiere.intensite=128;
	lumiere.rouge=236;
	lumiere.vert=136;
	lumiere.bleu=36;
	lumiere.hauteur=20;

	m_modelePersonnage.setPorteeLumineuse(lumiere);

	m_personnage.setVitesse(1);
}

/*void setArrivee(coordonnee casePointee,Map *map)
{
    if(map->getTypeCase(casePointee.x,casePointee.y)==2)
        m_monstreVise=map->getMonstre(casePointee);
    else
        m_monstreVise=-1;
    m_personnage.setArrivee(casePointee);
}*/

void Hero::placerCamera(sf::View *camera,coordonnee dimensionsMap)
{
	m_positionAffichage.y=(int)(((m_personnage.getCoordonneePixel().x+m_personnage.getCoordonneePixel().y)*64/COTE_TILE)/2);
	m_positionAffichage.x=(int)(((m_personnage.getCoordonneePixel().x-m_personnage.getCoordonneePixel().y)*64/COTE_TILE+dimensionsMap.y*64)-64);

	coordonnee positionCamera;
	positionCamera.y=m_positionAffichage.y-250*configuration.Resolution.y/600;
	positionCamera.x=m_positionAffichage.x-((400*configuration.Resolution.x/800))+64;
	camera->Rect.Top=positionCamera.y;
	camera->Rect.Left=positionCamera.x;
	camera->Rect.Bottom=positionCamera.y+configuration.Resolution.y;
	camera->Rect.Right=positionCamera.x+configuration.Resolution.x;
}

void Hero::testMontreVise(Monstre *monstre,int hauteurMap)
{
    if(m_enemiVise>-1)
    {

                            if(fabs(m_personnage.getCoordonnee().x-monstre->getCoordonnee().x)>1||fabs(m_personnage.getCoordonnee().y-monstre->getCoordonnee().y)>1)
                            {
                                m_personnage.setArrivee(monstre->getCoordonnee());
                                /*coordonnee arrivee,enCours;

                                arrivee=m_personnage.getCoordonnee();

                                if(fabs(m_personnage.getCoordonnee().x-monstre->getCoordonnee().x)!=0&&fabs(monstre->getCoordonnee().x-m_personnage.getCoordonnee().y!=0))
                                {
                                    enCours.x=arrivee.x-(m_personnage.getCoordonnee().x-monstre->getCoordonnee().x)/fabs(m_personnage.getCoordonnee().x-monstre->getCoordonnee().x);
                                    enCours.y=arrivee.y-(m_personnage.getCoordonnee().y-monstre->getCoordonnee().y)/fabs(m_personnage.getCoordonnee().y-monstre->getCoordonnee().y);
                                    //if(!getCollision(enCours.x,enCours.y))
                                        m_personnage.setArrivee(enCours);
                                }

                                if(fabs(m_personnage.getCoordonnee().x-monstre->getCoordonnee().x)!=0)
                                {
                                    enCours.x=arrivee.x-(m_personnage.getCoordonnee().x-monstre->getCoordonnee().x)/fabs(m_personnage.getCoordonnee().x-monstre->getCoordonnee().x);
                                    enCours.y=arrivee.y;
                                   // if(!getCollision(enCours.x,enCours.y))
                                        m_personnage.setArrivee(enCours);
                                }

                                if(fabs(monstre->getCoordonnee().x-m_personnage.getCoordonnee().y!=0))
                                {
                                    enCours.x=arrivee.x;
                                    enCours.y=arrivee.y-(m_personnage.getCoordonnee().y-monstre->getCoordonnee().y)/fabs(m_personnage.getCoordonnee().y-monstre->getCoordonnee().y);
                                    //if(!getCollision(enCours.x,enCours.y))
                                        m_personnage.setArrivee(enCours);
                                }

                                if(fabs(m_personnage.getCoordonnee().x-monstre->getCoordonnee().x)!=0&&fabs(monstre->getCoordonnee().x-m_personnage.getCoordonnee().y!=0))
                                {
                                    enCours.x=arrivee.x+(m_personnage.getCoordonnee().x-monstre->getCoordonnee().x)/fabs(m_personnage.getCoordonnee().x-monstre->getCoordonnee().x);
                                    enCours.y=arrivee.y-(m_personnage.getCoordonnee().y-monstre->getCoordonnee().y)/fabs(m_personnage.getCoordonnee().y-monstre->getCoordonnee().y);
                                   // if(!getCollision(enCours.x,enCours.y))
                                        m_personnage.setArrivee(enCours);
                                    enCours.x=arrivee.x-(m_personnage.getCoordonnee().x-monstre->getCoordonnee().x)/fabs(m_personnage.getCoordonnee().x-monstre->getCoordonnee().x);
                                    enCours.y=arrivee.y+(m_personnage.getCoordonnee().y-monstre->getCoordonnee().y)/fabs(m_personnage.getCoordonnee().y-monstre->getCoordonnee().y);
                                   // if(!getCollision(enCours.x,enCours.y))
                                        m_personnage.setArrivee(enCours);
                                }

                                if(fabs(monstre->getCoordonnee().x-m_personnage.getCoordonnee().y!=0))
                                {
                                    enCours.x=arrivee.x;
                                    enCours.y=arrivee.y+(m_personnage.getCoordonnee().y-monstre->getCoordonnee().y)/fabs(m_personnage.getCoordonnee().y-monstre->getCoordonnee().y);
                                   // if(!getCollision(enCours.x,enCours.y))
                                        m_personnage.setArrivee(enCours);
                                }

                                if(fabs(m_personnage.getCoordonnee().x-monstre->getCoordonnee().x)!=0)
                                {
                                    enCours.x=arrivee.x+(m_personnage.getCoordonnee().x-monstre->getCoordonnee().x)/fabs(m_personnage.getCoordonnee().x-monstre->getCoordonnee().x);
                                    enCours.y=arrivee.y;
                                   // if(!getCollision(enCours.x,enCours.y))
                                        m_personnage.setArrivee(enCours);
                                }

                                if(fabs(m_personnage.getCoordonnee().x-monstre->getCoordonnee().x)!=0&&fabs(monstre->getCoordonnee().x-m_personnage.getCoordonnee().y!=0))
                                {
                                    enCours.x=arrivee.x+(m_personnage.getCoordonnee().x-monstre->getCoordonnee().x)/fabs(m_personnage.getCoordonnee().x-monstre->getCoordonnee().x);
                                    enCours.y=arrivee.y+(m_personnage.getCoordonnee().y-monstre->getCoordonnee().y)/fabs(m_personnage.getCoordonnee().y-monstre->getCoordonnee().y);
                                   // if(!getCollision(enCours.x,enCours.y))
                                        m_personnage.setArrivee(enCours);
                                }*/

                            }
                            else
                            {
                                m_personnage.setArrivee(m_personnage.getCoordonnee());
                                m_personnage.setProchaineCase(m_personnage.getCoordonnee());
                               // m_personnage.setCoordonneePixel(m_personnage.getCoordonnee());
                                coordonnee temp,temp2;
                                temp.x=(m_personnage.getCoordonneePixel().x-m_personnage.getCoordonneePixel().y-1+hauteurMap)*64;
                                temp.y=(m_personnage.getCoordonneePixel().x+m_personnage.getCoordonneePixel().y)*32;
                                temp2.x=(monstre->getCoordonneePixel().x-monstre->getCoordonneePixel().y-1+hauteurMap)*64;
                                temp2.y=(monstre->getCoordonneePixel().x+monstre->getCoordonneePixel().y)*32;
                                m_personnage.frappe(temp2,temp);
                            }
    }
}

void Hero::setMonstreVise(int monstre){m_enemiVise=monstre;}

int Hero::getEnemiVise(){return m_enemiVise;}
#endif
