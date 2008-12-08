#include "constantes.h"
#include "Globale.h"
#include "jeu.h"
#include <dirent.h>


using namespace sf;


Jeu::Jeu()
{
    m_reset=false;
    if (sf::PostFX::CanUsePostFX() == true&&configuration.postFX)
    {

        if(!EffectContrastes.LoadFromFile(configuration.chemin_fx+configuration.nom_effetContrastes))
            console.Ajouter("Impossible de charger : "+configuration.chemin_fx+configuration.nom_effetContrastes,1);
        else
        {
            console.Ajouter("Chargement de : "+configuration.chemin_fx+configuration.nom_effetContrastes,0);
            EffectContrastes.SetTexture("framebuffer", NULL);
            EffectContrastes.SetParameter("color", 0.f, 0.f, 0.f);
        }

        if(!EffectNoir.LoadFromFile(configuration.chemin_fx+configuration.nom_effetNoir))
            console.Ajouter("Impossible de charger : "+configuration.chemin_fx+configuration.nom_effetNoir,1);
        else
        {
            console.Ajouter("Chargement de : "+configuration.chemin_fx+configuration.nom_effetNoir,0);
            EffectNoir.SetTexture("framebuffer", NULL);
            EffectNoir.SetParameter("color", 0.f, 0.f, 0.f);
        }
        configuration.effetMort=0;
    }
    Luminosite.Create(configuration.Resolution.x, configuration.Resolution.y, sf::Color(255,255,255));
}

void Jeu::Demarrer()
{
    sf::Sprite Sprite;
    Sprite.SetImage(Luminosite);
    Sprite.SetBlendMode(sf::Blend::Add);

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
    this->m_demarrage = new c_Demarrage(this);
    this->m_chargement = new c_Chargement(this);
    this->m_inventaire = new c_Inventaire(this);
    this->m_menuInGame = new c_MenuInGame(this);

    coordonnee temp={1,1,-1,-1};
    this->m_chargement->setC_Chargement(1,temp,1);
    this->m_contexte = this->m_demarrage;

    this->hero.Charger();

    this->m_run = true;
    while (this->m_run)
	{
	    configuration.Resolution.x=this->ecran.GetWidth();
	    configuration.Resolution.y=this->ecran.GetHeight();

		this->m_contexte->Utiliser(this);
		if(this->m_display)
		{
		    moteurGraphique.Afficher(&this->ecran,&this->camera);
            this->ecran.Draw(this->EffectNoir);

            if(configuration.contrastes>1&&sf::PostFX::CanUsePostFX() == true&&configuration.postFX)
            {
                this->ecran.Draw(this->EffectContrastes);
                EffectContrastes.SetParameter("color", configuration.contrastes, configuration.contrastes, configuration.contrastes);
            }
            if(configuration.luminosite>0)
            {
                Sprite.SetColor(sf::Color((int)configuration.luminosite,(int)configuration.luminosite,(int)configuration.luminosite,255));
                this->ecran.Draw(Sprite);
            }
            this->ecran.Display();
            moteurGraphique.Vider();
		}
	}
	this->hero.Sauvegarder();
	this->map.Sauvegarder();

	if(m_reset)
        Reset();

    delete this->m_demarrage;
    delete this->m_jeu;
    delete this->m_chargement;
    delete this->m_inventaire;
    delete this->m_menuInGame;

    console.Ajouter("");
    console.Ajouter("Fermeture des contextes effectuée avec succès.");
}


void Jeu::Reset()
{
    struct dirent *lecture;

	DIR *repertoire;
    repertoire = opendir(configuration.chemin_temps.c_str());
    while ((lecture = readdir(repertoire)))
    {
        std::string temp=configuration.chemin_temps+lecture->d_name;
        remove(temp.c_str());
    }
    closedir(repertoire);
}



