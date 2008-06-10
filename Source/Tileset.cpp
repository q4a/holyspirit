#include "Tileset.h"

#include "Globale.h"

#include <iostream>
#include <fstream>

#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>

using namespace std;
using namespace sf;

Tileset::Tileset()
{

m_image.SetSmooth(false);
}
Tileset::Tileset(string chemin)
{
    Charger(chemin);
}
Tileset::~Tileset()
{
	//delete[] m_sonAZero;
	//delete[] m_tile;
	//delete[] m_buffer;
	//delete[] m_sons;
}



Tileset Tileset::operator=(const Tileset &tileset)
{
    m_image = tileset.m_image;
    m_tile=tileset.m_tile;

    return *this;
}

bool Tileset::Charger(std::string chemin)
{
	string cheminFinal,temp;
	cheminFinal=chemin+".png";
	if(!m_image.LoadFromFile(cheminFinal.c_str()))
        console.Ajouter("Impossible de charger l'image : "+cheminFinal,1);
    else
    console.Ajouter("Chargement de : "+cheminFinal,0);

	int nombreTiles=0;
	cheminFinal=chemin+".txt";

	m_image.SetSmooth(false);

	ifstream fichier;
    fichier.open(cheminFinal.c_str(), ios::in);
    if(fichier)
    {
        fichier>>temp;
        if(temp=="NoAntiCrenelage")
        m_image.SetSmooth(false);

    	char caractere;
    	do
    	{
    		fichier.get(caractere);
    		if(caractere=='*')
    		{
    			string cheminDuSon;
                getline(fichier, cheminDuSon);

                sf::SoundBuffer bufferTemp;

                m_buffer.push_back(bufferTemp);

                if(!m_buffer[m_buffer.size()-1].LoadFromFile(cheminDuSon.c_str()))
                    console.Ajouter("Impossible de charger le son : "+cheminDuSon,1);
                else
                console.Ajouter("Chargement de : "+cheminDuSon,0);

    		}
    	}while(caractere!='$');

    	sf::Sound sonTemp;

    	m_sonAZero.resize(m_buffer.size(),0);
		m_sons.resize(m_buffer.size(),sonTemp);
		for(int i=0;i<m_buffer.size();i++)
			m_sons[i].SetBuffer(m_buffer[i]),m_sons[i].SetVolume(0);

    	do
    	{
    		fichier.get(caractere);
    		if(caractere=='*')
    		{
    			coordonnee position;
    			int animation=-1,son=-1;
    			Lumiere lumiere;
    			lumiere.intensite=0;
    			bool collision=0,ombre=0;
    			char orientation=' ';
    			do
    			{
    				fichier.get(caractere);
    				switch (caractere)
    				{
    					case 'x': fichier>>position.x; break;
    					case 'y': fichier>>position.y; break;
    					case 'w': fichier>>position.w; break;
    					case 'h': fichier>>position.h; break;
    					case 'c': fichier>>collision; break;
    					case 'a': fichier>>animation; break;
    					case 's': fichier>>son; break;
    					case 'o': fichier>>ombre; break;
    					case 'l':
                                fichier.get(caractere);
                                if(caractere=='r')
                                    fichier>>lumiere.rouge;
                                if(caractere=='v')
                                    fichier>>lumiere.vert;
                                if(caractere=='b')
                                    fichier>>lumiere.bleu;
                                if(caractere=='i')
                                    fichier>>lumiere.intensite;
                                if(caractere=='h')
                                    fichier>>lumiere.hauteur;
    					break;
    					case 'r': fichier>>orientation; break;
    				}
    			}while(caractere!='$');
    			//AjouterTile(position,collision,animation,son,lumiere,ombre,orientation);
    			Tile tileTemp;
    			m_tile.push_back(tileTemp);
    			m_tile[m_tile.size()-1].setTile(position,collision,animation,son,lumiere,ombre,orientation);

    			fichier.get(caractere);
    		}
    	}while(caractere!='$');
    }
    else
        console.Ajouter("Impossible d'ouvrir le fichier :"+cheminFinal,1);

    fichier.close();
    return 1;
}

Image *Tileset::getImage()
{
	return &m_image;
}

coordonnee Tileset::getPositionDuTile(int tile)
{
    if(tile>=0&&tile<m_tile.size())
        return m_tile[tile].getCoordonnee();
    else
    {
        coordonnee position;
        position.x=0;
        position.y=0;
        return position;
    }
}

bool Tileset::getCollisionTile(int tile)
{
    if(tile>=0&&tile<m_tile.size())
        return m_tile[tile].getCollision();

    return 0;
}

int Tileset::getAnimationTile(int tile)
{
    if(tile>=0&&tile<m_tile.size())
        return m_tile[tile].getAnimation();

    return 0;
}


int Tileset::getSonTile(int tile)
{
    if(tile>=0&&tile<m_tile.size())
        return m_tile[tile].getSon();
    return 0;
}

int Tileset::getTaille()
{
    return m_tile.size();
}

Lumiere Tileset::getLumiereDuTile(int tile)
{
    if(tile>=0&&tile<m_tile.size())
        return m_tile[tile].getLumiere();

    Lumiere temp;
    temp.intensite=0;
    temp.rouge=0;
    temp.vert=0;
    temp.bleu=0;
    temp.hauteur=0;

    return temp;
}

bool Tileset::getOmbreDuTile(int tile)
{
    if(tile>=0&&tile<m_tile.size())
        return m_tile[tile].getOmbre();

    return 0;
}
#include "Globale.h"

char Tileset::getOrientationDuTile(int tile)
{
    if(tile>=0&&tile<m_tile.size())
        return m_tile[tile].getOrientation();
    return 0;
}

void Tileset::remiseAZeroDesSons()
{
	for(int i=0;i<m_sonAZero.size();i++)
		m_sonAZero[i]=true;
}


void Tileset::jouerSon(int numeroSon,double distance,coordonnee position,coordonnee positionHero)
{
    if(numeroSon>=0&&numeroSon<m_sons.size())
    {
        if(m_sonAZero[numeroSon])
        {
            m_sons[numeroSon].SetVolume(100);
            m_sonAZero[numeroSon]=false;
            m_sons[numeroSon].SetPosition(-position.x,0,position.y);
        }
        else
        {
            m_sons[numeroSon].SetVolume(100);
            float x,y,z;
            m_sons[numeroSon].GetPosition(x,z,y);
            // Je test voir si le nouveau son du même type est plus près du perso que l'ancien, si oui, je mets la position du nouveau à la place de l'ancien
            if((double)(gpl::sqrt((positionHero.x+x)*(positionHero.x+x)+(positionHero.y-y)*(positionHero.y-y)))>(double)(gpl::sqrt((positionHero.x-position.x)*(positionHero.x-position.x)+(positionHero.y-position.y)*(positionHero.y-position.y))))
                m_sons[numeroSon].SetPosition(-position.x,0,position.y);
        }

        Sound::Status Status = m_sons[numeroSon].GetStatus();

        if(Status==0)
        m_sons[numeroSon].Play();
    }
}

void Tileset::deleteTiles()
{
    m_tile.clear();
	//for(int i=0;i<m_nombreSons;i++)
	//m_sons[i].Stop();
	if(m_sonAZero.size()>0)
	{
        m_sonAZero.clear();
        m_buffer.clear();
        m_sons.clear();
    }
}



