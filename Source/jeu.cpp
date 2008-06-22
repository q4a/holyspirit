#include "constantes.h"
#include "Globale.h"
#include "jeu.h"


using namespace sf;


void Jeu::Demarrer()
{
    //this-> m_ecran = new sf::RenderWindow(sf::VideoMode(configuration.Resolution.x, configuration.Resolution.y, 32),"EDF casse brique");
    if(configuration.plein_ecran)
        ecran.Create(sf::VideoMode(configuration.Resolution.x, configuration.Resolution.y, 32),"HolySpirit : Act of Faith",sf::Style::Fullscreen);
    else
        ecran.Create(sf::VideoMode(configuration.Resolution.x, configuration.Resolution.y, 32),"HolySpirit : Act of Faith");
    ecran.UseVerticalSync(configuration.syncronisation_verticale);
    //ecran.PreserveOpenGLStates(true);
    //ecran.OptimizeForNonOpenGL(true);

    this->m_jeu = new c_Jeu(this);

    this->m_contexte = this->m_jeu;

    this->m_run = true;
    while (this->m_run)
	{
		this->m_contexte->Utiliser(this);
		if(this->m_display)
            this->ecran.Display();
	}

    delete this->m_jeu;

	console.Rapport();
}





