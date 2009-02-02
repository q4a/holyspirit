
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

c_MenuInGame::c_MenuInGame(Jeu *jeu)
{
    m_alpha=0;
    retour=false;
}

void c_MenuInGame::Utiliser(Jeu *jeu)
{
    moteurGraphique.m_blur=0.0025*m_alpha/255;

    jeu->m_display=true;
    jeu->hero.placerCamera(&jeu->camera,jeu->map.getDimensions());
    jeu->ecran.SetView(jeu->camera);

    if(configuration.Lumiere)
        jeu->map.calculerOmbresEtLumieres(&jeu->ecran,&jeu->hero,&jeu->camera);

    temps_ecoule=0;
    temps_ecoule=jeu->Clock.GetElapsedTime();
    jeu->Clock.Reset();

    if(retour)
    {
        if(m_alpha>0)
            m_alpha-=temps_ecoule*1000;
        if(m_alpha<0)
            m_alpha=0,retour=false,jeu->m_contexte=jeu->m_jeu,moteurGraphique.m_blur=0;

    }
    else
    {
        if(m_alpha<255)
            m_alpha+=temps_ecoule*500;
        if(m_alpha>255)
            m_alpha=255;
    }



    jeu->eventManager.GererLesEvenements(&jeu->ecran,&jeu->camera,&jeu->m_run,temps_ecoule,jeu->map.getDimensions());

    if(jeu->eventManager.getEvenement(Key::Escape,"ET")&&m_alpha==255)
        jeu->eventManager.StopEvenement(Key::I,"ET"),retour=1;
    if(jeu->eventManager.getEvenement(Key::Space,"ET"))
        jeu->m_run=false;


   /* if(jeu->eventManager.getEvenement(Mouse::Left,"C"))
    {
        if(jeu->eventManager.getPositionSouris().y<configuration.Resolution.h/2)
            jeu->m_contexte=jeu->m_jeu,moteurGraphique.m_blur=0;
        else
            jeu->m_run=false;
    }*/

    coordonnee temp;

    jeu->map.Afficher(&jeu->ecran,&jeu->camera,1,&jeu->hero,temp,0);
    if(configuration.Minimap)
    {
        jeu->menu.Afficher(&jeu->ecran,2,255,&jeu->hero.m_classe);
        jeu->map.Afficher(&jeu->ecran,&jeu->camera,2,&jeu->hero,temp,255);
    }
    if(jeu->hero.getChercherSac().x!=-1&&jeu->map.getNombreObjets(jeu->hero.getChercherSac())>0)
    {
        jeu->menu.Afficher(&jeu->ecran,3,255,&jeu->hero.m_classe);
        jeu->map.Afficher(&jeu->ecran,&jeu->camera,2,&jeu->hero,temp,255);
    }
    jeu->menu.Afficher(&jeu->ecran,1,255,&jeu->hero.m_classe);
    jeu->menu.AfficherDynamique(&jeu->ecran,jeu->hero.m_caracteristiques,0,jeu->hero.m_caracteristiques,&jeu->hero.m_classe);


    texte.SetSize(56.f*configuration.Resolution.h/600);
    texte.SetText("Retour");

    texte.SetY(configuration.Resolution.h/2-texte.GetRect().GetHeight());
    texte.SetX(configuration.Resolution.w/2-texte.GetRect().GetWidth()/2);
    if(jeu->eventManager.getPositionSouris().y>configuration.Resolution.y/2-texte.GetRect().GetHeight()&&jeu->eventManager.getPositionSouris().y<configuration.Resolution.y/2)
    {
        texte.SetColor(Color(100,50,0,(int)m_alpha));
        if(jeu->eventManager.getEvenement(Mouse::Left,"C"))
            retour=1;

    }
    else
        texte.SetColor(Color(150,100,50,(int)m_alpha));
    moteurGraphique.AjouterTexte(&texte,18,1);

    texte.SetText("Sauvegarder et quitter");

    texte.SetY(configuration.Resolution.h/2);
    texte.SetX(configuration.Resolution.w/2-texte.GetRect().GetWidth()/2);
    if(jeu->eventManager.getPositionSouris().y>configuration.Resolution.y/2&&jeu->eventManager.getPositionSouris().y<configuration.Resolution.y/2+texte.GetRect().GetHeight())
    {
        texte.SetColor(Color(100,50,0,(int)m_alpha));
        if(jeu->eventManager.getEvenement(Mouse::Left,"C"))
            jeu->m_run=false;
    }
    else
        texte.SetColor(Color(150,100,50,(int)m_alpha));
    moteurGraphique.AjouterTexte(&texte,18,1);

    texte.SetText("Reinitialiser la partie et quitter");

    texte.SetY(configuration.Resolution.h/2+texte.GetRect().GetHeight());
    texte.SetX(configuration.Resolution.w/2-texte.GetRect().GetWidth()/2);
    if(jeu->eventManager.getPositionSouris().y>configuration.Resolution.y/2+texte.GetRect().GetHeight()&&jeu->eventManager.getPositionSouris().y<configuration.Resolution.y/2+texte.GetRect().GetHeight()*2)
    {
        texte.SetColor(Color(100,50,0,(int)m_alpha));
        if(jeu->eventManager.getEvenement(Mouse::Left,"C"))
            jeu->m_reset=true,jeu->m_run=false;
    }
    else
        texte.SetColor(Color(150,100,50,(int)m_alpha));
    moteurGraphique.AjouterTexte(&texte,18,1);

    jeu->eventManager.AfficherCurseur(&jeu->ecran);



}
