
#include "c_jeu.h"
#include "jeu.h"

#include "Globale.h"

#include <iostream>
#include <fstream>

#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

using namespace sf;
using namespace std;

c_Inventaire::c_Inventaire(Jeu *jeu)
{
    m_decalage=-600;
}

void c_Inventaire::Utiliser(Jeu *jeu)
{

    if(m_decalage<=-600)
        m_afficher=1;


        temps_ecoule=jeu->Clock.GetElapsedTime();
        jeu->m_display=true;
        jeu->Clock.Reset();

    jeu->eventManager.GererLesEvenements(&jeu->ecran,&camera,&jeu->m_run,0,jeu->map.getDimensions());

    if(jeu->hero.m_objetEnMain==-1)
        jeu->eventManager.AfficherCurseur(&jeu->ecran);

    if(jeu->eventManager.getEvenement(Key::I,"ET")||jeu->eventManager.getEvenement(Key::Escape,"ET"))
        m_afficher=0;

    if(jeu->eventManager.getEvenement(Mouse::Left,"C"))
    {
        if(jeu->hero.m_objetEnMain==-1&&jeu->map.getObjetPointe()==-1&&jeu->eventManager.getPositionSouris().x>436*configuration.Resolution.w/800&&jeu->eventManager.getPositionSouris().x<756*configuration.Resolution.w/800&&jeu->eventManager.getPositionSouris().y>30*configuration.Resolution.h/600&&jeu->eventManager.getPositionSouris().y<30*configuration.Resolution.h/600+20*configuration.Resolution.w/800)
            jeu->map.m_defilerObjets--,jeu->eventManager.StopEvenement(Mouse::Left,"C");

        if(jeu->hero.m_objetEnMain==-1&&jeu->map.getObjetPointe()==-1&&jeu->eventManager.getPositionSouris().x>436*configuration.Resolution.w/800&&jeu->eventManager.getPositionSouris().x<756*configuration.Resolution.w/800&&jeu->eventManager.getPositionSouris().y>30*configuration.Resolution.h/600+3*20*configuration.Resolution.w/800&&jeu->eventManager.getPositionSouris().y<30*configuration.Resolution.h/600+4*20*configuration.Resolution.w/800)
            jeu->map.m_defilerObjets++,jeu->eventManager.StopEvenement(Mouse::Left,"C");
    }

    if(jeu->eventManager.getEvenement(Mouse::Left,"C"))
    {
        if(jeu->map.ramasserObjet(&jeu->hero,1))
            jeu->eventManager.StopEvenement(Mouse::Left,"C");
    }

    if(jeu->eventManager.getEvenement(Mouse::Left,"C"))
    {
        jeu->map.AjouterObjet(jeu->hero.prendreEnMain(jeu->eventManager.getPositionSouris()));
        jeu->eventManager.StopEvenement(Mouse::Left,"C");
    }

    jeu->hero.placerCamera(&jeu->camera,jeu->map.getDimensions()); // On place la camera suivant ou se trouve le perso
    jeu->camera.Zoom(configuration.zoom);

    coordonnee temp;
    jeu->map.Afficher(&jeu->ecran,&jeu->camera,1,&jeu->hero,temp,0);

    if(configuration.Minimap)
    {
        jeu->menu.Afficher(&jeu->ecran,2);
        jeu->map.Afficher(&jeu->ecran,&jeu->camera,2,&jeu->hero,temp,255);
    }
    if(jeu->hero.getChercherSac().x!=-1&&jeu->map.getNombreObjets(jeu->hero.getChercherSac())>0)
    {
        jeu->menu.Afficher(&jeu->ecran,3);
        jeu->map.Afficher(&jeu->ecran,&jeu->camera,2,&jeu->hero,temp,255);
    }
    //jeu->menu.AfficherDynamique(&jeu->ecran,jeu->hero.m_personnage.getCaracteristique(),0,jeu->hero.m_personnage.getCaracteristique());

    if(m_afficher)
        m_decalage+=temps_ecoule*2000;
    else
        m_decalage-=temps_ecoule*2000;

    if(m_decalage>0)
        m_decalage=0;
    if(m_decalage<-600)
    {
        m_decalage=-600;
        jeu->m_contexte=jeu->m_jeu,jeu->eventManager.StopEvenement(Key::I,"ET");
    }

    jeu->menu.AfficherInventaire(&jeu->ecran,m_decalage);

    jeu->hero.afficherInventaire(&jeu->ecran,jeu->eventManager.getPositionSouris(),m_decalage);

    jeu->map.AfficherSacInventaire(&jeu->ecran,jeu->hero.m_personnage.getCoordonnee(),m_decalage,jeu->eventManager.getPositionSouris());

    jeu->menu.Afficher(&jeu->ecran,1);
    jeu->menu.AfficherDynamique(&jeu->ecran,jeu->hero.m_caracteristiques,-1,jeu->hero.m_caracteristiques);

}


