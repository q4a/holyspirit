
#include "c_jeu.h"
#include "jeu.h"

#include "Globale.h"

#include <iostream>
#include <fstream>

#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

using namespace sf;
using namespace std;

c_Demarrage::c_Demarrage(Jeu *jeu)
{
    m_alpha=0;
    m_augmenter=true;
    m_image=moteurGraphique.AjouterImage(configuration.chemin_aa);
    jeu->Clock.Reset();
}

void c_Demarrage::Utiliser(Jeu *jeu)
{
    temps_ecoule=jeu->Clock.GetElapsedTime();
    jeu->Clock.Reset();

    Sprite sprite;

    sprite.SetImage(*moteurGraphique.getImage(m_image));

    configuration.effetNoir=1;

    if(m_alpha<255)
        sprite.SetColor(Color(255,255,255,(int)m_alpha));
    else
        sprite.SetColor(Color(255,255,255,255));
    sprite.Resize(configuration.Resolution.w,configuration.Resolution.h);

    moteurGraphique.AjouterCommande(&sprite,0,0);

    if(m_augmenter)
        m_alpha+=temps_ecoule*200;
    else
        m_alpha-=temps_ecoule*200;

    if(m_alpha>384)
        m_augmenter=false;

    if(m_alpha<0)
        jeu->m_contexte=jeu->m_chargement;

    jeu->m_display=1;

}
