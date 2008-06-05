#include "Decor.h"



Decor::Decor(int tileset,int tile,int evenement,int monstre,int herbe)
{
    m_tileset=tileset;
	m_tile=tile;
	m_evenement=evenement;
	m_monstre=monstre;
	m_herbe=herbe;
	m_numeroHerbe=0;
}

void Decor::setDecor(int tileset,int tile,int evenement,int monstre,int herbe)
{
	m_tileset=tileset;
	m_tile=tile;
	m_evenement=evenement;
	m_monstre=monstre;
	m_herbe=herbe;
	m_numeroHerbe=0;
}

void Decor::setNumeroHerbe(int numero){m_numeroHerbe=numero;};

int Decor::getTile(){return m_tile;}
int Decor::getTileset(){return m_tileset;}
int Decor::getEvenement(){return m_evenement;}
int Decor::getMonstre(){return m_monstre;}
int Decor::getHerbe(){return m_herbe;}
int Decor::getNumeroHerbe(){return m_numeroHerbe;}



