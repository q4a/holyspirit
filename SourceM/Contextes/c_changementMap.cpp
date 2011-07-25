

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

int GestionBoutons(Jeu *jeu, bool = false, bool = false, bool = false);

c_Chargement::c_Chargement()
{
    m_thread_charger = NULL;
    m_chargement = false;
}
c_Chargement::~c_Chargement()
{
}

void c_Chargement::setC_Chargement(const std::string &prochaineMap,const coordonnee &coordonneePerso,bool debut)
{
    tempsActuel=0,tempsPrecedent=0,temps_ecoule=0,tempsEcouleDepuisDernierAffichage=0;

    console->Ajouter("",0);
    console->Ajouter("---------------------------------------------------------------------------------",0);
    console->Ajouter("EVENEMENT : Changement de map",0);
    console->Ajouter("---------------------------------------------------------------------------------",0);

    mort = false;
    z = 0;
    configuration->effetNoir=0;

    m_coordonneePerso.x=coordonneePerso.x;
    m_coordonneePerso.y=coordonneePerso.y;
    m_coordonneePerso.h=0;
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

    sf::Context context;

    if(!cheminFond.empty())
        m_fond=moteurGraphique->AjouterImage(cheminFond.at(rand()%cheminFond.size()),-1);

    cheminFond.clear();

    allerVersImageChargement=true;
    m_tempsChargement=0;

    if(m_debut)
        augmenterNoir = false;
}

void Charger(void* UserData)
{
    srand(time(NULL));

    sf::Context context;

    Jeu *jeu = static_cast<Jeu*>(UserData);

    if (!jeu->map->Charger(jeu->m_chargement->m_nomProchaineMap,&jeu->hero))
        console->Ajouter("CRITICAL ERROR",1), throw  "CRITICAL ERROR";
}

void c_Chargement::PreLoad(Jeu *jeu)
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

    buffer.clear();
    bufferModele.clear();

    for(unsigned i = 0 ; i < jeu->hero.m_amis.size() ; ++i)
    {
        if(!jeu->hero.m_amis[i]->m_heroic)
        {
            buffer.push_back(*jeu->hero.m_amis[i]);
            bufferModele.push_back(jeu->map->getModeleMonstre(jeu->hero.m_amis[i]->getModele()));
            jeu->hero.m_amis[i]->Kill();
            jeu->hero.m_amis[i]->m_inexistant = true;
        }
        else
        {
            buffer.push_back(Monstre ());
            buffer.back().m_heroic = true;
            bufferModele.push_back(Modele_Monstre ());
        }
    }

    if (!jeu->hero.m_personnage.EnVie())
    {
        mort = true;
        jeu->hero.m_personnage.InfligerDegats(-jeu->hero.m_caracteristiques.maxVie,4,0);
    }
    else
        mort = false;

    if (jeu->map!=NULL && !m_debut)
        jeu->map->Sauvegarder(&jeu->hero);

    musicEnCours = -1;
    if(jeu->map!=NULL)
        if(jeu->map->m_musiqueEnCours < (int)jeu->map->m_musiques.size())
            musicEnCours = jeu->map->m_musiques[jeu->map->m_musiqueEnCours];

    //if(m_nomProchaineMap!="Tutorial.map.hs" && m_nomProchaineMap!="Begin.map.hs")
    {
        jeu->hero.SauvegarderApercu();
        jeu->hero.Sauvegarder();
    }

    climatsEnCours.clear();
    climatsTimeEnCours.clear();
    if(jeu->map!=NULL)
        for(unsigned i = 0 ; i < jeu->map->m_climates.size() ; ++i)
            if(jeu->map->m_climates[i].m_actif)
            {
                climatsEnCours.push_back(jeu->map->m_climates[i].m_chemin);
                climatsTimeEnCours.push_back(jeu->map->m_climates[i].m_cur_time);
            }


    if (jeu->map!=NULL)
    {
        delete jeu->map;
        jeu->map = NULL;
    }

    moteurGraphique->LightManager->Delete_All_Light();
    moteurGraphique->LightManager->Delete_All_Wall();
    moteurGraphique->DecrementerImportance();

    moteurGraphique->m_ambientShadowTileset.ChargerImages();

    jeu->map=new Map();

    jeu->hero.ChargerModele(true);
    jeu->SendSkin();
}

void c_Chargement::PostLoad(Jeu *jeu)
{
    m_thread_charger->Wait();

    //for (std::list<Hero>::iterator p = jeu->m_personnageClients.begin(); p != jeu->m_personnageClients.end(); ++p)
    //p->ChargerModele(true);


    jeu->map->Initialiser(&jeu->hero);

    jeu->map->m_etat_chargement = 5;

    jeu->map->InitialiserMinimap();

    bool newMusic = true;
    for(unsigned i = 0 ; i < jeu->map->m_musiques.size() ; ++i)
        if(musicEnCours == jeu->map->m_musiques[i])
            jeu->map->m_musiqueEnCours = i, newMusic = false;
    if(newMusic && !jeu->map->m_musiques.empty())
        moteurSons->PlayNewMusic(jeu->map->m_musiques[jeu->map->m_musiqueEnCours]);

    for(unsigned j = 0 ; j < climatsEnCours.size() ; ++j)
        for(unsigned i = 0 ; i < jeu->map->m_climates.size() ; ++i)
            if(jeu->map->m_climates[i].m_chemin == climatsEnCours[j])
            {
                jeu->map->m_climates[i].m_actif = true;
                jeu->map->m_climates[i].m_cur_time = climatsTimeEnCours[j];
            }

    cout<<"BOUH !"<<endl;
    /*for(unsigned i = 0 ; i < jeu->hero.m_amis.size() ; ++i)
    if(!buffer[i].m_heroic)
    {
        coordonnee temp;

        buffer[i].setCoordonnee(jeu->hero.m_personnage.getCoordonnee());

        temp.x = jeu->hero.m_personnage.getCoordonnee().x - 1;
        temp.y = jeu->hero.m_personnage.getCoordonnee().y - 1;
        if(!jeu->map->getCollision(temp.x, temp.y))
            buffer[i].setCoordonnee(temp);
        temp.x = jeu->hero.m_personnage.getCoordonnee().x - 1;
        temp.y = jeu->hero.m_personnage.getCoordonnee().y;
        if(!jeu->map->getCollision(temp.x, temp.y))
            buffer[i].setCoordonnee(temp);
        temp.x = jeu->hero.m_personnage.getCoordonnee().x - 1;
        temp.y = jeu->hero.m_personnage.getCoordonnee().y + 1;
        if(!jeu->map->getCollision(temp.x, temp.y))
            buffer[i].setCoordonnee(temp);
        temp.x = jeu->hero.m_personnage.getCoordonnee().x;
        temp.y = jeu->hero.m_personnage.getCoordonnee().y - 1;
        if(!jeu->map->getCollision(temp.x, temp.y))
            buffer[i].setCoordonnee(temp);
        temp.x = jeu->hero.m_personnage.getCoordonnee().x;
        temp.y = jeu->hero.m_personnage.getCoordonnee().y + 1;
        if(!jeu->map->getCollision(temp.x, temp.y))
            buffer[i].setCoordonnee(temp);
        temp.x = jeu->hero.m_personnage.getCoordonnee().x + 1;
        temp.y = jeu->hero.m_personnage.getCoordonnee().y - 1;
        if(!jeu->map->getCollision(temp.x, temp.y))
            buffer[i].setCoordonnee(temp);
        temp.x = jeu->hero.m_personnage.getCoordonnee().x + 1;
        temp.y = jeu->hero.m_personnage.getCoordonnee().y + 1;
        if(!jeu->map->getCollision(temp.x, temp.y))
            buffer[i].setCoordonnee(temp);

        bool ajouter = true;
        for(int j = 0 ; j < jeu->map->getNombreModeleMonstres() && ajouter ; ++j)
        {
            if(bufferModele[i].m_chemin == jeu->map->getModeleMonstre(j).m_chemin)
            {
                for(unsigned o = jeu->map->getModeleMonstre(j).m_miracles.size() - 1 ; o < bufferModele[i].m_miracles.size() ; ++o)
                    jeu->map->getModeleMonstre(j).m_miracles.push_back(bufferModele[i].m_miracles[o]);
                ajouter = false, buffer[i].setModele(j);
            }
        }

        if(ajouter)
        {
            jeu->map->AjouterModeleMonstre(bufferModele[i]);
            buffer[i].setModele(jeu->map->getNombreModeleMonstres() - 1);
        }



        buffer[i].frappeEnCours = false;

        jeu->map->AjouterMonstre(buffer[i]);

        jeu->map->getEntiteMonstre(jeu->map->getNombreMonstres() - 1)->setCoordonnee(jeu->map->getEntiteMonstre(jeu->map->getNombreMonstres() - 1)->getCoordonnee());

        jeu->map->getEntiteMonstre(jeu->map->getNombreMonstres() - 1)->setDepart();

        for(unsigned j = 0 ; j < jeu->hero.m_personnage.m_miracleEnCours.size() ; ++j)
            for(unsigned k = 0 ; k < jeu->hero.m_personnage.m_miracleEnCours[j].m_infos.size() ; ++k)
            {
                if(jeu->hero.m_classe.miracles[jeu->hero.m_personnage.m_miracleEnCours[j].m_modele].m_effets[jeu->hero.m_personnage.m_miracleEnCours[j].m_infos[k]->m_effetEnCours].m_type == CHARME
                 ||jeu->hero.m_classe.miracles[jeu->hero.m_personnage.m_miracleEnCours[j].m_modele].m_effets[jeu->hero.m_personnage.m_miracleEnCours[j].m_infos[k]->m_effetEnCours].m_type == INVOCATION)
                    if(jeu->hero.m_personnage.m_miracleEnCours[j].m_infos[k]->m_cible == jeu->hero.m_amis[i])
                    {
                        jeu->hero.m_personnage.m_miracleEnCours[j].m_infos[k]->m_cible = jeu->map->getEntiteMonstre(jeu->map->getNombreMonstres() - 1);
                        jeu->hero.m_personnage.m_miracleEnCours[j].m_infos[k]->m_cible->setCoordonnee(jeu->hero.m_personnage.m_miracleEnCours[j].m_infos[k]->m_cible->getCoordonnee());
                        jeu->hero.m_personnage.m_miracleEnCours[j].m_infos[k]->m_cible->setDepart();
                    }
            }

        jeu->hero.m_amis[i] = jeu->map->getEntiteMonstre(jeu->map->getNombreMonstres() - 1);
    }*/
    cout<<"DIN !"<<endl;

    for (unsigned i = 0; i < jeu->hero.m_personnage.m_miracleEnCours.size(); ++i)
        for (int o = 0; o <  (int)jeu->hero.m_personnage.m_miracleEnCours[i].m_infos.size() ; ++o)
        {
            if(jeu->hero.m_classe.miracles[jeu->hero.m_personnage.m_miracleEnCours[i].m_modele].m_effets[jeu->hero.m_personnage.m_miracleEnCours[i].m_infos[o]->m_effetEnCours].m_type == EFFET)
            {
                if(jeu->hero.m_personnage.m_miracleEnCours[i].m_infos[o]->m_cible != &jeu->hero.m_personnage)
                    jeu->hero.m_personnage.m_miracleEnCours[i].m_infos.erase(jeu->hero.m_personnage.m_miracleEnCours[i].m_infos.begin() + o) ,o = -1;
            }
            else if(jeu->hero.m_classe.miracles[jeu->hero.m_personnage.m_miracleEnCours[i].m_modele].m_effets[jeu->hero.m_personnage.m_miracleEnCours[i].m_infos[o]->m_effetEnCours].m_type == CHARGE)
            {
                delete jeu->hero.m_personnage.m_miracleEnCours[i].m_infos[o];
                jeu->hero.m_personnage.m_miracleEnCours[i].m_infos.erase(jeu->hero.m_personnage.m_miracleEnCours[i].m_infos.begin() + o) ,o = -1;
            }
        }

    jeu->hero.m_personnage.setPousse(coordonneeDecimal(0,0));
    jeu->hero.m_personnage.frappeEnCours = false;
    jeu->hero.m_personnage.m_lancementMiracleEnCours = false;

//    coordonnee position;
  //  position.x=(jeu->hero.m_personnage.getCoordonnee().x-jeu->hero.m_personnage.getCoordonnee().y-1)/5;
   // position.y=(jeu->hero.m_personnage.getCoordonnee().x+jeu->hero.m_personnage.getCoordonnee().y)/5;
  //  Listener::SetGlobalVolume((float)configuration->volume);
   // Listener::SetPosition(-position.x, 0, position.y);
   // Listener::SetDirection(0, 0, 1);
    jeu->map->MusiquePlay();

    if (configuration->Lumiere)
        jeu->map->CalculerOmbresEtLumieres();

    coordonnee pos;
    pos.x=(int)(((jeu->hero.m_personnage.getCoordonneePixel().x-jeu->hero.m_personnage.getCoordonneePixel().y)*64/COTE_TILE));
    pos.y=(int)(((jeu->hero.m_personnage.getCoordonneePixel().x+jeu->hero.m_personnage.getCoordonneePixel().y)*32/COTE_TILE)/2+64);

    jeu->hero.m_personnage.Animer(&jeu->hero.m_modelePersonnage[0], 0);
    jeu->hero.m_personnage.m_entite_graphique.Initialiser(pos);


    for (std::list<Hero>::iterator p = jeu->m_personnageClients.begin(); p != jeu->m_personnageClients.end(); ++p)
    {
        p->m_personnage.ClearEmulate();
        pos.x=(int)(((p->m_personnage.getCoordonneePixel().x-p->m_personnage.getCoordonneePixel().y)*64/COTE_TILE));
        pos.y=(int)(((p->m_personnage.getCoordonneePixel().x+p->m_personnage.getCoordonneePixel().y)*32/COTE_TILE)/2+64);
        p->m_personnage.Animer(&p->m_modelePersonnage[0], 0);
        p->m_personnage.m_entite_graphique.Initialiser(pos);
    }

    jeu->map->Animer(&jeu->hero,0);
    jeu->map->GererMonstres(jeu,&jeu->hero,0,&jeu->menu);

    moteurGraphique->LightManager->SetQuality(jeu->hero.m_personnage.m_entite_graphique.m_light,24);
    moteurGraphique->LightManager->SetMovingLight(jeu->hero.m_personnage.m_entite_graphique.m_light,true);
    moteurGraphique->LightManager->SetIntensity(jeu->hero.m_personnage.m_entite_graphique.m_light,jeu->hero.getPorteeLumineuse().intensite);
    moteurGraphique->LightManager->SetRadius(jeu->hero.m_personnage.m_entite_graphique.m_light,jeu->hero.getPorteeLumineuse().intensite * 4);
    moteurGraphique->LightManager->SetColor(jeu->hero.m_personnage.m_entite_graphique.m_light,sf::Color(
            jeu->hero.getPorteeLumineuse().rouge,
            jeu->hero.getPorteeLumineuse().vert,
            jeu->hero.getPorteeLumineuse().bleu));


    jeu->hero.m_personnage.SeDeplacer(0);

    allerVersImageChargement=false;
    augmenterNoir=true;

    jeu->hero.PlacerCamera();

    if (configuration->Ombre)
        configuration->RafraichirOmbre=1;

    if (configuration->Lumiere)
    {
        configuration->RafraichirLumiere=true;
        moteurGraphique->LightManager->Generate(jeu->hero.m_personnage.m_entite_graphique.m_light);
    }

    jeu->map->Animer(&jeu->hero,1);

    jeu->Clock.Reset();

    jeu->map->GererMonstres(jeu,&jeu->hero,0,&jeu->menu);
    jeu->map->GererScript(jeu,&jeu->hero,0,&jeu->menu);
    jeu->map->Animer(&jeu->hero,0.1);

    m_chargement = false;
}

void c_Chargement::Utiliser(Jeu *jeu)
{
    if(z==49)
    {
        configuration->RafraichirOmbre=0;
        configuration->RafraichirLumiere=0;
    }

    temps_ecoule=0;
    temps_ecoule=jeu->Clock.GetElapsedTime()*0.001;
    tempsEcouleDepuisDernierAffichage+=temps_ecoule;
    jeu->Clock.Reset();

    if(!m_chargement)
    if ((z>=49 && !augmenterNoir && allerVersImageChargement) || (m_debut && jeu->m_display))
    {
        PreLoad(jeu);

        if (m_thread_charger)
        {
           // m_thread_charger->Wait();
            delete m_thread_charger;
        }

        m_thread_charger = new sf::Thread(&Charger, jeu);
        m_thread_charger->Launch();

        m_chargement = true;
    }

    if(!m_chargement)
    {
        if (z<=1&&augmenterNoir)
            augmenterNoir=false,z=1;
        if (z>=49&&!augmenterNoir)
            augmenterNoir=true,z=49;

        if (augmenterNoir)
            z-=temps_ecoule*200;
        else
            z+=temps_ecoule*200;


        if (z>49.5f)
            z=49.5f;
        if (z<0.5f)
            z=0.5f;
    }

    if(jeu->map)
        if(m_chargement && jeu->map->m_loaded)
            PostLoad(jeu);

    if (!m_chargement&&
        ((allerVersImageChargement&&z<49&&augmenterNoir)||(!allerVersImageChargement&&z>0&&!augmenterNoir)))
    {
        configuration->RafraichirOmbre=1;

        if ((!m_debut&&augmenterNoir)||(!augmenterNoir))
        {
            if(jeu->map!=NULL)
            {
                if(augmenterNoir)
                for(unsigned i = 0 ; i < jeu->map->m_ambiances.size() ; ++i)
                    jeu->map->m_playAmbiances[i] = false;

                jeu->map->Afficher(&jeu->hero,jeu->m_personnageClients,0,jeu->m_jeu->alpha_map);
                if(augmenterNoir)
                    jeu->map->GererAmbiance(temps_ecoule*20);
                else
                    jeu->map->GererAmbiance(temps_ecoule*5);
            }

            jeu->hero.AfficherAmisEtCraft();

            GestionBoutons(jeu);
            jeu->menu.AfficherHUD(&jeu->hero.m_classe);
            jeu->menu.AfficherDynamique(jeu->hero.m_caracteristiques,0,jeu->hero.m_personnage.getCaracteristique(),&jeu->hero.m_classe);
        }
        if(mort && allerVersImageChargement)
            configuration->effetMort = z*3;
    }
    else
    {
        jeu->menu.AfficherChargement(nomMap,m_fond,50);

        Sprite sprite,sprite2;

        sprite.SetImage(*moteurGraphique->getImage(jeu->hero.m_classe.barre_vie_monstre_vide.image));
        sprite.SetX(configuration->Resolution.w/2+jeu->hero.m_classe.barre_vie_monstre_vide.position.x);
        sprite.SetY(configuration->Resolution.h - 48);

        moteurGraphique->AjouterCommande(&sprite,16,0);

        if(jeu->map)
        {
           sprite2.SetImage(*moteurGraphique->getImage(jeu->hero.m_classe.barre_vie_monstre.image));

            sprite2.SetSubRect(sf::IntRect(0, 0,(int)((float)jeu->map->m_etat_chargement/5*moteurGraphique->getImage(jeu->hero.m_classe.barre_vie_monstre.image)->GetWidth()), 32));
            sprite2.SetX(configuration->Resolution.w/2 + jeu->hero.m_classe.barre_vie_monstre.position.x);
            sprite2.SetY(configuration->Resolution.h - 48);

            moteurGraphique->AjouterCommande(&sprite2,17,0);
        }
    }

    configuration->effetNoir=((float)z)/50;

    if(!m_chargement)
    if ((z>=49&&!augmenterNoir&&!allerVersImageChargement)||(m_debut))
    {
        mort = false;
        z = 0;
        configuration->effetNoir=0;
        jeu->Clock.Reset();
        jeu->m_contexte = jeu->m_jeu;

        jeu->map->m_etat_chargement = 0;
    }

    jeu->m_display=true;
}
