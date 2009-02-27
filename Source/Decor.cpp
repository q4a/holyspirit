#include "Decor.h"
#include "Globale.h"

Decor::Decor(int tileset,int tile,std::vector<int> evenement,int monstre,int herbe,int hauteur, int couche)
{
    m_tileset=tileset;
	m_tile=tile;
	m_evenement=evenement;
	m_monstre=monstre;
	m_herbe=herbe;
	m_numeroHerbe=0;
	m_animation=0;
	m_couche=couche;
    m_hauteur=hauteur;

	m_projectile=-1;
	m_effet=-1;

	m_sprite.SetSubRect(sf::IntRect(0,0,0,0));
	m_spriteHerbe.SetSubRect(sf::IntRect(0,0,0,0));
	m_spriteOmbre.SetSubRect(sf::IntRect(0,0,0,0));
}

Decor::Decor(int tileset,int tile,std::vector<int> evenement,int monstre,int herbe, int couche,int hauteur,std::vector <Objet> objets)
{
    m_tileset=tileset;
	m_tile=tile;
	m_evenement=evenement;
	m_monstre=monstre;
	m_herbe=herbe;
	m_numeroHerbe=0;
	m_animation=0;
	m_couche=couche;
	m_hauteur=hauteur;

	m_objets=objets;

	m_projectile=-1;
	m_effet=-1;

    m_sprite.SetSubRect(sf::IntRect(0,0,0,0));
	m_spriteHerbe.SetSubRect(sf::IntRect(0,0,0,0));
	m_spriteOmbre.SetSubRect(sf::IntRect(0,0,0,0));
}

Decor::~Decor()
{
    if(m_evenement.size()>0)
        m_evenement.clear();
    //if(configuration.debug)
       //console.Ajouter("/Destruction des evenements");
    if(m_objets.size()>0)
        m_objets.clear();
    //  if(configuration.debug)
       // console.Ajouter("/Destruction des objets");
}

Decor Decor::operator=(const Decor &Decor)
{
    m_tileset=Decor.m_tileset;
    m_tile=Decor.m_tile;
    m_monstre=Decor.m_monstre;
    m_projectile=Decor.m_projectile;
    m_effet=Decor.m_effet;
    m_couche=Decor.m_couche;
    m_animation=Decor.m_animation;
    m_position=Decor.m_position;
    m_herbe=Decor.m_herbe;
    m_numeroHerbe=Decor.m_numeroHerbe;
    m_herbe_taille=Decor.m_herbe_taille;
    m_herbe_decalage=Decor.m_herbe_decalage;
    m_herbe_couleur=Decor.m_herbe_couleur;
    m_hauteur=Decor.m_hauteur;


    m_evenement=Decor.m_evenement;
	m_objets=Decor.m_objets;

    return *this;
}

void Decor::setDecor(int tileset,int tile,std::vector<int> evenement,int monstre,int herbe, int couche,int hauteur)
{
	m_tileset=tileset;
	m_tile=tile;
	m_evenement=evenement;
	m_monstre=monstre;
	m_herbe=herbe;
	m_numeroHerbe=0;
	m_couche=couche;
	m_hauteur=hauteur;

	if(m_couche>4)
        m_couche=4;
    if(m_couche<0)
        m_couche=0;
}

void Decor::setDecor(int tileset,int tile,std::vector<int> evenement,int monstre,int herbe, int couche,int hauteur,std::vector <Objet> objets)
{
	m_tileset=tileset;
	m_tile=tile;
	m_evenement=evenement;
	m_monstre=monstre;
	m_herbe=herbe;
	m_numeroHerbe=0;
	m_couche=couche;
	m_hauteur=hauteur;

	if(m_couche>4)
        m_couche=4;
    if(m_couche<0)
        m_couche=0;
    m_objets=objets;
}


void Decor::setNumeroHerbe(int numero)
{
    m_numeroHerbe=numero;
    m_herbe_taille = (rand()% (125 - 75 )) + 75;
	m_herbe_decalage.x = 32 - (rand()%64);
	m_herbe_decalage.y = 0/*16 - (rand()%32)*/;

	int randomIntensite=rand() % ( 255 - 128 +1 ) + 128;
	int rougatre=rand() % (64);

	m_herbe_couleur.r=randomIntensite;
	m_herbe_couleur.g=randomIntensite - rougatre;
	m_herbe_couleur.b=randomIntensite - rougatre;
}
void Decor::setMonstre(int monstre){m_monstre=monstre;}
void Decor::setProjectile(int projectile){m_projectile=projectile;}
void Decor::setEffetGraphique(int effet){m_effet=effet;}
void Decor::setEvenement(int evenement, int numero)
{
    if(numero>=0&&numero<(int)m_evenement.size())
        m_evenement[numero]=evenement;
}
void Decor::ajouterEvenement(int evenement)
{
    m_evenement.push_back(evenement);
}
void Decor::setTileset( int tileset){m_tileset=tileset;}
void Decor::setTile(int tile){m_tile=tile;}
void Decor::setCouche(int couche){m_couche=couche;}
void Decor::ajouterObjet(Objet objet){m_objets.push_back(objet);}


void Decor::supprimerObjet(int numero)
{
    if(numero>=0&&numero<(int)m_objets.size())
    {
        m_objets.erase(m_objets.begin()+numero);
    }
}


int Decor::getTile(){return m_tile;}
int Decor::getTileset(){return m_tileset;}
std::vector<int> Decor::getEvenement(){return m_evenement;}
int Decor::getMonstre(){return m_monstre;}
int Decor::getProjectile(){return m_projectile;}
int Decor::getEffetGraphique(){return m_effet;}
int Decor::getCouche(){return m_couche;}

int Decor::getHerbe(){return m_herbe;}
int Decor::getNumeroHerbe(){return m_numeroHerbe;}
int Decor::getTailleHerbe(){return m_herbe_taille;}
sf::Color Decor::getCouleurHerbe(){return m_herbe_couleur;}
coordonnee Decor::getDecalageHerbe(){return m_herbe_decalage;}

float Decor::getAnimation(){return m_animation;}
Objet Decor::getObjet(int numero){ if(numero>=0&&numero<(int)m_objets.size()) return m_objets[numero]; Objet temp; return temp; }
std::vector<Objet> Decor::getObjets(){return m_objets;}
int Decor::getNombreObjets(){return m_objets.size();}
int Decor::getHauteur(){return m_hauteur;}

void Decor::decrementerAnimation(float nombre){m_animation-=nombre;}
void Decor::augmenterAnimation(float temps){m_animation+=temps;}



