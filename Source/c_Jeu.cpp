
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
        tempsActuel=0,tempsPrecedent=0,tempsDepuisDerniereAnimation=0,tempsEcoule=0,tempsEcouleDepuisDernierDeplacement=0,tempsEcouleDepuisDernierAffichage=0,tempsEcouleDepuisFPS=0,tempsEffetMort=1;

        configuration.heure=(rand() % (24));
        configuration.minute=0;

        sf::Listener::SetGlobalVolume((float)configuration.volume);

        camera=new sf::View();


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


        if(!jeu->hero.m_modelePersonnage.Charger("Data/Personnages/hero/GuerrierHache.char.hs")) // Chargement du h�ro
            throw("CRITICAL ERROR");


        if(!jeu->map.Charger(0)==1) // Chargement de  jeu->map0.txt
            throw("CRITICAL ERROR");

        jeu->ecran.ShowMouseCursor(false);
        //if(configuration.Lumiere)
        //jeu->map.calculerOmbresEtLumieres(ecran,&jeu->hero,&configuration);

        if(configuration.Lumiere)
        {
            jeu->map.calculerOmbresEtLumieres(&jeu->ecran,&jeu->hero,camera);
            lumiere=false;
        }

        jeu->Clock.Reset();

}

void c_Jeu::Utiliser(Jeu *jeu)
{
	//while(jeu->m_run)
	//{
            //Gestion du temps
            tempsEcoule = jeu->Clock.GetElapsedTime();

            if(tempsEcoule>0.001)
            {
                jeu->m_display=false;
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

                jeu->hero.m_personnage.regenererVie((float)jeu->hero.m_personnage.getCaracteristique().maxVie*tempsEcoule/100);
                jeu->hero.augmenterAme(tempsEcoule);

                jeu->Clock.Reset();

                if(jeu->hero.m_personnage.getCaracteristique().vie/(float)jeu->hero.m_personnage.getCaracteristique().maxVie<0.5)
                    configuration.effetMort=200-(jeu->hero.m_personnage.getCaracteristique().vie*400/jeu->hero.m_personnage.getCaracteristique().maxVie),jeu->sonMort.SetVolume(configuration.effetMort);
                else
                    configuration.effetMort=0,jeu->sonMort.SetVolume(0);
                ///D�placements

                if(tempsEcouleDepuisDernierDeplacement>=0.01)
                {
                    coordonnee temp={-1 ,-1 ,-1 ,-1};
                    if(jeu->hero.getMonstreVise()==-1)
                        temp.x=jeu->hero.m_personnage.getCoordonnee().x,temp.y=jeu->hero.m_personnage.getCoordonnee().y;

                    if(jeu->hero.m_personnage.seDeplacer(tempsEcouleDepuisDernierDeplacement*100))
                    {
                        jeu->hero.m_personnage.pathfinding(jeu->map.getAlentourDuPersonnage(jeu->hero.m_personnage.getCoordonnee()),temp); // Recherche du chemin
                        if(configuration.Lumiere)
                            //jeu->map.calculerOmbresEtLumieres(ecran,&jeu->hero,&camera);
                            lumiere=true;
                    }
                    if(jeu->hero.getMonstreVise()>-1)
                        jeu->hero.testMontreVise(jeu->map.getEntiteMonstre(jeu->hero.getMonstreVise()),jeu->map.getDimensions().y);

                    if(jeu->map.testEvenement(camera,jeu)>-1) // On test les �v�nement pour voir s'il on doit changer de jeu->map, faire des d�gats au perso, le r�g�n�rer, etc
                    {
                    }

                    //jeu->Clock.Reset();

                    jeu->map.gererMonstres(&jeu->hero,tempsEcouleDepuisDernierDeplacement);

                    /// On calcule les lumi�res et les ombres
                    if(configuration.Lumiere&&lumiere)
                    {
                        jeu->map.calculerOmbresEtLumieres(&jeu->ecran,&jeu->hero,camera);
                        lumiere=false;
                    }
                    //else
                    //jeu->map.calculerOmbresEtLumieresDuHero(&jeu->ecran,&jeu->hero,camera);
                    //else
                    //lumiere=true;



                    ///Placer l'�couteur, � la position du h�ro
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



                ///Animation

                if(tempsDepuisDerniereAnimation>0.055)
                {
                    lumiere=true;
                    if(jeu->hero.m_personnage.animer(&jeu->hero.m_modelePersonnage,jeu->map.getDimensions().y,tempsDepuisDerniereAnimation)==1) //Animation du h�ro
                    {
                        if(jeu->map.infligerDegats(jeu->hero.getMonstreVise(),(rand()%(jeu->hero.m_personnage.getCaracteristique().degatsMax - jeu->hero.m_personnage.getCaracteristique().degatsMin+1))+jeu->hero.m_personnage.getCaracteristique().degatsMin,&jeu->menu,camera)) // Si l'enemi meut, renvoi true
                            jeu->hero.m_personnage.setMauvaiseArrivee(jeu->eventManager.getCasePointee());
                        jeu->hero.setMonstreVise(-1);
                    }
                    jeu->map.animer(&jeu->hero,tempsDepuisDerniereAnimation,&jeu->menu); // Animation des tiles de la jeu->map
                    tempsDepuisDerniereAnimation=0;
                }

                ///Affichage

                if(tempsEcouleDepuisDernierAffichage>0.01&&configuration.syncronisation_verticale||!configuration.syncronisation_verticale)
                {
                    jeu->hero.placerCamera(camera,jeu->map.getDimensions()); // On place la camera suivant ou se trouve le perso
                    camera->Zoom(configuration.zoom);
                    /// On g�re les �v�nements, appui sur une touche, d�placement de la souris
                    jeu->eventManager.GererLesEvenements(&jeu->ecran,camera,&jeu->m_run,tempsEcoule,jeu->map.getDimensions());

                    int monstreVise=jeu->map.getMonstre(&jeu->hero,camera,&jeu->ecran,jeu->eventManager.getPositionSouris(),jeu->eventManager.getCasePointee());

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



                    jeu->map.Afficher(&jeu->ecran,camera,1,&jeu->hero,jeu->eventManager.getPositionSouris());//Affichage de la jeu->map

                    if(configuration.Minimap)
                    {
                        jeu->menu.Afficher(&jeu->ecran,1);//On affiche le fond noir de la mini-jeu->map
                        jeu->map.Afficher(&jeu->ecran,camera,2,&jeu->hero,jeu->eventManager.getPositionSouris()); // On affiche la mini-jeu->map
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
                        jeu->EffectMort.SetParameter("offset", 0.01*configuration.effetMort/100*(0.6+tempsEffetMort/10));
                        jeu->EffectMort.SetParameter("color",1+1*configuration.effetMort/100*tempsEffetMort, 1-0.5*configuration.effetMort/100*tempsEffetMort, 1-0.5*configuration.effetMort/100*tempsEffetMort);
                    }

                    if(configuration.console)
                    {
                        //jeu->ecran.Draw(fps); // On affiche le texte des FPS
                        //jeu->ecran.Draw(Version);
                        //jeu->ecran.Draw(Temps);
                        moteurGraphique.AjouterTexte(&fps);
                        moteurGraphique.AjouterTexte(&Version);
                        moteurGraphique.AjouterTexte(&Temps);

                        console.Afficher(&jeu->ecran);
                    }

                    jeu->m_display=true;

                    //jeu->ecran.Display(); // On affiche le tout � l'�cran

                    tempsEcouleDepuisDernierAffichage=0;
                }

                if(configuration.console)
                    if(tempsEcouleDepuisFPS>0.1)
                    {
                        //  Calcule du nombre de FPS
                       sprintf(chaine,"%ld FPS",(int)( 1.f / jeu->ecran.GetFrameTime()));
                        fps.SetText(chaine);


                        sprintf(chaine,"Temps : %ld h %ld ",configuration.heure,(int)configuration.minute);
                        Temps.SetText(chaine);

                        tempsEcouleDepuisFPS=0;
                    }
            }

	//}
}

