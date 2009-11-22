

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


#include "c_jeu.h"
#include "../jeu.h"

#include "../globale.h"

#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

#include <sstream>


using namespace sf;

void Sauvegarder(void* UserData)
{
    Jeu *jeu = static_cast<Jeu*>(UserData);

    jeu->map->Sauvegarder(&jeu->hero);
    jeu->hero.Sauvegarder();
    moteurGraphique->LightManager->GenerateWallShadow(moteurGraphique->m_angleOmbreSoleil,moteurGraphique->m_soleil);
}

c_Jeu::c_Jeu()
{
    continuer=true,lumiere=false,augmenter=false;
    tempsActuel=0,tempsPrecedent=0,tempsDepuisDerniereAnimation=0,tempsEcoule=0,tempsNbrTourBoucle=0,tempsEcouleDepuisDernierCalculLumiere=0.005,tempsEcouleDepuisDernierCalculOmbre=0,tempsEcouleDepuisDernierDeplacement=0,tempsEcouleDepuisDernierIA=0,tempsEcouleDepuisDernierAffichage=0,tempsEcouleDepuisFPS=0,tempsEffetMort=0,tempsSauvergarde=0;
    nbrTourBoucle=0;

    configuration->heure=(rand() % (24));
    configuration->minute=0;

    sf::Listener::SetGlobalVolume((float)configuration->volume);

    // Texte pour l'affichage des FPS
    fps.SetSize(16.f);

    Version.SetSize(16.f);
    Version.SetText("v "+configuration->version);
    Version.SetY(20);

    Temps.SetSize(16.f);
    Temps.SetY(40);

    TourBoucle.SetX(120);
    TourBoucle.SetSize(16.f);

    alpha_map=0;
    alpha_sac=0;
    alpha_dialog=0;
    lowFPS=-1;


    m_thread_sauvegarde = NULL;
}

void c_Jeu::Utiliser(Jeu *jeu)
{
    //Gestion du temps
    tempsEcoule = jeu->Clock.GetElapsedTime();
    if (tempsEcoule>0.1)
        tempsEcoule=0.1;


    jeu->m_display=false;
    GererTemps(jeu);
    if (tempsSauvergarde>=configuration->frequence_sauvegarde)
    {
        if (m_thread_sauvegarde)
        {
            m_thread_sauvegarde->Wait();
            delete m_thread_sauvegarde;
        }

        m_thread_sauvegarde = new sf::Thread(&Sauvegarder, jeu);
        m_thread_sauvegarde->Launch();
        tempsSauvergarde=0;
    }
    IA(jeu);
    Deplacements(jeu);
    Animation(jeu);
    Lumieres(jeu);

    if ((tempsEcouleDepuisDernierAffichage>0.012&&configuration->syncronisation_verticale)||(!configuration->syncronisation_verticale))
    {
        jeu->hero.PlacerCamera();

        Affichage(jeu);
        Evenements(jeu);
        tempsEcouleDepuisDernierAffichage=0;
    }
    FPS(jeu);
}


void c_Jeu::GererTemps(Jeu *jeu)
{
    tempsEcouleDepuisDernierDeplacement+=tempsEcoule;
    tempsDepuisDerniereAnimation+=tempsEcoule;
    tempsEcouleDepuisDernierAffichage+=tempsEcoule;
    tempsEcouleDepuisDernierIA+=tempsEcoule;
    tempsEcouleDepuisDernierCalculLumiere+=tempsEcoule;
    tempsSauvergarde+=tempsEcoule;
    configuration->minute+=tempsEcoule*0.1;
    tempsNbrTourBoucle+=tempsEcoule;

    if (configuration->minute>=60)
        configuration->minute=0,configuration->heure++;
    if (configuration->heure>23)
        configuration->heure=0;
    if (augmenter)
        tempsEffetMort+=tempsEcoule*10;
    else
        tempsEffetMort-=tempsEcoule;
    if (!jeu->hero.m_personnage.EnVie())
        tempsEffetMort=1;

    if (tempsEffetMort>1)
        augmenter=false;
    else if (tempsEffetMort<0)
        augmenter=true;
    tempsEcouleDepuisFPS+=tempsEcoule;

    jeu->hero.AugmenterAme(tempsEcoule);
    jeu->hero.RecalculerCaracteristiques();
    if (jeu->hero.m_personnage.EnVie())
    {
        jeu->hero.RegenererVie((float)jeu->hero.m_caracteristiques.maxVie*(float)(tempsEcoule/100));
        jeu->hero.RegenererFoi((float)jeu->hero.m_caracteristiques.maxFoi*(float)(tempsEcoule/50));
    }

    jeu->Clock.Reset();

    if (jeu->hero.m_personnage.EnVie())
    {
        if (jeu->hero.m_caracteristiques.maxVie!=0)
        {
            if (jeu->hero.m_caracteristiques.vie/(float)jeu->hero.m_caracteristiques.maxVie<0.5)
                configuration->effetMort=150-(jeu->hero.m_caracteristiques.vie*300/jeu->hero.m_caracteristiques.maxVie),jeu->sonMort.SetVolume(configuration->effetMort);
            else
                configuration->effetMort=0,jeu->sonMort.SetVolume(0);
        }
    }
    else
        configuration->effetMort=150;
}
void c_Jeu::IA(Jeu *jeu)
{
    if (tempsEcouleDepuisDernierIA>=0.027)
    {
        jeu->map->GererMonstres(jeu,&jeu->hero,tempsEcouleDepuisDernierIA,&jeu->menu);
        jeu->map->GererProjectilesEtEffets(&jeu->hero,tempsEcouleDepuisDernierIA);
        jeu->map->GererScript(jeu,&jeu->hero,tempsEcouleDepuisDernierIA,&jeu->menu);

        tempsEcouleDepuisDernierIA=0;
    }
}
void c_Jeu::Deplacements(Jeu *jeu)
{
    if (tempsEcouleDepuisDernierDeplacement >= 0.0)
    {
        coordonnee temp(-1 ,-1 ,-1 ,-1);
        if (jeu->hero.getMonstreVise()==-1)
            temp.x=jeu->hero.m_personnage.getCoordonnee().x,temp.y=jeu->hero.m_personnage.getCoordonnee().y;

        oldHeroPos = jeu->hero.m_personnage.getCoordonneePixel();

        if (jeu->hero.m_personnage.SeDeplacer(tempsEcouleDepuisDernierDeplacement*100,jeu->map->getDimensions(),
                                              (jeu->map->getCollisionPousse((int)((jeu->hero.m_personnage.getCoordonneePixel().x + jeu->hero.m_personnage.getPousse().x * tempsEcouleDepuisDernierDeplacement * COTE_TILE * 5 + COTE_TILE * 0.5 * (jeu->hero.m_personnage.getPousse().x >= 0))/COTE_TILE),
                                                                            (int)((jeu->hero.m_personnage.getCoordonneePixel().y + jeu->hero.m_personnage.getPousse().y * tempsEcouleDepuisDernierDeplacement * COTE_TILE * 5 + COTE_TILE * 0.5 * (jeu->hero.m_personnage.getPousse().y >= 0) )/COTE_TILE)) != 1)))
        {
            bool ok=true;
            if (jeu->hero.getMonstreVise()>-1)
                if (jeu->hero.TestMonstreVise(jeu->map->getEntiteMonstre(jeu->hero.getMonstreVise())))
                    ok=false;

            if (jeu->hero.m_personnage.getCoordonnee().x==jeu->hero.m_personnage.getArrivee().x && jeu->hero.m_personnage.getCoordonnee().y==jeu->hero.m_personnage.getArrivee().y)
                ok=false;

            if (ok)
                jeu->hero.m_personnage.Pathfinding(jeu->map->getAlentourDuPersonnage(jeu->hero.m_personnage.getCoordonnee()),temp); // Recherche du chemin

            if (eventManager->getEvenement(Mouse::Left,"CA")&&eventManager->getEvenement(Key::LShift,"ET"))
            {
                coordonnee temp((int)(eventManager->getCasePointee().x*COTE_TILE),(int)(eventManager->getCasePointee().y*COTE_TILE));
                jeu->hero.m_personnage.Frappe(jeu->hero.m_personnage.getCoordonneePixel(),temp);
            }
        }

        coordonneeDecimal old, heroPos;
        old.x = (oldHeroPos.x-oldHeroPos.y-1)*64;;
        old.y = (oldHeroPos.x+oldHeroPos.y)*32;

        heroPos.x = (jeu->hero.m_personnage.getCoordonneePixel().x-jeu->hero.m_personnage.getCoordonneePixel().y-1)*64;;
        heroPos.y = (jeu->hero.m_personnage.getCoordonneePixel().x+jeu->hero.m_personnage.getCoordonneePixel().y)*32;

        moteurGraphique->decalageReflection.x += heroPos.x - old.x;
        moteurGraphique->decalageReflection.y += heroPos.y - old.y;


        ///Placer l'écouteur, à la position du héro
        coordonnee position;
        position.x=(jeu->hero.m_personnage.getCoordonnee().x-jeu->hero.m_personnage.getCoordonnee().y-1)/5;
        position.y=(jeu->hero.m_personnage.getCoordonnee().x+jeu->hero.m_personnage.getCoordonnee().y)/5;

        Listener::SetGlobalVolume((float)configuration->volume);
        Listener::SetPosition(-position.x, 0, position.y);
        Listener::SetDirection(0, 0, 1);
        jeu->map->MusiquePlay(position);
        jeu->sonMort.SetPosition(position.x,0,position.y);

        tempsEcouleDepuisDernierDeplacement=0;
    }
}
void c_Jeu::Animation(Jeu *jeu)
{
    coordonnee positionHero;
    positionHero.x=(jeu->hero.m_personnage.getCoordonnee().x-jeu->hero.m_personnage.getCoordonnee().y-1)/5;
    positionHero.y=(jeu->hero.m_personnage.getCoordonnee().x+jeu->hero.m_personnage.getCoordonnee().y)/5;


    if (tempsDepuisDerniereAnimation >= 0.02)
    {
        jeu->map->TestEvenement(jeu,tempsDepuisDerniereAnimation); // On test les événement pour voir s'il on doit changer de jeu->map, faire des dégats au perso, le régénérer, etc

        if (tempsDepuisDerniereAnimation>0.12)
            tempsDepuisDerniereAnimation=0.12;


        int retour=-2;
        retour = jeu->hero.m_personnage.Animer(&jeu->hero.m_modelePersonnage[0],tempsDepuisDerniereAnimation,positionHero);

        jeu->hero.CalculerOrdreAffichage();

        jeu->hero.m_personnage.m_vientDeFrapper = NULL;
        jeu->hero.m_personnage.m_degatsInflige  = 0;

        if (retour==0) //Animation du héro
        {
            if (jeu->hero.m_personnage.frappeEnCours && !jeu->hero.m_personnage.m_lancementMiracleEnCours)
            {
                bool toucher=false;
                if (!jeu->hero.m_personnage.m_shooter)
                {
                    if (jeu->map->getEntiteMonstre(jeu->hero.getMonstreVise())!=NULL)
                        if (fabs(jeu->map->getEntiteMonstre(jeu->hero.getMonstreVise())->getCoordonnee().x-jeu->hero.m_personnage.getCoordonnee().x)<=2&&fabs(jeu->map->getEntiteMonstre(jeu->hero.getMonstreVise())->getCoordonnee().y-jeu->hero.m_personnage.getCoordonnee().y)<=2)
                            if (rand() % 100 < (float)((float)jeu->hero.m_caracteristiques.dexterite / ((float)jeu->map->getEntiteMonstre(jeu->hero.getMonstreVise())->getCaracteristique().dexterite+1))*75 )
                                if (!jeu->map->getEntiteMonstre(jeu->hero.getMonstreVise())->m_friendly)
                                {
                                    int degats = (rand()%(jeu->hero.m_caracteristiques.degatsMax - jeu->hero.m_caracteristiques.degatsMin+1))+jeu->hero.m_caracteristiques.degatsMin;
                                    toucher=true,jeu->map->InfligerDegats(jeu->hero.getMonstreVise(),degats,&jeu->hero,1);

                                    jeu->hero.m_personnage.m_vientDeFrapper = jeu->map->getEntiteMonstre(jeu->hero.getMonstreVise());
                                    jeu->hero.m_personnage.m_degatsInflige  = degats;

                                    jeu->map->getEntiteMonstre(jeu->hero.getMonstreVise())->m_vientDetreTouche = &jeu->hero.m_personnage;

                                    jeu->hero.m_personnage.InfligerDegats(-degats * jeu->hero.m_caracteristiques.volVie, NULL);
                                    jeu->hero.m_caracteristiques.foi += degats *jeu->hero.m_caracteristiques.volFoi;
                                }

                }
                else
                    toucher=true;

                if (toucher)
                    if (jeu->hero.AjouterMiracleArme())
                    {
                        coordonnee cible;

                        if (jeu->map->getEntiteMonstre(jeu->hero.getMonstreVise())!=NULL)
                            cible=jeu->map->getEntiteMonstre(jeu->hero.getMonstreVise())->getCoordonnee();
                        else
                            cible=eventManager->getCasePointee();

                        jeu->hero.m_personnage.m_miracleEnCours.back().m_infos.back()->m_cible   = jeu->map->getEntiteMonstre(jeu->hero.getMonstreVise());
                        jeu->hero.m_personnage.m_miracleEnCours.back().m_coordonneeCible        = cible;
                        //jeu->map->GererMiracle(&jeu->hero.m_personnage.m_miracleEnCours.back(),&jeu->hero.m_classe.miracles[jeu->hero.m_personnage.m_miracleEnCours.back().m_modele],&jeu->hero,0,jeu->hero.m_personnage.getCoordonnee(),cible,1);
                    }
            }
            jeu->hero.miracleEnCours=0;
        }

        if (retour==2)
            if (!eventManager->getEvenement(Mouse::Left,"C") || !jeu->map->getMonstreEnVie(jeu->hero.getMonstreVise()) )
                jeu->hero.setMonstreVise(-1),jeu->hero.m_personnage.frappeEnCours=false,jeu->hero.m_personnage.setEtat(0);

        if (retour==1)
        {
            if (!eventManager->getEvenement(Mouse::Left,"C") || !jeu->map->getMonstreEnVie(jeu->hero.getMonstreVise()))
                jeu->hero.setMonstreVise(-1),jeu->hero.m_personnage.frappeEnCours=false,jeu->hero.m_personnage.setEtat(0);
            if (jeu->map->getEntiteMonstre(jeu->hero.getMonstreVise()) != NULL)
                if (fabs(jeu->map->getEntiteMonstre(jeu->hero.getMonstreVise())->getCoordonnee().x - jeu->hero.m_personnage.getCoordonnee().x) > 1
                        || fabs(jeu->map->getEntiteMonstre(jeu->hero.getMonstreVise())->getCoordonnee().y - jeu->hero.m_personnage.getCoordonnee().y) > 1)
                    jeu->hero.m_personnage.frappeEnCours=false,jeu->hero.m_personnage.setEtat(0);
        }

        if(!jeu->hero.m_personnage.frappeEnCours)
            jeu->hero.m_personnage.m_lancementMiracleEnCours = false;


        jeu->map->Animer(&jeu->hero,tempsDepuisDerniereAnimation,&jeu->menu); // Animation des tiles de la jeu->map

        tempsDepuisDerniereAnimation=0;
    }

    jeu->map->GererMiracle(&jeu->hero.m_personnage,jeu->hero.m_classe.miracles,tempsEcoule,positionHero,&jeu->hero);

}
void c_Jeu::Lumieres(Jeu *jeu)
{
    if (configuration->Lumiere&&tempsEcouleDepuisDernierCalculLumiere>configuration->frequence_lumiere/2&&configuration->RafraichirOmbre==0)
        configuration->RafraichirOmbre=1;

    if (configuration->Lumiere&&tempsEcouleDepuisDernierCalculLumiere>configuration->frequence_lumiere)
    {
        if (configuration->RafraichirOmbre==2)
            configuration->RafraichirOmbre=0;

        jeu->map->CalculerOmbresEtLumieres();

        configuration->RafraichirLumiere=true;
        tempsEcouleDepuisDernierCalculLumiere=0;

        moteurGraphique->LightManager->Generate(jeu->hero.m_personnage.m_light);
    }
}


void GestionRaccourcisObjets(Jeu *jeu)
{
    if (eventManager->getEvenement('1',"ET")
            || eventManager->getEvenement(Mouse::Left,"CA")
            && eventManager->getPositionSouris().x > 255 * configuration->Resolution.w/800
            && eventManager->getPositionSouris().x < 275 * configuration->Resolution.w/800
            && eventManager->getPositionSouris().y > 492 * configuration->Resolution.y/600
            && eventManager->getPositionSouris().y < 512 * configuration->Resolution.y/600)
    {
        if(eventManager->getEvenement('1',"ET"))
            eventManager->StopEvenement('1',"ET");
        else
            eventManager->StopEvenement(Mouse::Left,"CA");

        jeu->hero.UtiliserObjet(jeu->hero.m_objets_raccourcis[0]);
           //jeu->map->GererMiracle(&jeu->hero.m_personnage.m_miracleEnCours.back(),&jeu->hero.m_classe.miracles[jeu->hero.m_personnage.m_miracleEnCours.back().m_modele],&jeu->hero,0,jeu->hero.m_personnage.getCoordonnee(),eventManager->getCasePointee(),1);
    }
    if (eventManager->getEvenement('2',"ET")
            || eventManager->getEvenement(Mouse::Left,"CA")
            && eventManager->getPositionSouris().x > 287 * configuration->Resolution.w/800
            && eventManager->getPositionSouris().x < 307 * configuration->Resolution.w/800
            && eventManager->getPositionSouris().y > 492 * configuration->Resolution.y/600
            && eventManager->getPositionSouris().y < 512 * configuration->Resolution.y/600)
    {
        if(eventManager->getEvenement('2',"ET"))
            eventManager->StopEvenement('2',"ET");
        else
            eventManager->StopEvenement(Mouse::Left,"CA");
        jeu->hero.UtiliserObjet(jeu->hero.m_objets_raccourcis[1]);
          //  jeu->map->GererMiracle(&jeu->hero.m_personnage.m_miracleEnCours.back(),&jeu->hero.m_classe.miracles[jeu->hero.m_personnage.m_miracleEnCours.back().m_modele],&jeu->hero,0,jeu->hero.m_personnage.getCoordonnee(),eventManager->getCasePointee(),1);
    }
    if (eventManager->getEvenement('3',"ET")
            || eventManager->getEvenement(Mouse::Left,"CA")
            && eventManager->getPositionSouris().x > 319 * configuration->Resolution.w/800
            && eventManager->getPositionSouris().x < 339 * configuration->Resolution.w/800
            && eventManager->getPositionSouris().y > 492 * configuration->Resolution.y/600
            && eventManager->getPositionSouris().y < 512 * configuration->Resolution.y/600)
    {
        if(eventManager->getEvenement('3',"ET"))
            eventManager->StopEvenement('3',"ET");
        else
            eventManager->StopEvenement(Mouse::Left,"CA");
        jeu->hero.UtiliserObjet(jeu->hero.m_objets_raccourcis[2]);
          //  jeu->map->GererMiracle(&jeu->hero.m_personnage.m_miracleEnCours.back(),&jeu->hero.m_classe.miracles[jeu->hero.m_personnage.m_miracleEnCours.back().m_modele],&jeu->hero,0,jeu->hero.m_personnage.getCoordonnee(),eventManager->getCasePointee(),1);
    }
    if (eventManager->getEvenement('4',"ET")
            || eventManager->getEvenement(Mouse::Left,"CA")
            && eventManager->getPositionSouris().x > 351 * configuration->Resolution.w/800
            && eventManager->getPositionSouris().x < 371 * configuration->Resolution.w/800
            && eventManager->getPositionSouris().y > 492 * configuration->Resolution.y/600
            && eventManager->getPositionSouris().y < 512 * configuration->Resolution.y/600)
    {
        if(eventManager->getEvenement('4',"ET"))
            eventManager->StopEvenement('4',"ET");
        else
            eventManager->StopEvenement(Mouse::Left,"CA");
        jeu->hero.UtiliserObjet(jeu->hero.m_objets_raccourcis[3]);
          //  jeu->map->GererMiracle(&jeu->hero.m_personnage.m_miracleEnCours.back(),&jeu->hero.m_classe.miracles[jeu->hero.m_personnage.m_miracleEnCours.back().m_modele],&jeu->hero,0,jeu->hero.m_personnage.getCoordonnee(),eventManager->getCasePointee(),1);
    }
}

void GestionRaccourcisMiracles(Jeu *jeu)
{
    if (eventManager->getEvenement(Key::F1,"ET")
            || eventManager->getEvenement(Mouse::Left,"CA")
            && eventManager->getPositionSouris().x > 432 * configuration->Resolution.w/800
            && eventManager->getPositionSouris().x < 452 * configuration->Resolution.w/800
            && eventManager->getPositionSouris().y > 492 * configuration->Resolution.h/600
            && eventManager->getPositionSouris().y < 512 * configuration->Resolution.h/600)
    {
        if(eventManager->getEvenement(Key::F1,"ET"))
            eventManager->StopEvenement(Key::F1,"ET");
        else
            eventManager->StopEvenement(Mouse::Left,"CA");
        jeu->hero.m_personnage.m_miracleALancer = jeu->hero.m_miracles_raccourcis[0];
    }
    if (eventManager->getEvenement(Key::F2,"ET")
            || eventManager->getEvenement(Mouse::Left,"CA")
            && eventManager->getPositionSouris().x > 464 * configuration->Resolution.w/800
            && eventManager->getPositionSouris().x < 484 * configuration->Resolution.w/800
            && eventManager->getPositionSouris().y > 492 * configuration->Resolution.y/600
            && eventManager->getPositionSouris().y < 512 * configuration->Resolution.y/600)
    {
        if(eventManager->getEvenement(Key::F2,"ET"))
            eventManager->StopEvenement(Key::F2,"ET");
        else
            eventManager->StopEvenement(Mouse::Left,"CA");
        jeu->hero.m_personnage.m_miracleALancer = jeu->hero.m_miracles_raccourcis[1];
    }
    if (eventManager->getEvenement(Key::F3,"ET")
            || eventManager->getEvenement(Mouse::Left,"CA")
            && eventManager->getPositionSouris().x > 496 * configuration->Resolution.w/800
            && eventManager->getPositionSouris().x < 516 * configuration->Resolution.w/800
            && eventManager->getPositionSouris().y > 492 * configuration->Resolution.y/600
            && eventManager->getPositionSouris().y < 512 * configuration->Resolution.y/600)
    {
        if(eventManager->getEvenement(Key::F3,"ET"))
            eventManager->StopEvenement(Key::F3,"ET");
        else
            eventManager->StopEvenement(Mouse::Left,"C");
        jeu->hero.m_personnage.m_miracleALancer = jeu->hero.m_miracles_raccourcis[2];
    }
    if (eventManager->getEvenement(Key::F4,"ET")
            || eventManager->getEvenement(Mouse::Left,"CA")
            && eventManager->getPositionSouris().x > 528 * configuration->Resolution.w/800
            && eventManager->getPositionSouris().x < 548 * configuration->Resolution.w/800
            && eventManager->getPositionSouris().y > 492 * configuration->Resolution.y/600
            && eventManager->getPositionSouris().y < 512 * configuration->Resolution.y/600)
    {
        if(eventManager->getEvenement(Key::F4,"ET"))
            eventManager->StopEvenement(Key::F4,"ET");
        else
            eventManager->StopEvenement(Mouse::Left,"CA");
        jeu->hero.m_personnage.m_miracleALancer = jeu->hero.m_miracles_raccourcis[3];
    }
}

int GestionBoutons(Jeu *jeu)
{
    if (eventManager->getEvenement(Key::M,"ET")
        || eventManager->getPositionSouris().x > 264 * configuration->Resolution.x/800
        && eventManager->getPositionSouris().x < 283 * configuration->Resolution.x/800
        && eventManager->getPositionSouris().y > 533 * configuration->Resolution.y/600
        && eventManager->getPositionSouris().y < 552 * configuration->Resolution.y/600)
    {
        if(!eventManager->getEvenement(Key::M,"ET"))
         moteurGraphique->AjouterTexte(configuration->getText(0,13),coordonnee(eventManager->getPositionSouris().x,
                                      eventManager->getPositionSouris().y - 20),
                                      19,0,12,sf::Color(224,224,224),1);

        if (eventManager->getEvenement(Key::M,"ET")
         || eventManager->getEvenement(Mouse::Left,"C"))
        {
            if(eventManager->getEvenement(Key::M,"ET"))
                eventManager->StopEvenement(Key::M,"ET");
            else
                eventManager->StopEvenement(Mouse::Left,"C");

            if (!configuration->Minimap)
                configuration->Minimap=true;
            else
                configuration->Minimap=false;
        }
    }
    if(configuration->Minimap)
    {
        sf::Sprite buf;
        buf.SetImage(*moteurGraphique->getImage(0));
        buf.SetPosition(264 * configuration->Resolution.x/800, 533 * configuration->Resolution.y/600);
        buf.Resize(20 * configuration->Resolution.x/800,20 * configuration->Resolution.y/600);
        buf.SetColor(sf::Color(0,0,0,128));
        moteurGraphique->AjouterCommande(&buf, 18, 0);
    }

    if (eventManager->getEvenement(Key::I,"ET")
        || eventManager->getPositionSouris().x > 302 * configuration->Resolution.x/800
        && eventManager->getPositionSouris().x < 321 * configuration->Resolution.x/800
        && eventManager->getPositionSouris().y > 533 * configuration->Resolution.y/600
        && eventManager->getPositionSouris().y < 552 * configuration->Resolution.y/600)
    {
        if(!eventManager->getEvenement(Key::I,"ET"))
        moteurGraphique->AjouterTexte(configuration->getText(0,14),coordonnee(eventManager->getPositionSouris().x,
                                      eventManager->getPositionSouris().y - 20),
                                      19,0,12,sf::Color(224,224,224),1);

        if (eventManager->getEvenement(Key::I,"ET")
                || eventManager->getEvenement(Mouse::Left,"CA"))
        {
            eventManager->StopEvenement(Key::I,"ET");
            eventManager->StopEvenement(Mouse::Left,"CA");
            jeu->map->m_defilerObjets=0;

            return 2;
        }
    }
    if(jeu->next_screen == 2)
    {
        sf::Sprite buf;
        buf.SetImage(*moteurGraphique->getImage(0));
        buf.SetPosition(302 * configuration->Resolution.x/800, 533 * configuration->Resolution.y/600);
        buf.Resize(20 * configuration->Resolution.x/800,20 * configuration->Resolution.y/600);
        buf.SetColor(sf::Color(0,0,0,128));
        moteurGraphique->AjouterCommande(&buf, 18, 0);
    }

    if (eventManager->getEvenement(Key::Q,"ET")
        || eventManager->getPositionSouris().x > 264 * configuration->Resolution.x/800
        && eventManager->getPositionSouris().x < 283 * configuration->Resolution.x/800
        && eventManager->getPositionSouris().y > 565 * configuration->Resolution.y/600
        && eventManager->getPositionSouris().y < 584 * configuration->Resolution.y/600)
    {
        if(!eventManager->getEvenement(Key::Q,"ET"))
        moteurGraphique->AjouterTexte(configuration->getText(0,15),coordonnee(eventManager->getPositionSouris().x,
                                      eventManager->getPositionSouris().y - 20),
                                      19,0,12,sf::Color(224,224,224),1);

        if (eventManager->getEvenement(Key::Q,"ET")
         || eventManager->getEvenement(Mouse::Left,"CA"))
        {
            eventManager->StopEvenement(Key::Q,"ET");
            eventManager->StopEvenement(Mouse::Left,"CA");

            return 6;
        }
    }
    if(jeu->next_screen == 6)
    {
        sf::Sprite buf;
        buf.SetImage(*moteurGraphique->getImage(0));
        buf.SetPosition(264 * configuration->Resolution.x/800, 565 * configuration->Resolution.y/600);
        buf.Resize(20 * configuration->Resolution.x/800,20 * configuration->Resolution.y/600);
        buf.SetColor(sf::Color(0,0,0,128));
        moteurGraphique->AjouterCommande(&buf, 18, 0);
    }

    if (eventManager->getEvenement(Key::T,"ET")
        || eventManager->getPositionSouris().x > 302 * configuration->Resolution.x/800
        && eventManager->getPositionSouris().x < 321 * configuration->Resolution.x/800
        && eventManager->getPositionSouris().y > 565 * configuration->Resolution.y/600
        && eventManager->getPositionSouris().y < 584 * configuration->Resolution.y/600)
    {
        if(!eventManager->getEvenement(Key::T,"ET"))
        moteurGraphique->AjouterTexte(configuration->getText(0,16),coordonnee(eventManager->getPositionSouris().x,
                                      eventManager->getPositionSouris().y - 20),
                                      19,0,12,sf::Color(224,224,224),1);

        if (eventManager->getEvenement(Key::T,"ET")
         || eventManager->getEvenement(Mouse::Left,"CA"))
        {
            eventManager->StopEvenement(Key::T,"ET");
            eventManager->StopEvenement(Mouse::Left,"CA");

            return 5;
        }
    }
    if(jeu->next_screen == 5)
    {
        sf::Sprite buf;
        buf.SetImage(*moteurGraphique->getImage(0));
        buf.SetPosition(302 * configuration->Resolution.x/800, 565 * configuration->Resolution.y/600);
        buf.Resize(20 * configuration->Resolution.x/800,20 * configuration->Resolution.y/600);
        buf.SetColor(sf::Color(0,0,0,128));
        moteurGraphique->AjouterCommande(&buf, 18, 0);
    }

    if (eventManager->getEvenement(Key::Escape,"ET")
     || eventManager->getPositionSouris().x > 333 * configuration->Resolution.x/800
     && eventManager->getPositionSouris().x < 352 * configuration->Resolution.x/800
     && eventManager->getPositionSouris().y > 577 * configuration->Resolution.y/600
     && eventManager->getPositionSouris().y < 596 * configuration->Resolution.y/600)
    {
        if(!eventManager->getEvenement(Key::Escape,"ET"))
        moteurGraphique->AjouterTexte(configuration->getText(0,17),coordonnee(eventManager->getPositionSouris().x,
                                      eventManager->getPositionSouris().y - 20),
                                      19,0,12,sf::Color(224,224,224),1);

        if (eventManager->getEvenement(Key::Escape,"ET")
         || eventManager->getEvenement(Mouse::Left,"CA"))
        {

            eventManager->StopEvenement(Key::Escape,"ET");
            eventManager->StopEvenement(Mouse::Left,"CA");
            return 4;
        }
    }
    if(jeu->next_screen == 4)
    {
        sf::Sprite buf;
        buf.SetImage(*moteurGraphique->getImage(0));
        buf.SetPosition(333 * configuration->Resolution.x/800, 577 * configuration->Resolution.y/600);
        buf.Resize(20 * configuration->Resolution.x/800,20 * configuration->Resolution.y/600);
        buf.SetColor(sf::Color(0,0,0,128));
        moteurGraphique->AjouterCommande(&buf, 18, 0);
    }

    return -1;
}


void c_Jeu::Evenements(Jeu *jeu)
{
    if (eventManager->getPositionSouris().y < 492 * configuration->Resolution.h/600)
    {
        if (!eventManager->getEvenement(Mouse::Left,"C"))
            jeu->map->getMonstre(&jeu->hero,eventManager->getPositionSouris(),eventManager->getCasePointee());

        if (eventManager->getEvenement(Key::LAlt,"ET"))
            jeu->map->m_monstreIllumine = -1;

        if(jeu->map->getEntiteMonstre(jeu->map->m_monstreIllumine) != NULL)
            if(!jeu->map->getEntiteMonstre(jeu->map->m_monstreIllumine)->EnVie())
                jeu->map->m_monstreIllumine = -1, eventManager->StopEvenement(Mouse::Left,"C");

        if (eventManager->getEvenement(Mouse::Left,"C")&&eventManager->getEvenement(Mouse::Left,"CA"))
        {
            jeu->hero.StopMiraclesFrappe();
            if (jeu->map->getMonstreIllumine()!=-1)
            {
                bool test=false;
                if (jeu->hero.getMonstreVise()>-1&&jeu->hero.m_personnage.getArrivee().x==jeu->hero.m_personnage.getCoordonnee().x&&jeu->hero.m_personnage.getArrivee().y==jeu->hero.m_personnage.getCoordonnee().y)
                    test=true;

                eventManager->StopEvenement(Mouse::Left,"CA");
                jeu->hero.setMonstreVise(jeu->map->getMonstreIllumine());
                if (test)
                    jeu->hero.TestMonstreVise(jeu->map->getEntiteMonstre(jeu->hero.getMonstreVise()));
            }
            else
                jeu->hero.setMonstreVise(-1);
        }


        if (eventManager->getEvenement(Mouse::Left,"C")&&!eventManager->getEvenement(Key::LShift,"ET"))
        {
            jeu->hero.StopMiraclesFrappe();
            if (!(eventManager->getPositionSouris().x>configuration->Resolution.w-configuration->Resolution.w*0.25
                    &&eventManager->getPositionSouris().y>configuration->Resolution.w*0.25&&eventManager->getPositionSouris().y<configuration->Resolution.w*0.25+configuration->Resolution.w*0.34
                    &&alpha_sac>=128)||alpha_sac<=128)
            {
                if (jeu->hero.getMonstreVise()==-1)
                {
                    if(jeu->menu.m_dialogue == " ")
                        jeu->hero.m_personnage.setArrivee(eventManager->getCasePointee());
                    else
                        eventManager->StopEvenement(Mouse::Left,"C");

                    jeu->menu.m_dialogue = " ";

                    jeu->hero.setSacVise(coordonnee (-1,-1,-1,-1));
                }
            }
            else if (jeu->hero.getChercherSac().x!=-1&&jeu->map->getObjetPointe()!=-1&&jeu->map->getNombreObjets(jeu->hero.getChercherSac())>4)
            {
                jeu->map->RamasserObjet(&jeu->hero);
                eventManager->StopEvenement(Mouse::Left,"CA");
                eventManager->StopEvenement(Mouse::Left,"C");
            }

            eventManager->StopEvenement(Mouse::Left,"CA");
        }

        if (eventManager->getEvenement(Mouse::Left,"C")/*&&eventManager->getEvenement(Mouse::Left,"CA")*/)
        {
            jeu->hero.setSacVise(jeu->map->getSacPointe());

            jeu->hero.m_objetVise=jeu->map->m_objetPointe;

            if (jeu->map->getSacPointe().x!=-1)
                eventManager->StopEvenement(Mouse::Left,"C"),jeu->hero.m_personnage.setArrivee(jeu->map->getSacPointe());
        }

        if (jeu->hero.getSacVise().x!=-1)
        {
            if (jeu->hero.getSacVise().x==jeu->hero.m_personnage.getCoordonnee().x&&jeu->hero.getSacVise().y==jeu->hero.m_personnage.getCoordonnee().y)
                jeu->hero.setChercherSac(jeu->hero.getSacVise());
            else
                jeu->hero.setChercherSac(coordonnee (-1,-1,-1,-1));
        }
        else
            jeu->hero.setChercherSac(coordonnee (-1,-1,-1,-1));

        if (eventManager->getEvenement(Mouse::Right,"C"))
        {
            if (eventManager->getEvenement(Mouse::Left,"C"))
                eventManager->StopEvenement(Mouse::Left,"C");
            else
            {
                if (!jeu->hero.m_personnage.frappeEnCours)
                {
                    eventManager->StopEvenement(Mouse::Right,"C");

                    jeu->hero.StopMiraclesFrappe();

                    coordonnee cible;

                    if (jeu->map->getEntiteMonstre(jeu->map->getMonstreIllumine())!=NULL)
                        cible = jeu->map->getEntiteMonstre(jeu->map->getMonstreIllumine())->getProchaineCase();
                    else
                        cible = eventManager->getCasePointee();

                    if (jeu->hero.UtiliserMiracle(jeu->hero.m_personnage.m_miracleALancer, jeu->map->getEntiteMonstre(jeu->map->getMonstreIllumine()), cible))
                    {
                        jeu->hero.m_personnage.m_miracleEnCours.back().m_infos.back()->m_cible = jeu->map->getEntiteMonstre(jeu->map->getMonstreIllumine());

                        coordonnee positionHero;
                        positionHero.x=(jeu->hero.m_personnage.getCoordonnee().x-jeu->hero.m_personnage.getCoordonnee().y-1)/5;
                        positionHero.y=(jeu->hero.m_personnage.getCoordonnee().x+jeu->hero.m_personnage.getCoordonnee().y)/5;

                       // jeu->map->GererMiracle(&jeu->hero.m_personnage.m_miracleEnCours.back(),&jeu->hero.m_classe.miracles[jeu->hero.m_personnage.m_miracleEnCours.back().m_modele],&jeu->hero,0,jeu->hero.m_personnage.getCoordonnee(),cible,1);
                        //jeu->map->GererMiracle(&jeu->hero.m_personnage,jeu->hero.m_classe.miracles,tempsDepuisDerniereAnimation,positionHero,&jeu->hero);
                    }
                    else
                        jeu->hero.m_personnage.setArrivee(eventManager->getCasePointee());
                }
            }
        }

    }

    /*if (alpha_sac>128)
    {
        if (eventManager->getEvenement(Mouse::Left,"C")&&eventManager->getPositionSouris().x>configuration->Resolution.w-configuration->Resolution.w*0.25&&eventManager->getPositionSouris().y>configuration->Resolution.w*0.265+10*20*configuration->Resolution.w/800+20*configuration->Resolution.w/800&&eventManager->getPositionSouris().y<configuration->Resolution.w*0.265+11*20*configuration->Resolution.w/800+20*configuration->Resolution.w/800)
            jeu->map->m_defilerObjets++,eventManager->StopEvenement(Mouse::Left,"C");

        if (eventManager->getEvenement(Mouse::Left,"C")&&eventManager->getPositionSouris().x>configuration->Resolution.w-configuration->Resolution.w*0.25&&eventManager->getPositionSouris().y>configuration->Resolution.w*0.265&&eventManager->getPositionSouris().y<configuration->Resolution.w*0.265+20*configuration->Resolution.w/800)
            jeu->map->m_defilerObjets--,eventManager->StopEvenement(Mouse::Left,"C");
    }*/
    GestionRaccourcisObjets(jeu);
    GestionRaccourcisMiracles(jeu);
    jeu->next_screen = GestionBoutons(jeu);

    if (jeu->next_screen >=0 )
        jeu->Next();


}


void c_Jeu::Affichage(Jeu *jeu)
{
    moteurGraphique->Gerer(tempsEcouleDepuisDernierAffichage,jeu->map->getDimensions().y);

    jeu->map->Afficher(&jeu->hero,eventManager->getEvenement(Key::LAlt,"ET"),alpha_map);//Affichage de la jeu->map

    jeu->hero.AfficherAmis();

    jeu->hero.AfficherFlecheQuetes(jeu->map->getNom(), tempsEcoule);

    if (configuration->Minimap)
    {
        alpha_map+=tempsEcoule*500;
        if (alpha_map>255)
            alpha_map=255;
    }
    else
    {
        alpha_map-=tempsEcoule*500;
        if (alpha_map<0)
            alpha_map=0;
    }
    if (alpha_map>0)
        jeu->menu.Afficher(2,alpha_map,&jeu->hero.m_classe);//On affiche la mini-map
    if (alpha_dialog>0)
        jeu->menu.Afficher(4,alpha_dialog,&jeu->hero.m_classe);//On affiche la mini-map


    if (jeu->hero.getChercherSac().x!=-1&&jeu->map->getNombreObjets(jeu->hero.getChercherSac())>4)
    {
        alpha_sac+=tempsEcoule*1000;
        if (alpha_sac>255)
            alpha_sac=255;
    }
    else
    {
        alpha_sac-=tempsEcoule*1000;
        if (alpha_sac<0)
            alpha_sac=0;
    }

    if (jeu->menu.m_dialogue != " ")
    {
        alpha_dialog+=tempsEcoule*1000;
        if (alpha_dialog>255)
            alpha_dialog=255;
    }
    else
    {
        alpha_dialog-=tempsEcoule*1000;
        if (alpha_dialog<0)
            alpha_dialog=0;
    }

    if (jeu->hero.getChercherSac().x!=-1&&alpha_sac<128&&jeu->hero.m_objetVise>=0)
    {
        jeu->map->m_objetPointe=jeu->hero.m_objetVise;
        jeu->map->RamasserObjet(&jeu->hero);
        jeu->map->m_objetPointe=-1;
        jeu->hero.setChercherSac(coordonnee (-1,-1,-1,-1));
        jeu->hero.setSacVise(coordonnee (-1,-1,-1,-1));
    }

    if (alpha_sac>0)
    {
        jeu->menu.Afficher(3,alpha_sac,&jeu->hero.m_classe);
        jeu->map->AfficherSac(jeu->hero.getChercherSac(),0,
                              coordonnee (600,(int)((float)configuration->Resolution.w*0.265),200,10),
                              jeu->hero.m_caracteristiques);
    }

    jeu->menu.Afficher(1,255,&jeu->hero.m_classe); // On affiche le hud

    if (jeu->map->getEntiteMonstre(jeu->map->getMonstreIllumine())!=NULL)
        jeu->menu.AfficherDynamique(jeu->hero.m_caracteristiques,1,jeu->map->getEntiteMonstre(jeu->map->getMonstreIllumine())->getCaracteristique(),&jeu->hero.m_classe);
    else
        jeu->menu.AfficherDynamique(jeu->hero.m_caracteristiques,0,jeu->hero.m_caracteristiques,&jeu->hero.m_classe);
    eventManager->AfficherCurseur(); // On affiche le curseur de la souris

    if (jeu->map->getEvenement(eventManager->getCasePointee())>=0)
        jeu->map->AfficherNomEvenement(eventManager->getCasePointee(),eventManager->getPositionSouris());

    if (configuration->effetMort/*&& sf::Shader::CanUseShader() == true*/&&configuration->postFX&&tempsEffetMort!=0)
    {
        moteurGraphique->EffectMort.SetParameter("offset", 0.003*configuration->effetMort/100*(0.6+tempsEffetMort/10));
        moteurGraphique->EffectMort.SetParameter("color",1+0.5*configuration->effetMort/100*tempsEffetMort, 1-1*configuration->effetMort/100*tempsEffetMort, 1-1*configuration->effetMort/100*tempsEffetMort);
    }

    if (configuration->console)
    {
        moteurGraphique->AjouterTexte(&fps,17);
        Version.SetY(20);
        moteurGraphique->AjouterTexte(&Version,17);
        Temps.SetY(40);
        moteurGraphique->AjouterTexte(&Temps,17);
        moteurGraphique->AjouterTexte(&TourBoucle,17);

        if (configuration->console!=2)
            console->Afficher();
    }

    if (lowFPS==-1&&moteurGraphique->GetFPS()>0)
        lowFPS=moteurGraphique->GetFPS();
    if (lowFPS>moteurGraphique->GetFPS() &&moteurGraphique->GetFPS()>0 )
        lowFPS=moteurGraphique->GetFPS();

    if (configuration->console)
        if (tempsEcouleDepuisFPS>1)
        {
            {
                std::ostringstream  buf;
                if (configuration->console&&moteurGraphique->GetFPS()!=0)
                    buf<<moteurGraphique->GetFPS()<<" / "<<(int)lowFPS<<" FPS";
                fps.SetText(buf.str());
            }

            {
                std::ostringstream  buf;

                buf<<"Temps : "<<configuration->heure<<" h "<<(int)configuration->minute;
                Temps.SetText(buf.str());
            }

            tempsEcouleDepuisFPS=0;
        }

    jeu->m_display=true;
}
void c_Jeu::FPS(Jeu *jeu)
{
    nbrTourBoucle++;

    if (tempsNbrTourBoucle>1)
    {
        lowFPS=-1;
        std::ostringstream  buf;
        buf<<"Nbr Tour de boucle : "<<nbrTourBoucle;
        TourBoucle.SetText(buf.str());
        nbrTourBoucle=0;

        tempsNbrTourBoucle=0;
    }
}

