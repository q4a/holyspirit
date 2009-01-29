
#ifndef _JEUH
#define _JEUH

#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>

#include "Contexte.h"
#include "Globale.h"

class c_Jeu : public Contexte
{
    public:
    c_Jeu(Jeu *jeu);
    void Utiliser(Jeu *jeu);

   // sf::View *camera;

    private:

	bool continuer,lumiere,augmenter;
	int nbrTourBoucle;
	char chaine[10];
	sf::String variableQuiNeSertARien,Version,Temps,fps,TourBoucle;

	float tempsActuel,tempsPrecedent,tempsDepuisDerniereAnimation,tempsEcoule,tempsNbrTourBoucle,tempsEcouleDepuisDernierCalculLumiere,tempsEcouleDepuisDernierDeplacement,tempsEcouleDepuisDernierIA,tempsEcouleDepuisDernierAffichage,tempsEcouleDepuisFPS,tempsEffetMort,tempsSauvergarde;
	float alpha_map,alpha_sac;

};

#endif


