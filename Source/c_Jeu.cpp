

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


#include "c_Jeu.h"
#include "jeu.h"

#include "globale.h"

#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>


using namespace sf;

c_Jeu::c_Jeu(Jeu *jeu)
{
        continuer=true,lumiere=false,augmenter=false;
        tempsActuel=0,tempsPrecedent=0,tempsDepuisDerniereAnimation=0,tempsEcoule=0,tempsNbrTourBoucle=0,tempsEcouleDepuisDernierCalculLumiere=0,tempsEcouleDepuisDernierDeplacement=0,tempsEcouleDepuisDernierIA=0,tempsEcouleDepuisDernierAffichage=0,tempsEcouleDepuisFPS=0,tempsEffetMort=0,tempsSauvergarde=0;
        nbrTourBoucle=0;

        configuration->heure=(rand() % (24));
        configuration->minute=0;

        sf::Listener::SetGlobalVolume((float)configuration->volume);

        // Texte pour l'affichage des FPS
        fps.SetSize(16.f);

        Version.SetSize(16.f);
        sprintf(chaine,"v %s",configuration->version.c_str());
        Version.SetText(chaine);
        Version.SetY(20);

        Temps.SetSize(16.f);
        sprintf(chaine,"Temps : %i h %i ",configuration->heure,(int)configuration->minute);
        Temps.SetText(chaine);
        Temps.SetY(40);

        TourBoucle.SetX(100);
        TourBoucle.SetSize(16.f);

        // Chargement des menus

        console->Ajouter("",0);
        console->Ajouter("Chargement des sons :",0);


        if(!jeu->bufferSonMort.LoadFromFile(configuration->chemin_son_mort))
            console->Ajouter("Impossible de charger : "+configuration->chemin_son_mort,1);
        else
            console->Ajouter("Chargement de : "+configuration->chemin_son_mort,0);

        jeu->sonMort.SetBuffer(jeu->bufferSonMort);
        jeu->sonMort.SetVolume(0);
        jeu->sonMort.SetLoop(true);
        jeu->sonMort.Play();



        jeu->hero.ChargerModele(); // Chargement du héro

        //jeu->hero.m_personnage.Charger(&jeu->hero.m_modelePersonnage);

        if(!jeu->map.Charger(0,&jeu->hero)==1) // Chargement de  jeu->map0.txt
            throw("CRITICAL ERROR");

        jeu->ecran.ShowMouseCursor(false);

        if(configuration->Lumiere)
        {
            jeu->map.calculerOmbresEtLumieres();
            lumiere=false;
        }

        jeu->Clock.Reset();

        alpha_map=0;
        alpha_sac=0;
}

void c_Jeu::Utiliser(Jeu *jeu)
{
            //Gestion du temps
            jeu->m_display=false;
            tempsEcoule = jeu->Clock.GetElapsedTime();
            if(tempsEcoule>0.1)
            tempsEcoule=0.1;

            if(tempsEcoule>0.01&&configuration->syncronisation_verticale||!configuration->syncronisation_verticale)
            {
                tempsEcouleDepuisDernierDeplacement+=tempsEcoule;
                tempsDepuisDerniereAnimation+=tempsEcoule;
                tempsEcouleDepuisDernierAffichage+=tempsEcoule;
                tempsEcouleDepuisDernierIA+=tempsEcoule;
                tempsEcouleDepuisDernierCalculLumiere+=tempsEcoule;
                tempsSauvergarde+=tempsEcoule;
                configuration->minute+=tempsEcoule;
                tempsNbrTourBoucle+=tempsEcoule;
                if(configuration->minute>=60)
                    configuration->minute=0,configuration->heure++;
                if(configuration->heure>23)
                    configuration->heure=0;
                if(augmenter)
                    tempsEffetMort+=tempsEcoule*10;
                else
                    tempsEffetMort-=tempsEcoule;

                if(!jeu->hero.m_personnage.enVie())
                    tempsEffetMort=1;

                if(tempsEffetMort>1)
                    augmenter=false;
                if(tempsEffetMort<0)
                    augmenter=true;
                tempsEcouleDepuisFPS+=tempsEcoule;


                jeu->hero.augmenterAme(tempsEcoule);
                jeu->hero.recalculerCaracteristiques();
                if(jeu->hero.m_personnage.enVie())
                {
                    jeu->hero.regenererVie((float)jeu->hero.m_caracteristiques.maxVie*(float)(tempsEcoule/100));
                    jeu->hero.regenererFoi((float)jeu->hero.m_caracteristiques.maxFoi*(float)(tempsEcoule/100));
                }

                jeu->Clock.Reset();

                if(jeu->hero.m_personnage.enVie())
                {
                    if(jeu->hero.m_caracteristiques.maxVie!=0)
                        if(jeu->hero.m_caracteristiques.vie/(float)jeu->hero.m_caracteristiques.maxVie<0.5)
                            configuration->effetMort=150-(jeu->hero.m_caracteristiques.vie*300/jeu->hero.m_caracteristiques.maxVie),jeu->sonMort.SetVolume(configuration->effetMort);
                    else
                        configuration->effetMort=0,jeu->sonMort.SetVolume(0);
                }
                else
                    configuration->effetMort=150;

                ///**********************************************************///
                ///Sauvegarde automatique
                ///**********************************************************///

                if(tempsSauvergarde>=configuration->frequence_sauvegarde)
                {
                    jeu->hero.Sauvegarder();
                    jeu->map.Sauvegarder(&jeu->hero);
                    tempsSauvergarde=0;
                }

                ///**********************************************************///
                ///IA
                ///**********************************************************///

                if(tempsEcouleDepuisDernierIA>=0.027)
                {
                    jeu->map.gererMonstres(&jeu->hero,tempsEcouleDepuisDernierIA,&jeu->camera,&jeu->menu);
                    tempsEcouleDepuisDernierIA=0;
                }

                ///**********************************************************///
                ///Déplacements
                ///**********************************************************///

                if(tempsEcouleDepuisDernierDeplacement>=0.0)
                {
                    coordonnee temp={-1 ,-1 ,-1 ,-1};
                    if(jeu->hero.getMonstreVise()==-1)
                        temp.x=jeu->hero.m_personnage.getCoordonnee().x,temp.y=jeu->hero.m_personnage.getCoordonnee().y;

                    if(jeu->hero.m_personnage.seDeplacer(tempsEcouleDepuisDernierDeplacement*100,jeu->map.getDimensions()))
                    {
                        bool ok=true;
                        if(jeu->hero.getMonstreVise()>-1)
                            if(jeu->hero.testMonstreVise(jeu->map.getEntiteMonstre(jeu->hero.getMonstreVise()),jeu->map.getDimensions().y))
                                ok=false;

                        if(jeu->hero.m_personnage.getCoordonnee().x==jeu->hero.m_personnage.getArrivee().x && jeu->hero.m_personnage.getCoordonnee().y==jeu->hero.m_personnage.getArrivee().y)
                            ok=false;

                        if(ok)
                        {
                            jeu->hero.m_personnage.pathfinding(jeu->map.getAlentourDuPersonnage(jeu->hero.m_personnage.getCoordonnee()),temp); // Recherche du chemin
                        }
                        if(configuration->Lumiere)
                            lumiere=true;
                    }
                    jeu->map.testEvenement(&jeu->camera,jeu,tempsEcoule); // On test les événement pour voir s'il on doit changer de jeu->map, faire des dégats au perso, le régénérer, etc


                    ///Placer l'écouteur, à la position du héro
                    coordonnee position;
                    position.x=(jeu->hero.m_personnage.getCoordonnee().x-jeu->hero.m_personnage.getCoordonnee().y-1+jeu->map.getDimensions().y)/5;
                    position.y=(jeu->hero.m_personnage.getCoordonnee().x+jeu->hero.m_personnage.getCoordonnee().y)/5;

                    Listener::SetGlobalVolume((float)configuration->volume);
                    Listener::SetPosition(-position.x, 0, position.y);
                    Listener::SetTarget(0, 0, 1);
                    jeu->map.musiquePlay(position);
                    jeu->sonMort.SetPosition(position.x,0,position.y);

                   jeu->hero.m_personnage.AjouterPointAme(jeu->menu.GererDynamique(tempsEcoule));

                    tempsEcouleDepuisDernierDeplacement=0;
                }

                ///**********************************************************///
                ///Animation
                ///**********************************************************///

                if(tempsDepuisDerniereAnimation>0.04)
                {
                    if(tempsDepuisDerniereAnimation>0.8)
                        tempsDepuisDerniereAnimation=0.8;

                    coordonnee positionHero;
                    positionHero.x=(jeu->hero.m_personnage.getCoordonnee().x-jeu->hero.m_personnage.getCoordonnee().y-1+jeu->map.getDimensions().y)/5;
                    positionHero.y=(jeu->hero.m_personnage.getCoordonnee().x+jeu->hero.m_personnage.getCoordonnee().y)/5;


                    bool a; // Variable qui ne sert pas ici, mais qui remplace le explosif des monstres
                    int retour=-2;
                    retour=jeu->hero.m_personnage.animer(&jeu->hero.m_modelePersonnage[0],jeu->map.getDimensions().y,tempsDepuisDerniereAnimation,&a,positionHero);
                    if(retour!=-2)
                        jeu->hero.CalculerOrdreAffichage();

                    if(retour==0) //Animation du héro
                    {
                        if(jeu->hero.miracleEnCours==1)
                            jeu->map.infligerDegatsMasse(jeu->hero.m_personnage.getCoordonnee(),1,rand()%(jeu->hero.m_caracteristiques.degatsMax - jeu->hero.m_caracteristiques.degatsMin +1) + jeu->hero.m_caracteristiques.degatsMin ,false,&jeu->hero,&jeu->menu,&jeu->camera);
                        else if(jeu->hero.m_personnage.frappeEnCours)
                            jeu->map.infligerDegats(jeu->hero.getMonstreVise(),(rand()%(jeu->hero.m_personnage.getCaracteristique().degatsMax - jeu->hero.m_personnage.getCaracteristique().degatsMin+1))+jeu->hero.m_personnage.getCaracteristique().degatsMin,&jeu->menu,&jeu->camera,&jeu->hero,1);

                        jeu->hero.miracleEnCours=0;
                    }

                    if(retour==2)
                       if(!jeu->eventManager.getEvenement(Mouse::Left,"C") || !jeu->map.getMonstreEnVie(jeu->hero.getMonstreVise()) )
                            jeu->hero.setMonstreVise(-1),jeu->hero.m_personnage.frappeEnCours=false,jeu->hero.m_personnage.setEtat(0);

                    if(retour==1)
                        if(!jeu->eventManager.getEvenement(Mouse::Left,"C"))
                            jeu->hero.setMonstreVise(-1),jeu->hero.m_personnage.frappeEnCours=false,jeu->hero.m_personnage.setEtat(0);


                    jeu->map.animer(&jeu->hero,tempsDepuisDerniereAnimation,&jeu->menu,&jeu->camera); // Animation des tiles de la jeu->map

                    tempsDepuisDerniereAnimation=0;
                }

                ///**********************************************************///
                /// On calcule les lumières et les ombre
                ///**********************************************************///

                jeu->ecran.SetView(jeu->camera);

                if(configuration->Lumiere&&tempsEcouleDepuisDernierCalculLumiere>configuration->frequence_lumiere)
                {
                    jeu->map.calculerOmbresEtLumieres();

                    configuration->RafraichirLumiere=true;
                    tempsEcouleDepuisDernierCalculLumiere=0;

                    moteurGraphique->LightManager->Generate(jeu->hero.m_personnage.m_light);
                }


                if(tempsEcouleDepuisDernierAffichage>0.01&&configuration->syncronisation_verticale||!configuration->syncronisation_verticale)
                {
                    jeu->hero.placerCamera(&jeu->camera,jeu->map.getDimensions()); // On place la camera suivant ou se trouve le perso
                    jeu->camera.Zoom(configuration->zoom);

                    ///**********************************************************///
                    ///Evenements
                    ///**********************************************************///
                    jeu->eventManager.GererLesEvenements(&jeu->ecran,&jeu->camera,&jeu->m_run,tempsEcoule,jeu->map.getDimensions());

                    int monstreVise=-1;

                    if(!jeu->eventManager.getEvenement(Mouse::Left,"C"))
                        jeu->map.getMonstre(&jeu->hero,&jeu->camera,&jeu->ecran,jeu->eventManager.getPositionSouris(),jeu->eventManager.getCasePointee());

                    if(jeu->eventManager.getEvenement(Mouse::Left,"CA")&&!jeu->eventManager.getEvenement(Key::LShift,"ET"))
                    {
                        if(!(jeu->eventManager.getPositionSouris().x>configuration->Resolution.w-configuration->Resolution.w*0.25
                          &&jeu->eventManager.getPositionSouris().y>configuration->Resolution.w*0.25&&jeu->eventManager.getPositionSouris().y<configuration->Resolution.w*0.25+configuration->Resolution.w*0.34
                          &&alpha_sac>=128)||alpha_sac<=128)
                        {
                            if(jeu->hero.getMonstreVise()==-1)
                            {
                                jeu->hero.m_personnage.setArrivee(jeu->eventManager.getCasePointee());

                                coordonnee temp={-1,-1,-1,-1};
                                jeu->hero.setSacVise(temp);
                            }
                        }
                        else if(jeu->hero.getChercherSac().x!=-1&&jeu->map.getObjetPointe()!=-1)
                        {
                            jeu->map.ramasserObjet(&jeu->hero);
                            jeu->eventManager.StopEvenement(Mouse::Left,"CA");
                        }
                    }

                    if(jeu->eventManager.getEvenement(Mouse::Left,"C")&&jeu->eventManager.getEvenement(Mouse::Left,"CA"))
                    {
                        if(jeu->map.getMonstreIllumine()!=-1)
                        {
                            jeu->eventManager.StopEvenement(Mouse::Left,"CA");
                            jeu->hero.setMonstreVise(jeu->map.getMonstreIllumine());
                        }
                        else
                            jeu->hero.setMonstreVise(-1);
                    }
                    if(jeu->eventManager.getEvenement(Mouse::Left,"C")&&jeu->eventManager.getEvenement(Mouse::Left,"CA"))
                    {
                        jeu->hero.setSacVise(jeu->map.getSacPointe());

                        if(jeu->map.getSacPointe().x!=-1)
                            jeu->eventManager.StopEvenement(Mouse::Left,"CA");
                    }

                    if(jeu->hero.getSacVise().x!=-1)
                    {
                        if(jeu->hero.getSacVise().x==jeu->hero.m_personnage.getCoordonnee().x&&jeu->hero.getSacVise().y==jeu->hero.m_personnage.getCoordonnee().y)
                            jeu->hero.setChercherSac(jeu->hero.getSacVise());
                        else
                        {
                            coordonnee temp={-1,-1,-1,-1};
                            jeu->hero.setChercherSac(temp);
                        }
                    }
                    else
                    {
                        coordonnee temp={-1,-1,-1,-1};
                        jeu->hero.setChercherSac(temp);
                    }

                    if(jeu->eventManager.getEvenement(Mouse::Right,"C"))
                    {
                        if(jeu->eventManager.getEvenement(Mouse::Left,"C"))
                            jeu->eventManager.StopEvenement(Mouse::Left,"C");
                        else
                            jeu->hero.utiliserClicDroit(jeu->eventManager.getPositionSouris(),monstreVise);
                    }

                    if(jeu->eventManager.getEvenement(Key::I,"ET"))
                        jeu->eventManager.StopEvenement(Key::I,"ET"),jeu->map.m_defilerObjets=0,jeu->m_contexte=jeu->m_inventaire;

                    if(jeu->eventManager.getEvenement(Key::Escape,"ET"))
                        jeu->eventManager.StopEvenement(Key::Escape,"ET"),jeu->m_contexte=jeu->m_menuInGame;

                    if(alpha_sac>128)
                    {
                        if(jeu->eventManager.getEvenement(Mouse::Left,"C")&&jeu->eventManager.getPositionSouris().x>configuration->Resolution.w-configuration->Resolution.w*0.25&&jeu->eventManager.getPositionSouris().y>configuration->Resolution.w*0.265+10*20*configuration->Resolution.w/800+20*configuration->Resolution.w/800&&jeu->eventManager.getPositionSouris().y<configuration->Resolution.w*0.265+11*20*configuration->Resolution.w/800+20*configuration->Resolution.w/800)
                            jeu->map.m_defilerObjets++,jeu->eventManager.StopEvenement(Mouse::Left,"C");

                        if(jeu->eventManager.getEvenement(Mouse::Left,"C")&&jeu->eventManager.getPositionSouris().x>configuration->Resolution.w-configuration->Resolution.w*0.25&&jeu->eventManager.getPositionSouris().y>configuration->Resolution.w*0.265&&jeu->eventManager.getPositionSouris().y<configuration->Resolution.w*0.265+20*configuration->Resolution.w/800)
                            jeu->map.m_defilerObjets--,jeu->eventManager.StopEvenement(Mouse::Left,"C");
                    }

                    ///**********************************************************///
                    ///Affichage
                    ///**********************************************************///

                    moteurGraphique->Gerer(&jeu->ecran,tempsEcouleDepuisDernierAffichage,jeu->map.getDimensions().y);

                    jeu->map.Afficher(&jeu->ecran,&jeu->camera,1,&jeu->hero,jeu->eventManager.getPositionSouris(),jeu->eventManager.getEvenement(Key::LAlt,"ET"),alpha_map);//Affichage de la jeu->map

                    if(configuration->Minimap)
                    {
                        alpha_map+=tempsEcoule*500;
                        if(alpha_map>255)
                            alpha_map=255;
                    }
                    else
                    {
                        alpha_map-=tempsEcoule*500;
                        if(alpha_map<0)
                            alpha_map=0;
                    }
                    if(alpha_map>0)
                        jeu->menu.Afficher(&jeu->ecran,2,alpha_map,&jeu->hero.m_classe);//On affiche la mini-map

                    if(jeu->hero.getChercherSac().x!=-1&&jeu->map.getNombreObjets(jeu->hero.getChercherSac())>0)
                    {
                        alpha_sac+=tempsEcoule*1000;
                        if(alpha_sac>255)
                            alpha_sac=255;
                    }
                    else
                    {
                        alpha_sac-=tempsEcoule*1000;
                        if(alpha_sac<0)
                            alpha_sac=0;
                    }
                    if(alpha_sac>0)
                    {
                        jeu->menu.Afficher(&jeu->ecran,3,alpha_sac,&jeu->hero.m_classe);
                        jeu->map.Afficher(&jeu->ecran,&jeu->camera,3,&jeu->hero,jeu->eventManager.getPositionSouris(),0,alpha_sac);
                    }

                    jeu->menu.Afficher(&jeu->ecran,1,255,&jeu->hero.m_classe); // On affiche le hud
                    if(jeu->map.getEntiteMonstre(jeu->map.getMonstreIllumine())!=NULL)
                        jeu->menu.AfficherDynamique(&jeu->ecran,jeu->hero.m_caracteristiques,1,jeu->map.getEntiteMonstre(jeu->map.getMonstreIllumine())->getCaracteristique(),&jeu->hero.m_classe);
                    else
                        jeu->menu.AfficherDynamique(&jeu->ecran,jeu->hero.m_caracteristiques,0,jeu->hero.m_caracteristiques,&jeu->hero.m_classe);
                    jeu->eventManager.AfficherCurseur(&jeu->ecran); // On affiche le curseur de la souris

                    if(jeu->map.getEvenement(jeu->eventManager.getCasePointee())>=0)
                        jeu->map.AfficherNomEvenement(&jeu->ecran,jeu->eventManager.getCasePointee(),jeu->eventManager.getPositionSouris());

                    if(configuration->effetMort&&sf::PostFX::CanUsePostFX() == true&&configuration->postFX&&tempsEffetMort!=0)
                    {
                        moteurGraphique->EffectMort.SetParameter("offset", 0.003*configuration->effetMort/100*(0.6+tempsEffetMort/10));
                        moteurGraphique->EffectMort.SetParameter("color",1+0.5*configuration->effetMort/100*tempsEffetMort, 1-1*configuration->effetMort/100*tempsEffetMort, 1-1*configuration->effetMort/100*tempsEffetMort);
                    }

                    if(configuration->console)
                    {
                        moteurGraphique->AjouterTexte(&fps,17);
                        Version.SetY(20);
                        moteurGraphique->AjouterTexte(&Version,17);
                        Temps.SetY(40);
                        moteurGraphique->AjouterTexte(&Temps,17);
                        moteurGraphique->AjouterTexte(&TourBoucle,17);

                        if(configuration->console!=2)
                            console->Afficher(&jeu->ecran);
                    }

                    jeu->m_display=true;

                    tempsEcouleDepuisDernierAffichage=0;
                }

                ///**********************************************************///
                ///Gestion des FPS
                ///**********************************************************///

                nbrTourBoucle++;

                if(tempsNbrTourBoucle>1)
                {
                    sprintf(chaine,"Nbr Tour de boucle : %i",nbrTourBoucle);
                    TourBoucle.SetText(chaine);
                    nbrTourBoucle=0;

                    tempsNbrTourBoucle=0;
                }

                if(configuration->console&&jeu->ecran.GetFrameTime()!=0)
                    sprintf(chaine,"%i FPS",(int)( 1.f / jeu->ecran.GetFrameTime()));

                if(configuration->console)
                    if(tempsEcouleDepuisFPS>0.1)
                    {
                        //  Calcule du nombre de FPS

                        fps.SetText(chaine);


                        sprintf(chaine,"Temps : %i h %i ",configuration->heure,(int)configuration->minute);
                        Temps.SetText(chaine);

                        tempsEcouleDepuisFPS=0;
                    }
            }
}

