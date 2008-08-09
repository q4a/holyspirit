
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

c_Chargement::c_Chargement(Jeu *jeu)
{
    z=0;
    augmenterNoir = true;
}

void c_Chargement::setC_Chargement(int numeroMap,coordonnee coordonneePerso,bool debut)
{
    m_coordonneePerso.x=coordonneePerso.x;
    m_coordonneePerso.y=coordonneePerso.y;
    numeroProchaineMap=numeroMap;
    z=50;
    augmenterNoir = true;

    m_debut=debut;

    char chemin[128];
    sprintf(chemin,"Data/Maps/map%ld.map.hs",numeroProchaineMap);


    ifstream fichier;
    fichier.open(chemin, ios::in);
    if(fichier)
    {
        char caractere;
        do
        {
            //Chargement du nom
            fichier.get(caractere);
            if(caractere=='*')
            {
                getline(fichier, nomMap);
            }
        }while(caractere!='$');

        do
        {
            //Chargement du nom
            fichier.get(caractere);
            if(caractere=='*')
            {
                string chemin;
                getline(fichier, chemin);
                m_fond=moteurGraphique.AjouterImage(chemin);
            }
        }while(caractere!='$');
    }
    fichier.close();

    allerVersImageChargement=true;
    m_tempsChargement=0;
}

void c_Chargement::Utiliser(Jeu *jeu)
{
    jeu->m_display=true;
    jeu->hero.placerCamera(&jeu->camera,jeu->map.getDimensions());
    jeu->ecran.SetView(jeu->camera);

    if(configuration.Lumiere)
        jeu->map.calculerOmbresEtLumieres(&jeu->ecran,&jeu->hero,&jeu->camera);

    temps_ecoule=0;
    temps_ecoule=jeu->Clock.GetElapsedTime();
    tempsEcouleDepuisDernierAffichage+=temps_ecoule;
    jeu->Clock.Reset();

    if(z>=49&&!augmenterNoir&&allerVersImageChargement)
    {
        jeu->Clock.Reset();
        jeu->hero.m_personnage.setCoordonnee(m_coordonneePerso);

        if(!m_debut)
            jeu->map.Sauvegarder();

        jeu->hero.Sauvegarder();

        jeu->map.Detruire();

        if(!jeu->map.Charger(numeroProchaineMap))
            throw "CRITICAL ERROR";

        jeu->hero.placerCamera(&jeu->camera,jeu->map.getDimensions());
        jeu->camera.Zoom(configuration.zoom);

        coordonnee position;
        position.x=(jeu->hero.m_personnage.getCoordonnee().x-jeu->hero.m_personnage.getCoordonnee().y-1+jeu->map.getDimensions().y)/5;
        position.y=(jeu->hero.m_personnage.getCoordonnee().x+jeu->hero.m_personnage.getCoordonnee().y)/5;
        Listener::SetGlobalVolume((float)configuration.volume);
        Listener::SetPosition(-position.x, 0, position.y);
        Listener::SetTarget(0, 0, 1);
        jeu->map.musiquePlay(position);

        if(configuration.Lumiere)
            jeu->map.calculerOmbresEtLumieres(&jeu->ecran,&jeu->hero,&jeu->camera);

        while(jeu->Clock.GetElapsedTime()<1){}
        allerVersImageChargement=false;
    }


     if(z<=1&&augmenterNoir)
        augmenterNoir=false,z=1;
    if(z>=49&&!augmenterNoir)
        augmenterNoir=true,z=49;

    if(augmenterNoir)
        z-=temps_ecoule*200;
    else
        z+=temps_ecoule*200;


    if(z>50)
        z=50;
    if(z<0)
        z=0;


    if (sf::PostFX::CanUsePostFX() == true&&configuration.postFX)
    {
        if(allerVersImageChargement&&z<49&&augmenterNoir||!allerVersImageChargement&&z>0&&!augmenterNoir)
        {
            jeu->camera.Zoom(configuration.zoom);
            jeu->map.setVolumeMusique((int)(z*(float)configuration.volume/50));
            if(!m_debut&&augmenterNoir||!augmenterNoir)
            {
                coordonnee temp;
                jeu->map.Afficher(&jeu->ecran,&jeu->camera,1,&jeu->hero,temp);

                if(configuration.Minimap)
                {
                    jeu->menu.Afficher(&jeu->ecran,1);
                    jeu->menu.Afficher(&jeu->ecran,2);
                }
                jeu->menu.Afficher(&jeu->ecran,3);
                jeu->menu.AfficherDynamique(&jeu->ecran,jeu->hero.m_personnage.getCaracteristique(),0,jeu->hero.m_personnage.getCaracteristique());
            }
        }
        else
        jeu->menu.AfficherChargement(&jeu->ecran,nomMap,m_fond,50);

        jeu->EffectNoir.SetParameter("color", ((float)z)/50, ((float)z)/50, ((float)z)/50);

    }
    else
    {
        if(allerVersImageChargement&&z<49&&augmenterNoir||!allerVersImageChargement&&z>0&&!augmenterNoir)
        {
            jeu->map.setVolumeMusique((int)(z*(float)configuration.volume/50));
            if(!m_debut&&augmenterNoir||!augmenterNoir)
            {
                coordonnee temp;
                jeu->map.Afficher(&jeu->ecran,&jeu->camera,1,&jeu->hero,temp);
                if(configuration.Minimap)
                {
                    jeu->menu.Afficher(&jeu->ecran,1);
                    jeu->menu.Afficher(&jeu->ecran,2);
                }
                jeu->menu.Afficher(&jeu->ecran,3);
                jeu->menu.AfficherDynamique(&jeu->ecran,jeu->hero.m_personnage.getCaracteristique(),0,jeu->hero.m_personnage.getCaracteristique());
            }
        }
        else
            jeu->menu.AfficherChargement(&jeu->ecran,nomMap,m_fond,(int)z);
    }

    if(z>=49&&!augmenterNoir&&!allerVersImageChargement)
    {
        jeu->m_contexte = jeu->m_jeu;
       // jeu->m_contexte->CopierCamera(jeu->m_jeu->camera);
    }
}
