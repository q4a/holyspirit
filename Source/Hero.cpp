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

	for(int i=0;i<10;++i)
        for(int j=0;j<10;++j)
            m_caseInventaire[i][j]=0;

    m_objetEnMain=-1;

    recalculerCaracteristiques();

    miracleEnCours=0;

    m_cas=0;

    for(int i=0;i<NOMBRE_MORCEAU_PERSONNAGE;i++)
            m_cheminModeleNouveau[i]="",m_cheminModele[i]="";
}

void Hero::Sauvegarder()
{
    console.Ajouter("");
    console.Ajouter("Sauvegarde du héro...");

    ofstream fichier((configuration.chemin_saves+"hero.sav.hs").c_str(), ios::out | ios::trunc | ios::binary);
    if(fichier)
    {
        if(configuration.debug)
            console.Ajouter("Ouverture du fichier.");

        std::string temp;
        char caractere,espace='_';
        int tempInt;
        float tempFloat;


        int n = m_personnage.getCaracteristique().nom.size()+1;
        fichier.write((char *)&n, sizeof(int));
        fichier.write(m_personnage.getCaracteristique().nom.c_str(), n);

        //fichier.write((char*)&m_personnage.getCaracteristique().nom, sizeof(string));

        fichier.write((char*)&m_personnage.getCaracteristique().vitesse, sizeof(float));

        fichier.write((char*)&m_personnage.getCaracteristique().pointAme, sizeof(int));

        fichier.write((char*)&m_personnage.getCaracteristique().niveau, sizeof(int));

        fichier.write((char*)&m_personnage.getCaracteristique().force, sizeof(int));

        fichier.write((char*)&m_personnage.getCaracteristique().dexterite, sizeof(int));

        fichier.write((char*)&m_personnage.getCaracteristique().vitalite, sizeof(int));

        fichier.write((char*)&m_personnage.getCaracteristique().piete, sizeof(int));

        fichier.write((char*)&m_personnage.getCaracteristique().charisme, sizeof(int));

        if(configuration.debug)
            console.Ajouter("/Ecriture des caracterstiques.");


       // caractere='$';
        //fichier.write((char*)&caractere, sizeof(char));
        //fichier.write((char*)&espace, sizeof(char));

        for(int i=0;i<10;++i)
        {
            caractere='*';
            fichier.write((char*)&caractere, sizeof(char));
            //fichier.write((char*)&espace, sizeof(char));

            for(int j=0;j<10;++j)
                fichier.write((char*)&m_caseInventaire[i][j], sizeof(bool));

            caractere='$';
            //fichier.write((char*)&espace, sizeof(char));
            fichier.write((char*)&caractere, sizeof(char));
            fichier.write((char*)&espace, sizeof(char));
        }
        caractere='$';
        //fichier.write((char*)&espace, sizeof(char));
        fichier.write((char*)&caractere, sizeof(char));

        if(configuration.debug)
            console.Ajouter("/Ecriture des cases inventaires.");

        for(int i=0;i<m_inventaire.size();++i)
        {
            m_inventaire[i].Sauvegarder(&fichier);
        }

        if(configuration.debug)
            console.Ajouter("/Ecriture des objets.");

       // fichier.write((char*)&espace, sizeof(char));
        caractere='$';
        fichier.write((char*)&caractere, sizeof(char));

        fichier.close();

        console.Ajouter("Sauvegarde du héro terminée !");

    }
    else
        console.Ajouter("Impossible de sauvegarder le héro !",1);



}
void Hero::Charger()
{

    for(int i=0;i<NOMBRE_MORCEAU_PERSONNAGE;i++)
        m_cheminModele[i]="";


    bool nouveau=true;
    string chemin;
    struct dirent *lecture;

	DIR *repertoire;
    repertoire = opendir(configuration.chemin_saves.c_str());
    while ((lecture = readdir(repertoire)))
    {
        string temp="hero.sav.hs";
        if(lecture->d_name==temp)
        {
            ifstream fichier;
            fichier.open((configuration.chemin_saves+"hero.sav.hs").c_str(), ios::in | ios::binary);
            if(fichier)
            {
                char caractere;
                Caracteristique charTemp;
                charTemp=m_personnage.getCaracteristique();

                int n;
                char *buffer;

                fichier.read((char *)&n, sizeof(int));
                buffer=new char[n];
                fichier.read(buffer,n);
                charTemp.nom=buffer;
                delete[] buffer;


                //fichier.read((char*)&charTemp.nom, sizeof(string));
                fichier.read((char*)&charTemp.vitesse, sizeof(float));
                fichier.read((char*)&charTemp.pointAme, sizeof(int)),charTemp.ancienPointAme=charTemp.pointAme,charTemp.positionAncienAme=charTemp.pointAme;
                fichier.read((char*)&charTemp.niveau, sizeof(int));

                fichier.read((char*)&charTemp.force, sizeof(int));
                fichier.read((char*)&charTemp.dexterite, sizeof(int));
                fichier.read((char*)&charTemp.vitalite, sizeof(int));
                fichier.read((char*)&charTemp.piete, sizeof(int));
                fichier.read((char*)&charTemp.charisme, sizeof(int));

                if(configuration.debug)
                    console.Ajouter("/Lectures des caracteristiques.");

                    coordonnee position={0,0,0,0};

                    do
                    {
                        fichier.read((char*)&caractere, sizeof(char));
                        if(caractere=='*')
                        {
                            for(int i=0;i<10;i++)
                            {
                                bool temp;
                                //fichier>>temp;
                                fichier.read((char*)&temp, sizeof(bool));
                                m_caseInventaire[position.y][position.x]=temp,position.x++;

                            }

                            do
                            {
                                fichier.read((char*)&caractere, sizeof(char));
                                 if(fichier.eof()){throw "Impossible de charger la sauvegarde";}
                            }while(caractere!='$');

                            fichier.read((char*)&caractere, sizeof(char));

                            position.y++;
                            position.x=0;

                            if(position.y>=10)
                                position.y=9;
                        }
                        if(fichier.eof()){throw "Impossible de charger la sauvegarde";}

                     }while(caractere!='$');

                     if(configuration.debug)
                            console.Ajouter("/Lectures des cases de l'inventaire.");

                    do
                    {
                        fichier.read((char*)&caractere, sizeof(char));
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
                                fichier.read((char*)&caractere, sizeof(char));
                                if(caractere=='e')
                                    fichier.read((char*)&equipe, sizeof(int));//fichier>>equipe;

                                else if(caractere=='r')
                                    fichier.read((char*)&rarete, sizeof(int));//fichier>>rarete;

                                else if(caractere=='x')
                                    fichier.read((char*)&position.x, sizeof(int));//fichier>>position.x;
                                else if(caractere=='y')
                                    fichier.read((char*)&position.y, sizeof(int));//fichier>>position.y;

                                else  if(caractere=='d')
                                {
                                    fichier.read((char*)&caractere, sizeof(char));
                                    if(caractere=='i')
                                       fichier.read((char*)&degMin, sizeof(int)); //fichier>>degMin;
                                    else if(caractere=='a')
                                       fichier.read((char*)&degMax, sizeof(int)); // fichier>>degMax;
                                }

                                else if(caractere=='a')
                                    fichier.read((char*)&armure, sizeof(int));//fichier>>armure;

                                else  if(caractere=='l')
                                {
                                    fichier.read((char*)&caractere, sizeof(char));
                                    if(caractere=='r')
                                        fichier.read((char*)&color.r, sizeof(Uint8));//fichier>>color.r;
                                    else if(caractere=='g')
                                        fichier.read((char*)&color.g, sizeof(Uint8));//fichier>>color.g;
                                    else if(caractere=='b')
                                       fichier.read((char*)&color.b, sizeof(Uint8)); //fichier>>color.b;
                                }
                                else if(caractere=='m')
                                {
                                    int m;
                                    char *buffer;

                                    fichier.read((char*)&m, sizeof(int));
                                    buffer=new char[m];
                                    fichier.read(buffer,m);


                                    m_inventaire.push_back(Objet ());
                                    m_inventaire[m_inventaire.size()-1].Charger(buffer);
                                    delete[] buffer;
                                }
                                else if(caractere=='b')
                                {
                                    int type=0,info1=0,info2=0;

                                    //fichier>>type;
                                    fichier.read((char*)&type, sizeof(int));

                                    do
                                    {
                                        fichier.read((char*)&caractere, sizeof(char));
                                        if(caractere=='i')
                                        {
                                            fichier.read((char*)&caractere, sizeof(char));
                                            if(caractere=='1')
                                                fichier.read((char*)&info1, sizeof(int));//fichier>>info1;
                                            if(caractere=='2')
                                                fichier.read((char*)&info2, sizeof(int));//fichier>>info2;
                                        }
                                    }while(caractere!='$');

                                    bene.push_back(benediction ());
                                    bene[bene.size()-1].type=type;
                                    bene[bene.size()-1].info1=info1;
                                    bene[bene.size()-1].info2=info2;
                                    fichier.read((char*)&caractere, sizeof(char));
                                }

                                if(fichier.eof()){throw "Impossible de charger la sauvegarde";}
                            }while(caractere!='$');

                            if(m_inventaire.size()>0)
                            {

                                m_inventaire[m_inventaire.size()-1].setRarete(rarete);
                                m_inventaire[m_inventaire.size()-1].setPosition(position.x,position.y);
                                m_inventaire[m_inventaire.size()-1].m_equipe=equipe;

                                m_inventaire[m_inventaire.size()-1].m_armure=armure;
                                m_inventaire[m_inventaire.size()-1].m_degatsMin=degMin;
                                m_inventaire[m_inventaire.size()-1].m_degatsMax=degMax;

                                m_inventaire[m_inventaire.size()-1].m_color=color;


                                m_inventaire[m_inventaire.size()-1].m_benedictions=bene;
                            }

                            fichier.read((char*)&caractere, sizeof(char));
                        }
                        if(fichier.eof()){throw "Impossible de charger la sauvegarde";}

                    }while(caractere!='$');

                    if(configuration.debug)
                        console.Ajouter("/Lectures des objets.");



                m_personnage.setCaracteristique(charTemp);
                nouveau=false;
            }
            fichier.close();
        }
    }
    closedir(repertoire);
    recalculerCaracteristiques();

    m_caracteristiques.vie=m_caracteristiques.maxVie;
    m_caracteristiques.foi=m_caracteristiques.maxFoi;

    if(nouveau)
    {
        sf::Color color;
        color.r=255;
        color.g=255;
        color.b=255;
        m_inventaire.push_back(Objet ());
        m_inventaire[m_inventaire.size()-1].Charger("Data/Items/Small_sword.item.hs");

        m_inventaire[m_inventaire.size()-1].setRarete(0);
        m_inventaire[m_inventaire.size()-1].setPosition(0,0);
        m_inventaire[m_inventaire.size()-1].m_equipe=ARME_PRINCIPAL;

        m_inventaire[m_inventaire.size()-1].m_armure=0;
        m_inventaire[m_inventaire.size()-1].m_degatsMin=1;
        m_inventaire[m_inventaire.size()-1].m_degatsMax=2;

        m_inventaire[m_inventaire.size()-1].m_color=color;
    }

    ChargerModele();
}

void Hero::ReChargerModele()
{
    m_cas=0;

    int nombreArme=0;

    for(int i=0;i<m_inventaire.size();i++)
    {
        if(m_inventaire[i].m_equipe==ARME_PRINCIPAL&&m_inventaire[i].m_type==ARME||m_inventaire[i].m_equipe==BOUCLIER&&m_inventaire[i].m_type==ARME)
            nombreArme++;

        if(m_inventaire[i].m_equipe==ARME_PRINCIPAL&&m_inventaire[i].m_type==ARME)
            for(int j=0;j<m_inventaire[i].m_emplacementImpossible.size();j++)
                if(m_inventaire[i].m_emplacementImpossible[j]==BOUCLIER)
                    m_cas=2;
    }

    if(nombreArme==2)
        m_cas=1;



    bool pasEquipe[NOMBRE_MORCEAU_PERSONNAGE];

    for(int i=0;i<NOMBRE_MORCEAU_PERSONNAGE;i++)
        pasEquipe[i]=true,m_cheminModele[i]="",m_cheminModeleNouveau[i]="";


    for(int i=0;i<m_inventaire.size();i++)
        if(m_inventaire[i].m_equipe>0)
        {
                if(m_inventaire[i].m_emplacementImageHero.size()>0)
                {
                    int temp=m_cas;

                    if(m_inventaire[i].m_equipe==ARME_PRINCIPAL&&m_inventaire[i].m_type==ARME&&m_cas>0)
                        temp=0;

                    if(m_inventaire[i].m_equipe==BOUCLIER&&m_inventaire[i].m_type==ARME&&m_cas>0)
                        temp=1;

                    if(temp>=m_inventaire[i].m_emplacementImageHero.size())
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
        if(m_cas==0)
            if(m_cheminModele[0]!="Data/Entities/hero/Nude/OneHand.rs.hs")
            {
                m_modelePersonnage[0].Charger("Data/Entities/hero/Nude/OneHand.rs.hs");
                m_cheminModele[0]="Data/Entities/hero/Nude/OneHand.rs.hs";
            }
        if(m_cas==1)
            if(m_cheminModele[0]!="Data/Entities/hero/Nude/TwoWeapons.rs.hs")
            {
                m_modelePersonnage[0].Charger("Data/Entities/hero/Nude/TwoWeapons.rs.hs");
                m_cheminModele[0]="Data/Entities/hero/Nude/TwoWeapons.rs.hs";
            }
        if(m_cas==2)
            if(m_cheminModele[0]!="Data/Entities/hero/Nude/TwoHand.rs.hs")
            {
                m_modelePersonnage[0].Charger("Data/Entities/hero/Nude/TwoHand.rs.hs");
                m_cheminModele[0]="Data/Entities/hero/Nude/TwoHand.rs.hs";
            }
    }

    CalculerOrdreAffichage();
}

void Hero::ChargerModele()
{
    m_cas=0;

    int nombreArme=0;

    for(int i=0;i<m_inventaire.size();i++)
    {
        if(m_inventaire[i].m_equipe==ARME_PRINCIPAL&&m_inventaire[i].m_type==ARME||m_inventaire[i].m_equipe==BOUCLIER&&m_inventaire[i].m_type==ARME)
            nombreArme++;

        if(m_inventaire[i].m_equipe==ARME_PRINCIPAL&&m_inventaire[i].m_type==ARME)
            for(int j=0;j<m_inventaire[i].m_emplacementImpossible.size();j++)
                if(m_inventaire[i].m_emplacementImpossible[j]==BOUCLIER)
                    m_cas=2;
    }

    if(nombreArme==2)
        m_cas=1;



    bool pasEquipe[NOMBRE_MORCEAU_PERSONNAGE];

    for(int i=0;i<NOMBRE_MORCEAU_PERSONNAGE;i++)
        pasEquipe[i]=true,m_cheminModeleNouveau[i]="";


    for(int i=0;i<m_inventaire.size();i++)
        if(m_inventaire[i].m_equipe>0)
        {
                if(m_inventaire[i].m_emplacementImageHero.size()>0)
                {
                    int temp=m_cas;

                    if(m_inventaire[i].m_equipe==ARME_PRINCIPAL&&m_inventaire[i].m_type==ARME&&m_cas>0)
                        temp=0;

                    if(m_inventaire[i].m_equipe==BOUCLIER&&m_inventaire[i].m_type==ARME&&m_cas>0)
                        temp=1;

                    if(temp>=m_inventaire[i].m_emplacementImageHero.size())
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
        if(m_cas==0)
            if(m_cheminModele[0]!="Data/Entities/hero/Nude/OneHand.rs.hs")
            {
                m_modelePersonnage[0].Charger("Data/Entities/hero/Nude/OneHand.rs.hs");
                m_cheminModele[0]="Data/Entities/hero/Nude/OneHand.rs.hs";
            }
        if(m_cas==1)
            if(m_cheminModele[0]!="Data/Entities/hero/Nude/TwoWeapons.rs.hs")
            {
                m_modelePersonnage[0].Charger("Data/Entities/hero/Nude/TwoWeapons.rs.hs");
                m_cheminModele[0]="Data/Entities/hero/Nude/TwoWeapons.rs.hs";
            }
        if(m_cas==2)
            if(m_cheminModele[0]!="Data/Entities/hero/Nude/TwoHand.rs.hs")
            {
                m_modelePersonnage[0].Charger("Data/Entities/hero/Nude/TwoHand.rs.hs");
                m_cheminModele[0]="Data/Entities/hero/Nude/TwoHand.rs.hs";
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
        bool ajouter=true;

        ordre=m_personnage.getOrdre(&m_modelePersonnage[i]);
        if(ordre!=-10)
            ordreAffichage[NOMBRE_MORCEAU_PERSONNAGE/2+ordre]=i;
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
     string.SetSize(16*configuration.Resolution.h/600);

     string.SetColor(sf::Color(255,255,255));


    sprintf(chaine,"%i",m_caracteristiques.niveau);
    string.SetText(chaine);
    string.SetX((129*configuration.Resolution.w/800)+22*configuration.Resolution.w/800-(string.GetRect().Right-string.GetRect().Left)/2);
    string.SetY(263*configuration.Resolution.h/600-decalage*configuration.Resolution.h/600);
    string.SetColor(sf::Color(255,255,255));
    moteurGraphique.AjouterTexte(&string,15);


    sprintf(chaine,"%s",m_caracteristiques.nom.c_str());
    string.SetText(chaine);
    string.SetX((1*configuration.Resolution.w/800)+62*configuration.Resolution.w/800-(string.GetRect().Right-string.GetRect().Left)/2);
    string.SetY(263*configuration.Resolution.h/600-decalage*configuration.Resolution.h/600);
    string.SetColor(sf::Color(255,255,255));
    moteurGraphique.AjouterTexte(&string,15);




    sprintf(chaine,"%i / %i",(int)m_caracteristiques.vie,(int)m_caracteristiques.maxVie);
    string.SetText(chaine);
    string.SetX((178*configuration.Resolution.w/800)+54*configuration.Resolution.w/800-(string.GetRect().Right-string.GetRect().Left)/2);
    string.SetY(263*configuration.Resolution.h/600-decalage*configuration.Resolution.h/600);
    if(m_caracteristiques.maxVie!=m_personnage.getCaracteristique().maxVie)
        string.SetColor(sf::Color(0,128,255));
    else
        string.SetColor(sf::Color(255,255,255));
    moteurGraphique.AjouterTexte(&string,15);

    sprintf(chaine,"%i / %i",(int)m_caracteristiques.foi,(int)m_caracteristiques.maxFoi);
    string.SetText(chaine);
    string.SetX((290*configuration.Resolution.w/800)+54*configuration.Resolution.w/800-(string.GetRect().Right-string.GetRect().Left)/2);
    string.SetY(263*configuration.Resolution.h/600-decalage*configuration.Resolution.h/600);
    if(m_caracteristiques.maxFoi!=m_personnage.getCaracteristique().maxFoi)
        string.SetColor(sf::Color(0,128,255));
    else
        string.SetColor(sf::Color(255,255,255));
    moteurGraphique.AjouterTexte(&string,15);

    sprintf(chaine,"%i",m_caracteristiques.force);
    string.SetText(chaine);
    string.SetX((129*configuration.Resolution.w/800)+21*configuration.Resolution.w/800-(string.GetRect().Right-string.GetRect().Left)/2);
    string.SetY(311*configuration.Resolution.h/600-decalage*configuration.Resolution.h/600);
    if(m_caracteristiques.force!=m_personnage.getCaracteristique().force)
        string.SetColor(sf::Color(0,128,255));
    else
        string.SetColor(sf::Color(255,255,255));
    moteurGraphique.AjouterTexte(&string,15);

    sprintf(chaine,"%i",m_caracteristiques.dexterite);
    string.SetText(chaine);
    string.SetX((129*configuration.Resolution.w/800)+21*configuration.Resolution.w/800-(string.GetRect().Right-string.GetRect().Left)/2);
    string.SetY(338*configuration.Resolution.h/600-decalage*configuration.Resolution.h/600);
    if(m_caracteristiques.dexterite!=m_personnage.getCaracteristique().dexterite)
        string.SetColor(sf::Color(0,128,255));
    else
        string.SetColor(sf::Color(255,255,255));
    moteurGraphique.AjouterTexte(&string,15);

    sprintf(chaine,"%i",m_caracteristiques.vitalite);
    string.SetText(chaine);
    string.SetX((129*configuration.Resolution.w/800)+21*configuration.Resolution.w/800-(string.GetRect().Right-string.GetRect().Left)/2);
    string.SetY(365*configuration.Resolution.h/600-decalage*configuration.Resolution.h/600);
    if(m_caracteristiques.vitalite!=m_personnage.getCaracteristique().vitalite)
        string.SetColor(sf::Color(0,128,255));
    else
        string.SetColor(sf::Color(255,255,255));
    moteurGraphique.AjouterTexte(&string,15);

    sprintf(chaine,"%i",m_caracteristiques.piete);
    string.SetText(chaine);
    string.SetX((129*configuration.Resolution.w/800)+21*configuration.Resolution.w/800-(string.GetRect().Right-string.GetRect().Left)/2);
    string.SetY(392*configuration.Resolution.h/600-decalage*configuration.Resolution.h/600);
    if(m_caracteristiques.piete!=m_personnage.getCaracteristique().piete)
        string.SetColor(sf::Color(0,128,255));
    else
        string.SetColor(sf::Color(255,255,255));
    moteurGraphique.AjouterTexte(&string,15);

    sprintf(chaine,"%i",m_caracteristiques.charisme);
    string.SetText(chaine);
    string.SetX((129*configuration.Resolution.w/800)+21*configuration.Resolution.w/800-(string.GetRect().Right-string.GetRect().Left)/2);
    string.SetY(419*configuration.Resolution.h/600-decalage*configuration.Resolution.h/600);
    if(m_caracteristiques.charisme!=m_personnage.getCaracteristique().charisme)
        string.SetColor(sf::Color(0,128,255));
    else
        string.SetColor(sf::Color(255,255,255));
    moteurGraphique.AjouterTexte(&string,15);

    sprintf(chaine,"%i - %i",m_caracteristiques.degatsMin,m_caracteristiques.degatsMax);
    string.SetText(chaine);
    string.SetX((314*configuration.Resolution.w/800)+32*configuration.Resolution.w/800-(string.GetRect().Right-string.GetRect().Left)/2);
    string.SetY(300*configuration.Resolution.h/600-decalage*configuration.Resolution.h/600);
    if(m_caracteristiques.degatsMin!=m_personnage.getCaracteristique().degatsMin||m_caracteristiques.degatsMax!=m_personnage.getCaracteristique().degatsMax)
        string.SetColor(sf::Color(0,128,255));
    else
        string.SetColor(sf::Color(255,255,255));
    moteurGraphique.AjouterTexte(&string,15);

    sprintf(chaine,"%i",m_caracteristiques.armure);
    string.SetText(chaine);
    string.SetX((314*configuration.Resolution.w/800)+32*configuration.Resolution.w/800-(string.GetRect().Right-string.GetRect().Left)/2);
    string.SetY(327*configuration.Resolution.h/600-decalage*configuration.Resolution.h/600);
    if(m_caracteristiques.armure!=m_personnage.getCaracteristique().armure)
        string.SetColor(sf::Color(0,128,255));
    else
        string.SetColor(sf::Color(255,255,255));
    moteurGraphique.AjouterTexte(&string,15);
}

void Hero::afficherInventaire(sf::RenderWindow *ecran,coordonnee positionSouris,float decalage)
{

    //positionSouris.x=(int)ecran->ConvertCoords(ecran->GetInput().GetMouseX(),ecran->GetInput().GetMouseY()).x;
    //positionSouris.y=(int)ecran->ConvertCoords(ecran->GetInput().GetMouseX(), ecran->GetInput().GetMouseY()).y;


    for(int i=0;i<m_inventaire.size();i++)
    if(i!=m_objetEnMain)
        {
            coordonneeDecimal position;
            float rotation=0;
             sf::Sprite sprite;

            sprite.SetImage(*moteurGraphique.getImage(0));
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

            sprite.Resize(m_inventaire[i].getTaille().x*32*configuration.Resolution.w/800,m_inventaire[i].getTaille().y*32*configuration.Resolution.h/600);

            if(m_inventaire[i].m_equipe==AUCUN)
            {
                sprite.SetX((m_inventaire[i].getPosition().x*32+436)*configuration.Resolution.w/800);
                sprite.SetY(((m_inventaire[i].getPosition().y-1)*32+150+32)*configuration.Resolution.h/600-decalage*configuration.Resolution.h/600);

                position.x=(m_inventaire[i].getPosition().x*32+436)*configuration.Resolution.w/800;
                position.y=((m_inventaire[i].getPosition().y-1)*32+150+32)*configuration.Resolution.h/600-decalage*configuration.Resolution.h/600;

                position.h=m_inventaire[i].getTaille().y*32*configuration.Resolution.h/600;
                position.w=m_inventaire[i].getTaille().x*32*configuration.Resolution.w/800;
            }
            else
            {


                if(m_inventaire[i].m_equipe==ARME_PRINCIPAL)
                {
                    sprite.Resize(64*configuration.Resolution.w/800,160*configuration.Resolution.h/600);

                    position.x=(20 + 32 - m_inventaire[i].getPositionImage().w/2 )*configuration.Resolution.w/800;
                    position.y=(20 + 80 - m_inventaire[i].getPositionImage().h/2 )*configuration.Resolution.h/600-decalage*configuration.Resolution.h/600;

                    position.h=160*configuration.Resolution.w/800;
                    position.w=64*configuration.Resolution.h/600;

                    sprite.SetX((20)*configuration.Resolution.w/800);
                    sprite.SetY((20)*configuration.Resolution.h/600-decalage*configuration.Resolution.h/600);
                    if(m_objetEnMain==-1&&positionSouris.x>20*configuration.Resolution.x/800&&positionSouris.x<84*configuration.Resolution.x/800&&positionSouris.y>20*configuration.Resolution.y/600&&positionSouris.y<180*configuration.Resolution.y/600)
                        m_inventaire[i].AfficherCaracteristiques(ecran,positionSouris);
                }

                if(m_inventaire[i].m_equipe==BOUCLIER)
                {
                    rotation=-45;
                    sprite.Resize(96*configuration.Resolution.w/800,96*configuration.Resolution.h/600);

                    position.h=96*configuration.Resolution.h/600;
                    position.w=96*configuration.Resolution.w/800;

                    position.x=(94 + 48 - m_inventaire[i].getPositionImage().w/2 )*configuration.Resolution.w/800;
                    position.y=(40 + 48 - m_inventaire[i].getPositionImage().h/2 )*configuration.Resolution.h/600-decalage*configuration.Resolution.h/600;

                    sprite.SetX((94)*configuration.Resolution.w/800);
                    sprite.SetY((40)*configuration.Resolution.h/600-decalage*configuration.Resolution.h/600);
                    if(m_objetEnMain==-1&&positionSouris.x>94*configuration.Resolution.x/800&&positionSouris.x<190*configuration.Resolution.x/800&&positionSouris.y>40*configuration.Resolution.y/600&&positionSouris.y<136*configuration.Resolution.y/600)
                        m_inventaire[i].AfficherCaracteristiques(ecran,positionSouris);
                }

                if(m_inventaire[i].m_equipe==ARMURE_CORPS)
                {
                    sprite.Resize(64*configuration.Resolution.w/800,96*configuration.Resolution.h/600);

                    position.h=96*configuration.Resolution.h/600;
                    position.w=64*configuration.Resolution.w/800;

                    position.x=(222 + 32 - m_inventaire[i].getPositionImage().w/2 )*configuration.Resolution.w/800;
                    position.y=(83 + 48 - m_inventaire[i].getPositionImage().h/2 )*configuration.Resolution.h/600-decalage*configuration.Resolution.h/600;

                    sprite.SetX((222)*configuration.Resolution.w/800);
                    sprite.SetY((83)*configuration.Resolution.h/600-decalage*configuration.Resolution.h/600);
                    if(m_objetEnMain==-1&&positionSouris.x>222*configuration.Resolution.x/800&&positionSouris.x<284*configuration.Resolution.x/800&&positionSouris.y>83*configuration.Resolution.y/600&&positionSouris.y<178*configuration.Resolution.y/600)
                        m_inventaire[i].AfficherCaracteristiques(ecran,positionSouris);
                }

                if(m_inventaire[i].m_equipe==ARMURE_CORPS2)
                {
                    sprite.Resize(64*configuration.Resolution.w/800,96*configuration.Resolution.h/600);

                    position.h=96*configuration.Resolution.h/600;
                    position.w=64*configuration.Resolution.w/800;

                    position.x=(301 + 32 - m_inventaire[i].getPositionImage().w/2 )*configuration.Resolution.w/800;
                    position.y=(83 + 48 - m_inventaire[i].getPositionImage().h/2 )*configuration.Resolution.h/600-decalage*configuration.Resolution.h/600;

                    sprite.SetX((301)*configuration.Resolution.w/800);
                    sprite.SetY((83)*configuration.Resolution.h/600-decalage*configuration.Resolution.h/600);
                    if(m_objetEnMain==-1&&positionSouris.x>301*configuration.Resolution.x/800&&positionSouris.x<364*configuration.Resolution.x/800&&positionSouris.y>83*configuration.Resolution.y/600&&positionSouris.y<178*configuration.Resolution.y/600)
                        m_inventaire[i].AfficherCaracteristiques(ecran,positionSouris);
                }

                if(m_inventaire[i].m_equipe==CASQUE)
                {
                    sprite.Resize(64*configuration.Resolution.w/800,64*configuration.Resolution.h/600);

                    position.h=64*configuration.Resolution.h/600;
                    position.w=64*configuration.Resolution.w/800;

                    position.x=(262 + 32 - m_inventaire[i].getPositionImage().w/2 )*configuration.Resolution.w/800;
                    position.y=(14 + 32 - m_inventaire[i].getPositionImage().h/2 )*configuration.Resolution.h/600-decalage*configuration.Resolution.h/600;

                    sprite.SetX((262)*configuration.Resolution.w/800);
                    sprite.SetY((14)*configuration.Resolution.h/600-decalage*configuration.Resolution.h/600);
                    if(m_objetEnMain==-1&&positionSouris.x>262*configuration.Resolution.x/800&&positionSouris.x<324*configuration.Resolution.x/800&&positionSouris.y>14*configuration.Resolution.y/600&&positionSouris.y<77*configuration.Resolution.y/600)
                        m_inventaire[i].AfficherCaracteristiques(ecran,positionSouris);
                }

                if(m_inventaire[i].m_equipe==BOTTES)
                {
                    sprite.Resize(64*configuration.Resolution.w/800,64*configuration.Resolution.h/600);

                    position.h=64*configuration.Resolution.h/600;
                    position.w=64*configuration.Resolution.w/800;

                    position.x=(300 + 32 - m_inventaire[i].getPositionImage().w/2 )*configuration.Resolution.w/800;
                    position.y=(184 + 32 - m_inventaire[i].getPositionImage().h/2 )*configuration.Resolution.h/600-decalage*configuration.Resolution.h/600;

                    sprite.SetX((300)*configuration.Resolution.w/800);
                    sprite.SetY((184)*configuration.Resolution.h/600-decalage*configuration.Resolution.h/600);
                    if(m_objetEnMain==-1&&positionSouris.x>300*configuration.Resolution.x/800&&positionSouris.x<364*configuration.Resolution.x/800&&positionSouris.y>184*configuration.Resolution.y/600&&positionSouris.y<248*configuration.Resolution.y/600)
                        m_inventaire[i].AfficherCaracteristiques(ecran,positionSouris);
                }

                if(m_inventaire[i].m_equipe==ANNEAU1)
                {
                    sprite.Resize(32*configuration.Resolution.w/800,32*configuration.Resolution.h/600);

                    position.h=32*configuration.Resolution.h/600;
                    position.w=32*configuration.Resolution.w/800;

                    position.x=(153 + 16 - m_inventaire[i].getPositionImage().w/2 )*configuration.Resolution.w/800;
                    position.y=(148 + 16 - m_inventaire[i].getPositionImage().h/2 )*configuration.Resolution.h/600-decalage*configuration.Resolution.h/600;

                    sprite.SetX((153)*configuration.Resolution.w/800);
                    sprite.SetY((148)*configuration.Resolution.h/600-decalage*configuration.Resolution.h/600);
                    if(m_objetEnMain==-1&&positionSouris.x>153*configuration.Resolution.x/800&&positionSouris.x<185*configuration.Resolution.x/800&&positionSouris.y>148*configuration.Resolution.y/600&&positionSouris.y<180*configuration.Resolution.y/600)
                        m_inventaire[i].AfficherCaracteristiques(ecran,positionSouris);
                }

                if(m_inventaire[i].m_equipe==ANNEAU2)
                {
                    sprite.Resize(32*configuration.Resolution.w/800,32*configuration.Resolution.h/600);

                    position.h=32*configuration.Resolution.h/600;
                    position.w=32*configuration.Resolution.w/800;

                    position.x=(104 + 16 - m_inventaire[i].getPositionImage().w/2 )*configuration.Resolution.w/800;
                    position.y=(148 + 16 - m_inventaire[i].getPositionImage().h/2 )*configuration.Resolution.h/600-decalage*configuration.Resolution.h/600;

                    sprite.SetX((104)*configuration.Resolution.w/800);
                    sprite.SetY((148)*configuration.Resolution.h/600-decalage*configuration.Resolution.h/600);
                    if(m_objetEnMain==-1&&positionSouris.x>104*configuration.Resolution.x/800&&positionSouris.x<136*configuration.Resolution.x/800&&positionSouris.y>148*configuration.Resolution.y/600&&positionSouris.y<180*configuration.Resolution.y/600)
                        m_inventaire[i].AfficherCaracteristiques(ecran,positionSouris);
                }
            }

            moteurGraphique.AjouterCommande(&sprite,16,0);

            //sprite.SetCenter(m_inventaire[i].getTaille().x/2,m_inventaire[i].getTaille().y/2);
            sprite.Resize(position.w-4*configuration.Resolution.w/800,position.h-4*configuration.Resolution.h/600);
            sprite.Move(2*configuration.Resolution.w/800,2*configuration.Resolution.h/600);

            moteurGraphique.AjouterCommande(&sprite,16,0);

            sprite.SetCenter(m_inventaire[i].getPositionImage().w/2,m_inventaire[i].getPositionImage().h/2);

            sprite.SetX(position.x+m_inventaire[i].getPositionImage().w/2*configuration.Resolution.w/800);
            sprite.SetY(position.y+m_inventaire[i].getPositionImage().h/2*configuration.Resolution.h/600);

            sprite.SetColor(m_inventaire[i].m_color);

            sprite.SetImage(*moteurGraphique.getImage(m_inventaire[i].getImage()));
            sprite.SetSubRect(IntRect(m_inventaire[i].getPositionImage().x, m_inventaire[i].getPositionImage().y, m_inventaire[i].getPositionImage().x+m_inventaire[i].getPositionImage().w, m_inventaire[i].getPositionImage().y+m_inventaire[i].getPositionImage().h));
            sprite.Resize(m_inventaire[i].getTaille().x*32*configuration.Resolution.w/800,m_inventaire[i].getTaille().y*32*configuration.Resolution.h/600);



            sprite.SetRotation(rotation);
            moteurGraphique.AjouterCommande(&sprite,17,0);
            sprite.SetRotation(0);
        }

    if(m_objetEnMain>=0&&m_objetEnMain<m_inventaire.size())
    {
        sf::Sprite sprite;

        sprite.SetImage(*moteurGraphique.getImage(0));

        sprite.Resize(m_inventaire[m_objetEnMain].getTaille().x*32*configuration.Resolution.w/800,m_inventaire[m_objetEnMain].getTaille().y*32*configuration.Resolution.h/600);

        coordonnee caseVisee;
        caseVisee.x=((positionSouris.x+8*configuration.Resolution.x/800)-436*configuration.Resolution.x/800)/(32*configuration.Resolution.x/800) - m_inventaire[m_objetEnMain].getTaille().x/2;
        caseVisee.y=((positionSouris.y+8*configuration.Resolution.y/600)-150*configuration.Resolution.y/600)/(32*configuration.Resolution.y/600) - m_inventaire[m_objetEnMain].getTaille().y/2;

        sprite.SetX(positionSouris.x*configuration.Resolution.w/configuration.Resolution.x - m_inventaire[m_objetEnMain].getTaille().x*32*configuration.Resolution.x/1600);
        sprite.SetY(positionSouris.y*configuration.Resolution.h/configuration.Resolution.y - m_inventaire[m_objetEnMain].getTaille().y*32*configuration.Resolution.y/1200);

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

        if(positionSouris.x<436*configuration.Resolution.x/800||positionSouris.x>436*configuration.Resolution.x/800+32*10*configuration.Resolution.x/800||positionSouris.y<150*configuration.Resolution.y/600||positionSouris.y>150*configuration.Resolution.y/600+32*10*configuration.Resolution.y/600)
        {
            if(positionSouris.x>435*configuration.Resolution.x/800&&positionSouris.x<758*configuration.Resolution.x/800&&positionSouris.y>30*configuration.Resolution.y/600&&positionSouris.y<114*configuration.Resolution.x/600)
                sprite.SetColor(sf::Color(128,0,0,128));
        }
        else
        {
            sprite.SetX(caseVisee.x*32*configuration.Resolution.w/800 + 436*configuration.Resolution.w/800);
            sprite.SetY(caseVisee.y*32*configuration.Resolution.h/600 + 150*configuration.Resolution.h/600);
        }

        moteurGraphique.AjouterCommande(&sprite,18,0);

        sprite.SetColor(m_inventaire[m_objetEnMain].m_color);

        sprite.SetImage(*moteurGraphique.getImage(m_inventaire[m_objetEnMain].getImage()));
        sprite.SetSubRect(IntRect(m_inventaire[m_objetEnMain].getPositionImage().x, m_inventaire[m_objetEnMain].getPositionImage().y, m_inventaire[m_objetEnMain].getPositionImage().x+m_inventaire[m_objetEnMain].getPositionImage().w, m_inventaire[m_objetEnMain].getPositionImage().y+m_inventaire[m_objetEnMain].getPositionImage().h));
        sprite.Resize(m_inventaire[m_objetEnMain].getTaille().x*32*configuration.Resolution.w/800,m_inventaire[m_objetEnMain].getTaille().y*32*configuration.Resolution.h/600);

        sprite.SetX(positionSouris.x*configuration.Resolution.w/configuration.Resolution.x - m_inventaire[m_objetEnMain].getTaille().x*32*configuration.Resolution.x/1600);
        sprite.SetY(positionSouris.y*configuration.Resolution.h/configuration.Resolution.y - m_inventaire[m_objetEnMain].getTaille().y*32*configuration.Resolution.y/1200);

        /*if(positionSouris.x<436*configuration.Resolution.x/800||positionSouris.x>436*configuration.Resolution.y/800+32*10*configuration.Resolution.x/800||positionSouris.y<150*configuration.Resolution.y/600||positionSouris.y>150*configuration.Resolution.y/600+32*10*configuration.Resolution.y/600)
        {
            sprite.SetX(positionSouris.x- m_inventaire[m_objetEnMain].getTaille().x*32*configuration.Resolution.x/1600);
            sprite.SetY(positionSouris.y- m_inventaire[m_objetEnMain].getTaille().y*32*configuration.Resolution.y/1200);
        }*/

        moteurGraphique.AjouterCommande(&sprite,19,0);
    }
    else if(positionSouris.x>436*configuration.Resolution.x/800&&positionSouris.x<436*configuration.Resolution.x/800+32*10*configuration.Resolution.x/800&&positionSouris.y>150*configuration.Resolution.y/600&&positionSouris.y<150*configuration.Resolution.y/600+32*10*configuration.Resolution.y/600)
    {
        coordonnee caseVisee;
        caseVisee.x=(positionSouris.x-436*configuration.Resolution.x/800)/(32*configuration.Resolution.x/800);
        caseVisee.y=(positionSouris.y-150*configuration.Resolution.y/600)/(32*configuration.Resolution.y/600);

        for(int i=0;i<m_inventaire.size();i++)
            if(caseVisee.x>=m_inventaire[i].getPosition().x&&caseVisee.x<=m_inventaire[i].getPosition().x+m_inventaire[i].getTaille().x-1
             &&caseVisee.y>=m_inventaire[i].getPosition().y&&caseVisee.y<=m_inventaire[i].getPosition().y+m_inventaire[i].getTaille().y-1)
                 if(m_inventaire[i].m_equipe==AUCUN)
                 {
                    positionSouris.y-=32*configuration.Resolution.h/600;
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
	positionCamera.y=m_positionAffichage.y-250*configuration.Resolution.h/600;
	positionCamera.x=m_positionAffichage.x-((400*configuration.Resolution.w/800))+64;

	camera->SetFromRect(sf::FloatRect(positionCamera.x,positionCamera.y,positionCamera.x+configuration.Resolution.w,positionCamera.y+configuration.Resolution.h));
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
    temp.maxVie=temp.vitalite*25;
    temp.maxFoi=temp.piete*25;


    m_caracteristiques=m_personnage.getCaracteristique();
    m_caracteristiques.maxVie=0;
    m_caracteristiques.maxFoi=0;

    for(int i=0;i<m_inventaire.size();i++)
        if(m_inventaire[i].m_equipe>0)
            for(int j=0;j<m_inventaire[i].m_benedictions.size();++j)
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

    m_caracteristiques.maxVie+=m_caracteristiques.vitalite*25;
    m_caracteristiques.maxFoi+=m_caracteristiques.piete*25;


    for(int i=0;i<m_inventaire.size();i++)
    {
        if(m_inventaire[i].m_equipe>0)
        {
            int accru=100;
            for(int j=0;j<m_inventaire[i].m_benedictions.size();++j)
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

    /*temp.degatsMin=m_caracteristiques.degatsMin;
    temp.degatsMax=m_caracteristiques.degatsMax;
    temp.armure=m_caracteristiques.armure;*/

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
            temp.x=configuration.Resolution.x/2;
            temp.y=configuration.Resolution.y/2;

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

bool Hero::ajouterObjet(Objet objet,bool enMain)
{
    bool ramasser=false;

    if(!enMain)
    {
        for(int i=0;i<10;i++)
            for(int j=0;j<10;j++)
            {
                if(!m_caseInventaire[i][j])
                {
                    bool ok=true;
                    for(int x=j;x<j+objet.getTaille().x;x++)
                        for(int y=i;y<i+objet.getTaille().y;y++)
                            if(x<10&&y<10)
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

                        i=10,j=10;
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

    return ramasser;
}

void Hero::LibererCases(int numero)
{
    if(numero>=0&&numero<m_inventaire.size())
    {
        for(int x=0;x<m_inventaire[numero].getTaille().x;x++)
            for(int y=0;y<m_inventaire[numero].getTaille().y;y++)
                m_caseInventaire[y+m_inventaire[numero].getPosition().y][x+m_inventaire[numero].getPosition().x]=0;
    }
}

void Hero::AttribuerPositionObjet(coordonnee position,int numero)
{
    if(numero>=0&&numero<m_inventaire.size())
    {
        m_inventaire[numero].setPosition(position.x,position.y);

        m_inventaire[numero].m_equipe=0;

        for(int x=position.x;x<position.x+m_inventaire[numero].getTaille().x;x++)
                for(int y=position.y;y<position.y+m_inventaire[numero].getTaille().y;y++)
                    m_caseInventaire[y][x]=1;
    }
}

Objet Hero::DeposerObjet()
{
    if(m_objetADeposer>=0&&m_objetADeposer<m_inventaire.size())
    {
        Objet temp=m_inventaire[m_objetADeposer];

        m_inventaire.erase(m_inventaire.begin()+m_objetADeposer);

        temp.setPosition(m_personnage.getCoordonnee().x,m_personnage.getCoordonnee().y);

        temp.m_equipe=0;

        recalculerCaracteristiques();

        m_objetADeposer=-1;
        m_objetEnMain=-1;

        return temp;
    }
}

bool Hero::prendreEnMain(coordonnee positionSouris)
{
    m_objetADeposer=-1;
    if(positionSouris.x>436*configuration.Resolution.x/800&&positionSouris.x<436*configuration.Resolution.x/800+32*10*configuration.Resolution.x/800&&positionSouris.y>150*configuration.Resolution.y/600&&positionSouris.y<150*configuration.Resolution.y/600+32*10*configuration.Resolution.y/600)
    {
        coordonnee caseVisee;
        if(m_objetEnMain>=0&&m_objetEnMain<m_inventaire.size())
        {
            caseVisee.x=((positionSouris.x+8*configuration.Resolution.x/800)-436*configuration.Resolution.x/800)/(32*configuration.Resolution.x/800) - m_inventaire[m_objetEnMain].getTaille().x/2;
            caseVisee.y=((positionSouris.y+8*configuration.Resolution.y/600)-150*configuration.Resolution.y/600)/(32*configuration.Resolution.y/600) - m_inventaire[m_objetEnMain].getTaille().y/2;
        }
        else
        {
            caseVisee.x=((positionSouris.x)-436*configuration.Resolution.x/800)/(32*configuration.Resolution.x/800);
            caseVisee.y=((positionSouris.y)-150*configuration.Resolution.y/600)/(32*configuration.Resolution.y/600);
        }

        if(m_objetEnMain>=0&&m_objetEnMain<m_inventaire.size())
        {
            int temp=-1;
            bool ok=true;

            for(int i=caseVisee.y;i<caseVisee.y+m_inventaire[m_objetEnMain].getTaille().y;i++)
                for(int j=caseVisee.x;j<caseVisee.x+m_inventaire[m_objetEnMain].getTaille().x;j++)
                    if(i<10&&j<10&&i>=0&&j>=0)
                    {
                       // if(m_caseInventaire[i][j])
                            for(int z=0;z<m_inventaire.size();z++)
                                for(int y=m_inventaire[z].getPosition().y;y<m_inventaire[z].getPosition().y+m_inventaire[z].getTaille().y;y++)
                                    for(int x=m_inventaire[z].getPosition().x;x<m_inventaire[z].getPosition().x+m_inventaire[z].getTaille().x;x++)
                                        if(m_inventaire[z].m_equipe==AUCUN)
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
                if(temp!=-1)
                    LibererCases(temp);

                AttribuerPositionObjet(caseVisee,m_objetEnMain);

                m_objetEnMain=-1;

                if(temp!=-1)
                    m_objetEnMain=temp;
            }
        }
        else
        {
            for(int z=0;z<m_inventaire.size();z++)
                if(m_inventaire[z].m_equipe==AUCUN)
                    if(caseVisee.x>=m_inventaire[z].getPosition().x&&caseVisee.x<m_inventaire[z].getPosition().x+m_inventaire[z].getTaille().x
                     &&caseVisee.y>=m_inventaire[z].getPosition().y&&caseVisee.y<m_inventaire[z].getPosition().y+m_inventaire[z].getTaille().y)
                        m_objetEnMain=z,LibererCases(m_objetEnMain);
        }

    }
    else
    {
        if(positionSouris.x>435*configuration.Resolution.x/800&&positionSouris.x<758*configuration.Resolution.x/800&&positionSouris.y>30*configuration.Resolution.y/600&&positionSouris.y<114*configuration.Resolution.y/600)
        {
            if(m_objetEnMain>=0&&m_objetEnMain<m_inventaire.size())
            {
                m_objetADeposer=m_objetEnMain;
                m_objetEnMain=-1;

                return 1;

            }
        }
        else
        {
            if(positionSouris.x>20*configuration.Resolution.x/800&&positionSouris.x<84*configuration.Resolution.x/800&&positionSouris.y>20*configuration.Resolution.y/600&&positionSouris.y<180*configuration.Resolution.y/600)
                equiper(m_objetEnMain,ARME_PRINCIPAL);
            else if(positionSouris.x>222*configuration.Resolution.x/800&&positionSouris.x<284*configuration.Resolution.x/800&&positionSouris.y>83*configuration.Resolution.y/600&&positionSouris.y<178*configuration.Resolution.y/600)
                equiper(m_objetEnMain,ARMURE_CORPS);
            else if(positionSouris.x>301*configuration.Resolution.x/800&&positionSouris.x<364*configuration.Resolution.x/800&&positionSouris.y>83*configuration.Resolution.y/600&&positionSouris.y<178*configuration.Resolution.y/600)
                equiper(m_objetEnMain,ARMURE_CORPS2);
            else if(positionSouris.x>262*configuration.Resolution.x/800&&positionSouris.x<324*configuration.Resolution.x/800&&positionSouris.y>14*configuration.Resolution.y/600&&positionSouris.y<77*configuration.Resolution.y/600)
                equiper(m_objetEnMain,CASQUE);
            else if(positionSouris.x>300*configuration.Resolution.x/800&&positionSouris.x<364*configuration.Resolution.x/800&&positionSouris.y>184*configuration.Resolution.y/600&&positionSouris.y<248*configuration.Resolution.y/600)
                equiper(m_objetEnMain,BOTTES);
            else if(positionSouris.x>153*configuration.Resolution.x/800&&positionSouris.x<185*configuration.Resolution.x/800&&positionSouris.y>148*configuration.Resolution.y/600&&positionSouris.y<180*configuration.Resolution.y/600)
                equiper(m_objetEnMain,ANNEAU1);
            else if(positionSouris.x>104*configuration.Resolution.x/800&&positionSouris.x<136*configuration.Resolution.x/800&&positionSouris.y>148*configuration.Resolution.y/600&&positionSouris.y<180*configuration.Resolution.y/600)
                equiper(m_objetEnMain,ANNEAU2);
            else if(positionSouris.x>94*configuration.Resolution.x/800&&positionSouris.x<190*configuration.Resolution.x/800&&positionSouris.y>40*configuration.Resolution.y/600&&positionSouris.y<136*configuration.Resolution.y/600)
                equiper(m_objetEnMain,BOUCLIER);

            else if(m_objetEnMain>=0&&m_objetEnMain<m_inventaire.size())
                if(m_inventaire[m_objetEnMain].m_equipe==AUCUN)
                {
                    for(int i=0;i<10;i++)
                        for(int j=0;j<10;j++)
                            if(!m_caseInventaire[i][j])
                            {
                                bool ok=true;
                                for(int x=j;x<j+m_inventaire[m_objetEnMain].getTaille().x;x++)
                                    for(int y=i;y<i+m_inventaire[m_objetEnMain].getTaille().y;y++)
                                        if(x<10&&y<10)
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

                                    i=10,j=10;
                                }
                            }
                }
                else
                {
                    m_objetEnMain=-1;
                }
        }
    }
    return 0;
    //recalculerCaracteristiques();
}

bool Hero::equiper(int numero, int emplacement)
{
    int ancienEquipe=-1;
    bool ok=true;

    if(m_objetEnMain>=0 && m_objetEnMain<m_inventaire.size())
    {
        ok=false;
        for(int i=0;i<m_inventaire[m_objetEnMain].m_emplacement.size();i++)
        {
            if(m_inventaire[m_objetEnMain].m_emplacement[i]==emplacement)
                ok=true;
        }

        if(ok)
        {
            for(int i=0;i<m_inventaire.size();i++)
            {
                if(m_inventaire[i].m_equipe==emplacement)
                    if(numero!=i)
                        ancienEquipe=i;

                for(int j=0;j<m_inventaire[i].m_emplacementImpossible.size();j++)
                    if(numero!=i)
                        if(m_inventaire[i].m_equipe>0)
                            if(m_inventaire[i].m_emplacementImpossible[j]==emplacement)
                                ok=false;

                if(m_objetEnMain>=0&&m_objetEnMain<m_inventaire.size())
                    for(int j=0;j<m_inventaire[m_objetEnMain].m_emplacementImpossible.size();j++)
                        if(m_inventaire[i].m_equipe==m_inventaire[m_objetEnMain].m_emplacementImpossible[j])
                            ok=false;
            }

            if(ok)
                if(numero>=0&&numero<m_inventaire.size())
                {
                    ok=false;
                    for(int i=0;i<m_inventaire[numero].m_emplacement.size();++i)
                        if(m_inventaire[numero].m_emplacement[i]==emplacement)
                            ok=true;

                    if(ok)
                        m_inventaire[numero].m_equipe=emplacement;

                    m_inventaire[ancienEquipe].m_equipe=0;

                }

            if(ok)
            {
                m_objetEnMain=ancienEquipe;

            }
        }
    }
    else
    {
        for(int i=0;i<m_inventaire.size();i++)
            if(m_inventaire[i].m_equipe==emplacement)
                m_objetEnMain=i;
    }

    recalculerCaracteristiques();
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
