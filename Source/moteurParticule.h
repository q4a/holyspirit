

#ifndef MOTEURPARTICULES
#define MOTEURPARTICULES

#include "constantes.h"
#include "modelemoteurparticule.h"

struct Particule
{
    coordonnee3D position;
    coordonnee3D vecteur;
    float vitesse;
    float vitesse_rotation;
    float rotation;
    float alpha;
    sf::Color color;

    int numero;
};

class ParticuleSysteme
{
    public:
    ParticuleSysteme();
    ParticuleSysteme(int modele);
    ParticuleSysteme(int , ModeleParticuleSysteme *, coordonnee , sf::Color,float );
    ~ParticuleSysteme();
    void Afficher(sf::RenderWindow *ecran, ModeleParticuleSysteme *modele);
    void Generer(float force, ModeleParticuleSysteme *modele,coordonnee position);
    bool Gerer(float temps);

    int m_modele;
    std::vector <Particule> m_particules;

    private:
    sf::Color m_color;
};

#endif

