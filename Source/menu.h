///**************************************************\\\
||| Dernière modification : Le 06/02/08 par Gregouar |||
///**************************************************///



#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>


#ifndef MENUH
#define MENUH

#include "constantes.h"

class Menu
{
	public:
	Menu();
	void Afficher(sf::RenderWindow* ecran,int type);
	void AfficherDynamique(sf::RenderWindow* ecran,Caracteristique caracteristique);

	private:
	sf::Image m_imageMiniMap,m_fondMiniMap,m_imageHUD,m_imageBulleVie;
};

#endif
