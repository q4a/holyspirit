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
	lumiere.intensite=160;
	lumiere.rouge=255;
	lumiere.vert=255;
	lumiere.bleu=255;
	lumiere.hauteur=20;

	m_modelePersonnage.setPorteeLumineuse(lumiere);

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

	for(int i=0;i<5;++i)
        for(int j=0;j<8;++j)
            m_caseInventaire[i][j]=0;

    m_objetEnMain=-1;

    recalculerCaracteristiques();
}

void Hero::Sauvegarder()
{
    ofstream fichier;
    fichier.open((configuration.chemin_saves+"hero.sav.hs").c_str(), ios::out);
    if(fichier)
    {
        fichier<<"* nom: "<<m_personnage.getCaracteristique().nom<<"\n";

        fichier<<"* vitesse: "<<crypter((int)m_personnage.getCaracteristique().vitesse*1000)<<"\n";
        fichier<<"* ptAme: "<<crypter(m_personnage.getCaracteristique().pointAme)<<"\n";
        fichier<<"* niveau: "<<crypter(m_personnage.getCaracteristique().niveau)<<"\n";

        fichier<<"* force: "<<crypter(m_personnage.getCaracteristique().force)<<"\n";
        fichier<<"* dexterite: "<<crypter(m_personnage.getCaracteristique().dexterite)<<"\n";
        fichier<<"* vitalite: "<<crypter(m_personnage.getCaracteristique().vitalite)<<"\n";
        fichier<<"* piete: "<<crypter(m_personnage.getCaracteristique().piete)<<"\n";
        fichier<<"* charisme: "<<crypter(m_personnage.getCaracteristique().charisme)<<"\n";

        fichier<<"$"<<endl;
        for(int i=0;i<5;++i)
        {
            fichier<<"* ";
            for(int j=0;j<8;++j)
                fichier<<m_caseInventaire[i][j]<<" ";
            fichier<<" $"<<endl;
        }
        fichier<<"$"<<endl;

        for(int i=0;i<m_inventaire.size();++i)
        {
            m_inventaire[i].Sauvegarder(&fichier);
        }

        fichier<<" $"<<endl;

    }
    fichier.close();
}
void Hero::Charger()
{
    struct dirent *lecture;

	DIR *repertoire;
    repertoire = opendir(configuration.chemin_saves.c_str());
    while ((lecture = readdir(repertoire)))
    {
        string temp="hero.sav.hs";
        if(lecture->d_name==temp)
        {
            ifstream fichier;
            fichier.open((configuration.chemin_saves+"hero.sav.hs").c_str(), ios::in);
            if(fichier)
            {
                char caractere;
                Caracteristique charTemp;
                charTemp=m_personnage.getCaracteristique();

                    do
                    {
                        fichier.get(caractere);
                        if(caractere=='*')
                        {
                            int temp2;
                            string temp;
                            fichier>>temp;
                            if(temp == "nom:")  fichier>>charTemp.nom;
                            if(temp == "vitesse:")  fichier>>temp2,charTemp.vitesse=decrypter(temp2)/1000;
                            if(temp == "ptAme:")  fichier>>temp2,charTemp.pointAme=decrypter(temp2),charTemp.ancienPointAme=charTemp.pointAme,charTemp.positionAncienAme=charTemp.pointAme;
                            if(temp == "niveau:")  fichier>>temp2,charTemp.niveau=decrypter(temp2);

                            if(temp == "force:")  fichier>>temp2,charTemp.force=decrypter(temp2);
                            if(temp == "dexterite:")  fichier>>temp2,charTemp.dexterite=decrypter(temp2);
                            if(temp == "vitalite:")  fichier>>temp2,charTemp.vitalite=decrypter(temp2);
                            if(temp == "piete:")  fichier>>temp2,charTemp.piete=decrypter(temp2);
                            if(temp == "charisme:")  fichier>>temp2,charTemp.charisme=decrypter(temp2);
                        }
                        if(fichier.eof()){throw "Impossible de charger la sauvegarde";}
                     }while(caractere!='$');

                    coordonnee position={0,0,0,0};

                    do
                    {

                        fichier.get(caractere);
                        if(caractere=='*')
                        {
                            for(int i=0;i<8;i++)
                            {
                                int temp;
                                fichier>>temp;
                                m_caseInventaire[position.y][position.x]=temp,position.x++;
                            }

                            do
                            {
                                fichier.get(caractere);
                                 if(fichier.eof()){throw "Impossible de charger la sauvegarde";}
                            }while(caractere!='$');

                            fichier.get(caractere);

                            position.y++;
                            position.x=0;

                            if(position.y>=5)
                                position.y=4;
                        }
                        if(fichier.eof()){throw "Impossible de charger la sauvegarde";}


                     }while(caractere!='$');

                    do
                    {
                        fichier.get(caractere);
                        if(caractere=='*')
                        {
                            int rarete=0,equipe=0;
                            int degMin=0,degMax=0,armure=0;
                            position.x=0;
                            position.y=0;
                            std::vector <benediction> bene;

                            do
                            {
                                fichier.get(caractere);
                                if(caractere=='e')
                                    fichier>>equipe;

                                else if(caractere=='r')
                                    fichier>>rarete;

                                else if(caractere=='x')
                                    fichier>>position.x;
                                else if(caractere=='y')
                                    fichier>>position.y;

                               else  if(caractere=='d')
                                {
                                    fichier.get(caractere);
                                    if(caractere=='i')
                                        fichier>>degMin;
                                    else if(caractere=='a')
                                        fichier>>degMax;
                                }

                                else if(caractere=='a')
                                    fichier>>armure;

                                else if(caractere=='m')
                                {
                                    int type=0;
                                    string chemin;
                                    fichier>>chemin;
                                    m_inventaire.push_back(Objet ());
                                    m_inventaire[m_inventaire.size()-1].Charger(chemin);
                                }
                                else if(caractere=='b')
                                {
                                    int type=0,info1=0,info2=0;

                                    fichier>>type;

                                    do
                                    {
                                        fichier.get(caractere);
                                        if(caractere=='i')
                                        {
                                            fichier.get(caractere);
                                            if(caractere=='1')
                                                fichier>>info1;
                                            if(caractere=='2')
                                                fichier>>info2;
                                        }
                                    }while(caractere!='$');

                                    bene.push_back(benediction ());
                                    bene[bene.size()-1].type=type;
                                    bene[bene.size()-1].info1=info1;
                                    bene[bene.size()-1].info2=info2;
                                    fichier.get(caractere);
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

                                m_inventaire[m_inventaire.size()-1].m_benedictions=bene;
                            }

                            fichier.get(caractere);
                        }
                        if(fichier.eof()){throw "Impossible de charger la sauvegarde";}

                    }while(caractere!='$');



                m_personnage.setCaracteristique(charTemp);
            }
            fichier.close();
        }
    }
    closedir(repertoire);
    recalculerCaracteristiques();

    m_caracteristiques.vie=m_caracteristiques.maxVie;
    m_caracteristiques.foi=m_caracteristiques.maxFoi;
}

void Hero::afficherCaracteristiques(sf::RenderWindow *ecran,coordonnee positionSouris,float decalage)
{
    sf::String string;
    char chaine[255];
     string.SetSize(16*configuration.Resolution.h/600);

    sprintf(chaine,"%i / %i",(int)m_caracteristiques.vie,(int)m_caracteristiques.maxVie);
    string.SetText(chaine);
    string.SetX((456*configuration.Resolution.w/800)+42*configuration.Resolution.w/800-(string.GetRect().Right-string.GetRect().Left)/2);
    string.SetY(45*configuration.Resolution.h/600-decalage*configuration.Resolution.h/600);
    if(m_caracteristiques.maxVie!=m_personnage.getCaracteristique().maxVie)
        string.SetColor(sf::Color(0,128,255));
    else
        string.SetColor(sf::Color(255,255,255));
    moteurGraphique.AjouterTexte(&string,15);

    sprintf(chaine,"%i / %i",(int)m_caracteristiques.foi,(int)m_caracteristiques.maxFoi);
    string.SetText(chaine);
    string.SetX((456*configuration.Resolution.w/800)+42*configuration.Resolution.w/800-(string.GetRect().Right-string.GetRect().Left)/2);
    string.SetY(78*configuration.Resolution.h/600-decalage*configuration.Resolution.h/600);
    if(m_caracteristiques.maxFoi!=m_personnage.getCaracteristique().maxFoi)
        string.SetColor(sf::Color(0,128,255));
    else
        string.SetColor(sf::Color(255,255,255));
    moteurGraphique.AjouterTexte(&string,15);

    sprintf(chaine,"%i",m_caracteristiques.force);
    string.SetText(chaine);
    string.SetX((475*configuration.Resolution.w/800)+17*configuration.Resolution.w/800-(string.GetRect().Right-string.GetRect().Left)/2);
    string.SetY(115*configuration.Resolution.h/600-decalage*configuration.Resolution.h/600);
    if(m_caracteristiques.force!=m_personnage.getCaracteristique().force)
        string.SetColor(sf::Color(0,128,255));
    else
        string.SetColor(sf::Color(255,255,255));
    moteurGraphique.AjouterTexte(&string,15);

    sprintf(chaine,"%i",m_caracteristiques.dexterite);
    string.SetText(chaine);
    string.SetX((475*configuration.Resolution.w/800)+17*configuration.Resolution.w/800-(string.GetRect().Right-string.GetRect().Left)/2);
    string.SetY(148*configuration.Resolution.h/600-decalage*configuration.Resolution.h/600);
    if(m_caracteristiques.dexterite!=m_personnage.getCaracteristique().dexterite)
        string.SetColor(sf::Color(0,128,255));
    else
        string.SetColor(sf::Color(255,255,255));
    moteurGraphique.AjouterTexte(&string,15);

    sprintf(chaine,"%i",m_caracteristiques.vitalite);
    string.SetText(chaine);
    string.SetX((475*configuration.Resolution.w/800)+17*configuration.Resolution.w/800-(string.GetRect().Right-string.GetRect().Left)/2);
    string.SetY(181*configuration.Resolution.h/600-decalage*configuration.Resolution.h/600);
    if(m_caracteristiques.vitalite!=m_personnage.getCaracteristique().vitalite)
        string.SetColor(sf::Color(0,128,255));
    else
        string.SetColor(sf::Color(255,255,255));
    moteurGraphique.AjouterTexte(&string,15);

    sprintf(chaine,"%i",m_caracteristiques.piete);
    string.SetText(chaine);
    string.SetX((475*configuration.Resolution.w/800)+17*configuration.Resolution.w/800-(string.GetRect().Right-string.GetRect().Left)/2);
    string.SetY(214*configuration.Resolution.h/600-decalage*configuration.Resolution.h/600);
    if(m_caracteristiques.piete!=m_personnage.getCaracteristique().piete)
        string.SetColor(sf::Color(0,128,255));
    else
        string.SetColor(sf::Color(255,255,255));
    moteurGraphique.AjouterTexte(&string,15);

    sprintf(chaine,"%i",m_caracteristiques.charisme);
    string.SetText(chaine);
    string.SetX((475*configuration.Resolution.w/800)+17*configuration.Resolution.w/800-(string.GetRect().Right-string.GetRect().Left)/2);
    string.SetY(247*configuration.Resolution.h/600-decalage*configuration.Resolution.h/600);
    if(m_caracteristiques.charisme!=m_personnage.getCaracteristique().charisme)
        string.SetColor(sf::Color(0,128,255));
    else
        string.SetColor(sf::Color(255,255,255));
    moteurGraphique.AjouterTexte(&string,15);

    sprintf(chaine,"%i - %i",m_caracteristiques.degatsMin,m_caracteristiques.degatsMax);
    string.SetText(chaine);
    string.SetX((456*configuration.Resolution.w/800)+42*configuration.Resolution.w/800-(string.GetRect().Right-string.GetRect().Left)/2);
    string.SetY(280*configuration.Resolution.h/600-decalage*configuration.Resolution.h/600);
    if(m_caracteristiques.degatsMin!=m_personnage.getCaracteristique().degatsMin||m_caracteristiques.degatsMax!=m_personnage.getCaracteristique().degatsMax)
        string.SetColor(sf::Color(0,128,255));
    else
        string.SetColor(sf::Color(255,255,255));
    moteurGraphique.AjouterTexte(&string,15);

    sprintf(chaine,"%i",m_caracteristiques.armure);
    string.SetText(chaine);
    string.SetX((456*configuration.Resolution.w/800)+42*configuration.Resolution.w/800-(string.GetRect().Right-string.GetRect().Left)/2);
    string.SetY(313*configuration.Resolution.h/600-decalage*configuration.Resolution.h/600);
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
                sprite.SetX((m_inventaire[i].getPosition().x*32+477)*configuration.Resolution.w/800);
                sprite.SetY(((m_inventaire[i].getPosition().y-1)*32+399)*configuration.Resolution.h/600-decalage*configuration.Resolution.h/600);
            }
            else
            {
                if(m_inventaire[i].m_equipe==ARME_PRINCIPAL)
                {
                    sprite.SetX(589*configuration.Resolution.w/800);
                    sprite.SetY(110*configuration.Resolution.h/600-decalage*configuration.Resolution.h/600);
                    if(positionSouris.x>589*configuration.Resolution.x/800&&positionSouris.x<(589+32)*configuration.Resolution.x/800&&positionSouris.y>110*configuration.Resolution.y/600&&positionSouris.y<(110+64)*configuration.Resolution.y/600)
                        m_inventaire[i].AfficherCaracteristiques(ecran,positionSouris);
                }

                if(m_inventaire[i].m_equipe==ARMURE_CORPS)
                {
                    sprite.SetX(638*configuration.Resolution.w/800);
                    sprite.SetY(110*configuration.Resolution.h/600-decalage*configuration.Resolution.h/600);
                    if(positionSouris.x>636*configuration.Resolution.x/800&&positionSouris.x<(636+64)*configuration.Resolution.x/800&&positionSouris.y>110*configuration.Resolution.y/600&&positionSouris.y<(110+96)*configuration.Resolution.y/600)
                        m_inventaire[i].AfficherCaracteristiques(ecran,positionSouris);
                }
            }

            moteurGraphique.AjouterCommande(&sprite,16,0);

            //sprite.SetCenter(m_inventaire[i].getTaille().x/2,m_inventaire[i].getTaille().y/2);
            sprite.Resize(m_inventaire[i].getTaille().x*32*configuration.Resolution.w/800-4*configuration.Resolution.w/800,m_inventaire[i].getTaille().y*32*configuration.Resolution.h/600-4*configuration.Resolution.h/600);
            sprite.Move(2*configuration.Resolution.w/800,2*configuration.Resolution.h/600);

            moteurGraphique.AjouterCommande(&sprite,16,0);

            sprite.Move(-2*configuration.Resolution.w/800,-2*configuration.Resolution.h/600);

            sprite.SetColor(sf::Color(255,255,255,255));

            sprite.SetImage(*moteurGraphique.getImage(m_inventaire[i].getImage()));
            sprite.SetSubRect(IntRect(m_inventaire[i].getPositionImage().x, m_inventaire[i].getPositionImage().y, m_inventaire[i].getPositionImage().x+m_inventaire[i].getPositionImage().w, m_inventaire[i].getPositionImage().y+m_inventaire[i].getPositionImage().h));
            sprite.Resize(m_inventaire[i].getTaille().x*32*configuration.Resolution.w/800,m_inventaire[i].getTaille().y*32*configuration.Resolution.h/600);
            moteurGraphique.AjouterCommande(&sprite,17,0);

        }

    if(m_objetEnMain>=0&&m_objetEnMain<m_inventaire.size())
    {
        sf::Sprite sprite;

        sprite.SetImage(*moteurGraphique.getImage(0));

        sprite.Resize(m_inventaire[m_objetEnMain].getTaille().x*32*configuration.Resolution.w/800,m_inventaire[m_objetEnMain].getTaille().y*32*configuration.Resolution.h/600);

        sprite.SetX(positionSouris.x);
        sprite.SetY(positionSouris.y);

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

        if(positionSouris.x<477*configuration.Resolution.w/800||positionSouris.x>477*configuration.Resolution.w/800+32*8*configuration.Resolution.w/800||positionSouris.y<367*configuration.Resolution.h/600||positionSouris.y>399*configuration.Resolution.h/600+32*5*configuration.Resolution.h/600)
        {
            if(positionSouris.x<400*configuration.Resolution.w/800)
                sprite.SetColor(sf::Color(128,0,0,128));
        }
        else
        {
            sprite.SetX(((positionSouris.x)-477*configuration.Resolution.w/800)/(32*configuration.Resolution.w/800)*(32*configuration.Resolution.w/800)+477*configuration.Resolution.w/800);
            sprite.SetY(((positionSouris.y)-367*configuration.Resolution.h/600)/(32*configuration.Resolution.h/600)*(32*configuration.Resolution.h/600)+367*configuration.Resolution.h/600);
        }

        moteurGraphique.AjouterCommande(&sprite,18,0);

        sprite.SetColor(sf::Color(255,255,255,255));

        sprite.SetImage(*moteurGraphique.getImage(m_inventaire[m_objetEnMain].getImage()));
        sprite.SetSubRect(IntRect(m_inventaire[m_objetEnMain].getPositionImage().x, m_inventaire[m_objetEnMain].getPositionImage().y, m_inventaire[m_objetEnMain].getPositionImage().x+m_inventaire[m_objetEnMain].getPositionImage().w, m_inventaire[m_objetEnMain].getPositionImage().y+m_inventaire[m_objetEnMain].getPositionImage().h));
        sprite.Resize(m_inventaire[m_objetEnMain].getTaille().x*32*configuration.Resolution.w/800,m_inventaire[m_objetEnMain].getTaille().y*32*configuration.Resolution.h/600);
        sprite.SetX(((positionSouris.x)-477*configuration.Resolution.w/800)/(32*configuration.Resolution.w/800)*(32*configuration.Resolution.w/800)+477*configuration.Resolution.w/800);
        sprite.SetY(((positionSouris.y)-367*configuration.Resolution.h/600)/(32*configuration.Resolution.h/600)*(32*configuration.Resolution.h/600)+367*configuration.Resolution.h/600);

        if(positionSouris.x<477*configuration.Resolution.w/800||positionSouris.x>477*configuration.Resolution.w/800+32*8*configuration.Resolution.w/800||positionSouris.y<367*configuration.Resolution.h/600||positionSouris.y>399*configuration.Resolution.h/600+32*5*configuration.Resolution.h/600)
        {
            sprite.SetX(positionSouris.x);
            sprite.SetY(positionSouris.y);
        }

        moteurGraphique.AjouterCommande(&sprite,19,0);
    }
    else if(positionSouris.x>477*configuration.Resolution.w/800&&positionSouris.x<477*configuration.Resolution.w/800+32*8*configuration.Resolution.w/800&&positionSouris.y>367*configuration.Resolution.h/600&&positionSouris.y<399+32*5*configuration.Resolution.h/600)
    {
        coordonnee caseVisee;
        caseVisee.x=(positionSouris.x-477*configuration.Resolution.x/800)/(32*configuration.Resolution.w/800);
        caseVisee.y=(positionSouris.y-367*configuration.Resolution.y/600)/(32*configuration.Resolution.h/600);

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

void Hero::testMontreVise(Monstre *monstre,int hauteurMap)
{
    if(m_monstreVise>-1&&m_caracteristiques.vie>0)
    {
        if(fabs(m_personnage.getCoordonnee().x-monstre->getProchaineCase().x)>1||fabs(m_personnage.getCoordonnee().y-monstre->getProchaineCase().y)>1)
                m_personnage.setArrivee(monstre->getProchaineCase());
        else if(m_personnage.getArrivee().x==m_personnage.getCoordonnee().x&&m_personnage.getArrivee().x==m_personnage.getCoordonnee().x)
        {
            m_personnage.setArrivee(m_personnage.getCoordonnee());
            coordonnee temp,temp2;
            temp.x=(m_personnage.getCoordonneePixel().x-m_personnage.getCoordonneePixel().y-1+hauteurMap)*64;
            temp.y=(m_personnage.getCoordonneePixel().x+m_personnage.getCoordonneePixel().y)*32;
            temp2.x=(monstre->getCoordonneePixel().x-monstre->getCoordonneePixel().y-1+hauteurMap)*64;
            temp2.y=(monstre->getCoordonneePixel().x+monstre->getCoordonneePixel().y)*32;
            m_personnage.frappe(temp2,temp);
        }
    }
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

    //recalculerCaracteristiques();
}

void Hero::recalculerCaracteristiques()
{
    int vie=(int)m_caracteristiques.vie,foi=(int)m_caracteristiques.foi;

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

    m_caracteristiques.degatsMin=m_caracteristiques.force/2;
    m_caracteristiques.degatsMax=(int)(m_caracteristiques.force);
    m_caracteristiques.armure=m_caracteristiques.dexterite;

    m_caracteristiques.maxVie+=m_caracteristiques.vitalite*25;
    m_caracteristiques.maxFoi+=m_caracteristiques.piete*25;

    for(int i=0;i<m_inventaire.size();i++)
    {
        if(m_inventaire[i].m_equipe==ARME_PRINCIPAL)
        {
            m_caracteristiques.degatsMin=m_caracteristiques.force+m_inventaire[i].m_degatsMin;
            m_caracteristiques.degatsMax=m_caracteristiques.force+m_inventaire[i].m_degatsMax;


            temp.degatsMin=m_caracteristiques.degatsMin;
            temp.degatsMax=m_caracteristiques.degatsMax;
            /*for(int j=0;j<m_inventaire[i].m_benedictions.size();j++)
                if(m_inventaire[i].m_benedictions[j].type==EFFICACITE_ACCRUE)
                    temp.degatsMin=0,temp.degatsMax=0;*/
        }

        if(m_inventaire[i].m_equipe==ARMURE_CORPS)
        {
            m_caracteristiques.armure+=m_inventaire[i].m_armure;

            temp.armure=m_caracteristiques.armure;
            /*for(int j=0;j<m_inventaire[i].m_benedictions.size();j++)
                if(m_inventaire[i].m_benedictions[j].type==EFFICACITE_ACCRUE)
                    temp.armure=0;*/

        }
    }

    temp.degatsMin=m_caracteristiques.degatsMin;
    temp.degatsMax=m_caracteristiques.degatsMax;
    temp.armure=m_caracteristiques.armure;

    m_personnage.setCaracteristique(temp);

    m_caracteristiques.vie=vie,m_caracteristiques.foi=foi;
}

bool Hero::ajouterObjet(Objet objet)
{
    bool ramasser=false;
    for(int i=0;i<5;i++)
        for(int j=0;j<8;j++)
        {
            if(!m_caseInventaire[i][j])
            {
                bool ok=true;
                for(int x=j;x<j+objet.getTaille().x;x++)
                    for(int y=i;y<i+objet.getTaille().y;y++)
                        if(x<8&&y<5)
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

                    i=5,j=8;
                }
            }
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

Objet Hero::prendreEnMain(coordonnee positionSouris)
{
    if(positionSouris.x>477*configuration.Resolution.w/800&&positionSouris.x<477*configuration.Resolution.w/800+32*8*configuration.Resolution.w/800&&positionSouris.y>367*configuration.Resolution.h/600&&positionSouris.y<399*configuration.Resolution.h/600+32*5*configuration.Resolution.h/600)
    {
        coordonnee caseVisee;
        caseVisee.x=((positionSouris.x)-477*configuration.Resolution.w/800)/(32*configuration.Resolution.w/800);
        caseVisee.y=((positionSouris.y)-367*configuration.Resolution.h/600)/(32*configuration.Resolution.h/600);

        if(m_objetEnMain>=0&&m_objetEnMain<m_inventaire.size())
        {
            int temp=-1;
            bool ok=true;

            for(int i=caseVisee.y;i<caseVisee.y+m_inventaire[m_objetEnMain].getTaille().y;i++)
                for(int j=caseVisee.x;j<caseVisee.x+m_inventaire[m_objetEnMain].getTaille().x;j++)
                    if(i<5&&j<8)
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
        if(positionSouris.x*configuration.Resolution.w/800<400)
        {
            if(m_objetEnMain>=0&&m_objetEnMain<m_inventaire.size())
            {
                Objet temp=m_inventaire[m_objetEnMain];

                m_inventaire.erase(m_inventaire.begin()+m_objetEnMain);

                m_objetEnMain=-1;

                temp.setPosition(m_personnage.getCoordonnee().x,m_personnage.getCoordonnee().y);

                temp.m_equipe=0;

                recalculerCaracteristiques();

                return temp;
            }
        }
        else
        {
            if(positionSouris.x>589*configuration.Resolution.w/800&&positionSouris.x<(589+32)*configuration.Resolution.w/800&&positionSouris.y>110*configuration.Resolution.h/600&&positionSouris.y<(110+64)*configuration.Resolution.h/600)
                equiper(m_objetEnMain,ARME_PRINCIPAL);
            else if(positionSouris.x>636*configuration.Resolution.w/800&&positionSouris.x<(636+64)*configuration.Resolution.w/800&&positionSouris.y>110*configuration.Resolution.h/600&&positionSouris.y<(110+64)*configuration.Resolution.h/600)
                equiper(m_objetEnMain,ARMURE_CORPS);

            else if(m_objetEnMain>=0&&m_objetEnMain<m_inventaire.size())
                if(m_inventaire[m_objetEnMain].m_equipe==AUCUN)
                {
                    AttribuerPositionObjet(m_inventaire[m_objetEnMain].getPosition(),m_objetEnMain);
                    m_objetEnMain=-1;
                }
                else
                {
                    m_objetEnMain=-1;
                }
        }
    }
    recalculerCaracteristiques();

    Objet temp2;
    return temp2;
}

bool Hero::equiper(int numero, int emplacement)
{
    int ancienEquipe=-1;
    bool ok=true;

    for(int i=0;i<m_inventaire.size();i++)
        if(m_inventaire[i].m_equipe==emplacement)
            if(numero!=i)
                ancienEquipe=i;

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

        //if(ancienEquipe>=0)
            //m_inventaire[ancienEquipe].m_equipe=0;
    }

    recalculerCaracteristiques();
}

void Hero::infligerDegats(float degats)
{
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

void Hero::setMonstreVise(int monstre){m_monstreVise=monstre;}
void Hero::setChercherSac(coordonnee a){m_chercherSac=a;}

coordonnee Hero::getChercherSac(){return m_chercherSac;};
int Hero::getMonstreVise(){return m_monstreVise;}
#endif
