#include "menu.h"
#include "Globale.h"

#include <iostream>
#include <fstream>

#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>

using namespace std;
using namespace sf;

Menu::Menu()
{
    m_alphaSang=0;
    console.Ajouter("",0);
    console.Ajouter("Chargement des menus :",0);
	if(!m_imageMiniMap.LoadFromFile(configuration.chemin_menus+configuration.nom_minimap))
	    console.Ajouter("Impossible de charger : "+configuration.chemin_menus+configuration.nom_minimap,1);
    else
    console.Ajouter("Chargement de : "+configuration.chemin_menus+configuration.nom_minimap,0);
    if(!m_imageHUD.LoadFromFile(configuration.chemin_menus+configuration.nom_hud))
	    console.Ajouter("Impossible de charger : "+configuration.chemin_menus+configuration.nom_hud,1);
    else
    console.Ajouter("Chargement de : "+configuration.chemin_menus+configuration.nom_hud,0);

    if(!m_imageBulleVie.LoadFromFile(configuration.chemin_menus+configuration.nom_bulle_vie))
	    console.Ajouter("Impossible de charger : "+configuration.chemin_menus+configuration.nom_bulle_vie,1);
    else
    console.Ajouter("Chargement de : "+configuration.chemin_menus+configuration.nom_bulle_vie,0);

    if(!m_imageAme.LoadFromFile(configuration.chemin_menus+configuration.nom_ame))
	    console.Ajouter("Impossible de charger : "+configuration.chemin_menus+configuration.nom_ame,1);
    else
    console.Ajouter("Chargement de : "+configuration.chemin_menus+configuration.nom_ame,0);

     if(!m_imageSang.LoadFromFile(configuration.chemin_menus+configuration.nom_sang))
	    console.Ajouter("Impossible de charger : "+configuration.chemin_menus+configuration.nom_sang,1);
    else
    console.Ajouter("Chargement de : "+configuration.chemin_menus+configuration.nom_sang,0);

    if(!m_barrePointAme.LoadFromFile(configuration.chemin_menus+configuration.nom_barre_ame))
	    console.Ajouter("Impossible de charger : "+configuration.chemin_menus+configuration.nom_barre_ame,1);
    else
    console.Ajouter("Chargement de : "+configuration.chemin_menus+configuration.nom_barre_ame,0);

    if(!m_barreVie.LoadFromFile(configuration.chemin_menus+configuration.nom_barre_vie))
	    console.Ajouter("Impossible de charger : "+configuration.chemin_menus+configuration.nom_barre_vie,1);
    else
    console.Ajouter("Chargement de : "+configuration.chemin_menus+configuration.nom_barre_vie,0);

    if(!m_barreVieVide.LoadFromFile(configuration.chemin_menus+configuration.nom_barre_vie_vide))
	    console.Ajouter("Impossible de charger : "+configuration.chemin_menus+configuration.nom_barre_vie_vide,1);
    else
    console.Ajouter("Chargement de : "+configuration.chemin_menus+configuration.nom_barre_vie_vide,0);
	m_fondMiniMap.Create(configuration.Resolution.x/4, configuration.Resolution.x/4, sf::Color(0, 0, 0));

	texte.SetSize(16.f);
}

void Menu::Afficher(sf::RenderWindow* ecran,int type)
{
	Sprite Sprite,Sprite2;

	if(type==1)
    {
        //On affiche le fond noir de la mini-map
		Sprite.SetImage(m_fondMiniMap);
		Sprite.SetX(configuration.Resolution.x-configuration.Resolution.x*0.25);
		Sprite.SetY(0);
		Sprite.SetColor(sf::Color(255,255,255,64));
		ecran->Draw(Sprite);
    }

    if(type==2)
    {
        //On affiche le cadran de la mini map
		Sprite.SetImage(m_imageMiniMap);
		Sprite.SetX(configuration.Resolution.x-configuration.Resolution.x/4);
		Sprite.SetY(0);
		Sprite.Resize(configuration.Resolution.x*0.25, configuration.Resolution.x*0.25);
		ecran->Draw(Sprite);
    }

    if(type==3)
    {
        //On affiche l'HUD
		Sprite2.SetImage(m_imageHUD);
		Sprite2.SetX(0);
		Sprite2.SetY(configuration.Resolution.y-128*configuration.Resolution.y/600);
		Sprite2.Resize(configuration.Resolution.x, 128*configuration.Resolution.y/600);
		ecran->Draw(Sprite2);
    }
}


void Menu::AfficherDynamique(sf::RenderWindow* ecran,Caracteristique caracteristique,int monstreVise,Caracteristique caracteristiqueMonstre)
{
    if(caracteristique.vie>0)
    {
        Sprite Sprite;

        Sprite.SetImage(m_imageBulleVie);
        Sprite.Resize(74*configuration.Resolution.x/800, 107*configuration.Resolution.y/600);
        Sprite.SetSubRect(sf::IntRect(0, (int)(107-caracteristique.vie*107/caracteristique.maxVie), 74, 107));
        Sprite.SetX(10*configuration.Resolution.x/800);
        Sprite.SetY(configuration.Resolution.y-(128-10)*configuration.Resolution.y/600+(caracteristique.maxVie-caracteristique.vie)/caracteristique.maxVie*107*configuration.Resolution.y/600);

        ecran->Draw(Sprite);
    }

    if(caracteristique.pointAme>0)
    {
        Sprite Sprite;

        Sprite.SetImage(m_barrePointAme);
        Sprite.Resize(configuration.Resolution.x/4, 16*configuration.Resolution.x/800);
        Sprite.SetSubRect(sf::IntRect(0, 0, (int)((float)(caracteristique.ancienPointAme-(float)((caracteristique.niveau-1)*(caracteristique.niveau-1)*10))/(float)(((caracteristique.niveau)*(caracteristique.niveau)*10)-(float)((caracteristique.niveau-1)*(caracteristique.niveau-1)*10))*200), (int)(16)));

        Sprite.SetX(configuration.Resolution.x*1.5/4+4);
        Sprite.SetY(configuration.Resolution.y-20*configuration.Resolution.y/600);

        ecran->Draw(Sprite);
    }

        char chaine[3];

        sprintf(chaine,"%ld",caracteristique.niveau);
        texte.SetText(chaine);

        texte.SetSize(16.f*configuration.Resolution.y/600);

        texte.SetX(configuration.Resolution.x/2-(texte.GetRect().Right-texte.GetRect().Left)/2);
        texte.SetY(configuration.Resolution.y-40*configuration.Resolution.y/600);

        ecran->Draw(texte);

    for(int i=0;i<m_ame.size();i++)
    {
        if(m_ame[i].m_mode<3)
        {
            Sprite Sprite;

            Sprite.SetImage(m_imageAme);
             Sprite.SetCenter(16,16);
            Sprite.Resize(32*configuration.Resolution.x/800*m_ame[i].m_taille, 32*configuration.Resolution.x/800*m_ame[i].m_taille);
            Sprite.SetColor(sf::Color(255,255,255,(int)m_ame[i].m_alpha));
            Sprite.SetX((m_ame[i].m_position.x+16)*configuration.Resolution.x/800);
            Sprite.SetY((m_ame[i].m_position.y+16)*configuration.Resolution.y/600);


            Sprite.SetRotation(m_ame[i].m_rotation);
            ecran->Draw(Sprite);
        }
    }

    for(int i=0;i<m_sang.size();i++)
    {
        Sprite Sprite;

        Sprite.SetImage(m_imageSang);
        Sprite.SetCenter(200*m_sang[i].m_taille,200*m_sang[i].m_taille);
        Sprite.SetRotation(m_sang[i].m_rotation);
         Sprite.SetCenter(0,0);
        Sprite.Resize(200*configuration.Resolution.x/800*m_sang[i].m_taille, 200*configuration.Resolution.y/600*m_sang[i].m_taille);
        Sprite.SetColor(sf::Color(255,255,255,(int)m_alphaSang));
        Sprite.SetX(m_sang[i].m_position.x*configuration.Resolution.x/800);
        Sprite.SetY(m_sang[i].m_position.y*configuration.Resolution.y/600);

        ecran->Draw(Sprite);
    }

    if(monstreVise>-1)
    {
        Sprite Sprite,Sprite2;

        Sprite.SetImage(m_barreVieVide);
         Sprite.Resize(configuration.Resolution.x/2, 32*configuration.Resolution.x/800);
         Sprite.SetX(configuration.Resolution.x/2-Sprite.GetSize().x/2);
        Sprite.SetY(8);

        ecran->Draw(Sprite);

        Sprite2.SetImage(m_barreVie);
        Sprite2.Resize(configuration.Resolution.x/2, 32*configuration.Resolution.x/800);
        Sprite2.SetSubRect(sf::IntRect(0, 0, (int)(caracteristiqueMonstre.vie/caracteristiqueMonstre.maxVie*400), 32));
        Sprite2.SetX(configuration.Resolution.x/2-Sprite.GetSize().x/2);
        Sprite2.SetY(8);


        ecran->Draw(Sprite2);

        char chaine[255];

         texte.SetSize(18.f*configuration.Resolution.y/600);

        sprintf(chaine,"%s (%ld)",caracteristiqueMonstre.nom.c_str(),caracteristiqueMonstre.niveau);
        texte.SetText(chaine);

        texte.SetX(configuration.Resolution.x/2-(texte.GetRect().Right-texte.GetRect().Left)/2+2);
        texte.SetY(14*configuration.Resolution.y/600);
        texte.SetColor(Color(0,0,0,255));
        ecran->Draw(texte);
        texte.SetColor(Color(255,255,255,255));

        texte.SetX(configuration.Resolution.x/2-(texte.GetRect().Right-texte.GetRect().Left)/2);
        texte.SetY(12*configuration.Resolution.y/600);
        ecran->Draw(texte);


    }
}

void Menu::AjouterSang(coordonnee position)
{
    m_sang.push_back(position);
    m_alphaSang+=64;
    if(m_alphaSang>255)
        m_alphaSang=255;
}
void Menu::AjouterAme(coordonneeDecimal position,int pointAme)
{
    Ame temp(position,pointAme);
    m_ame.push_back(temp);
}
int Menu::GererDynamique(float temps)
{
    int nombre_ame_absorbee=0, retour=0;
     for (Iter = m_ame.begin(); Iter != m_ame.end(); Iter++ )
    {
        if(Iter->m_mode==0)
        {
            if(Iter->m_depart.x<=400&&Iter->m_position.x>=383&&Iter->m_position.y>=520||Iter->m_depart.x>400&&Iter->m_position.x<=384&&Iter->m_position.y>=520)
            {
                Iter->m_position.x=384;
                Iter->m_position.y=520;
                Iter->m_mode=1,retour+=Iter->m_pointAme;
            }
            else
            {
                Iter->m_rotation-=200*temps;
                Iter->m_position.x+=(384-Iter->m_depart.x)*temps*0.5;
                Iter->m_position.y+=(520-Iter->m_depart.y)*temps*0.5;

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

    if(nombre_ame_absorbee>=m_ame.size())
        m_ame.clear();

    m_alphaSang-=temps*200;

    if(m_alphaSang<0)
        m_alphaSang=255,m_sang.clear();

    return retour;
}

