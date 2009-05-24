

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

#include <iostream>
#include <fstream>

#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

using namespace sf;
using namespace std;

c_Chargement::c_Chargement()
{
}

void c_Chargement::setC_Chargement(std::string prochaineMap,coordonnee coordonneePerso,bool debut)
{
    moteurGraphique->LightManager->Delete_All_Light();
    moteurGraphique->LightManager->Delete_All_Wall();

    tempsActuel=0,tempsPrecedent=0,temps_ecoule=0,tempsEcouleDepuisDernierAffichage=0;

    m_coordonneePerso.x=coordonneePerso.x;
    m_coordonneePerso.y=coordonneePerso.y;
    m_nomProchaineMap=prochaineMap;
    z=50;
    augmenterNoir = true;

    m_debut=debut;

    vector <string> cheminFond;

    cDAT reader;
    reader.Read(configuration->chemin_maps);

    ifstream *fichier=reader.GetInfos(prochaineMap);
    if (fichier)
    {
        char caractere;
        do
        {
            //Chargement du nom
            fichier->get(caractere);
            if (caractere=='*')
            {
                *fichier>>nomMap;
                for (int i=0;i<(int)nomMap.size();i++)
                    if (nomMap[i]=='_')
                        nomMap[i]=' ';
            }
        }
        while (caractere!='$');

        do
        {
            //Chargement du nom
            fichier->get(caractere);
            if (caractere=='*')
            {
                string temp;
                *fichier>>temp;
                cheminFond.push_back(temp);

            }
        }
        while (caractere!='$');

        fichier->close();
    }


    delete fichier;

    int random=rand()%cheminFond.size();

    m_fond=moteurGraphique->AjouterImage(cheminFond.at(random));

    cheminFond.clear();

    allerVersImageChargement=true;
    m_tempsChargement=0;
}

void c_Chargement::Utiliser(Jeu *jeu)
{
    jeu->m_display=true;

    temps_ecoule=0;
    temps_ecoule=jeu->Clock.GetElapsedTime();
    tempsEcouleDepuisDernierAffichage+=temps_ecoule;
    jeu->Clock.Reset();

    if (z>=49&&!augmenterNoir&&allerVersImageChargement||m_debut)
    {
        jeu->Clock.Reset();
        jeu->hero.m_personnage.setCoordonnee(m_coordonneePerso);

        moteurGraphique->ViderParticules();

        if (jeu->map!=NULL&&!m_debut)
           jeu->map->Sauvegarder(&jeu->hero);

        jeu->hero.Sauvegarder();

        if (jeu->map!=NULL)
        {
            delete jeu->map;
            jeu->map = NULL;
        }

        jeu->map=new Map();

        jeu->hero.m_personnage.m_light=moteurGraphique->LightManager->Add_Dynamic_Light(sf::Vector2f(0,0),255,384,16,sf::Color(255,255,255));

        jeu->hero.ChargerModele(true);

        if (!jeu->map->Charger(m_nomProchaineMap,&jeu->hero))
            console->Ajouter("CRITICAL ERROR"), throw  "CRITICAL ERROR";

        moteurGraphique->DecrementerImportance();

        jeu->hero.PlacerCamera(jeu->map->getDimensions());

        coordonnee position;
        position.x=(jeu->hero.m_personnage.getCoordonnee().x-jeu->hero.m_personnage.getCoordonnee().y-1+jeu->map->getDimensions().y)/5;
        position.y=(jeu->hero.m_personnage.getCoordonnee().x+jeu->hero.m_personnage.getCoordonnee().y)/5;
        Listener::SetGlobalVolume((float)configuration->volume);
        Listener::SetPosition(-position.x, 0, position.y);
        Listener::SetTarget(0, 0, 1);
        jeu->map->MusiquePlay(position);

        if (configuration->Lumiere)
            jeu->map->CalculerOmbresEtLumieres();

        moteurGraphique->LightManager->GenerateWallShadow(moteurGraphique->m_angleOmbreSoleil,moteurGraphique->m_soleil);

        configuration->RafraichirLumiere=true;

        sf::Vector2f pos;
        pos.x=(((jeu->hero.m_personnage.getCoordonneePixel().x-jeu->hero.m_personnage.getCoordonneePixel().y)*64/COTE_TILE));
        pos.y=(((jeu->hero.m_personnage.getCoordonneePixel().x+jeu->hero.m_personnage.getCoordonneePixel().y)*64/COTE_TILE)/2+32)*2;

        moteurGraphique->LightManager->SetPosition(jeu->hero.m_personnage.m_light,pos);
        allerVersImageChargement=false;

        jeu->Clock.Reset();
    }


    if (z<=1&&augmenterNoir)
        augmenterNoir=false,z=1;
    if (z>=49&&!augmenterNoir)
        augmenterNoir=true,z=49;

    if (augmenterNoir)
        z-=temps_ecoule*200;
    else
        z+=temps_ecoule*200;


    if (z>50)
        z=50;
    if (z<0)
        z=0;

    if (allerVersImageChargement&&z<49&&augmenterNoir||!allerVersImageChargement&&z>0&&!augmenterNoir)
    {
        //jeu->camera.Zoom(configuration->zoom);
        if(jeu->map!=NULL)
            jeu->map->setVolumeMusique((int)(z*(float)configuration->volume/50));
        if (!m_debut&&augmenterNoir||!augmenterNoir)
        {
            coordonnee temp;
            if(jeu->map!=NULL)
                jeu->map->Afficher(&jeu->hero,temp,0);

            if (configuration->Minimap)
                jeu->menu.Afficher(2,255,&jeu->hero.m_classe);

            jeu->menu.Afficher(1,255,&jeu->hero.m_classe);
            jeu->menu.AfficherDynamique(jeu->hero.m_caracteristiques,0,jeu->hero.m_personnage.getCaracteristique(),&jeu->hero.m_classe);
        }
    }
    else
        jeu->menu.AfficherChargement(nomMap,m_fond,50);

    configuration->effetNoir=((float)z)/50;


    if (z>=49&&!augmenterNoir&&!allerVersImageChargement||m_debut)
    {
        configuration->effetNoir=0;
        jeu->m_contexte = jeu->m_jeu;
    }
}
