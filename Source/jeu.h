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
#include "c_changementMap.h"
#include "c_inventaire.h"
#include "c_menuInGame.h"
#include "c_demarrage.h"

class Jeu
{
    public:

    Jeu();
    void Demarrer();
    void Reset();

    sf::RenderWindow ecran;
    sf::View camera;
    Menu menu;
    EventManager eventManager;
    Hero hero;
    Map map;
    sf::Clock Clock;
    sf::SoundBuffer bufferSonMort;
	sf::Sound  sonMort;

	bool m_run,m_display,m_reset;

	c_Demarrage *m_demarrage;
    c_Jeu *m_jeu;
    c_Chargement *m_chargement;
    c_Inventaire *m_inventaire;
    c_MenuInGame *m_menuInGame;
    Contexte *m_contexte;

    private:

};

#endif



