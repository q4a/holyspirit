#ifndef HEROCPP
#define HEROCPP

#include "Hero.h"
#include "Globale.h"

#include <iostream>
#include <fstream>

#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include <math.h>
#include <dirent.h>

using namespace std;
using namespace sf;

int crypter(int);
int decrypter(int);


Hero::Hero()
{
   ///Je donnes des valeur à mes variables juste pour les tests
	m_personnage.setEtat(ARRET);

	m_chercherSac.x=-1;
	m_chercherSac.y=-1;

	coordonnee position;
	position.x=-10000;
	position.y=-10000;

	m_personnage.setCoordonnee(position);

	Lumiere lumiere;
	lumiere.intensite=192;
	lumiere.rouge=255;
	lumiere.vert=255;
	lumiere.bleu=255;
	lumiere.hauteur=20;

	m_modelePersonnage[0].setPorteeLumineuse(lumiere);

	lumiere.intensite=0;

    for(int i=1;i<NOMBRE_MORCEAU_PERSONNAGE;i++)
        m_modelePersonnage[i].setPorteeLumineuse(lumiere);

    m_personnage.setPorteeLumineuse(lumiere);

	Caracteristique temp;

	temp.vie=100;
	temp.maxVie=100;
	temp.foi=100;
	temp.maxFoi=100;

	temp.vitesse=1;
	temp.pointAme=0;
	temp.ancienPointAme=0;
	temp.positionAncienAme=0;
	temp.niveau=1;
	temp.nom="Héro";

	temp.force=4;
    temp.dexterite=4;
    temp.vitalite=4;
    temp.piete=4;
    temp.charisme=4;

    temp.modificateurTaille=1;

	m_personnage.setCaracteristique(temp);

	m_monstreVise=-1;

    m_objetEnMain=-1;

    recalculerCaracteristiques();

    miracleEnCours=0;

    m_cas=0;

    for(int i=0;i<NOMBRE_MORCEAU_PERSONNAGE;i++)
            m_cheminModeleNouveau[i]="",m_cheminModele[i]="";

    m_contenuSave.push_back(configuration->chemin_temps+"hero.sav.txt");

    m_personnage.m_light=moteurGraphique->LightManager->Add_Dynamic_Light(sf::Vector2f(m_personnage.getCoordonnee().x,m_personnage.getCoordonnee().y),512,2048,8,sf::Color(255,255,255));
}

void Hero::Sauvegarder()
{
    console->Ajouter("");
    console->Ajouter("Sauvegarde du héro...");

    ofstream fichier((configuration->chemin_temps+"hero.sav.txt").c_str(), ios::out | ios::trunc | ios::binary);

    if(fichier)
    {
        if(configuration->debug)
            console->Ajouter("Ouverture du fichier.");

        std::string temp;
        char caractere;

        int n = m_personnage.getCaracteristique().nom.size()+1;
        fichier.write((char *)&n, sizeof(int));
        fichier.write(m_personnage.getCaracteristique().nom.c_str(), n);

        n = m_cheminClasse.size()+1;
        fichier.write((char *)&n, sizeof(int));
        fichier.write(m_cheminClasse.c_str(), n);

        //fichier.write((char*)&m_personnage.getCaracteristique().nom, sizeof(string));

        fichier.write((char*)&m_personnage.getCaracteristique().vitesse, sizeof(float));

        fichier.write((char*)&m_personnage.getCaracteristique().pointAme, sizeof(int));

        fichier.write((char*)&m_personnage.getCaracteristique().niveau, sizeof(int));

        fichier.write((char*)&m_personnage.getCaracteristique().force, sizeof(int));

        fichier.write((char*)&m_personnage.getCaracteristique().dexterite, sizeof(int));

        fichier.write((char*)&m_personnage.getCaracteristique().vitalite, sizeof(int));

        fichier.write((char*)&m_personnage.getCaracteristique().piete, sizeof(int));

        fichier.write((char*)&m_personnage.getCaracteristique().charisme, sizeof(int));

        if(configuration->debug)
            console->Ajouter("/Ecriture des caracterstiques.");


       // caractere='$';
        //fichier.write((char*)&caractere, sizeof(char));
        //fichier.write((char*)&espace, sizeof(char));

        /*for(int i=0;i<(int)m_caseInventaire.size();++i)
        {
            caractere='*';
            fichier.write((char*)&caractere, sizeof(char));
            //fichier.write((char*)&espace, sizeof(char));

            for(int j=0;j<(int)m_caseInventaire[i].size();++j)
                fichier.write((char*)&m_caseInventaire[i][j], sizeof(bool));

            caractere='$';
            //fichier.write((char*)&espace, sizeof(char));
            fichier.write((char*)&caractere, sizeof(char));
            fichier.write((char*)&espace, sizeof(char));
        }
        caractere='$';*/
        //fichier.write((char*)&espace, sizeof(char));
        fichier.write((char*)&caractere, sizeof(char));

        if(configuration->debug)
            console->Ajouter("/Ecriture des cases inventaires.");

        for(int i=0;i<(int)m_inventaire.size();++i)
        {
            m_inventaire[i].Sauvegarder(&fichier);
        }

        if(configuration->debug)
            console->Ajouter("/Ecriture des objets.");

       // fichier.write((char*)&espace, sizeof(char));
        caractere='$';
        fichier.write((char*)&caractere, sizeof(char));

        fichier.close();

        console->Ajouter("Sauvegarde du héro terminée !");

    }
    else
        console->Ajouter("Impossible de sauvegarder le héro !",1);

    cDAT fichierSave;

    fichierSave.Create(m_contenuSave, configuration->chemin_saves+"hero.sav.hs");
    string buffer=configuration->chemin_temps+"hero.sav.txt";
    //remove(buffer.c_str());
    //m_contenuSave.clear();

}

void Classe::Charger(string chemin)
{
    emplacements.clear();
    equipementParDefaut.clear();

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
                fichier>>nom;
            }
    		if(fichier.eof()){ char temp[255]; sprintf(temp,"Erreur : Classe \" %s \" Invalide",chemin.c_str());console->Ajouter(temp,1); caractere='$'; }
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
                        case 'f' : fichier>>caracteristique.force; break;
                        case 'v' : fichier>>caracteristique.vitalite; break;
                        case 'p' : fichier>>caracteristique.piete; break;
                        case 'd' : fichier>>caracteristique.dexterite; break;
                        case 'c' : fichier>>caracteristique.charisme; break;
                    }

                    if(fichier.eof()){ char temp[255]; sprintf(temp,"Erreur : Classe \" %s \" Invalide",chemin.c_str());console->Ajouter(temp,1); caractere='$'; }

                }while(caractere!='$');
                fichier.get(caractere);
            }
    		if(fichier.eof()){ char temp[255]; sprintf(temp,"Erreur : Classe \" %s \" Invalide",chemin.c_str());console->Ajouter(temp,1); caractere='$'; }

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
                        case 'm' : string temp; fichier>>temp; equipementParDefaut.push_back(temp); break;
                    }

                    if(fichier.eof()){ char temp[255]; sprintf(temp,"Erreur : Classe \" %s \" Invalide",chemin.c_str());console->Ajouter(temp,1); caractere='$'; }

                }while(caractere!='$');
                fichier.get(caractere);
            }
    		if(fichier.eof()){ char temp[255]; sprintf(temp,"Erreur : Classe \" %s \" Invalide",chemin.c_str());console->Ajouter(temp,1); caractere='$'; }

    	}while(caractere!='$');

    	do
    	{
    		fichier.get(caractere);
    		if(caractere=='*')
            {
                int temp=0;
                do
                {
                    fichier.get(caractere);
                    switch (caractere)
                    {
                        case 'm' : fichier>>modeleNu[temp]; temp++; break;
                    }

                    if(fichier.eof()){ char temp[255]; sprintf(temp,"Erreur : Classe \" %s \" Invalide",chemin.c_str());console->Ajouter(temp,1); caractere='$'; }

                }while(caractere!='$');
                fichier.get(caractere);
            }
    		if(fichier.eof()){ char temp[255]; sprintf(temp,"Erreur : Classe \" %s \" Invalide",chemin.c_str());console->Ajouter(temp,1); caractere='$'; }

    	}while(caractere!='$');

    	std::string string;

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
                        case 'm' : fichier>>string; inventaire.image=moteurGraphique->AjouterImage(string,-1); break;
                        case 'x' : fichier>>inventaire.position.x; break;
                        case 'y' : fichier>>inventaire.position.y; break;
                        case 'w' : fichier>>inventaire.position.w; break;
                        case 'h' : fichier>>inventaire.position.h; break;
                    }

                    if(fichier.eof()){ char temp[255]; sprintf(temp,"Erreur : Classe \" %s \" Invalide",chemin.c_str());console->Ajouter(temp,1); caractere='$'; }
                }while(caractere!='$');
                fichier.get(caractere);
            }
    		if(fichier.eof()){ char temp[255]; sprintf(temp,"Erreur : Classe \" %s \" Invalide",chemin.c_str());console->Ajouter(temp,1); caractere='$'; }

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
                        case 'm' : fichier>>string; hud.image=moteurGraphique->AjouterImage(string,-1); break;
                        case 'x' : fichier>>hud.position.x; break;
                        case 'y' : fichier>>hud.position.y; break;
                        case 'w' : fichier>>hud.position.w; break;
                        case 'h' : fichier>>hud.position.h; break;
                    }

                    if(fichier.eof()){ char temp[255]; sprintf(temp,"Erreur : Classe \" %s \" Invalide",chemin.c_str());console->Ajouter(temp,1); caractere='$'; }

                }while(caractere!='$');
                fichier.get(caractere);
            }
    		if(fichier.eof()){ char temp[255]; sprintf(temp,"Erreur : Classe \" %s \" Invalide",chemin.c_str());console->Ajouter(temp,1); caractere='$'; }

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
                        case 'm' : fichier>>string; orbe_vie.image=moteurGraphique->AjouterImage(string,-1); break;
                        case 'x' : fichier>>orbe_vie.position.x; break;
                        case 'y' : fichier>>orbe_vie.position.y; break;
                        case 'w' : fichier>>orbe_vie.position.w; break;
                        case 'h' : fichier>>orbe_vie.position.h; break;
                    }

                    if(fichier.eof()){ char temp[255]; sprintf(temp,"Erreur : Classe \" %s \" Invalide",chemin.c_str());console->Ajouter(temp,1); caractere='$'; }

                }while(caractere!='$');
                fichier.get(caractere);
            }
    		if(fichier.eof()){ char temp[255]; sprintf(temp,"Erreur : Classe \" %s \" Invalide",chemin.c_str());console->Ajouter(temp,1); caractere='$'; }

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
                        case 'm' : fichier>>string; orbe_foi.image=moteurGraphique->AjouterImage(string,-1); break;
                        case 'x' : fichier>>orbe_foi.position.x; break;
                        case 'y' : fichier>>orbe_foi.position.y; break;
                        case 'w' : fichier>>orbe_foi.position.w; break;
                        case 'h' : fichier>>orbe_foi.position.h; break;
                    }

                    if(fichier.eof()){ char temp[255]; sprintf(temp,"Erreur : Classe \" %s \" Invalide",chemin.c_str());console->Ajouter(temp,1); caractere='$'; }

                }while(caractere!='$');
                fichier.get(caractere);
            }
    		if(fichier.eof()){ char temp[255]; sprintf(temp,"Erreur : Classe \" %s \" Invalide",chemin.c_str());console->Ajouter(temp,1); caractere='$'; }

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
                        case 'x' : fichier>>position_sac_inventaire.x; break;
                        case 'y' : fichier>>position_sac_inventaire.y; break;
                        case 'w' : fichier>>position_sac_inventaire.w; break;
                        case 'h' : fichier>>position_sac_inventaire.h; break;
                    }

                    if(fichier.eof()){ char temp[255]; sprintf(temp,"Erreur : Classe \" %s \" Invalide",chemin.c_str());console->Ajouter(temp,1); caractere='$'; }

                }while(caractere!='$');
                fichier.get(caractere);
            }
    		if(fichier.eof()){ char temp[255]; sprintf(temp,"Erreur : Classe \" %s \" Invalide",chemin.c_str());console->Ajouter(temp,1); caractere='$'; }

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
                        case 'x' : fichier>>position_contenu_inventaire.x; break;
                        case 'y' : fichier>>position_contenu_inventaire.y; break;
                        case 'w' : fichier>>position_contenu_inventaire.w; break;
                        case 'h' : fichier>>position_contenu_inventaire.h; break;
                    }

                    if(fichier.eof()){ char temp[255]; sprintf(temp,"Erreur : Classe \" %s \" Invalide",chemin.c_str());console->Ajouter(temp,1); caractere='$'; }

                }while(caractere!='$');
                fichier.get(caractere);
            }
    		if(fichier.eof()){ char temp[255]; sprintf(temp,"Erreur : Classe \" %s \" Invalide",chemin.c_str());console->Ajouter(temp,1); caractere='$'; }

    	}while(caractere!='$');

    	do
    	{
    		fichier.get(caractere);
    		if(caractere=='*')
            {
                emplacements.push_back(Emplacement_inventaire ());
                do
                {
                    fichier.get(caractere);
                    switch (caractere)
                    {
                        case 'e' : fichier>>emplacements.back().emplacement; break;
                        case 'x' : fichier>>emplacements.back().position.x; break;
                        case 'y' : fichier>>emplacements.back().position.y; break;
                        case 'w' : fichier>>emplacements.back().position.w; break;
                        case 'h' : fichier>>emplacements.back().position.h; break;
                    }

                    if(fichier.eof()){ char temp[255]; sprintf(temp,"Erreur : Classe \" %s \" Invalide",chemin.c_str());console->Ajouter(temp,1); caractere='$'; }

                }while(caractere!='$');
                fichier.get(caractere);
            }
    		if(fichier.eof()){ char temp[255]; sprintf(temp,"Erreur : Classe \" %s \" Invalide",chemin.c_str());console->Ajouter(temp,1); caractere='$'; }

    	}while(caractere!='$');


        fichier.close();
    }

}

void Hero::Charger()
{
    console->Ajouter("Chargement du hero.");
    m_cheminClasse="Data/Entities/Heroes/Crusader.class.hs";
    for(int i=0;i<NOMBRE_MORCEAU_PERSONNAGE;i++)
        m_cheminModele[i]="";


    bool nouveau=true;
    string chemin;
   // struct dirent *lecture;

	//DIR *repertoire;
    //repertoire = opendir(configuration->chemin_saves.c_str());
    //while ((lecture = readdir(repertoire)))
  //  {
       // string temp="hero.sav.hs";
       // if(lecture->d_name==temp)
        //{
            cDAT reader;
            if(reader.Read(configuration->chemin_saves+"hero.sav.hs"))
            {
                ifstream* fichier=reader.GetInfos(configuration->chemin_temps+"hero.sav.txt");
                //fichier.open((configuration->chemin_saves+"hero.sav.hs").c_str(), ios::in | ios::binary);
                if(fichier)
                {
                    char caractere;
                    Caracteristique charTemp;
                    charTemp=m_personnage.getCaracteristique();

                    int n;
                    char *buffer;

                    fichier->read((char *)&n, sizeof(int));
                    buffer=new char[n];
                    fichier->read(buffer,n);
                    charTemp.nom=buffer;
                    delete[] buffer;

                    fichier->read((char *)&n, sizeof(int));
                    buffer=new char[n];
                    fichier->read(buffer,n);
                    m_cheminClasse=buffer;
                    delete[] buffer;

                    fichier->read((char*)&charTemp.vitesse, sizeof(float));
                    fichier->read((char*)&charTemp.pointAme, sizeof(int)),charTemp.ancienPointAme=charTemp.pointAme,charTemp.positionAncienAme=charTemp.pointAme;
                    fichier->read((char*)&charTemp.niveau, sizeof(int));

                    fichier->read((char*)&charTemp.force, sizeof(int));
                    fichier->read((char*)&charTemp.dexterite, sizeof(int));
                    fichier->read((char*)&charTemp.vitalite, sizeof(int));
                    fichier->read((char*)&charTemp.piete, sizeof(int));
                    fichier->read((char*)&charTemp.charisme, sizeof(int));

                    if(configuration->debug)
                        console->Ajouter("/Lectures des caracteristiques.");

                        coordonnee position={0,0,0,0};


                         if(configuration->debug)
                                console->Ajouter("/Lectures des cases de l'inventaire.");

                        do
                        {
                            fichier->read((char*)&caractere, sizeof(char));
                            if(caractere=='*')
                            {
                                int rarete=0,equipe=0;
                                int degMin=0,degMax=0,armure=0;
                                position.y=0;
                                position.x=0;
                                std::vector <benediction> bene;
                                sf::Color color;

                                do
                                {
                                    fichier->read((char*)&caractere, sizeof(char));
                                    if(caractere=='e')
                                        fichier->read((char*)&equipe, sizeof(int));//fichier>>equipe;

                                    else if(caractere=='r')
                                        fichier->read((char*)&rarete, sizeof(int));//fichier>>rarete;

                                    else if(caractere=='x')
                                        fichier->read((char*)&position.x, sizeof(int));//fichier>>position.x;
                                    else if(caractere=='y')
                                        fichier->read((char*)&position.y, sizeof(int));//fichier>>position.y;

                                    else  if(caractere=='d')
                                    {
                                        fichier->read((char*)&caractere, sizeof(char));
                                        if(caractere=='i')
                                           fichier->read((char*)&degMin, sizeof(int)); //fichier>>degMin;
                                        else if(caractere=='a')
                                           fichier->read((char*)&degMax, sizeof(int)); // fichier>>degMax;
                                    }

                                    else if(caractere=='a')
                                        fichier->read((char*)&armure, sizeof(int));//fichier>>armure;

                                    else  if(caractere=='l')
                                    {
                                        fichier->read((char*)&caractere, sizeof(char));
                                        if(caractere=='r')
                                            fichier->read((char*)&color.r, sizeof(Uint8));//fichier>>color.r;
                                        else if(caractere=='g')
                                            fichier->read((char*)&color.g, sizeof(Uint8));//fichier>>color.g;
                                        else if(caractere=='b')
                                           fichier->read((char*)&color.b, sizeof(Uint8)); //fichier>>color.b;
                                    }
                                    else if(caractere=='m')
                                    {
                                        int m;
                                        char *buffer;

                                        fichier->read((char*)&m, sizeof(int));
                                        buffer=new char[m];
                                        fichier->read(buffer,m);


                                        m_inventaire.push_back(Objet ());
                                        m_inventaire.back().Charger(buffer);
                                        delete[] buffer;
                                    }
                                    else if(caractere=='b')
                                    {
                                        int type=0,info1=0,info2=0;

                                        //fichier>>type;
                                        fichier->read((char*)&type, sizeof(int));

                                        do
                                        {
                                            fichier->read((char*)&caractere, sizeof(char));
                                            if(caractere=='i')
                                            {
                                                fichier->read((char*)&caractere, sizeof(char));
                                                if(caractere=='1')
                                                    fichier->read((char*)&info1, sizeof(int));//fichier>>info1;
                                                if(caractere=='2')
                                                    fichier->read((char*)&info2, sizeof(int));//fichier>>info2;
                                            }
                                        }while(caractere!='$');

                                        bene.push_back(benediction ());
                                        bene.back().type=type;
                                        bene.back().info1=info1;
                                        bene.back().info2=info2;
                                        fichier->read((char*)&caractere, sizeof(char));
                                    }

                                    if(fichier->eof()){throw "Impossible de charger la sauvegarde";}
                                }while(caractere!='$');

                                if(m_inventaire.size()>0)
                                {

                                    m_inventaire.back().setRarete(rarete);
                                    m_inventaire.back().setPosition(position.x,position.y);
                                    m_inventaire.back().m_equipe=equipe;

                                    m_inventaire.back().m_armure=armure;
                                    m_inventaire.back().m_degatsMin=degMin;
                                    m_inventaire.back().m_degatsMax=degMax;

                                    m_inventaire.back().m_color=color;


                                    m_inventaire.back().m_benedictions=bene;
                                }

                                fichier->read((char*)&caractere, sizeof(char));
                            }
                            if(fichier->eof()){throw "Impossible de charger la sauvegarde";}

                        }while(caractere!='$');

                        if(configuration->debug)
                            console->Ajouter("/Lectures des objets.");



                    m_personnage.setCaracteristique(charTemp);
                    nouveau=false;
                }
                fichier->close();
            }
        //}
    //}
   // closedir(repertoire);

    m_classe.Charger(m_cheminClasse);

    m_caseInventaire.resize(m_classe.position_contenu_inventaire.h , std::vector<bool> ());
        for(int i=0;i<m_classe.position_contenu_inventaire.h;++i)
            m_caseInventaire[i].resize(m_classe.position_contenu_inventaire.w, 0);

    GenererGrille();

    if(nouveau)
    {
        //m_classe.Charger(m_cheminClasse);

        sf::Color color;
        color.r=255;
        color.g=255;
        color.b=255;


        for(int i=0;i<(int)m_classe.equipementParDefaut.size();i++)
        {
            Objet temp;

            temp.Charger(m_classe.equipementParDefaut[i]);
            temp.Generer(1);
            ajouterObjet(temp);

            Caracteristique charTemp;
            charTemp=m_personnage.getCaracteristique();

            charTemp.force=m_classe.caracteristique.force;
            charTemp.vitalite=m_classe.caracteristique.vitalite;
            charTemp.dexterite=m_classe.caracteristique.dexterite;
            charTemp.piete=m_classe.caracteristique.piete;
            charTemp.charisme=m_classe.caracteristique.charisme;

            m_personnage.setCaracteristique(charTemp);
        }



    }
    GenererGrille();
    ChargerModele();

    recalculerCaracteristiques();

    m_caracteristiques.vie=m_caracteristiques.maxVie;
    m_caracteristiques.foi=m_caracteristiques.maxFoi;
}



void Hero::ChargerModele(bool tout)
{
    m_personnage.setPose(0);
    m_cas=0;

    int nombreArme=0;

    for(int i=0;i<(int)m_inventaire.size();i++)
    {
        if(m_inventaire[i].m_equipe>=0&&m_inventaire[i].m_equipe<(int)m_classe.emplacements.size())
        {
            if(m_classe.emplacements[m_inventaire[i].m_equipe].emplacement==ARME_PRINCIPAL&&m_inventaire[i].m_type==ARME||m_classe.emplacements[m_inventaire[i].m_equipe].emplacement==BOUCLIER&&m_inventaire[i].m_type==ARME)
                nombreArme++;

            if(m_classe.emplacements[m_inventaire[i].m_equipe].emplacement==ARME_PRINCIPAL&&m_inventaire[i].m_type==ARME)
                for(int j=0;j<(int)m_inventaire[i].m_emplacementImpossible.size();j++)
                    if(m_inventaire[i].m_emplacementImpossible[j]==BOUCLIER)
                        m_cas=2;
        }
    }

    if(nombreArme==2)
        m_cas=1;



    bool pasEquipe[NOMBRE_MORCEAU_PERSONNAGE];

    for(int i=0;i<NOMBRE_MORCEAU_PERSONNAGE;i++)
    {
        pasEquipe[i]=true,m_cheminModeleNouveau[i]="";
        if(tout)
            m_cheminModele[i]="";
    }


    for(int i=0;i<(int)m_inventaire.size();i++)
        if(m_inventaire[i].m_equipe>=0)
        {
                if(m_inventaire[i].m_emplacementImageHero.size()>0)
                {
                    int temp=m_cas;

                    if(m_classe.emplacements[m_inventaire[i].m_equipe].emplacement==ARME_PRINCIPAL&&m_inventaire[i].m_type==ARME&&m_cas>0)
                        temp=0;

                    if(m_classe.emplacements[m_inventaire[i].m_equipe].emplacement==BOUCLIER&&m_inventaire[i].m_type==ARME&&m_cas>0)
                        temp=1;

                    if(temp>=(int)m_inventaire[i].m_emplacementImageHero.size())
                        temp=0;

                    if(m_inventaire[i].m_emplacementImageHero[temp]>=0&&m_inventaire[i].m_emplacementImageHero[temp]<NOMBRE_MORCEAU_PERSONNAGE)
                        //if(m_cheminModele[m_inventaire[i].m_emplacementImageHero[temp]]!=m_inventaire[i].m_cheminImageHero[temp])
                        {
                            m_cheminModeleNouveau[m_inventaire[i].m_emplacementImageHero[temp]]=m_inventaire[i].m_cheminImageHero[temp];
                            pasEquipe[m_inventaire[i].m_emplacementImageHero[temp]]=false;
                        }
                }
        }

    for(int i=0;i<NOMBRE_MORCEAU_PERSONNAGE;i++)
    {
        if(m_cheminModeleNouveau[i]!="" && m_cheminModeleNouveau[i]!=m_cheminModele[i])
            m_modelePersonnage[i].Charger(m_cheminModeleNouveau[i]);
        else if(m_cheminModeleNouveau[i]=="")
            m_modelePersonnage[i].Reinitialiser();

        m_cheminModele[i]=m_cheminModeleNouveau[i];
    }

    if(pasEquipe[0])
    {
        if(m_cheminModele[0]!=m_classe.modeleNu[m_cas])
        {
            m_modelePersonnage[0].Charger(m_classe.modeleNu[m_cas]);
            m_cheminModele[0]=m_classe.modeleNu[m_cas];
        }
    }

    CalculerOrdreAffichage();

}

void Hero::CalculerOrdreAffichage()
{
    for(int i=0;i<NOMBRE_MORCEAU_PERSONNAGE;i++)
        ordreAffichage[i]=-1;

    int ordre;


    for(int i=0;i<NOMBRE_MORCEAU_PERSONNAGE;i++)
    {
        ordre=m_personnage.getOrdre(&m_modelePersonnage[i]);
        if(ordre!=-10)
            ordreAffichage[(int)(NOMBRE_MORCEAU_PERSONNAGE/2+ordre)]=i;
    }
}

void Hero::Afficher(sf::RenderWindow* ecran,sf::View *camera,coordonnee position,coordonnee dimensionsMap)
{
    for(int i=0;i<NOMBRE_MORCEAU_PERSONNAGE;i++)
        if(ordreAffichage[i]!=-1)
            m_personnage.Afficher(ecran,camera,position,dimensionsMap,&m_modelePersonnage[ordreAffichage[i]]);
}

void Hero::afficherCaracteristiques(sf::RenderWindow *ecran,coordonnee positionSouris,float decalage)
{
    sf::String string;
    char chaine[255];
     string.SetSize(16*configuration->Resolution.h/600);

     string.SetColor(sf::Color(255,255,255));


    sprintf(chaine,"%i",m_caracteristiques.niveau);
    string.SetText(chaine);
    string.SetX((129*configuration->Resolution.w/800)+22*configuration->Resolution.w/800-(string.GetRect().Right-string.GetRect().Left)/2);
    string.SetY(263*configuration->Resolution.h/600-decalage*configuration->Resolution.h/600);
    string.SetColor(sf::Color(255,255,255));
    moteurGraphique->AjouterTexte(&string,15);


    sprintf(chaine,"%s",m_caracteristiques.nom.c_str());
    string.SetText(chaine);
    string.SetX((1*configuration->Resolution.w/800)+62*configuration->Resolution.w/800-(string.GetRect().Right-string.GetRect().Left)/2);
    string.SetY(263*configuration->Resolution.h/600-decalage*configuration->Resolution.h/600);
    string.SetColor(sf::Color(255,255,255));
    moteurGraphique->AjouterTexte(&string,15);




    sprintf(chaine,"%i / %i",(int)m_caracteristiques.vie,(int)m_caracteristiques.maxVie);
    string.SetText(chaine);
    string.SetX((178*configuration->Resolution.w/800)+54*configuration->Resolution.w/800-(string.GetRect().Right-string.GetRect().Left)/2);
    string.SetY(263*configuration->Resolution.h/600-decalage*configuration->Resolution.h/600);
    if(m_caracteristiques.maxVie!=m_personnage.getCaracteristique().maxVie)
        string.SetColor(sf::Color(0,128,255));
    else
        string.SetColor(sf::Color(255,255,255));
    moteurGraphique->AjouterTexte(&string,15);

    sprintf(chaine,"%i / %i",(int)m_caracteristiques.foi,(int)m_caracteristiques.maxFoi);
    string.SetText(chaine);
    string.SetX((290*configuration->Resolution.w/800)+54*configuration->Resolution.w/800-(string.GetRect().Right-string.GetRect().Left)/2);
    string.SetY(263*configuration->Resolution.h/600-decalage*configuration->Resolution.h/600);
    if(m_caracteristiques.maxFoi!=m_personnage.getCaracteristique().maxFoi)
        string.SetColor(sf::Color(0,128,255));
    else
        string.SetColor(sf::Color(255,255,255));
    moteurGraphique->AjouterTexte(&string,15);

    sprintf(chaine,"%i",m_caracteristiques.force);
    string.SetText(chaine);
    string.SetX((129*configuration->Resolution.w/800)+21*configuration->Resolution.w/800-(string.GetRect().Right-string.GetRect().Left)/2);
    string.SetY(311*configuration->Resolution.h/600-decalage*configuration->Resolution.h/600);
    if(m_caracteristiques.force!=m_personnage.getCaracteristique().force)
        string.SetColor(sf::Color(0,128,255));
    else
        string.SetColor(sf::Color(255,255,255));
    moteurGraphique->AjouterTexte(&string,15);

    sprintf(chaine,"%i",m_caracteristiques.dexterite);
    string.SetText(chaine);
    string.SetX((129*configuration->Resolution.w/800)+21*configuration->Resolution.w/800-(string.GetRect().Right-string.GetRect().Left)/2);
    string.SetY(338*configuration->Resolution.h/600-decalage*configuration->Resolution.h/600);
    if(m_caracteristiques.dexterite!=m_personnage.getCaracteristique().dexterite)
        string.SetColor(sf::Color(0,128,255));
    else
        string.SetColor(sf::Color(255,255,255));
    moteurGraphique->AjouterTexte(&string,15);

    sprintf(chaine,"%i",m_caracteristiques.vitalite);
    string.SetText(chaine);
    string.SetX((129*configuration->Resolution.w/800)+21*configuration->Resolution.w/800-(string.GetRect().Right-string.GetRect().Left)/2);
    string.SetY(365*configuration->Resolution.h/600-decalage*configuration->Resolution.h/600);
    if(m_caracteristiques.vitalite!=m_personnage.getCaracteristique().vitalite)
        string.SetColor(sf::Color(0,128,255));
    else
        string.SetColor(sf::Color(255,255,255));
    moteurGraphique->AjouterTexte(&string,15);

    sprintf(chaine,"%i",m_caracteristiques.piete);
    string.SetText(chaine);
    string.SetX((129*configuration->Resolution.w/800)+21*configuration->Resolution.w/800-(string.GetRect().Right-string.GetRect().Left)/2);
    string.SetY(392*configuration->Resolution.h/600-decalage*configuration->Resolution.h/600);
    if(m_caracteristiques.piete!=m_personnage.getCaracteristique().piete)
        string.SetColor(sf::Color(0,128,255));
    else
        string.SetColor(sf::Color(255,255,255));
    moteurGraphique->AjouterTexte(&string,15);

    sprintf(chaine,"%i",m_caracteristiques.charisme);
    string.SetText(chaine);
    string.SetX((129*configuration->Resolution.w/800)+21*configuration->Resolution.w/800-(string.GetRect().Right-string.GetRect().Left)/2);
    string.SetY(419*configuration->Resolution.h/600-decalage*configuration->Resolution.h/600);
    if(m_caracteristiques.charisme!=m_personnage.getCaracteristique().charisme)
        string.SetColor(sf::Color(0,128,255));
    else
        string.SetColor(sf::Color(255,255,255));
    moteurGraphique->AjouterTexte(&string,15);

    sprintf(chaine,"%i - %i",m_caracteristiques.degatsMin,m_caracteristiques.degatsMax);
    string.SetText(chaine);
    string.SetX((314*configuration->Resolution.w/800)+32*configuration->Resolution.w/800-(string.GetRect().Right-string.GetRect().Left)/2);
    string.SetY(300*configuration->Resolution.h/600-decalage*configuration->Resolution.h/600);
    if(m_caracteristiques.degatsMin!=m_personnage.getCaracteristique().degatsMin||m_caracteristiques.degatsMax!=m_personnage.getCaracteristique().degatsMax)
        string.SetColor(sf::Color(0,128,255));
    else
        string.SetColor(sf::Color(255,255,255));
    moteurGraphique->AjouterTexte(&string,15);

    sprintf(chaine,"%i",m_caracteristiques.armure);
    string.SetText(chaine);
    string.SetX((314*configuration->Resolution.w/800)+32*configuration->Resolution.w/800-(string.GetRect().Right-string.GetRect().Left)/2);
    string.SetY(327*configuration->Resolution.h/600-decalage*configuration->Resolution.h/600);
    if(m_caracteristiques.armure!=m_personnage.getCaracteristique().armure)
        string.SetColor(sf::Color(0,128,255));
    else
        string.SetColor(sf::Color(255,255,255));
    moteurGraphique->AjouterTexte(&string,15);
}

void Hero::afficherInventaire(sf::RenderWindow *ecran,coordonnee positionSouris,float decalage)
{

    //positionSouris.x=(int)ecran->ConvertCoords(ecran->GetInput().GetMouseX(),ecran->GetInput().GetMouseY()).x;
    //positionSouris.y=(int)ecran->ConvertCoords(ecran->GetInput().GetMouseX(), ecran->GetInput().GetMouseY()).y;


    for(int i=0;i<(int)m_inventaire.size();i++)
    if(i!=m_objetEnMain)
        {
            coordonneeDecimal position;
            float rotation=0;
             sf::Sprite sprite;

            sprite.SetImage(*moteurGraphique->getImage(0));
            if(m_inventaire[i].getRarete()==NORMAL)
                sprite.SetColor(sf::Color(224,224,224,128));
            if(m_inventaire[i].getRarete()==BONNEFACTURE)
                sprite.SetColor(sf::Color(128,0,128,128));
            if(m_inventaire[i].getRarete()==BENI)
                sprite.SetColor(sf::Color(0,64,128,128));
            if(m_inventaire[i].getRarete()==SACRE)
                sprite.SetColor(sf::Color(255,255,128,128));
            if(m_inventaire[i].getRarete()==SANCTIFIE)
                sprite.SetColor(sf::Color(128,255,255,128));
            if(m_inventaire[i].getRarete()==DIVIN)
                sprite.SetColor(sf::Color(255,164,32,128));
            if(m_inventaire[i].getRarete()==INFERNAL)
                sprite.SetColor(sf::Color(224,0,0,128));
            if(m_inventaire[i].getRarete()==CRAFT)
                sprite.SetColor(sf::Color(128,64,0,128));

            sprite.Resize(m_inventaire[i].getTaille().x*32*configuration->Resolution.w/800,m_inventaire[i].getTaille().y*32*configuration->Resolution.h/600);

            if(m_inventaire[i].m_equipe==-1)
            {
                sprite.SetX((m_inventaire[i].getPosition().x*32+m_classe.position_contenu_inventaire.x)*configuration->Resolution.w/800);
                sprite.SetY(((m_inventaire[i].getPosition().y-1)*32+(m_classe.position_contenu_inventaire.y))*configuration->Resolution.h/600-decalage*configuration->Resolution.h/600);

                position.x=(m_inventaire[i].getPosition().x*32+m_classe.position_contenu_inventaire.x)*configuration->Resolution.w/800;
                position.y=((m_inventaire[i].getPosition().y-1)*32+(m_classe.position_contenu_inventaire.y))*configuration->Resolution.h/600-decalage*configuration->Resolution.h/600;

                position.h=m_inventaire[i].getTaille().y*32*configuration->Resolution.h/600;
                position.w=m_inventaire[i].getTaille().x*32*configuration->Resolution.w/800;
            }
            else
            {

                if(m_inventaire[i].m_equipe>=0&&m_inventaire[i].m_equipe<(int)m_classe.emplacements.size())
                {
                    sprite.Resize(m_classe.emplacements[m_inventaire[i].m_equipe].position.w*configuration->Resolution.w/800,m_classe.emplacements[m_inventaire[i].m_equipe].position.h*configuration->Resolution.h/600);

                    position.x=(m_classe.emplacements[m_inventaire[i].m_equipe].position.x + m_classe.emplacements[m_inventaire[i].m_equipe].position.w/2 - m_inventaire[i].getPositionImage().w/2 )*configuration->Resolution.w/800;
                    position.y=(m_classe.emplacements[m_inventaire[i].m_equipe].position.y + m_classe.emplacements[m_inventaire[i].m_equipe].position.h/2 - m_inventaire[i].getPositionImage().h/2 )*configuration->Resolution.h/600-decalage*configuration->Resolution.h/600;

                    position.h=m_classe.emplacements[m_inventaire[i].m_equipe].position.h*configuration->Resolution.w/800;
                    position.w=m_classe.emplacements[m_inventaire[i].m_equipe].position.w*configuration->Resolution.h/600;

                    sprite.SetX((m_classe.emplacements[m_inventaire[i].m_equipe].position.x)*configuration->Resolution.w/800);
                    sprite.SetY((m_classe.emplacements[m_inventaire[i].m_equipe].position.y)*configuration->Resolution.h/600-decalage*configuration->Resolution.h/600);
                    if(m_objetEnMain==-1&&positionSouris.x>m_classe.emplacements[m_inventaire[i].m_equipe].position.x*configuration->Resolution.x/800&&positionSouris.x<(m_classe.emplacements[m_inventaire[i].m_equipe].position.x+m_classe.emplacements[m_inventaire[i].m_equipe].position.w)*configuration->Resolution.x/800&&positionSouris.y>m_classe.emplacements[m_inventaire[i].m_equipe].position.y*configuration->Resolution.y/600&&positionSouris.y<(m_classe.emplacements[m_inventaire[i].m_equipe].position.y+m_classe.emplacements[m_inventaire[i].m_equipe].position.h)*configuration->Resolution.y/600)
                        m_inventaire[i].AfficherCaracteristiques(ecran,positionSouris);
                }
            }

            moteurGraphique->AjouterCommande(&sprite,16,0);

            //sprite.SetCenter(m_inventaire[i].getTaille().x/2,m_inventaire[i].getTaille().y/2);
            sprite.Resize(position.w-4*configuration->Resolution.w/800,position.h-4*configuration->Resolution.h/600);
            sprite.Move(2*configuration->Resolution.w/800,2*configuration->Resolution.h/600);

            moteurGraphique->AjouterCommande(&sprite,16,0);

            sprite.SetCenter(m_inventaire[i].getPositionImage().w/2,m_inventaire[i].getPositionImage().h/2);

            sprite.SetX(position.x+m_inventaire[i].getPositionImage().w/2*configuration->Resolution.w/800);
            sprite.SetY(position.y+m_inventaire[i].getPositionImage().h/2*configuration->Resolution.h/600);

            sprite.SetColor(m_inventaire[i].m_color);

            sprite.SetImage(*moteurGraphique->getImage(m_inventaire[i].getImage()));
            sprite.SetSubRect(IntRect(m_inventaire[i].getPositionImage().x, m_inventaire[i].getPositionImage().y, m_inventaire[i].getPositionImage().x+m_inventaire[i].getPositionImage().w, m_inventaire[i].getPositionImage().y+m_inventaire[i].getPositionImage().h));
            sprite.Resize(m_inventaire[i].getTaille().x*32*configuration->Resolution.w/800,m_inventaire[i].getTaille().y*32*configuration->Resolution.h/600);



            sprite.SetRotation(rotation);
            moteurGraphique->AjouterCommande(&sprite,17,0);
            sprite.SetRotation(0);
        }

    if(m_objetEnMain>=0&&m_objetEnMain<(int)m_inventaire.size())
    {
        sf::Sprite sprite;

        sprite.SetImage(*moteurGraphique->getImage(0));

        sprite.Resize(m_inventaire[m_objetEnMain].getTaille().x*32*configuration->Resolution.w/800,m_inventaire[m_objetEnMain].getTaille().y*32*configuration->Resolution.h/600);

        coordonnee caseVisee;
        caseVisee.x=((positionSouris.x+8*configuration->Resolution.x/800)-m_classe.position_contenu_inventaire.x*configuration->Resolution.x/800)/(32*configuration->Resolution.x/800) - m_inventaire[m_objetEnMain].getTaille().x/2;
        caseVisee.y=((positionSouris.y+8*configuration->Resolution.y/600)-(m_classe.position_contenu_inventaire.y-32)*configuration->Resolution.y/600)/(32*configuration->Resolution.y/600) - m_inventaire[m_objetEnMain].getTaille().y/2;

        sprite.SetX(positionSouris.x*configuration->Resolution.w/configuration->Resolution.x - m_inventaire[m_objetEnMain].getTaille().x*32*configuration->Resolution.x/1600);
        sprite.SetY(positionSouris.y*configuration->Resolution.h/configuration->Resolution.y - m_inventaire[m_objetEnMain].getTaille().y*32*configuration->Resolution.y/1200);

        if(m_inventaire[m_objetEnMain].getRarete()==NORMAL)
            sprite.SetColor(sf::Color(224,224,224,128));
        if(m_inventaire[m_objetEnMain].getRarete()==BONNEFACTURE)
            sprite.SetColor(sf::Color(128,0,128,128));
        if(m_inventaire[m_objetEnMain].getRarete()==BENI)
            sprite.SetColor(sf::Color(0,64,128,128));
        if(m_inventaire[m_objetEnMain].getRarete()==SACRE)
            sprite.SetColor(sf::Color(255,255,128,128));
        if(m_inventaire[m_objetEnMain].getRarete()==SANCTIFIE)
            sprite.SetColor(sf::Color(128,255,255,128));
        if(m_inventaire[m_objetEnMain].getRarete()==DIVIN)
            sprite.SetColor(sf::Color(255,164,32,128));
        if(m_inventaire[m_objetEnMain].getRarete()==INFERNAL)
            sprite.SetColor(sf::Color(224,0,0,128));
        if(m_inventaire[m_objetEnMain].getRarete()==CRAFT)
            sprite.SetColor(sf::Color(128,64,0,128));

        if(positionSouris.x<m_classe.position_contenu_inventaire.x*configuration->Resolution.x/800||positionSouris.x>m_classe.position_contenu_inventaire.x*configuration->Resolution.x/800+32*m_classe.position_contenu_inventaire.w*configuration->Resolution.x/800||positionSouris.y<(m_classe.position_contenu_inventaire.y-32)*configuration->Resolution.y/600||positionSouris.y>(m_classe.position_contenu_inventaire.y-32)*configuration->Resolution.y/600+32*m_classe.position_contenu_inventaire.h*configuration->Resolution.y/600)
        {
            if(positionSouris.x>435*configuration->Resolution.x/800&&positionSouris.x<758*configuration->Resolution.x/800&&positionSouris.y>30*configuration->Resolution.y/600&&positionSouris.y<114*configuration->Resolution.x/600)
                sprite.SetColor(sf::Color(128,0,0,128));
        }
        else
        {
            sprite.SetX(caseVisee.x*32*configuration->Resolution.w/800 + m_classe.position_contenu_inventaire.x*configuration->Resolution.w/800);
            sprite.SetY(caseVisee.y*32*configuration->Resolution.h/600 + (m_classe.position_contenu_inventaire.y-32)*configuration->Resolution.h/600);
        }

        moteurGraphique->AjouterCommande(&sprite,18,0);

        sprite.SetColor(m_inventaire[m_objetEnMain].m_color);

        sprite.SetImage(*moteurGraphique->getImage(m_inventaire[m_objetEnMain].getImage()));
        sprite.SetSubRect(IntRect(m_inventaire[m_objetEnMain].getPositionImage().x, m_inventaire[m_objetEnMain].getPositionImage().y, m_inventaire[m_objetEnMain].getPositionImage().x+m_inventaire[m_objetEnMain].getPositionImage().w, m_inventaire[m_objetEnMain].getPositionImage().y+m_inventaire[m_objetEnMain].getPositionImage().h));
        sprite.Resize(m_inventaire[m_objetEnMain].getTaille().x*32*configuration->Resolution.w/800,m_inventaire[m_objetEnMain].getTaille().y*32*configuration->Resolution.h/600);

        sprite.SetX(positionSouris.x*configuration->Resolution.w/configuration->Resolution.x - m_inventaire[m_objetEnMain].getTaille().x*32*configuration->Resolution.x/1600);
        sprite.SetY(positionSouris.y*configuration->Resolution.h/configuration->Resolution.y - m_inventaire[m_objetEnMain].getTaille().y*32*configuration->Resolution.y/1200);

        /*if(positionSouris.x<m_classe.position_contenu_inventaire.x*configuration->Resolution.x/800||positionSouris.x>m_classe.position_contenu_inventaire.x*configuration->Resolution.y/800+32*10*configuration->Resolution.x/800||positionSouris.y<m_classe.position_contenu_inventaire.y*configuration->Resolution.y/600||positionSouris.y>m_classe.position_contenu_inventaire.y*configuration->Resolution.y/600+32*10*configuration->Resolution.y/600)
        {
            sprite.SetX(positionSouris.x- m_inventaire[m_objetEnMain].getTaille().x*32*configuration->Resolution.x/1600);
            sprite.SetY(positionSouris.y- m_inventaire[m_objetEnMain].getTaille().y*32*configuration->Resolution.y/1200);
        }*/

        moteurGraphique->AjouterCommande(&sprite,19,0);
    }
    else if(positionSouris.x>m_classe.position_contenu_inventaire.x*configuration->Resolution.x/800&&positionSouris.x<m_classe.position_contenu_inventaire.x*configuration->Resolution.x/800+32*m_classe.position_contenu_inventaire.w*configuration->Resolution.x/800&&positionSouris.y>(m_classe.position_contenu_inventaire.y-32)*configuration->Resolution.y/600&&positionSouris.y<(m_classe.position_contenu_inventaire.y-32)*configuration->Resolution.y/600+32*m_classe.position_contenu_inventaire.h*configuration->Resolution.y/600)
    {
        coordonnee caseVisee;
        caseVisee.x=(positionSouris.x-m_classe.position_contenu_inventaire.x*configuration->Resolution.x/800)/(32*configuration->Resolution.x/800);
        caseVisee.y=(positionSouris.y-(m_classe.position_contenu_inventaire.y-32)*configuration->Resolution.y/600)/(32*configuration->Resolution.y/600);

        for(int i=0;i<(int)m_inventaire.size();i++)
            if(caseVisee.x>=m_inventaire[i].getPosition().x&&caseVisee.x<=m_inventaire[i].getPosition().x+m_inventaire[i].getTaille().x-1
             &&caseVisee.y>=m_inventaire[i].getPosition().y&&caseVisee.y<=m_inventaire[i].getPosition().y+m_inventaire[i].getTaille().y-1)
                 if(m_inventaire[i].m_equipe==-1)
                 {
                    positionSouris.y-=32*configuration->Resolution.h/600;
                    m_inventaire[i].AfficherCaracteristiques(ecran,positionSouris);
                 }
    }



    afficherCaracteristiques(ecran,positionSouris,decalage);
}

void Hero::placerCamera(sf::View *camera,coordonnee dimensionsMap)
{
	m_positionAffichage.y=(((m_personnage.getCoordonneePixel().x+m_personnage.getCoordonneePixel().y)*64/COTE_TILE)/2);
	m_positionAffichage.x=(((m_personnage.getCoordonneePixel().x-m_personnage.getCoordonneePixel().y)*64/COTE_TILE+dimensionsMap.y*64)-64);

	coordonneeDecimal positionCamera;
	positionCamera.y=m_positionAffichage.y-250*configuration->Resolution.h/600 ;
	positionCamera.x=m_positionAffichage.x-((400*configuration->Resolution.w/800))+64;

	camera->SetFromRect(sf::FloatRect(positionCamera.x,positionCamera.y,positionCamera.x+configuration->Resolution.w,positionCamera.y+configuration->Resolution.h));
}

bool Hero::testMonstreVise(Monstre *monstre,int hauteurMap)
{
    if(m_monstreVise>-1&&m_caracteristiques.vie>0)
    {
        if((fabs(m_personnage.getCoordonnee().x-monstre->getCoordonnee().x)<=1&&fabs(m_personnage.getCoordonnee().y-monstre->getCoordonnee().y)<=1) || (fabs(m_personnage.getCoordonnee().x-monstre->getProchaineCase().x)<=1&&fabs(m_personnage.getCoordonnee().y-monstre->getProchaineCase().y)<=1))
        {
            m_personnage.setArrivee(m_personnage.getCoordonnee());
            m_personnage.frappe(m_personnage.getCoordonnee(),monstre->getCoordonnee());

            return 1;
        }
        else
             m_personnage.setArrivee(monstre->getProchaineCase());
    }
    return 0;
}

void Hero::augmenterAme(float temps)
{
    Caracteristique temp = m_personnage.getCaracteristique();

    temp.ancienPointAme+=(temp.pointAme-temp.positionAncienAme)*temps*0.7;

    if(temp.ancienPointAme>=temp.pointAme)
        temp.ancienPointAme=temp.pointAme,temp.positionAncienAme=temp.ancienPointAme;

    if(temp.ancienPointAme>=CALCUL_PA_PROCHAIN_NIVEAU)
    {
        temp.niveau++;
        temp.force++;
        temp.dexterite++;
        temp.vitalite++;
        temp.piete++;
        temp.charisme++;

        temp.maxVie=temp.vitalite*25;
        temp.vie=temp.maxVie;

        temp.maxFoi=temp.piete*25;
        temp.foi=temp.maxFoi;

        recalculerCaracteristiques();

        m_caracteristiques.vie=m_caracteristiques.maxVie;
        m_caracteristiques.foi=m_caracteristiques.maxFoi;
    }

    m_personnage.setCaracteristique(temp);

    m_caracteristiques.ancienPointAme=temp.ancienPointAme;

    //recalculerCaracteristiques();
}

void Hero::recalculerCaracteristiques()
{
    float vie=m_caracteristiques.vie,foi=m_caracteristiques.foi;

    Caracteristique temp=m_personnage.getCaracteristique();
    temp.maxVie=temp.vitalite*10;
    temp.maxFoi=temp.piete*10;


    m_caracteristiques=m_personnage.getCaracteristique();
    m_caracteristiques.maxVie=0;
    m_caracteristiques.maxFoi=0;

    for(int i=0;i<(int)m_inventaire.size();i++)
        if(m_inventaire[i].m_equipe>=0)
            for(int j=0;j<(int)m_inventaire[i].m_benedictions.size();++j)
                switch (m_inventaire[i].m_benedictions[j].type)
                {
                    case FO_SUPP:
                        m_caracteristiques.force+=m_inventaire[i].m_benedictions[j].info1;
                    break;
                    case VIT_SUPP:
                        m_caracteristiques.vitalite+=m_inventaire[i].m_benedictions[j].info1;
                    break;
                    case PI_SUPP:
                        m_caracteristiques.piete+=m_inventaire[i].m_benedictions[j].info1;
                    break;
                    case DEX_SUPP:
                        m_caracteristiques.dexterite+=m_inventaire[i].m_benedictions[j].info1;
                    break;
                    case CH_SUPP:
                        m_caracteristiques.charisme+=m_inventaire[i].m_benedictions[j].info1;
                    break;
                    case VIE_SUPP:
                        m_caracteristiques.maxVie+=m_inventaire[i].m_benedictions[j].info1;
                    break;
                    case FOI_SUPP:
                        m_caracteristiques.maxFoi+=m_inventaire[i].m_benedictions[j].info1;
                    break;
                }

    m_caracteristiques.degatsMin=m_caracteristiques.force/3;
    m_caracteristiques.degatsMax=(int)(m_caracteristiques.force)/2;
    m_caracteristiques.armure=m_caracteristiques.dexterite;

    temp.degatsMin=m_caracteristiques.force/3;
    temp.degatsMax=(int)(m_caracteristiques.force)/2;
    temp.armure=m_caracteristiques.dexterite;

    m_caracteristiques.maxVie+=m_caracteristiques.vitalite*10;
    m_caracteristiques.maxFoi+=m_caracteristiques.piete*10;


    for(int i=0;i<(int)m_inventaire.size();i++)
    {
        if(m_inventaire[i].m_equipe>=0)
        {
            int accru=100;
            for(int j=0;j<(int)m_inventaire[i].m_benedictions.size();++j)
                if(m_inventaire[i].m_benedictions[j].type==EFFICACITE_ACCRUE)
                    accru+=m_inventaire[i].m_benedictions[j].info1;

            m_caracteristiques.degatsMin+=m_inventaire[i].m_degatsMin*accru/100;
            m_caracteristiques.degatsMax+=m_inventaire[i].m_degatsMax*accru/100;
            m_caracteristiques.armure+=m_inventaire[i].m_armure*accru/100;

            temp.degatsMin+=m_inventaire[i].m_degatsMin;
            temp.degatsMax+=m_inventaire[i].m_degatsMax;
            temp.armure+=m_inventaire[i].m_armure;
        }
    }

    m_caracteristiques.niveau=temp.niveau;

    m_personnage.setCaracteristique(temp);

    m_caracteristiques.vie=vie,m_caracteristiques.foi=foi;

    if(m_caracteristiques.vie>m_caracteristiques.maxVie)
        m_caracteristiques.vie=m_caracteristiques.maxVie;
    if(m_caracteristiques.foi>m_caracteristiques.maxFoi)
        m_caracteristiques.foi=m_caracteristiques.maxFoi;
}

int Hero::utiliserClicDroit(coordonnee positionSouris, int monstreVise)
{
    if(!m_personnage.frappeEnCours)
        if(m_caracteristiques.foi-(m_caracteristiques.degatsMin+m_caracteristiques.degatsMax)*1.5>0)
        {
            coordonnee temp;
            temp.x=configuration->Resolution.x/2;
            temp.y=configuration->Resolution.y/2;

            m_monstreVise=-1;

            if(m_monstreVise==-1)
                m_personnage.frappe(positionSouris,temp);

            m_personnage.setArrivee(m_personnage.getCoordonnee());

            miracleEnCours=1;
           // m_personnage.frappeEnCours=1;

            m_caracteristiques.foi-=(m_caracteristiques.degatsMin+m_caracteristiques.degatsMax)*1.5;

            return 1;
        }
    return 0;
}

void Hero::GenererGrille()
{
    for(int i=0;i<m_classe.position_contenu_inventaire.h;i++)
            for(int j=0;j<m_classe.position_contenu_inventaire.w;j++)
                m_caseInventaire[i][j]=0;

    for(int i=0;i<(int)m_inventaire.size();i++)
        if(m_inventaire[i].m_equipe==-1&&i!=m_objetEnMain)
            for(int y=0;y<m_inventaire[i].getTaille().y;y++)
                for(int x=0;x<m_inventaire[i].getTaille().x;x++)
                    m_caseInventaire[(int)(y+m_inventaire[i].getPosition().y)][(int)(x+m_inventaire[i].getPosition().x)]=1;
}

bool Hero::ajouterObjet(Objet objet,bool enMain)
{
    bool ramasser=false;

    if(!enMain)
    {
        for(int i=0;i<m_classe.position_contenu_inventaire.h;i++)
            for(int j=0;j<m_classe.position_contenu_inventaire.w;j++)
            {
                if(!m_caseInventaire[i][j])
                {
                    bool ok=true;
                    for(int x=j;x<j+objet.getTaille().x;x++)
                        for(int y=i;y<i+objet.getTaille().y;y++)
                            if(x<m_classe.position_contenu_inventaire.w&&y<m_classe.position_contenu_inventaire.h)
                            {
                                if(m_caseInventaire[y][x])
                                    ok=false;
                            }
                            else
                            {
                                ok=false;
                            }

                    if(ok)
                    {
                        objet.setPosition(j,i);
                        m_inventaire.push_back(objet);

                        for(int x=j;x<j+objet.getTaille().x;x++)
                            for(int y=i;y<i+objet.getTaille().y;y++)
                                m_caseInventaire[y][x]=1;

                        ramasser=true;

                        i=m_classe.position_contenu_inventaire.h,j=m_classe.position_contenu_inventaire.w;
                    }
                }
            }
    }
    else if(m_objetEnMain==-1)
    {
        m_inventaire.push_back(objet);

        m_objetEnMain=m_inventaire.size()-1;

        ramasser=true;
    }

    GenererGrille();

    return ramasser;
}

void Hero::AttribuerPositionObjet(coordonnee position,int numero)
{
    if(numero>=0&&numero<(int)m_inventaire.size())
    {
        m_inventaire[numero].setPosition(position.x,position.y);

        m_inventaire[numero].m_equipe=-1;

        GenererGrille();
    }
}

Objet Hero::DeposerObjet()
{
    Objet temp;
    if(m_objetADeposer>=0&&m_objetADeposer<(int)m_inventaire.size())
    {
        temp=m_inventaire[m_objetADeposer];

        m_inventaire.erase(m_inventaire.begin()+m_objetADeposer);

        temp.setPosition(m_personnage.getCoordonnee().x,m_personnage.getCoordonnee().y);

        temp.m_equipe=-1;

        recalculerCaracteristiques();

        m_objetADeposer=-1;
        m_objetEnMain=-1;
    }
    return temp;
}

bool Hero::prendreEnMain(coordonnee positionSouris)
{
    m_objetADeposer=-1;
    if(positionSouris.x>m_classe.position_contenu_inventaire.x*configuration->Resolution.x/800&&positionSouris.x<m_classe.position_contenu_inventaire.x*configuration->Resolution.x/800+32*m_classe.position_contenu_inventaire.w*configuration->Resolution.x/800&&positionSouris.y>(m_classe.position_contenu_inventaire.y-32)*configuration->Resolution.y/600&&positionSouris.y<(m_classe.position_contenu_inventaire.y-32)*configuration->Resolution.y/600+32*m_classe.position_contenu_inventaire.h*configuration->Resolution.y/600)
    {
        coordonnee caseVisee;
        if(m_objetEnMain>=0&&m_objetEnMain<(int)m_inventaire.size())
        {
            caseVisee.x=((positionSouris.x+8*configuration->Resolution.x/800)-m_classe.position_contenu_inventaire.x*configuration->Resolution.x/800)/(32*configuration->Resolution.x/800) - m_inventaire[m_objetEnMain].getTaille().x/2;
            caseVisee.y=((positionSouris.y+8*configuration->Resolution.y/600)-(m_classe.position_contenu_inventaire.y-32)*configuration->Resolution.y/600)/(32*configuration->Resolution.y/600) - m_inventaire[m_objetEnMain].getTaille().y/2;
        }
        else
        {
            caseVisee.x=((positionSouris.x)-m_classe.position_contenu_inventaire.x*configuration->Resolution.x/800)/(32*configuration->Resolution.x/800);
            caseVisee.y=((positionSouris.y)-(m_classe.position_contenu_inventaire.y-32)*configuration->Resolution.y/600)/(32*configuration->Resolution.y/600);
        }

        if(m_objetEnMain>=0&&m_objetEnMain<(int)m_inventaire.size())
        {
            int temp=-1;
            bool ok=true;

            for(int i=caseVisee.y;i<caseVisee.y+m_inventaire[m_objetEnMain].getTaille().y;i++)
                for(int j=caseVisee.x;j<caseVisee.x+m_inventaire[m_objetEnMain].getTaille().x;j++)
                    if(i<m_classe.position_contenu_inventaire.h&&j<m_classe.position_contenu_inventaire.w&&i>=0&&j>=0)
                    {
                       // if(m_caseInventaire[i][j])
                            for(int z=0;z<(int)m_inventaire.size();z++)
                                for(int y=m_inventaire[z].getPosition().y;y<m_inventaire[z].getPosition().y+m_inventaire[z].getTaille().y;y++)
                                    for(int x=m_inventaire[z].getPosition().x;x<m_inventaire[z].getPosition().x+m_inventaire[z].getTaille().x;x++)
                                        if(m_inventaire[z].m_equipe==-1)
                                            if(j==x&&i==y&&m_objetEnMain!=z)
                                            {
                                                if(temp!=z&&temp!=-1)
                                                    ok=false;

                                                temp=z;
                                            }
                    }
                    else
                        ok=false;

            if(ok)
            {
                //if(temp!=-1)
                  //  LibererCases(temp);

                AttribuerPositionObjet(caseVisee,m_objetEnMain);

                m_objetEnMain=-1;

                if(temp!=-1)
                    m_objetEnMain=temp;
            }
        }
        else
        {
            for(int z=0;z<(int)m_inventaire.size();z++)
                if(m_inventaire[z].m_equipe==-1)
                    if(caseVisee.x>=m_inventaire[z].getPosition().x&&caseVisee.x<m_inventaire[z].getPosition().x+m_inventaire[z].getTaille().x
                     &&caseVisee.y>=m_inventaire[z].getPosition().y&&caseVisee.y<m_inventaire[z].getPosition().y+m_inventaire[z].getTaille().y)
                        m_objetEnMain=z/*,LibererCases(m_objetEnMain)*/;
        }

    }
    else
    {
        if(positionSouris.x>435*configuration->Resolution.x/800&&positionSouris.x<758*configuration->Resolution.x/800&&positionSouris.y>30*configuration->Resolution.y/600&&positionSouris.y<114*configuration->Resolution.y/600)
        {
            if(m_objetEnMain>=0&&m_objetEnMain<(int)m_inventaire.size())
            {
                m_objetADeposer=m_objetEnMain;
                m_objetEnMain=-1;

                GenererGrille();
                recalculerCaracteristiques();

                return 1;

            }
        }
        else
        {
            bool equipe=false;

            for(int k=0;k<(int)m_classe.emplacements.size();k++)
            {
                if(positionSouris.x>m_classe.emplacements[k].position.x*configuration->Resolution.x/800&&positionSouris.x<(m_classe.emplacements[k].position.x+m_classe.emplacements[k].position.w)*configuration->Resolution.x/800&&positionSouris.y>m_classe.emplacements[k].position.y*configuration->Resolution.y/600&&positionSouris.y<(m_classe.emplacements[k].position.y+m_classe.emplacements[k].position.h)*configuration->Resolution.y/600)
                    equipe=true,equiper(m_objetEnMain,k);

            }

            if(!equipe&&m_objetEnMain>=0&&m_objetEnMain<(int)m_inventaire.size())
                if(m_inventaire[m_objetEnMain].m_equipe==-1)
                {
                    for(int i=0;i<m_classe.position_contenu_inventaire.h;i++)
                        for(int j=0;j<m_classe.position_contenu_inventaire.w;j++)
                            if(!m_caseInventaire[i][j])
                            {
                                bool ok=true;
                                for(int x=j;x<j+m_inventaire[m_objetEnMain].getTaille().x;x++)
                                    for(int y=i;y<i+m_inventaire[m_objetEnMain].getTaille().y;y++)
                                        if(x<m_classe.position_contenu_inventaire.w&&y<m_classe.position_contenu_inventaire.h)
                                        {
                                            if(m_caseInventaire[y][x])
                                                ok=false;
                                        }
                                        else
                                        {
                                            ok=false;
                                        }

                                if(ok)
                                {
                                    coordonnee pos={j,i,-1,-1};
                                    m_inventaire[m_objetEnMain].setPosition(j,i);

                                    AttribuerPositionObjet(pos,m_objetEnMain);

                                    for(int x=j;x<j+m_inventaire[m_objetEnMain].getTaille().x;x++)
                                        for(int y=i;y<i+m_inventaire[m_objetEnMain].getTaille().y;y++)
                                                m_caseInventaire[y][x]=1;

                                    m_objetEnMain=-1;

                                    i=m_classe.position_contenu_inventaire.h,j=m_classe.position_contenu_inventaire.w;
                                }
                            }
                }
                else
                {
                    m_objetEnMain=-1;
                }
        }
    }

    recalculerCaracteristiques();
    GenererGrille();
    return 0;
}

bool Hero::equiper(int numero, int emplacement)
{
    GenererGrille();
    int ancienEquipe=-1;
    bool ok=true;

    if(m_objetEnMain>=0 && m_objetEnMain<(int)m_inventaire.size())
    {
        ok=false;
        for(int i=0;i<(int)m_inventaire[m_objetEnMain].m_emplacement.size();i++)
        {
            if(m_inventaire[m_objetEnMain].m_emplacement[i]==m_classe.emplacements[emplacement].emplacement)
                ok=true;
        }

        if(ok)
        {
            for(int i=0;i<(int)m_inventaire.size();i++)
            {
                if(m_inventaire[i].m_equipe==emplacement)
                    if(numero!=i)
                        ancienEquipe=i;

                for(int j=0;j<(int)m_inventaire[i].m_emplacementImpossible.size();j++)
                    if(numero!=i)
                        if(m_inventaire[i].m_equipe>=0)
                            if(m_inventaire[i].m_emplacementImpossible[j]==m_classe.emplacements[emplacement].emplacement)
                                ok=false;


                if(m_objetEnMain>=0&&m_objetEnMain<(int)m_inventaire.size())
                    for(int j=0;j<(int)m_inventaire[m_objetEnMain].m_emplacementImpossible.size();j++)
                        if(m_inventaire[i].m_equipe>=0&&m_inventaire[i].m_equipe<(int)m_classe.emplacements.size())
                            if(m_classe.emplacements[m_inventaire[i].m_equipe].emplacement==m_inventaire[m_objetEnMain].m_emplacementImpossible[j])
                                ok=false;
            }

            if(ok)
                if(numero>=0&&numero<(int)m_inventaire.size())
                {
                    ok=false;
                    for(int i=0;i<(int)m_inventaire[numero].m_emplacement.size();++i)
                        if(m_inventaire[numero].m_emplacement[i]==m_classe.emplacements[emplacement].emplacement)
                            ok=true;

                    if(ok)
                        m_inventaire[numero].m_equipe=emplacement;

                    m_inventaire[ancienEquipe].m_equipe=-1;

                }

            if(ok)
            {
                m_objetEnMain=ancienEquipe;

            }
        }
    }
    else
    {
        for(int i=0;i<(int)m_inventaire.size();i++)
            if(m_inventaire[i].m_equipe==emplacement)
                m_objetEnMain=i;
    }

    recalculerCaracteristiques();
    return 0;
}

void Hero::infligerDegats(float degats)
{
    degats-=m_caracteristiques.armure/50;
    if(degats<0)
        degats=0;

    m_caracteristiques.vie-=degats;
    if(m_caracteristiques.vie<=0)
        m_personnage.infligerDegats(m_caracteristiques.maxVie);
}
void Hero::regenererVie(float vie)
{
    m_caracteristiques.vie+=vie;
    if(m_caracteristiques.vie>m_caracteristiques.maxVie)
        m_caracteristiques.vie=m_caracteristiques.maxVie;

    Caracteristique temp=m_personnage.getCaracteristique();
    temp.vie=m_caracteristiques.vie;
    m_personnage.setCaracteristique(temp);
}
void Hero::regenererFoi(float foi)
{
    m_caracteristiques.foi+=foi;
    if(m_caracteristiques.foi>m_caracteristiques.maxFoi)
        m_caracteristiques.foi=m_caracteristiques.maxFoi;

    Caracteristique temp=m_personnage.getCaracteristique();
    temp.foi=m_caracteristiques.foi;
    m_personnage.setCaracteristique(temp);
}

void Hero::setMonstreVise(int monstre){m_monstreVise=monstre;}
void Hero::setChercherSac(coordonnee a){m_chercherSac=a;}
void Hero::setSacVise(coordonnee a){m_sacVise=a;}

Lumiere Hero::getPorteeLumineuse()
{
    Lumiere moyenne;

    moyenne.rouge=255;
    moyenne.vert=255;
    moyenne.bleu=255;
    moyenne.intensite=192;

    /*for(int i=0;i<NOMBRE_MORCEAU_PERSONNAGE;i++)
    {
        moyenne.rouge=(moyenne.rouge*moyenne.intensite+m_modelePersonnage[i].getPorteeLumineuse().rouge*m_modelePersonnage[i].getPorteeLumineuse().intensite)/(m_modelePersonnage[i].getPorteeLumineuse().intensite+moyenne.intensite);
        moyenne.vert=(moyenne.vert*moyenne.intensite+m_modelePersonnage[i].getPorteeLumineuse().vert*m_modelePersonnage[i].getPorteeLumineuse().intensite)/(m_modelePersonnage[i].getPorteeLumineuse().intensite+moyenne.intensite);
        moyenne.bleu=(moyenne.bleu*moyenne.intensite+m_modelePersonnage[i].getPorteeLumineuse().bleu*m_modelePersonnage[i].getPorteeLumineuse().intensite)/(m_modelePersonnage[i].getPorteeLumineuse().intensite+moyenne.intensite);

        moyenne.intensite+=m_modelePersonnage[i].getPorteeLumineuse().intensite;

        if(moyenne.intensite>255)
            moyenne.intensite=255;
        if(moyenne.intensite<0)
            moyenne.intensite=0;
    }*/

    return moyenne;
}

coordonnee Hero::getChercherSac(){return m_chercherSac;};
coordonnee Hero::getSacVise(){return m_sacVise;};
int Hero::getMonstreVise(){return m_monstreVise;}
#endif
