

#ifndef C_INVENTAIRE
#define C_INVENTAIRE

#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>

#include "Contexte.h"

class c_Inventaire : public Contexte
{
    public:
    c_Inventaire(Jeu *jeu);
    void Utiliser(Jeu *jeu);

    private:

    sf::View camera;

	float tempsActuel,tempsPrecedent,temps_ecoule,tempsEcouleDepuisDernierAffichage;
};

#endif

