

#ifndef CONSOLEH
#define CONSOLEH

#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>


class Console
{
	public:
	~Console();
	void Afficher(sf::RenderWindow* ecran);
	void Ajouter(std::string texte,bool erreur=0);
	void Ajouter(int texte,bool erreur=0);
    int getTaille();
    void defiler(bool direction);


	private:

	void Rapport();

	std::vector <std::string> m_textes;
	std::vector <bool> m_erreur;
	int m_defilement;
};


#endif





