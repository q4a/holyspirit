

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
}

c_Jeu::c_Jeu()
{
    continuer=true,lumiere=false,augmenter=false;
    tempsActuel=0,tempsPrecedent=0,tempsDepuisDerniereAnimation=0,tempsEcoule=0,tempsNbrTourBoucle=0,tempsEcouleDepuisDernierCalculLumiere=0,tempsEcouleDepuisDernierCalculOmbre=0,tempsEcouleDepuisDernierDeplacement=0,tempsEcouleDepuisDernierIA=0,tempsEcouleDepuisDernierAffichage=0,tempsEcouleDepuisFPS=0,tempsEffetMort=0,tempsSauvergarde=0;
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

    if ((tempsEcouleDepuisDernierAffichage>0.02&&configuration->syncronisation_verticale)||(!configuration->syncronisation_verticale))
    {
        jeu->hero.PlacerCamera();

        Evenements(jeu);
        Affichage(jeu);
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
    configuration->minute+=tempsEcoule;
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

        tempsEcouleDepuisDernierIA=0;
    }
}
void c_Jeu::Deplacements(Jeu *jeu)
{
    if (tempsEcouleDepuisDernierDeplacement>=0.01)
    {
        coordonnee temp(-1 ,-1 ,-1 ,-1);
        if (jeu->hero.getMonstreVise()==-1)
            temp.x=jeu->hero.m_personnage.getCoordonnee().x,temp.y=jeu->hero.m_personnage.getCoordonnee().y;

        if (jeu->hero.m_personnage.SeDeplacer(tempsEcouleDepuisDernierDeplacement*100,jeu->map->getDimensions(),
                                              (jeu->map->getTypeCase((int)((jeu->hero.m_personnage.getCoordonneePixel().x + jeu->hero.m_personnage.getPousse().x * tempsEcouleDepuisDernierDeplacement * COTE_TILE * 5 + COTE_TILE * 0.5)/COTE_TILE),
                                                                     (int)((jeu->hero.m_personnage.getCoordonneePixel().y + jeu->hero.m_personnage.getPousse().y * tempsEcouleDepuisDernierDeplacement * COTE_TILE * 5 + COTE_TILE * 0.5)/COTE_TILE)) != 1)))
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
    if (tempsDepuisDerniereAnimation >= 0.02)
    {
        jeu->map->TestEvenement(jeu,tempsDepuisDerniereAnimation); // On test les événement pour voir s'il on doit changer de jeu->map, faire des dégats au perso, le régénérer, etc

        if (tempsDepuisDerniereAnimation>0.12)
            tempsDepuisDerniereAnimation=0.12;

        coordonnee positionHero;
        positionHero.x=(jeu->hero.m_personnage.getCoordonnee().x-jeu->hero.m_personnage.getCoordonnee().y-1)/5;
        positionHero.y=(jeu->hero.m_personnage.getCoordonnee().x+jeu->hero.m_personnage.getCoordonnee().y)/5;

        int retour=-2;
        retour = jeu->hero.m_personnage.Animer(&jeu->hero.m_modelePersonnage[0],tempsDepuisDerniereAnimation,positionHero);

        jeu->hero.CalculerOrdreAffichage();

        if (retour==0) //Animation du héro
        {
            if (jeu->hero.m_personnage.frappeEnCours)
            {
                bool toucher=false;
                if (!jeu->hero.m_personnage.m_shooter)
                {
                    if (jeu->map->getEntiteMonstre(jeu->hero.getMonstreVise())!=NULL)
                        if (fabs(jeu->map->getEntiteMonstre(jeu->hero.getMonstreVise())->getCoordonnee().x-jeu->hero.m_personnage.getCoordonnee().x)<=2&&fabs(jeu->map->getEntiteMonstre(jeu->hero.getMonstreVise())->getCoordonnee().y-jeu->hero.m_personnage.getCoordonnee().y)<=2)
                            if (rand() % 100 < (float)((float)jeu->hero.m_caracteristiques.dexterite / ((float)jeu->map->getEntiteMonstre(jeu->hero.getMonstreVise())->getCaracteristique().dexterite+1))*75 )
                                if (!jeu->map->getEntiteMonstre(jeu->hero.getMonstreVise())->m_friendly)
                                    toucher=true,jeu->map->InfligerDegats(jeu->hero.getMonstreVise(),(rand()%(jeu->hero.m_personnage.getCaracteristique().degatsMax - jeu->hero.m_personnage.getCaracteristique().degatsMin+1))+jeu->hero.m_personnage.getCaracteristique().degatsMin,&jeu->hero,1);
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
                        jeu->map->GererMiracle(&jeu->hero.m_personnage.m_miracleEnCours.back(),&jeu->hero.m_classe.miracles[jeu->hero.m_personnage.m_miracleEnCours.back().m_modele],&jeu->hero,0,jeu->hero.m_personnage.getCoordonnee(),cible,1);
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


        jeu->map->Animer(&jeu->hero,tempsDepuisDerniereAnimation,&jeu->menu); // Animation des tiles de la jeu->map

        jeu->map->AnimerMiracle(&jeu->hero.m_personnage,jeu->hero.m_classe.miracles,tempsDepuisDerniereAnimation,positionHero,&jeu->hero);

        tempsDepuisDerniereAnimation=0;
    }
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
            && eventManager->getPositionSouris().x > 255
            && eventManager->getPositionSouris().x < 275
            && eventManager->getPositionSouris().y > 492
            && eventManager->getPositionSouris().y < 512)
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
            && eventManager->getPositionSouris().x > 287
            && eventManager->getPositionSouris().x < 307
            && eventManager->getPositionSouris().y > 492
            && eventManager->getPositionSouris().y < 512)
    {
        if(eventManager->getEvenement('2',"ET"))
            eventManager->StopEvenement('2',"ET");
        else
            eventManager->StopEvenement(Mouse::Left,"CA");
        if (jeu->hero.UtiliserObjet(jeu->hero.m_objets_raccourcis[1]))
            jeu->map->GererMiracle(&jeu->hero.m_personnage.m_miracleEnCours.back(),&jeu->hero.m_classe.miracles[jeu->hero.m_personnage.m_miracleEnCours.back().m_modele],&jeu->hero,0,jeu->hero.m_personnage.getCoordonnee(),eventManager->getCasePointee(),1);
    }
    if (eventManager->getEvenement('3',"ET")
            || eventManager->getEvenement(Mouse::Left,"CA")
            && eventManager->getPositionSouris().x > 319
            && eventManager->getPositionSouris().x < 339
            && eventManager->getPositionSouris().y > 492
            && eventManager->getPositionSouris().y < 512)
    {
        if(eventManager->getEvenement('3',"ET"))
            eventManager->StopEvenement('3',"ET");
        else
            eventManager->StopEvenement(Mouse::Left,"CA");
        if (jeu->hero.UtiliserObjet(jeu->hero.m_objets_raccourcis[2]))
            jeu->map->GererMiracle(&jeu->hero.m_personnage.m_miracleEnCours.back(),&jeu->hero.m_classe.miracles[jeu->hero.m_personnage.m_miracleEnCours.back().m_modele],&jeu->hero,0,jeu->hero.m_personnage.getCoordonnee(),eventManager->getCasePointee(),1);
    }
    if (eventManager->getEvenement('4',"ET")
            || eventManager->getEvenement(Mouse::Left,"CA")
            && eventManager->getPositionSouris().x > 351
            && eventManager->getPositionSouris().x < 371
            && eventManager->getPositionSouris().y > 492
            && eventManager->getPositionSouris().y < 512)
    {
        if(eventManager->getEvenement('4',"ET"))
            eventManager->StopEvenement('4',"ET");
        else
            eventManager->StopEvenement(Mouse::Left,"CA");
        if (jeu->hero.UtiliserObjet(jeu->hero.m_objets_raccourcis[3]))
            jeu->map->GererMiracle(&jeu->hero.m_personnage.m_miracleEnCours.back(),&jeu->hero.m_classe.miracles[jeu->hero.m_personnage.m_miracleEnCours.back().m_modele],&jeu->hero,0,jeu->hero.m_personnage.getCoordonnee(),eventManager->getCasePointee(),1);
    }
}

void GestionRaccourcisMiracles(Jeu *jeu)
{
    if (eventManager->getEvenement(Key::F1,"ET")
            || eventManager->getEvenement(Mouse::Left,"CA")
            && eventManager->getPositionSouris().x > 432 * configuration->Resolution.w/800
            &&eventManager->getPositionSouris().x < 452 * configuration->Resolution.w/800
            &&eventManager->getPositionSouris().y > 492 * configuration->Resolution.h/600
            &&eventManager->getPositionSouris().y < 512 * configuration->Resolution.h/600)
    {
        if(eventManager->getEvenement(Key::F1,"ET"))
            eventManager->StopEvenement(Key::F1,"ET");
        else
            eventManager->StopEvenement(Mouse::Left,"CA");
        jeu->hero.m_personnage.m_miracleALancer = jeu->hero.m_miracles_raccourcis[0];
    }
    if (eventManager->getEvenement(Key::F2,"ET")
            || eventManager->getEvenement(Mouse::Left,"CA")
            && eventManager->getPositionSouris().x > 464
            && eventManager->getPositionSouris().x < 484
            && eventManager->getPositionSouris().y > 492
            && eventManager->getPositionSouris().y < 512)
    {
        if(eventManager->getEvenement(Key::F2,"ET"))
            eventManager->StopEvenement(Key::F2,"ET");
        else
            eventManager->StopEvenement(Mouse::Left,"CA");
        jeu->hero.m_personnage.m_miracleALancer = jeu->hero.m_miracles_raccourcis[1];
    }
    if (eventManager->getEvenement(Key::F3,"ET")
            || eventManager->getEvenement(Mouse::Left,"CA")
            && eventManager->getPositionSouris().x > 496
            && eventManager->getPositionSouris().x < 516
            && eventManager->getPositionSouris().y > 492
            && eventManager->getPositionSouris().y < 512)
    {
        if(eventManager->getEvenement(Key::F3,"ET"))
            eventManager->StopEvenement(Key::F3,"ET");
        else
            eventManager->StopEvenement(Mouse::Left,"CA");
        jeu->hero.m_personnage.m_miracleALancer = jeu->hero.m_miracles_raccourcis[2];
    }
    if (eventManager->getEvenement(Key::F4,"ET")
            || eventManager->getEvenement(Mouse::Left,"CA")
            && eventManager->getPositionSouris().x > 528
            && eventManager->getPositionSouris().x < 548
            && eventManager->getPositionSouris().y > 492
            && eventManager->getPositionSouris().y < 512)
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
        || eventManager->getPositionSouris().x > 264
        && eventManager->getPositionSouris().x < 283
        && eventManager->getPositionSouris().y > 533
        && eventManager->getPositionSouris().y < 552)
    {
         moteurGraphique->AjouterTexte("Afficher/Cacher carte (M)",coordonnee(eventManager->getPositionSouris().x,
                                      eventManager->getPositionSouris().y - 20),
                                      19,0,12,sf::Color(224,224,224),1);

        if (eventManager->getEvenement(Key::M,"ET")
         || eventManager->getEvenement(Mouse::Left,"CA"))
        {
            if(eventManager->getEvenement(Key::M,"ET"))
                eventManager->StopEvenement(Key::M,"ET");
            else
                eventManager->StopEvenement(Mouse::Left,"CA");

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
        buf.SetPosition(264, 533);
        buf.Resize(20,20);
        buf.SetColor(sf::Color(0,0,0,128));
        moteurGraphique->AjouterCommande(&buf, 18, 0);
    }

    if (eventManager->getEvenement(Key::I,"ET")
        || eventManager->getPositionSouris().x > 302
        && eventManager->getPositionSouris().x < 321
        && eventManager->getPositionSouris().y > 533
        && eventManager->getPositionSouris().y < 552)
    {
        moteurGraphique->AjouterTexte("Inventaire (I)",coordonnee(eventManager->getPositionSouris().x,
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
        buf.SetPosition(302, 533);
        buf.Resize(20,20);
        buf.SetColor(sf::Color(0,0,0,128));
        moteurGraphique->AjouterCommande(&buf, 18, 0);
    }

    if (eventManager->getEvenement(Key::Q,"ET")
        || eventManager->getPositionSouris().x > 264
        && eventManager->getPositionSouris().x < 283
        && eventManager->getPositionSouris().y > 565
        && eventManager->getPositionSouris().y < 584)
    {
        moteurGraphique->AjouterTexte("Quêtes (Q)",coordonnee(eventManager->getPositionSouris().x,
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
        buf.SetPosition(264, 565);
        buf.Resize(20,20);
        buf.SetColor(sf::Color(0,0,0,128));
        moteurGraphique->AjouterCommande(&buf, 18, 0);
    }

    if (eventManager->getEvenement(Key::T,"ET")
        || eventManager->getPositionSouris().x > 302
        && eventManager->getPositionSouris().x < 321
        && eventManager->getPositionSouris().y > 565
        && eventManager->getPositionSouris().y < 584)
    {
        moteurGraphique->AjouterTexte("Miracles (T)",coordonnee(eventManager->getPositionSouris().x,
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
        buf.SetPosition(302, 565);
        buf.Resize(20,20);
        buf.SetColor(sf::Color(0,0,0,128));
        moteurGraphique->AjouterCommande(&buf, 18, 0);
    }

    if (eventManager->getEvenement(Key::Escape,"ET")
     || eventManager->getPositionSouris().x > 333
     && eventManager->getPositionSouris().x < 352
     && eventManager->getPositionSouris().y > 577
     && eventManager->getPositionSouris().y < 596)
    {
        moteurGraphique->AjouterTexte("Menu (Echappe)",coordonnee(eventManager->getPositionSouris().x,
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
        buf.SetPosition(333, 577);
        buf.Resize(20,20);
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

        if (eventManager->getEvenement(Mouse::Left,"CA")&&!eventManager->getEvenement(Key::LShift,"ET"))
        {
            jeu->hero.StopMiraclesFrappe();
            if (!(eventManager->getPositionSouris().x>configuration->Resolution.w-configuration->Resolution.w*0.25
                    &&eventManager->getPositionSouris().y>configuration->Resolution.w*0.25&&eventManager->getPositionSouris().y<configuration->Resolution.w*0.25+configuration->Resolution.w*0.34
                    &&alpha_sac>=128)||alpha_sac<=128)
            {
                if (jeu->hero.getMonstreVise()==-1)
                {
                    jeu->hero.m_personnage.setArrivee(eventManager->getCasePointee());

                    jeu->menu.m_dialogue = " ";

                    jeu->hero.setSacVise(coordonnee (-1,-1,-1,-1));
                }
            }
            else if (jeu->hero.getChercherSac().x!=-1&&jeu->map->getObjetPointe()!=-1&&jeu->map->getNombreObjets(jeu->hero.getChercherSac())>4)
            {
                jeu->map->RamasserObjet(&jeu->hero);
                eventManager->StopEvenement(Mouse::Left,"CA");
            }
        }

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
        if (eventManager->getEvenement(Mouse::Left,"C")&&eventManager->getEvenement(Mouse::Left,"CA"))
        {
            jeu->hero.setSacVise(jeu->map->getSacPointe());

            jeu->hero.m_objetVise=jeu->map->m_objetPointe;

            if (jeu->map->getSacPointe().x!=-1)
                eventManager->StopEvenement(Mouse::Left,"CA"),jeu->hero.m_personnage.setArrivee(jeu->map->getSacPointe());
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
                        jeu->hero.m_personnage.m_miracleEnCours.back().m_cible = jeu->map->getEntiteMonstre(jeu->map->getMonstreIllumine());

                        coordonnee positionHero;
                        positionHero.x=(jeu->hero.m_personnage.getCoordonnee().x-jeu->hero.m_personnage.getCoordonnee().y-1)/5;
                        positionHero.y=(jeu->hero.m_personnage.getCoordonnee().x+jeu->hero.m_personnage.getCoordonnee().y)/5;

                        jeu->map->GererMiracle(&jeu->hero.m_personnage.m_miracleEnCours.back(),&jeu->hero.m_classe.miracles[jeu->hero.m_personnage.m_miracleEnCours.back().m_modele],&jeu->hero,0,jeu->hero.m_personnage.getCoordonnee(),cible,1);
                        jeu->map->AnimerMiracle(&jeu->hero.m_personnage,jeu->hero.m_classe.miracles,tempsDepuisDerniereAnimation,positionHero,&jeu->hero);
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

    if (configuration->effetMort&&sf::PostFX::CanUsePostFX() == true&&configuration->postFX&&tempsEffetMort!=0)
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
        if (tempsEcouleDepuisFPS>0.1)
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

