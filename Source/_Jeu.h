
#ifndef _JEUH
#define _JEUH

#include "Contexte.h"

class c_Jeu : public Contexte
{
    public:
    c_Jeu();

    sf::View camera;
	sf::View miniMap;

    private:

	bool continuer,lumiere,augmenter;
	char chaine[10];
	sf::String FPS,Version,Temps;
	sf::PostFX EffectBlur,EffectColorize;

	float tempsActuel,tempsPrecedent,tempsDepuisDerniereAnimation,tempsEcoule,tempsEcouleDepuisDernierDeplacement,tempsEcouleDepuisDernierAffichage,tempsEcouleDepuisFPS,tempsEffetMort;

};

#endif

