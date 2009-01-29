

#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>


#ifndef MENUH
#define MENUH

#include "constantes.h"

class Ame
{
    public:
    Ame (coordonneeDecimal position,int pointAme)
    {
        m_position=position;
        m_depart=position;
        m_taille=0;
        m_mode=-1;
        m_rotation=0;
        m_pointAme=pointAme;
        m_alpha=0;
        augmenter=true;
    }
    coordonneeDecimal m_position,m_depart;
    float m_taille,m_rotation,m_alpha;
    int m_mode,m_pointAme;
    bool augmenter;
};

class Sang
{
    public:
    Sang (coordonneeDecimal position)
    {
        m_position=position;
        m_taille=(float)(rand()%(100-25) + 25)/100;
        m_rotation=(float)(rand()%360);
        m_alpha=255;
        m_numero=(rand()%(NOMBRE_SANG));
    }
    coordonneeDecimal m_position;
    float m_taille,m_rotation,m_alpha;
    int m_numero;
};

class Menu
{
	public:
	Menu();
	~Menu();
	void Afficher(sf::RenderWindow* ecran,int type,float alpha=255);
	void AfficherDynamique(sf::RenderWindow* ecran,Caracteristique caracteristique,int type,Caracteristique caracteristiqueMonstre);
	void AfficherChargement(sf::RenderWindow* ecran,std::string nom,int fond,int z);
	void AfficherInventaire(sf::RenderWindow* ecran,float);

	void AjouterSang(coordonneeDecimal position);

	void AjouterAme(coordonneeDecimal position,int pointAme);
	int GererDynamique(float temps);


	private:
	int m_fondMiniMap,m_imageHUD,m_imageBulleVie,m_imageBulleFoi,m_imageAme,m_imageSang,m_barrePointAme,m_barreVie,m_barreVieVide,m_inventaire;

	sf::String texte;

	float m_alphaSang;

	int m_imageMiniMap,;

	std::vector <Sang> m_sang;
	std::vector <Sang>::iterator IterSang;

	std::vector <Ame> m_ame;
	std::vector<Ame>::iterator Iter;
};

#endif

