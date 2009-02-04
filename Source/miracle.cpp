
#include "miracle.h"
#include "globale.h"

#include <iostream>
#include <fstream>

using namespace std;
using namespace sf;



void Projectile::Afficher(sf::RenderWindow* ecran,sf::View *camera,coordonnee position,coordonnee dimensionsMap)
{
    Sprite sprite;

    sprite.SetImage(*moteurGraphique.getImage(m_image));
    sprite.SetSubRect(IntRect(m_positionImage.x, m_positionImage.y, m_positionImage.x+m_positionImage.w, m_positionImage.y+m_positionImage.h));

    sprite.FlipX(false);

    sprite.SetX(((m_position.x-m_position.y)*64/COTE_TILE+dimensionsMap.y*64) - (m_centre.x - m_positionImage.w/2) * cos(-m_rotationReelle-M_PI/4));
    sprite.SetY(((m_position.x+m_position.y)*64/COTE_TILE)/2+(64-sprite.GetSize().y)+16);

    sprite.SetCenter(m_centre.x,m_centre.y);

    sprite.Rotate((-(m_rotation)*180/M_PI)/*-45*/);

    if(sprite.GetPosition().x+sprite.GetSize().x>=camera->GetRect().Left
    &&sprite.GetPosition().x<camera->GetRect().Right
    &&sprite.GetPosition().y+sprite.GetSize().y>=camera->GetRect().Top
    &&sprite.GetPosition().y<camera->GetRect().Bottom)
    moteurGraphique.AjouterCommande(&sprite,10,1);
}

void EffetGraphique::Afficher(sf::RenderWindow* ecran,sf::View *camera,coordonnee position,coordonnee dimensionsMap)
{
    Sprite sprite;

    sprite.SetImage(*moteurGraphique.getImage(m_image));
    sprite.SetSubRect(IntRect(m_positionImage.x, m_positionImage.y, m_positionImage.x+m_positionImage.w, m_positionImage.y+m_positionImage.h));

    sprite.FlipX(false);

    sprite.SetX(((m_position.x-m_position.y)*64/COTE_TILE+dimensionsMap.y*64)/*-64+(64-sprite.GetSize().x/2)*/);
    sprite.SetY(((m_position.x+m_position.y)*64/COTE_TILE)/2+(64-sprite.GetSize().y)+16);

    sprite.SetCenter(m_positionImage.w/2,m_positionImage.h/2);

    if(sprite.GetPosition().x+sprite.GetSize().x>=camera->GetRect().Left
    &&sprite.GetPosition().x<camera->GetRect().Right
    &&sprite.GetPosition().y+sprite.GetSize().y>=camera->GetRect().Top
    &&sprite.GetPosition().y<camera->GetRect().Bottom)
    moteurGraphique.AjouterCommande(&sprite,10,1);
}

void Projectile::Deplacer(float temps)
{
    m_position.x+=m_vecteur.x*temps*500;
    m_position.y+=m_vecteur.y*temps*500;
}


Miracle::Miracle()
{
}

Miracle::Miracle(std::string chemin)
{
    Charger(chemin);
}

Miracle::~Miracle()
{
	m_sons.clear();
	for(int i=0;i<(int)m_tile.size();i++)
        m_tile[i].clear();
    m_tile.clear();
    m_image.clear();
}


void Miracle::Charger(std::string chemin)
{
    m_chemin=chemin;

    ifstream fichier;
    fichier.open(m_chemin.c_str(), ios::in);
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
                m_image.push_back(moteurGraphique.AjouterImage(cheminImage));
    		}
    		if(fichier.eof()){ char temp[1000]; sprintf(temp,"Erreur : Miracle \" %s \" Invalide",chemin.c_str());console.Ajouter(temp,1); caractere='$'; }
    	}while(caractere!='$');

    	do
    	{
    		fichier.get(caractere);
    		if(caractere=='*')
    		{
    		    string cheminDuSon;
                getline(fichier, cheminDuSon);

                m_sons.push_back(moteurSons.AjouterBuffer(cheminDuSon));
    		}
    		if(fichier.eof()){ char temp[1000]; sprintf(temp,"Erreur : Miracle \" %s \" Invalide",chemin.c_str());console.Ajouter(temp,1); caractere='$'; }
    	}while(caractere!='$');


    	do
    	{
    		fichier.get(caractere);
    		if(caractere=='*')
    		{
    		    m_effets.push_back(Effet ());
                do
                {
                    fichier.get(caractere);
                    switch(caractere)
                    {
                        case 't':
                            fichier>>m_effets.back().m_type;
                        break;

                        case 'l':
                            int buffer;
                            fichier>>buffer;
                            m_effets.back().m_lien.push_back(buffer);
                        break;

                        case 's':
                            fichier>>m_effets.back().m_sequence;
                        break;

                        case 'i':
                            fichier.get(caractere);
                            if(caractere=='a') { fichier>>m_effets.back().m_informations[0];}
                            if(caractere=='b') { fichier>>m_effets.back().m_informations[1];}
                            if(caractere=='c') { fichier>>m_effets.back().m_informations[2];}
                        break;
                    }
                    if(fichier.eof()){ char temp[1000]; sprintf(temp,"Erreur : Miracle \" %s \" Invalide",chemin.c_str());console.Ajouter(temp,1); caractere='$'; }

                }while(caractere!='$');

                fichier.get(caractere);
    		}

    		if(fichier.eof()){ char temp[1000]; sprintf(temp,"Erreur : Miracle \" %s \" Invalide",chemin.c_str());console.Ajouter(temp,1); caractere='$'; }
    	}while(caractere!='$');

    	do
    	{
    		fichier.get(caractere);
    		if(caractere=='*')
    		{

    		    m_tile.push_back(vector <Tile> () );

    		    do
                {
                    fichier.get(caractere);
                    if(caractere=='*')
                    {
                        coordonnee position={-1,-1,-1,-1},centre={-1,-1,-1,-1};
                        int animation=m_tile.size(),son=-1,image=0;
                        Lumiere lumiere;
                        lumiere.intensite=0;
                        bool collision=0,ombre=0,transparent=0;
                        char orientation=' ';
                        float tempsAnimation=0.075;

                        do
                        {
                            fichier.get(caractere);
                            switch (caractere)
                            {
                                case 'x': fichier>>position.x; break;
                                case 'y': fichier>>position.y; break;
                                case 'w': fichier>>position.w; break;
                                case 'h': fichier>>position.h; break;
                                case 'i': fichier>>image; break;
                                case 'c': fichier>>collision; break;
                                case 'a': fichier>>animation; break;
                                case 's': fichier>>son; break;
                                case 'o': fichier>>ombre; break;
                                case 't': fichier>>transparent; break;
                                case 'n': fichier>>tempsAnimation; break;

                                case 'e':
                                        fichier.get(caractere);
                                        if(caractere=='y')
                                            fichier>>centre.y;
                                        if(caractere=='x')
                                            fichier>>centre.x;
                                break;

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
                            if(fichier.eof()){ char temp[1000]; sprintf(temp,"Erreur : Miracle \" %s \" Invalide",chemin.c_str());console.Ajouter(temp,1); caractere='$'; }
                        }while(caractere!='$');
                        //AjouterTile(position,collision,animation,son,lumiere,ombre,orientation);
                        m_tile.back().push_back(Tile());
                        m_tile.back().back().setTile(position,image,collision,animation,son,lumiere,ombre,orientation,transparent,centre,tempsAnimation);

                        fichier.get(caractere);
                    }
                    if(fichier.eof()){ char temp[1000]; sprintf(temp,"Erreur : Miracle \" %s \" Invalide",chemin.c_str());console.Ajouter(temp,1); caractere='$'; }
                }while(caractere!='$');
                fichier.get(caractere);
    		}
    		if(fichier.eof()){ char temp[1000]; sprintf(temp,"Erreur : Miracle \" %s \" Invalide",chemin.c_str());console.Ajouter(temp,1); caractere='$'; }
    	}while(caractere!='$');
    }
    else
        console.Ajouter("Impossible d'ouvrir le fichier : "+chemin,1);
}


void Miracle::jouerSon(int numeroSon,coordonnee position,coordonnee positionHero)
{
    if(numeroSon>=0&&numeroSon<(int)m_sons.size())
    {
        coordonnee pos;
        pos.x=-position.x;
        pos.y=position.y;

        moteurSons.JouerSon(m_sons[numeroSon],pos,positionHero,0);
    }
}

