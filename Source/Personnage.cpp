///**************************************************\\\
||| Dernière modification : Le 06/02/08 par Gregouar |||
///**************************************************///




#include "Personnage.h"
#include "Globale.h"

#include <iostream>
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
}
Modele_Personnage::Modele_Personnage()
{
}
Modele_Personnage::~Modele_Personnage()
{
    for(int i=0;i<m_pose.size();i++)
    {
         for(int j=0;j<m_pose[i].size();j++)
            m_pose[i][j].clear();
         m_pose[i].clear();
    }
    m_pose.clear();
}

bool Modele_Personnage::Charger(string chemin)
{
    console.Ajouter("",0);
	console.Ajouter("Chargement du personnage : "+chemin,0);

    ifstream fichier;
    fichier.open(chemin.c_str(), ios::in);
    if(fichier)
    {
    	char caractere;
    	do
    	{
    		fichier.get(caractere);
    		if(caractere=='*')
    		{

    			string cheminImage;
                getline(fichier, cheminImage);
                //AjouterImage(cheminImage);
                sf::Image temp;
                m_image.push_back(temp);
                if(!m_image[m_image.size()-1].LoadFromFile(cheminImage.c_str()))
                    console.Ajouter("Impossible de charger : "+cheminImage,1);
                else
                console.Ajouter("Chargement de : "+cheminImage,0);
    		}
    	}while(caractere!='$');

    	do
    	{
    		fichier.get(caractere);
    		if(caractere=='*')
    		{

    			string cheminSon;
                getline(fichier, cheminSon);
                 sf::SoundBuffer temp;
                m_buffer.push_back(temp);
                if(!m_buffer[m_buffer.size()-1].LoadFromFile(cheminSon.c_str()))
                    console.Ajouter("Impossible de charger : "+cheminSon,1);
                else
                console.Ajouter("Chargement de : "+cheminSon,0);
    		}
    	}while(caractere!='$');

    	sf::Sound temp;
    	m_sons.resize(m_buffer.size(),temp);

    	for(int i=0;i<m_buffer.size();i++)
			m_sons[i].SetBuffer(m_buffer[i]),m_sons[i].SetVolume(100);

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
                        coordonnee position;
                        int animation,son,image,attaque=-1;

                        do
                        {
                            fichier.get(caractere);
                            switch (caractere)
                            {
                                case 'x': fichier>>position.x; break;
                                case 'y': fichier>>position.y; break;
                                case 'w': fichier>>position.w; break;
                                case 'h': fichier>>position.h; break;
                                case 'a': fichier>>animation; break;
                                case 's': fichier>>son; break;
                                case 'i': fichier>>image; break;
                                case 'd': fichier>>attaque; break;
                            }
                        }while(caractere!='$');
                        m_pose[i][j].push_back(poseTemp);
                        m_pose[i][j][m_pose[i][j].size()-1].setPose(position,animation,son,image,attaque);
                        fichier.get(caractere);
                    }
                    fichier.get(caractere);
                }while(caractere!='$');
    	    }
    	}
    }
    else
    {
        cout<<"Impossible d'ouvrir : "<<chemin<<endl;
        return 0;
    }
    fichier.close();

    return 1;
}

void Personnage::Afficher(sf::RenderWindow* ecran,sf::View *camera,coordonnee position,coordonnee dimensionsMap,LumiereOmbrage *lumiere,Modele_Personnage *modele)
{
    if((int)(m_angle/45)>=0&&(int)(m_angle/45)<8)
    {
        Sprite Sprite;
        Sprite.SetImage(modele->m_image[modele->m_pose[m_etat][(int)(m_angle/45)][m_poseEnCours].getImage()]);
        Sprite.SetSubRect(IntRect(modele->m_pose[m_etat][(int)(m_angle/45)][m_poseEnCours].getCoordonnee().x, modele->m_pose[m_etat][(int)(m_angle/45)][m_poseEnCours].getCoordonnee().y, modele->m_pose[m_etat][(int)(m_angle/45)][m_poseEnCours].getCoordonnee().x+modele->m_pose[m_etat][(int)(m_angle/45)][m_poseEnCours].getCoordonnee().w, modele->m_pose[m_etat][(int)(m_angle/45)][m_poseEnCours].getCoordonnee().y+modele->m_pose[m_etat][(int)(m_angle/45)][m_poseEnCours].getCoordonnee().h));

        if(configuration.Ombre)
        {
            for(int o=0;o<lumiere->m_ombre.size();o++)
            {
                Sprite.SetColor(sf::Color(0,0,0,lumiere->m_ombre[o].intensite));

                 Sprite.SetScale(1, lumiere->m_ombre[o].taille);

                Sprite.SetRotationCenter((modele->m_pose[m_etat][(int)(m_angle/45)][m_poseEnCours].getCoordonnee().w/2),(modele->m_pose[m_etat][(int)(m_angle/45)][m_poseEnCours].getCoordonnee().h-32)*lumiere->m_ombre[o].taille);
                Sprite.SetRotation(lumiere->m_ombre[o].angle);

                if(lumiere->m_ombre[o].angle>90&&lumiere->m_ombre[o].angle<270)
                    Sprite.FlipX(true);

                Sprite.SetLeft(((m_positionPixel.x-m_positionPixel.y)*64/COTE_TILE+dimensionsMap.y*64)-64+(64-Sprite.GetWidth()/2)+64-modele->m_pose[m_etat][(int)(m_angle/45)][m_poseEnCours].getCoordonnee().w/2);
                Sprite.SetTop(((m_positionPixel.x+m_positionPixel.y)*64/COTE_TILE)/2+(64-Sprite.GetHeight())+32*lumiere->m_ombre[o].taille-32);

                ecran->Draw(Sprite);
                Sprite.SetScale(1, 1);
                Sprite.SetRotation(0);
            }
        }

        Sprite.FlipX(false);

        Sprite.SetLeft(((m_positionPixel.x-m_positionPixel.y)*64/COTE_TILE+dimensionsMap.y*64)-64+(64-Sprite.GetWidth()/2)+64-modele->m_pose[m_etat][(int)(m_angle/45)][m_poseEnCours].getCoordonnee().w/2);
        Sprite.SetTop(((m_positionPixel.x+m_positionPixel.y)*64/COTE_TILE)/2+(64-Sprite.GetHeight()));

        if(configuration.Lumiere)
            Sprite.SetColor(sf::Color((lumiere->intensite*lumiere->rouge)/255,(lumiere->intensite*lumiere->vert)/255,(lumiere->intensite*lumiere->bleu)/255, 255));

        if(position.x-camera->Rect.Left+Sprite.GetWidth()>0-32*camera->Zoom*10)
        if(position.x-camera->Rect.Left<800+32*camera->Zoom*10)
        if(position.y-camera->Rect.Top+Sprite.GetHeight()>0-32*camera->Zoom*10)
        if(position.y-camera->Rect.Top<500+32*camera->Zoom*10)
        ecran->Draw(Sprite);
    }
}

coordonnee Personnage::getCoordonnee()
{
	return m_positionCase;
}

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
}
void Personnage::setVitesse(float vitesse){m_vitesse=vitesse;}
void Personnage::setEtat(int etat){m_etat=etat;}

int Personnage::getEtat(){return m_etat;}

coordonnee Personnage::getCoordonneePixel()
{
    coordonnee position;
    position.x=(int)m_positionPixel.x;
    position.y=(int)m_positionPixel.y;
	return position;
}

coordonnee Personnage::getProchaineCase()
{
    if(m_cheminFinal.x!=m_positionCase.x||m_cheminFinal.y!=m_positionCase.y)
	return m_cheminFinal;
	else
	return m_positionCase;
}

Lumiere Modele_Personnage::getPorteeLumineuse()
{
    return m_porteeLumineuse;
}

int Personnage::pathfinding(vector<vector<bool> > map)
{
	bool erreur=false;
	liste_case casesVisitee;
	coordonnee depart,arrivee,decalage;

	decalage.x=m_positionCase.x-10;
	decalage.y=m_positionCase.y-10;

	depart.x=m_positionCase.x-decalage.x;
	depart.y=m_positionCase.y-decalage.y;
	arrivee.x=m_arrivee.x-decalage.x;
	arrivee.y=m_arrivee.y-decalage.y;

	casesVisitee.setTailleListe(0);

	casesVisitee.ajouterCase(depart);

	while(!casesVisitee.testerCasesEnCours(arrivee)&&!erreur)
	{
		casesVisitee.incrementerDistanceEnCours();
		casesVisitee.ajouterCasesAdjacentes(map,&arrivee);
		if(casesVisitee.getDistance()>20)
			erreur=true;
	}

	if(!erreur)
	{
		m_cheminFinal=arrivee;

		while(casesVisitee.getDistance()>1)
		{
			m_cheminFinal=casesVisitee.trouverLeChemin(m_cheminFinal);
			casesVisitee.decrementerDistanceEnCours();
		}

		m_arrivee.x=arrivee.x+decalage.x;
		m_arrivee.y=arrivee.y+decalage.y;

		m_cheminFinal.x+=decalage.x,m_cheminFinal.y+=decalage.y;
		m_ancienneArrivee=m_arrivee;
	}
	else
	{
	     m_mauvaiseArrivee=m_arrivee;
        if(m_positionCase.x<m_arrivee.x)
        m_arrivee.x=m_arrivee.x-1;
        if(m_positionCase.x>m_arrivee.x)
        m_arrivee.x=m_arrivee.x+1;
        if(m_positionCase.y<m_arrivee.y)
        m_arrivee.y=m_arrivee.y-1;
        if(m_positionCase.y>m_arrivee.y)
        m_arrivee.y=m_arrivee.y+1;



       // m_arrivee.y=m_ancienneArrivee.y;
        //if(!(m_positionCase.y==m_arrivee.y&&m_positionCase.x==m_arrivee.x))
	    //pathfinding(map);

        m_arrivee.x=m_ancienneArrivee.x;
        m_arrivee.y=m_ancienneArrivee.y;
        m_etat=0;
    }

	if(m_etat==0)
	return 0;
	else
	return 1;
}




bool Personnage::seDeplacer(float tempsEcoule)
{
	if(m_positionCase.y!=m_cheminFinal.y||m_positionCase.x!=m_cheminFinal.x)
	{
	    m_etat=1;
		m_positionPixelPrecedente.x=(int)m_positionPixel.x;
		m_positionPixelPrecedente.y=(int)m_positionPixel.y;

		if(m_positionCase.x<m_cheminFinal.x)
		{
			if(m_positionCase.y>m_cheminFinal.y)
				m_positionPixel.x+=2*tempsEcoule*m_vitesse;
			else
				m_positionPixel.x+=4*tempsEcoule*m_vitesse;
		}
		if(m_positionCase.x>m_cheminFinal.x)
		{
			if(m_positionCase.y<m_cheminFinal.y)
				m_positionPixel.x-=2*tempsEcoule*m_vitesse;
			else
				m_positionPixel.x-=4*tempsEcoule*m_vitesse;
		}
		if(m_positionCase.y<m_cheminFinal.y)
		{
			if(m_positionCase.x>m_cheminFinal.x)
				m_positionPixel.y+=2*tempsEcoule*m_vitesse;
			else
				m_positionPixel.y+=4*tempsEcoule*m_vitesse;
		}
		if(m_positionCase.y>m_cheminFinal.y)
		{
			if(m_positionCase.x<m_cheminFinal.x)
				m_positionPixel.y-=2*tempsEcoule*m_vitesse;
			else
				m_positionPixel.y-=4*tempsEcoule*m_vitesse;
		}

		//m_angle=atan((double)(m_positionCase.y-m_cheminFinal.y)/(double)(m_positionCase.x-m_cheminFinal.x))*360/(2*M_PI);

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

				m_positionCase.y=m_cheminFinal.y;
				m_positionCase.x=m_cheminFinal.x;

				return 1;
			}


	}
	else if(m_arrivee.x!=m_positionCase.x||m_arrivee.y!=m_positionCase.y)
	return 1;
	else if(m_etat!=2||m_etat==2&&m_poseEnCours==0)
	m_etat=0;

	return 0;
}


void Personnage::setArrivee(coordonnee arrivee)
{
    if(!(m_mauvaiseArrivee.x==arrivee.x&&m_mauvaiseArrivee.y==arrivee.y))
    {
        m_arrivee=arrivee;
    }
}

void Personnage::setCoordonneePixel(coordonnee position)
{
    m_positionPixel.x=position.x*COTE_TILE;
    m_positionPixel.y=position.y*COTE_TILE;
}
void Personnage::setProchaineCase(coordonnee position)
{
    m_cheminFinal=position;
}


void Personnage::animer(Modele_Personnage *modele,int hauteur_map,float temps)
{
    m_animation+=temps;

    while(m_animation>=0.075)
    {
        coordonnee position;
        position.x=(m_positionCase.x-m_positionCase.y-1+hauteur_map)/5;
        position.y=(m_positionCase.x+m_positionCase.y)/5;

        m_poseEnCours=modele->m_pose[m_etat][(int)(m_angle/45)][m_poseEnCours].getAnimation();

        if(m_poseEnCours>modele->m_pose[m_etat][(int)(m_angle/45)].size())
        m_poseEnCours=0;

        modele->jouerSon(modele->m_pose[m_etat][(int)(m_angle/45)][m_poseEnCours].getSon(),0,position);
        m_animation-=0.075;
        if(modele->m_pose[m_etat][(int)(m_angle/45)][m_poseEnCours].getAttaque()==0)
            configuration.effetMort+=3;
        if(configuration.effetMort>200)
        configuration.effetMort=200;
    }
}

void Personnage::frappe(coordonnee direction,coordonnee position)
{
    if(m_etat!=2)
    if(m_positionCase.x==m_cheminFinal.x&&m_positionCase.y==m_cheminFinal.y)
    {
        m_etat=2;
        m_poseEnCours=1;
    }

        double m=atan(-((double)direction.y-position.y)/((double)direction.x-position.x));
		if(direction.x-position.x<0)
		m-=M_PI;
		m+=M_PI/8;
		m_angle=(int)(0+(m*360)/(2*M_PI));
		if(m_angle>=360)
		m_angle=0;
		while(m_angle<0)
		m_angle=360+m_angle;

    m_arrivee=m_cheminFinal;
}


void Modele_Personnage::jouerSon(int numeroSon,double distance,coordonnee position)
{
    if(numeroSon>=0&&numeroSon<m_sons.size())
    {
        m_sons[numeroSon].SetVolume(1000);
        m_sons[numeroSon].SetPosition(-position.x,0,position.y);
        Sound::Status Status = m_sons[numeroSon].GetStatus();

        if(Status==0)
        m_sons[numeroSon].Play();
    }
}

void Modele_Personnage::setPorteeLumineuse(Lumiere  lumiere){m_porteeLumineuse=lumiere;}


int Modele_Personnage::getNombreSons(){return m_buffer.size();}
