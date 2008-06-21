


c_Jeu::c_Jeu(Jeu *)
{

        continuer=true,lumiere=false,augmenter=false;
        chaine[10];
        tempsActuel=0,tempsPrecedent=0,tempsDepuisDerniereAnimation=0,tempsEcoule=0,tempsEcouleDepuisDernierDeplacement=0,tempsEcouleDepuisDernierAffichage=0,tempsEcouleDepuisFPS=0,tempsEffetMort=1;

        configuration.heure=(rand() % (24));
        configuration.minute=0;

        camera(FloatRect(0, 0, configuration.Resolution.x,configuration.Resolution.y), 1.f);
        minijeu->map(FloatRect(0, 0, configuration.Resolution.x, configuration.Resolution.y), 0.125);
        sf::Listener::SetGlobalVolume((float)configuration.volume);




        FPS.SetText("FPS"); // Texte pour l'affichage des FPS
        FPS.SetSize(16.f);

        Version.SetText("Version");
        Version.SetSize(16.f);

        sprintf(chaine,"v %s",configuration.version.c_str());
        Version.SetText(chaine);
        Version.SetTop(20);

        Temps.SetText("Temps : ");
        Temps.SetSize(16.f);

        sprintf(chaine,"Temps : %ld h %ld ",configuration.heure,(int)configuration.minute);
        Temps.SetText(chaine);
        Temps.SetTop(40);

        // Chargement des jeu->menus


        console.Ajouter("",0);
        console.Ajouter("Chargement des effets :",0);



        if (sf::PostFX::CanUsePostFX() == true&&configuration.postFX)
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

        if(!bufferSonMort.LoadFromFile(configuration.chemin_son_mort))
            console.Ajouter("Impossible de charger : "+configuration.chemin_son_mort,1);
        else
            console.Ajouter("Chargement de : "+configuration.chemin_son_mort,0);

        sonMort.SetBuffer(bufferSonMort),sonMort.SetVolume(0);
        sonMort.SetLoop(true);
        sonMort.Play();


        if(!jeu->hero.m_modelePersonnage.Charger("Data/Personnages/jeu->hero/GuerrierHache.txt")) // Chargement du h�ro
        {
            console.Rapport();
            return 0;
        }


        camera.Rect.Left=0;
        camera.Rect.Top=0;

        if(!jeu->map.Charger(0)==1) // Chargement de  jeu->map0.txt
        {
            console.Rapport();
            return 0;
        }

        ecran->ShowMouseCursor(false);
        //if(configuration.Lumiere)
        //jeu->map.calculerOmbresEtLumieres(ecran,&jeu->hero,&configuration);

        if(configuration.Lumiere)
        {
            jeu->jeu->map.calculerOmbresEtLumieres(ecran,&jeu->hero,&camera);
            lumiere=false;
        }

        Clock.Reset();

}

void c_Jeu::Utiliser(Jeu *)
{
	//while(continuer)
	//{
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

		    jeu->jeu->hero.m_personnage.regenererVie((float)jeu->hero.m_personnage.getCaracteristique().maxVie*tempsEcoule/100);

		    Clock.Reset();

		    if(jeu->jeu->hero.m_personnage.getCaracteristique().vie/(float)jeu->jeu->hero.m_personnage.getCaracteristique().maxVie<0.5)
                configuration.effetMort=200-(jeu->jeu->hero.m_personnage.getCaracteristique().vie*400/jeu->jeu->hero.m_personnage.getCaracteristique().maxVie),sonMort.SetVolume(configuration.effetMort);

		    ///D�placements

			if(tempsEcouleDepuisDernierDeplacement>=0.0)
			{
			    coordonnee temp={-1 ,-1 ,-1 ,-1};
				if(jeu->hero.m_personnage.seDeplacer(tempsEcouleDepuisDernierDeplacement*100))
				{
					jeu->hero.m_personnage.pathfinding(jeu->map.getAlentourDuPersonnage(jeu->hero.m_personnage.getCoordonnee()),temp); // Recherche du chemin
					if(configuration.Lumiere)
                        //jeu->map.calculerOmbresEtLumieres(ecran,&jeu->hero,&camera);
                        lumiere=true;
				}
				if(jeu->hero.getMonstreVise()>-1)
                    jeu->hero.testMontreVise(jeu->map.getEntiteMonstre(jeu->hero.getMonstreVise()),jeu->map.getDimensions().y);

				if(!jeu->map.testEvenement(ecran,&jeu->hero,&camera,&jeu->menu)) // On test les �v�nement pour voir s'il on doit changer de jeu->map, faire des d�gats au perso, le r�g�n�rer, etc
                {
                    console.Rapport();
                    return 0;
                }

                Clock.Reset();

				jeu->hero.placerCamera(&camera,jeu->map.getDimensions()); // On place la camera suivant ou se trouve le perso

				jeu->map.gererMonstres(&jeu->hero,tempsEcouleDepuisDernierDeplacement);

				ecran->SetView(&camera);

				/// On calcule les lumi�res et les ombres
				if(configuration.Lumiere&&lumiere)
				{
                    jeu->map.calculerOmbresEtLumieres(ecran,&jeu->hero,&camera);
                    lumiere=false;
				}
				else
				lumiere=true;



				///Placer l'�couteur, � la position du h�ro
				coordonnee position;
				position.x=(jeu->hero.m_personnage.getCoordonnee().x-jeu->hero.m_personnage.getCoordonnee().y-1+jeu->map.getDimensions().y)/5;
                position.y=(jeu->hero.m_personnage.getCoordonnee().x+jeu->hero.m_personnage.getCoordonnee().y)/5;

                Listener::SetGlobalVolume((float)configuration.volume);
				Listener::SetPosition(-position.x, 0, position.y);
				Listener::SetTarget(0, 0, 1);
				jeu->map.musiquePlay(position);
				sonMort.SetPosition(position.x,0,position.y);

				jeu->hero.m_personnage.AjouterPointAme(jeu->menu.GererDynamique(tempsEcoule));

				tempsEcouleDepuisDernierDeplacement=0;
			}

		    /// On g�re les �v�nements, appui sur une touche, d�placement de la souris
			jeu->eventManager.GererLesEvenements(ecran,&camera,&continuer,tempsEcoule,&jeu->hero,&jeu->map);



			///Animation

			if(tempsDepuisDerniereAnimation>0.05)
			{
			    if(jeu->hero.m_personnage.animer(&jeu->hero.m_modelePersonnage,jeu->map.getDimensions().y,tempsDepuisDerniereAnimation)==1) //Animation du h�ro
			    {
                    if(jeu->map.infligerDegats(jeu->hero.getMonstreVise(),(rand()%(jeu->hero.m_personnage.getCaracteristique().degatsMax - jeu->hero.m_personnage.getCaracteristique().degatsMin+1))+jeu->hero.m_personnage.getCaracteristique().degatsMin,&jeu->menu,&camera)) // Si l'enemi meut, renvoi true
                        jeu->eventManager.arreterClique();
                    jeu->hero.setMonstreVise(-1);
			    }
                jeu->map.animer(&jeu->hero,tempsDepuisDerniereAnimation,&jeu->menu); // Animation des tiles de la jeu->map
                tempsDepuisDerniereAnimation=0;
			}

			///Affichage

			if(tempsEcouleDepuisDernierAffichage>0.1&&configuration.syncronisation_verticale||!configuration.syncronisation_verticale)
			{
                ecran->SetView(&camera);

                jeu->map.Afficher(ecran,&camera,1,&jeu->hero,jeu->eventManager.getPositionSouris());//Affichage de la jeu->map

                ecran->SetView(NULL);

                if(configuration.Minijeu->map)
                {
                    jeu->menu.Afficher(ecran,1);//On affiche le fond noir de la mini-jeu->map
                    jeu->map.Afficher(ecran,&camera,2,&jeu->hero,jeu->eventManager.getPositionSouris()); // On affiche la mini-jeu->map
                    jeu->menu.Afficher(ecran,2); // On affiche le cadran de la mini-jeu->map
                }

                jeu->menu.Afficher(ecran,3); // On affiche le hud
                jeu->menu.AfficherDynamique(ecran,jeu->hero.m_personnage.getCaracteristique());
                 jeu->eventManager.AfficherCurseur(ecran); // On affiche le curseur de la souris

                if(jeu->map.getEvenement(jeu->eventManager.getCasePointee())>=0)
                    jeu->map.AfficherNomEvenement(ecran,jeu->eventManager.getCasePointee(),jeu->eventManager.getPositionSouris());

                if(configuration.effetMort&&sf::PostFX::CanUsePostFX() == true&&configuration.postFX)
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



                ecran->Display(); // On affiche le tout � l'�cran

                tempsEcouleDepuisDernierAffichage=0;
			}




			if(configuration.console)
                if(tempsEcouleDepuisFPS>0.1)
                {
                    //  Calcule du nombre de FPS
                    sprintf(chaine,"%ld FPS",(int)( 1.f / ecran->GetFrameTime()));
                    FPS.SetText(chaine);
                    tempsEcouleDepuisFPS=0;

                    sprintf(chaine,"Temps : %ld h %ld ",configuration.heure,(int)configuration.minute);
                    Temps.SetText(chaine);
                }

	//}
}

