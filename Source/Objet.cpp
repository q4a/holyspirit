
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
                        case 't' : fichier>>m_type; break;
                    }

                    if(fichier.eof()){ char temp[255]; sprintf(temp,"Erreur : Objet \" %s \" Invalide",chemin.c_str());console.Ajouter(temp,1); caractere='$'; }

                }while(caractere!='$');
                fichier.get(caractere);
            }
    		if(fichier.eof()){ char temp[255]; sprintf(temp,"Erreur : Objet \" %s \" Invalide",chemin.c_str());console.Ajouter(temp,1); caractere='$'; }

    	}while(caractere!='$');


    	m_chemin=chemin;

    	do
    	{
    		fichier.get(caractere);
    		if(caractere=='*')
            {
                getline(fichier, m_nom);
            }
    		if(fichier.eof()){ char temp[255]; sprintf(temp,"Erreur : Objet \" %s \" Invalide",chemin.c_str());console.Ajouter(temp,1); caractere='$'; }

    	}while(caractere!='$');

    	std::string description;

    	do
    	{
    		fichier.get(caractere);
    		if(caractere=='*')
            {
                getline(fichier, description);
            }
    		if(fichier.eof()){ char temp[255]; sprintf(temp,"Erreur : Objet \" %s \" Invalide",chemin.c_str());console.Ajouter(temp,1); caractere='$'; }

    	}while(caractere!='$');

        std::string::size_type stTemp = description.find('\\');

        while(stTemp !=  std::string::npos)
        {
            m_description.push_back(description.substr(0, stTemp)+'\n');
            description = description.substr(stTemp + 1);
            stTemp = description.find('\\');;
        }
        m_description.push_back(description.substr(0, stTemp)+'\n');


    	do
    	{
    		fichier.get(caractere);
    		if(caractere=='*')
            {
                string temp;
                getline(fichier, temp);

                m_image=moteurGraphique.AjouterImage(temp);
            }
    		if(fichier.eof()){ char temp[255]; sprintf(temp,"Erreur : Objet \" %s \" Invalide",chemin.c_str());console.Ajouter(temp,1); caractere='$'; }

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

                    if(fichier.eof()){ char temp[255]; sprintf(temp,"Erreur : Objet \" %s \" Invalide",chemin.c_str());console.Ajouter(temp,1); caractere='$'; }

                }while(caractere!='$');
                fichier.get(caractere);
            }
    		if(fichier.eof()){ char temp[255]; sprintf(temp,"Erreur : Objet \" %s \" Invalide",chemin.c_str());console.Ajouter(temp,1); caractere='$'; }

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

                    if(fichier.eof()){ char temp[255]; sprintf(temp,"Erreur : Objet \" %s \" Invalide",chemin.c_str());console.Ajouter(temp,1); caractere='$'; }

                }while(caractere!='$');
                fichier.get(caractere);
            }
    		if(fichier.eof()){ char temp[255]; sprintf(temp,"Erreur : Objet \" %s \" Invalide",chemin.c_str());console.Ajouter(temp,1); caractere='$'; }

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

                    if(fichier.eof()){ char temp[255]; sprintf(temp,"Erreur : Objet \" %s \" Invalide",chemin.c_str());console.Ajouter(temp,1); caractere='$'; }

                }while(caractere!='$');
                fichier.get(caractere);
            }
    		if(fichier.eof()){ char temp[255]; sprintf(temp,"Erreur : Objet \" %s \" Invalide",chemin.c_str());console.Ajouter(temp,1); caractere='$'; }

    	}while(caractere!='$');

        this->ChargerCaracteristiques(&fichier);
    }
    else
    console.Ajouter("Impossible d'ouvrir : "+chemin,1);


    m_position.x=0;
    m_position.y=0;
}

void Objet::ChargerCaracteristiques(std::ifstream *fichier)
{
    if(m_type==1)
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

                    if(fichier->eof()){ char temp[255]; sprintf(temp,"Erreur : Objet \" %s \" Invalide",m_chemin.c_str());console.Ajouter(temp,1); caractere='$'; }

                }while(caractere!='$');
                fichier->get(caractere);
            }
            if(fichier->eof()){ char temp[255]; sprintf(temp,"Erreur : Objet \" %s \" Invalide",m_chemin.c_str());console.Ajouter(temp,1); caractere='$'; }

        }while(caractere!='$');
    }
}


sf::String Objet::AjouterCaracteristiqueAfficher(coordonnee position,coordonnee *decalage,coordonnee *tailleCadran, char *chaine)
{
    sf::String string;

    string.SetSize(14.f);
    string.SetText(chaine);
    string.SetY(position.y+decalage->y+10);
    string.SetX(position.x+decalage->x+10-((int)string.GetRect().Right-(int)string.GetRect().Left));

    if(tailleCadran->x<((int)string.GetRect().Right-(int)string.GetRect().Left))
        tailleCadran->x=((int)string.GetRect().Right-(int)string.GetRect().Left);

    decalage->y+=(int)string.GetRect().Bottom-(int)string.GetRect().Top+5;

    return string;
}


void Objet::AfficherCaracteristiques(sf::RenderWindow *ecran,coordonnee position)
{
    std::vector <sf::String> temp;

    char chaine[255];

    sf::Sprite sprite;
    sf::String string;

    coordonnee tailleCadran={0,0,0,0},decalage={-10,0,0,0};


    sprintf(chaine,"%s",m_nom.c_str());
    temp.push_back(AjouterCaracteristiqueAfficher(position,&decalage,&tailleCadran,chaine));

    if(m_rarete==NORMAL)
        temp[0].SetColor(sf::Color(224,224,224));
    if(m_rarete==BONNEFACTURE)
        temp[0].SetColor(sf::Color(128,0,128));
    if(m_rarete==BENI)
        temp[0].SetColor(sf::Color(0,64,128));
    if(m_rarete==SACRE)
        temp[0].SetColor(sf::Color(255,255,128));
    if(m_rarete==SANCTIFIE)
        temp[0].SetColor(sf::Color(128,255,255));
    if(m_rarete==DIVIN)
        temp[0].SetColor(sf::Color(255,164,32));
    if(m_rarete==INFERNAL)
        temp[0].SetColor(sf::Color(224,0,0));
    if(m_rarete==CRAFT)
        temp[0].SetColor(sf::Color(128,64,0));

    for(int i=0;i<m_description.size();i++)
    {
        sprintf(chaine,"%s",m_description[i].c_str());
        temp.push_back(AjouterCaracteristiqueAfficher(position,&decalage,&tailleCadran,chaine));
    }

    sprintf(chaine,"");
    temp.push_back(AjouterCaracteristiqueAfficher(position,&decalage,&tailleCadran,chaine));

    if(m_type==1)
    {
        sprintf(chaine,"Dégats : %i - %i",m_degatsMin,m_degatsMax);
        temp.push_back(AjouterCaracteristiqueAfficher(position,&decalage,&tailleCadran,chaine));
    }


    for(int i=0;i<temp.size();i++)
    {
        temp[i].SetX(position.x+(tailleCadran.x/2-((int)temp[i].GetRect().Right-(int)temp[i].GetRect().Left)/2)-tailleCadran.x);
        moteurGraphique.AjouterTexte(&temp[i],18);
    }

    tailleCadran.y=decalage.y;

    tailleCadran.y+=20;
    tailleCadran.x+=20;

    sprite.SetImage(*moteurGraphique.getImage(0));
    sprite.SetColor(sf::Color(0,0,0,224));
    sprite.SetY(position.y);
    sprite.SetX(position.x-tailleCadran.x+10);
    sprite.Resize(tailleCadran.x,tailleCadran.y);
    moteurGraphique.AjouterCommande(&sprite,17,0);

}






void Objet::setChanceTrouver(int chance){ m_chanceTrouver=chance; }

int Objet::getChanceTrouver(){return m_chanceTrouver;}
