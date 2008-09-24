
#include "c_jeu.h"
#include "jeu.h"

#include "Globale.h"

#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

using namespace sf;

c_Jeu::c_Jeu(Jeu *jeu)
{
        continuer=true,lumiere=false,augmenter=false;
        chaine[10];
        tempsActuel=0,tempsPrecedent=0,tempsDepuisDerniereAnimation=0,tempsSauvergarde=0,tempsEcoule=0,tempsEcouleDepuisDernierDeplacement=0,tempsEcouleDepuisDernierAffichage=0,tempsEcouleDepuisFPS=0,tempsEffetMort=1,tempsNbrTourBoucle=0;
        nbrTourBoucle=0;

        configuration.heure=(rand() % (24));
        configuration.minute=0;

        sf::Listener::SetGlobalVolume((float)configuration.volume);

       // camera=new sf::View();

        // Texte pour l'affichage des FPS
        fps.SetSize(16.f);

        Version.SetSize(16.f);
        sprintf(chaine,"v %s",configuration.version.c_str());
        Version.SetText(chaine);
        Version.SetY(20);

        Temps.SetSize(16.f);
        sprintf(chaine,"Temps : %ld h %ld ",configuration.heure,(int)configuration.minute);
        Temps.SetText(chaine);
        Temps.SetY(40);

        TourBoucle.SetX(100);
        TourBoucle.SetSize(16.f);

        // Chargement des menus

        console.Ajouter("",0);
        console.Ajouter("Chargement des effets :",0);


        if(!jeu->bufferSonMort.LoadFromFile(configuration.chemin_son_mort))
            console.Ajouter("Impossible de charger : "+configuration.chemin_son_mort,1);
        else
            console.Ajouter("Chargement de : "+configuration.chemin_son_mort,0);

        jeu->sonMort.SetBuffer(jeu->bufferSonMort);
        jeu->sonMort.SetVolume(0);
        jeu->sonMort.SetLoop(true);
        jeu->sonMort.Play();


        if(!jeu->hero.m_modelePersonnage.Charger("Data/Entites/hero/GuerrierHache.char.hs")) // Chargement du héro
            throw("CRITICAL ERROR");


        if(!jeu->map.Charger(0)==1) // Chargement de  jeu->map0.txt
            throw("CRITICAL ERROR");

        jeu->ecran.ShowMouseCursor(false);
        //if(configuration.Lumiere)
        //jeu->map.calculerOmbresEtLumieres(ecran,&jeu->hero,&configuration);

        if(configuration.Lumiere)
        {
            jeu->map.calculerOmbresEtLumieres(&jeu->ecran,&jeu->hero,&jeu->camera);
            lumiere=false;
        }

        jeu->Clock.Reset();

}

void c_Jeu::Utiliser(Jeu *jeu)
{
	//while(jeu->m_run)
	//{
            //Gestion du temps
            jeu->m_display=false;
            tempsEcoule = jeu->Clock.GetElapsedTime();

            if(tempsEcoule>0.00)
            {
                tempsEcouleDepuisDernierDeplacement+=tempsEcoule;
                tempsDepuisDerniereAnimation+=tempsEcoule;
                tempsEcouleDepuisDernierAffichage+=tempsEcoule;
                tempsEcouleDepuisDernierIA+=tempsEcoule;
                tempsEcouleDepuisDernierCalculLumiere+=tempsEcoule;
                tempsSauvergarde+=tempsEcoule;
                configuration.minute+=tempsEcoule;
                tempsNbrTourBoucle+=tempsEcoule;
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

                jeu->hero.m_personnage.regenererVie((float)jeu->hero.m_personnage.getCaracteristique().maxVie*tempsEcoule/100);
                jeu->hero.augmenterAme(tempsEcoule);

                jeu->Clock.Reset();

                if(jeu->hero.m_personnage.getCaracteristique().vie/(float)jeu->hero.m_personnage.getCaracteristique().maxVie<0.5)
                    configuration.effetMort=200-(jeu->hero.m_personnage.getCaracteristique().vie*400/jeu->hero.m_personnage.getCaracteristique().maxVie),jeu->sonMort.SetVolume(configuration.effetMort);
                else
                    configuration.effetMort=0,jeu->sonMort.SetVolume(0);

                ///Sauvegarde automatique

                if(tempsSauvergarde>=configuration.frequence_sauvegarde)
                {
                    //jeu->hero.Sauvegarder();
                    //jeu->map.Sauvegarder();
                    tempsSauvergarde=0;
                }

                ///IA

                if(tempsEcouleDepuisDernierIA>=0.027)
                {
                    jeu->map.gererMonstres(&jeu->hero,tempsEcouleDepuisDernierIA,&jeu->camera,&jeu->menu);
                    tempsEcouleDepuisDernierIA=0;
                }

                ///Déplacements
                if(tempsEcouleDepuisDernierDeplacement>=0.011)
                {
                    coordonnee temp={-1 ,-1 ,-1 ,-1};
                    if(jeu->hero.getMonstreVise()==-1)
                        temp.x=jeu->hero.m_personnage.getCoordonnee().x,temp.y=jeu->hero.m_personnage.getCoordonnee().y;

                    if(jeu->hero.m_personnage.seDeplacer(tempsEcouleDepuisDernierDeplacement*100))
                    {
                        jeu->hero.m_personnage.pathfinding(jeu->map.getAlentourDuPersonnage(jeu->hero.m_personnage.getCoordonnee()),temp); // Recherche du chemin
                        if(configuration.Lumiere)
                            lumiere=true;
                    }
                    jeu->map.testEvenement(&jeu->camera,jeu,tempsEcoule); // On test les événement pour voir s'il on doit changer de jeu->map, faire des dégats au perso, le régénérer, etc
                    if(jeu->hero.getMonstreVise()>-1)
                        jeu->hero.testMontreVise(jeu->map.getEntiteMonstre(jeu->hero.getMonstreVise()),jeu->map.getDimensions().y);

                    ///Placer l'écouteur, à la position du héro
                    coordonnee position;
                    position.x=(jeu->hero.m_personnage.getCoordonnee().x-jeu->hero.m_personnage.getCoordonnee().y-1+jeu->map.getDimensions().y)/5;
                    position.y=(jeu->hero.m_personnage.getCoordonnee().x+jeu->hero.m_personnage.getCoordonnee().y)/5;

                    Listener::SetGlobalVolume((float)configuration.volume);
                    Listener::SetPosition(-position.x, 0, position.y);
                    Listener::SetTarget(0, 0, 1);
                    jeu->map.musiquePlay(position);
                    jeu->sonMort.SetPosition(position.x,0,position.y);

                    jeu->hero.m_personnage.AjouterPointAme(jeu->menu.GererDynamique(tempsEcoule));

                    tempsEcouleDepuisDernierDeplacement=0;
                }


                /// On calcule les lumières et les ombre
                if(tempsEcouleDepuisDernierCalculLumiere>configuration.frequence_lumiere)
                {
                    lumiere=true;
                    tempsEcouleDepuisDernierCalculLumiere=0;
                }
                if(configuration.Lumiere&&lumiere)
                {
                        jeu->map.calculerOmbresEtLumieres(&jeu->ecran,&jeu->hero,&jeu->camera);
                        lumiere=false;
                }


                ///Animation

               if(tempsDepuisDerniereAnimation>0.043)
                {
                    bool a; // Variable qui ne sert pas ici, mais qui remplace le explosif des monstres
                    if(jeu->hero.m_personnage.animer(&jeu->hero.m_modelePersonnage,jeu->map.getDimensions().y,tempsDepuisDerniereAnimation,&a)==1) //Animation du héro
                    {
                        jeu->map.infligerDegats(jeu->hero.getMonstreVise(),(rand()%(jeu->hero.m_personnage.getCaracteristique().degatsMax - jeu->hero.m_personnage.getCaracteristique().degatsMin+1))+jeu->hero.m_personnage.getCaracteristique().degatsMin,&jeu->menu,&jeu->camera);
                        jeu->hero.setMonstreVise(-1);
                    }
                    jeu->map.animer(&jeu->hero,tempsDepuisDerniereAnimation,&jeu->menu); // Animation des tiles de la jeu->map
                    tempsDepuisDerniereAnimation=0;
                }

                ///Affichage

                if(tempsEcouleDepuisDernierAffichage>0.013&&configuration.syncronisation_verticale||!configuration.syncronisation_verticale)
                {
                    jeu->hero.placerCamera(&jeu->camera,jeu->map.getDimensions()); // On place la camera suivant ou se trouve le perso
                    jeu->camera.Zoom(configuration.zoom);
                    /// On gère les événements, appui sur une touche, déplacement de la souris
                    jeu->eventManager.GererLesEvenements(&jeu->ecran,&jeu->camera,&jeu->m_run,tempsEcoule,jeu->map.getDimensions());

                    int monstreVise=jeu->map.getMonstre(&jeu->hero,&jeu->camera,&jeu->ecran,jeu->eventManager.getPositionSouris(),jeu->eventManager.getCasePointee());

                    if(jeu->eventManager.getEvenement(Mouse::Left,"C")&&!jeu->eventManager.getEvenement(Key::LShift,"ET"))
                    {
                        jeu->hero.setMonstreVise(monstreVise);
                        if(jeu->hero.getMonstreVise()==-1)
                            jeu->hero.m_personnage.setArrivee(jeu->eventManager.getCasePointee());
                    }
                    if(jeu->eventManager.getEvenement(Mouse::Right,"C")||jeu->eventManager.getEvenement(Mouse::Left,"C")&&jeu->eventManager.getEvenement(Key::LShift,"ET"))
                    {
                        coordonnee temp;
                        temp.x=configuration.Resolution.x/2;
                        temp.y=configuration.Resolution.y/2;
                        if(monstreVise==-1)
                        jeu->hero.m_personnage.frappe(jeu->eventManager.getPositionSouris(),temp);
                        jeu->hero.setMonstreVise(monstreVise);
                    }

                    if(jeu->eventManager.getEvenement(Key::I,"ET"))
                        jeu->m_contexte=jeu->m_inventaire,jeu->eventManager.StopEvenement(Key::I,"ET");

                    if(jeu->eventManager.getEvenement(Key::Escape,"ET"))
                        jeu->m_contexte=jeu->m_menuInGame,jeu->eventManager.StopEvenement(Key::Escape,"ET");

                    jeu->map.Afficher(&jeu->ecran,&jeu->camera,1,&jeu->hero,jeu->eventManager.getPositionSouris(),jeu->eventManager.getEvenement(Key::LAlt,"ET"));//Affichage de la jeu->map

                    if(configuration.Minimap)
                    {
                        jeu->menu.Afficher(&jeu->ecran,1);//On affiche le fond noir de la mini-jeu->map
                        jeu->map.Afficher(&jeu->ecran,&jeu->camera,2,&jeu->hero,jeu->eventManager.getPositionSouris(),0); // On affiche la mini-jeu->map
                        jeu->menu.Afficher(&jeu->ecran,2); // On affiche le cadran de la mini-jeu->map
                    }

                    jeu->menu.Afficher(&jeu->ecran,3); // On affiche le hud
                    if(jeu->map.getEntiteMonstre(jeu->map.getMonstreIllumine())!=NULL)
                        jeu->menu.AfficherDynamique(&jeu->ecran,jeu->hero.m_personnage.getCaracteristique(),1,jeu->map.getEntiteMonstre(jeu->map.getMonstreIllumine())->getCaracteristique());
                    else
                        jeu->menu.AfficherDynamique(&jeu->ecran,jeu->hero.m_personnage.getCaracteristique(),0,jeu->hero.m_personnage.getCaracteristique());
                    jeu->eventManager.AfficherCurseur(&jeu->ecran); // On affiche le curseur de la souris

                    if(jeu->map.getEvenement(jeu->eventManager.getCasePointee())>=0)
                        jeu->map.AfficherNomEvenement(&jeu->ecran,jeu->eventManager.getCasePointee(),jeu->eventManager.getPositionSouris());

                    if(configuration.effetMort&&sf::PostFX::CanUsePostFX() == true&&configuration.postFX)
                    {
                        moteurGraphique.EffectMort.SetParameter("offset", 0.01*configuration.effetMort/100*(0.6+tempsEffetMort/10));
                        moteurGraphique.EffectMort.SetParameter("color",1+1*configuration.effetMort/100*tempsEffetMort, 1-0.5*configuration.effetMort/100*tempsEffetMort, 1-0.5*configuration.effetMort/100*tempsEffetMort);
                    }

                    if(configuration.console)
                    {
                        moteurGraphique.AjouterTexte(&fps);
                        Version.SetY(20);
                        moteurGraphique.AjouterTexte(&Version);
                        Temps.SetY(40);
                        moteurGraphique.AjouterTexte(&Temps);
                        moteurGraphique.AjouterTexte(&TourBoucle);

                        console.Afficher(&jeu->ecran);
                    }

                    jeu->m_display=true;

                    tempsEcouleDepuisDernierAffichage=0;
                }

                nbrTourBoucle++;


                if(tempsNbrTourBoucle>1)
                {
                    sprintf(chaine,"Nbr Tour de boucle : %ld",nbrTourBoucle);
                    TourBoucle.SetText(chaine);
                    nbrTourBoucle=0;

                    tempsNbrTourBoucle=0;
                }

                if(configuration.console)
                    sprintf(chaine,"%ld FPS",(int)( 1.f / jeu->ecran.GetFrameTime()));

                if(configuration.console)
                    if(tempsEcouleDepuisFPS>0.1)
                    {
                        //  Calcule du nombre de FPS

                        fps.SetText(chaine);


                        sprintf(chaine,"Temps : %ld h %ld ",configuration.heure,(int)configuration.minute);
                        Temps.SetText(chaine);

                        tempsEcouleDepuisFPS=0;
                    }
            }

	//}
}

