#include <iostream>
#include <fstream>

#include "Monstre.h"
#include "Constantes.h"
#include "Globale.h"

using namespace std;

Monstre::Monstre()
{
    m_vu=0;
    m_modele=-1;
    m_monstre=true;
}

void Monstre::Charger(int numero,Modele_Monstre *modele)
{
    m_modele=numero;
    m_caracteristique=modele->getCaracteristique();
    m_porteeLumineuse=modele->getPorteeLumineuse();
    if(m_caracteristique.rang==0&&m_caracteristique.pointAme>0)
    {
        int temp=rand()%(1000);
        if(temp<50)
            m_caracteristique.rang=1;
        if(temp<15)
            m_caracteristique.rang=2;

        if(m_caracteristique.rang==1)
        {
            m_porteeLumineuse.intensite=512;
            m_caracteristique.maxVie*=3;
            m_caracteristique.vie*=3;
            m_caracteristique.degatsMin*=1;
            m_caracteristique.degatsMax*=1;
            m_caracteristique.pointAme*=2;

            m_caracteristique.modificateurTaille*=1.1;
        }
        if(m_caracteristique.rang==2)
        {
            m_porteeLumineuse.intensite=768;
            m_caracteristique.maxVie*=5;
            m_caracteristique.vie*=5;
            m_caracteristique.degatsMin*=2;
            m_caracteristique.degatsMax*=2;
            m_caracteristique.pointAme*=4;

            m_caracteristique.modificateurTaille*=1.2;
        }

        if(m_caracteristique.rang>0)
        {
            m_porteeLumineuse.rouge=rand()%255;
            m_porteeLumineuse.vert=rand()%255;
            m_porteeLumineuse.bleu=rand()%255;
        }
    }
}

bool Modele_Monstre::Charger(string chemin)
{
    console.Ajouter("",0);
	console.Ajouter("Chargement du monstre : "+chemin,0);

    m_caracteristique.vitesse=0;
    m_caracteristique.vie=0;
    m_caracteristique.degatsMin=0;
    m_caracteristique.degatsMax=0;

    m_chemin=chemin;

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
                /*sf::Image temp;
                m_image.push_back(temp);
                if(!m_image[m_image.size()-1].LoadFromFile(cheminImage.c_str()))
                    console.Ajouter("Impossible de charger : "+cheminImage,1);
                else
                console.Ajouter("Chargement de : "+cheminImage,0);*/
                m_image.push_back(moteurGraphique.AjouterImage(cheminImage));
    		}
    		if(fichier.eof()){ char temp[1000]; sprintf(temp,"Erreur : Monstre \" %s \" Invalide",chemin.c_str());console.Ajouter(temp,1); caractere='$'; m_caracteristique.maxVie=0; }
    	}while(caractere!='$');

    	m_buffer.clear();
    	do
    	{
    		fichier.get(caractere);
    		if(caractere=='*')
    		{

    			string cheminSon;
                getline(fichier, cheminSon);
                sf::SoundBuffer temp;
                m_buffer.push_back(temp);
                if(m_buffer.size()>0)
                    if(!m_buffer[m_buffer.size()-1].LoadFromFile(cheminSon.c_str()))
                        console.Ajouter("Impossible de charger : "+cheminSon,1);
                else
                console.Ajouter("Chargement de : "+cheminSon,0);
    		}
    		if(fichier.eof()){ char temp[1000]; sprintf(temp,"Erreur : Monstre \" %s \" Invalide",chemin.c_str());console.Ajouter(temp,1); caractere='$'; m_caracteristique.maxVie=0;}
    	}while(caractere!='$');

    	sf::Sound temp;
    	m_sons.resize(m_buffer.size(),temp);

    	for(int i=0;i<m_buffer.size();i++)
			m_sons[i].SetBuffer(m_buffer[i]),m_sons[i].SetVolume(100);


        do
    	{
    	    //Chargement de la lumière ambiante
    		fichier.get(caractere);
    		if(caractere=='*')
            {
                do
                {
                    fichier.get(caractere);
                    switch (caractere)
                    {
                        case 'r': fichier>>m_porteeLumineuse.rouge; break;
                        case 'v': fichier>>m_porteeLumineuse.vert; break;
                        case 'b': fichier>>m_porteeLumineuse.bleu; break;
                        case 'i': fichier>>m_porteeLumineuse.intensite; break;
                    }
                     if(fichier.eof()){ char temp[1000]; sprintf(temp,"Erreur : Monstre \" %s \" Invalide",chemin.c_str());console.Ajouter(temp,1); caractere='$'; m_caracteristique.maxVie=0;}
                }while(caractere!='$');
                fichier.get(caractere);
                 if(fichier.eof()){ char temp[1000]; sprintf(temp,"Erreur : Monstre \" %s \" Invalide",chemin.c_str());console.Ajouter(temp,1); caractere='$'; m_caracteristique.maxVie=0;}
            }
    		if(fichier.eof()){ char temp[1000]; sprintf(temp,"Erreur : Monstre \" %s \" Invalide",chemin.c_str());console.Ajouter(temp,1); caractere='$'; }

    	}while(caractere!='$');

    	do
    	{
    	    //Chargement de la lumière ambiante
    		fichier.get(caractere);
    		if(caractere=='*')
            {
                getline(fichier, m_caracteristique.nom);
            }
    		if(fichier.eof()){ char temp[1000]; sprintf(temp,"Erreur : Monstre \" %s \" Invalide",chemin.c_str());console.Ajouter(temp,1); caractere='$'; }

    	}while(caractere!='$');

    	do
    	{
    	    //Chargement de la lumière ambiante
    		fichier.get(caractere);
    		if(caractere=='*')
            {
                fichier>>m_caracteristique.rang;
            }
    		if(fichier.eof()){ char temp[1000]; sprintf(temp,"Erreur : Monstre \" %s \" Invalide",chemin.c_str());console.Ajouter(temp,1); caractere='$'; }

    	}while(caractere!='$');

    	do
        {
            if(caractere=='*')
            {
                m_caracteristique.modificateurTaille=1;
                m_caracteristique.sang=0;
                m_explosif=false;
                do
                {
                    fichier.get(caractere);
                    switch (caractere)
                    {
                        case 'v': fichier>>m_caracteristique.maxVie; break;
                        case 'd':  fichier.get(caractere); if(caractere=='i') fichier>>m_caracteristique.degatsMin; else fichier>>m_caracteristique.degatsMax ;  break;
                        case 'm': fichier>>m_caracteristique.vitesse; break;
                        case 'o': fichier>>m_ombre; break;
                        case 'a': fichier>>m_caracteristique.pointAme; break;
                        case 'n': fichier>>m_caracteristique.niveau; break;

                        case 's': fichier>>m_caracteristique.sang; break;

                        case 't': fichier>>m_caracteristique.modificateurTaille; break;

                        case 'e': fichier>>m_explosif; break;
                    }
                     if(fichier.eof()){ char temp[1000]; sprintf(temp,"Erreur : Monstre \" %s \" Invalide",chemin.c_str());console.Ajouter(temp,1); caractere='$'; m_caracteristique.maxVie=0;}
                }while(caractere!='$');


                m_caracteristique.vie=m_caracteristique.maxVie;

                fichier.get(caractere);
                 if(fichier.eof()){ char temp[1000]; sprintf(temp,"Erreur : Monstre \" %s \" Invalide",chemin.c_str());console.Ajouter(temp,1); caractere='$'; m_caracteristique.maxVie=0;}
            }
            fichier.get(caractere);
            if(fichier.eof()){ char temp[1000]; sprintf(temp,"Erreur : Monstre \" %s \" Invalide",chemin.c_str());console.Ajouter(temp,1); caractere='$'; m_caracteristique.maxVie=0;  }
        }while(caractere!='$');


        do
    	{
    	    //Chargement de la lumière ambiante
    		fichier.get(caractere);
    		if(caractere=='*')
            {
                ModeleObjet tempModeleObjet;
                do
                {
                    fichier.get(caractere);
                    if(caractere=='r')
                    {
                        int temp2;
                        fichier>>temp2;
                        tempModeleObjet.setChanceTrouver(temp2);
                    }

                    if(caractere=='*')
                    {
                        string temp2;
                        getline(fichier, temp2);
                        tempModeleObjet.Charger(temp2);
                    }

                    if(fichier.eof()){ char temp[1000]; sprintf(temp,"Erreur : Monstre \" %s \" Invalide",chemin.c_str());console.Ajouter(temp,1); caractere='$'; }
                }while(caractere!='$');
                m_objets.push_back(tempModeleObjet);
                fichier.get(caractere);
            }
    		if(fichier.eof()){ char temp[1000]; sprintf(temp,"Erreur : Monstre \" %s \" Invalide",chemin.c_str());console.Ajouter(temp,1); caractere='$'; }

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
                        coordonnee position;
                        int animation,son,image,attaque=-1,lumiere=m_porteeLumineuse.intensite;

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
                                case 'l': fichier>>lumiere; break;
                            }
                            if(fichier.eof()){ char temp[1000]; sprintf(temp,"Erreur : Monstre \" %s \" Invalide",chemin.c_str());console.Ajouter(temp,1); caractere='$'; m_caracteristique.maxVie=0;}
                        }while(caractere!='$');

                        m_pose[i][j].push_back(poseTemp);
                        m_pose[i][j][m_pose[i][j].size()-1].setPose(position,animation,son,image,attaque,lumiere);
                        fichier.get(caractere);
                        if(fichier.eof()){ char temp[1000]; sprintf(temp,"Erreur : Monstre \" %s \" Invalide",chemin.c_str());console.Ajouter(temp,1); caractere='$'; m_caracteristique.maxVie=0;  }
                    }
                    fichier.get(caractere);
                    if(fichier.eof()){ char temp[1000]; sprintf(temp,"Erreur : Monstre \" %s \" Invalide",chemin.c_str());console.Ajouter(temp,1); caractere='$'; m_caracteristique.maxVie=0; }
                }while(caractere!='$');
    	    }
    	}
    }
    else
    console.Ajouter("Impossible d'ouvrir : "+chemin,1);

    fichier.close();

    return 1;
}

std::vector<ModeleObjet> Modele_Monstre::getObjets(){return m_objets;}

void Monstre::testerVision(coordonnee positionHero)
{
    if(m_caracteristique.vie>0)
    {
        if(fabs(positionHero.x-m_positionCase.x)<5&&fabs(positionHero.y-m_positionCase.y)<5)
        {
            m_vu=1;
            if(m_etat==0) m_poseEnCours=0;
        }
        if(fabs(positionHero.x-m_positionCase.x)>10||fabs(positionHero.y-m_positionCase.y)>10)
        {
            if(m_vu)
                setDepart();
            m_vu=0,m_etat=0,m_poseEnCours=0;

        }
    }
}

coordonnee Monstre::getDepart(){return m_depart;}

int Monstre::getModele(){ return m_modele; }

bool Monstre::getVu(){return m_vu;}
void Monstre::setVu(bool vu){m_vu=vu;}

void Monstre::setDepart()
{
    m_depart=m_positionCase;
}
