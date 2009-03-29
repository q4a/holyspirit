

/*Copyright (C) 2009 Naisse Grégoire

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License along
with this program; if not, write to the Free Software Foundation, Inc.,
51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.*/


#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

#ifndef MOTEURGRAPHIQUEH
#define MOTEURGRAPHIQUEH

#include "map.h"
#include "commande.h"
#include "moteurParticule.h"
#include "LightManager.h"

struct Image_moteur
{
    sf::Image img;
    std::string nom;
    int importance;
};

class MoteurGraphique : public CSingleton<MoteurGraphique>
{
	public:

	friend MoteurGraphique* CSingleton<MoteurGraphique>::GetInstance();
    friend void CSingleton<MoteurGraphique>::Kill();

	void Afficher(sf::RenderWindow *, sf::View *,coordonnee );

	void Gerer(sf::RenderWindow *,float,int);

	int AjouterImage(std::string,int importance = 3);
	int AjouterImage(const char *Data, std::size_t SizeInBytes, std::string nom,int importance =5);
	int AjouterModeleSystemeParticules(std::string);

	void AjouterSystemeParticules(int ID,coordonnee position,sf::Color color,float force,float angle);
	void AjouterCommande(sf::Sprite*,int=0, bool=0);
	void AjouterTexte(sf::String*, int couche=0,bool titre=false);

	void DecrementerImportance();

	void Charger();

	void Vider();
	void ViderParticules();

	sf::Image* getImage(int IDimage);
	ModeleParticuleSysteme* getModeleMoteurParticules(int ID);

	std::string getCheminImage(int IDimage);

	sf::PostFX EffectBlur,EffectMort,EffectNoir,EffectContrastes;

	sf::Font m_font_titre;

	float m_blur;

	Light_Manager *LightManager;
    Lumiere m_soleil;
    float m_angleOmbreSoleil;

    sf::Image m_light_screen;
    sf::Image m_light_screen2;
    sf::Image m_light_screen3;

	protected:

	MoteurGraphique();
	~MoteurGraphique();

	std::vector <Image_moteur> m_images;

	std::vector <ModeleParticuleSysteme> m_modeleSystemeParticules;

	std::vector <sf::String> m_textes[21];
	std::vector <Commande> m_commandes[21];
	std::vector <ParticuleSysteme> m_systemeParticules;
	std::vector <ParticuleSysteme>::iterator m_systemeParticules_iter;

	std::vector <Commande>::iterator IterCommande;
};

#endif



