

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
    tempsActuel=0,tempsPrecedent=0,temps_ecoule=0,tempsEcouleDepuisDernierAffichage=0;

    m_coordonneePerso.x=coordonneePerso.x;
    m_coordonneePerso.y=coordonneePerso.y;
    m_nomProchaineMap=prochaineMap;
    z=49;
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
                do
                {
                    //Chargement du nom
                    fichier->get(caractere);
                    if (caractere=='n')
                    {
                        int no;
                        *fichier>>no;
                        nomMap = configuration->getText(5, no);
                    }
                }
                while (caractere!='$');
                fichier->get(caractere);
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

    if(cheminFond.size() > 0)
        m_fond=moteurGraphique->AjouterImage(cheminFond.at(rand()%cheminFond.size()));

    cheminFond.clear();

    allerVersImageChargement=true;
    m_tempsChargement=0;
}

void c_Chargement::Utiliser(Jeu *jeu)
{
    if(z==49)
    {
        configuration->RafraichirOmbre=0;
        configuration->RafraichirLumiere=0;
    }

    jeu->m_display=true;

    temps_ecoule=0;
    temps_ecoule=jeu->Clock.GetElapsedTime();
    tempsEcouleDepuisDernierAffichage+=temps_ecoule;
    jeu->Clock.Reset();

    if ((z>=49 && !augmenterNoir && allerVersImageChargement) || m_debut)
    {
        if(jeu->m_jeu->m_thread_sauvegarde && ! m_debut)
        {
            jeu->m_jeu->m_thread_sauvegarde->Wait();
            delete jeu->m_jeu->m_thread_sauvegarde;
            jeu->m_jeu->m_thread_sauvegarde = NULL;
        }

        jeu->Clock.Reset();
        jeu->hero.m_personnage.setCoordonnee(m_coordonneePerso);

        moteurGraphique->ViderParticules();
        moteurSons->StopAllSounds();

        std::vector<Personnage> buffer;
        std::vector<Modele_Monstre> bufferModele;

        for(unsigned i = 0 ; i < jeu->hero.m_amis.size() ; ++i)
        {
            buffer.push_back(*jeu->hero.m_amis[i]);
            bufferModele.push_back(jeu->map->getModeleMonstre(jeu->hero.m_amis[i]->getModele()));
            jeu->hero.m_amis[i]->InfligerDegats(jeu->hero.m_amis[i]->getCaracteristique().vie * 2,4, &bufferModele.back());
        }

        if (!jeu->hero.m_personnage.EnVie())
        {
            mort = true;
            jeu->hero.m_personnage.InfligerDegats(-jeu->hero.m_caracteristiques.maxVie*0.5,4,&jeu->hero.m_modelePersonnage[0]);
        }
        else
            mort = false;

        if (jeu->map!=NULL && !m_debut)
            jeu->map->Sauvegarder(&jeu->hero);


        std::string musicEnCours;
        if(jeu->map!=NULL)
            if(jeu->map->m_musiqueEnCours < jeu->map->m_musiques.size())
                musicEnCours = jeu->map->m_musiques[jeu->map->m_musiqueEnCours];

        if(m_nomProchaineMap!="Tutorial.map.hs" && m_nomProchaineMap!="Begin.map.hs")
            jeu->hero.Sauvegarder();

        if (jeu->map!=NULL)
        {
            delete jeu->map;
            jeu->map = NULL;
        }

        jeu->map=new Map();

         moteurGraphique->LightManager->Delete_All_Light();
        moteurGraphique->LightManager->Delete_All_Wall();


        jeu->hero.ChargerModele(true);
        jeu->hero.m_minimap.clear();

        if (!jeu->map->Charger(m_nomProchaineMap,&jeu->hero))
            console->Ajouter("CRITICAL ERROR"), throw  "CRITICAL ERROR";

        bool newMusic = true;
        for(int i = 0 ; i < jeu->map->m_musiques.size() ; ++i)
            if(musicEnCours == jeu->map->m_musiques[i])
                jeu->map->m_musiqueEnCours = i, newMusic = false;
        if(newMusic && !jeu->map->m_musiques.empty())
            moteurSons->PlayNewMusic(jeu->map->m_musiques[jeu->map->m_musiqueEnCours]);

        for(unsigned i = 0 ; i < jeu->hero.m_amis.size() ; ++i)
        {
            buffer[i].setCoordonnee(jeu->hero.m_personnage.getCoordonnee());

            bool ajouter = true;
            for(int j = 0 ; j < jeu->map->getNombreModeleMonstres() && ajouter ; ++j)
            {
                if(bufferModele[i].m_chemin == jeu->map->getModeleMonstre(j).m_chemin)
                    ajouter = false, buffer[i].setModele(j);
            }

            if(ajouter)
            {
                jeu->map->AjouterModeleMonstre(bufferModele[i]);
                buffer[i].setModele(jeu->map->getNombreModeleMonstres() - 1);
            }

            jeu->map->AjouterMonstre((Monstre) buffer[i]);

            for(unsigned j = 0 ; j < jeu->hero.m_personnage.m_miracleEnCours.size() ; ++j)
                for(unsigned k = 0 ; k < jeu->hero.m_personnage.m_miracleEnCours[j].m_infos.size() ; ++k)
                {
                    if(jeu->hero.m_classe.miracles[jeu->hero.m_personnage.m_miracleEnCours[j].m_modele].m_effets[jeu->hero.m_personnage.m_miracleEnCours[j].m_infos[k]->m_effetEnCours].m_type == CHARME)
                        if(jeu->hero.m_personnage.m_miracleEnCours[j].m_infos[k]->m_cible == jeu->hero.m_amis[i])
                        {
                            jeu->hero.m_personnage.m_miracleEnCours[j].m_infos[k]->m_cible = jeu->map->getEntiteMonstre(jeu->map->getNombreMonstres() - 1);
                            jeu->hero.m_personnage.m_miracleEnCours[j].m_infos[k]->m_cible->setCoordonnee(jeu->hero.m_personnage.m_miracleEnCours[j].m_infos[k]->m_cible->getCoordonnee());
                            jeu->hero.m_personnage.m_miracleEnCours[j].m_infos[k]->m_cible->setDepart();
                        }
                }

            jeu->hero.m_amis[i] = jeu->map->getEntiteMonstre(jeu->map->getNombreMonstres() - 1);
        }

        for (int i = 0; i < (int)jeu->hero.m_personnage.m_miracleEnCours.size(); ++i)
            for (int o = 0; o < (int) jeu->hero.m_personnage.m_miracleEnCours[i].m_infos.size() ; ++o)
                if(jeu->hero.m_classe.miracles[jeu->hero.m_personnage.m_miracleEnCours[i].m_modele].m_effets[jeu->hero.m_personnage.m_miracleEnCours[i].m_infos[o]->m_effetEnCours].m_type == EFFET)
                {
                    if(jeu->hero.m_personnage.m_miracleEnCours[i].m_infos[o]->m_cible != &jeu->hero.m_personnage)
                    {
                        delete jeu->hero.m_personnage.m_miracleEnCours[i].m_infos[o];
                        jeu->hero.m_personnage.m_miracleEnCours[i].m_infos.erase(jeu->hero.m_personnage.m_miracleEnCours[i].m_infos.begin() + o) ,o = -1;
                    }
                }


        jeu->hero.m_personnage.setPousse(coordonneeDecimal(0,0));
        jeu->hero.m_personnage.frappeEnCours = false;
        jeu->hero.m_personnage.m_lancementMiracleEnCours = false;

        moteurGraphique->DecrementerImportance();

        coordonnee position;
        position.x=(jeu->hero.m_personnage.getCoordonnee().x-jeu->hero.m_personnage.getCoordonnee().y-1)/5;
        position.y=(jeu->hero.m_personnage.getCoordonnee().x+jeu->hero.m_personnage.getCoordonnee().y)/5;
        Listener::SetGlobalVolume((float)configuration->volume);
        Listener::SetPosition(-position.x, 0, position.y);
        Listener::SetDirection(0, 0, 1);
        jeu->map->MusiquePlay(position);

        if (configuration->Lumiere)
            jeu->map->CalculerOmbresEtLumieres();


        moteurGraphique->LightManager->GenerateWallShadow(moteurGraphique->m_angleOmbreSoleil,moteurGraphique->m_soleil);

        coordonnee pos;
        pos.x=(int)(((jeu->hero.m_personnage.getCoordonneePixel().x-jeu->hero.m_personnage.getCoordonneePixel().y)*64/COTE_TILE));
        pos.y=(int)(((jeu->hero.m_personnage.getCoordonneePixel().x+jeu->hero.m_personnage.getCoordonneePixel().y)*32/COTE_TILE)/2+64);

        jeu->hero.m_personnage.Animer(&jeu->hero.m_modelePersonnage[0], 0);
        jeu->hero.m_personnage.m_entite_graphique.Initialiser(pos);
        jeu->map->Animer(&jeu->hero,0,&jeu->menu);

        moteurGraphique->LightManager->SetIntensity(jeu->hero.m_personnage.m_entite_graphique.m_light,jeu->hero.getPorteeLumineuse().intensite);
        moteurGraphique->LightManager->SetRadius(jeu->hero.m_personnage.m_entite_graphique.m_light,jeu->hero.getPorteeLumineuse().intensite * 3);
        moteurGraphique->LightManager->SetColor(jeu->hero.m_personnage.m_entite_graphique.m_light,sf::Color(
                jeu->hero.getPorteeLumineuse().rouge,
                jeu->hero.getPorteeLumineuse().vert,
                jeu->hero.getPorteeLumineuse().bleu));

        jeu->hero.m_personnage.SeDeplacer(0, coordonnee ());

        allerVersImageChargement=false;

        jeu->hero.PlacerCamera();

        if (configuration->Ombre)
            configuration->RafraichirOmbre=1;

        if (configuration->Lumiere)
        {
            configuration->RafraichirLumiere=true;
            moteurGraphique->LightManager->Generate(jeu->hero.m_personnage.m_entite_graphique.m_light);
        }

        jeu->map->Animer(&jeu->hero,1,&jeu->menu);

        jeu->Clock.Reset();
    }

    configuration->RafraichirOmbre=1;


    if (z<=1&&augmenterNoir)
        augmenterNoir=false,z=1;
    if (z>=49&&!augmenterNoir)
        augmenterNoir=true,z=49;

    if (augmenterNoir)
        z-=temps_ecoule*200;
    else
        z+=temps_ecoule*200;


    if (z>49.5)
        z=49.5;
    if (z<0.5)
        z=0.5;

    if ((allerVersImageChargement&&z<49&&augmenterNoir)||(!allerVersImageChargement&&z>0&&!augmenterNoir))
    {
        if(jeu->map!=NULL)
            moteurSons->setVolumeMusique((int)(z*(float)configuration->volume/50));
        if ((!m_debut&&augmenterNoir)||(!augmenterNoir))
        {
            if(jeu->map!=NULL)
            {
                jeu->map->Afficher(&jeu->hero,0,jeu->m_jeu->alpha_map);
                //jeu->menu.Afficher(2,jeu->m_jeu->alpha_map,&jeu->hero.m_classe);
            }

            jeu->menu.AfficherHUD(&jeu->hero.m_classe);
            jeu->menu.AfficherDynamique(jeu->hero.m_caracteristiques,0,jeu->hero.m_personnage.getCaracteristique(),&jeu->hero.m_classe);
        }
        if(mort)
            configuration->effetMort = z*3;
    }
    else
        jeu->menu.AfficherChargement(nomMap,m_fond,50);

    configuration->effetNoir=((float)z)/50;

    if ((z>=49&&!augmenterNoir&&!allerVersImageChargement)||(m_debut))
    {
        mort = false;
        z = 0;
        configuration->effetNoir=0;
        jeu->Clock.Reset();
        jeu->m_contexte = jeu->m_jeu;
    }
}
