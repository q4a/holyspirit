

/*Copyright (C) 2009 Naisse Grégoire

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License along
with this program; if not, write to the Free Software Foundation, Inc.,
51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.*/


#include "constantes.h"
#include "globale.h"
#include "jeu.h"
#include "datFile.h"

#include <dirent.h>

void lireVideo(sf::RenderWindow *ecran,std::string Chemin);


using namespace sf;


Jeu::Jeu()
{
    m_reset=false;
    map=NULL;
}

void Jeu::Demarrer()
{
    //map=new Map();

    //lireVideo("Data/Menus/Videos/Cinematique test 1-2");

    this->hero.Charger();

    {
        cDAT reader;
        if (reader.Read(configuration->chemin_saves+"hero.sav.hs"))
            for (int i=0;i<(int)reader.GetNumberFile();i++)
                if (reader.GetFileName(i)!=configuration->chemin_temps+"hero.sav.txt")
                    reader.ExportFile(i),this->hero.m_contenuSave.push_back(reader.GetFileName(i));
    }

    this->hero.ChargerModele();

    this->m_jeu = new c_Jeu();
    this->m_demarrage = new c_Demarrage();
    this->m_chargement = new c_Chargement();
    this->m_inventaire = new c_Inventaire();
    this->m_menuInGame = new c_MenuInGame();

    coordonnee temp={0,0,-1,-1};
    this->m_chargement->setC_Chargement("Begin.map.hs",temp,1);
    this->m_contexte = this->m_demarrage;

    Clock.Reset();

    this->m_run = true;
    this->m_display = true;
    while (this->m_run)
    {
        if(map!=NULL)
            eventManager->GererLesEvenements(&m_run,Clock.GetElapsedTime(),map->getDimensions());
        else
        {
            coordonnee buf={1,1,1,1};
            eventManager->GererLesEvenements(&m_run,Clock.GetElapsedTime(),buf);
        }

        if (eventManager->getEvenement(Key::F1,"ET"))
        {
            moteurGraphique->Printscreen();
            eventManager->StopEvenement(Key::F1,"ET");
        }

        this->m_contexte->Utiliser(this);
        if (this->m_display)
            moteurGraphique->Afficher();
    }

    if (m_reset)
        Reset();
    else
        map->Sauvegarder(&this->hero);

    hero.Sauvegarder();

    Reset();

    this->m_contexte=NULL;

    delete map;

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



