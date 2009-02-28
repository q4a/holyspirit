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
	bool Charger(int numeroMap,Hero *hero);
	void Initialiser();
	void CreerSprite(sf::Vector3f position_case);
	void Sauvegarder(Hero *hero);
	void Afficher(sf::RenderWindow* ecran,sf::View *camera,int type,Hero *hero,coordonnee positionSouris,bool alt,float alpha=255);

	void AfficherSacInventaire(sf::RenderWindow* ecran,coordonnee positionSac,float decalage,coordonnee positionSouris,Classe *classe);

	void AfficherNomEvenement(sf::RenderWindow* ecran,coordonnee casePointee,coordonnee positionSouris);

	int getMonstreIllumine();
	bool getCollision(int positionX,int positionY); // Retourne 1 s'il y a une collision avec le décors se trouvant à la position X et Y
	int getTypeCase(int positionX,int positionY);
	int getEvenement(coordonnee casePointee);
	int getMonstre(Hero *hero,sf::View *camera,sf::RenderWindow *ecran,coordonnee positionSouris,coordonnee casePointee);
	coordonnee getSacPointe();
	int getObjetPointe();
	int getNombreObjets(coordonnee position);
	Monstre *getEntiteMonstre (int numeroMonstre);
	coordonnee getPositionMonstre(int numeroMonstre);
	casePathfinding ** getAlentourDuPersonnage(coordonnee positionPersonnage); // Retourne un tableau de bool contenant toutes les collitions dans les alentour du héro, pour le pathfinding

	bool getMonstreEnVie(int numeroMonstre);


	void animer(Hero *hero,float temps,Menu *menu,sf::View *camera); // Animation des tiles
	bool testEvenement(sf::View *camera, Jeu *jeu,float temps);
	void calculerOmbresEtLumieres();
	void Detruire();

	int gererMiracle(EntiteMiracle *entiteMiracle,Miracle *modeleMiracle,Hero *hero,bool monstre,coordonnee lanceur, coordonnee cible,int couche);

	void infligerDegatsMasse(coordonnee position,int rayon,int degats,bool sourceConcernee, Hero *hero,Menu *menu,sf::View *camera);

	bool infligerDegats(int numeroMontre, float degats,Menu *menu,sf::View *camera, Hero *hero,bool pousser);
	void PousserMonstre(int numeroMonstre, coordonnee vecteur);
	void PousserMonstreCase(int numeroMonstre, coordonnee vecteur);
	void gererMonstres(Hero *hero,float temps,sf::View *camera,Menu *menu);

	void gererEvenements(int evenement,int z,int couche,int x,int y);

	void musiquePlay(coordonnee position);

	void setVolumeMusique(int volume);

	void verifierDeclencheursDegats(int i, int j);

	bool ramasserObjet(Hero *hero,bool enMain = false);
	void AjouterObjet(Objet objet);

	coordonnee getDimensions();



	void gererConditions(Script *script,int noInstruction,int i, int j, int k,Hero *hero,float temps,sf::View *camera,Menu *menu);


	int m_defilerObjets;

	private:
	int m_monstreIllumine,m_numero,IDImageSac,m_objetPointe,m_calculLumiere,m_musiqueEnCours,m_nombreMusique;

	coordonnee m_sacPointe;
	Lumiere m_lumiere[24];
	std::vector <Tileset> m_tileset;
	std::vector <Herbe> m_herbe;
	std::vector < std::vector < Decor > >  m_decor[NOMBRE_COUCHE_MAP];
	std::vector <Evenement> m_evenement;

	sf::Music m_musique[MAX_MUSIQUE];
	std::string m_nom;

	std::vector <std::string> m_fond,m_cheminMusique;

	sf::Image lumiereMask;

	std::vector <Modele_Monstre> m_ModeleMonstre;
	std::vector <Monstre> m_monstre;
	std::vector <Projectile> m_projectile;
	std::vector <EffetGraphique> m_effets;

	sf::Image carreBrun,carreBleu,carreRouge,carreVert,carreJaune;

	std::vector<Decor>::iterator IterY;
	std::vector<Decor>::iterator Iter;
};


#endif

