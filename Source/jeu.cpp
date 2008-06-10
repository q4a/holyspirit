#include "jeu.h"
#include "Globale.h"

using namespace sf;



bool Jeu(RenderWindow *ecran)
{
	Clock Clock;
	bool continuer=true,lumiere=false,augmenter=true;
	char chaine[10];
	View camera(FloatRect(0, 0, configuration.Resolution.x,configuration.Resolution.y), 1.f);
	View miniMap(FloatRect(0, 0, configuration.Resolution.x, configuration.Resolution.y), 0.125);
	float tempsActuel=0,tempsPrecedent=0,tempsDepuisDerniereAnimation=0,tempsEcoule=0,tempsEcouleDepuisDernierDeplacement=0,tempsEcouleDepuisDernierAffichage=0,tempsEcouleDepuisFPS=0,tempsEffetMort=0;

	configuration.heure=(rand() % (24));
	configuration.minute=0;

	sf::Listener::SetGlobalVolume((float)configuration.volume);

	sf::String FPS;
    FPS.SetText("FPS"); // Texte pour l'affichage des FPS
    FPS.SetSize(16.f);

    sf::String Version;
    Version.SetText("Version");
    Version.SetSize(16.f);

    sprintf(chaine,"v %s",configuration.version.c_str());
    Version.SetText(chaine);
    Version.SetTop(20);

    sf::String Temps;
    Temps.SetText("Temps : ");
    Temps.SetSize(16.f);

    sprintf(chaine,"Temps : %ld h %ld ",configuration.heure,(int)configuration.minute);
    Temps.SetText(chaine);
    Temps.SetTop(40);

    Menu menu; // Chargement des menus
    EventManager eventManager;

    console.Ajouter("",0);
    console.Ajouter("Chargement des effets :",0);

     sf::PostFX EffectBlur,EffectColorize;

    if (sf::PostFX::CanUsePostFX() == true)
    {
        if(!EffectBlur.LoadFromFile(configuration.chemin_fx+configuration.nom_effetBlur))
                    console.Ajouter("Impossible de charger : "+configuration.chemin_fx+configuration.nom_effetBlur,1);
        else
            console.Ajouter("Chargement de : "+configuration.chemin_fx+configuration.nom_effetBlur,0);
        if(!EffectColorize.LoadFromFile(configuration.chemin_fx+configuration.nom_effetColorize))
                    console.Ajouter("Impossible de charger : "+configuration.chemin_fx+configuration.nom_effetColorize,1);
        else
            console.Ajouter("Chargement de : "+configuration.chemin_fx+configuration.nom_effetColorize,0);

        configuration.effetMort=0;

        EffectBlur.SetParameter("offset", 0);

        EffectColorize.SetTexture("framebuffer", NULL);
        EffectColorize.SetParameter("color",1, 1, 1);
    }

	Hero hero;
	if(!hero.m_modelePersonnage.Charger("Data/Personnages/Hero/GuerrierHache.txt")) // Chargement du héro
        return 0;


    camera.Rect.Left=0;
    camera.Rect.Top=0;

	Map map;
	if(!map.Charger(0)==1) // Chargement de  map0.txt
        return 0;

	ecran->ShowMouseCursor(false);
	//if(configuration.Lumiere)
	//map.calculerOmbresEtLumieres(ecran,&hero,&configuration);

	if(configuration.Lumiere)
    {
        map.detruireOmbresEtLumieres(&hero);
        map.calculerOmbresEtLumieres(ecran,&hero,&camera);
        lumiere=false;
    }

    Clock.Reset();

	while(continuer)
	{
            //Gestion du temps
            tempsEcoule = Clock.GetElapsedTime();
		    tempsEcouleDepuisDernierDeplacement+=tempsEcoule;
		    tempsDepuisDerniereAnimation+=tempsEcoule;
		    tempsEcouleDepuisDernierAffichage+=tempsEcoule;
		    configuration.minute+=tempsEcoule;
		    if(configuration.minute>=60)
		    configuration.minute=0,configuration.heure++;
		    if(configuration.heure>23)
            configuration.heure=0;
		    if(augmenter)
		    tempsEffetMort+=tempsEcoule*10;
		    else
		    tempsEffetMort-=tempsEcoule;
		    if(tempsEffetMort>1)
		    augmenter=false;
		    if(tempsEffetMort<0)
		    augmenter=true;
		    tempsEcouleDepuisFPS+=tempsEcoule;

		    Clock.Reset();

		    ///Déplacements

			if(tempsEcouleDepuisDernierDeplacement>=0.01)
			{
				if(hero.m_personnage.seDeplacer(tempsEcouleDepuisDernierDeplacement*100))
					hero.m_personnage.pathfinding(map.getAlentourDuPersonnage(hero.m_personnage.getCoordonnee())); // Recherche du chemin

				if(!map.testEvenement(ecran,&hero,&camera,&menu)) // On test les événement pour voir s'il on doit changer de map, faire des dégats au perso, le régénérer, etc
                    return 0;

				hero.placerCamera(&camera,map.getDimensions()); // On place la camera suivant ou se trouve le perso

				ecran->SetView(&camera);

				/// On calcule les lumières et les ombres
				if(configuration.Lumiere&&lumiere)
				{
                    map.detruireOmbresEtLumieres(&hero);
                    map.calculerOmbresEtLumieres(ecran,&hero,&camera);
                    lumiere=false;
				}

				if(!lumiere)
				lumiere=true;

				///Placer l'écouteur, à la position du héro
				coordonnee position;
				position.x=(hero.m_personnage.getCoordonnee().x-hero.m_personnage.getCoordonnee().y-1+map.getDimensions().y)/5;
                position.y=(hero.m_personnage.getCoordonnee().x+hero.m_personnage.getCoordonnee().y)/5;

                Listener::SetGlobalVolume((float)configuration.volume);
				Listener::SetPosition(-position.x, 0, position.y);
				Listener::SetTarget(0, 0, 1);
				map.musiquePlay(position);

				tempsEcouleDepuisDernierDeplacement=0;
			}

		    /// On gère les événements, appui sur une touche, déplacement de la souris
			eventManager.GererLesEvenements(ecran,&camera,&continuer,tempsEcoule,&hero,&map);



			///Animation

			if(tempsDepuisDerniereAnimation>0.075)
			{
			    hero.m_personnage.animer(&hero.m_modelePersonnage,map.getDimensions().y); //Animation du héro
                map.animer(&hero); // Animation des tiles de la map
                tempsDepuisDerniereAnimation=0;
			}

			///Affichage

			if(tempsEcouleDepuisDernierAffichage>0.01)
			{
                ecran->SetView(&camera);

                map.Afficher(ecran,&camera,1,&hero,eventManager.getPositionSouris());//Affichage de la map

                ecran->SetView(NULL);

                if(configuration.Minimap)
                {
                    menu.Afficher(ecran,1);//On affiche le fond noir de la mini-map
                    map.Afficher(ecran,&camera,2,&hero,eventManager.getPositionSouris()); // On affiche la mini-map
                    menu.Afficher(ecran,2); // On affiche le cadran de la mini-map
                }

                menu.Afficher(ecran,3); // On affiche le hud
                 eventManager.AfficherCurseur(ecran); // On affiche le curseur de la souris

                if(map.getEvenement(eventManager.getCasePointee())>=0)
                    map.AfficherNomEvenement(ecran,eventManager.getCasePointee(),eventManager.getPositionSouris());

                if(configuration.effetMort&&sf::PostFX::CanUsePostFX() == true)
                {
                    EffectBlur.SetParameter("offset", 0.01*configuration.effetMort/100*(0.6+tempsEffetMort/10));
                    EffectColorize.SetParameter("color",1+1*configuration.effetMort/100*tempsEffetMort, 1-0.5*configuration.effetMort/100*tempsEffetMort, 1-0.5*configuration.effetMort/100*tempsEffetMort);
                    ecran->Draw(EffectBlur);
                    ecran->Draw(EffectColorize);
                }

                if(configuration.console)
                {
                    ecran->Draw(FPS); // On affiche le texte des FPS
                    ecran->Draw(Version);
                    ecran->Draw(Temps);

                    console.Afficher(ecran);
                }



                ecran->Display(); // On affiche le tout à l'écran

                tempsEcouleDepuisDernierAffichage=0;
			}
			if(configuration.console)
                if(tempsEcouleDepuisFPS>0.2)
                {
                    //  Calcule du nombre de FPS
                    sprintf(chaine,"%ld FPS",(int)( 1.f / ecran->GetFrameTime()));
                    FPS.SetText(chaine);
                    tempsEcouleDepuisFPS=0;

                    sprintf(chaine,"Temps : %ld h %ld ",configuration.heure,(int)configuration.minute);
                    Temps.SetText(chaine);
                }

	}

	return 1;
}






