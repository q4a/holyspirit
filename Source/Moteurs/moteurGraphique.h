

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
#include <list>

#ifndef MOTEURGRAPHIQUEH
#define MOTEURGRAPHIQUEH

#include "commande.h"
#include "moteurParticule.h"
#include "lightManager.h"
#include "Entite_graphique.h"
#include "../Map/tileset.h"

struct Image_moteur
{
    Image_moteur()
    {
        importance  = 0;
        nom         = "";
        img         =  NULL;
    }
    sf::Image *img;
    std::string nom;
    int importance;
};

struct Tileset_moteur
{
    Tileset_moteur()
    {
        importance  = 0;
        nom         = "";
        tileset     =  NULL;
    }
    Tileset *tileset;
    std::string nom;
    int importance;
};

enum  {TREMBLEMENT,ENVOLEMENT};
enum  {E_SOUFFLE, E_TORNADE, E_VERTICAL};

struct Effet_ecran
{
    Effet_ecran()
    {
        type    = -1;
        info1   = 0;
        info2   = 0;
        temps   = 0;
    }

    int type;
    float info1;
    float info2;
    float temps;
    sf::Vector2f pos;
};

class MoteurGraphique : public CSingleton<MoteurGraphique>
{
	public:

	friend MoteurGraphique* CSingleton<MoteurGraphique>::GetInstance();
    friend void CSingleton<MoteurGraphique>::Kill();

	void Afficher();

	void CreateNewWindow();

	void Gerer(float,int);

	int AjouterImage(std::string,int importance = 5);
	int AjouterImage(const sf::Image &,int importance = 5);
	int AjouterImage(const char *Data, std::size_t SizeInBytes, std::string nom,int importance =5);
	int AjouterTileset(std::string,int importance = 5);
	int AjouterTileset(std::ifstream &fichier, std::string nom = "",int importance = 5);
	int AjouterModeleSystemeParticules(std::string);

	void AjouterSystemeParticules(int ID,coordonnee position,sf::Color color,float force,float angle);
	void AjouterCommande(sf::Sprite*,int=0, bool=0);
	void AjouterEntiteGraphique(Entite_graphique*);
	void AjouterTexte(sf::Text*, int couche=0,bool titre=false);
	void AjouterTexteNonChevauchable(sf::Text*, int couche=0,bool titre=false);
	void AjouterTexte(const std::string &, coordonnee, int couche=0, bool titre=false, int size = 14, sf::Color color = sf::Color(224,224,224), bool fond = false);

	Entite_graphique getEntiteGraphique(int noTileset, int noTile, int couche);

	void DecrementerImportance();

	void Charger();

	void Vider();
	void ViderParticules();

	sf::Vector2f getPositionSouris();
	bool getEvent(sf::Event &EventReceived);

	void Printscreen();
	int GetFPS();

	sf::Image*  getImage(int);
	Tileset*    getTileset(int );

	ModeleParticuleSysteme* getModeleMoteurParticules(int ID);

	std::string getCheminImage(int IDimage);

	sf::Shader  EffectBlur,EffectBlur2,EffectBlurScreen,EffectMort,EffectNoir,EffectContrastes,EffectFiltre, EffectWater, EffectDistortion;
	sf::RenderImage bufferImage;
	sf::RenderImage m_water_screen;
	sf::RenderImage m_distortion_screen;
	sf::RenderImage m_light_screen;
    sf::RenderImage m_light_screen2;

	sf::Font m_font,m_font_titre;

	float m_blur;

    sf::View m_camera;

	Light_Manager *LightManager;
    Lumiere m_soleil;
    float m_angleOmbreSoleil;

    int m_img_corner, m_img_water, m_img_item_background, m_img_item_unusable;

  //  sf::RenderImage m_light_screen3;

    std::vector <Effet_ecran> m_effetsEcran;
    std::vector <Effet_ecran>::iterator m_effetsEcran_iter;

    coordonneeDecimal decalageCamera, decalageCameraSouhaite;
    bool cameraDecale;
    coordonneeDecimal decalageReflection, m_transWater;

	//protected:

	MoteurGraphique();
	~MoteurGraphique();

	sf::Vector2f decalageLumiere,decalageOmbre;

	std::vector <Image_moteur>      m_images;
	std::vector <Tileset_moteur>    m_tileset;

	std::vector <ModeleParticuleSysteme> m_modeleSystemeParticules;

	std::list <sf::Text> m_textes[21];
	std::list <Commande> m_commandes[21];
	std::list <Commande> m_distortion_commandes;
	std::vector <ParticuleSysteme> m_systemeParticules;
	std::vector <ParticuleSysteme>::iterator m_systemeParticules_iter;

	std::list <Commande>::iterator IterCommande;
	std::list <sf::Text>::iterator IterTextes;

	sf::RenderWindow m_ecran;


};

#endif



