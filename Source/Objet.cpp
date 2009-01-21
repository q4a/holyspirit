
#include "Objet.h"

#include "Globale.h"
#include <fstream>

using namespace std;
using namespace sf;


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
    m_capaciteBenediction=0;
}

Objet::~Objet()
{
    m_description.clear();
    m_benedictions.clear();
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
    char caractere,espace='_';
    caractere='*';
    fichier->write((char*)&caractere, sizeof(char));
    //fichier->write((char*)&espace, sizeof(char));

    caractere='e';
    fichier->write((char*)&caractere, sizeof(char));
    fichier->write((char*)&m_equipe, sizeof(int));
    //fichier->write((char*)&espace, sizeof(char));


    caractere='r';
    fichier->write((char*)&caractere, sizeof(char));
    fichier->write((char*)&m_rarete, sizeof(int));
    //fichier->write((char*)&espace, sizeof(char));

    caractere='x';
    fichier->write((char*)&caractere, sizeof(char));
    fichier->write((char*)&m_position.x, sizeof(int));
    //fichier->write((char*)&espace, sizeof(char));
    caractere='y';
    fichier->write((char*)&caractere, sizeof(char));
    fichier->write((char*)&m_position.y, sizeof(int));
    //fichier->write((char*)&espace, sizeof(char));

    caractere='d';
    fichier->write((char*)&caractere, sizeof(char));
    caractere='i';
    fichier->write((char*)&caractere, sizeof(char));
    fichier->write((char*)&m_degatsMin, sizeof(int));
   // fichier->write((char*)&espace, sizeof(char));

    caractere='d';
    fichier->write((char*)&caractere, sizeof(char));
    caractere='a';
    fichier->write((char*)&caractere, sizeof(char));
    fichier->write((char*)&m_degatsMax, sizeof(int));
    //fichier->write((char*)&espace, sizeof(char));

    caractere='a';
    fichier->write((char*)&caractere, sizeof(char));
    fichier->write((char*)&m_armure, sizeof(int));
    //fichier->write((char*)&espace, sizeof(char));


    caractere='l';
    fichier->write((char*)&caractere, sizeof(char));
    caractere='r';
    fichier->write((char*)&caractere, sizeof(char));
    fichier->write((char*)&m_color.r, sizeof(Uint8));
    //fichier->write((char*)&espace, sizeof(char));

    caractere='l';
    fichier->write((char*)&caractere, sizeof(char));
    caractere='g';
    fichier->write((char*)&caractere, sizeof(char));
    fichier->write((char*)&m_color.g, sizeof(Uint8));
    //fichier->write((char*)&espace, sizeof(char));

    caractere='l';
    fichier->write((char*)&caractere, sizeof(char));
    caractere='b';
    fichier->write((char*)&caractere, sizeof(char));
    fichier->write((char*)&m_color.b, sizeof(Uint8));
   // fichier->write((char*)&espace, sizeof(char));

    caractere='m';
    fichier->write((char*)&caractere, sizeof(char));
    int n=m_chemin.size()+1;
    fichier->write((char *)&n, sizeof(int));
    fichier->write((char*)m_chemin.c_str(), n);
   // fichier->write((char*)&espace, sizeof(char));

    for(int i=0;i<m_benedictions.size();i++)
    {
        caractere='b';
        fichier->write((char*)&caractere, sizeof(char));
        fichier->write((char*)&m_benedictions[i].type, sizeof(int));
        //fichier->write((char*)&espace, sizeof(char));

        caractere='i';
        fichier->write((char*)&caractere, sizeof(char));
        caractere='1';
        fichier->write((char*)&caractere, sizeof(char));
        fichier->write((char*)&m_benedictions[i].info1, sizeof(int));
        //fichier->write((char*)&espace, sizeof(char));

        caractere='i';
        fichier->write((char*)&caractere, sizeof(char));
        caractere='2';
        fichier->write((char*)&caractere, sizeof(char));
        fichier->write((char*)&m_benedictions[i].info2, sizeof(int));
        //fichier->write((char*)&espace, sizeof(char));

        caractere='$';
         //fichier->write((char*)&espace, sizeof(char));
        fichier->write((char*)&caractere, sizeof(char));
        fichier->write((char*)&espace, sizeof(char));
    }

    caractere='$';
   // fichier->write((char*)&espace, sizeof(char));
    fichier->write((char*)&caractere, sizeof(char));
    fichier->write((char*)&espace, sizeof(char));

    if(configuration.debug)
            console.Ajouter("/Sauvegarde de "+m_chemin);
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
                        case 'i' :  fichier>>temp; m_emplacementImpossible.push_back(temp); break;

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
                        case 'b' : fichier>>m_capaciteBenediction; break;
                    }

                    if(fichier.eof()){ char temp[255]; sprintf(temp,"Erreur : Objet \" %s \" Invalide",chemin.c_str());console.Ajouter(temp,1); caractere='$'; }

                }while(caractere!='$');
                fichier.get(caractere);
            }
    		if(fichier.eof()){ char temp[255]; sprintf(temp,"Erreur : Objet \" %s \" Invalide",chemin.c_str());console.Ajouter(temp,1); caractere='$'; }

    	}while(caractere!='$');

        this->ChargerCaracteristiques(&fichier);




        do
    	{

    		fichier.get(caractere);
    		if(caractere=='*')
            {
                int b=-1,ia=-1,ib=-1;
                do
                {
                    fichier.get(caractere);
                    switch (caractere)
                    {
                        case 'b' : fichier>>b; break;
                        case 'i' :
                            fichier.get(caractere);
                            if(caractere=='1')
                                fichier>>ia;
                            if(caractere=='2')
                                fichier>>ib;
                        break;
                    }

                    if(fichier.eof()){ char temp[255]; sprintf(temp,"Erreur : Objet \" %s \" Invalide",chemin.c_str());console.Ajouter(temp,1); caractere='$'; }

                }while(caractere!='$');

                m_benedictions.push_back(benediction ());
                m_benedictions[m_benedictions.size()-1].type=b;
                m_benedictions[m_benedictions.size()-1].info1=ia;
                m_benedictions[m_benedictions.size()-1].info2=ib;

                fichier.get(caractere);
            }

    		if(fichier.eof()){ caractere='$'; }

    	}while(caractere!='$');
    }
    else
    console.Ajouter("Impossible d'ouvrir : "+chemin,1);


    m_position.x=0;
    m_position.y=0;
}

void Objet::Generer(int bonus)
{
    m_armure=(rand() % (aa - ai + 1)) + ai;
    m_degatsMin=(rand() % (dia - dii + 1)) + dii;
    m_degatsMax=(rand() % (daa - dai + 1)) + dai;

    m_color.r=255;
    m_color.g=255;
    m_color.b=255;
    m_color.a=255;

    if(m_rarete==0)
    {
         int nbrBene=0;
        int random=rand()%10000 / bonus;
        if(random<=3000)
        {
            m_rarete=BONNEFACTURE;
            m_armure*=1;
            m_degatsMin*=1;
            m_degatsMax*=1;
            nbrBene=1;

            m_color.r=255-rand()%64;
            m_color.g=255-rand()%64;
            m_color.b=255-rand()%64;
        }
        if(random<=300)
        {
            m_rarete=BENI;
            m_armure*=1;
            m_degatsMin*=1;
            m_degatsMax*=1;
            nbrBene=rand()%(5-2)+2;

            m_color.r=255-rand()%128;
            m_color.g=255-rand()%128;
            m_color.b=255-rand()%128;
        }
        if(random<=50)
        {
            m_rarete=SACRE;
            m_armure*=1;
            m_degatsMin*=1;
            m_degatsMax*=1;
            nbrBene=rand()%(10-5)+5;

            m_color.r=255-rand()%192;
            m_color.g=255-rand()%192;
            m_color.b=255-rand()%192;
        }
        if(random<10)
        {
            m_rarete=SANCTIFIE;
            m_armure*=1;
            m_degatsMin*=1;
            m_degatsMax*=1;
            nbrBene=rand()%(15-10)+10;

            m_color.r=255-rand()%255;
            m_color.g=255-rand()%255;
            m_color.b=255-rand()%255;
        }
        /*if(random<3)
        {
            m_rarete=DIVIN;
            m_armure*=1;
            m_degatsMin*=1;
            m_degatsMax*=1;
            nbrBene=rand()%(30-15)+15;

            m_color.r=255-rand()%255;
            m_color.g=255-rand()%255;
            m_color.b=255-rand()%255;
        }
        if(random==1)
        {
            m_rarete=INFERNAL;
            m_armure*=1;
            m_degatsMin*=1;
            m_degatsMax*=1;
            nbrBene=rand()%(50-30)+30;

            m_color.r=255-rand()%255;
            m_color.g=255-rand()%255;
            m_color.b=255-rand()%255;
        }*/

        for(int i=0;i<nbrBene;i++)
        {
            bool ajouter=true;

            benediction temp;
            temp.type=rand()%NOMBRE_BENEDICTION;

            if(temp.type==VIE_SUPP||temp.type==FOI_SUPP)
                temp.info1=rand()%(m_capaciteBenediction*20 - (int)(m_capaciteBenediction*5))+m_capaciteBenediction*5;
            else if(temp.type==EFFICACITE_ACCRUE)
                temp.info1=(int)(rand()%(m_capaciteBenediction*10 - (int)((float)m_capaciteBenediction*2.5))+(float)m_capaciteBenediction*2.5);
            else
                temp.info1=(int)(rand()%(m_capaciteBenediction*2 - (int)((float)m_capaciteBenediction*0.5))+(float)m_capaciteBenediction*0.5);

            for(int j=0;j<m_benedictions.size();j++)
                if(m_benedictions[j].type==temp.type)
                    m_benedictions[j].info1+=temp.info1,ajouter=false;

            if(ajouter)
            {
                m_benedictions.push_back(benediction ());
                m_benedictions[m_benedictions.size()-1]=temp;
                m_benedictions[m_benedictions.size()-1].info2=0;
            }
        }

        for(int i=0;i<m_benedictions.size();i++)
            if(m_benedictions[i].type==EFFICACITE_ACCRUE)
            {
                m_armure+=(int)((float)m_armure*(float)m_benedictions[i].info1*0.01);
                m_degatsMin+=(int)(m_degatsMin*m_benedictions[i].info1*0.01);
                m_degatsMax+=(int)(m_degatsMax*m_benedictions[i].info1*0.01);
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

    string.SetSize(14.f*configuration.Resolution.h/600);
    string.SetText(chaine);

    if(position.x<400)
        string.SetX((position.x-10+((int)string.GetRect().Right-(int)string.GetRect().Left)));
    else
        string.SetX((position.x+decalage->x+10-((int)string.GetRect().Right-(int)string.GetRect().Left)));


    string.SetY((position.y+decalage->y+10));

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

    float multiplieurEfficacite=100;

    for(int i=0;i<m_benedictions.size();i++)
        if(m_benedictions[i].type==EFFICACITE_ACCRUE)
            multiplieurEfficacite+=m_benedictions[i].info1;

    switch(m_type)
    {
        case ARME:
            sprintf(chaine,"Dégats : %i - %i",(int)(m_degatsMin*multiplieurEfficacite/100),(int)(m_degatsMax*multiplieurEfficacite/100));
            temp.push_back(AjouterCaracteristiqueAfficher(position,&decalage,&tailleCadran,chaine));
        break;
        case ARMURE:
            sprintf(chaine,"Armure : %i",(int)(m_armure*multiplieurEfficacite/100));
            temp.push_back(AjouterCaracteristiqueAfficher(position,&decalage,&tailleCadran,chaine));
        break;
    }

    sprintf(chaine,"");
    temp.push_back(AjouterCaracteristiqueAfficher(position,&decalage,&tailleCadran,chaine));

    for(int i=0;i<m_benedictions.size();i++)
    {
        sprintf(chaine,"%s %i",configuration.text_benedictions[m_benedictions[i].type].c_str(),m_benedictions[i].info1);
        temp.push_back(AjouterCaracteristiqueAfficher(position,&decalage,&tailleCadran,chaine));
    }

    for(int i=0;i<temp.size();i++)
    {
        if(position.x<400)
            temp[i].SetX(position.x+(tailleCadran.x/2-((int)temp[i].GetRect().Right-(int)temp[i].GetRect().Left)/2));
        else
            temp[i].SetX(position.x+(tailleCadran.x/2-((int)temp[i].GetRect().Right-(int)temp[i].GetRect().Left)/2)-tailleCadran.x);
        moteurGraphique.AjouterTexte(&temp[i],19);
    }

    tailleCadran.y=decalage.y;

    tailleCadran.y+=20;
    tailleCadran.x+=20;

    sprite.SetImage(*moteurGraphique.getImage(0));
    sprite.SetColor(sf::Color(0,0,0,224));
    sprite.SetY(position.y);
    if(position.x>400)
        sprite.SetX(position.x-tailleCadran.x+10);
    else
        sprite.SetX(position.x-10);
    sprite.Resize(tailleCadran.x,tailleCadran.y);
    moteurGraphique.AjouterCommande(&sprite,18,0);

    temp.clear();
}






void Objet::setChanceTrouver(int chance){ m_chanceTrouver=chance; }

int Objet::getChanceTrouver(){return m_chanceTrouver;}
