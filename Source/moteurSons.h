#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

#include "constantes.h"

#ifndef MOTEURSONSH
#define MOTEURSONSH


class MoteurSons
{
	public:
	MoteurSons();
	~MoteurSons();

	void Gerer();
	void Vider();
	int AjouterBuffer(std::string);
	//int AjouterSon(int ID);
	void JouerSon(int ID,coordonnee position,coordonnee positionHero,bool unique=0);

	//sf::Image* getImage(int IDimage);
	std::string getCheminSon(int IDimage);
	private:
	int sonEnCours;
	sf::Sound m_sons[NOMBRE_SONS];
	int m_IDSons[NOMBRE_SONS];
	//std::vector <bool> m_sonAZero;
	//std::vector <int> m_sonIDBuffer;
	std::vector <sf::SoundBuffer> m_buffers;
	std::vector <std::string> m_cheminsSons;//Permet de s'assurer d'avoir des singletons
};

#endif




