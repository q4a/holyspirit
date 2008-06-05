#ifndef HEROCPP
#define HEROCPP

#include "Hero.h"
#include "Globale.h"

#include <iostream>
#include <fstream>

#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include <math.h>

using namespace std;
using namespace sf;

Hero::Hero()
{
   ///Je donnes des valeur à mes variables juste pour les tests
	m_personnage.setEtat(ARRET);

	coordonnee position;
	position.x=0;
	position.y=0;

	m_personnage.setCoordonnee(position);

	Lumiere lumiere;
	lumiere.intensite=128;
	lumiere.rouge=236;
	lumiere.vert=136;
	lumiere.bleu=36;

	m_modelePersonnage.setPorteeLumineuse(lumiere);
}


void Hero::placerCamera(sf::View *camera,coordonnee dimensionsMap)
{
	m_positionAffichage.y=(int)(((m_personnage.getCoordonneePixel().x+m_personnage.getCoordonneePixel().y)*64/COTE_TILE)/2);
	m_positionAffichage.x=(int)(((m_personnage.getCoordonneePixel().x-m_personnage.getCoordonneePixel().y)*64/COTE_TILE+dimensionsMap.y*64)-64);

	coordonnee positionCamera;
	positionCamera.y=m_positionAffichage.y-250*configuration.Resolution.y/600;
	positionCamera.x=m_positionAffichage.x-((400*configuration.Resolution.x/800))+64;
	camera->Rect.Top=positionCamera.y;
	camera->Rect.Left=positionCamera.x;
	camera->Rect.Bottom=positionCamera.y+configuration.Resolution.y;
	camera->Rect.Right=positionCamera.x+configuration.Resolution.x;
}
#endif
