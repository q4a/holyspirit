#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

#ifndef COMMANDEH
#define COMMANDEH

extern Console *console;

struct Commande
{
	public:

	Commande(sf::Sprite *sprite,bool utiliserCamera,bool wall)
	{
	    m_sprite=*sprite;
	    m_utiliserCamera=utiliserCamera;
	    wall=wall;
    }

	sf::Sprite m_sprite;
	bool m_utiliserCamera;
	bool wall;
};

#endif


