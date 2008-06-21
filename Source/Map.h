#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

#ifndef MAPH
#define MAPH

#include "Tileset.h"
#include "Decor.h"
#include "Menu.h"
#include "Monstre.h"
#include "Evenement.h"


class Map
{
	public:
	Map();
	~Map();
	bool Charger(int numeroMap);
	void Afficher(sf::RenderWindow* ecran,sf::View *camera,int type,Hero *hero,coordonnee positionSouris);
	void AfficherNomEvenement(sf::RenderWindow* ecran,coordonnee casePointee,coordonnee positionSouris);

	bool getCollision(int positionX,int positionY); // Retourne 1 s'il y a une collision avec le décors se trouvant à la position X et Y
	int getTypeCase(int positionX,int positionY);
	int getEvenement(coordonnee casePointee);
	int getMonstre(Hero *hero,sf::View *camera,sf::RenderWindow *ecran,coordonnee positionSouris,coordonnee casePointee);
	Monstre *getEntiteMonstre (int numeroMonstre);
	coordonnee getPositionMonstre(int numeroMonstre);
	std::vector<std::vector<bool> > getAlentourDuPersonnage(coordonnee positionPersonnage); // Retourne un tableau de bool contenant toutes les collitions dans les alentour du héro, pour le pathfinding

	void animer(Hero *hero,float temps,Menu *menu); // Animation des tiles
	bool testEvenement(sf::RenderWindow* ecran,Hero *hero,sf::View *camera,Menu* menu);
	void calculerOmbresEtLumieres(sf::RenderWindow* ecran,Hero *hero,sf::View *camera);
	void detruireOmbresEtLumieres(Hero *hero);

	bool infligerDegats(int numeroMontre, int degats,Menu *menu,sf::View *camera);

	void gererMonstres(Hero *hero,float temps);

	void musiquePlay(coordonnee position);

	coordonnee getDimensions();

	private:
	int m_monstreIllumine;
	Lumiere m_lumiere[24];
	std::vector <Tileset> m_tileset;
	std::vector <Tileset> m_herbe;
	std::vector < std::vector< std::vector < Decor > > > m_decor/*(2,std::vector< std::vector< Decor > >(0,std::vector < Decor > (0,-1,-1,-1,-1)))*/;
	std::vector <Evenement> m_evenement;
	sf::Music m_musique;
	std::string m_nom;

	sf::Image lumiereMask;

	sf::PostFX EffectNoir;

	std::vector <Modele_Monstre> m_ModeleMonstre;
	std::vector <Monstre> m_monstre;

	sf::Image carreBrun,carreBleu,carreRouge,carreVert;

	LumiereOmbrage m_tableauDesLampes[30][30],m_lumiereHero; // Tableau de lumière et ombres, 30X30 car c'est le nbr max de tiles affichables à l'écran
};

#endif

