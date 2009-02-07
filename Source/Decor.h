


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
	void setDecor(int tileset,int tile,std::vector<int> evenement,int monstre,int herbe,int couche,int hauteur,std::vector <Objet> objets); //Définir une valeur au décor
	void setNumeroHerbe(int numero);
	void setMonstre(int monstre);
	void setProjectile(int projectile);
	void setEffetGraphique(int effet);

	float getAnimation();

	void decrementerAnimation(float nombre);
	void augmenterAnimation(float temps);

	int getTile(); // Prendre le numéro du tile du décor
	int getTileset(); // Prendre le numéro du tileset du décor
	int getPosition(char type); // Prendre la position du décor
	std::vector<int> getEvenement(); // Prendre le numéro de l'événement du décor
	int getMonstre(); // Retourne l'ID du monstre sur la case
	int getProjectile();
	int getEffetGraphique();
	int getCouche();
	int getHerbe(); // Retourne le numero de l'herbe du décor
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


