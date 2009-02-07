


#ifndef DECORH
#define DECORH

#include <iostream>
#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "constantes.h"

#include "Objet.h"

class Decor
{
	public:
	Decor(){}
	Decor(int tileset,int tile,std::vector<int> evenement,int monstre,int herbe, int couche,int hauteur);
	Decor(int tileset,int tile,std::vector<int> evenement,int monstre,int herbe, int couche,int hauteur,std::vector <Objet> objets);
	~Decor();
	Decor operator=(const Decor &Decor);
	void setDecor(int tileset,int tile,std::vector<int> evenement,int monstre,int herbe,int couche,int hauteur);
	void setDecor(int tileset,int tile,std::vector<int> evenement,int monstre,int herbe,int couche,int hauteur,std::vector <Objet> objets); //D�finir une valeur au d�cor
	void setNumeroHerbe(int numero);
	void setMonstre(int monstre);
	void setProjectile(int projectile);
	void setEffetGraphique(int effet);

	float getAnimation();

	void decrementerAnimation(float nombre);
	void augmenterAnimation(float temps);

	int getTile(); // Prendre le num�ro du tile du d�cor
	int getTileset(); // Prendre le num�ro du tileset du d�cor
	int getPosition(char type); // Prendre la position du d�cor
	std::vector<int> getEvenement(); // Prendre le num�ro de l'�v�nement du d�cor
	int getMonstre(); // Retourne l'ID du monstre sur la case
	int getProjectile();
	int getEffetGraphique();
	int getCouche();
	int getHerbe(); // Retourne le numero de l'herbe du d�cor
	int getNumeroHerbe();
	int getTailleHerbe();
	int getHauteur();
	sf::Color getCouleurHerbe();
    coordonnee getDecalageHerbe();
	Objet getObjet(int numero);
	void supprimerObjet(int numero);
	std::vector<Objet> getObjets();
	int getNombreObjets();

	void ajouterObjet(Objet);


	void setEvenement(int evenement,int numero);
	void ajouterEvenement(int evenement);
	void setTileset( int tileset);
	void setTile(int tile);

	private:
	int m_tileset,m_tile,m_monstre,m_projectile,m_effet,m_couche;
	std::vector<int> m_evenement;
	float m_animation;
	coordonnee m_position;

	std::vector<Objet> m_objets;

	int m_herbe,m_numeroHerbe;
	int m_herbe_taille;
	int m_hauteur;
	coordonnee m_herbe_decalage;
	sf::Color m_herbe_couleur;
};

#endif


