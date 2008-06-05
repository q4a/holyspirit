///**************************************************\\\
||| Derni�re modification : Le 06/02/08 par Gregouar |||
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
	void setDecor(int tileset,int tile,int evenement,int monstre,int herbe); //D�finir une valeur au d�cor
	void setNumeroHerbe(int numero);

	int getTile(); // Prendre le num�ro du tile du d�cor
	int getTileset(); // Prendre le num�ro du tileset du d�cor
	int getPosition(char type); // Prendre la position du d�cor
	int getEvenement(); // Prendre le num�ro de l'�v�nement du d�cor
	int getMonstre(); // Retourne l'ID du monstre sur la case
	int getHerbe(); // Retourne le numero de l'herbe du d�cor
	int getNumeroHerbe();

	private:
	int m_tileset,m_tile,m_evenement,m_monstre,m_herbe,m_numeroHerbe;
	coordonnee m_position;
};

#endif

