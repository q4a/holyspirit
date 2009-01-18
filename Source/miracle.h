
#include "constantes.h"
#include "tile.h"

#ifndef MIRACLEH
#define MIRACLEH

enum  {PROJECTILE,CORPS_A_CORPS,DEGATS,EFFET_GRAPHIQUE};

class Projectile
{
    public:
    void Afficher(sf::RenderWindow* ecran,sf::View *camera,coordonnee position,coordonnee dimensionsMap);
    void Deplacer(float temp);

    bool m_monstre,m_actif;
    int m_degats;
    coordonneeDecimal m_position,m_vecteur;
    float m_rotation;

    int m_image;
    coordonnee m_positionImage;

    Lumiere m_lumiere;

    float m_animation;
    int m_couche;

    bool m_dejaDeplace;
};

class EffetGraphique
{
    public:
    void Afficher(sf::RenderWindow* ecran,sf::View *camera,coordonnee position,coordonnee dimensionsMap);

    bool m_actif;
    coordonneeDecimal m_position;

    int m_image;
    int m_compteur;
    coordonnee m_positionImage;

    Lumiere m_lumiere;

    float m_animation;
    int m_couche;
};

struct Effet
{
    Effet()
    {
        m_lien=-1;
        m_type=0;
        m_sequence=0;

        m_informations[0];
        m_informations[0];
        m_informations[0];
    }

    int m_lien;
    int m_type;
    int m_sequence;

    int m_informations[3];
};

class Miracle
{
    public:
    Miracle();
    Miracle(std::string chemin);
    ~Miracle();
    void Charger(std::string chemin);
    void jouerSon(int numeroSon,coordonnee position,coordonnee positionHero);

    std::vector < std::vector <Tile> > m_tile;
    std::vector <Effet> m_effets;


    std::vector <int> m_image;
	std::vector <int> m_sons;
	std::string m_chemin;

    int m_coutFoi;
};

class EntiteMiracle
{
    public:
    EntiteMiracle()
    {
        m_effetEnCours=0;
        m_imageEnCours=0;
        m_modele=0;
    }

    int m_effetEnCours;
    int m_imageEnCours;
    int m_modele;
    int m_IDObjet;

    coordonneeDecimal m_position;
};

#endif
