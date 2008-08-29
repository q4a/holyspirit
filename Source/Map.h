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
#include "c_changementMap.h"

class Jeu;

class Map
{
	public:
	Map();
	~Map();
	bool Charger(int numeroMap);
	void Sauvegarder();
	void Afficher(sf::RenderWindow* ecran,sf::View *camera,int type,Hero *hero,coordonnee positionSouris);
	void AfficherNomEvenement(sf::RenderWindow* ecran,coordonnee casePointee,coordonnee positionSouris);

	int getMonstreIllumine();
	bool getCollision(int positionX,int positionY); // Retourne 1 s'il y a une collision avec le décors se trouvant à la position X et Y
	int getTypeCase(int positionX,int positionY);
	int getEvenement(coordonnee casePointee);
	int getMonstre(Hero *hero,sf::View *camera,sf::RenderWindow *ecran,coordonnee positionSouris,coordonnee casePointee);
	Monstre *getEntiteMonstre (int numeroMonstre);
	coordonnee getPositionMonstre(int numeroMonstre);
	std::vector<std::vector<bool> > getAlentourDuPersonnage(coordonnee positionPersonnage); // Retourne un tableau de bool contenant toutes les collitions dans les alentour du héro, pour le pathfinding

	void animer(Hero *hero,float temps,Menu *menu); // Animation des tiles
	bool testEvenement(sf::View *camera, Jeu *jeu,float temps);
	void calculerOmbresEtLumieres(sf::RenderWindow* ecran,Hero *hero,sf::View *camera);
	void Detruire();

	bool infligerDegats(int numeroMontre, int degats,Menu *menu,sf::View *camera);

	void gererMonstres(Hero *hero,float temps);

	void musiquePlay(coordonnee position);

	void setVolumeMusique(int volume);

	void verifierDeclencheursDegats(int i, int j);

	coordonnee getDimensions();

	private:
	int m_monstreIllumine,m_numero;
	Lumiere m_lumiere[24];
	std::vector <Tileset> m_tileset;
	std::vector <Tileset> m_herbe;
	std::vector < std::vector< std::vector < Decor > > > m_decor;
	std::vector <Evenement> m_evenement;
	sf::Music m_musique;
	std::string m_nom,m_fond,m_cheminMusique;

	sf::Image lumiereMask;

	std::vector <Modele_Monstre> m_ModeleMonstre;
	std::vector <Monstre> m_monstre;

	sf::Image carreBrun,carreBleu,carreRouge,carreVert;

	LumiereOmbrage m_tableauDesLampes[30][30],m_tableauDesLampesBasique[30][30],m_lumiereHero; // Tableau de lumière et ombres, 30X30 car c'est le nbr max de tiles affichables à l'écran
};


#endif

