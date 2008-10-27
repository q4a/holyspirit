
#include "Objet.h"

#include "Globale.h"
#include <fstream>

using namespace std;

Objet::Objet()
{
    m_nom="Un objet merveilleux";
    m_rarete=0;
}

Objet::Objet(std::string nom, int rarete)
{
    m_nom=nom;
    m_rarete=rarete;
    m_image=0;
    m_chemin="";
}

std::string Objet::getNom(){return m_nom;}
int Objet::getRarete(){return m_rarete;}
int Objet::getImage(){return m_image;};
coordonnee Objet::getPosition(){return m_position;};
coordonnee Objet::getTaille(){return m_taille;};
coordonnee Objet::getPositionImage(){return m_positionImage;};

void Objet::setRarete(int chance){ m_rarete=chance; }
void Objet::setPosition(int x, int y){m_position.x=x,m_position.y=y;}



void Objet::Sauvegarder(std::ofstream *fichier)
{
    *fichier<<"* ";
    *fichier<<"r"<<m_rarete<<" ";

    *fichier<<"x"<<m_position.x<<" ";
    *fichier<<"y"<<m_position.y<<" ";

    *fichier<<"m"<<m_chemin<<" ";
    *fichier<<"$"<<endl;
}


void ModeleObjet::Charger(std::string chemin)
{
    console.Ajouter("",0);
	console.Ajouter("Chargement de l'objet : "+chemin,0);

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
                getline(fichier, m_nom);
            }
    		if(fichier.eof()){ char temp[1000]; sprintf(temp,"Erreur : Objet \" %s \" Invalide",chemin.c_str());console.Ajouter(temp,1); caractere='$'; }

    	}while(caractere!='$');

    	do
    	{
    		fichier.get(caractere);
    		if(caractere=='*')
            {
                string temp;
                getline(fichier, temp);

                m_image=moteurGraphique.AjouterImage(temp);
            }
    		if(fichier.eof()){ char temp[1000]; sprintf(temp,"Erreur : Objet \" %s \" Invalide",chemin.c_str());console.Ajouter(temp,1); caractere='$'; }

    	}while(caractere!='$');

    	do
    	{
    		fichier.get(caractere);
    		if(caractere=='*')
            {
                do
                {
                    fichier.get(caractere);
                    switch (caractere)
                    {
                        case 'x' : fichier>>m_positionImage.x; break;
                        case 'y' : fichier>>m_positionImage.y; break;
                        case 'w' : fichier>>m_positionImage.w; break;
                        case 'h' : fichier>>m_positionImage.h; break;
                    }

                    if(fichier.eof()){ char temp[1000]; sprintf(temp,"Erreur : Objet \" %s \" Invalide",chemin.c_str());console.Ajouter(temp,1); caractere='$'; }

                }while(caractere!='$');
                fichier.get(caractere);
            }
    		if(fichier.eof()){ char temp[1000]; sprintf(temp,"Erreur : Objet \" %s \" Invalide",chemin.c_str());console.Ajouter(temp,1); caractere='$'; }

    	}while(caractere!='$');

    	do
    	{
    		fichier.get(caractere);
    		if(caractere=='*')
            {
                do
                {
                    fichier.get(caractere);
                    switch (caractere)
                    {
                        case 'x' : fichier>>m_taille.x; break;
                        case 'y' : fichier>>m_taille.y; break;
                    }

                    if(fichier.eof()){ char temp[1000]; sprintf(temp,"Erreur : Objet \" %s \" Invalide",chemin.c_str());console.Ajouter(temp,1); caractere='$'; }

                }while(caractere!='$');
                fichier.get(caractere);
            }
    		if(fichier.eof()){ char temp[1000]; sprintf(temp,"Erreur : Objet \" %s \" Invalide",chemin.c_str());console.Ajouter(temp,1); caractere='$'; }

    	}while(caractere!='$');

    	do
    	{
    		fichier.get(caractere);
    		if(caractere=='*')
            {
                do
                {
                    fichier.get(caractere);
                    switch (caractere)
                    {
                        case 'r' : fichier>>m_rarete; break;
                    }

                    if(fichier.eof()){ char temp[1000]; sprintf(temp,"Erreur : Objet \" %s \" Invalide",chemin.c_str());console.Ajouter(temp,1); caractere='$'; }

                }while(caractere!='$');
                fichier.get(caractere);
            }
    		if(fichier.eof()){ char temp[1000]; sprintf(temp,"Erreur : Objet \" %s \" Invalide",chemin.c_str());console.Ajouter(temp,1); caractere='$'; }

    	}while(caractere!='$');
    }
    else
    console.Ajouter("Impossible d'ouvrir : "+chemin,1);

    m_position.x=0;
    m_position.y=0;
}


void ModeleObjet::setChanceTrouver(int chance){ m_chanceTrouver=chance; }

int ModeleObjet::getChanceTrouver(){return m_chanceTrouver;}
