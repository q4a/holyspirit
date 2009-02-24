

#ifndef CONSOLEH
#define CONSOLEH

#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "Singleton.h"

class Console : public CSingleton<Console>
{
	public:

	friend Console* CSingleton<Console>::GetInstance();
    friend void CSingleton<Console>::Kill();

	void Afficher(sf::RenderWindow* ecran);
	void Ajouter(std::string texte,bool erreur=0);
	void Ajouter(int texte,bool erreur=0);
    int getTaille();
    void defiler(bool direction);


	protected:

	Console(){m_defilement=0;}
	~Console();

	void Rapport();

	std::vector <std::string> m_textes;
	std::vector <bool> m_erreur;
	int m_defilement;
};


#endif





