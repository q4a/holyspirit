#include "Decor.h"
#include "Globale.h"

Decor::Decor(int tileset,int tile,std::vector<int> evenement,int monstre,int herbe)
{
    m_tileset=tileset;
	m_tile=tile;
	m_evenement=evenement;
	m_monstre=monstre;
	m_herbe=herbe;
	m_numeroHerbe=0;
	m_animation=0;
}

void Decor::setDecor(int tileset,int tile,std::vector<int> evenement,int monstre,int herbe)
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
void Decor::setEvenement(int evenement, int numero)
{
    if(numero>=0&&numero<m_evenement.size())
        m_evenement[numero]=evenement;
}
void Decor::ajouterEvenement(int evenement)
{
    m_evenement.push_back(evenement);
}
void Decor::setTileset( int tileset){m_tileset=tileset;}
void Decor::setTile(int tile){m_tile=tile;}
void Decor::ajouterObjet(Objet objet){m_objets.push_back(objet);}


void Decor::supprimerObjet(int numero)
{
    if(numero>=0&&numero<m_objets.size())
    {
        m_objets.erase(m_objets.begin()+numero);
    }
}


int Decor::getTile(){return m_tile;}
int Decor::getTileset(){return m_tileset;}
std::vector<int> Decor::getEvenement(){return m_evenement;}
int Decor::getMonstre(){return m_monstre;}
int Decor::getHerbe(){return m_herbe;}
int Decor::getNumeroHerbe(){return m_numeroHerbe;}
float Decor::getAnimation(){return m_animation;}
Objet Decor::getObjet(int numero){ if(numero>=0&&numero<m_objets.size()) return m_objets[numero];}
std::vector<Objet> Decor::getObjets(){return m_objets;}
int Decor::getNombreObjets(){return m_objets.size();}

void Decor::decrementerAnimation(){m_animation-=0.075;}
void Decor::augmenterAnimation(float temps){m_animation+=temps;}



