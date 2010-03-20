

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

inline sf::Vector2f AutoScreenAdjust(float x, float y, float decalage = 0)
{
    sf::Vector2f temp;
    temp.x = x + (configuration->Resolution.x - 800) * 0.5;
    temp.y = y + (configuration->Resolution.y - 600) - decalage * configuration->Resolution.h/600;
    return temp;
}


c_Jeu::c_Jeu()
{
    continuer=true,lumiere=false,augmenter=false;
    tempsActuel=0,tempsPrecedent=0,tempsDepuisDerniereAnimation=0,tempsEcoule=0,tempsNbrTourBoucle=0,tempsEcouleDepuisDernierCalculLumiere=0.005,tempsEcouleDepuisDernierCalculOmbre=0,tempsEcouleDepuisDernierDeplacement=0,tempsEcouleDepuisDernierIA=0,tempsEcouleDepuisDernierAffichage=0,tempsEcouleDepuisFPS=0,tempsEffetMort=0,tempsSauvergarde=0;
    nbrTourBoucle=0;

    temps[0] = 0;
    temps[1] = 0;
    temps[2] = 0;
    temps[3] = 0;
    temps[4] = 0;

    configuration->heure=(rand() % (24));
    configuration->minute=0;

    sf::Listener::SetGlobalVolume((float)configuration->volume);

    // Texte pour l'affichage des FPS
    fps.SetCharacterSize(16);

    Version.SetCharacterSize(16);
    Version.SetString("v "+configuration->version);
    Version.SetY(20);

    Temps.SetCharacterSize(16);
    Temps.SetY(40);

    TourBoucle.SetX(120);
    TourBoucle.SetCharacterSize(16);

    alpha_map=0;
    alpha_sac=0;
    alpha_dialog=0;
    lowFPS=-1;


    m_thread_sauvegarde = NULL;
}

/*c_Jeu::~c_Jeu()
{
    if(m_thread_sauvegarde)
    {
        m_thread_sauvegarde->Wait();
        delete m_thread_sauvegarde;
        m_thread_sauvegarde = NULL;
    }
}*/

void c_Jeu::Utiliser(Jeu *jeu)
{
    //Gestion du temps
    temps[4] = temps[3];
    temps[3] = temps[2];
    temps[2] = temps[1];
    temps[1] = temps[0];
    temps[0] = jeu->Clock.GetElapsedTime();

    tempsEcoule = (temps[0] + temps[1] + temps[2] + temps[3] + temps[4]) / 5;
    if(temps[0] - tempsEcoule > 0.01)
    {
        if(temps[1] - tempsEcoule > 0.01)
            tempsEcoule = temps[1];
        else if(temps[2] - tempsEcoule > 0.01)
            tempsEcoule = temps[2];
        else if(temps[3] - tempsEcoule > 0.01)
            tempsEcoule = temps[3];
        else
            tempsEcoule = temps[4];
    }

    if (tempsEcoule>0.1f)
        tempsEcoule=0.1f;


    jeu->m_display=false;
    GererTemps(jeu);
    if (tempsSauvergarde>=configuration->frequence_sauvegarde)
    {
        if (m_thread_sauvegarde)
        {
            m_thread_sauvegarde->Terminate();
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

    if ((tempsEcouleDepuisDernierAffichage>0.012f&&configuration->syncronisation_verticale)||(!configuration->syncronisation_verticale))
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
        jeu->hero.RegenererVie(tempsEcoule);
        jeu->hero.RegenererFoi(tempsEcoule);
        jeu->hero.RegenererMiracles(tempsEcoule);
    }

    jeu->Clock.Reset();

    if (jeu->hero.m_personnage.EnVie())
    {
        if (jeu->hero.m_caracteristiques.maxVie!=0)
        {
            if (jeu->hero.m_caracteristiques.vie/(float)jeu->hero.m_caracteristiques.maxVie<0.25)
                configuration->effetMort=150-(jeu->hero.m_caracteristiques.vie*600/jeu->hero.m_caracteristiques.maxVie),jeu->sonMort.SetVolume(configuration->effetMort);
            else
                configuration->effetMort=0,jeu->sonMort.SetVolume(0);
        }
    }
    else
    {
        configuration->effetMort=150;

        std::string nomMap=jeu->hero.m_potales[jeu->hero.m_last_potale].chemin;

        console->Ajouter("",0);
        console->Ajouter("---------------------------------------------------------------------------------",0);
        console->Ajouter("EVENEMENT : Changement de map",0);
        console->Ajouter("---------------------------------------------------------------------------------",0);

        coordonnee coordonneePerso;
        coordonneePerso.x=jeu->hero.m_potales[jeu->hero.m_last_potale].position.x;
        coordonneePerso.y=jeu->hero.m_potales[jeu->hero.m_last_potale].position.y;

        jeu->m_chargement->setC_Chargement(nomMap,coordonneePerso);

        jeu->next_screen = 8;
        jeu->Next();
    }


}
void c_Jeu::IA(Jeu *jeu)
{
    if (tempsEcouleDepuisDernierIA>=0.027f)
    {
        jeu->map->GererMonstres(jeu,&jeu->hero,tempsEcouleDepuisDernierIA,&jeu->menu);
        jeu->map->GererProjectilesEtEffets(&jeu->hero,tempsEcouleDepuisDernierIA);
        jeu->map->GererScript(jeu,&jeu->hero,tempsEcouleDepuisDernierIA,&jeu->menu);

        tempsEcouleDepuisDernierIA=0;
    }
}
void c_Jeu::Deplacements(Jeu *jeu)
{
   // if (tempsEcouleDepuisDernierDeplacement >= 0.0)
    {

        coordonnee temp(-1 ,-1 ,-1 ,-1);
        if (jeu->hero.getMonstreVise()==-1)
            temp.x=jeu->hero.m_personnage.getCoordonnee().x,temp.y=jeu->hero.m_personnage.getCoordonnee().y;

        oldHeroPos = jeu->hero.m_personnage.getCoordonneePixel();

        jeu->map->TesterPoussable(jeu->hero.m_personnage, tempsEcouleDepuisDernierDeplacement);

        if (jeu->hero.m_personnage.SeDeplacer(tempsEcouleDepuisDernierDeplacement*100,jeu->map->getDimensions()))/*,
                                              (jeu->map->getCollisionPousse((int)((jeu->hero.m_personnage.getCoordonneePixel().x + jeu->hero.m_personnage.getPousse().x * tempsEcouleDepuisDernierDeplacement * COTE_TILE * 5 + COTE_TILE * 0.5 * (jeu->hero.m_personnage.getPousse().x >= 0))/COTE_TILE),
                                                                            (int)((jeu->hero.m_personnage.getCoordonneePixel().y + jeu->hero.m_personnage.getPousse().y * tempsEcouleDepuisDernierDeplacement * COTE_TILE * 5 + COTE_TILE * 0.5 * (jeu->hero.m_personnage.getPousse().y >= 0) )/COTE_TILE)) != 1)))*/
        {
            bool ok=true;
            if (jeu->hero.getMonstreVise()>-1)
                if (jeu->hero.TestMonstreVise(jeu->map->getEntiteMonstre(jeu->hero.getMonstreVise())))
                    ok=false;

            if (jeu->hero.m_personnage.getCoordonnee().x==jeu->hero.m_personnage.getArrivee().x && jeu->hero.m_personnage.getCoordonnee().y==jeu->hero.m_personnage.getArrivee().y)
                ok=false;

            if (ok)
                jeu->hero.m_personnage.Pathfinding(jeu->map->getAlentourDuPersonnage(jeu->hero.m_personnage.getCoordonnee()),temp); // Recherche du chemin

            if (eventManager->getEvenement(Mouse::Left,EventClicA)&&eventManager->getEvenement(Key::LShift,EventKey))
            {
                coordonnee temp((int)(eventManager->getCasePointee().x*COTE_TILE),(int)(eventManager->getCasePointee().y*COTE_TILE));
                jeu->hero.m_personnage.Frappe(jeu->hero.m_personnage.getCoordonneePixel(),temp);
            }
        }

        if(fabs(jeu->hero.m_personnage.getPousse().x) > 0 || fabs(jeu->hero.m_personnage.getPousse().y) > 0)
            jeu->hero.setMonstreVise(-1),jeu->hero.m_personnage.frappeEnCours=false;

        coordonneeDecimal old, heroPos;
        old.x = (oldHeroPos.x-oldHeroPos.y-1)*64;
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

    //if (tempsDepuisDerniereAnimation >= 0.02)
    {
        jeu->map->TestEvenement(jeu,tempsDepuisDerniereAnimation); // On test les événement pour voir s'il on doit changer de jeu->map, faire des dégats au perso, le régénérer, etc

        if (tempsDepuisDerniereAnimation>0.12f)
            tempsDepuisDerniereAnimation=0.12f;


        int retour=-2;
        retour = jeu->hero.m_personnage.Animer(&jeu->hero.m_modelePersonnage[0],tempsDepuisDerniereAnimation);

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
                            if (rand() % 100 < (float)((float)(jeu->hero.m_caracteristiques.dexterite + 100) / ((float)(jeu->map->getEntiteMonstre(jeu->hero.getMonstreVise())->getCaracteristique().dexterite + 100)))*75 )
                                if (!jeu->map->getEntiteMonstre(jeu->hero.getMonstreVise())->m_friendly && jeu->map->getEntiteMonstre(jeu->hero.getMonstreVise())->EnVie())
                                {
                                    int degats = (rand()%(jeu->hero.m_caracteristiques.degatsMax[PHYSIQUE] - jeu->hero.m_caracteristiques.degatsMin[PHYSIQUE]+1))+jeu->hero.m_caracteristiques.degatsMin[PHYSIQUE];
                                    toucher=true,jeu->map->InfligerDegats(jeu->hero.getMonstreVise(),degats,PHYSIQUE,&jeu->hero,1);

                                    jeu->hero.m_personnage.m_vientDeFrapper = jeu->map->getEntiteMonstre(jeu->hero.getMonstreVise());
                                    jeu->hero.m_personnage.m_degatsInflige  = degats;

                                    jeu->map->getEntiteMonstre(jeu->hero.getMonstreVise())->m_vientDetreTouche = &jeu->hero.m_personnage;

                                    jeu->hero.m_personnage.InfligerDegats(-degats * jeu->hero.m_caracteristiques.volVie, 4, NULL);
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
            jeu->hero.miracleEnCours = 0;
        }

        if (retour==2)
            if (!eventManager->getEvenement(Mouse::Left,EventClic) || !jeu->map->getMonstreEnVie(jeu->hero.getMonstreVise()) )
                jeu->hero.setMonstreVise(-1),jeu->hero.m_personnage.frappeEnCours=false,jeu->hero.m_personnage.setEtat(0);

        if (retour==1)
        {
            if (!eventManager->getEvenement(Mouse::Left,EventClic) || !jeu->map->getMonstreEnVie(jeu->hero.getMonstreVise()))
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
    /*if (configuration->Lumiere&&tempsEcouleDepuisDernierCalculLumiere>configuration->frequence_lumiere/2&&configuration->RafraichirOmbre==0)
        configuration->RafraichirOmbre=1;

    if (configuration->Lumiere&&tempsEcouleDepuisDernierCalculLumiere>configuration->frequence_lumiere)
    {
        if (configuration->RafraichirOmbre==2)
            configuration->RafraichirOmbre=0;

        jeu->map->CalculerOmbresEtLumieres();

        configuration->RafraichirLumiere=true;
        tempsEcouleDepuisDernierCalculLumiere=0;

        moteurGraphique->LightManager->Generate(jeu->hero.m_personnage.m_entite_graphique.m_light);
    }*/

    if(configuration->RafraichirOmbre > 0)
    {
        jeu->map->CalculerOmbresEtLumieres();

        configuration->RafraichirLumiere = true;
        configuration->RafraichirOmbre = false;
        tempsEcouleDepuisDernierCalculLumiere=0;

        moteurGraphique->LightManager->Generate(jeu->hero.m_personnage.m_entite_graphique.m_light);
    }
    else
    {
        configuration->RafraichirLumiere = false;
        configuration->RafraichirOmbre = true;
    }
}


void GestionRaccourcis(Jeu *jeu)
{
    int newmiracle = -1;

    for(int i = 0 ; i < 8 ; ++i)
    {
        if (eventManager->getEvenement('1'+i,EventKey)
            || eventManager->getEvenement(Mouse::Left,EventClicA)
            && eventManager->getPositionSouris().x > AutoScreenAdjust(jeu->hero.m_classe.position_raccourcis[i].x,0).x
            && eventManager->getPositionSouris().x < AutoScreenAdjust(jeu->hero.m_classe.position_raccourcis[i].x,0).x + jeu->hero.m_classe.position_raccourcis[i].w
            && eventManager->getPositionSouris().y > AutoScreenAdjust(0,jeu->hero.m_classe.position_raccourcis[i].y).y
            && eventManager->getPositionSouris().y < AutoScreenAdjust(0,jeu->hero.m_classe.position_raccourcis[i].y).y + jeu->hero.m_classe.position_raccourcis[i].h)
        {
            if(eventManager->getEvenement('1'+i,EventKey))
                eventManager->StopEvenement('1'+i,EventKey);
            else
                eventManager->StopEvenement(Mouse::Left,EventClicA);

            if(jeu->hero.m_raccourcis[i].no >= 0)
            {
                if(jeu->hero.m_raccourcis[i].miracle)
                    newmiracle = jeu->hero.m_raccourcis[i].no;
                else
                    jeu->hero.UtiliserObjet(jeu->hero.m_raccourcis[i].no);
            }
        }
    }

    if(newmiracle >= 0 && newmiracle < jeu->hero.m_classe.miracles.size())
    {
        if(jeu->hero.m_classe.miracles[newmiracle].m_direct)
        {
            if (!jeu->hero.m_personnage.frappeEnCours)
            {
                eventManager->StopEvenement(Mouse::Right,EventClic);

                coordonnee cible;

                if (jeu->map->getEntiteMonstre(jeu->map->getMonstreIllumine())!=NULL)
                    cible = jeu->map->getEntiteMonstre(jeu->map->getMonstreIllumine())->getProchaineCase();
                else
                    cible = eventManager->getCasePointee();

                if (jeu->hero.UtiliserMiracle(newmiracle, jeu->map->getEntiteMonstre(jeu->map->getMonstreIllumine()), cible))
                {
                    jeu->hero.m_personnage.m_miracleEnCours.back().m_infos.back()->m_cible = jeu->map->getEntiteMonstre(jeu->map->getMonstreIllumine());

                    coordonnee positionHero;
                    positionHero.x=(jeu->hero.m_personnage.getCoordonnee().x-jeu->hero.m_personnage.getCoordonnee().y-1)/5;
                    positionHero.y=(jeu->hero.m_personnage.getCoordonnee().x+jeu->hero.m_personnage.getCoordonnee().y)/5;
                }
                else
                    jeu->hero.m_personnage.setArrivee(eventManager->getCasePointee());
            }
        }
        else
            jeu->hero.m_personnage.m_miracleALancer = newmiracle;
    }
}

int GestionBoutons(Jeu *jeu)
{
    if (   eventManager->getPositionSouris().x > AutoScreenAdjust(750,0).x
        && eventManager->getPositionSouris().y < AutoScreenAdjust(0,50).y
        && eventManager->getEvenement(Mouse::Left,EventClic))
        {
            eventManager->StopEvenement(Mouse::Left,EventClic);
            return 3;
        }

    if (eventManager->getEvenement(Key::M,EventKey)
        || eventManager->getPositionSouris().x > AutoScreenAdjust(459,0).x
        && eventManager->getPositionSouris().x < AutoScreenAdjust(487,0).x
        && eventManager->getPositionSouris().y > AutoScreenAdjust(0,522).y
        && eventManager->getPositionSouris().y < AutoScreenAdjust(0,537).y)
    {
        if(!eventManager->getEvenement(Key::M,EventKey))
         moteurGraphique->AjouterTexte(configuration->getText(0,13),coordonnee(eventManager->getPositionSouris().x,
                                      eventManager->getPositionSouris().y - 20),
                                      19,0,12,sf::Color(224,224,224),1);

        if (eventManager->getEvenement(Key::M,EventKey)
         || eventManager->getEvenement(Mouse::Left,EventClic))
        {
            if(eventManager->getEvenement(Key::M,EventKey))
                eventManager->StopEvenement(Key::M,EventKey);
            else
                eventManager->StopEvenement(Mouse::Left,EventClic);

            if (!configuration->Minimap)
                configuration->Minimap=true;
            else
                configuration->Minimap=false;
        }
    }


    if (eventManager->getEvenement(Key::Tab,EventKey)
        || eventManager->getPositionSouris().x > AutoScreenAdjust(314,0).x
        && eventManager->getPositionSouris().x < AutoScreenAdjust(342,0).x
        && eventManager->getPositionSouris().y > AutoScreenAdjust(0,522).y
        && eventManager->getPositionSouris().y < AutoScreenAdjust(0,537).y)
    {
        if(!eventManager->getEvenement(Key::Tab,EventKey))
         moteurGraphique->AjouterTexte(configuration->getText(0,51),coordonnee(eventManager->getPositionSouris().x,
                                      eventManager->getPositionSouris().y - 20),
                                      19,0,12,sf::Color(224,224,224),1);

        if (eventManager->getEvenement(Key::Tab,EventKey)
         || eventManager->getEvenement(Mouse::Left,EventClic))
        {
            if(eventManager->getEvenement(Key::Tab,EventKey))
                eventManager->StopEvenement(Key::Tab,EventKey);
            else
                eventManager->StopEvenement(Mouse::Left,EventClic);

            if (configuration->console==0)
                configuration->console=1;
            else if (configuration->console==1)
                configuration->console=2;
            else
                configuration->console=0;
        }
    }

    if (eventManager->getEvenement(Key::I,EventKey)
        || eventManager->getPositionSouris().x > AutoScreenAdjust(372,0).x
        && eventManager->getPositionSouris().x < AutoScreenAdjust(400,0).x
        && eventManager->getPositionSouris().y > AutoScreenAdjust(0,522).y
        && eventManager->getPositionSouris().y < AutoScreenAdjust(0,537).y)
    {
        if(!eventManager->getEvenement(Key::I,EventKey))
        moteurGraphique->AjouterTexte(configuration->getText(0,14),coordonnee(eventManager->getPositionSouris().x,
                                      eventManager->getPositionSouris().y - 20),
                                      19,0,12,sf::Color(224,224,224),1);

        if (eventManager->getEvenement(Key::I,EventKey)
         || eventManager->getEvenement(Mouse::Left,EventClicA))
        {
            eventManager->StopEvenement(Key::I,EventKey);
            eventManager->StopEvenement(Mouse::Left,EventClicA);
            eventManager->StopEvenement(Mouse::Left,EventClic);
            jeu->map->m_defilerObjets=0;

            return 2;
        }
    }


    if (eventManager->getEvenement(Key::Q,EventKey)
        || eventManager->getPositionSouris().x > AutoScreenAdjust(401,0).x
        && eventManager->getPositionSouris().x < AutoScreenAdjust(429,0).x
        && eventManager->getPositionSouris().y > AutoScreenAdjust(0,522).y
        && eventManager->getPositionSouris().y < AutoScreenAdjust(0,537).y)
    {
        if(!eventManager->getEvenement(Key::Q,EventKey))
        moteurGraphique->AjouterTexte(configuration->getText(0,15),coordonnee(eventManager->getPositionSouris().x,
                                      eventManager->getPositionSouris().y - 20),
                                      19,0,12,sf::Color(224,224,224),1);

        if (eventManager->getEvenement(Key::Q,EventKey)
         || eventManager->getEvenement(Mouse::Left,EventClicA))
        {
            eventManager->StopEvenement(Key::Q,EventKey);
            eventManager->StopEvenement(Mouse::Left,EventClicA);
            eventManager->StopEvenement(Mouse::Left,EventClic);

            return 6;
        }
    }

    if (eventManager->getEvenement(Key::T,EventKey)
        || eventManager->getPositionSouris().x > AutoScreenAdjust(343,0).x
        && eventManager->getPositionSouris().x < AutoScreenAdjust(371,0).x
        && eventManager->getPositionSouris().y > AutoScreenAdjust(0,522).y
        && eventManager->getPositionSouris().y < AutoScreenAdjust(0,537).y)
    {
        if(!eventManager->getEvenement(Key::T,EventKey))
        moteurGraphique->AjouterTexte(configuration->getText(0,16),coordonnee(eventManager->getPositionSouris().x,
                                      eventManager->getPositionSouris().y - 20),
                                      19,0,12,sf::Color(224,224,224),1);

        if (eventManager->getEvenement(Key::T,EventKey)
         || eventManager->getEvenement(Mouse::Left,EventClicA))
        {
            eventManager->StopEvenement(Key::T,EventKey);
            eventManager->StopEvenement(Mouse::Left,EventClicA);
            eventManager->StopEvenement(Mouse::Left,EventClic);

            return 5;
        }
    }
    if (eventManager->getEvenement(Key::Escape,EventKey)
     || eventManager->getPositionSouris().x > AutoScreenAdjust(430,0).x
     && eventManager->getPositionSouris().x < AutoScreenAdjust(458,0).x
     && eventManager->getPositionSouris().y > AutoScreenAdjust(0,522).y
     && eventManager->getPositionSouris().y < AutoScreenAdjust(0,537).y)
    {
        if(!eventManager->getEvenement(Key::Escape,EventKey))
        moteurGraphique->AjouterTexte(configuration->getText(0,17),coordonnee(eventManager->getPositionSouris().x,
                                      eventManager->getPositionSouris().y - 20),
                                      19,0,12,sf::Color(224,224,224),1);

        if (eventManager->getEvenement(Key::Escape,EventKey)
         || eventManager->getEvenement(Mouse::Left,EventClicA))
        {

            eventManager->StopEvenement(Key::Escape,EventKey);
            eventManager->StopEvenement(Mouse::Left,EventClicA);
            eventManager->StopEvenement(Mouse::Left,EventClic);
            return 4;
        }
    }



    if (eventManager->getEvenement(Key::Return,EventKey)
     || eventManager->getPositionSouris().x > AutoScreenAdjust(jeu->hero.m_classe.position_bouton_dialogue.x,0).x
     && eventManager->getPositionSouris().x < AutoScreenAdjust(jeu->hero.m_classe.position_bouton_dialogue.x + jeu->hero.m_classe.position_bouton_dialogue.w,0).x
     && eventManager->getPositionSouris().y > AutoScreenAdjust(0,jeu->hero.m_classe.position_bouton_dialogue.y).y
     && eventManager->getPositionSouris().y < AutoScreenAdjust(0,jeu->hero.m_classe.position_bouton_dialogue.y + jeu->hero.m_classe.position_bouton_dialogue.h).y
     && eventManager->getEvenement(Mouse::Left,EventClicA))
    {
        if(!jeu->menu.m_dialogue.empty())
        {
            eventManager->StopEvenement(Key::Escape,EventKey);
            eventManager->StopEvenement(Mouse::Left,EventClicA);
            eventManager->StopEvenement(Mouse::Left,EventClic);
            jeu->menu.m_dialogue.clear();
            jeu->hero.setMonstreVise(-1);
        }
        jeu->menu.ClearSpeakChoice();
    }

    if(jeu->hero.m_caracteristiques.pts_restant > 0)
    if (eventManager->getPositionSouris().x > AutoScreenAdjust(jeu->hero.m_classe.hud_pt_caract_rest.position.x,0).x
     && eventManager->getPositionSouris().x < AutoScreenAdjust(jeu->hero.m_classe.hud_pt_caract_rest.position.x
                                                             + jeu->hero.m_classe.hud_pt_caract_rest.position.w,0).x
     && eventManager->getPositionSouris().y > AutoScreenAdjust(0,jeu->hero.m_classe.hud_pt_caract_rest.position.y).y
     && eventManager->getPositionSouris().y < AutoScreenAdjust(0,jeu->hero.m_classe.hud_pt_caract_rest.position.y
                                                               + jeu->hero.m_classe.hud_pt_caract_rest.position.h).y)
    {
        if(eventManager->getEvenement(Mouse::Left,EventClicA))
        {
            eventManager->StopEvenement(Mouse::Left,EventClicA);
            eventManager->StopEvenement(Mouse::Left,EventClic);
            return 2;
        }

        moteurGraphique->AjouterTexte(configuration->getText(0,43),coordonnee(eventManager->getPositionSouris().x,
                                      eventManager->getPositionSouris().y - 20),
                                      19,0,12,sf::Color(224,224,224),1);
    }

    if(jeu->hero.m_caracteristiques.miracles_restant > 0)
    if (eventManager->getPositionSouris().x > AutoScreenAdjust(jeu->hero.m_classe.hud_pt_miracle_rest.position.x,0).x
     && eventManager->getPositionSouris().x < AutoScreenAdjust(jeu->hero.m_classe.hud_pt_miracle_rest.position.x
                                                             + jeu->hero.m_classe.hud_pt_miracle_rest.position.w,0).x
     && eventManager->getPositionSouris().y > AutoScreenAdjust(0,jeu->hero.m_classe.hud_pt_miracle_rest.position.y).y
     && eventManager->getPositionSouris().y < AutoScreenAdjust(0,jeu->hero.m_classe.hud_pt_miracle_rest.position.y
                                                               + jeu->hero.m_classe.hud_pt_miracle_rest.position.h).y)
    {
        if(eventManager->getEvenement(Mouse::Left,EventClicA))
        {
            eventManager->StopEvenement(Mouse::Left,EventClicA);
            eventManager->StopEvenement(Mouse::Left,EventClic);
            return 5;
        }

        moteurGraphique->AjouterTexte(configuration->getText(0,43),coordonnee(eventManager->getPositionSouris().x,
                                      eventManager->getPositionSouris().y - 20),
                                      19,0,12,sf::Color(224,224,224),1);
    }

    return -1;
}


void c_Jeu::Evenements(Jeu *jeu)
{
    if(!configuration->error_message.empty())
        jeu->menu.m_dialogue = configuration->error_message,configuration->error_message.clear();

    if (eventManager->getPositionSouris().y < 492 * configuration->Resolution.h/600)
    if(jeu->menu.m_dialogue.empty())
    {
        if (!eventManager->getEvenement(Mouse::Left,EventClic))
            jeu->map->getMonstre(&jeu->hero,eventManager->getPositionSouris(),eventManager->getCasePointee());

        if (eventManager->getEvenement(Key::LAlt,EventKey))
            jeu->map->m_monstreIllumine = -1;

        if(jeu->map->getEntiteMonstre(jeu->map->m_monstreIllumine) != NULL)
            if(!jeu->map->getEntiteMonstre(jeu->map->m_monstreIllumine)->EnVie())
                jeu->map->m_monstreIllumine = -1, eventManager->StopEvenement(Mouse::Left,EventClic);

        if (eventManager->getEvenement(Mouse::Left,EventClic)&&eventManager->getEvenement(Mouse::Left,EventClicA))
        {
            jeu->hero.m_personnage.m_miracleBloquant = false;
            jeu->hero.StopMiraclesFrappe();
            if (jeu->map->getMonstreIllumine()!=-1)
            {
                bool test=false;
                if (jeu->hero.getMonstreVise()>-1&&jeu->hero.m_personnage.getArrivee().x==jeu->hero.m_personnage.getCoordonnee().x&&jeu->hero.m_personnage.getArrivee().y==jeu->hero.m_personnage.getCoordonnee().y)
                    test=true;

                eventManager->StopEvenement(Mouse::Left,EventClicA);
                jeu->hero.setMonstreVise(jeu->map->getMonstreIllumine());
                if (test)
                    jeu->hero.TestMonstreVise(jeu->map->getEntiteMonstre(jeu->hero.getMonstreVise()));
            }
            else
                jeu->hero.setMonstreVise(-1);
        }


        if (eventManager->getEvenement(Mouse::Left,EventClic)&&!eventManager->getEvenement(Key::LShift,EventKey))
        {
            jeu->hero.StopMiraclesFrappe();
            if (!(eventManager->getPositionSouris().x>configuration->Resolution.w-configuration->Resolution.w*0.25
                    &&eventManager->getPositionSouris().y>configuration->Resolution.w*0.25&&eventManager->getPositionSouris().y<configuration->Resolution.w*0.25+configuration->Resolution.w*0.34
                    &&alpha_sac>=128)||alpha_sac<=128)
            {
                if (jeu->hero.getMonstreVise()==-1)
                {
                    jeu->hero.m_personnage.setArrivee(eventManager->getCasePointee());
                    jeu->hero.setSacVise(coordonnee (-1,-1,-1,-1));
                }
            }
            else if (jeu->hero.getChercherSac().x!=-1&&jeu->map->getObjetPointe()!=-1&&jeu->map->getNombreObjets(jeu->hero.getChercherSac())>4)
            {
                jeu->map->RamasserObjet(&jeu->hero);
                eventManager->StopEvenement(Mouse::Left,EventClicA);
                eventManager->StopEvenement(Mouse::Left,EventClic);
            }

            eventManager->StopEvenement(Mouse::Left,EventClicA);
        }

        if (eventManager->getEvenement(Mouse::Left,EventClic)/*&&eventManager->getEvenement(Mouse::Left,EventClicA)*/)
        {
            jeu->hero.setSacVise(jeu->map->getSacPointe());

            jeu->hero.m_objetVise=jeu->map->m_objetPointe;

            if (jeu->map->getSacPointe().x!=-1)
                eventManager->StopEvenement(Mouse::Left,EventClic),jeu->hero.m_personnage.setArrivee(jeu->map->getSacPointe());
        }

        if (jeu->hero.getSacVise().x!=-1)
        {
            if (fabs(jeu->hero.getSacVise().x - jeu->hero.m_personnage.getCoordonnee().x) <= 1
            &&  fabs(jeu->hero.getSacVise().y - jeu->hero.m_personnage.getCoordonnee().y) <= 1)
            {
                jeu->hero.setChercherSac(jeu->hero.getSacVise());
                jeu->hero.m_personnage.setArrivee(jeu->hero.m_personnage.getProchaineCase());
            }
            else
                jeu->hero.setChercherSac(coordonnee (-1,-1,-1,-1));
        }
        else
            jeu->hero.setChercherSac(coordonnee (-1,-1,-1,-1));

        if (eventManager->getEvenement(Mouse::Right,EventClic))
        {
            if (eventManager->getEvenement(Mouse::Left,EventClic))
                eventManager->StopEvenement(Mouse::Left,EventClic);
            else
            {
                if (!jeu->hero.m_personnage.frappeEnCours)
                {
                    eventManager->StopEvenement(Mouse::Right,EventClic);

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
                    }
                    else if(!eventManager->getEvenement(Key::LShift,EventKey))
                        jeu->hero.m_personnage.setArrivee(eventManager->getCasePointee());
                    else
                        jeu->hero.m_personnage.setArrivee(jeu->hero.m_personnage.getProchaineCase());
                }
            }
        }

    }

    GestionRaccourcis(jeu);
    jeu->next_screen = GestionBoutons(jeu);

    if (jeu->next_screen >=0 )
        jeu->Next();
}


void c_Jeu::Affichage(Jeu *jeu)
{
    moteurGraphique->Gerer(tempsEcouleDepuisDernierAffichage,jeu->map->getDimensions().y);

    jeu->map->Afficher(&jeu->hero,eventManager->getEvenement(Key::LAlt,EventKey),alpha_map);//Affichage de la jeu->map

    jeu->hero.AfficherAmis();

   // jeu->hero.AfficherFlecheQuetes(jeu->map->getNom(), tempsEcoule);

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
   // if (alpha_map>0)
       // jeu->menu.Afficher(2,alpha_map,&jeu->hero.m_classe);//On affiche la mini-map
    if (alpha_dialog>0)
        jeu->menu.AfficherDialogue((int)alpha_dialog,&jeu->hero.m_classe);//On affiche la mini-map


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

    if (!jeu->menu.m_dialogue.empty())
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

    jeu->menu.AfficherHUD(&jeu->hero.m_classe); // On affiche le hud

    if (jeu->map->getEntiteMonstre(jeu->map->getMonstreIllumine())!=NULL)
        jeu->menu.AfficherDynamique(jeu->hero.m_caracteristiques,1,jeu->map->getEntiteMonstre(jeu->map->getMonstreIllumine())->getCaracteristique(),&jeu->hero.m_classe);
    else
        jeu->menu.AfficherDynamique(jeu->hero.m_caracteristiques,0,jeu->hero.m_caracteristiques,&jeu->hero.m_classe);
    eventManager->AfficherCurseur(); // On affiche le curseur de la souris

    if (jeu->map->getEvenement(eventManager->getCasePointee())>=0)
        jeu->map->AfficherNomEvenement(eventManager->getCasePointee(),eventManager->getPositionSouris());

    if (configuration->effetMort&&configuration->postFX&&tempsEffetMort!=0)
    {
        moteurGraphique->EffectMort.SetParameter("offset", 0.003*configuration->effetMort/100*(0.6+tempsEffetMort/10));
        moteurGraphique->EffectMort.SetParameter("color",1+0.5*configuration->effetMort/100*tempsEffetMort, 1-1*configuration->effetMort/100*tempsEffetMort, 1-1*configuration->effetMort/100*tempsEffetMort);
        moteurGraphique->EffectMort.SetParameter("black",1-configuration->effetMort/150);
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
        if (tempsEcouleDepuisFPS>1.0f)
        {
            {
                std::ostringstream  buf;
                if (configuration->console&&moteurGraphique->GetFPS()!=0)
                    buf<<moteurGraphique->GetFPS()<<" / "<<(int)lowFPS<<" FPS";
                fps.SetString(buf.str());
            }

            {
                std::ostringstream  buf;

                buf<<"Temps : "<<configuration->heure<<" h "<<(int)configuration->minute;
                Temps.SetString(buf.str());
            }

            tempsEcouleDepuisFPS=0;
        }

    jeu->m_display=true;
}
void c_Jeu::FPS(Jeu *jeu)
{
    nbrTourBoucle++;

    if (tempsNbrTourBoucle>1.0f)
    {
        lowFPS=-1;
        std::ostringstream  buf;
        buf<<"Nbr Tour de boucle : "<<nbrTourBoucle;
        TourBoucle.SetString(buf.str());
        nbrTourBoucle=0;

        tempsNbrTourBoucle=0;
    }
}

