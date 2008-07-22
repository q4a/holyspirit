
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
    ~c_Jeu(){/*delete camera;*/}
    void Utiliser(Jeu *jeu);

   // sf::View *camera;

    private:

	bool continuer,lumiere,augmenter;
	char chaine[10];
	sf::String variableQuiNeSerARien,Version,Temps,fps;

	float tempsActuel,tempsPrecedent,tempsDepuisDerniereAnimation,tempsEcoule,tempsEcouleDepuisDernierCalculLumiere,tempsEcouleDepuisDernierDeplacement,tempsEcouleDepuisDernierIA,tempsEcouleDepuisDernierAffichage,tempsEcouleDepuisFPS,tempsEffetMort;

};

#endif

