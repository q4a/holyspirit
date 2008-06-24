
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
    if (sf::PostFX::CanUsePostFX() == true&&configuration.postFX)
    {

        if(!EffectNoir.LoadFromFile(configuration.chemin_fx+configuration.nom_effetNoir))
            console.Ajouter("Impossible de charger : "+configuration.chemin_fx+configuration.nom_effetNoir,1);
        else
        {
            console.Ajouter("Chargement de : "+configuration.chemin_fx+configuration.nom_effetNoir,0);
            EffectNoir.SetTexture("framebuffer", NULL);
            EffectNoir.SetParameter("color", 0.f, 0.f, 0.f);
        }
    }

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
    sprintf(chemin,"Data/Maps/map%ld.txt",numeroProchaineMap);


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
    }
    fichier.close();
}

void c_Chargement::Utiliser(Jeu *jeu)
{
    jeu->m_display=false;
    jeu->hero.placerCamera(&camera,jeu->map.getDimensions());
    jeu->ecran.SetView(camera);

    if(configuration.Lumiere)
        jeu->map.calculerOmbresEtLumieres(&jeu->ecran,&jeu->hero,&camera);

    temps_ecoule=0;
    temps_ecoule=jeu->Clock.GetElapsedTime();
    tempsEcouleDepuisDernierAffichage+=temps_ecoule;
    jeu->Clock.Reset();

    if(z<=0&&augmenterNoir)
    {
        jeu->hero.m_personnage.setCoordonnee(m_coordonneePerso);

        jeu->map.Detruire();

        if(!jeu->map.Charger(numeroProchaineMap))
            throw "CRITICAL ERROR";

        jeu->hero.placerCamera(&camera,jeu->map.getDimensions());

        coordonnee position;
        position.x=(jeu->hero.m_personnage.getCoordonnee().x-jeu->hero.m_personnage.getCoordonnee().y-1+jeu->map.getDimensions().y)/5;
        position.y=(jeu->hero.m_personnage.getCoordonnee().x+jeu->hero.m_personnage.getCoordonnee().y)/5;
        Listener::SetGlobalVolume((float)configuration.volume);
        Listener::SetPosition(-position.x, 0, position.y);
        Listener::SetTarget(0, 0, 1);
        jeu->map.musiquePlay(position);

        if(configuration.Lumiere)
            jeu->map.calculerOmbresEtLumieres(&jeu->ecran,&jeu->hero,&camera);

        jeu->Clock.Reset();

        augmenterNoir=false;
    }

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
        camera.Zoom(configuration.zoom);
        jeu->map.setVolumeMusique((int)(z*(float)configuration.volume/50));
        jeu->ecran.SetView(jeu->ecran.GetDefaultView());
        if(!m_debut&&augmenterNoir||!augmenterNoir)
        {
            jeu->ecran.SetView(camera);
            coordonnee temp;
            jeu->map.Afficher(&jeu->ecran,&camera,1,&jeu->hero,temp);
            jeu->ecran.SetView(jeu->ecran.GetDefaultView());

            if(configuration.Minimap)
            {
                jeu->menu.Afficher(&jeu->ecran,1);
                jeu->menu.Afficher(&jeu->ecran,2);
            }
            jeu->menu.Afficher(&jeu->ecran,3);
            jeu->menu.AfficherDynamique(&jeu->ecran,jeu->hero.m_personnage.getCaracteristique(),jeu->hero.getMonstreVise(),jeu->hero.m_personnage.getCaracteristique());
        }
        EffectNoir.SetParameter("color", ((float)z)/50, ((float)z)/50, ((float)z)/50);
        jeu->ecran.Draw(EffectNoir);

        jeu->menu.AfficherChargement(&jeu->ecran,(int)z,nomMap);

        jeu->m_display=true;
    }
    else
    {
        jeu->map.setVolumeMusique((int)(z*(float)configuration.volume/50));
        jeu->ecran.SetView(jeu->ecran.GetDefaultView());
        if(!m_debut&&augmenterNoir||!augmenterNoir)
        {
            jeu->ecran.SetView(camera);
            coordonnee temp;
            jeu->map.Afficher(&jeu->ecran,&camera,1,&jeu->hero,temp);
            jeu->ecran.SetView(jeu->ecran.GetDefaultView());

            if(configuration.Minimap)
            {
                jeu->menu.Afficher(&jeu->ecran,1);
                jeu->menu.Afficher(&jeu->ecran,2);
            }
            jeu->menu.Afficher(&jeu->ecran,3);
            jeu->menu.AfficherDynamique(&jeu->ecran,jeu->hero.m_personnage.getCaracteristique(),jeu->hero.getMonstreVise(),jeu->hero.m_personnage.getCaracteristique());
        }

        jeu->menu.AfficherChargement(&jeu->ecran,(int)z,nomMap);

        jeu->m_display=true;
    }

    if(z>=49&&!augmenterNoir)
    {
        jeu->m_contexte = jeu->m_jeu;
    }
}
