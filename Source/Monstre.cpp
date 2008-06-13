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
    m_vitesse=0.7;
}

void Monstre::Charger(int numero,Modele_Monstre modele)
{
    m_modele=numero;
}

int Monstre::getModele(){ return m_modele; }

void Monstre::testerVision(coordonnee positionHero)
{
    if(fabs(positionHero.x-m_positionCase.x)<5&&fabs(positionHero.y-m_positionCase.y)<5)
        m_vu=1;
    if(fabs(positionHero.x-m_positionCase.x)>20||fabs(positionHero.y-m_positionCase.y)>20)
        m_vu=0,m_etat=0;
}

bool Monstre::getVu(){return m_vu;}

bool Modele_Monstre::Charger(string chemin)
{
    console.Ajouter("",0);
	console.Ajouter("Chargement du monstre : "+chemin,0);

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
    			fichier>>m_porteeLumineuse.rouge;
    			fichier>>m_porteeLumineuse.vert;
    			fichier>>m_porteeLumineuse.bleu;
    			fichier>>m_porteeLumineuse.intensite;
    		}

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
                        int animation,son,image;

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
                            }
                        }while(caractere!='$');
                        m_pose[i][j].push_back(poseTemp);
                        m_pose[i][j][m_pose[i][j].size()-1].setPose(position,animation,son,image);
                        fichier.get(caractere);
                    }
                    fichier.get(caractere);
                }while(caractere!='$');
    	    }
    	}
    }
    else
    console.Ajouter("Impossible d'ouvrir : "+chemin,1);

    fichier.close();

    return 1;
}
