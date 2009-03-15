

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




#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>

#include <dirent.h>
#include "ChargThread.h"
#include "main.h"


using namespace std;
//using namespace sf;

Console *console;
Configuration *configuration;
MoteurGraphique *moteurGraphique;
MoteurSons *moteurSons;


int main ( int argc, char** argv )
{
    std::set_terminate( __gnu_cxx::__verbose_terminate_handler);

    moteurGraphique=MoteurGraphique::GetInstance();

    configuration=Configuration::GetInstance();

    moteurSons=MoteurSons::GetInstance();
    console=Console::GetInstance();


    srand(time(NULL));

    console->Ajouter("---------------------------------------------------------------------------------");
    console->Ajouter("Demarrage du jeu",0);
    console->Ajouter("---------------------------------------------------------------------------------");
    console->Ajouter("");

    try
	{
	    ///Chargement de la configuration
        configuration->Charger();
        ChargThread chargThread(moteurGraphique);
        chargThread.charger();
        //moteurGraphique->Charger();
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
	console->Ajouter("Sauvegarde de la configuration");

    configuration->Sauvegarder();

    moteurGraphique->Kill();
    moteurSons->Kill();
    configuration->Kill();

    console->Ajouter("");
    console->Ajouter("EXIT_SUCCESS");

    console->Kill();

    return EXIT_SUCCESS;
}
