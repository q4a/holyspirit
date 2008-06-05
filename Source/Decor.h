///**************************************************\\\
||| Dernière modification : Le 06/02/08 par Gregouar |||
///**************************************************///




#ifndef DECORH
#define DECORH

#include <iostream>
#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "constantes.h"

class Decor
{
	public:
	Decor(int tileset,int tile,int evenement,int monstre,int herbe);
	void setDecor(int tileset,int tile,int evenement,int monstre,int herbe); //Définir une valeur au décor
	void setNumeroHerbe(int numero);

	int getTile(); // Prendre le numéro du tile du décor
	int getTileset(); // Prendre le numéro du tileset du décor
	int getPosition(char type); // Prendre la position du décor
	int getEvenement(); // Prendre le numéro de l'événement du décor
	int getMonstre(); // Retourne l'ID du monstre sur la case
	int getHerbe(); // Retourne le numero de l'herbe du décor
	int getNumeroHerbe();

	private:
	int m_tileset,m_tile,m_evenement,m_monstre,m_herbe,m_numeroHerbe;
	coordonnee m_position;
};

#endif

