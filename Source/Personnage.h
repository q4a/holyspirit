
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
	void Reinitialiser();

	void setPorteeLumineuse(Lumiere  lumiere);

	Lumiere getPorteeLumineuse();
	int getNombreSons();
	//int getBuffer(int ID);
	Caracteristique getCaracteristique();

	void jouerSon(int numeroSon,coordonnee position,coordonnee positionHero, bool uniqueSound=false);

	std::vector <std::vector<std::vector<Pose> > > m_pose;
	std::vector <int> m_image;

	bool m_ombre,m_explosif;

	protected:
	Caracteristique m_caracteristique;
	Lumiere m_porteeLumineuse;
	std::vector <int> m_sons;
	//std::vector <sf::Sound>  m_sons;
};

class Personnage
{
	public:
	Personnage();



	coordonnee getCoordonnee();
	coordonnee getArrivee();
	coordonnee getCoordonneePixel();
	coordonnee getProchaineCase();
	bool getErreurPathfinding();
	bool enVie();
	int animer(Modele_Personnage *modele,int hauteur_map,float temps,bool *explosif,coordonnee positionHero);

	void Pousser(coordonnee vecteur);
	void PousserCase(coordonnee vecteur);

	bool seDeplacer(float);
	void setCoordonnee(coordonnee nouvellesCoordonnees);
	void Afficher(sf::RenderWindow* ecran,sf::View *camera,coordonnee position,coordonnee dimensionsMap,Modele_Personnage *modele);

	int pathfinding(casePathfinding** map,coordonnee exception);

	void frappe(coordonnee direction,coordonnee position);



	void infligerDegats(float degats);

	void setErreurPathfinding(bool);

	void setArrivee(coordonnee arrivee);
	void setMauvaiseArrivee(coordonnee arrivee);
	void setEtat(int  etat);
	void setPose(int  pose);
	void setAngle(int  angle);
	void setVitesse(float vitesse);
	void setCoordonneePixel(coordonnee position);
	void setProchaineCase(coordonnee position);
	void setCaracteristique(Caracteristique caracteristique);
	void setPorteeLumineuse(Lumiere  lumiere);
	void regenererVie(float vie);

	void AjouterPointAme(int pointAme);

	int getEtat();
	int getAngle();
	int getPose();
	Lumiere getPorteeLumineuse();
	Caracteristique getCaracteristique();

	int getOrdre(Modele_Personnage *modele);

	LumiereOmbrage m_lumiere;
	bool frappeEnCours;
	int m_nombreInvocation;

	protected:
	int m_etat,m_poseEnCours,m_angle;
	float m_animation;
	bool m_monstre,m_erreurPathfinding;
	Caracteristique m_caracteristique;
	coordonnee m_cheminFinal,m_arrivee,m_ancienneArrivee,m_mauvaiseArrivee, m_positionCase,m_positionAffichage,m_positionPixelPrecedente;
	coordonneeDecimal m_positionPixel;
	Lumiere m_porteeLumineuse,m_porteeLumineuseBasique;


	//std::vector <int>  m_sons;
};


#endif

