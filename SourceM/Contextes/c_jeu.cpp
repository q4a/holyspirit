

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
    sf::Context context;

    Jeu *jeu = static_cast<Jeu*>(UserData);

    jeu->map->Sauvegarder(&jeu->hero);
    jeu->hero.Sauvegarder();
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
    continuer=true;
    lumiere=false;
    augmenter=false;
    tempsEcoule=0;
    tempsNbrTourBoucle=0;
    tempsEcouleDepuisFPS=0;
    tempsEffetMort=0;
    tempsSauvergarde=0;
    nbrTourBoucle=0;

    m_dead = 0;

    temps[0] = 0;
    temps[1] = 0;
    temps[2] = 0;
    temps[3] = 0;
    temps[4] = 0;

    configuration->heure=12;//(rand() % (24));
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
    lowFPS=-1;

    m_thread_sauvegarde = NULL;
}

c_Jeu::~c_Jeu()
{

}

void c_Jeu::Utiliser(Jeu *jeu)
{
    //Gestion du temps
    temps[4] = temps[3];
    temps[3] = temps[2];
    temps[2] = temps[1];
    temps[1] = temps[0];
    temps[0] = jeu->Clock.GetElapsedTime()*0.001;

    configuration->elapsed_time = temps[0];

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

    if (tempsEcoule>0.025f)
        tempsEcoule=0.025f;

    jeu->Clock.Reset();

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

    Lumieres(jeu);
    if (jeu->hero.m_personnage.EnVie())
    {
        Evenements(jeu);
        Deplacements(jeu);
        Animation(jeu);
    }
    else
        jeu->map->GererScript(jeu,&jeu->hero,tempsEcoule,&jeu->menu);

    jeu->hero.PlacerCamera();

    Affichage(jeu);

    FPS();
}


void c_Jeu::GererTemps(Jeu *jeu)
{
    tempsSauvergarde                        += tempsEcoule;
    configuration->minute                   += tempsEcoule;
    tempsNbrTourBoucle                      += tempsEcoule;

    if (configuration->minute>=60)
        configuration->minute=0,configuration->heure++;
    if (configuration->heure>23)
        configuration->heure=0, configuration->jour++;
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

    jeu->hero.GererTemps(tempsEcoule);

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

   // if (alpha_dialog>0)
    jeu->menu.AfficherDialogue(tempsEcoule,&jeu->hero.m_classe);
    jeu->map->GererAmbiance(tempsEcoule);

    if (jeu->hero.m_personnage.EnVie())
    {
        jeu->hero.RegenererVie(tempsEcoule);
        jeu->hero.RegenererFoi(tempsEcoule);
        jeu->hero.RegenererMiracles(tempsEcoule);

        if (jeu->hero.m_caracteristiques.maxVie != 0.0f)
        {
            if (jeu->hero.m_caracteristiques.vie/(float)jeu->hero.m_caracteristiques.maxVie<0.25)
                configuration->effetMort=75-(jeu->hero.m_caracteristiques.vie*600/jeu->hero.m_caracteristiques.maxVie)/2,jeu->sonMort.SetVolume(configuration->effetMort);
            else
                configuration->effetMort=0,jeu->sonMort.SetVolume(0);
        }
    }
    else
    {
        if(m_dead == 0)
            moteurSons->JouerSon(jeu->hero.m_classe.m_sound_death,coordonnee (), true);

        configuration->effetMort=150;
        m_dead += tempsEcoule;

        jeu->hero.m_personnage.m_cible = NULL;

        if(eventManager->getEvenement(sf::Mouse::Left,EventClic) && m_dead >= 2)
        {
            m_dead = 0;
            std::string nomMap=jeu->hero.m_potales[jeu->hero.m_last_potale].chemin;

            coordonnee coordonneePerso;
            coordonneePerso.x=jeu->hero.m_potales[jeu->hero.m_last_potale].position.x;
            coordonneePerso.y=jeu->hero.m_potales[jeu->hero.m_last_potale].position.y;

            jeu->m_chargement->setC_Chargement(nomMap,coordonneePerso);

            jeu->next_screen = 8;
            jeu->Next();
        }

        moteurGraphique->special_typo.Draw(configuration->getText(0,102), sf::Vector2f(configuration->Resolution.w/2,
                                                          configuration->Resolution.h/2-96), 64, 19, true, sf::Color(255,255,255,(m_dead <= 2) ? m_dead/2*255 : 255));
        moteurGraphique->special_typo.Draw(configuration->getText(0,103), sf::Vector2f(configuration->Resolution.w/2,
                                                          configuration->Resolution.h/2), 32, 19, true, sf::Color(255,255,255,(m_dead <= 2) ? m_dead/2*255 : 255));

        if(m_dead >= 2)
        {
            moteurGraphique->special_typo_p.Draw(configuration->getText(0,102), sf::Vector2f(configuration->Resolution.w/2,
                                                              configuration->Resolution.h/2-96), 64, 19, true, sf::Color(255,255,255,fabs(m_dead - (int)((m_dead+1)/2)*2)*255));
            moteurGraphique->special_typo_p.Draw(configuration->getText(0,103), sf::Vector2f(configuration->Resolution.w/2,
                                                              configuration->Resolution.h/2), 32, 19, true, sf::Color(255,255,255,fabs(m_dead - (int)((m_dead+1)/2)*2)*255));
        }
    }

}
void c_Jeu::IA(Jeu *jeu)
{
    jeu->map->GererMonstres(jeu,&jeu->hero,tempsEcoule,&jeu->menu);
    jeu->map->GererProjectilesEtEffets(&jeu->hero,tempsEcoule);
    jeu->map->GererScript(jeu,&jeu->hero,tempsEcoule,&jeu->menu);
}
void c_Jeu::Deplacements(Jeu *jeu)
{
    coordonnee temp(-1 ,-1 ,-1 ,-1);
    if (jeu->hero.m_personnage.m_cible == NULL)
        temp.x=jeu->hero.m_personnage.getCoordonnee().x,temp.y=jeu->hero.m_personnage.getCoordonnee().y;

    oldHeroPos = jeu->hero.m_personnage.getCoordonneePixel();

    jeu->map->TesterPoussable(jeu->hero.m_personnage, tempsEcoule);

    bool seDeplacer = jeu->hero.m_personnage.SeDeplacer(tempsEcoule*100);

    IA(jeu);

    if (seDeplacer)
    {
        bool ok=true;
        if (jeu->hero.m_personnage.m_cible != NULL)
            if (jeu->hero.TestMonstreVise(jeu->hero.m_personnage.m_cible))
                ok=false;

        if (jeu->hero.m_personnage.getCoordonnee().x==jeu->hero.m_personnage.getArrivee().x
         && jeu->hero.m_personnage.getCoordonnee().y==jeu->hero.m_personnage.getArrivee().y)
            ok=false;

        if (ok)
        {
            jeu->hero.m_personnage.Pathfinding(jeu->map->getAlentourDuPersonnage(jeu->hero.m_personnage.getCoordonnee()),temp);
            jeu->hero.m_personnage.SeDeplacer(0);
        }

        if (eventManager->getEvenement(Mouse::Left,EventClicA)&&eventManager->getEvenement(Key::LShift,EventKey))
        {
            coordonnee temp2((int)(eventManager->getCasePointee().x*COTE_TILE),(int)(eventManager->getCasePointee().y*COTE_TILE));
            jeu->hero.m_personnage.Frappe(jeu->hero.m_personnage.getCoordonneePixel(),temp2);

            jeu->hero.m_case_visee = eventManager->getCasePointee();
        }
    }

    if(fabs(jeu->hero.m_personnage.getPousse().x) > 0 || fabs(jeu->hero.m_personnage.getPousse().y) > 0)
        jeu->hero.m_personnage.m_cible = NULL,jeu->hero.m_personnage.frappeEnCours=false;

    coordonneeDecimal old, heroPos;
    old.x = (oldHeroPos.x-oldHeroPos.y-1)*64;
    old.y = (oldHeroPos.x+oldHeroPos.y)*32;

    heroPos.x = (jeu->hero.m_personnage.getCoordonneePixel().x-jeu->hero.m_personnage.getCoordonneePixel().y-1)*64;;
    heroPos.y = (jeu->hero.m_personnage.getCoordonneePixel().x+jeu->hero.m_personnage.getCoordonneePixel().y)*32;

    moteurGraphique->decalageReflection.x += heroPos.x - old.x;
    moteurGraphique->decalageReflection.y += heroPos.y - old.y;

    coordonnee position;
    position.x=(jeu->hero.m_personnage.getCoordonnee().x-jeu->hero.m_personnage.getCoordonnee().y-1)/5;
    position.y=(jeu->hero.m_personnage.getCoordonnee().x+jeu->hero.m_personnage.getCoordonnee().y)/5;

    Listener::SetGlobalVolume((float)configuration->volume);
    Listener::SetPosition(-position.x, 1, position.y);
    Listener::SetDirection(0, 0, 1);
    jeu->map->MusiquePlay();
    jeu->sonMort.SetPosition(-position.x,0,position.y);
}
void c_Jeu::Animation(Jeu *jeu)
{
    float temps2 = tempsEcoule;

    coordonnee positionHero;
    positionHero.x=(jeu->hero.m_personnage.getCoordonnee().x-jeu->hero.m_personnage.getCoordonnee().y-1)/5;
    positionHero.y=(jeu->hero.m_personnage.getCoordonnee().x+jeu->hero.m_personnage.getCoordonnee().y)/5;

    if (temps2>0.12f)
        temps2=0.12f;

    for (std::list<Hero>::iterator p = jeu->m_personnageClients.begin(); p != jeu->m_personnageClients.end(); ++p)
    {
       /* p->m_personnage.m_entite_graphique.NextTile(true);
        p->m_personnage.m_entite_graphique.Generer();
        p->m_personnage.Animer(&p->m_modelePersonnage[0],0, true);*/
        p->CalculerOrdreAffichage();
        p->m_personnage.EmulerDeplacement(tempsEcoule);
    }

    int retour = -2;
    retour = jeu->hero.m_personnage.Gerer(&jeu->hero.m_modelePersonnage[0],temps2, true);

    jeu->hero.CalculerOrdreAffichage();
    jeu->hero.m_personnage.m_vientDeFrapper = NULL;
    jeu->hero.m_personnage.m_degatsInflige  = 0;
    jeu->hero.m_personnage.m_vientDAttaquer.x = -1;

    if (retour==0) //Animation du héro
    {
        if (jeu->hero.m_personnage.frappeEnCours
        && (!jeu->hero.m_personnage.m_lancementMiracleEnCours || jeu->hero.m_personnage.m_miracleFrappeEnCours) )
        {
            if (!jeu->hero.m_personnage.m_shooter)
            {
                if (jeu->hero.m_personnage.m_cible!=NULL)
                {

                    jeu->hero.m_personnage.m_vientDAttaquer = jeu->hero.m_personnage.m_cible->getCoordonnee();

                    if (fabs(jeu->hero.m_personnage.m_cible->getCoordonnee().x-jeu->hero.m_personnage.getCoordonnee().x)<=2
                      &&fabs(jeu->hero.m_personnage.m_cible->getCoordonnee().y-jeu->hero.m_personnage.getCoordonnee().y)<=2)
                        if (rand() % 100 < (float)((float)(jeu->hero.m_caracteristiques.dexterite + 100) / ((float)(jeu->hero.m_personnage.m_cible->getCaracteristique().dexterite + 100)))*75 )
                            if (!jeu->hero.m_personnage.m_cible->m_friendly && jeu->hero.m_personnage.m_cible->EnVie())
                            {
                                int degats = (rand()%(jeu->hero.m_personnage.getCaracteristique().degatsMax[PHYSIQUE] - jeu->hero.m_personnage.getCaracteristique().degatsMin[PHYSIQUE]+1))+jeu->hero.m_personnage.getCaracteristique().degatsMin[PHYSIQUE];

                                if(degats > 0)
                                    jeu->hero.JouerSonAttaque(jeu->hero.m_personnage.m_cible->m_materiau);

                                jeu->hero.m_personnage.m_vientDeFrapper = jeu->hero.m_personnage.m_cible;
                                jeu->hero.m_personnage.m_vientDAttaquer = jeu->hero.m_personnage.m_cible->getCoordonnee();
                                jeu->hero.m_personnage.m_degatsInflige  = degats;

                                jeu->hero.m_personnage.m_cible->m_vientDetreTouche = &jeu->hero.m_personnage;

                                jeu->map->InfligerDegats(jeu->hero.m_personnage.m_cible,&jeu->hero.m_personnage,degats,PHYSIQUE,jeu,0);

                                jeu->hero.m_personnage.InfligerDegats(-degats * jeu->hero.m_caracteristiques.volVie, 4,0);
                                jeu->hero.m_caracteristiques.foi += degats *jeu->hero.m_caracteristiques.volFoi;

                                jeu->hero.m_personnage.m_miracleFrappeEnCours = false;
                            }

                }
            }
            else
            {
                if (jeu->hero.m_personnage.m_cible!=NULL)
                    jeu->hero.m_personnage.m_vientDAttaquer = jeu->hero.m_personnage.m_cible->getProchaineCase();
                else
                    jeu->hero.m_personnage.m_vientDAttaquer = jeu->hero.m_case_visee;
            }

        }
        jeu->hero.miracleEnCours = 0;
    }

    if (retour == 2 || retour == 1)
    {
        if (jeu->hero.m_personnage.m_cible == NULL)
            jeu->hero.m_personnage.frappeEnCours=false,jeu->hero.m_personnage.setEtat(0,0);

        if (!eventManager->getEvenement(Mouse::Left,EventClic)
         && !eventManager->getEvenement(Mouse::Right,EventClic))
            jeu->hero.m_personnage.m_cible = NULL,jeu->hero.m_personnage.frappeEnCours=false,jeu->hero.m_personnage.setEtat(0,0);

        if (jeu->hero.m_personnage.m_cible != NULL)
            if(!jeu->hero.m_personnage.m_cible->EnVie())
                jeu->hero.m_personnage.m_cible = NULL,jeu->hero.m_personnage.frappeEnCours=false,jeu->hero.m_personnage.setEtat(0,0);
    }


    if (retour==1)
    {
        if (jeu->hero.m_personnage.m_cible != NULL)
            if (fabs(jeu->hero.m_personnage.m_cible->getCoordonnee().x - jeu->hero.m_personnage.getCoordonnee().x) > 1
             || fabs(jeu->hero.m_personnage.m_cible->getCoordonnee().y - jeu->hero.m_personnage.getCoordonnee().y) > 1)
                jeu->hero.m_personnage.frappeEnCours=false,jeu->hero.m_personnage.setEtat(0,0);
    }

    if(!jeu->hero.m_personnage.frappeEnCours)
        jeu->hero.m_personnage.m_lancementMiracleEnCours = false;

    jeu->map->Animer(&jeu->hero,temps2); // Animation des tiles de la jeu->map

    if(jeu->hero.m_personnage.m_vientDeToucher != NULL)
        jeu->hero.m_personnage.m_vientDeFrapper = jeu->hero.m_personnage.m_vientDeToucher, jeu->hero.m_personnage.m_vientDeToucher = NULL;

    jeu->map->GererMiracle(&jeu->hero.m_personnage,jeu->hero.m_classe.miracles,tempsEcoule,jeu);
}
void c_Jeu::Lumieres(Jeu *jeu)
{
    if(configuration->RafraichirOmbre > 0)
    {
        jeu->map->CalculerOmbresEtLumieres();

        configuration->RafraichirLumiere = true;
        configuration->RafraichirOmbre = false;

        moteurGraphique->LightManager->Generate(jeu->hero.m_personnage.m_entite_graphique.m_light);
    }
    else
    {
        configuration->RafraichirLumiere = false;
        configuration->RafraichirOmbre = true;
    }
}


void GestionRaccourcis(Jeu *jeu, bool diplace_mode = false)
{
    if(eventManager->getMolette() != 0)
    {
        int i = 0;
        for(int j = 0; j < 8 ; ++j)
            if(jeu->hero.m_raccourcis[j].miracle
            && jeu->hero.m_raccourcis[j].no == jeu->hero.m_miracle_droite[jeu->hero.m_weaponsSet])
                i = j + eventManager->getMolette();

        for(int j = 0; j < 8 ; ++j,i+=eventManager->getMolette())
        {
            if(i >= 8)
                i = 0;
            if(i < 0)
                i = 7;

            if(jeu->hero.m_raccourcis[i].miracle
            && jeu->hero.m_raccourcis[i].no >= 0
            && jeu->hero.m_raccourcis[i].no < (int)jeu->hero.m_classe.miracles.size())
                if(!jeu->hero.m_classe.miracles[jeu->hero.m_raccourcis[i].no].m_direct)
                    jeu->hero.m_miracle_droite[jeu->hero.m_weaponsSet] = jeu->hero.m_raccourcis[i].no, j = 8;
        }
    }

    int newmiracle = -1;

    if(!configuration->entering_text)
    for(int i = 0 ; i < 8 ; ++i)
    {
        if (eventManager->getEvenement(configuration->m_key_actions[K_SHORTCUT_1+i],EventKey))
        {
            if(!diplace_mode || eventManager->getEvenement(configuration->m_key_actions[K_SHORTCUT_1+i],EventKey))
                if(jeu->hero.m_raccourcis[i].no >= 0)
                {
                    if(jeu->hero.m_raccourcis[i].miracle)
                        newmiracle = jeu->hero.m_raccourcis[i].no;
                    else if(jeu->hero.m_raccourcis[i].no != jeu->hero.m_no_schema_craft &&
                            jeu->hero.m_raccourcis[i].no != jeu->hero.m_no_result_craft &&
                            jeu->hero.m_raccourcis[i].no != jeu->hero.m_no_schema_bless &&
                            jeu->hero.m_raccourcis[i].no != jeu->hero.m_no_result_bless)
                        jeu->hero.UtiliserObjet(jeu->hero.m_raccourcis[i].no);
                }

            if(eventManager->getEvenement(configuration->m_key_actions[K_SHORTCUT_1+i],EventKey))
                eventManager->StopEvenement(configuration->m_key_actions[K_SHORTCUT_1+i],EventKey);
            else
                eventManager->StopEvenement(Mouse::Left,EventClicA);
        }
    }

    if(newmiracle >= 0 && newmiracle < (int)jeu->hero.m_classe.miracles.size())
    {
        //if(jeu->hero.m_classe.miracles[newmiracle].m_direct)
        {
          //  if (!jeu->hero.m_personnage.frappeEnCours)
            {
                eventManager->StopEvenement(Mouse::Right,EventClic);

                coordonnee cible;

                if (jeu->map->getEntiteMonstre(jeu->map->getMonstreIllumine())!=NULL)
                    cible = jeu->map->getEntiteMonstre(jeu->map->getMonstreIllumine())->getProchaineCase();
                else
                    cible = eventManager->getCasePointee();

                if (jeu->hero.UtiliserMiracle(newmiracle, jeu->map->getEntiteMonstre(jeu->map->getMonstreIllumine()), cible, jeu))
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
        //else
          //  jeu->hero.m_personnage.m_miracleALancer = newmiracle;
    }
}

int GestionBoutons(Jeu *jeu, bool diplace_mode = false, bool inventory = false, bool hideLeft = false)
{
    jeu->hero.GererRaccourcisMiracles();
    jeu->hero.GererRaccourcisObjets(inventory, hideLeft);

    if(eventManager->getEvenement(sf::Key::Return,EventKey))
    {
        eventManager->StopEvenement(sf::Key::Return,EventKey);
        if(!configuration->entering_text)
        {
            configuration->entering_text = true;
            jeu->hero.m_personnage.m_speak.clear();
        }
        else
        {
            configuration->entering_text = false;
            if(!jeu->hero.m_personnage.m_speak.empty())
            jeu->hero.m_personnage.m_speak_time = 5000;
            jeu->SendMessage(jeu->hero.m_personnage.m_speak);

            console->Ajouter(jeu->hero.m_caracteristiques.nom + " : "+jeu->hero.m_personnage.m_speak);
        }
    }

    if(eventManager->getEvenement(sf::Key::Escape,EventKey) && configuration->entering_text)
    {
        eventManager->StopEvenement(sf::Key::Escape,EventKey);
        configuration->entering_text = false;
        jeu->hero.m_personnage.m_speak.clear();
        jeu->hero.m_personnage.m_speak_time = 0;
    }

    if(configuration->entering_text)
    {
        jeu->hero.m_personnage.m_speak_time = 1000;

        if(eventManager->IsEnteredText())
            jeu->hero.m_personnage.m_speak += eventManager->getChar();

        if(eventManager->getEvenement(sf::Key::Back,EventKey))
            if(!jeu->hero.m_personnage.m_speak.empty())
                jeu->hero.m_personnage.m_speak.erase(jeu->hero.m_personnage.m_speak.begin() + jeu->hero.m_personnage.m_speak.size() - 1);
        eventManager->StopEvenement(sf::Key::Back,EventKey);
    }

    if (   eventManager->getPositionSouris().x > AutoScreenAdjust(775,0).x
        && eventManager->getPositionSouris().x < AutoScreenAdjust(800,0).x
        && eventManager->getPositionSouris().y > AutoScreenAdjust(0,0).y
        && eventManager->getPositionSouris().y < AutoScreenAdjust(0,25).y
        && eventManager->getEvenement(Mouse::Left,EventClicA))
        {
            eventManager->StopEvenement(Mouse::Left,EventClicA);
            return 3;
        }

    int choix = -1;

    for(unsigned i = 0 ; i < jeu->hero.m_classe.boutons_menus_hud.size() ; ++i)
    {
        Bouton *bouton;

        if((jeu->hero.m_classe.boutons_menus_hud[i].lien == B_MIRACLES
         && jeu->hero.m_caracteristiques.miracles_restant > 0)
        || (jeu->hero.m_classe.boutons_menus_hud[i].lien == B_INVENTAIRE
         && jeu->hero.m_caracteristiques.pts_restant > 0)
        || (jeu->hero.m_classe.boutons_menus_hud[i].lien == B_QUETES
         && jeu->hero.newQuest)
        || (jeu->hero.m_classe.boutons_menus_hud[i].lien == B_DOCS
         && jeu->hero.newDoc))
            bouton = &jeu->hero.m_classe.boutons_menus_hud_2[i];
        else
            bouton = &jeu->hero.m_classe.boutons_menus_hud[i];

        sf::Sprite sprite;

        sprite.SetImage(*moteurGraphique->getImage(bouton->image.image));
        sprite.SetSubRect(sf::IntRect(bouton->image.position.x,
                                      bouton->image.position.y,
                                      bouton->image.position.w,
                                      bouton->image.position.h));

        sprite.SetPosition(AutoScreenAdjust(bouton->position.x,
                                            bouton->position.y));

        sprite.Resize(bouton->position.w, bouton->position.h);

        if(bouton->lien == B_MAP
        && configuration->Minimap)
            sprite.Move(0,9);
        if(bouton->lien == B_MIRACLES
        && jeu->next_screen == 5)
            sprite.Move(0,9);
        if(bouton->lien == B_INVENTAIRE
        && jeu->next_screen == 2)
            sprite.Move(0,9);
        if(bouton->lien == B_QUETES
        && jeu->next_screen == 6)
            sprite.Move(0,9);
        if(bouton->lien == B_MENU
        && jeu->next_screen == 4)
            sprite.Move(0,9);
        if(bouton->lien == B_CHAT
        && configuration->console != 0)
            sprite.Move(0,9);
        if(bouton->lien == B_DOCS
        && jeu->next_screen == 9)
            sprite.Move(0,9);


        moteurGraphique->AjouterCommande(&sprite, 17,0);

        if(bouton->Survol())
        {
            moteurGraphique->AjouterTexte(bouton->nom,coordonnee(eventManager->getPositionSouris().x,
                                                       eventManager->getPositionSouris().y - 20),
                                            jeu->hero.m_classe.border,
                                            19,0,12,sf::Color(224,224,224));
            if(eventManager->getEvenement(Mouse::Left,EventClicA))
            {
                eventManager->StopEvenement(Mouse::Left,EventClicA);

                if(!diplace_mode)
                    choix = bouton->lien;
            }
        }
    }

    if(!configuration->entering_text)
    {
        if(eventManager->getEvenement(configuration->m_key_actions[K_MAP],EventKey))
            eventManager->StopEvenement(configuration->m_key_actions[K_MAP],EventKey), choix = B_MAP;
        if(eventManager->getEvenement(configuration->m_key_actions[K_MIRACLES],EventKey))
            eventManager->StopEvenement(configuration->m_key_actions[K_MIRACLES],EventKey), choix = B_MIRACLES;
        if(eventManager->getEvenement(configuration->m_key_actions[K_INVENTORY],EventKey))
            eventManager->StopEvenement(configuration->m_key_actions[K_INVENTORY],EventKey), choix = B_INVENTAIRE;
        if(eventManager->getEvenement(configuration->m_key_actions[K_QUESTS],EventKey))
            eventManager->StopEvenement(configuration->m_key_actions[K_QUESTS],EventKey), choix = B_QUETES;
        if(eventManager->getEvenement(configuration->m_key_actions[K_DOCS],EventKey))
            eventManager->StopEvenement(configuration->m_key_actions[K_DOCS],EventKey), choix = B_DOCS;
        if(eventManager->getEvenement(configuration->m_key_actions[K_MENU],EventKey))
            eventManager->StopEvenement(configuration->m_key_actions[K_MENU],EventKey), choix = B_MENU;
        if(eventManager->getEvenement(Key::Tab,EventKey))
            eventManager->StopEvenement(Key::Tab,EventKey), choix = B_CHAT;


        if(eventManager->getEvenement(configuration->m_key_actions[K_CHANGE_WEAPONS],EventKey))
        {
            eventManager->StopEvenement(configuration->m_key_actions[K_CHANGE_WEAPONS],EventKey);
            jeu->hero.m_weaponsSet = (jeu->hero.m_weaponsSet == 1) ? 0 : 1;
            jeu->hero.RecalculerCaracteristiques(true);
            jeu->hero.ChargerModele();
            jeu->SendSkin();
        }
    }

    if(choix == B_MAP)
    {
        if (!configuration->Minimap)
            configuration->Minimap=true;
        else
            configuration->Minimap=false;
    }
    else if(choix == B_CHAT)
    {
        if (configuration->console==0)
            configuration->console=1;
        else if (configuration->console==1)
            configuration->console=2;
        else
            configuration->console=0;
    }
    else if(choix >= 0)
    {
        eventManager->StopEvenement(Key::I,EventKey);
        eventManager->StopEvenement(Mouse::Left,EventClicA);
        eventManager->StopEvenement(Mouse::Left,EventClic);
        jeu->map->m_defilerObjets=0;

        if(choix == B_INVENTAIRE)
            return 2;
        if(choix == B_QUETES)
            return 6;
        if(choix == B_DOCS)
            return 9;
        if(choix == B_MIRACLES)
            return 5;
        if(choix == B_MENU)
        {
            if(jeu->menu.m_dialogue.empty() || jeu->m_contexte != jeu->m_jeu)
                return 4;
            else if(!jeu->menu.m_forced_dialogue)
                jeu->menu.ClearSpeakChoice(),jeu->menu.m_dialogue.clear(),jeu->hero.m_personnage.m_cible = NULL;
        }
    }

    if(!jeu->menu.m_forced_dialogue)
    if (eventManager->getEvenement(Key::Escape,EventKey)
     ||(eventManager->getPositionSouris().x > AutoScreenAdjust(jeu->hero.m_classe.position_bouton_dialogue.x,0).x
     && eventManager->getPositionSouris().x < AutoScreenAdjust(jeu->hero.m_classe.position_bouton_dialogue.x + jeu->hero.m_classe.position_bouton_dialogue.w,0).x
     && eventManager->getPositionSouris().y > AutoScreenAdjust(0,jeu->hero.m_classe.position_bouton_dialogue.y + jeu->hero.m_classe.talk.position.h - jeu->menu.m_hauteur).y
     && eventManager->getPositionSouris().y < AutoScreenAdjust(0,jeu->hero.m_classe.position_bouton_dialogue.y + jeu->hero.m_classe.talk.position.h - jeu->menu.m_hauteur + jeu->hero.m_classe.position_bouton_dialogue.h).y
     && eventManager->getEvenement(Mouse::Left,EventClicA)))
    {
        if(!jeu->menu.m_dialogue.empty() /* && jeu->m_jeu->alpha_dialog > 192*/)
        {
            eventManager->StopEvenement(Key::Escape,EventKey);
            eventManager->StopEvenement(Mouse::Left,EventClicA);
            eventManager->StopEvenement(Mouse::Left,EventClic);
            jeu->menu.m_dialogue.clear();
            jeu->hero.m_personnage.m_cible = NULL;
        }
        jeu->menu.ClearSpeakChoice();
    }

    for(unsigned i = 0 ; i < jeu->hero.m_classe.boutons_menus_weapons_t.size() ; ++i)
    {
        Bouton *bouton;

        if(jeu->hero.m_weaponsSet == jeu->hero.m_classe.boutons_menus_weapons_t[i].lien)
            bouton = &jeu->hero.m_classe.boutons_menus_weapons_t_2[i];
        else
            bouton = &jeu->hero.m_classe.boutons_menus_weapons_t[i];

        sf::Sprite sprite;

        sprite.SetImage(*moteurGraphique->getImage(bouton->image.image));
        sprite.SetSubRect(sf::IntRect(bouton->image.position.x,
                                      bouton->image.position.y,
                                      bouton->image.position.w,
                                      bouton->image.position.h));

        sprite.SetPosition(AutoScreenAdjust(bouton->position.x,
                                            bouton->position.y));

        sprite.Resize(bouton->position.w, bouton->position.h);

        moteurGraphique->AjouterCommande(&sprite, 17,0);

        if(bouton->Survol())
        {
            moteurGraphique->AjouterTexte(bouton->nom,coordonnee(eventManager->getPositionSouris().x,
                                                       eventManager->getPositionSouris().y - 20),
                                            jeu->hero.m_classe.border,
                                            19,0,12,sf::Color(224,224,224));
            if(eventManager->getEvenement(Mouse::Left,EventClic))
            {
                eventManager->StopEvenement(Mouse::Left,EventClic);

                jeu->hero.m_weaponsSet = jeu->hero.m_classe.boutons_menus_weapons_t[i].lien;

                jeu->hero.ChargerModele();
                jeu->SendSkin();
                jeu->hero.RecalculerCaracteristiques(true);
            }
        }
    }


    return -1;
}


void c_Jeu::Evenements(Jeu *jeu)
{
    /*if(eventManager->getEvenement(sf::Key::S,EventKey))
        moteurSons->DebugRefreshSound();
    if(eventManager->getEvenement(sf::Key::L,EventKey))
    {
        eventManager->StopEvenement(sf::Key::L,EventKey);
        if(configuration->volume > 0)
        {
            debug_oldSound = configuration->volume;
            configuration->volume = 0;
        }
        else if(debug_oldSound > 0)
            configuration->volume = debug_oldSound;
        else
            configuration->volume = 100;
    }*/

    GestionRaccourcis(jeu, m_diplace_mode);
    jeu->next_screen = GestionBoutons(jeu, m_diplace_mode);

    if (jeu->next_screen >=0 )
        jeu->Next();

    if(!eventManager->getEvenement(Mouse::Left,EventClic))
        m_diplace_mode = false;


    if(!jeu->menu.m_dialogue.empty())
        jeu->map->m_monstreIllumine = -1;

    if(eventManager->getPositionSouris().y < 492 * configuration->Resolution.h/600)
    if(jeu->menu.m_dialogue.empty()
    || eventManager->getPositionSouris().x < AutoScreenAdjust(jeu->hero.m_classe.position_contenu_dialogue.x,0).x
    || eventManager->getPositionSouris().x > AutoScreenAdjust(jeu->hero.m_classe.position_contenu_dialogue.x,0).x
        + jeu->hero.m_classe.position_contenu_dialogue.w
    || eventManager->getPositionSouris().y < AutoScreenAdjust(0,jeu->hero.m_classe.position_contenu_dialogue.y).y
  /*  || eventManager->getPositionSouris().y > AutoScreenAdjust(0,jeu->hero.m_classe.position_contenu_dialogue.y).y
        + jeu->hero.m_classe.position_contenu_dialogue.h*/)
    {
        if(!eventManager->getEvenement(Mouse::Left,EventClic)
        && !eventManager->getEvenement(Mouse::Right,EventClic)
        && jeu->menu.m_dialogue.empty())
            jeu->map->getMonstre(eventManager->getCasePointee());

        if (eventManager->getEvenement(configuration->m_key_actions[K_PICKITEMS],EventKey))
            jeu->map->m_monstreIllumine = -1;

        if(jeu->map->getEntiteMonstre(jeu->map->m_monstreIllumine) != NULL)
            if(!jeu->map->getEntiteMonstre(jeu->map->m_monstreIllumine)->EnVie())
                jeu->map->m_monstreIllumine = -1, eventManager->StopEvenement(Mouse::Left,EventClic),
                                                  eventManager->StopEvenement(Mouse::Right,EventClic);

        bool attaque_normale_g = true;
        bool attaque_normale_d = true;
        if (eventManager->getEvenement(Mouse::Left,EventClic) && jeu->hero.m_miracle_gauche[jeu->hero.m_weaponsSet] >= 0)
        {
            if(eventManager->getEvenement(configuration->m_key_actions[K_STAND], EventKey) || jeu->map->getEntiteMonstre(jeu->map->getMonstreIllumine())!=NULL)
            {
                //if (!jeu->hero.m_personnage.frappeEnCours)
                {
                    coordonnee cible;

                    if (jeu->map->getEntiteMonstre(jeu->map->getMonstreIllumine())!=NULL)
                        cible = jeu->map->getEntiteMonstre(jeu->map->getMonstreIllumine())->getProchaineCase();
                    else
                        cible = eventManager->getCasePointee();

                    bool ok = true;

                    if (jeu->map->getEntiteMonstre(jeu->map->getMonstreIllumine())!=NULL)
                        if(jeu->map->getEntiteMonstre(jeu->map->getMonstreIllumine())->m_friendly)
                            ok = false;

                    jeu->hero.StopMiraclesFrappe();
                    if(ok)
                    if (jeu->hero.UtiliserMiracle(jeu->hero.m_miracle_gauche[jeu->hero.m_weaponsSet], jeu->map->getEntiteMonstre(jeu->map->getMonstreIllumine()), cible, jeu))
                    {
                        attaque_normale_g = false;
                        eventManager->StopEvenement(Mouse::Left,EventClicA);

                        if(!eventManager->getEvenement(configuration->m_key_actions[K_STAND], EventKey))
                            jeu->hero.m_personnage.m_miracleEnCours.back().m_infos.back()->m_cible = jeu->map->getEntiteMonstre(jeu->map->getMonstreIllumine());
                        else//if(eventManager->getEvenement(configuration->m_key_actions[K_STAND], EventKey))
                            jeu->hero.m_personnage.m_miracleEnCours.back().m_forced_maj = true;

                        coordonnee positionHero;
                        positionHero.x=(jeu->hero.m_personnage.getCoordonnee().x-jeu->hero.m_personnage.getCoordonnee().y-1)/5;
                        positionHero.y=(jeu->hero.m_personnage.getCoordonnee().x+jeu->hero.m_personnage.getCoordonnee().y)/5;

                        jeu->hero.m_personnage.setArrivee(jeu->hero.m_personnage.getProchaineCase());

                        jeu->hero.m_personnage.m_cible = NULL;
                    }
                }
            }
        }

        if (eventManager->getEvenement(Mouse::Right,EventClic))
        {
            if (eventManager->getEvenement(Mouse::Left,EventClic))
                eventManager->StopEvenement(Mouse::Left,EventClic);
            else
            {
                //if (!jeu->hero.m_personnage.frappeEnCours)
                {
                    coordonnee cible;

                    if (jeu->map->getEntiteMonstre(jeu->map->getMonstreIllumine())!=NULL)
                        cible = jeu->map->getEntiteMonstre(jeu->map->getMonstreIllumine())->getProchaineCase();
                    else
                        cible = eventManager->getCasePointee();

                    if (jeu->hero.UtiliserMiracle(jeu->hero.m_miracle_droite[jeu->hero.m_weaponsSet], jeu->map->getEntiteMonstre(jeu->map->getMonstreIllumine()), cible, jeu))
                    {
                        attaque_normale_d = false;
                        eventManager->StopEvenement(Mouse::Right,EventClic);

                        if(!eventManager->getEvenement(configuration->m_key_actions[K_STAND], EventKey))
                            jeu->hero.m_personnage.m_miracleEnCours.back().m_infos.back()->m_cible = jeu->map->getEntiteMonstre(jeu->map->getMonstreIllumine());
                        else
                            jeu->hero.m_personnage.m_miracleEnCours.back().m_forced_maj = true;

                        coordonnee positionHero;
                        positionHero.x=(jeu->hero.m_personnage.getCoordonnee().x-jeu->hero.m_personnage.getCoordonnee().y-1)/5;
                        positionHero.y=(jeu->hero.m_personnage.getCoordonnee().x+jeu->hero.m_personnage.getCoordonnee().y)/5;

                        jeu->hero.m_personnage.setArrivee(jeu->hero.m_personnage.getProchaineCase());

                        jeu->hero.m_personnage.m_cible = NULL;
                    }
                  /*  else if(!eventManager->getEvenement(Key::LShift,EventKey))
                        jeu->hero.m_personnage.setArrivee(eventManager->getCasePointee());
                    else
                        jeu->hero.m_personnage.setArrivee(jeu->hero.m_personnage.getProchaineCase());*/
                }
            }
        }

        if((attaque_normale_g || attaque_normale_d) && jeu->hero.m_select_friend < 0)
        {
            if((eventManager->getEvenement(Mouse::Left,EventClic)&&
                eventManager->getEvenement(Mouse::Left,EventClicA)&&
                attaque_normale_g)
            || (eventManager->getEvenement(Mouse::Right,EventClic)&&
                eventManager->getEvenement(Mouse::Right,EventClicA)&&
                attaque_normale_d))
            {
                if (jeu->hero.m_objetEnMain>=0)
                {
                    jeu->hero.m_objetADeposer=jeu->hero.m_objetEnMain;
                    jeu->map->AjouterObjet(jeu->hero.DeposerObjet());
                }
                jeu->hero.m_objetEnMain = -1;
                jeu->hero.m_miracleEnMain = -1;

                jeu->hero.m_personnage.m_miracleBloquant = false;
                jeu->hero.StopMiraclesFrappe();
                if (jeu->map->getMonstreIllumine() != -1)
                {
                    bool test=false;
                    if(jeu->hero.m_personnage.m_cible != NULL
                    && jeu->hero.m_personnage.getArrivee().x==jeu->hero.m_personnage.getCoordonnee().x
                    && jeu->hero.m_personnage.getArrivee().y==jeu->hero.m_personnage.getCoordonnee().y)
                        test=true;

                    //eventManager->StopEvenement(Mouse::Left,EventClicA);
                    jeu->hero.m_personnage.m_cible = jeu->map->getEntiteMonstre(jeu->map->getMonstreIllumine());

                    if (test)
                    {
                        jeu->hero.TestMonstreVise(jeu->hero.m_personnage.m_cible);
                        jeu->hero.m_case_visee = jeu->hero.m_personnage.m_cible->getProchaineCase();
                    }

                }
                else
                    jeu->hero.m_personnage.m_cible = NULL;
            }

            if((eventManager->getEvenement(Mouse::Left,EventClic) || eventManager->getEvenement(Mouse::Right,EventClic))
            && !eventManager->getEvenement(Key::LShift,EventKey))
            {
                m_diplace_mode = true;

                jeu->hero.StopMiraclesFrappe();

                if (jeu->hero.m_personnage.m_cible == NULL)
                {
                    jeu->hero.m_personnage.setArrivee(eventManager->getCasePointee());
                    jeu->hero.setSacVise(coordonnee (-1,-1,-1,-1));
                }
            }
        }

        if (eventManager->getEvenement(Mouse::Left,EventClic))
        {
            jeu->hero.setSacVise(jeu->map->getSacPointe());

            jeu->hero.m_objetVise=jeu->map->m_objetPointe;

            if (jeu->map->getSacPointe().x!=-1)
                eventManager->StopEvenement(Mouse::Left,EventClic),
                jeu->hero.m_personnage.setArrivee(jeu->map->getSacPointe());
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
    }

    if(fabs(jeu->menu.m_dialogue_position.x - jeu->hero.m_personnage.getCoordonnee().x) > 2
    || fabs(jeu->menu.m_dialogue_position.y - jeu->hero.m_personnage.getCoordonnee().y) > 2)
    if(!jeu->menu.m_forced_dialogue)
    {
        jeu->menu.ClearSpeakChoice();
        jeu->menu.m_dialogue.clear();
    }

    if(!jeu->menu.m_dialogue.empty() && !jeu->menu.m_forced_dialogue)
    if(eventManager->getEvenement(sf::Mouse::Left, EventClic))
    if(eventManager->getPositionSouris().x < AutoScreenAdjust(jeu->hero.m_classe.position_contenu_dialogue.x,0).x
    || eventManager->getPositionSouris().x > AutoScreenAdjust(jeu->hero.m_classe.position_contenu_dialogue.x,0).x + jeu->hero.m_classe.position_contenu_dialogue.w
    || eventManager->getPositionSouris().y < AutoScreenAdjust(0,jeu->hero.m_classe.position_contenu_dialogue.y + jeu->hero.m_classe.talk.position.h - jeu->menu.m_hauteur).y)//AutoScreenAdjust(0,jeu->hero.m_classe.talk.position.y).y)
    {
        jeu->menu.m_dialogue.clear();
        jeu->menu.ClearSpeakChoice();
        jeu->hero.m_personnage.m_cible = NULL;
    }
}


void c_Jeu::Affichage(Jeu *jeu)
{
    moteurGraphique->Gerer(tempsEcoule);

    jeu->map->Afficher(&jeu->hero,jeu->m_personnageClients,eventManager->getEvenement(configuration->m_key_actions[K_PICKITEMS],EventKey),alpha_map);

    jeu->hero.AfficherAmisEtCraft();

    if (jeu->hero.getChercherSac().x!=-1&&jeu->hero.m_objetVise>=0)
    {
        jeu->map->m_objetPointe=jeu->hero.m_objetVise;
        jeu->map->RamasserObjet(&jeu->hero);
        jeu->map->m_objetPointe=-1;
        jeu->hero.setChercherSac(coordonnee (-1,-1,-1,-1));
        jeu->hero.setSacVise(coordonnee (-1,-1,-1,-1));
    }

    jeu->menu.AfficherHUD(&jeu->hero.m_classe);

    if (jeu->map->getEntiteMonstre(jeu->map->getMonstreIllumine())!=NULL && jeu->hero.m_personnage.EnVie())
        jeu->menu.AfficherDynamique(jeu->hero.m_caracteristiques,1,jeu->map->getEntiteMonstre(jeu->map->getMonstreIllumine())->getCaracteristique(),&jeu->hero.m_classe);
    else
        jeu->menu.AfficherDynamique(jeu->hero.m_caracteristiques,0,jeu->hero.m_caracteristiques,&jeu->hero.m_classe);

    if(!eventManager->getEvenement(sf::Key::C,EventKey))
        eventManager->AfficherCurseur();

    if (configuration->effetMort&&configuration->postFX&&tempsEffetMort!=0.0f)
    {
        moteurGraphique->EffectMort.SetParameter("offset", 0.003*configuration->effetMort/100*(0.6+tempsEffetMort/10));
        moteurGraphique->EffectMort.SetParameter("color",1+0.5*configuration->effetMort/100*tempsEffetMort, 1-1*configuration->effetMort/100*tempsEffetMort, 1-1*configuration->effetMort/100*tempsEffetMort);
        moteurGraphique->EffectMort.SetParameter("black",1-configuration->effetMort/150);
    }

    if (configuration->console)
    {
        moteurGraphique->AjouterTexte(&fps,17);
        Version.SetY(20);
        //moteurGraphique->AjouterTexte(&Version,17);
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
        if (tempsEcouleDepuisFPS>0.1f)
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
void c_Jeu::FPS()
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

