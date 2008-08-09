#include "Decor.h"

Decor::Decor(int tileset,int tile,int evenement,int monstre,int herbe)
{
    m_tileset=tileset;
	m_tile=tile;
	m_evenement=evenement;
	m_monstre=monstre;
	m_herbe=herbe;
	m_numeroHerbe=0;
	m_animation=0;
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

void Decor::setNumeroHerbe(int numero){m_numeroHerbe=numero;}
void Decor::setMonstre(int monstre){m_monstre=monstre;}

int Decor::getTile(){return m_tile;}
int Decor::getTileset(){return m_tileset;}
int Decor::getEvenement(){return m_evenement;}
int Decor::getMonstre(){return m_monstre;}
int Decor::getHerbe(){return m_herbe;}
int Decor::getNumeroHerbe(){return m_numeroHerbe;}
float Decor::getAnimation(){return m_animation;}

void Decor::decrementerAnimation(){m_animation-=0.075;}
void Decor::augmenterAnimation(float temps){m_animation+=temps;}



