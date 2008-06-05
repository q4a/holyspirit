#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>
#include <fstream>

#include "constantes.h"
#include "Globale.h"


using namespace sf;

void lireVideo(sf::RenderWindow *ecran,std::string Chemin,int nombreImage)
{
	Clock Clock;
	Image image;
	Sprite Sprite;
	std::string cheminFinal;
	char numero[4];
	int numeroImagePrecedente=-1;

	float tempsEcoule=0;


	while(tempsEcoule<5)
	{
		tempsEcoule += Clock.GetElapsedTime()*15;
		Clock.Reset();
	}


	tempsEcoule=0;

	bool continuer=true;
	while(continuer)
	{
	    /// Gestion des événements
		Event Event;
		while(ecran->GetEvent(Event))
		{
			switch(Event.Type)
			{
        	case Event::KeyReleased:
				continuer = false;
            	break;
            case Event::MouseButtonReleased:
                continuer = false;
                break;
			}
			if (Event.Type == Event::Closed)
			continuer = false;
		}
		tempsEcoule += Clock.GetElapsedTime()*15;
		Clock.Reset();

		/// Chargement de l'image à afficher

		if(numeroImagePrecedente<(int)tempsEcoule)
		{
			if((int)tempsEcoule<10)
			{
				sprintf(numero,"%ld",(int)tempsEcoule);
				cheminFinal=Chemin+"000"+numero+".jpg";
			}
			else if((int)tempsEcoule<100)
			{
				sprintf(numero,"%ld",(int)tempsEcoule);
				cheminFinal=Chemin+"00"+numero+".jpg";
			}
			else if((int)tempsEcoule<1000)
			{
				sprintf(numero,"%ld",(int)tempsEcoule);
				cheminFinal=Chemin+"0"+numero+".jpg";
			}
			else
			{
				sprintf(numero,"%ld",(int)tempsEcoule);
				cheminFinal=Chemin+numero+".jpg";
			}
			image.LoadFromFile(cheminFinal.c_str());
			Sprite.SetImage(image);
			numeroImagePrecedente=(int)tempsEcoule;

		}
		if((int)tempsEcoule>nombreImage-3)
		continuer=false;

		/// Affichage

		Sprite.Resize(configuration.Resolution.x, configuration.Resolution.y);

		ecran->Draw(Sprite);
		ecran->Display();
	}
}

