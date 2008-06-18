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
}

bool Modele_Monstre::Charger(string chemin)
{
    console.Ajouter("",0);
	console.Ajouter("Chargement du monstre : "+chemin,0);

    m_caracteristique.vitesse=0;
    m_caracteristique.vie=0;
    m_caracteristique.degatsMin=0;
    m_caracteristique.degatsMax=0;

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
            if(caractere=='*')
            {
                do
                {
                    fichier.get(caractere);
                    switch (caractere)
                    {
                        case 'v': fichier>>m_caracteristique.maxVie; break;
                        case 'd':  fichier.get(caractere); if(caractere=='i') fichier>>m_caracteristique.degatsMin; else fichier>>m_caracteristique.degatsMax ;  break;
                        case 'm': fichier>>m_caracteristique.vitesse; break;
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
                            if(fichier.eof()){ char temp[1000]; sprintf(temp,"Erreur : Monstre \" %s \" Invalide",chemin.c_str());console.Ajouter(temp,1); caractere='$'; m_caracteristique.maxVie=0;}
                        }while(caractere!='$');

                        m_pose[i][j].push_back(poseTemp);
                        m_pose[i][j][m_pose[i][j].size()-1].setPose(position,animation,son,image,attaque);
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
        m_vu=0,m_etat=0,m_poseEnCours=0;
    }
}

int Monstre::getModele(){ return m_modele; }

bool Monstre::getVu(){return m_vu;}
