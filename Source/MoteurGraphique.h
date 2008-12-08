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
	void AjouterCommande(sf::Sprite*,int=0, bool=0);
	void AjouterTexte(sf::String*, int couche=0,bool titre=false);

	void Charger();

	void Vider();

	void calculerOmbresEtLumieres(sf::RenderWindow* ecran,Hero *hero,sf::View *camera,Map *map);
	void AjouterCommandesMap(Map *map);

	sf::Image* getImage(int IDimage);

	std::string getCheminImage(int IDimage);



	sf::PostFX EffectBlur,EffectMort;

	sf::Font m_font_titre;

	float m_blur;

	private:

	std::vector <sf::Image> m_images;
	std::vector <std::string> m_cheminsImages;//Permet de s'assurer d'avoir des singletons
	std::vector <sf::String> m_textes[20];
	std::vector <Commande>m_commandes[20];


};

#endif



