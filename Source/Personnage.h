///**************************************************\\\
||| Dernière modification : Le 06/02/08 par Gregouar |||
///**************************************************///



#ifndef PERSONNAGEH
#define PERSONNAGEH

#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

#include "constantes.h"
//#include "map.h"
#include "Decor.h"
#include "liste_case.h"
#include "Pose.h"


class Modele_Personnage
{
	public:
	Modele_Personnage();
	~Modele_Personnage();
	bool Charger(std::string chemin);

	void jouerSon(int numeroSon,double distance,coordonnee position);

	void setPorteeLumineuse(Lumiere  lumiere);

	Lumiere getPorteeLumineuse();
	int getNombreSons();

	std::vector <std::vector<std::vector<Pose> > > m_pose;
	std::vector <sf::Image> m_image;

	protected:
	Lumiere m_porteeLumineuse;
	std::vector <sf::SoundBuffer> m_buffer;
	std::vector <sf::Sound>  m_sons;
};

class Personnage
{
	public:
	Personnage();
	coordonnee getCoordonnee();
	coordonnee getCoordonneePixel();
	coordonnee getProchaineCase();
	void animer(Modele_Personnage *modele,int hauteur_map,float temps);

	bool seDeplacer(float);
	void setCoordonnee(coordonnee nouvellesCoordonnees);
	void Afficher(sf::RenderWindow* ecran,sf::View *camera,coordonnee position,coordonnee dimensionsMap,LumiereOmbrage *lumiere,Modele_Personnage *modele);


	int pathfinding(std::vector<std::vector<bool> > map);

	void frappe(coordonnee direction,coordonnee position);

	void setArrivee(coordonnee arrivee);
	void setEtat(int  etat);
	void setVitesse(float vitesse);
	void setCoordonneePixel(coordonnee position);
	void setProchaineCase(coordonnee position);

	int getEtat();

	protected:
	int m_etat,m_poseEnCours,m_angle;
	float m_animation,m_vitesse;
	coordonnee m_cheminFinal,m_arrivee,m_ancienneArrivee,m_mauvaiseArrivee, m_positionCase,m_positionAffichage,m_positionPixelPrecedente;
	coordonneeDecimal m_positionPixel;
};


#endif
