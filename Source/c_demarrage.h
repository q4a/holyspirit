
#ifndef C_DEMARRAGE
#define C_DEMARRAGE

#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>

#include "Contexte.h"

class c_Demarrage : public Contexte
{
    public:
    c_Demarrage(Jeu *jeu);
    void Utiliser(Jeu *jeu);

    //sf::View *camera;

    private:
    float temps_ecoule,m_alpha;
    int m_image;
    bool m_augmenter;
};

#endif

