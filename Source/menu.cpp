#include "menu.h"
#include "Globale.h"

#include <iostream>
#include <fstream>

#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>

using namespace std;
using namespace sf;

Menu::~Menu()
{
    m_sang.clear();
    m_ame.clear();
}

Menu::Menu()
{
    m_alphaSang=0;
    console.Ajouter("",0);
    console.Ajouter("Chargement des menus :",0);
    m_imageMiniMap=moteurGraphique.AjouterImage(configuration.chemin_menus+configuration.nom_minimap);
    m_imageHUD=moteurGraphique.AjouterImage(configuration.chemin_menus+configuration.nom_hud);
    m_imageBulleVie=moteurGraphique.AjouterImage(configuration.chemin_menus+configuration.nom_bulle_vie);
    m_imageBulleFoi=moteurGraphique.AjouterImage(configuration.chemin_menus+configuration.nom_bulle_foi);

    m_imageAme=moteurGraphique.AjouterImage(configuration.chemin_menus+configuration.nom_ame);
    m_imageSang=moteurGraphique.AjouterImage(configuration.chemin_menus+configuration.nom_sang);
    m_barrePointAme=moteurGraphique.AjouterImage(configuration.chemin_menus+configuration.nom_barre_ame);
    m_inventaire=moteurGraphique.AjouterImage(configuration.chemin_menus+configuration.nom_inventaire);

    m_barreVie=moteurGraphique.AjouterImage(configuration.chemin_menus+configuration.nom_barre_vie);
    m_barreVieVide=moteurGraphique.AjouterImage(configuration.chemin_menus+configuration.nom_barre_vie_vide);
}

void Menu::Afficher(sf::RenderWindow* ecran,int type,float alpha)
{
	Sprite sprite,sprite2;

	if(type==1)
    {
	    //On affiche l'HUD
		sprite2.SetImage(*moteurGraphique.getImage(m_imageHUD));
		sprite2.SetX(0);
		sprite2.SetY(configuration.Resolution.h-128*configuration.Resolution.h/600);
		sprite2.Resize(configuration.Resolution.w, 128*configuration.Resolution.h/600);
		sprite.SetColor(sf::Color(255,255,255,(int)alpha));
		moteurGraphique.AjouterCommande(&sprite2,17,0);
    }

	if(type==2)
    {
        //On affiche le fond noir de la mini-map
		sprite.SetImage(*moteurGraphique.getImage(0));
		sprite.SetX(configuration.Resolution.w-configuration.Resolution.w*0.25);
		sprite.SetY(0);
		sprite.SetColor(sf::Color(0,0,0,224*(int)alpha/255));
		sprite.Resize(configuration.Resolution.w*0.25, configuration.Resolution.w*0.25);
		moteurGraphique.AjouterCommande(&sprite,13,0);

		//On affiche le cadran de la mini map
		sprite2.SetImage(*moteurGraphique.getImage(m_imageMiniMap));
		sprite2.SetX(configuration.Resolution.w-configuration.Resolution.w*0.25);
		sprite2.SetY(0);
		sprite2.Resize(configuration.Resolution.w*0.25, configuration.Resolution.w*0.25);
		sprite2.SetColor(sf::Color(255,255,255,(int)alpha));
		moteurGraphique.AjouterCommande(&sprite2,15,0);
    }


    if(type==3)
    {
        //On affiche le fond noir du menu du sac
		sprite.SetImage(*moteurGraphique.getImage(0));
		sprite.SetX(configuration.Resolution.w-configuration.Resolution.w*0.25*alpha/255);
		sprite.SetY(configuration.Resolution.w*0.25);
		sprite.SetColor(sf::Color(0,0,0,224*(int)alpha/255));
		sprite.Resize(configuration.Resolution.w*0.25, configuration.Resolution.w*0.34);
		moteurGraphique.AjouterCommande(&sprite,13,0);

        //On affiche le cadran du menu du sac
		sprite2.SetImage(*moteurGraphique.getImage(m_imageMiniMap));
		sprite2.SetX(configuration.Resolution.w-configuration.Resolution.w*0.25*alpha/255);
		sprite2.SetY(configuration.Resolution.w*0.25);
		sprite2.Resize(configuration.Resolution.w*0.25, configuration.Resolution.w*0.34);
		sprite2.SetColor(sf::Color(255,255,255));
		moteurGraphique.AjouterCommande(&sprite2,15,0);
    }
}


void Menu::AfficherDynamique(sf::RenderWindow* ecran,Caracteristique caracteristique,int type,Caracteristique caracteristiqueMonstre)
{

    if(caracteristique.vie>0)
    {
        Sprite sprite;
        sprite.SetImage(*moteurGraphique.getImage(m_imageBulleVie));
        sprite.Resize(96*configuration.Resolution.w/800, 66*configuration.Resolution.h/600);
        sprite.SetSubRect(sf::IntRect(0, (int)(66-caracteristique.vie*66/caracteristique.maxVie), 88, 66));
        sprite.SetX(116*configuration.Resolution.w/800);
        sprite.SetY((int)(configuration.Resolution.h-(128-12)*configuration.Resolution.h/600+(caracteristique.maxVie-caracteristique.vie)/caracteristique.maxVie*66*configuration.Resolution.h/600));

        moteurGraphique.AjouterCommande(&sprite,17,0);
    }

    if(caracteristique.foi>0)
    {
        Sprite sprite;
        sprite.SetImage(*moteurGraphique.getImage(m_imageBulleFoi));
        sprite.Resize(96*configuration.Resolution.w/800, 66*configuration.Resolution.h/600);
        sprite.SetSubRect(sf::IntRect(0, (int)(66-caracteristique.foi*66/caracteristique.maxFoi), 88, 66));
        sprite.SetX(588*configuration.Resolution.w/800);
        sprite.SetY((int)(configuration.Resolution.h-(128-12)*configuration.Resolution.h/600+(caracteristique.maxFoi-caracteristique.foi)/caracteristique.maxFoi*66*configuration.Resolution.h/600));

        moteurGraphique.AjouterCommande(&sprite,17,0);
    }

    if(caracteristique.ancienPointAme>0)
    {
        Sprite sprite;

        sprite.SetImage(*moteurGraphique.getImage(m_barrePointAme));
        sprite.Resize(90*configuration.Resolution.w/800, 88*configuration.Resolution.w/800);


        //(int)((float)((float)((caracteristique.niveau-1)*(caracteristique.niveau-1)*(caracteristique.niveau-1)*10)-caracteristique.ancienPointAme)/(float)(((caracteristique.niveau)*(caracteristique.niveau)*(caracteristique.niveau)*10)-(float)((caracteristique.niveau-1)*(caracteristique.niveau-1)*(caracteristique.niveau-1)*10))*90)

        int temp= (int) ( (caracteristique.ancienPointAme-((caracteristique.niveau-1)*(caracteristique.niveau-1)*(caracteristique.niveau-1)*10)) * 88 / (((caracteristique.niveau)*(caracteristique.niveau)*(caracteristique.niveau)*10) - ((caracteristique.niveau-1)*(caracteristique.niveau-1)*(caracteristique.niveau-1)*10)));

        sprite.SetSubRect(sf::IntRect(0,88-temp, 90, 88));

        sprite.SetX(configuration.Resolution.w/2-46*configuration.Resolution.w/800);
        sprite.SetY(configuration.Resolution.h-(temp)*configuration.Resolution.h/600);

        moteurGraphique.AjouterCommande(&sprite,17,0);
    }

    texte.SetSize(16.f*configuration.Resolution.y/600);

    char chaine[255],chaine2[255];

    texte.SetColor(Color(255,255,255,255));

    sprintf(chaine,"%ld",caracteristique.niveau);
    texte.SetText(chaine);

    texte.SetX(configuration.Resolution.w/2-(texte.GetRect().Right-texte.GetRect().Left)/2);
    texte.SetY(configuration.Resolution.h-52*configuration.Resolution.h/600);

    moteurGraphique.AjouterTexte(&texte,17);
    //ecran->Draw(texte);

    if(type!=-1)
    {
        for(int i=0;i<m_ame.size();i++)
        {
            if(m_ame[i].m_mode<3)
            {
                Sprite sprite;

                sprite.SetImage(*moteurGraphique.getImage(m_imageAme));
                sprite.SetCenter(16,16);
                sprite.Resize(32*configuration.Resolution.w/800*m_ame[i].m_taille, 32*configuration.Resolution.w/800*m_ame[i].m_taille);
                sprite.SetColor(sf::Color(255,255,255,(int)m_ame[i].m_alpha));
                sprite.SetX((m_ame[i].m_position.x+16)*configuration.Resolution.w/800);
                sprite.SetY((m_ame[i].m_position.y+16)*configuration.Resolution.h/600);


                sprite.SetRotation(m_ame[i].m_rotation);
                moteurGraphique.AjouterCommande(&sprite,17,0);
            }
        }

        for(int i=0;i<m_sang.size();i++)
        {
            Sprite sprite;

            sprite.SetImage(*moteurGraphique.getImage(m_imageSang));
            sprite.SetCenter(200*m_sang[i].m_taille,200*m_sang[i].m_taille);
            sprite.SetRotation(m_sang[i].m_rotation);
            sprite.SetCenter(0,0);
            sprite.SetSubRect(sf::IntRect(200*m_sang[i].m_numero, 0,300+300*m_sang[i].m_numero, 200));
            sprite.Resize(300*configuration.Resolution.x/800*m_sang[i].m_taille, 200*configuration.Resolution.y/600*m_sang[i].m_taille);
            sprite.SetColor(sf::Color(255,255,255,(int)m_sang[i].m_alpha));
            sprite.SetX(m_sang[i].m_position.x*configuration.Resolution.x/800);
            sprite.SetY(m_sang[i].m_position.y*configuration.Resolution.y/600);

            moteurGraphique.AjouterCommande(&sprite,17,0);
        }
    }

    if(type==1)
    {
        Sprite sprite,sprite2;

        sprite.SetImage(*moteurGraphique.getImage(m_barreVieVide));
         sprite.Resize(configuration.Resolution.w/2, 32*configuration.Resolution.w/800);
         sprite.SetX(configuration.Resolution.w/2-sprite.GetSize().x/2);
        sprite.SetY(8);

        moteurGraphique.AjouterCommande(&sprite,16,0);

        sprite2.SetImage(*moteurGraphique.getImage(m_barreVie));
        if(caracteristiqueMonstre.rang==1){sprite2.SetColor(Color(128,64,255,255));}
        if(caracteristiqueMonstre.rang==2){sprite2.SetColor(Color(32,32,255,255));}
        sprite2.Resize(configuration.Resolution.w/2, 32*configuration.Resolution.w/800);
        sprite2.SetSubRect(sf::IntRect(0, 0, (int)(caracteristiqueMonstre.vie/caracteristiqueMonstre.maxVie*400), 32));
        sprite2.SetX(configuration.Resolution.w/2-sprite.GetSize().x/2);
        sprite2.SetY(8);


        moteurGraphique.AjouterCommande(&sprite2,17,0);

        //char chaine[255];

        texte.SetSize(20.f*configuration.Resolution.h/600);
         texte.SetStyle(1);

        sprintf(chaine,"%s (%ld)",caracteristiqueMonstre.nom.c_str(),caracteristiqueMonstre.niveau);
        sprintf(chaine2,"%s",chaine);
        if(caracteristiqueMonstre.rang==1){sprintf(chaine2,"Champion : %s",chaine);}
        if(caracteristiqueMonstre.rang==2){sprintf(chaine2,"Chef : %s",chaine);}
        texte.SetText(chaine2);


        texte.SetX(configuration.Resolution.w/2-(texte.GetRect().Right-texte.GetRect().Left)/2+2);
        texte.SetY(12*configuration.Resolution.h/600+2);
        texte.SetColor(Color(0,0,0,255));

        moteurGraphique.AjouterTexte(&texte,18);
        //ecran->Draw(texte);
        texte.SetColor(Color(224,224,224,255));
        if(caracteristiqueMonstre.rang==1){texte.SetColor(Color(100,50,200,255));}
        if(caracteristiqueMonstre.rang==2){texte.SetColor(Color(32,32,128,255));}

        texte.SetX(configuration.Resolution.w/2-(texte.GetRect().Right-texte.GetRect().Left)/2);
        texte.SetY(12*configuration.Resolution.h/600);
        moteurGraphique.AjouterTexte(&texte,18);
        //ecran->Draw(texte);

         texte.SetStyle(0);
    }
}

void Menu::AfficherChargement(sf::RenderWindow* ecran,string nom,int fond,int z=50)
{
    Sprite sprite;

    sprite.SetImage(*moteurGraphique.getImage(fond));
    sprite.SetColor(Color(255,255,255,z*255/50));
    sprite.SetX(0);
    sprite.SetY(0);
    sprite.Resize(configuration.Resolution.w,configuration.Resolution.h*5/6);
    moteurGraphique.AjouterCommande(&sprite,0);

    texte.SetSize(50.f*configuration.Resolution.h/600);
    char chaine[255];
    sprintf(chaine,"%s",nom.c_str());
    texte.SetText(chaine);

    texte.SetX(configuration.Resolution.w/2-(texte.GetRect().Right-texte.GetRect().Left)/2);
    texte.SetY(configuration.Resolution.h-(texte.GetRect().Bottom-texte.GetRect().Top)/2-60*configuration.Resolution.h/600);
    texte.SetColor(Color(150,100,50,z*255/50));
    moteurGraphique.AjouterTexte(&texte,0,1);
}

void Menu::AfficherInventaire(sf::RenderWindow* ecran,float decalage)
{
    Sprite sprite;

    sprite.SetImage(*moteurGraphique.getImage(m_inventaire));
    sprite.Resize(configuration.Resolution.w, configuration.Resolution.h*550/600);
    sprite.SetY(-decalage*configuration.Resolution.h/600);

    moteurGraphique.AjouterCommande(&sprite,15,0);

   // ecran->Draw(sprite);
}

void Menu::AjouterSang(coordonneeDecimal position)
{
    m_sang.push_back(position);
    /*m_alphaSang+=64;
    if(m_alphaSang>255)
        m_alphaSang=255;*/
}
void Menu::AjouterAme(coordonneeDecimal position,int pointAme)
{
    Ame temp(position,pointAme);
    m_ame.push_back(temp);
}
int Menu::GererDynamique(float temps)
{
    int nombre_ame_absorbee=0, nombre_sang_disparu=0, retour=0;
    for (Iter = m_ame.begin(); Iter != m_ame.end(); Iter++ )
    {
        if(Iter->m_mode==-1)
        {
            Iter->m_taille+=5*temps;
            Iter->m_alpha+=500*temps;

            if(Iter->m_alpha>=128)
                Iter->m_alpha=128;

            if(Iter->m_taille>=1.5)
                Iter->m_taille=1.5;

            if(Iter->m_alpha>=128&&Iter->m_taille>=1)
            {
                Iter->m_alpha=128;
                Iter->m_taille=1;
                Iter->m_mode=0;

            }
        }
        if(Iter->m_mode==0)
        {
            if(Iter->m_depart.x<=400&&Iter->m_position.x>=383&&Iter->m_position.y>=540||Iter->m_depart.x>400&&Iter->m_position.x<=384&&Iter->m_position.y>=540)
            {
                Iter->m_position.x=384;
                Iter->m_position.y=540;
                Iter->m_mode=1,retour+=Iter->m_pointAme;
            }
            else
            {
                Iter->m_rotation-=200*temps;
                Iter->m_position.x+=(384-Iter->m_depart.x)*temps*0.5;
                Iter->m_position.y+=(540-Iter->m_depart.y)*temps*0.5;

                if(Iter->m_taille>1.5)
                    Iter->augmenter=false;
                 if(Iter->m_taille<0.75)
                    Iter->augmenter=true;

                if(Iter->augmenter)
                    Iter->m_taille+=3*temps;
                else
                    Iter->m_taille-=3*temps;

            }
        }
        if(Iter->m_mode==1)
        {
            if(Iter->m_taille<2)
            {
                Iter->m_taille+=2*temps;
                Iter->m_alpha+=200*temps;
                Iter->m_rotation-=300*temps;
            }
            else
                Iter->m_mode=2;
        }
        if(Iter->m_mode==2)
        {
            if(Iter->m_taille>0.1)
            {
                Iter->m_taille-=1*temps*2;
                Iter->m_alpha-=200*temps;
                Iter->m_rotation-=500*temps;
            }
            else
                Iter->m_mode=3;
        }
        if(Iter->m_mode==3)
            nombre_ame_absorbee++;
    }

    for (IterSang = m_sang.begin(); IterSang != m_sang.end(); IterSang++ )
    {
        IterSang->m_alpha-=temps*300;

        if(IterSang->m_alpha<0)
            IterSang->m_alpha=0;
        if(IterSang->m_alpha==0)
            nombre_sang_disparu++;
    }

    if(nombre_ame_absorbee>=m_ame.size()&&m_ame.size()>0)
        m_ame.clear();
    if(nombre_sang_disparu>=m_sang.size()&&m_sang.size()>0)
        m_sang.clear();

    //m_alphaSang-=temps*200;

   /* if(m_alphaSang<0)
        m_alphaSang=255,m_sang.clear();*/

    return retour;
}

