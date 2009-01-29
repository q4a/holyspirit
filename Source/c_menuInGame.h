
#ifndef C_MENUINGAME
#define C_MENUINGAME

#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>

#include "Contexte.h"

class c_MenuInGame : public Contexte
{
    public:
    c_MenuInGame(Jeu *jeu);
    void Utiliser(Jeu *jeu);

    //sf::View *camera;

    private:
    float temps_ecoule,m_alpha;
    bool retour;

    sf::String texte;

};

#endif
