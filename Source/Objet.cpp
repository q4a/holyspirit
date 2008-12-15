
#include "Objet.h"

#include "Globale.h"
#include <fstream>

using namespace std;

Objet::Objet()
{
    m_nom="Un objet merveilleux";
    m_chemin="";
    m_rarete=0;
    m_equipe=0;

    m_armure=0;
    m_degatsMin=0;
    m_degatsMax=0;

    ai=0,aa=0,dii=0,dia=0,dai=0,daa=0;
}

Objet::Objet(std::string nom, int rarete)
{
    m_nom=nom;
    m_rarete=rarete;
    m_image=0;
    m_chemin="";
    m_equipe=0;
}

Objet::~Objet()
{
    m_chemin.erase();
    m_nom.erase();
    m_description.clear();
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
    *fichier<<"e"<<m_equipe<<" ";

    *fichier<<"r"<<m_rarete<<" ";

    *fichier<<"x"<<m_position.x<<" ";
    *fichier<<"y"<<m_position.y<<" ";

    *fichier<<"di"<<m_degatsMin<<" ";
    *fichier<<"da"<<m_degatsMax<<" ";
    *fichier<<"a"<<m_armure<<" ";

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
                        case 'e' : int temp; fichier>>temp; m_emplacement.push_back(temp); break;
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

void Objet::Generer()
{
    m_armure=(rand() % (aa - ai + 1)) + ai;
    m_degatsMin=(rand() % (dia - dii + 1)) + dii;
    m_degatsMax=(rand() % (daa - dai + 1)) + dai;

    if(m_rarete==0)
    {
        int random=rand()%10000;
        if(random<=3000)
        {
            m_rarete=BONNEFACTURE;
            m_armure*=1.25;
            m_degatsMin*=1.25;
            m_degatsMax*=1.25;
        }
        if(random<=300)
        {
            m_rarete=BENI;
            m_armure*=1.5;
            m_degatsMin*=1.5;
            m_degatsMax*=1.5;
        }
        if(random<=50)
        {
            m_rarete=SACRE;
            m_armure*=2;
            m_degatsMin*=2;
            m_degatsMax*=2;
        }
        if(random<20)
        {
            m_rarete=SANCTIFIE;
            m_armure*=5;
            m_degatsMin*=5;
            m_degatsMax*=5;
        }
        if(random<3)
        {
            m_rarete=DIVIN;
            m_armure*=10;
            m_degatsMin*=10;
            m_degatsMax*=10;
        }
        if(random==1)
        {
            m_rarete=INFERNAL;
            m_armure*=20;
            m_degatsMin*=20;
            m_degatsMax*=20;
        }
    }
}

void Objet::ChargerCaracteristiques(std::ifstream *fichier)
{
    if(m_type==ARME)
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
                        case 'd' :
                            fichier->get(caractere);
                            if(caractere=='i')
                            {
                                fichier->get(caractere);
                                if(caractere=='i')
                                    *fichier>>dii;
                                if(caractere=='a')
                                    *fichier>>dia;
                            }
                            else if(caractere=='a')
                            {
                                fichier->get(caractere);
                                if(caractere=='i')
                                    *fichier>>dai;
                                if(caractere=='a')
                                    *fichier>>daa;
                            }
                        break;
                    }

                    if(fichier->eof()){ char temp[255]; sprintf(temp,"Erreur : Objet \" %s \" Invalide",m_chemin.c_str());console.Ajouter(temp,1); caractere='$'; }

                }while(caractere!='$');
                fichier->get(caractere);



            }
            if(fichier->eof()){ char temp[255]; sprintf(temp,"Erreur : Objet \" %s \" Invalide",m_chemin.c_str());console.Ajouter(temp,1); caractere='$'; }

        }while(caractere!='$');
    }

    if(m_type==ARMURE)
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
                        case 'a' :

                            fichier->get(caractere);
                            if(caractere=='i')
                                *fichier>>ai;
                            if(caractere=='a')
                                *fichier>>aa;

                          break;
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

    switch(m_type)
    {
        case ARME:
            sprintf(chaine,"Dégats : %i - %i",m_degatsMin,m_degatsMax);
            temp.push_back(AjouterCaracteristiqueAfficher(position,&decalage,&tailleCadran,chaine));
        break;
        case ARMURE:
            sprintf(chaine,"Armure : %i",m_armure);
            temp.push_back(AjouterCaracteristiqueAfficher(position,&decalage,&tailleCadran,chaine));
        break;
    }


    for(int i=0;i<temp.size();i++)
    {
        temp[i].SetX(position.x+(tailleCadran.x/2-((int)temp[i].GetRect().Right-(int)temp[i].GetRect().Left)/2)-tailleCadran.x);
        moteurGraphique.AjouterTexte(&temp[i],19);
    }

    tailleCadran.y=decalage.y;

    tailleCadran.y+=20;
    tailleCadran.x+=20;

    sprite.SetImage(*moteurGraphique.getImage(0));
    sprite.SetColor(sf::Color(0,0,0,224));
    sprite.SetY(position.y);
    sprite.SetX(position.x-tailleCadran.x+10);
    sprite.Resize(tailleCadran.x,tailleCadran.y);
    moteurGraphique.AjouterCommande(&sprite,18,0);

}






void Objet::setChanceTrouver(int chance){ m_chanceTrouver=chance; }

int Objet::getChanceTrouver(){return m_chanceTrouver;}
