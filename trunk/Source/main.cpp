

/*Copyright (C) 2009 Naisse Grégoire

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2 of the License, or
(at your option) any later version.ce u

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License along
with this program; if not, write to the Free Software Foundation, Inc.,
51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.*/

#include <iostream>
#include <sstream>

#include <dirent.h>
#include "main.h"


using namespace std;

Console *console;
Configuration *configuration;
MoteurGraphique *moteurGraphique;
MoteurSons *moteurSons;
EventManager *eventManager;
Network *net;

int main (  )
{
    srand(time(NULL));
 //   std::set_terminate( __gnu_cxx::__verbose_terminate_handler)

    configuration=Configuration::GetInstance();
    console=Console::GetInstance();

    console->Ajouter("--------------------------------------------------------------------------------");
    console->Ajouter("Starting the game",0);
    console->Ajouter("--------------------------------------------------------------------------------");
    console->Ajouter("");

    console->Ajouter("Initializing graphics engine");
    moteurGraphique = MoteurGraphique::GetInstance();

    console->Ajouter("Initializing sound engine");
    moteurSons = MoteurSons::GetInstance();

    console->Ajouter("Initializing network engine");
    net = Network::GetInstance();

    console->Ajouter("");

    try
    {
        ///Chargement de la configuration
        configuration->Charger();

        eventManager=EventManager::GetInstance();

       if (sf::Shader::isAvailable() == false)
            configuration->postFX = false;

        moteurGraphique->Charger();
        moteurGraphique->CreateNewWindow();

        configuration->numero_screen=0;
        ///On démarre le jeu
        Jeu m_jeu;

        m_jeu.Demarrer();

    }
    catch (const std::string& str)
    {
        console->Ajouter("FATAL ERROR : ",1);
        console->Ajouter(str,1);

        return EXIT_FAILURE;
    }


    console->Ajouter("");
    console->Ajouter("Saving configuration");

    configuration->Sauvegarder();

    console->Ajouter("");
    console->Ajouter("Destruction of engines");

    net->Kill();
    moteurGraphique->Kill();
    moteurSons->Kill();
    configuration->Kill();
    eventManager->Kill();

    console->Ajouter("");
    console->Ajouter("EXIT_SUCCESS");

    console->Kill();

    cout<<"Closing console"<<endl;

    return EXIT_SUCCESS;
}

