///**************************************************\\\
||| Dernière modification : Le 06/02/08 par Gregouar |||
///**************************************************///



#ifndef CONSOLEH
#define CONSOLEH

#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>


class Console
{
	public:
	void Afficher(sf::RenderWindow* ecran);
	void Ajouter(std::string texte,bool erreur=0);
	void AjouterNombre(int nombre,bool erreur=0);
    int getTaille();
    void defiler(bool direction);
    void Rapport();


	private:
	std::vector <std::string> m_textes;
	std::vector <bool> m_erreur;
	int m_defilement;
};


#endif




