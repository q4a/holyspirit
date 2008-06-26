#include "constantes.h"
#include "Globale.h"
#include "jeu.h"


using namespace sf;

Jeu::Jeu()
{
    if (sf::PostFX::CanUsePostFX() == true&&configuration.postFX)
    {

        if(!EffectNoir.LoadFromFile(configuration.chemin_fx+configuration.nom_effetNoir))
            console.Ajouter("Impossible de charger : "+configuration.chemin_fx+configuration.nom_effetNoir,1);
        else
        {
            console.Ajouter("Chargement de : "+configuration.chemin_fx+configuration.nom_effetNoir,0);
            EffectNoir.SetTexture("framebuffer", NULL);
            EffectNoir.SetParameter("color", 0.f, 0.f, 0.f);
        }
    }
}

void Jeu::Demarrer()
{
    //this-> m_ecran = new sf::RenderWindow(sf::VideoMode(configuration.Resolution.x, configuration.Resolution.y, 32),"EDF casse brique");
    if(!configuration.mode_fenetre)
        ecran.Create(sf::VideoMode(configuration.Resolution.x, configuration.Resolution.y, 32),"HolySpirit : Act of Faith",sf::Style::Fullscreen);
    else
        ecran.Create(sf::VideoMode(configuration.Resolution.x, configuration.Resolution.y, 32),"HolySpirit : Act of Faith");
    if(configuration.syncronisation_verticale)
    {
        ecran.UseVerticalSync(true);
        ecran.SetFramerateLimit(60);
    }

    this->m_jeu = new c_Jeu(this);
    this->m_chargement = new c_Chargement(this);
    this->m_inventaire = new c_Inventaire(this);

    coordonnee temp={1,1,-1,-1};
    this->m_chargement->setC_Chargement(1,temp,1);

    this->m_contexte = this->m_chargement;

    this->m_run = true;
    while (this->m_run)
	{
		this->m_contexte->Utiliser(this);
		if(this->m_display)
		{
		    this->EffectNoir.SetParameter("color", (float)configuration.luminosite/50,(float)configuration.luminosite/50, (float)configuration.luminosite/50);
            this->ecran.Draw(this->EffectNoir);
            this->ecran.Display();
		}
	}

    delete this->m_jeu;
    delete this->m_chargement;
    delete this->m_inventaire;

	console.Rapport();
}





