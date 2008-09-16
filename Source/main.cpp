///**************************************************\\\
||| Dernière modification : Le 06/02/08 par Gregouar |||
///**************************************************///


#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>
#include <fstream>
#include <dirent.h>

#include "main.h"
#include "MoteurGraphique.h"

using namespace std;
//using namespace sf;

Console console;
Configuration configuration;
MoteurGraphique moteurGraphique;



int main ( int argc, char** argv )
{
    srand(time(NULL));

    console.Rapport();


    try
	{
	    ///Chargement de la configuration

        configuration.Charger();

        moteurGraphique.Charger();

        ///On démarre le jeu
		Jeu m_jeu;

		m_jeu.Demarrer();
	}
	catch (const string& str)
	{
	    console.Ajouter("FATAL ERROR : ",1);
		console.Ajouter(str,1);

		return EXIT_FAILURE;
	}



    //if(!Jeu(&ecran))
      //  return EXIT_FAILURE;

    configuration.Sauvegarder();

    console.Ajouter("");
    console.Ajouter("EXIT_SUCCESS");

    return EXIT_SUCCESS;
}
