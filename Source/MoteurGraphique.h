#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

#ifndef MOTEURGRAPHIQUEH
#define MOTEURGRAPHIQUEH

#include "map.h"
#include "commande.h"

class MoteurGraphique
{
	public:
	MoteurGraphique();
	~MoteurGraphique();
	void Afficher(sf::RenderWindow *, sf::View *);

	int AjouterImage(std::string);
	void AjouterCommande(sf::Sprite*, bool);
	void AjouterTexte(sf::String*);

	void Vider();

	void calculerOmbresEtLumieres(sf::RenderWindow* ecran,Hero *hero,sf::View *camera,Map *map);
	void AjouterCommandesMap(Map *map);

	sf::Image* getImage(int IDimage);

	std::string getCheminImage(int IDimage);

	std::vector <Commande> m_commandes;

	private:

	std::vector <sf::Image> m_images;
	std::vector <std::string> m_cheminsImages;
	std::vector <sf::String> m_textes;
	//Permet de s'assurer d'avoir des singletons

	LumiereOmbrage m_tableauDesLampes[30][30],m_lumiereHero; // Tableau de lumière et ombres, 30X30 car c'est le nbr max de tiles affichables à l'écran
};

#endif


