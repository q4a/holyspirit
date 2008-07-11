
#ifndef C_CHARGEMENT
#define C_CHARGEMENT

#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>

#include "Contexte.h"

class c_Chargement : public Contexte
{
    public:
    c_Chargement(Jeu *jeu);
    ~c_Chargement(){delete camera;}
    void Utiliser(Jeu *jeu);
    void setC_Chargement(int numeroMap,coordonnee coordonneePerso,bool debut=false);

    sf::View *camera;

    private:

	std::string nomMap;


	float tempsActuel,tempsPrecedent,temps_ecoule,tempsEcouleDepuisDernierAffichage;
	float z;
	bool augmenterNoir,allerVersImageChargement,m_debut;
	coordonnee m_coordonneePerso;
	int numeroProchaineMap,m_fond;
	float m_tempsChargement;

};

#endif


