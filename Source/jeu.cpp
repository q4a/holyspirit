#include "constantes.h"
#include "Globale.h"
#include "jeu.h"


using namespace sf;

Jeu::Jeu()
{
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

        if(!EffectMort.LoadFromFile(configuration.chemin_fx+configuration.nom_effetMort))
            console.Ajouter("Impossible de charger : "+configuration.chemin_fx+configuration.nom_effetMort,1);
        else
            console.Ajouter("Chargement de : "+configuration.chemin_fx+configuration.nom_effetMort,0);
        configuration.effetMort=0;

        EffectMort.SetParameter("offset", 0);
        EffectMort.SetTexture("framebuffer", NULL);
        EffectMort.SetParameter("color",1, 1, 1);
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
    this->m_chargement = new c_Chargement(this);
    this->m_inventaire = new c_Inventaire(this);

    coordonnee temp={1,1,-1,-1};
//    this->m_contexte->CopierCamera(this->m_chargement->camera);
    //this->m_contexte->camera=this->m_chargement->camera;
    this->m_chargement->setC_Chargement(1,temp,1);

    this->m_contexte = this->m_chargement;
//:    m_chargement->CopierCamera(this->m_jeu->camera);

    this->hero.Charger();

    this->m_run = true;
    while (this->m_run)
	{
		this->m_contexte->Utiliser(this);
		if(this->m_display)
		{
		    Sprite.SetColor(sf::Color((int)configuration.luminosite,(int)configuration.luminosite,(int)configuration.luminosite,255));
		    EffectContrastes.SetParameter("color", configuration.contrastes, configuration.contrastes, configuration.contrastes);

		    moteurGraphique.Afficher(&this->ecran,&this->camera);
            this->ecran.Draw(this->EffectNoir);
            if(configuration.effetMort>0)
                this->ecran.Draw(EffectMort);
            if(configuration.contrastes>1&&sf::PostFX::CanUsePostFX() == true&&configuration.postFX)
                this->ecran.Draw(this->EffectContrastes);
            if(configuration.luminosite>0)
                this->ecran.Draw(Sprite);
            this->ecran.Display();
            moteurGraphique.Vider();
		}
	}
	console.Rapport();
	this->hero.Sauvegarder();
	this->map.Sauvegarder();

    delete this->m_jeu;
    delete this->m_chargement;
    delete this->m_inventaire;

    console.Ajouter("");
    console.Ajouter("Fermeture des contextes effectuée avec succès.");
	console.Rapport();
}






