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
	Caracteristique getCaracteristique();

	std::vector <std::vector<std::vector<Pose> > > m_pose;
	std::vector <sf::Image> m_image;

	bool m_ombre;

	protected:
	Caracteristique m_caracteristique;
	Lumiere m_porteeLumineuse;
	std::vector <sf::SoundBuffer> m_buffer;
	std::vector <sf::Sound>  m_sons;
};

class Personnage
{
	public:
	Personnage();
	coordonnee getCoordonnee();
	coordonnee getArrivee();
	coordonnee getCoordonneePixel();
	coordonnee getProchaineCase();
	int animer(Modele_Personnage *modele,int hauteur_map,float temps);

	bool seDeplacer(float);
	void setCoordonnee(coordonnee nouvellesCoordonnees);
	void Afficher(sf::RenderWindow* ecran,sf::View *camera,coordonnee position,coordonnee dimensionsMap,LumiereOmbrage *lumiere,Modele_Personnage *modele);

	int pathfinding(std::vector<std::vector<bool> > map,coordonnee exception);

	void frappe(coordonnee direction,coordonnee position);

	void infligerDegats(int degats);

	void setArrivee(coordonnee arrivee);
	void setMauvaiseArrivee(coordonnee arrivee);
	void setEtat(int  etat);
	void setVitesse(float vitesse);
	void setCoordonneePixel(coordonnee position);
	void setProchaineCase(coordonnee position);
	void setCaracteristique(Caracteristique caracteristique);
	void regenererVie(float vie);

	void AjouterPointAme(int pointAme);

	int getEtat();
	int getAngle();
	int getPose();
	Caracteristique getCaracteristique();

	protected:
	int m_etat,m_poseEnCours,m_angle;
	float m_animation;
	bool m_monstre;
	Caracteristique m_caracteristique;
	coordonnee m_cheminFinal,m_arrivee,m_ancienneArrivee,m_mauvaiseArrivee, m_positionCase,m_positionAffichage,m_positionPixelPrecedente;
	coordonneeDecimal m_positionPixel;
};


#endif
