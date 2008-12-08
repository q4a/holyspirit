
#include "Objet.h"

#include "Globale.h"
#include <fstream>

using namespace std;

Objet::Objet()
{
    m_nom="Un objet merveilleux";
    m_chemin="";
    m_rarete=0;
}

Objet::Objet(std::string nom, int rarete)
{
    m_nom=nom;
    m_rarete=rarete;
    m_image=0;
    m_chemin="";
}

Objet::~Objet()
{
    m_chemin.erase();
    m_nom.erase();
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


void Objet::Charger(std::string chemin)
{
    console.Ajouter("",0);
	console.Ajouter("Chargement de l'objet : "+chemin,0);


	ifstream fichier;
    fichier.open(chemin.c_str(), ios::in);
    if(fichier)
    {
        int type=0;
        char caractere;

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
                        case 't' : fichier>>type; break;
                    }

                    if(fichier.eof()){ char temp[1000]; sprintf(temp,"Erreur : Objet \" %s \" Invalide",chemin.c_str());console.Ajouter(temp,1); caractere='$'; }

                }while(caractere!='$');
                fichier.get(caractere);
            }
    		if(fichier.eof()){ char temp[1000]; sprintf(temp,"Erreur : Objet \" %s \" Invalide",chemin.c_str());console.Ajouter(temp,1); caractere='$'; }

    	}while(caractere!='$');

        if(type==1)
    	{
            Arme temp;
    	    *this = temp;
    	}

    	m_chemin=chemin;

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

    	ChargerCaracteristiques(&fichier);
    }
    else
    console.Ajouter("Impossible d'ouvrir : "+chemin,1);


    m_position.x=0;
    m_position.y=0;
}

void Objet::ChargerCaracteristiques(std::ifstream *fichier){}

void Arme::ChargerCaracteristiques(std::ifstream *fichier)
{
    char caractere;
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
                    case 'd' : fichier->get(caractere); if(caractere=='i') *fichier>>m_degatsMin; else if(caractere=='a') *fichier>>m_degatsMax;  break;
                }

                if(fichier->eof()){ char temp[1000]; sprintf(temp,"Erreur : Objet \" %s \" Invalide",m_chemin.c_str());console.Ajouter(temp,1); caractere='$'; }

            }while(caractere!='$');
            fichier->get(caractere);
        }
        if(fichier->eof()){ char temp[1000]; sprintf(temp,"Erreur : Objet \" %s \" Invalide",m_chemin.c_str());console.Ajouter(temp,1); caractere='$'; }

    }while(caractere!='$');
}


void Objet::AfficherCaracteristiques(sf::RenderWindow *ecran,coordonnee position)
{
    char chaine[255];

    sf::Sprite sprite;
    sf::String string;

    coordonnee tailleCadran={0,0,0,0},decalage={0,0,0,0};

    string.SetSize(16.f);
    sprintf(chaine,"%s",m_nom.c_str());
    string.SetText(chaine);
    string.SetY(position.y+decalage.y+10);
    string.SetX(position.x+decalage.x+10);

    decalage.y+=(int)string.GetRect().Bottom-(int)string.GetRect().Top+10;

    moteurGraphique.AjouterTexte(&string,17);
}

void Arme::AfficherCaracteristiques(sf::RenderWindow *ecran,coordonnee position)
{
    Objet::AfficherCaracteristiques(ecran,position);

    char chaine[255];

    sf::Sprite sprite;
    sf::String string;

    coordonnee tailleCadran={0,0,0,0},decalage={0,32,0,0};

    string.SetSize(16.f);

   // decalage.y+=string.GetRect().Bottom-string.GetRect().Top+10;

    moteurGraphique.AjouterTexte(&string);

    sprintf(chaine,"Dégats : %i - %i",m_degatsMin,m_degatsMax);
    string.SetText(chaine);
    string.SetY(position.y+decalage.y+10);
    string.SetX(position.x+decalage.x+10);

    decalage.y+=(int)string.GetRect().Bottom-(int)string.GetRect().Top+10;

    moteurGraphique.AjouterTexte(&string);
}




void Objet::setChanceTrouver(int chance){ m_chanceTrouver=chance; }

int Objet::getChanceTrouver(){return m_chanceTrouver;}
