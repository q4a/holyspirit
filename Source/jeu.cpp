#include "constantes.h"
#include "Globale.h"
#include "jeu.h"
#include "datFile.h"

#include <dirent.h>

void lireVideo(sf::RenderWindow *ecran,std::string Chemin);


using namespace sf;


Jeu::Jeu()
{
    m_reset=false;
}

void Jeu::Demarrer()
{

    if(!configuration->mode_fenetre)
        ecran.Create(sf::VideoMode(configuration->Resolution.x, configuration->Resolution.y, 32),"HolySpirit : Act of Faith",sf::Style::Fullscreen);
    else
        ecran.Create(sf::VideoMode(configuration->Resolution.x, configuration->Resolution.y, 32),"HolySpirit : Act of Faith");
    if(configuration->syncronisation_verticale)
    {
        ecran.UseVerticalSync(true);
        ecran.SetFramerateLimit(60);
    }


    lireVideo(&ecran,"Data/Menus/Videos/Cinematique test 1-2");

    this->hero.Charger();

    {
        cDAT reader;
        if(reader.Read(configuration->chemin_saves+"hero.sav.hs"))
            for(int i=0;i<(int)reader.GetNumberFile();i++)
                if(reader.GetFileName(i)!=configuration->chemin_temps+"hero.sav.txt")
                    reader.ExportFile(i),this->hero.m_contenuSave.push_back(reader.GetFileName(i));

    }



    this->m_jeu = new c_Jeu(this);
    this->m_demarrage = new c_Demarrage(this);
    this->m_chargement = new c_Chargement(this);
    this->m_inventaire = new c_Inventaire(this);
    this->m_menuInGame = new c_MenuInGame(this);

    coordonnee temp={1,1,-1,-1};
    this->m_chargement->setC_Chargement(1,temp,1);
    this->m_contexte = this->m_demarrage;



    Clock.Reset();

    this->m_run = true;
    while (this->m_run)
	{
	    configuration->Resolution.x=this->ecran.GetWidth();
	    configuration->Resolution.y=this->ecran.GetHeight();

		this->m_contexte->Utiliser(this);
		if(this->m_display)
		{
		    moteurSons->Vider();
		    moteurGraphique->Afficher(&this->ecran,&this->camera,this->map.getDimensions());
            this->ecran.Display();
            moteurGraphique->Vider();
            ecran.Clear();
		}
	}

	this->map.Sauvegarder(&this->hero);

	if(m_reset)
        Reset();

    this->hero.Sauvegarder();

    Reset();

    delete this->m_demarrage;
    delete this->m_jeu;
    delete this->m_chargement;
    delete this->m_inventaire;
    delete this->m_menuInGame;

    console->Ajouter("");
    console->Ajouter("Fermeture des contextes effectuée avec succès.");
}


void Jeu::Reset()
{
    struct dirent *lecture;

	DIR *repertoire;
    repertoire = opendir(configuration->chemin_temps.c_str());
    while ((lecture = readdir(repertoire)))
    {
        std::string temp=configuration->chemin_temps+lecture->d_name;
        remove(temp.c_str());
    }
    closedir(repertoire);

    this->hero.m_contenuSave.clear();
    this->hero.m_contenuSave.push_back(configuration->chemin_temps+"hero.sav.txt");
}



