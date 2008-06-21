
#ifndef _JEUH
#define _JEUH

#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>

#include "Contexte.h"

class c_Jeu : public Contexte
{
    public:
    c_Jeu(Jeu *jeu);
    void Utiliser(Jeu *jeu);

    sf::View camera;

    private:

	bool continuer,lumiere,augmenter;
	char chaine[10];
	sf::String variableQuiNeSerARien,Version,Temps,fps;
	sf::PostFX EffectBlur,EffectColorize;

	float tempsActuel,tempsPrecedent,tempsDepuisDerniereAnimation,tempsEcoule,tempsEcouleDepuisDernierDeplacement,tempsEcouleDepuisDernierAffichage,tempsEcouleDepuisFPS,tempsEffetMort;

};

#endif

