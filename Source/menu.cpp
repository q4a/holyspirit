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
    console.Ajouter("",0);
    console.Ajouter("Chargement des menus :",0);
	if(!m_imageMiniMap.LoadFromFile(configuration.chemin_menus+configuration.nom_minimap))
	    console.Ajouter("Impossible de charger l'image :"+configuration.chemin_menus+configuration.nom_minimap,1);
    else
    console.Ajouter("Chargement de : "+configuration.chemin_menus+configuration.nom_minimap,0);
    if(!m_imageHUD.LoadFromFile(configuration.chemin_menus+configuration.nom_hud))
	    console.Ajouter("Impossible de charger l'image :"+configuration.chemin_menus+configuration.nom_hud,1);
    else
    console.Ajouter("Chargement de : "+configuration.chemin_menus+configuration.nom_hud,0);

    if(!m_imageBulleVie.LoadFromFile(configuration.chemin_menus+configuration.nom_bulle_vie))
	    console.Ajouter("Impossible de charger l'image :"+configuration.chemin_menus+configuration.nom_bulle_vie,1);
    else
    console.Ajouter("Chargement de : "+configuration.chemin_menus+configuration.nom_bulle_vie,0);

	m_fondMiniMap.Create(configuration.Resolution.x/4, configuration.Resolution.x/4, sf::Color(0, 0, 0));
}

void Menu::Afficher(sf::RenderWindow* ecran,int type)
{
	Sprite Sprite,Sprite2;

	if(type==1)
    {
        //On affiche le fond noir de la mini-map
		Sprite.SetImage(m_fondMiniMap);
		Sprite.SetLeft(configuration.Resolution.x-configuration.Resolution.x*0.25);
		Sprite.SetTop(0);
		ecran->Draw(Sprite);
    }

    if(type==2)
    {
        //On affiche le cadran de la mini map
		Sprite.SetImage(m_imageMiniMap);
		Sprite.SetLeft(configuration.Resolution.x-configuration.Resolution.x/4);
		Sprite.SetTop(0);
		Sprite.Resize(configuration.Resolution.x*0.25, configuration.Resolution.x*0.25);
		ecran->Draw(Sprite);
    }

    if(type==3)
    {
        //On affiche l'HUD
		Sprite2.SetImage(m_imageHUD);
		Sprite2.SetLeft(0);
		Sprite2.SetTop(configuration.Resolution.y-128*configuration.Resolution.y/600);
		Sprite2.Resize(configuration.Resolution.x, 128*configuration.Resolution.y/600);
		ecran->Draw(Sprite2);
    }
}


void Menu::AfficherDynamique(sf::RenderWindow* ecran,Caracteristique caracteristique)
{
    if(caracteristique.vie>0)
    {
        Sprite Sprite;

        Sprite.SetImage(m_imageBulleVie);
        Sprite.Resize(74*configuration.Resolution.x/800, 107*configuration.Resolution.y/600);
        Sprite.SetSubRect(sf::IntRect(0, (int)(107-caracteristique.vie*107/caracteristique.maxVie), 74, 107));
        Sprite.SetLeft(10*configuration.Resolution.x/800);
        Sprite.SetTop(configuration.Resolution.y-(128-10)*configuration.Resolution.y/600+(caracteristique.maxVie-caracteristique.vie)/caracteristique.maxVie*107);


        ecran->Draw(Sprite);
    }
}
