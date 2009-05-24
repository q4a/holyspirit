

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


using namespace sf;

c_Jeu::c_Jeu(Jeu *jeu)
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
    sprintf(chaine,"v %s",configuration->version.c_str());
    Version.SetText(chaine);
    Version.SetY(20);

    Temps.SetSize(16.f);
    sprintf(chaine,"Temps : %i h %i ",configuration->heure,(int)configuration->minute);
    Temps.SetText(chaine);
    Temps.SetY(40);

    TourBoucle.SetX(120);
    TourBoucle.SetSize(16.f);

    // Chargement des menus

    console->Ajouter("",0);
    console->Ajouter("Chargement des sons :",0);


    if (!jeu->bufferSonMort.LoadFromFile(configuration->chemin_son_mort))
        console->Ajouter("Impossible de charger : "+configuration->chemin_son_mort,1);
    else
        console->Ajouter("Chargement de : "+configuration->chemin_son_mort,0);

    jeu->sonMort.SetBuffer(jeu->bufferSonMort);
    jeu->sonMort.SetVolume(0);
    jeu->sonMort.SetLoop(true);
    jeu->sonMort.Play();



    jeu->hero.ChargerModele(); // Chargement du héro

    //jeu->hero.m_personnage.Charger(&jeu->hero.m_modelePersonnage);

    if (!jeu->map->Charger("Begin.map.hs",&jeu->hero)==1) // Chargement de  jeu->map0.txt
        throw("CRITICAL ERROR");

    if (configuration->Lumiere)
    {
        jeu->map->CalculerOmbresEtLumieres();
        lumiere=false;
    }

    jeu->Clock.Reset();

    alpha_map=0;
    alpha_sac=0;
    lowFPS=-1;
}

void c_Jeu::Utiliser(Jeu *jeu)
{
    //Gestion du temps
    jeu->m_display=false;
    tempsEcoule = jeu->Clock.GetElapsedTime();

    if (tempsEcoule>0.1)
        tempsEcoule=0.1;

    if (tempsEcoule>0.001/*&&configuration->syncronisation_verticale||!configuration->syncronisation_verticale*/)
    {
        GererTemps(jeu);
        Sauvegarder(jeu);
        IA(jeu);
        Deplacements(jeu);
        Animation(jeu);
        Lumieres(jeu);

        if (tempsEcouleDepuisDernierAffichage>0.01&&configuration->syncronisation_verticale||!configuration->syncronisation_verticale)
        {
            jeu->hero.PlacerCamera(jeu->map->getDimensions()); // On place la camera suivant ou se trouve le perso

            Evenements(jeu);
            Affichage(jeu);
            tempsEcouleDepuisDernierAffichage=0;
        }
        FPS(jeu);
    }
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
    if (!jeu->hero.m_personnage.enVie())
        tempsEffetMort=1;
    if (tempsEffetMort>1)
        augmenter=false;
    if (tempsEffetMort<0)
        augmenter=true;
    tempsEcouleDepuisFPS+=tempsEcoule;

    jeu->hero.AugmenterAme(tempsEcoule);
    jeu->hero.RecalculerCaracteristiques();
    if (jeu->hero.m_personnage.enVie())
    {
        jeu->hero.RegenererVie((float)jeu->hero.m_caracteristiques.maxVie*(float)(tempsEcoule/100));
        jeu->hero.RegenererFoi((float)jeu->hero.m_caracteristiques.maxFoi*(float)(tempsEcoule/100));
    }

    jeu->Clock.Reset();

    if (jeu->hero.m_personnage.enVie())
    {
        if (jeu->hero.m_caracteristiques.maxVie!=0)
            if (jeu->hero.m_caracteristiques.vie/(float)jeu->hero.m_caracteristiques.maxVie<0.5)
                configuration->effetMort=150-(jeu->hero.m_caracteristiques.vie*300/jeu->hero.m_caracteristiques.maxVie),jeu->sonMort.SetVolume(configuration->effetMort);
            else
                configuration->effetMort=0,jeu->sonMort.SetVolume(0);
    }
    else
        configuration->effetMort=150;
}
void c_Jeu::Sauvegarder(Jeu *jeu)
{
    if (tempsSauvergarde>=configuration->frequence_sauvegarde)
    {
        jeu->map->Sauvegarder(&jeu->hero);
        jeu->hero.Sauvegarder();
        tempsSauvergarde=0;
    }
}
void c_Jeu::IA(Jeu *jeu)
{
    if (tempsEcouleDepuisDernierIA>=0.027)
    {
        jeu->map->GererMonstres(jeu,&jeu->hero,tempsEcouleDepuisDernierIA,&jeu->menu);
        tempsEcouleDepuisDernierIA=0;
    }
}
void c_Jeu::Deplacements(Jeu *jeu)
{
    if (tempsEcouleDepuisDernierDeplacement>=0.0)
    {
        coordonnee temp={-1 ,-1 ,-1 ,-1};
        if (jeu->hero.getMonstreVise()==-1)
            temp.x=jeu->hero.m_personnage.getCoordonnee().x,temp.y=jeu->hero.m_personnage.getCoordonnee().y;

        if (jeu->hero.m_personnage.seDeplacer(tempsEcouleDepuisDernierDeplacement*100,jeu->map->getDimensions())/*&&!jeu->eventManager.getEvenement(Key::LShift,"ET")||jeu->eventManager.getEvenement(Key::LShift,"ET")*/)
        {
            bool ok=true;
            if (jeu->hero.getMonstreVise()>-1)
                if (jeu->hero.TestMonstreVise(jeu->map->getEntiteMonstre(jeu->hero.getMonstreVise()),jeu->map->getDimensions().y))
                    ok=false;

            if (jeu->hero.m_personnage.getCoordonnee().x==jeu->hero.m_personnage.getArrivee().x && jeu->hero.m_personnage.getCoordonnee().y==jeu->hero.m_personnage.getArrivee().y)
                ok=false;

            if (ok)
                jeu->hero.m_personnage.pathfinding(jeu->map->getAlentourDuPersonnage(jeu->hero.m_personnage.getCoordonnee()),temp); // Recherche du chemin

            if (jeu->eventManager.getEvenement(Mouse::Left,"CA")&&jeu->eventManager.getEvenement(Key::LShift,"ET"))
            {
                coordonnee temp={(int)(jeu->eventManager.getCasePointee().x*COTE_TILE),(int)(jeu->eventManager.getCasePointee().y*COTE_TILE),0,0};
                jeu->hero.m_personnage.frappe(jeu->hero.m_personnage.getCoordonneePixel(),temp);
            }
        }
        jeu->map->TestEvenement(jeu,tempsEcoule); // On test les événement pour voir s'il on doit changer de jeu->map, faire des dégats au perso, le régénérer, etc


        ///Placer l'écouteur, à la position du héro
        coordonnee position;
        position.x=(jeu->hero.m_personnage.getCoordonnee().x-jeu->hero.m_personnage.getCoordonnee().y-1)/5;
        position.y=(jeu->hero.m_personnage.getCoordonnee().x+jeu->hero.m_personnage.getCoordonnee().y)/5;

        Listener::SetGlobalVolume((float)configuration->volume);
        Listener::SetPosition(-position.x, 0, position.y);
        Listener::SetTarget(0, 0, 1);
        jeu->map->MusiquePlay(position);
        jeu->sonMort.SetPosition(position.x,0,position.y);

        jeu->hero.m_personnage.AjouterPointAme(jeu->menu.GererDynamique(tempsEcoule));

        tempsEcouleDepuisDernierDeplacement=0;
    }
}
void c_Jeu::Animation(Jeu *jeu)
{
    if (tempsDepuisDerniereAnimation>0.04)
    {
        if (tempsDepuisDerniereAnimation>0.8)
            tempsDepuisDerniereAnimation=0.8;

        coordonnee positionHero;
        positionHero.x=(jeu->hero.m_personnage.getCoordonnee().x-jeu->hero.m_personnage.getCoordonnee().y-1+jeu->map->getDimensions().y)/5;
        positionHero.y=(jeu->hero.m_personnage.getCoordonnee().x+jeu->hero.m_personnage.getCoordonnee().y)/5;

        bool a; // Variable qui ne sert pas ici, mais qui remplace le explosif des monstres
        int retour=-2;
        retour=jeu->hero.m_personnage.animer(&jeu->hero.m_modelePersonnage[0],tempsDepuisDerniereAnimation,&a,positionHero);

        jeu->hero.CalculerOrdreAffichage();

        if (retour==0) //Animation du héro
        {
            if (jeu->hero.miracleEnCours==1)
                jeu->map->InfligerDegatsMasse(jeu->hero.m_personnage.getCoordonnee(),1,rand()%(jeu->hero.m_caracteristiques.degatsMax - jeu->hero.m_caracteristiques.degatsMin +1) + jeu->hero.m_caracteristiques.degatsMin ,false,&jeu->hero,&jeu->menu);
            else if (jeu->hero.m_personnage.frappeEnCours)
            {
                if (!jeu->hero.m_personnage.m_shooter)
                    if (jeu->map->getEntiteMonstre(jeu->hero.getMonstreVise())!=NULL)
                        if (fabs(jeu->map->getEntiteMonstre(jeu->hero.getMonstreVise())->getCoordonnee().x-jeu->hero.m_personnage.getCoordonnee().x)<=2&&fabs(jeu->map->getEntiteMonstre(jeu->hero.getMonstreVise())->getCoordonnee().y-jeu->hero.m_personnage.getCoordonnee().y)<=2)
                            jeu->map->InfligerDegats(jeu->hero.getMonstreVise(),(rand()%(jeu->hero.m_personnage.getCaracteristique().degatsMax - jeu->hero.m_personnage.getCaracteristique().degatsMin+1))+jeu->hero.m_personnage.getCaracteristique().degatsMin,&jeu->menu,&jeu->hero,1);

                coordonnee cible;

                if (jeu->map->getEntiteMonstre(jeu->hero.getMonstreVise())!=NULL)
                    cible=jeu->map->getEntiteMonstre(jeu->hero.getMonstreVise())->getCoordonnee();
                else
                    cible=jeu->eventManager.getCasePointee();

                if (jeu->hero.AjouterMiracleArme())
                    jeu->map->GererMiracle(&jeu->hero.m_personnage.m_miracleEnCours.back(),&jeu->hero.m_classe.miracles[jeu->hero.m_personnage.m_miracleEnCours.back().m_modele],&jeu->hero,0,jeu->hero.m_personnage.getCoordonnee(),cible,1);

            }
            jeu->hero.miracleEnCours=0;
        }

        jeu->map->AnimerMiracle(&jeu->hero.m_personnage,jeu->hero.m_classe.miracles,tempsDepuisDerniereAnimation,positionHero,&jeu->hero);

        if (retour==2)
            if (!jeu->eventManager.getEvenement(Mouse::Left,"C") || !jeu->map->getMonstreEnVie(jeu->hero.getMonstreVise()) )
                jeu->hero.setMonstreVise(-1),jeu->hero.m_personnage.frappeEnCours=false,jeu->hero.m_personnage.setEtat(0);

        if (retour==1)
            if (!jeu->eventManager.getEvenement(Mouse::Left,"C"))
                jeu->hero.setMonstreVise(-1),jeu->hero.m_personnage.frappeEnCours=false,jeu->hero.m_personnage.setEtat(0);


        jeu->map->Animer(&jeu->hero,tempsDepuisDerniereAnimation,&jeu->menu); // Animation des tiles de la jeu->map

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



void c_Jeu::Evenements(Jeu *jeu)
{
    int monstreVise=-1;

    if (!jeu->eventManager.getEvenement(Mouse::Left,"C"))
        jeu->map->getMonstre(&jeu->hero,jeu->eventManager.getPositionSouris(),jeu->eventManager.getCasePointee());

    if (jeu->eventManager.getEvenement(Mouse::Left,"CA")&&!jeu->eventManager.getEvenement(Key::LShift,"ET"))
    {
        if (!(jeu->eventManager.getPositionSouris().x>configuration->Resolution.w-configuration->Resolution.w*0.25
                &&jeu->eventManager.getPositionSouris().y>configuration->Resolution.w*0.25&&jeu->eventManager.getPositionSouris().y<configuration->Resolution.w*0.25+configuration->Resolution.w*0.34
                &&alpha_sac>=128)||alpha_sac<=128)
        {
            if (jeu->hero.getMonstreVise()==-1)
            {
                jeu->hero.m_personnage.setArrivee(jeu->eventManager.getCasePointee());

                coordonnee temp={-1,-1,-1,-1};
                jeu->hero.setSacVise(temp);
            }
        }
        else if (jeu->hero.getChercherSac().x!=-1&&jeu->map->getObjetPointe()!=-1)
        {
            jeu->map->RamasserObjet(&jeu->hero);
            jeu->eventManager.StopEvenement(Mouse::Left,"CA");
        }
    }

    if (jeu->eventManager.getEvenement(Mouse::Left,"C")&&jeu->eventManager.getEvenement(Mouse::Left,"CA"))
    {
        if (jeu->map->getMonstreIllumine()!=-1)
        {
            bool test=false;
            if (jeu->hero.getMonstreVise()>-1&&jeu->hero.m_personnage.getArrivee().x==jeu->hero.m_personnage.getCoordonnee().x&&jeu->hero.m_personnage.getArrivee().y==jeu->hero.m_personnage.getCoordonnee().y)
                test=true;

            jeu->eventManager.StopEvenement(Mouse::Left,"CA");
            jeu->hero.setMonstreVise(jeu->map->getMonstreIllumine());
            if (test)
                jeu->hero.TestMonstreVise(jeu->map->getEntiteMonstre(jeu->hero.getMonstreVise()),jeu->map->getDimensions().y);
        }
        else
            jeu->hero.setMonstreVise(-1);
    }
    if (jeu->eventManager.getEvenement(Mouse::Left,"C")&&jeu->eventManager.getEvenement(Mouse::Left,"CA"))
    {
        jeu->hero.setSacVise(jeu->map->getSacPointe());

        if (jeu->map->getSacPointe().x!=-1)
            jeu->eventManager.StopEvenement(Mouse::Left,"CA");
    }

    if (jeu->hero.getSacVise().x!=-1)
    {
        if (jeu->hero.getSacVise().x==jeu->hero.m_personnage.getCoordonnee().x&&jeu->hero.getSacVise().y==jeu->hero.m_personnage.getCoordonnee().y)
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

    if (jeu->eventManager.getEvenement(Mouse::Right,"C"))
    {
        if (jeu->eventManager.getEvenement(Mouse::Left,"C"))
            jeu->eventManager.StopEvenement(Mouse::Left,"C");
        else
            jeu->hero.UtiliserClicDroit(jeu->eventManager.getPositionSouris(),monstreVise);
    }

    if (jeu->eventManager.getEvenement(Key::I,"ET"))
        jeu->eventManager.StopEvenement(Key::I,"ET"),jeu->map->m_defilerObjets=0,jeu->m_contexte=jeu->m_inventaire;

    if (jeu->eventManager.getEvenement(Key::Escape,"ET"))
        jeu->eventManager.StopEvenement(Key::Escape,"ET"),jeu->m_contexte=jeu->m_menuInGame;

    if (alpha_sac>128)
    {
        if (jeu->eventManager.getEvenement(Mouse::Left,"C")&&jeu->eventManager.getPositionSouris().x>configuration->Resolution.w-configuration->Resolution.w*0.25&&jeu->eventManager.getPositionSouris().y>configuration->Resolution.w*0.265+10*20*configuration->Resolution.w/800+20*configuration->Resolution.w/800&&jeu->eventManager.getPositionSouris().y<configuration->Resolution.w*0.265+11*20*configuration->Resolution.w/800+20*configuration->Resolution.w/800)
            jeu->map->m_defilerObjets++,jeu->eventManager.StopEvenement(Mouse::Left,"C");

        if (jeu->eventManager.getEvenement(Mouse::Left,"C")&&jeu->eventManager.getPositionSouris().x>configuration->Resolution.w-configuration->Resolution.w*0.25&&jeu->eventManager.getPositionSouris().y>configuration->Resolution.w*0.265&&jeu->eventManager.getPositionSouris().y<configuration->Resolution.w*0.265+20*configuration->Resolution.w/800)
            jeu->map->m_defilerObjets--,jeu->eventManager.StopEvenement(Mouse::Left,"C");
    }
}
void c_Jeu::Affichage(Jeu *jeu)
{
    moteurGraphique->Gerer(tempsEcouleDepuisDernierAffichage,jeu->map->getDimensions().y);

    jeu->map->Afficher(&jeu->hero,jeu->eventManager.getPositionSouris(),jeu->eventManager.getEvenement(Key::LAlt,"ET"),alpha_map);//Affichage de la jeu->map

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

    if (jeu->hero.getChercherSac().x!=-1&&jeu->map->getNombreObjets(jeu->hero.getChercherSac())>=1)
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

    if (jeu->hero.getChercherSac().x!=-1&&jeu->map->getNombreObjets(jeu->hero.getChercherSac())==1&&alpha_sac<128)
        jeu->map->m_objetPointe=0,jeu->map->RamasserObjet(&jeu->hero);

    if (alpha_sac>0)
    {
        coordonnee temp={600,(int)((float)configuration->Resolution.w*0.265),200,10};
        jeu->menu.Afficher(3,alpha_sac,&jeu->hero.m_classe);
        jeu->map->AfficherSac(jeu->hero.getChercherSac(),0,jeu->eventManager.getPositionSouris(),temp,jeu->hero.m_caracteristiques);
    }

    jeu->menu.Afficher(1,255,&jeu->hero.m_classe); // On affiche le hud

    if (jeu->map->getEntiteMonstre(jeu->map->getMonstreIllumine())!=NULL)
        jeu->menu.AfficherDynamique(jeu->hero.m_caracteristiques,1,jeu->map->getEntiteMonstre(jeu->map->getMonstreIllumine())->getCaracteristique(),&jeu->hero.m_classe);
    else
        jeu->menu.AfficherDynamique(jeu->hero.m_caracteristiques,0,jeu->hero.m_caracteristiques,&jeu->hero.m_classe);
    jeu->eventManager.AfficherCurseur(); // On affiche le curseur de la souris

    if (jeu->map->getEvenement(jeu->eventManager.getCasePointee())>=0)
        jeu->map->AfficherNomEvenement(jeu->eventManager.getCasePointee(),jeu->eventManager.getPositionSouris());

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
            if (configuration->console&&moteurGraphique->GetFPS()!=0)
                sprintf(chaine,"%i / %i FPS",moteurGraphique->GetFPS(), (int)lowFPS);
            //  Calcule du nombre de FPS
            fps.SetText(chaine);


            sprintf(chaine,"Temps : %i h %i ",configuration->heure,(int)configuration->minute);
            Temps.SetText(chaine);

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

        sprintf(chaine,"Nbr Tour de boucle : %i",nbrTourBoucle);
        TourBoucle.SetText(chaine);
        nbrTourBoucle=0;

        tempsNbrTourBoucle=0;
    }
}

