#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

#ifndef COMMANDEH
#define COMMANDEH

extern Console console;

class Commande
{
	public:

	Commande(sf::Sprite *sprite,bool utiliserCamera)
	{
	    m_sprite=*sprite;
	    m_utiliserCamera=utiliserCamera;
    }

	sf::Sprite m_sprite;
	bool m_utiliserCamera;
};

#endif


