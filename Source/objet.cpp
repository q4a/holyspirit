

/*Copyright (C) 2009 Naisse Grégoire

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License along
with this program; if not, write to the Free Software Foundation, Inc.,
51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.*/



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
    m_equipe=-1;

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
    m_equipe=-1;
    m_capaciteBenediction=0;
}

Objet::~Objet()
{
    m_description.clear();
    m_benedictions.clear();
    m_emplacement.clear();
    m_emplacementImpossible.clear();
    m_emplacementImageHero.clear();
    m_cheminImageHero.clear();
}

Objet Objet::operator=(const Objet &objet)
{
	m_type=objet.m_type;

	m_equipe=objet.m_equipe;
	m_emplacement=objet.m_emplacement;
	m_emplacementImpossible=objet.m_emplacementImpossible;
	m_degatsMin=objet.m_degatsMin;
	m_degatsMax=objet.m_degatsMax;
	m_armure=objet.m_armure;
	m_capaciteBenediction=objet.m_capaciteBenediction;
	m_emplacementImageHero=objet.m_emplacementImageHero;
	m_cheminImageHero=objet.m_cheminImageHero;
	m_benedictions=objet.m_benedictions;
	m_color=objet.m_color;
	m_nom=objet.m_nom;
	m_chemin=objet.m_chemin;
	m_description=objet.m_description;
	m_rarete=objet.m_rarete;
	m_image=objet.m_image;
	m_chanceTrouver=objet.m_chanceTrouver;
	m_positionImage=objet.m_positionImage;
	m_taille=objet.m_taille;
	m_position=objet.m_position;
	ai=objet.ai;
	aa=objet.aa;
	dii=objet.dii;
	dia=objet.dia;
	dai=objet.dai;
	daa=objet.daa;

    return *this;
}

std::string Objet::getChemin(){return m_chemin;}
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
    fichier->write(m_chemin.c_str(), n);
   // fichier->write((char*)&espace, sizeof(char));

    for(int i=0;i<(int)m_benedictions.size();i++)
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

    if(configuration->debug)
            console->Ajouter("/Sauvegarde de "+m_chemin);
}

void Objet::SauvegarderTexte(std::ofstream *fichier)
{
    *fichier<<" o ";
    *fichier<<" e"<<m_equipe;
    *fichier<<" r"<<m_rarete;

    *fichier<<" di"<<m_degatsMin;
    *fichier<<" da"<<m_degatsMax;
    *fichier<<" a"<<m_armure;

    *fichier<<" lr"<<m_color.r;
    *fichier<<" lg"<<m_color.g;
    *fichier<<" lb"<<m_color.b;

    *fichier<<" m"<<m_chemin;

    for(int i=0;i<(int)m_benedictions.size();i++)
    {
        *fichier<<" b"<<m_benedictions[i].type;

        *fichier<<" i1"<<m_benedictions[i].info1;
        *fichier<<" i2"<<m_benedictions[i].info2;

        *fichier<<" $ ";
    }

    *fichier<<" $ ";

    if(configuration->debug)
            console->Ajouter("/Sauvegarde de "+m_chemin);
}

void Objet::ChargerTexte(std::ifstream *fichier)
{
    m_rarete=0,m_equipe=-1;
    m_degatsMin=0,m_degatsMax=0,m_armure=0;
    m_position.y=0;
    m_position.x=0;

    char caractere;

    do
    {
        fichier->get(caractere);
        if(caractere=='e')
            *fichier>>m_equipe;

        else if(caractere=='r')
            *fichier>>m_rarete;

        else if(caractere=='x')
            *fichier>>m_position.x;
        else if(caractere=='y')
            *fichier>>m_position.y;

        else if(caractere=='d')
        {
            fichier->get(caractere);
            if(caractere=='i')
                *fichier>>m_degatsMin;
            else if(caractere=='a')
                *fichier>>m_degatsMax;
        }

        else if(caractere=='a')
            *fichier>>m_armure;

        else if(caractere=='l')
        {
            fichier->get(caractere);
            if(caractere=='r')
                *fichier>>m_color.r;
            else if(caractere=='g')
                *fichier>>m_color.g;
            else if(caractere=='b')
                *fichier>>m_color.b;
        }
        else if(caractere=='m')
            *fichier>>m_chemin;
        else if(caractere=='b')
        {
            int type=0,info1=0,info2=0;

            *fichier>>type;

            do
            {
                fichier->get(caractere);
                if(caractere=='i')
                {
                    fichier->get(caractere);
                    if(caractere=='1')
                        *fichier>>info1;
                    if(caractere=='2')
                        *fichier>>info2;
                }
            }while(caractere!='$');

            m_benedictions.push_back(benediction ());
            m_benedictions.back().type=type;
            m_benedictions.back().info1=info1;
            m_benedictions.back().info2=info2;
            fichier->get(caractere);
        }

        if(fichier->eof()){throw "Impossible de charger l'objet";}
    }while(caractere!='$');
}


void Objet::Charger(std::string chemin)
{
    console->Ajouter("",0);
	console->Ajouter("Chargement de l'objet : "+chemin,0);

	cDAT reader;
	reader.Read(configuration->chemin_items);

	ifstream *fichier=reader.GetInfos(chemin);
    //fichier.open(chemin.c_str(), ios::in);
    if(fichier)
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
                        case 't' : *fichier>>m_type; break;
                        case 'e' : int temp; *fichier>>temp; m_emplacement.push_back(temp); break;
                        case 'i' :  *fichier>>temp; m_emplacementImpossible.push_back(temp); break;
                        case 'c' :  *fichier>>temp; m_IDClasse.push_back(temp); break;
                    }

                    if(fichier->eof()){ char temp[255]; sprintf(temp,"Erreur : Objet \" %s \" Invalide",chemin.c_str());console->Ajouter(temp,1); caractere='$'; }

                }while(caractere!='$');
                fichier->get(caractere);
            }
    		if(fichier->eof()){ char temp[255]; sprintf(temp,"Erreur : Objet \" %s \" Invalide",chemin.c_str());console->Ajouter(temp,1); caractere='$'; }

    	}while(caractere!='$');


    	m_chemin=chemin;

    	do
    	{
    		fichier->get(caractere);
    		if(caractere=='*')
            {
                *fichier>>m_nom;
                for(int i=0;i<(int)m_nom.size();i++)
                    if(m_nom[i]=='_')
                        m_nom[i]=' ';
                //getline(fichier, m_nom);
            }
    		if(fichier->eof()){ char temp[255]; sprintf(temp,"Erreur : Objet \" %s \" Invalide",chemin.c_str());console->Ajouter(temp,1); caractere='$'; }

    	}while(caractere!='$');

    	std::string description;

    	do
    	{
    		fichier->get(caractere);
    		if(caractere=='*')
            {
                *fichier>>description;
                for(int i=0;i<(int)description.size();i++)
                    if(description[i]=='_')
                        description[i]=' ';
                //getline(fichier, description);
            }
    		if(fichier->eof()){ char temp[255]; sprintf(temp,"Erreur : Objet \" %s \" Invalide",chemin.c_str());console->Ajouter(temp,1); caractere='$'; }

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
    		fichier->get(caractere);
    		if(caractere=='*')
            {
                string temp;
                //getline(fichier, temp);
                *fichier>>temp;

                m_image=moteurGraphique->AjouterImage(temp,-1);
            }
    		if(fichier->eof()){ char temp[255]; sprintf(temp,"Erreur : Objet \" %s \" Invalide",chemin.c_str());console->Ajouter(temp,1); caractere='$'; }

    	}while(caractere!='$');

    	//m_cheminImageHero="";
        //m_emplacementImageHero=-1;

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
                        case 'e' : int temp; *fichier>>temp; m_emplacementImageHero.push_back(temp); break;
                        case 'm' : string temp2; *fichier>>temp2; m_cheminImageHero.push_back(temp2); break;
                    }

                    if(fichier->eof()){ char temp[255]; sprintf(temp,"Erreur : Objet \" %s \" Invalide",chemin.c_str());console->Ajouter(temp,1); caractere='$'; }

                }while(caractere!='$');
                fichier->get(caractere);
            }
    		if(fichier->eof()){ char temp[255]; sprintf(temp,"Erreur : Objet \" %s \" Invalide",chemin.c_str());console->Ajouter(temp,1); caractere='$'; }

    	}while(caractere!='$');




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
                        case 'x' : *fichier>>m_positionImage.x; break;
                        case 'y' : *fichier>>m_positionImage.y; break;
                        case 'w' : *fichier>>m_positionImage.w; break;
                        case 'h' : *fichier>>m_positionImage.h; break;
                    }

                    if(fichier->eof()){ char temp[255]; sprintf(temp,"Erreur : Objet \" %s \" Invalide",chemin.c_str());console->Ajouter(temp,1); caractere='$'; }

                }while(caractere!='$');
                fichier->get(caractere);
            }
    		if(fichier->eof()){ char temp[255]; sprintf(temp,"Erreur : Objet \" %s \" Invalide",chemin.c_str());console->Ajouter(temp,1); caractere='$'; }

    	}while(caractere!='$');

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
                        case 'x' : *fichier>>m_taille.x; break;
                        case 'y' : *fichier>>m_taille.y; break;
                    }

                    if(fichier->eof()){ char temp[255]; sprintf(temp,"Erreur : Objet \" %s \" Invalide",chemin.c_str());console->Ajouter(temp,1); caractere='$'; }

                }while(caractere!='$');
                fichier->get(caractere);
            }
    		if(fichier->eof()){ char temp[255]; sprintf(temp,"Erreur : Objet \" %s \" Invalide",chemin.c_str());console->Ajouter(temp,1); caractere='$'; }

    	}while(caractere!='$');


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
                        case 'r' : *fichier>>m_rarete; break;
                        case 'b' : *fichier>>m_capaciteBenediction; break;
                    }

                    if(fichier->eof()){ char temp[255]; sprintf(temp,"Erreur : Objet \" %s \" Invalide",chemin.c_str());console->Ajouter(temp,1); caractere='$'; }

                }while(caractere!='$');
                fichier->get(caractere);
            }
    		if(fichier->eof()){ char temp[255]; sprintf(temp,"Erreur : Objet \" %s \" Invalide",chemin.c_str());console->Ajouter(temp,1); caractere='$'; }

    	}while(caractere!='$');

        this->ChargerCaracteristiques(fichier);




        do
    	{

    		fichier->get(caractere);
    		if(caractere=='*')
            {
                int b=-1,ia=-1,ib=-1;
                do
                {
                    fichier->get(caractere);
                    switch (caractere)
                    {
                        case 'b' : *fichier>>b; break;
                        case 'i' :
                            fichier->get(caractere);
                            if(caractere=='1')
                                *fichier>>ia;
                            if(caractere=='2')
                                *fichier>>ib;
                        break;
                    }

                    if(fichier->eof()){ char temp[255]; sprintf(temp,"Erreur : Objet \" %s \" Invalide",chemin.c_str());console->Ajouter(temp,1); caractere='$'; }

                }while(caractere!='$');

                m_benedictions.push_back(benediction ());
                m_benedictions.back().type=b;
                m_benedictions.back().info1=ia;
                m_benedictions.back().info2=ib;

                fichier->get(caractere);
            }

    		if(fichier->eof()){ caractere='$'; }

    	}while(caractere!='$');
    	fichier->close();
    }
    else
    console->Ajouter("Impossible d'ouvrir : "+chemin,1);

    delete fichier;



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

    if(m_rarete<DIVIN)
    {
        int nbrBene=0;
        int rarete=NORMAL;
        int random=rand()%10000 / bonus;
        if(random<=3000)
            rarete=BONNEFACTURE;

        if(random<=300)
            rarete=BENI;

        if(random<=30)
            rarete=SACRE;

        if(random<3)
            rarete=SANCTIFIE;

        if(rarete<m_rarete)
            rarete=m_rarete;
        m_rarete=rarete;

        if(m_rarete==BONNEFACTURE)
        {
            m_armure*=1;
            m_degatsMin*=1;
            m_degatsMax*=1;
            nbrBene=1;

            m_color.r=255-rand()%64;
            m_color.g=255-rand()%64;
            m_color.b=255-rand()%64;
        }

        if(m_rarete==BENI)
        {
            m_armure*=1;
            m_degatsMin*=1;
            m_degatsMax*=1;
            nbrBene=rand()%(4-2)+2;

            m_color.r=255-rand()%128;
            m_color.g=255-rand()%128;
            m_color.b=255-rand()%128;
        }

        if(m_rarete==SACRE)
        {
            m_armure*=1;
            m_degatsMin*=1;
            m_degatsMax*=1;
            nbrBene=rand()%(9-5)+5;

            m_color.r=255-rand()%192;
            m_color.g=255-rand()%192;
            m_color.b=255-rand()%192;
        }

        if(m_rarete==SANCTIFIE)
        {
            m_armure*=1;
            m_degatsMin*=1;
            m_degatsMax*=1;
            nbrBene=rand()%(15-10)+10;

            m_color.r=255-rand()%255;
            m_color.g=255-rand()%255;
            m_color.b=255-rand()%255;
        }

        for(int i=0;i<nbrBene;i++)
        {
            bool ajouter=true;

            benediction temp;
            temp.type=rand()%NOMBRE_BENEDICTION;

            if(temp.type==VIE_SUPP||temp.type==FOI_SUPP)
                temp.info1=rand()%(m_capaciteBenediction*10 - (int)(m_capaciteBenediction*3))+m_capaciteBenediction*3;
            else if(temp.type==EFFICACITE_ACCRUE&&(m_type==ARME||m_type==ARMURE))
                temp.info1=(int)(rand()%(m_capaciteBenediction*10 - (int)((float)m_capaciteBenediction*2.5))+(float)m_capaciteBenediction*2.5);
            else
                temp.info1=(int)(rand()%(m_capaciteBenediction*1 - (int)((float)m_capaciteBenediction*0.5))+(float)m_capaciteBenediction*0.5);

            if(temp.type==EFFICACITE_ACCRUE&&!(m_type==ARME||m_type==ARMURE))
                ajouter=false,i--;

            for(int j=0;j<(int)m_benedictions.size();j++)
                if(m_benedictions[j].type==temp.type)
                    m_benedictions[j].info1+=temp.info1,ajouter=false;

            if(ajouter)
            {
                m_benedictions.push_back(benediction ());
                m_benedictions.back()=temp;
                m_benedictions.back().info2=0;
            }
        }

        /*for(int i=0;i<(int)m_benedictions.size();i++)
            if(m_benedictions[i].type==EFFICACITE_ACCRUE)
            {
                m_armure+=(int)((float)m_armure*(float)m_benedictions[i].info1*0.01);
                m_degatsMin+=(int)(m_degatsMin*m_benedictions[i].info1*0.01);
                m_degatsMax+=(int)(m_degatsMax*m_benedictions[i].info1*0.01);
            }*/
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

                    if(fichier->eof()){ char temp[255]; sprintf(temp,"Erreur : Objet \" %s \" Invalide",m_chemin.c_str());console->Ajouter(temp,1); caractere='$'; }

                }while(caractere!='$');
                fichier->get(caractere);



            }
            if(fichier->eof()){ char temp[255]; sprintf(temp,"Erreur : Objet \" %s \" Invalide",m_chemin.c_str());console->Ajouter(temp,1); caractere='$'; }

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

                    if(fichier->eof()){ char temp[255]; sprintf(temp,"Erreur : Objet \" %s \" Invalide",m_chemin.c_str());console->Ajouter(temp,1); caractere='$'; }

                }while(caractere!='$');



                fichier->get(caractere);
            }
            if(fichier->eof()){ char temp[255]; sprintf(temp,"Erreur : Objet \" %s \" Invalide",m_chemin.c_str());console->Ajouter(temp,1); caractere='$'; }

        }while(caractere!='$');


    }
}


sf::String Objet::AjouterCaracteristiqueAfficher(coordonnee position,coordonnee *decalage,coordonnee *tailleCadran, char *chaine,bool bleu)
{
    sf::String string;

    if(bleu)
        string.SetColor(sf::Color(0,128,255));

    string.SetSize(14.f*configuration->Resolution.h/600);
    string.SetText(chaine);

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

    //position.x*=configuration->Resolution.w/configuration->Resolution.x;
    //position.y*=configuration->Resolution.h/configuration->Resolution.y;

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

    for(int i=0;i<(int)m_description.size();i++)
    {
        sprintf(chaine,"%s",m_description[i].c_str());
        temp.push_back(AjouterCaracteristiqueAfficher(position,&decalage,&tailleCadran,chaine));
    }

    sprintf(chaine," ");
    temp.push_back(AjouterCaracteristiqueAfficher(position,&decalage,&tailleCadran,chaine));

    float multiplieurEfficacite=100;

    for(int i=0;i<(int)m_benedictions.size();i++)
        if(m_benedictions[i].type==EFFICACITE_ACCRUE)
            multiplieurEfficacite+=m_benedictions[i].info1;

    switch(m_type)
    {
        case ARME:
            sprintf(chaine,"Dégats : %i - %i",(int)(m_degatsMin*multiplieurEfficacite/100),(int)(m_degatsMax*multiplieurEfficacite/100));

            if(multiplieurEfficacite!=100)
                temp.push_back(AjouterCaracteristiqueAfficher(position,&decalage,&tailleCadran,chaine,1));
            else
                temp.push_back(AjouterCaracteristiqueAfficher(position,&decalage,&tailleCadran,chaine));

        break;
        case ARMURE:
            sprintf(chaine,"Armure : %i",(int)(m_armure*multiplieurEfficacite/100));

            if(multiplieurEfficacite!=100)
                temp.push_back(AjouterCaracteristiqueAfficher(position,&decalage,&tailleCadran,chaine,1));
            else
                temp.push_back(AjouterCaracteristiqueAfficher(position,&decalage,&tailleCadran,chaine));
        break;
    }

    sprintf(chaine," ");
    temp.push_back(AjouterCaracteristiqueAfficher(position,&decalage,&tailleCadran,chaine));

    for(int i=0;i<(int)m_benedictions.size();i++)
    {
        if(m_benedictions[i].type>=0&&m_benedictions[i].type<NOMBRE_BENEDICTION)
        {
            sprintf(chaine,"%s %i",configuration->text_benedictions[m_benedictions[i].type].c_str(),m_benedictions[i].info1);
            temp.push_back(AjouterCaracteristiqueAfficher(position,&decalage,&tailleCadran,chaine));
        }
    }

    if(position.x-tailleCadran.x-10<0)
            position.x=tailleCadran.x+10;

    if(position.y+decalage.y+10>configuration->Resolution.h)
            position.y=configuration->Resolution.h-decalage.y-10;

    int decalY=0;
    for(int i=0;i<(int)temp.size();i++)
    {
        temp[i].SetY((position.y+decalY+10));
        temp[i].SetX(position.x+(tailleCadran.x/2-((int)temp[i].GetRect().Right-(int)temp[i].GetRect().Left)/2)-tailleCadran.x);

        decalY+=(int)temp[i].GetRect().Bottom-(int)temp[i].GetRect().Top+5;

        moteurGraphique->AjouterTexte(&temp[i],19);
    }

    tailleCadran.y=decalage.y;

    tailleCadran.y+=20;
    tailleCadran.x+=20;

    sprite.SetImage(*moteurGraphique->getImage(0));
    sprite.SetColor(sf::Color(0,0,0,224));
    sprite.SetY(position.y);
    sprite.SetX(position.x-tailleCadran.x+10);
    sprite.Resize(tailleCadran.x,tailleCadran.y);
    moteurGraphique->AjouterCommande(&sprite,18,0);
    temp.clear();
}






void Objet::setChanceTrouver(int chance){ m_chanceTrouver=chance; }

int Objet::getChanceTrouver(){return m_chanceTrouver;}
