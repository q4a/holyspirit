///**************************************************\\\
||| Dernière modification : Le 06/02/08 par Gregouar |||
///**************************************************///



#ifndef JEUH
#define JEUH


#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>

#include "constantes.h"
#include "map.h"
#include "eventManager.h"
#include "menu.h"
#include "personnage.h"

#include "c_Jeu.h"

class Jeu
{
    public:

    void Demarrer();

    sf::RenderWindow ecran;
    Menu menu;
    EventManager eventManager;
    Hero hero;
    Map map;
    sf::Clock Clock;
    sf::SoundBuffer bufferSonMort;
	sf::Sound  sonMort;

	bool m_run,m_display;

    c_Jeu *m_jeu;
    Contexte *m_contexte;
};



#endif

