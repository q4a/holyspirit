

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
    map         = NULL;
    next_screen = 0;

    m_no_printscreen = false;

    m_jeu           = NULL;
    m_demarrage     = NULL;
    m_chargement    = NULL;
    m_inventaire    = NULL;
    m_quetes        = NULL;
    m_docs          = NULL;
    m_potales       = NULL;
    m_craft         = NULL;
    m_bless         = NULL;
    m_miracles      = NULL;
    m_menuInGame    = NULL;
    m_mainMenu      = NULL;

    m_listener.Listen(6666);
    m_selector.Add(m_listener);
    configuration->multi = false;
    m_host = NULL;
    m_thread_clientTCP = NULL;
    m_thread_clientUDP = NULL;
    m_thread_host = NULL;
}


void Jeu::Demarrer()
{
    m_jeu           = new c_Jeu;
    m_demarrage     = new c_Demarrage;
    m_chargement    = new c_Chargement;
    m_inventaire    = new c_Inventaire;
    m_quetes        = new c_Quetes;
    m_docs          = new c_Documents;
    m_potales       = new c_Potales;
    m_craft         = new c_Craft;
    m_bless         = new c_Bless;
    m_miracles      = new c_Miracles;
    m_menuInGame    = new c_MenuInGame;
    m_mainMenu      = new c_MainMenu;

    m_contexte = m_mainMenu;

    Clock.Reset();

    moteurSons->setVolumeMusique((int)configuration->music_volume);

    m_run = true;
    m_display = true;
    while (m_run)
    {
        GlobalMutex.Lock();

        if(map != NULL && map->m_loaded)
            eventManager->GererLesEvenements(&m_run,Clock.GetElapsedTime()*0.001,map->getDimensions());
        else
        {
            coordonnee buf(1,1,1,1);
            eventManager->GererLesEvenements(&m_run,Clock.GetElapsedTime()*0.001,buf);
        }

        if (!m_no_printscreen && eventManager->getEvenement(Key::P,EventKey))
        {
            moteurGraphique->Printscreen();
            eventManager->StopEvenement(Key::P,EventKey);
        }

        m_contexte->Utiliser(this);

        if(m_display)
            moteurGraphique->Afficher();

        moteurSons->Gerer(MusicClock.GetElapsedTime()*0.001);
        MusicClock.Reset();

        GlobalMutex.Unlock();

        if(m_contexte != m_mainMenu && configuration->multi)
            GererMultijoueur();
    }

    if(m_jeu->m_thread_sauvegarde)
        m_jeu->m_thread_sauvegarde->Wait(),delete m_jeu->m_thread_sauvegarde;

    m_contexte=NULL;

    delete map;


    delete m_demarrage;
    delete m_jeu;
    delete m_chargement;
    delete m_inventaire;
    delete m_quetes;
    delete m_docs;
    delete m_potales;
    delete m_craft;
    delete m_bless;
    delete m_miracles;
    delete m_menuInGame;
    delete m_mainMenu;

    console->Ajouter("");
    console->Ajouter("Fermeture des contextes effectuée avec succès.");
}




void Jeu::Next()
{
    if(next_screen == 2)
        m_contexte = m_inventaire;
    else if(next_screen == 3)
        m_contexte = m_jeu;
    else if(next_screen == 4)
        m_contexte = m_menuInGame, m_menuInGame->options = false;
    else if(next_screen == 5)
        m_contexte = m_miracles;
    else if(next_screen == 6)
        m_contexte = m_quetes;
    else if(next_screen == 7)
        m_contexte = m_potales;
    else if(next_screen == 8)
        m_contexte = m_chargement;
    else if(next_screen == 9)
        m_contexte = m_docs;
}

