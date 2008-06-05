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

	m_fondMiniMap.Create(configuration.Resolution.x/4, configuration.Resolution.y/3, sf::Color(0, 0, 0));
}

void Menu::Afficher(sf::RenderWindow* ecran,int type)
{
	Sprite Sprite,Sprite2;

	if(type==1)
    {
        //On affiche le fond noir de la mini-map
		Sprite.SetImage(m_fondMiniMap);
		Sprite.SetLeft(configuration.Resolution.x-configuration.Resolution.x/4);
		Sprite.SetTop(0);
		ecran->Draw(Sprite);
    }

    if(type==2)
    {
        //On affiche le cadran de la mini map
		Sprite.SetImage(m_imageMiniMap);
		Sprite.SetLeft(configuration.Resolution.x-configuration.Resolution.x/4);
		Sprite.SetTop(0);
		Sprite.Resize(configuration.Resolution.x/4, configuration.Resolution.y/3);
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
