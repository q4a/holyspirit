
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

c_Inventaire::c_Inventaire(Jeu *jeu)
{
}

void c_Inventaire::Utiliser(Jeu *jeu)
{
    jeu->eventManager.GererLesEvenements(&jeu->ecran,&camera,&jeu->m_run,0,jeu->map.getDimensions());

    if(jeu->eventManager.getEvenement(Key::I,"ET"))
            jeu->m_contexte=jeu->m_jeu,jeu->eventManager.StopEvenement(Key::I,"ET");

    jeu->ecran.SetView(jeu->ecran.GetDefaultView());

    jeu->menu.AfficherInventaire(&jeu->ecran);

    jeu->menu.Afficher(&jeu->ecran,3);
    jeu->menu.AfficherDynamique(&jeu->ecran,jeu->hero.m_personnage.getCaracteristique(),-1,jeu->hero.m_personnage.getCaracteristique());

    jeu->eventManager.AfficherCurseur(&jeu->ecran);

    jeu->m_display=true;
}

