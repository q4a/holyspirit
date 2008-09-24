#ifndef HEROCPP
#define HEROCPP

#include "Hero.h"
#include "Globale.h"

#include <iostream>
#include <fstream>

#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include <math.h>
#include <dirent.h>

using namespace std;
using namespace sf;

int crypter(int);
int decrypter(int);


Hero::Hero()
{
   ///Je donnes des valeur à mes variables juste pour les tests
	m_personnage.setEtat(ARRET);

	coordonnee position;
	position.x=-10000;
	position.y=-10000;

	m_personnage.setCoordonnee(position);

	Lumiere lumiere;
	lumiere.intensite=160;
	lumiere.rouge=255;
	lumiere.vert=255;
	lumiere.bleu=255;
	lumiere.hauteur=20;

	m_modelePersonnage.setPorteeLumineuse(lumiere);

	Caracteristique temp;

	temp.vie=100;
	temp.maxVie=100;
	temp.degatsMin=1;
	temp.degatsMax=3;
	temp.vitesse=1;
	temp.pointAme=0;
	temp.ancienPointAme=0;
	temp.positionAncienAme=0;
	temp.niveau=1;
	temp.nom="Héro";

    temp.modificateurTaille=1;

	m_personnage.setCaracteristique(temp);

	m_monstreVise=-1;
}

void Hero::Sauvegarder()
{
    ofstream fichier;
    fichier.open((configuration.chemin_saves+"hero.sav.hs").c_str(), ios::out);
    if(fichier)
    {
        fichier<<"* nom: "<<m_personnage.getCaracteristique().nom<<"\n";
        fichier<<"* maxVie: "<<crypter(m_personnage.getCaracteristique().maxVie)<<"\n";
        fichier<<"* dgtsMin: "<<crypter(m_personnage.getCaracteristique().degatsMin)<<"\n";
        fichier<<"* dgtsMax: "<<crypter(m_personnage.getCaracteristique().degatsMax)<<"\n";
        fichier<<"* vitesse: "<<crypter((int)m_personnage.getCaracteristique().vitesse*1000)<<"\n";
        fichier<<"* ptAme: "<<crypter(m_personnage.getCaracteristique().pointAme)<<"\n";
        fichier<<"* niveau: "<<crypter(m_personnage.getCaracteristique().niveau)<<"\n";

    }
    fichier.close();
}
void Hero::Charger()
{

    struct dirent *lecture;

	DIR *repertoire;
    repertoire = opendir(configuration.chemin_saves.c_str());
    while ((lecture = readdir(repertoire)))
    {
        string temp="hero.sav.hs";
        if(lecture->d_name==temp)
        {
            ifstream fichier;
            fichier.open((configuration.chemin_saves+"hero.sav.hs").c_str(), ios::in);
            if(fichier)
            {
                char caractere;
                Caracteristique charTemp;
                charTemp=m_personnage.getCaracteristique();
                do
                {
                    fichier.get(caractere);
                    if(caractere=='*')
                    {
                        int temp2;
                        string temp;
                        fichier>>temp;
                            if(temp == "nom:")  fichier>>charTemp.nom;
                            if(temp == "maxVie:")  fichier>>temp2,charTemp.maxVie=decrypter(temp2),charTemp.vie=charTemp.maxVie;
                            if(temp == "dgtsMin:")  fichier>>temp2,charTemp.degatsMin=decrypter(temp2);
                            if(temp == "dgtsMax:")  fichier>>temp2,charTemp.degatsMax=decrypter(temp2);
                            if(temp == "vitesse:")  fichier>>temp2,charTemp.vitesse=decrypter(temp2)/1000;
                            if(temp == "ptAme:")  fichier>>temp2,charTemp.pointAme=decrypter(temp2),charTemp.ancienPointAme=charTemp.pointAme,charTemp.positionAncienAme=charTemp.pointAme;
                            if(temp == "niveau:")  fichier>>temp2,charTemp.niveau=decrypter(temp2);
                    }
                }while(!fichier.eof());
                m_personnage.setCaracteristique(charTemp);
            }
            fichier.close();
        }
    }
    closedir(repertoire);

}

void Hero::placerCamera(sf::View *camera,coordonnee dimensionsMap)
{
	m_positionAffichage.y=(((m_personnage.getCoordonneePixel().x+m_personnage.getCoordonneePixel().y)*64/COTE_TILE)/2);
	m_positionAffichage.x=(((m_personnage.getCoordonneePixel().x-m_personnage.getCoordonneePixel().y)*64/COTE_TILE+dimensionsMap.y*64)-64);

	coordonneeDecimal positionCamera;
	positionCamera.y=m_positionAffichage.y-250*configuration.Resolution.y/600;
	positionCamera.x=m_positionAffichage.x-((400*configuration.Resolution.x/800))+64;

	camera->SetFromRect(sf::FloatRect(positionCamera.x,positionCamera.y,positionCamera.x+configuration.Resolution.x,positionCamera.y+configuration.Resolution.y));
}

void Hero::testMontreVise(Monstre *monstre,int hauteurMap)
{
    if(m_monstreVise>-1&&m_personnage.getCaracteristique().vie>0)
    {
        if(monstre->getCaracteristique().vitesse>0&&fabs(m_personnage.getCoordonnee().x-monstre->getProchaineCase().x)>1||monstre->getCaracteristique().vitesse>0&&fabs(m_personnage.getCoordonnee().y-monstre->getProchaineCase().y)>1
        ||monstre->getCaracteristique().vitesse<=0&&fabs(m_personnage.getCoordonnee().x-monstre->getCoordonnee().x)>1||monstre->getCaracteristique().vitesse<=0&&fabs(m_personnage.getCoordonnee().y-monstre->getCoordonnee().y)>1)
        {
                m_personnage.setArrivee(monstre->getProchaineCase());
        }
        else if(m_personnage.getArrivee().x==m_personnage.getCoordonnee().x&&m_personnage.getArrivee().x==m_personnage.getCoordonnee().x)
        {
            m_personnage.setArrivee(m_personnage.getProchaineCase());
            coordonnee temp,temp2;
            temp.x=(m_personnage.getCoordonneePixel().x-m_personnage.getCoordonneePixel().y-1+hauteurMap)*64;
            temp.y=(m_personnage.getCoordonneePixel().x+m_personnage.getCoordonneePixel().y)*32;
            temp2.x=(monstre->getCoordonneePixel().x-monstre->getCoordonneePixel().y-1+hauteurMap)*64;
            temp2.y=(monstre->getCoordonneePixel().x+monstre->getCoordonneePixel().y)*32;
            m_personnage.frappe(temp2,temp);
        }
    }
}

void Hero::augmenterAme(float temps)
{
    Caracteristique temp = m_personnage.getCaracteristique();

    temp.ancienPointAme+=(temp.pointAme-temp.positionAncienAme)*temps*0.7;

    if(temp.ancienPointAme>=temp.pointAme)
        temp.ancienPointAme=temp.pointAme,temp.positionAncienAme=temp.ancienPointAme;

    if(temp.ancienPointAme>=CALCUL_PA_PROCHAIN_NIVEAU)
    {
        temp.niveau++;
        temp.maxVie+=25;
        temp.vie=temp.maxVie;
        temp.degatsMax++;
        temp.degatsMin++;
    }

    m_personnage.setCaracteristique(temp);
}

void Hero::setMonstreVise(int monstre){m_monstreVise=monstre;}

int Hero::getMonstreVise(){return m_monstreVise;}
#endif
