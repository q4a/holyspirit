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

    ///Chargement de la configuration

    configuration.Charger();

    ///On démarre le jeu

    console.Rapport();

    try
	{
		Jeu m_jeu;

		m_jeu.Demarrer();
	}
	catch (char *str)
	{
	    console.Ajouter("FATAL ERROR : ",1);
		console.Ajouter(str,1);
		console.Rapport();

		return EXIT_FAILURE;
	}



    //if(!Jeu(&ecran))
      //  return EXIT_FAILURE;

    console.Ajouter("");
    console.Ajouter("EXIT_SUCCESS");
    console.Rapport();

    return EXIT_SUCCESS;
}
