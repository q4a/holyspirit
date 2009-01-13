#include "Decor.h"
#include "Globale.h"

Decor::Decor(int tileset,int tile,std::vector<int> evenement,int monstre,int herbe, int couche)
{
    m_tileset=tileset;
	m_tile=tile;
	m_evenement=evenement;
	m_monstre=monstre;
	m_herbe=herbe;
	m_numeroHerbe=0;
	m_animation=0;
	m_couche=couche;
}

void Decor::setDecor(int tileset,int tile,std::vector<int> evenement,int monstre,int herbe, int couche)
{
	m_tileset=tileset;
	m_tile=tile;
	m_evenement=evenement;
	m_monstre=monstre;
	m_herbe=herbe;
	m_numeroHerbe=0;
	m_couche=couche;

	if(m_couche>4)
        m_couche=4;
    if(m_couche<0)
        m_couche=0;
}

void Decor::setNumeroHerbe(int numero)
{
    m_numeroHerbe=numero;
    m_herbe_taille = (rand()% (125 - 75 )) + 75;
	m_herbe_decalage.x = 48 - (rand()%96);
	m_herbe_decalage.y = 16 - (rand()%32);

	int randomIntensite=rand() % ( 255 - 128 +1 ) + 128;
	int rougatre=rand() % (64);

	m_herbe_couleur.r=randomIntensite;
	m_herbe_couleur.g=randomIntensite - rougatre;
	m_herbe_couleur.b=randomIntensite - rougatre;
}
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
int Decor::getCouche(){return m_couche;}

int Decor::getHerbe(){return m_herbe;}
int Decor::getNumeroHerbe(){return m_numeroHerbe;}
int Decor::getTailleHerbe(){return m_herbe_taille;}
sf::Color Decor::getCouleurHerbe(){return m_herbe_couleur;}
coordonnee Decor::getDecalageHerbe(){return m_herbe_decalage;}

float Decor::getAnimation(){return m_animation;}
Objet Decor::getObjet(int numero){ if(numero>=0&&numero<m_objets.size()) return m_objets[numero];}
std::vector<Objet> Decor::getObjets(){return m_objets;}
int Decor::getNombreObjets(){return m_objets.size();}

void Decor::decrementerAnimation(){m_animation-=0.075;}
void Decor::augmenterAnimation(float temps){m_animation+=temps;}



