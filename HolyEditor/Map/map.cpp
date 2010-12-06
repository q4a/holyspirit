

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


#include "map.h"

#include <iostream>
#include <fstream>
#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <dirent.h>
#include <sstream>


using namespace std;
using namespace sf;

#include "../globale.h"
//#include "../jeu.h"

Map::Map()
{
    console->Ajouter("");
    console->Ajouter("Chargements d'images diverses :");
    IDImageSac=moteurGraphique->AjouterImage(configuration->chemin_menus+configuration->nom_sac,-1);
    m_pret= false;

    m_selectTileset   = -1;
    m_selectTile      = -1;
    m_selectHerbe     = -1;
    m_selectEntite    = -1;
    m_mode_brush      = false;

    m_selectCouche   = 0;
    m_no_backup      = 0;
    m_taillePinceau  = 0;

    m_aleatoireTileset = 0;

    m_entite_angle   = 0 ;
}

Map::~Map()
{
    Detruire();
}


void Map::Detruire()
{
    moteurGraphique->LightManager->Delete_All_Light();
    moteurGraphique->LightManager->Delete_All_Wall();

    console->Ajouter("");
    console->Ajouter("Destruction de la map...");

    m_tileset.clear();
    m_herbe.clear();

    if (configuration->debug)
        console->Ajouter("Destruction des cases...");

    for (int i=0;i<NOMBRE_COUCHE_MAP;++i)
    {
        for (int j=0;j<(int)m_decor[i].size();j++)
            m_decor[i][j].clear();
        m_decor[i].clear();
    }

    if (configuration->debug)
        console->Ajouter("Cases détruites !");

    m_ModeleMonstre.clear();

    if (configuration->debug)
        console->Ajouter("Modèles monstres détruits !");

    m_monstre.clear();

    if (configuration->debug)
        console->Ajouter("Monstres détruits !");

    for (int i=0;i<MAX_MUSIQUE;++i)
        m_musique[i].Stop();

    m_fond.clear();
    if (configuration->debug)
        console->Ajouter("Fonds détruits !");

    m_cheminMusique.clear();
    if (configuration->debug)
        console->Ajouter("Chemins musiques détruits !");

    m_projectile.clear();
    if (configuration->debug)
        console->Ajouter("Projectiles détruits !");

    m_effets.clear();
    if (configuration->debug)
        console->Ajouter("Effets détruits !");

    console->Ajouter("Map détruite !");
}

void Map::Creer(int x, int y)
{
    m_dimensions.x = x;
    m_dimensions.y = y;

    m_nom_fichier="";
   // string chemin = nomMap,chemin2 = configuration->chemin_temps+nomMap;

    m_lumiere[0].intensite=1;
    m_lumiere[0].rouge=0;
    m_lumiere[0].vert=0;
    m_lumiere[0].bleu=0;

    m_musiqueEnCours=0;

    {
        /*do
        {
            fichier->get(caractere);

            if (caractere=='*')
            {
                string nom;
                *fichier>>nom;
                m_fond.push_back(nom);
            }

            if (fichier->eof())
            {
                console->Ajouter("Erreur : Map \" "+chemin+" \" Invalide",1);
                return 0;
            }
        }
        while (caractere!='$');*/

        m_nombreMusique=0;
        /*do
        {
            //Chargement des musiques
            fichier->get(caractere);
            if (caractere=='*'&&m_musiqueEnCours<MAX_MUSIQUE)
            {
                std::string temp2;

                *fichier>>temp2;

                if (!m_musique[m_musiqueEnCours].OpenFromFile(temp2.c_str()))
                    console->Ajouter("Impossible de charger : "+temp2,1);
                else
                    console->Ajouter("Chargement de : "+temp2,0);

                m_cheminMusique.push_back(temp2);

                m_musique[m_musiqueEnCours].SetLoop(false);
                m_musique[m_musiqueEnCours].SetVolume(100);


                m_musiqueEnCours++;
                m_nombreMusique++;
            }
            if (fichier->eof())
            {
                console->Ajouter("Erreur : Map \" "+chemin+" \" Invalide",1);
                return 0;
            }
        }
        while (caractere!='$');*/

        m_musique[0].Play();

        m_musiqueEnCours=0;

   /*     int heureEnCours=0;
        do
        {

            //Chargement de la lumière ambiante
            fichier->get(caractere);
            if (caractere=='*')
            {
                *fichier>>m_lumiere[heureEnCours].rouge;
                *fichier>>m_lumiere[heureEnCours].vert;
                *fichier>>m_lumiere[heureEnCours].bleu;
                *fichier>>m_lumiere[heureEnCours].intensite;
                *fichier>>m_lumiere[heureEnCours].hauteur;
                heureEnCours++;

                if (heureEnCours>23)
                    heureEnCours=23;
            }
            if (fichier->eof())
            {
                console->Ajouter("Erreur : Map \" "+chemin+" \" Invalide",1);
                return 0;
            }
        }
        while (caractere!='$');

        while (heureEnCours<24)
        {
            m_lumiere[heureEnCours].rouge=m_lumiere[0].rouge;
            m_lumiere[heureEnCours].vert=m_lumiere[0].vert;
            m_lumiere[heureEnCours].bleu=m_lumiere[0].bleu;
            m_lumiere[heureEnCours].intensite=m_lumiere[0].intensite;
            m_lumiere[heureEnCours].hauteur=m_lumiere[0].hauteur;
            heureEnCours++;
        }*/

//        Decor decorTemp(-1,-1,std::vector<int> (),std::vector<int> (),-1,0,0);

        coordonnee position;

            for (int couche=0;couche<NOMBRE_COUCHE_MAP;couche++)
            {
                position.x=0;
                position.y=0;

                for( position.y = 0 ; position.y < y ; ++position.y)
                {
                    m_decor[couche].push_back(std::vector<std::vector<Decor > > ());
                    for( position.x = 0 ; position.x < x ; ++position.x)
                    {
                        m_decor[couche][position.y].push_back(std::vector<Decor > ());

                        int layer;
                        if(couche == 0)
                            layer = 1;
                        else
                            layer = 10;

                        m_decor[couche][position.y].back().push_back(Decor (-1,std::vector <int> (),std::vector <int> (),-1,layer,0,std::vector <Objet> ()));
                        m_decor[couche][position.y].back().back().m_moduleAleatoireMin = 0;
                        m_decor[couche][position.y].back().back().m_moduleAleatoireMax = 9;
                    }
                }
            }
    }
    int heureEnCours = 1;
    while (heureEnCours<24)
        {
            m_lumiere[heureEnCours].rouge=m_lumiere[0].rouge;
            m_lumiere[heureEnCours].vert=m_lumiere[0].vert;
            m_lumiere[heureEnCours].bleu=m_lumiere[0].bleu;
            m_lumiere[heureEnCours].intensite=m_lumiere[0].intensite;
            m_lumiere[heureEnCours].hauteur=m_lumiere[0].hauteur;
            heureEnCours++;
        }



    for (int i=0;i<24;++i)
        if (m_lumiere[i].intensite<1)
            m_lumiere[i].intensite=1;

    //m_dimensions.x=(int)m_decor[0][0].size();
    //m_dimensions.y=(int)m_decor[0].size();

    console->Ajouter("Chargement de la map terminé.");
    console->Ajouter("");

    Initialiser();

    console->Ajouter("Initialisation de la map terminé.");
    console->Ajouter("");

    m_pret = true;
}

bool Map::Charger(std::string nomMap)
{
    m_max_ID = 0;

    m_dimensions.x=0;
    m_dimensions.y=0;

    m_nom_fichier=nomMap;
    string chemin = nomMap,chemin2 = configuration->chemin_temps+nomMap;

    m_lumiere[0].intensite=1;
    m_lumiere[0].rouge=0;
    m_lumiere[0].vert=0;
    m_lumiere[0].bleu=0;

    m_musiqueEnCours=0;

    console->Ajouter("",0);
    console->Ajouter("Chargement de la map : "+nomMap);


    ifstream *fichier=NULL;

    fichier = new ifstream;
    fichier->open(nomMap.c_str(), ios::in);

    if (*fichier)
    {
        char caractere;
        do
        {
            //Chargement du nom
            fichier->get(caractere);
            if (caractere=='*')
            {
                m_no_nom = 0;
                do
                {
                    //Chargement du nom
                    fichier->get(caractere);
                    if (caractere=='n')
                        *fichier>>m_no_nom;

                    if (fichier->eof())
                    {
                        console->Ajouter("Erreur : Map \" "+chemin+" \" Invalide",1);
                        throw ("Erreur : Map \" "+chemin+" \" Invalide");
                    }
                }
                while (caractere!='$');

                m_nom = configuration->getText(5,m_no_nom);
                fichier->get(caractere);
            }

            if (fichier->eof())
            {
                console->Ajouter("Erreur : Map \" "+chemin+" \" Invalide",1);
                throw ("Erreur : Map \" "+chemin+" \" Invalide");
            }
        }
        while (caractere!='$');
        if (configuration->debug)
            console->Ajouter("/Lectures du nom.");

        do
        {
            fichier->get(caractere);

            if (caractere=='*')
            {
                string nom;
                *fichier>>nom;
                m_fond.push_back(nom);
            }

            if (fichier->eof())
            {
                console->Ajouter("Erreur : Map \" "+chemin+" \" Invalide",1);
                return 0;
            }
        }
        while (caractere!='$');

        if (configuration->debug)
            console->Ajouter("/Lectures des fonds.");

        for (int i=0;i<MAX_MUSIQUE;++i)
            m_musique[i].Stop();

        m_nombreMusique=0;
        do
        {
            //Chargement des musiques
            fichier->get(caractere);
            if (caractere=='*'&&m_musiqueEnCours<MAX_MUSIQUE)
            {
                std::string temp2;

                *fichier>>temp2;

                if (!m_musique[m_musiqueEnCours].OpenFromFile(temp2.c_str()))
                    console->Ajouter("Impossible de charger : "+temp2,1);
                else
                    console->Ajouter("Chargement de : "+temp2,0);

                m_cheminMusique.push_back(temp2);

                //m_musique[m_musiqueEnCours].SetLoop(false);
                //m_musique[m_musiqueEnCours].SetVolume(100);


                m_musiqueEnCours++;
                m_nombreMusique++;
            }
            if (fichier->eof())
            {
                console->Ajouter("Erreur : Map \" "+chemin+" \" Invalide",1);
                return 0;
            }
        }
        while (caractere!='$');

        if (configuration->debug)
            console->Ajouter("/Lectures des musiques.");

      //  m_musique[0].Play();

        m_musiqueEnCours=0;

        int heureEnCours=0;
        do
        {

            //Chargement de la lumière ambiante
            fichier->get(caractere);
            if (caractere=='*')
            {
                *fichier>>m_lumiere[heureEnCours].rouge;
                *fichier>>m_lumiere[heureEnCours].vert;
                *fichier>>m_lumiere[heureEnCours].bleu;
                *fichier>>m_lumiere[heureEnCours].intensite;
                *fichier>>m_lumiere[heureEnCours].hauteur;
                heureEnCours++;

                if (heureEnCours>23)
                    heureEnCours=23;
            }
            if (fichier->eof())
            {
                console->Ajouter("Erreur : Map \" "+chemin+" \" Invalide",1);
                return 0;
            }
        }
        while (caractere!='$');

        if (configuration->debug)
            console->Ajouter("/Lectures des lumières.");

        do
        {

            //Chargement de la lumière ambiante
            fichier->get(caractere);
            if (caractere=='*')
            {
                *fichier>>m_nom_img_sky;
                m_img_sky = moteurGraphique->AjouterImage(m_nom_img_sky, 3);
            }
            if (fichier->eof())
            {
                console->Ajouter("Erreur : Map \" "+chemin+" \" Invalide",1);
                throw ("Erreur : Map \" "+chemin+" \" Invalide");
            }
        }
        while (caractere!='$');


        do
        {

            //Chargement de la lumière ambiante
            fichier->get(caractere);
            if (caractere=='*')
            {
                m_climates.push_back("");
                *fichier>>m_climates.back();
            }
            if (fichier->eof())
            {
                console->Ajouter("Erreur : Map \" "+chemin+" \" Invalide",1);
                throw ("Erreur : Map \" "+chemin+" \" Invalide");
            }
        }
        while (caractere!='$');

        if (configuration->debug)
            console->Ajouter("/Lectures des ciels.");


        while (heureEnCours<24)
        {
            m_lumiere[heureEnCours].rouge=m_lumiere[0].rouge;
            m_lumiere[heureEnCours].vert=m_lumiere[0].vert;
            m_lumiere[heureEnCours].bleu=m_lumiere[0].bleu;
            m_lumiere[heureEnCours].intensite=m_lumiere[0].intensite;
            m_lumiere[heureEnCours].hauteur=m_lumiere[0].hauteur;
            heureEnCours++;
        }

        do
        {
            //Chargement des tileset
            fichier->get(caractere);
            if (caractere=='*')
            {
                string cheminDuTileset;
                *fichier>>cheminDuTileset;
                m_tileset.push_back(moteurGraphique->AjouterTileset(cheminDuTileset));
            }
            if (fichier->eof())
            {
                console->Ajouter("Erreur : Map \" "+chemin+" \" Invalide",1);
                throw ("Erreur : Map \" "+chemin+" \" Invalide");
            }

        }
        while (caractere!='$');

        if (configuration->debug)
            console->Ajouter("/Lectures des tilesets.");

        do
        {
            //Chargement des tileset
            fichier->get(caractere);
            if (caractere=='*')
            {
                string cheminDuTileset;
                *fichier>>cheminDuTileset;
                m_herbe.push_back(moteurGraphique->AjouterTileset(cheminDuTileset));
            }
            if (fichier->eof())
            {
                console->Ajouter("Erreur : Map \" "+chemin+" \" Invalide",1);
                throw ("Erreur : Map \" "+chemin+" \" Invalide");
            }

        }
        while (caractere!='$');

        if (configuration->debug)
            console->Ajouter("/Lectures des herbes.");


        do
        {

            //Chargement des tileset
            fichier->get(caractere);
            if (caractere=='*')
            {
                string cheminDuMonstre;
                //getline(fichier, cheminDuMonstre);
                *fichier>>cheminDuMonstre;
                m_ModeleMonstre.push_back(Modele_Monstre ());
                m_ModeleMonstre.back().Charger(cheminDuMonstre);
                console->Ajouter("Chargement de : "+cheminDuMonstre+" terminé",0);

            }
            if (fichier->eof())
            {
                console->Ajouter("Erreur : Map \" "+chemin+" \" Invalide",1);
                return 0;
            }
        }
        while (caractere!='$');

        if (configuration->debug)
            console->Ajouter("/Lectures des monstres.");

        console->Ajouter("");

//        Decor decorTemp(-1,-1,std::vector<int> (),std::vector<int> (),-1,0,0);

        coordonnee position;

            for (int couche=0;couche<NOMBRE_COUCHE_MAP;couche++)
            {
                position.x=0;
                position.y=0;

                do
                {
                    //Chargement des "décors", des cases de la map
                    fichier->get(caractere);
                    if (caractere=='*')
                    {
                        m_decor[couche].push_back(std::vector<std::vector<Decor > > ());
                        do
                        {
                            Caracteristique tempCaract;

                            int pos;
                            int tileset=-1,tileFinal=-1,herbe=-1,layer=0,hauteur=0;
                            int temp;
                            int entite_angle = 0;
                            vector <int>tile;
                            vector <int>monstre, monstreFinal;
                            vector <Objet> objets;

                            Entite_graphique entite_decor;
                            bool dernierEtaitMonstre = false;

                            m_decor[couche][position.y].push_back(std::vector<Decor > ());
                            do
                            {
                                switch (caractere)
                                {

                                case '+':
                                    fichier->get(caractere);
                                    if(caractere == 'm')
                                    {
                                        int temp3;
                                        *fichier>>temp3;
                                        if (temp3>=0&&temp3<(int)m_ModeleMonstre.size())
                                        {
                                            dernierEtaitMonstre = true;
                                            m_monstre.push_back(Monstre ());
                                            m_monstre.back().Charger(temp3,&m_ModeleMonstre[temp3]);
                                            m_monstre.back().setCoordonnee(position),m_monstre.back().setDepart();



                                            //TrierInventaire(m_monstre.back().getPointeurObjets(),hero->m_classe.position_contenu_marchand.w);
                                            monstreFinal.push_back(m_monstre.size()-1);

                                            m_add_monstre.push_back(true);
                                        }
                                    }
                                    break;

                                case 's':
                                    *fichier>>tileset;
                                    break;
                                case 't':
                                    *fichier>>temp;
                                    tile.push_back(temp);
                                    dernierEtaitMonstre = false;
                                    break;
                                case 'm':
                                    int temp3;
                                    *fichier>>temp3;
                                    if (temp3>=0&&temp3<(int)m_ModeleMonstre.size())
                                    {
                                        dernierEtaitMonstre = true;
                                        m_monstre.push_back(Monstre ());
                                        m_monstre.back().Charger(temp3,&m_ModeleMonstre[temp3]);
                                        m_monstre.back().setCoordonnee(position),m_monstre.back().setDepart();



                                        //TrierInventaire(m_monstre.back().getPointeurObjets(),hero->m_classe.position_contenu_marchand.w);
                                        monstreFinal.push_back(m_monstre.size()-1);

                                        m_add_monstre.push_back(false);
                                    }
                                    break;
                                case 'd':
                                    *fichier>>m_monstre.back().m_ID;
                                    if(m_monstre.back().m_ID > m_max_ID)
                                        m_max_ID = m_monstre.back().m_ID;
                                    break;
                                case 'h':
                                    *fichier>>herbe;
                                    break;
                                case 'l':
                                    *fichier>>layer;
                                    break;
                                case 'i':
                                    *fichier>>hauteur;
                                    break;
                                case 'a':
                                    *fichier>>entite_angle;
                                    if(!m_monstre.empty())
                                        m_monstre.back().setAngle(entite_angle);
                                    break;

                                case 'o':
                                    pos=fichier->tellg();
                                    objets.push_back(Objet ());
                                    objets.back().ChargerTexte(fichier,tempCaract,true);
                                    objets.back().Charger(objets.back().getChemin(),tempCaract,true);
                                    fichier->seekg(pos, ios::beg);
                                    objets.back().m_benedictions.clear();
                                    objets.back().ChargerTexte(fichier,tempCaract);
                                    break;

                                case 'p':
                                    if(dernierEtaitMonstre)
                                        m_monstre.back().m_entite_graphique.LoadParameters(*fichier);
                                    else
                                        entite_decor.LoadParameters(*fichier);
                                    break;

                                case 'r':
                                    int r_tileset = tileset;
                                    int r_herbe=-1;
                                    int r_layer=0;
                                    int r_hauteur=0;
                                    vector <int>r_tile;
                                    vector <int>r_monstre, r_monstreFinal;
                                    vector <Objet> r_objets;
                                    Entite_graphique r_entite_decor;
                                    bool r_dernierEtaitMonstre;

                                    fichier->get(caractere);
                                    int noModuleCaseMin=-1,noModuleCaseMax=-1;
                                    do
                                    {
                                        if (caractere=='i')
                                            *fichier>>noModuleCaseMin;
                                        else if (caractere=='a')
                                            *fichier>>noModuleCaseMax;
                                        else if (caractere=='*')
                                        {
                                            do
                                            {
                                                fichier->get(caractere);

                                                //if (numeroModuleAleatoire>=noModuleCaseMin&&numeroModuleAleatoire<=noModuleCaseMax)
                                                    switch (caractere)
                                                    {
                                                    case '+':
                                                        fichier->get(caractere);
                                                        if(caractere == 'm')
                                                        {
                                                            int temp3;
                                                            *fichier>>temp3;
                                                            if (temp3>=0&&temp3<(int)m_ModeleMonstre.size())
                                                            {
                                                                r_dernierEtaitMonstre = true;
                                                                m_monstre.push_back(Monstre ());
                                                                m_monstre.back().Charger(temp3,&m_ModeleMonstre[temp3]);
                                                                m_monstre.back().setCoordonnee(position),m_monstre.back().setDepart();



                                                                //TrierInventaire(m_monstre.back().getPointeurObjets(),hero->m_classe.position_contenu_marchand.w);
                                                                r_monstreFinal.push_back(m_monstre.size()-1);

                                                                m_add_monstre.push_back(true);
                                                            }
                                                        }
                                                        break;
                                                    case 's':
                                                        *fichier>>r_tileset;
                                                        break;
                                                    case 't':
                                                        *fichier>>temp;
                                                        r_tile.push_back(temp);
                                                        r_dernierEtaitMonstre = false;
                                                        break;
                                                    case 'm':
                                                        int temp3;
                                                        *fichier>>temp3;
                                                        if (temp3>=0&&temp3<(int)m_ModeleMonstre.size())
                                                        {
                                                            r_dernierEtaitMonstre = true;
                                                            m_monstre.push_back(Monstre ());
                                                            m_monstre.back().Charger(temp3,&m_ModeleMonstre[temp3]);
                                                            m_monstre.back().setCoordonnee(position),m_monstre.back().setDepart();



                                                            r_monstreFinal.push_back(m_monstre.size()-1);

                                                            m_add_monstre.push_back(false);
                                                        }
                                                        break;
                                                    case 'd':
                                                        *fichier>>m_monstre.back().m_ID;
                                                        if(m_monstre.back().m_ID > m_max_ID)
                                                            m_max_ID = m_monstre.back().m_ID;
                                                        break;
                                                    case 'h':
                                                        *fichier>>r_herbe;
                                                        break;
                                                    case 'l':
                                                        *fichier>>r_layer;
                                                        break;
                                                    case 'i':
                                                        *fichier>>r_hauteur;
                                                        break;
                                                    case 'a':
                                                        *fichier>>entite_angle;
                                                        if(!m_monstre.empty())
                                                            m_monstre.back().setAngle(entite_angle);
                                                        break;

                                                    case 'p':
                                                        if(r_dernierEtaitMonstre)
                                                            m_monstre.back().m_entite_graphique.LoadParameters(*fichier);
                                                        else
                                                            r_entite_decor.LoadParameters(*fichier);

                                                        break;

                                                    case 'o':
                                                        pos=fichier->tellg();
                                                        r_objets.push_back(Objet ());
                                                        r_objets.back().ChargerTexte(fichier,tempCaract,true);
                                                        r_objets.back().Charger(objets.back().getChemin(),tempCaract,true);
                                                        fichier->seekg(pos, ios::beg);
                                                        r_objets.back().m_benedictions.clear();
                                                        r_objets.back().ChargerTexte(fichier,tempCaract);
                                                        break;
                                                    }

                                                if (fichier->eof())
                                                {
                                                    console->Ajouter("Erreur : Map \" "+chemin+" \" Invalide",1);
                                                    return 0;
                                                }
                                            }
                                            while (caractere!='$');

                                           /* if (couche==1)
                                                if (m_decor[0][position.y][position.x].back().getHerbe()>=0&&herbe<0)
                                                    r_herbe=m_decor[0][position.y][position.x].back().getHerbe();*/

                                            if (couche==0)
                                                r_layer+=1;
                                            else
                                                r_layer+=10;

                                            m_decor[couche][position.y].back().push_back(Decor (r_tileset,r_tile,r_monstreFinal,r_herbe,r_layer,r_hauteur,r_objets));
                                            m_decor[couche][position.y].back().back().m_moduleAleatoireMin = noModuleCaseMin;
                                            m_decor[couche][position.y].back().back().m_moduleAleatoireMax = noModuleCaseMax;
                                            m_decor[couche][position.y].back().back().m_entite_graphique = r_entite_decor;
                                        }

                                        fichier->get(caractere);

                                        if (fichier->eof())
                                        {
                                            console->Ajouter("Erreur : Map \" "+chemin+" \" Invalide",1);
                                            return 0;
                                        }
                                    }
                                    while (caractere!='$');
                                    break;
                                }
                                fichier->get(caractere);
                                if (fichier->eof())
                                {
                                    console->Ajouter("Erreur : Map \" "+chemin+" \" Invalide",1);
                                    return 0;
                                }
                            }
                            while (caractere!='|' && caractere!='$');

                            if (caractere!='$')
                            {
                                /*if (couche==1)
                                    if (m_decor[0][position.y][position.x].back().getHerbe()>=0&&herbe<0)
                                        herbe=m_decor[0][position.y][position.x].back().getHerbe();*/

                                if (fichier->eof())
                                {
                                    console->Ajouter("Erreur : Map \" "+chemin+" \" Invalide",1);
                                    return 0;
                                }

                                if (couche==0)
                                    layer+=1;
                                else
                                    layer+=10;

                                m_decor[couche][position.y].back().push_back(Decor (tileset,tile,monstreFinal,herbe,layer,hauteur,objets));
                                m_decor[couche][position.y].back().back().m_moduleAleatoireMin = 0;
                                m_decor[couche][position.y].back().back().m_moduleAleatoireMax = 9;
                                m_decor[couche][position.y].back().back().m_entite_graphique = entite_decor;

                                tileset=-1,tile.clear(),tileFinal=-1,monstreFinal.clear(),herbe=-1,layer=0,hauteur=0;
                                objets.clear();
                                position.x++;
                            }
                            else
                            {
                                m_decor[couche][position.y].erase(m_decor[couche][position.y].end() - 1);
                            }
                        }
                        while (caractere!='$');

                        fichier->get(caractere);

                        position.x=0;
                        position.y++;
                    }
                    if (fichier->eof())
                    {
                        console->Ajouter("Erreur : Map \" "+chemin+" \" Invalide",1);
                        return 0;
                    }
                }
                while (caractere!='$');

        }
        if (configuration->debug)
            console->Ajouter("/Lectures des cases.");

        m_script.Charger(*fichier);

        fichier->close();
    }
    else
    {
        console->Ajouter("Impossible d'ouvrir le fichier : "+chemin,1);
        return 0;
    }


    for (int i=0;i<24;++i)
        if (m_lumiere[i].intensite<1)
            m_lumiere[i].intensite=1;

    delete fichier;

    m_dimensions.x=(int)m_decor[0][0].size();
    m_dimensions.y=(int)m_decor[0].size();

    console->Ajouter("Chargement de la map terminé.");
    console->Ajouter("");

    Initialiser();

    console->Ajouter("Initialisation de la map terminé.");
    console->Ajouter("");

    m_pret = true;

    return 1;
}


void Map::Initialiser()
{
    //CalculerOmbresEtLumieres();

    for (int couche=0;couche<2;couche++)
        for (int i=0;i<m_dimensions.y;++i)
            for (int j=0;j<m_dimensions.x;j++)
            for (unsigned z=0;z<m_decor[couche][i][j].size();z++)
            {
                if (m_decor[couche][i][j][z].getHerbe()>=0&&m_decor[couche][i][j][z].getHerbe()<(int)m_herbe.size())
                {
                    if (moteurGraphique->getTileset(m_herbe[m_decor[couche][i][j][z].getHerbe()])->getTaille()>0)
                    {
                        coordonnee position,positionPartieDecor;

                        int numeroHerbe=0;
                        if (moteurGraphique->getTileset(m_herbe[m_decor[couche][i][j][z].getHerbe()])->getTaille()>0)
                            numeroHerbe = (rand() % (moteurGraphique->getTileset(m_herbe[m_decor[couche][i][j][z].getHerbe()])->getTaille()));
                        m_decor[couche][i][j][z].setNumeroHerbe(numeroHerbe);

                        position.x=(j-i)*64;
                        position.y=(j+i+1)*32;

                        if (couche==0)
                            position.y-=32;
                        position.x+=m_decor[0][i][j][z].getDecalageHerbe().x;

                        m_decor[couche][i][j][z].m_entite_herbe = moteurGraphique->getEntiteGraphique(m_herbe[m_decor[couche][i][j][z].getHerbe()], numeroHerbe, 10);
                        m_decor[couche][i][j][z].m_entite_herbe.m_sprite.SetPosition(position.x, position.y - m_decor[0][i][j][z].getHauteur());
                        m_decor[couche][i][j][z].m_entite_herbe.m_sprite.SetScale((float)m_decor[couche][i][j][z].getTailleHerbe()/100,(float)m_decor[couche][i][j][z].getTailleHerbe()/100);
                        m_decor[couche][i][j][z].m_entite_herbe.m_sprite.SetColor(m_decor[couche][i][j][z].getCouleurHerbe());

                        m_decor[couche][i][j][z].m_entite_herbe.Initialiser(coordonnee ());




                      /*  coordonnee position,positionPartieDecor;

                        int numeroHerbe=0;
                        if (moteurGraphique->getTileset(m_herbe[m_decor[couche][i][j][z].getHerbe()])->getTaille()>0)
                            numeroHerbe = (rand() % (moteurGraphique->getTileset(m_herbe[m_decor[couche][i][j][z].getHerbe()])->getTaille()));
                        m_decor[couche][i][j][z].setNumeroHerbe(numeroHerbe);

                        position.x=(j-i-1)*64;
                        position.y=(j+i)*32;



                        positionPartieDecor=moteurGraphique->getTileset(m_herbe[m_decor[couche][i][j][z].getHerbe()])->getPositionDuTile(m_decor[couche][i][j][z].getNumeroHerbe());

                        if (couche==0)
                            position.y-=32;
                        position.x+=m_decor[couche][i][j][z].getDecalageHerbe().x;

                        m_decor[couche][i][j][z].m_entite_herbe.m_sprite.SetImage(*moteurGraphique->getImage(moteurGraphique->getTileset(m_herbe[m_decor[couche][i][j][z].getHerbe()])->getImage(m_decor[couche][i][j][z].getNumeroHerbe())));
                        m_decor[couche][i][j][z].m_entite_herbe.m_sprite.SetSubRect(IntRect(positionPartieDecor.x, positionPartieDecor.y, positionPartieDecor.x+positionPartieDecor.w, positionPartieDecor.y+positionPartieDecor.h));
                        m_decor[couche][i][j][z].m_entite_herbe.m_sprite.SetScale((float)m_decor[couche][i][j][z].getTailleHerbe()/100,(float)m_decor[couche][i][j][z].getTailleHerbe()/100);
                        m_decor[couche][i][j][z].m_entite_herbe.m_sprite.SetX(position.x+64-positionPartieDecor.w/2);
                        m_decor[couche][i][j][z].m_entite_herbe.m_sprite.SetY(position.y-positionPartieDecor.h+64);
                        m_decor[couche][i][j][z].m_entite_herbe.m_sprite.SetColor(m_decor[couche][i][j][z].getCouleurHerbe());*/
                    }
                }
            }

    /*for (int i=0;i<(int)m_monstre.size();++i)
        if (m_monstre[i].getCaracteristique().rang>=0)
        {
            m_monstre[i].m_light=moteurGraphique->LightManager->Add_Dynamic_Light();
            moteurGraphique->LightManager->SetQuality(m_monstre[i].m_light,6);

            sf::Vector2f pos;
            pos.x=(((m_monstre[i].getCoordonneePixel().x-m_monstre[i].getCoordonneePixel().y)*64/COTE_TILE));
            pos.y=(((m_monstre[i].getCoordonneePixel().x+m_monstre[i].getCoordonneePixel().y)*64/COTE_TILE)/2+32)*2;

            moteurGraphique->LightManager->SetPosition(m_monstre[i].m_light,pos);

            moteurGraphique->LightManager->SetColor(m_monstre[i].m_light,sf::Color(m_monstre[i].getPorteeLumineuse().rouge,m_monstre[i].getPorteeLumineuse().vert,m_monstre[i].getPorteeLumineuse().bleu));
        }*/

    sf::Vector2f pos;

    for (int i=0;i<NOMBRE_COUCHE_MAP;++i)
        for (int j=0;j<m_dimensions.y;j++)
            for (int k=0;k<m_dimensions.x;k++)
                {
                    CreerSprite(sf::Vector3f(k,j,i));
                }

}

void Map::CreerSprite(sf::Vector3f position_case)
{
    for(unsigned z = 0 ; z < m_decor[(int)position_case.z][(int)position_case.y][(int)position_case.x].size() ; ++z)
    if (m_decor[(int)position_case.z][(int)position_case.y][(int)position_case.x][z].getTileset()>=0
      &&m_decor[(int)position_case.z][(int)position_case.y][(int)position_case.x][z].getTileset()<(int)m_tileset.size())
    {
        coordonnee position,positionPartieDecor;

        position.x=((int)position_case.x-(int)position_case.y-1)*64;
        position.y=((int)position_case.x+(int)position_case.y)*32;

        positionPartieDecor=moteurGraphique->getTileset(m_tileset[m_decor[(int)position_case.z][(int)position_case.y][(int)position_case.x][z].getTileset()])->getPositionDuTile(m_decor[(int)position_case.z][(int)position_case.y][(int)position_case.x][z].getTile()[m_decor[(int)position_case.z][(int)position_case.y][(int)position_case.x][z].random_animation]);

        m_decor[(int)position_case.z][(int)position_case.y][(int)position_case.x][z].m_entite_graphique.m_sprite.SetImage(*moteurGraphique->getImage(moteurGraphique->getTileset(m_tileset[m_decor[(int)position_case.z][(int)position_case.y][(int)position_case.x][z].getTileset()])->getImage(m_decor[(int)position_case.z][(int)position_case.y][(int)position_case.x][z].getTile()[m_decor[(int)position_case.z][(int)position_case.y][(int)position_case.x][z].random_animation])));
        m_decor[(int)position_case.z][(int)position_case.y][(int)position_case.x][z].m_entite_graphique.m_sprite.SetSubRect(IntRect(positionPartieDecor.x, positionPartieDecor.y, positionPartieDecor.w, positionPartieDecor.h));

        m_decor[(int)position_case.z][(int)position_case.y][(int)position_case.x][z].m_entite_graphique.m_sprite.SetOrigin(moteurGraphique->getTileset(m_tileset[m_decor[(int)position_case.z][(int)position_case.y][(int)position_case.x][z].getTileset()])->getCentreDuTile(m_decor[(int)position_case.z][(int)position_case.y][(int)position_case.x][z].getTile()[m_decor[(int)position_case.z][(int)position_case.y][(int)position_case.x][z].random_animation]).x,
                                                                                                     moteurGraphique->getTileset(m_tileset[m_decor[(int)position_case.z][(int)position_case.y][(int)position_case.x][z].getTileset()])->getCentreDuTile(m_decor[(int)position_case.z][(int)position_case.y][(int)position_case.x][z].getTile()[m_decor[(int)position_case.z][(int)position_case.y][(int)position_case.x][z].random_animation]).y);


        m_decor[(int)position_case.z][(int)position_case.y][(int)position_case.x][z].m_entite_graphique.m_sprite.SetX(position.x+64);
        m_decor[(int)position_case.z][(int)position_case.y][(int)position_case.x][z].m_entite_graphique.m_sprite.SetY(position.y+32);

       /* m_decor[(int)position_case.z][(int)position_case.y][(int)position_case.x][z].m_entite_graphique.m_sprite.SetScale(
            m_decor[(int)position_case.z][(int)position_case.y][(int)position_case.x][z].m_entite_graphique.m_scale.x*0.01,
            m_decor[(int)position_case.z][(int)position_case.y][(int)position_case.x][z].m_entite_graphique.m_scale.y*0.01);

        m_decor[(int)position_case.z][(int)position_case.y][(int)position_case.x][z].m_entite_graphique.m_sprite.SetRotation(
            m_decor[(int)position_case.z][(int)position_case.y][(int)position_case.x][z].m_entite_graphique.m_rotation);

        m_decor[(int)position_case.z][(int)position_case.y][(int)position_case.x][z].m_entite_graphique.m_sprite.SetColor(
            m_decor[(int)position_case.z][(int)position_case.y][(int)position_case.x][z].m_entite_graphique.m_color);*/

    }

    //m_decor[(int)position_case.z][(int)position_case.y][(int)position_case.x].m_sprite.SetColor(sf::Color(255,255,255,m_tileset[m_decor[(int)position_case.z][(int)position_case.y][(int)position_case.x].getTileset()].getOpacityDuTile(m_decor[(int)position_case.z][(int)position_case.y][(int)position_case.x].getTile()[m_decor[(int)position_case.z][(int)position_case.y][(int)position_case.x].random_animation])));
}


void Map::Sauvegarder(std::string chemin)
{
   // string chemin = configuration->chemin_temps+m_nom_fichier;

    console->Ajouter("",0);
    console->Ajouter("Sauvegarde de la map : "+chemin,0);

    ofstream fichier(chemin.c_str(), ios::out | ios::trunc);

    if (fichier)
    {
        fichier<<"* n"<<m_no_nom<<" $ \n$\n";
        for (int i=0;i<(int)m_fond.size();++i)
            fichier<<"*"<<m_fond[i]<<"\n";
        fichier<<"$\n";
        for (int i=0;i<(int)m_cheminMusique.size();++i)
            fichier<<"*"<<m_cheminMusique[i]<<"\n";
        fichier<<"$\n";

        for (int i=0;i<24;++i)
            fichier<<"*"<<m_lumiere[i].rouge<<" "<<m_lumiere[i].vert<<" "<<m_lumiere[i].bleu<<" "<<m_lumiere[i].intensite<<" "<<m_lumiere[i].hauteur<<"\n";

        fichier<<"$\n";

        if(!m_nom_img_sky.empty())
            fichier<<" *"<<m_nom_img_sky<<endl;
        fichier<<"$\n";

        for(int i = 0 ; i < m_climates.size() ; ++i)
            fichier<<" *"<<m_climates[i]<<endl;
        fichier<<"$\n";


        for (int i=0;i<(int)m_tileset.size();++i)
            fichier<<"*"<<moteurGraphique->getTileset(m_tileset[i])->getChemin()<<"\n";

        fichier<<"$\n";

        for (int i=0;i<(int)m_herbe.size();++i)
            fichier<<"*"<<moteurGraphique->getTileset(m_herbe[i])->getChemin()<<"\n";

        fichier<<"$\n";

        for (int i=0;i<(int)m_ModeleMonstre.size();++i)
            fichier<<"*"<<m_ModeleMonstre[i].m_chemin<<"\n";

        fichier<<"$\n";

        for (int couche=0;couche<2;couche++)
        {
            for (int i=0;i<m_dimensions.y;++i)
            {
                fichier<<"* ";
                for (int j=0;j<m_dimensions.x;j++)
                {
                    for(unsigned z = 0 ; z < m_decor[couche][i][j].size() ; ++z)
                    if(m_decor[couche][i][j][z].m_tile.size() > 0
                    || m_decor[couche][i][j][z].m_monstre.size() > 0
                    || m_decor[couche][i][j][z].m_herbe >= 0
                    || m_decor[couche][i][j][z].getNombreObjets() > 0
                    || m_decor[couche][i][j][z].getHauteur() > 0 )
                    {
                        if(m_decor[couche][i][j][z].m_moduleAleatoireMin != 0 || m_decor[couche][i][j][z].m_moduleAleatoireMax != 9)
                            fichier<<" r i"<<m_decor[couche][i][j][z].m_moduleAleatoireMin<<" a"<<m_decor[couche][i][j][z].m_moduleAleatoireMax<<" * ";

                        if(m_decor[couche][i][j][z].getTileset() >= 0)
                            fichier<<" s"<<m_decor[couche][i][j][z].getTileset()<<" ";
                        for (unsigned k = 0 ; k < m_decor[couche][i][j][z].getTile().size() ; ++k)
                        {
                            fichier<<"t"<<m_decor[couche][i][j][z].getTile()[k]<<" ";
                            m_decor[couche][i][j][z].m_entite_graphique.SaveParameters(fichier);
                        }

                        for (unsigned k = 0 ; k < m_decor[couche][i][j][z].getMonstre().size() ; ++k)
                            if (m_decor[couche][i][j][z].getMonstre()[k] >= 0 && m_decor[couche][i][j][z].getMonstre()[k] < (int)m_monstre.size())
                            {
                                if(m_add_monstre[m_decor[couche][i][j][z].getMonstre()[k]])
                                    fichier<<"+";
                                fichier<<"m"<<m_monstre[m_decor[couche][i][j][z].getMonstre()[k]].getModele()<<" ";
                                if(m_monstre[m_decor[couche][i][j][z].getMonstre()[k]].getAngle() != 0)
                                    fichier<<"a"<<m_monstre[m_decor[couche][i][j][z].getMonstre()[k]].getAngle()<<" ";

                                m_monstre[m_decor[couche][i][j][z].getMonstre()[k]].m_entite_graphique.SaveParameters(fichier);


                                if(m_monstre[m_decor[couche][i][j][z].getMonstre()[k]].m_ID >= 0)
                                    fichier<<" d"<<m_monstre[m_decor[couche][i][j][z].getMonstre()[k]].m_ID<<" ";
                            }

                        if(m_decor[couche][i][j][z].getHerbe() >= 0)
                            fichier<<"h"<<m_decor[couche][i][j][z].getHerbe()<<" ";

                        if(m_decor[couche][i][j][z].getHauteur() > 0)
                            fichier<<"i"<<m_decor[couche][i][j][z].getHauteur()<<" ";

                        if (couche==0)
                        {
                            if(m_decor[couche][i][j][z].getCouche()-1 > 0)
                                fichier<<"l"<<m_decor[couche][i][j][z].getCouche()-1<<" ";
                        }
                        else
                            fichier<<"l"<<m_decor[couche][i][j][z].getCouche()-10<<" ";

                        for (int o=0;o<(int)m_decor[couche][i][j][z].getNombreObjets();o++)
                            m_decor[couche][i][j][z].getObjet(o)->SauvegarderTexte(&fichier);

                        if(m_decor[couche][i][j][z].m_moduleAleatoireMin != 0 || m_decor[couche][i][j][z].m_moduleAleatoireMax != 9)
                            fichier<<" $ $ ";
                    }
                    fichier<<"|";
                }
                fichier<<"$\n";
            }
            fichier<<"$\n";
        }

        //m_script.Sauvegarder(fichier);
        fichier<<endl<<m_script.m_text<<endl;

        fichier.close();
    }
    else
    {
        console->Ajouter("Impossible d'ouvrir le fichier : "+chemin,1);
        throw "";
    }

    console->Ajouter("Sauvegarde de la map terminée !");
}





void Map::CalculerOmbresEtLumieres()
{
    if (configuration->heure+1<24)
    {
        moteurGraphique->m_soleil.intensite=(int)(((float)m_lumiere[configuration->heure].intensite*(60-configuration->minute)+((float)m_lumiere[configuration->heure+1].intensite*configuration->minute))*0.016666666666666666666666666666667);
        moteurGraphique->m_soleil.rouge=(int)(((float)m_lumiere[configuration->heure].rouge*(60-configuration->minute)+((float)m_lumiere[configuration->heure+1].rouge*configuration->minute))*0.016666666666666666666666666666667);
        moteurGraphique->m_soleil.vert=(int)(((float)m_lumiere[configuration->heure].vert*(60-configuration->minute)+((float)m_lumiere[configuration->heure+1].vert*configuration->minute))*0.016666666666666666666666666666667);
        moteurGraphique->m_soleil.bleu=(int)(((float)m_lumiere[configuration->heure].bleu*(60-configuration->minute)+((float)m_lumiere[configuration->heure+1].bleu*configuration->minute))*0.016666666666666666666666666666667);
        moteurGraphique->m_soleil.hauteur=((float)m_lumiere[configuration->heure].hauteur*(60-configuration->minute)+((float)m_lumiere[configuration->heure+1].hauteur*configuration->minute))*0.016666666666666666666666666666667;
    }
    else
    {
        moteurGraphique->m_soleil.intensite=(int)(((float)m_lumiere[configuration->heure].intensite*(60-configuration->minute)+((float)m_lumiere[0].intensite*configuration->minute))*0.016666666666666666666666666666667);
        moteurGraphique->m_soleil.rouge=(int)(((float)m_lumiere[configuration->heure].rouge*(60-configuration->minute)+((float)m_lumiere[0].rouge*configuration->minute))*0.016666666666666666666666666666667);
        moteurGraphique->m_soleil.vert=(int)(((float)m_lumiere[configuration->heure].vert*(60-configuration->minute)+((float)m_lumiere[0].vert*configuration->minute))*0.016666666666666666666666666666667);
        moteurGraphique->m_soleil.bleu=(int)(((float)m_lumiere[configuration->heure].bleu*(60-configuration->minute)+((float)m_lumiere[0].bleu*configuration->minute))*0.016666666666666666666666666666667);
        moteurGraphique->m_soleil.hauteur=((float)m_lumiere[configuration->heure].hauteur*(60-configuration->minute)+((float)m_lumiere[0].hauteur*configuration->minute))*0.016666666666666666666666666666667;
    }

    moteurGraphique->m_angleOmbreSoleil=((float)configuration->heure*60+configuration->minute)*180/720;

}

void Map::AfficherSac(coordonnee positionSac,float decalage,coordonnee position_sac_inventaire,Caracteristique caract)
{
   /* Sprite Sprite;
    String texte;

    m_objetPointe=-1;
    coordonnee position;

    if (positionSac.x>=0&&positionSac.x<m_dimensions.x
            &&positionSac.y>=0&&positionSac.y<m_dimensions.y)
        if (m_decor[1][positionSac.y][positionSac.x].getNombreObjets()>0)
        {

            if (m_defilerObjets>m_decor[1][positionSac.y][positionSac.x].getNombreObjets()-position_sac_inventaire.h+1)
                m_defilerObjets=m_decor[1][positionSac.y][positionSac.x].getNombreObjets()-position_sac_inventaire.h+1;
            if (m_defilerObjets<0)
                m_defilerObjets=0;

            for (int z=m_defilerObjets;z<m_decor[1][positionSac.y][positionSac.x].getNombreObjets()&&z<position_sac_inventaire.h+m_defilerObjets;z++)
            {
                if (eventManager->getPositionSouris().x>position_sac_inventaire.x*configuration->Resolution.w/800
                        &&eventManager->getPositionSouris().x<(position_sac_inventaire.x+position_sac_inventaire.w)*configuration->Resolution.w/800
                        &&eventManager->getPositionSouris().y>position_sac_inventaire.y*configuration->Resolution.h/600+(z-m_defilerObjets)*20*configuration->Resolution.w/800
                        &&eventManager->getPositionSouris().y<position_sac_inventaire.y*configuration->Resolution.h/600+(z-m_defilerObjets)*20*configuration->Resolution.w/800+20*configuration->Resolution.w/800)
                {
                    Sprite.SetImage(*moteurGraphique->getImage(0));
                    Sprite.SetColor(sf::Color(255,255,255,128));
                    Sprite.Resize(position_sac_inventaire.w*configuration->Resolution.w/800,20*configuration->Resolution.h/600);
                    Sprite.SetX(position_sac_inventaire.x*configuration->Resolution.w/800);
                    Sprite.SetY(position_sac_inventaire.y*configuration->Resolution.h/600+(z-m_defilerObjets)*20*configuration->Resolution.w/800 - decalage*configuration->Resolution.h/600);
                    moteurGraphique->AjouterCommande(&Sprite,16,0);

                    if (!(z-m_defilerObjets==0&&m_defilerObjets>0)&&!((z-m_defilerObjets==position_sac_inventaire.h-1&&z+m_defilerObjets<=m_decor[1][positionSac.y][positionSac.x].getNombreObjets()+1)))
                    {
                        m_objetPointe=z;
                        m_decor[1][positionSac.y][positionSac.x].getObjet(z)->AfficherCaracteristiques(eventManager->getPositionSouris(),caract);
                    }
                }


                //int rarete=m_decor[1][positionSac.y][positionSac.x].getObjet(z)->getRarete();

                texte.SetColor(GetItemColor(m_decor[1][positionSac.y][positionSac.x].getObjet(z)->getRarete()));
                texte.SetFont(moteurGraphique->m_font);

                if (z-m_defilerObjets==0&&m_defilerObjets>0)
                    texte.SetString("...");
                else if ((z-m_defilerObjets==position_sac_inventaire.h-1&&z+m_defilerObjets<=m_decor[1][positionSac.y][positionSac.x].getNombreObjets()+1))
                    texte.SetString("...");
                else
                    texte.SetString(m_decor[1][positionSac.y][positionSac.x].getObjet(z)->getNom());
                texte.SetSize(16*configuration->Resolution.w/800);

                position.x=(int)((position_sac_inventaire.x*configuration->Resolution.w/800)+((position_sac_inventaire.w/2)*configuration->Resolution.w/800)-(texte.GetRect().Right-texte.GetRect().Left)/2);
                position.y=(int)(position_sac_inventaire.y*configuration->Resolution.h/600+(z-m_defilerObjets)*20*configuration->Resolution.w/800 - decalage*configuration->Resolution.h/600 );

                texte.SetY(position.y);
                texte.SetX(position.x);

                moteurGraphique->AjouterTexte(&texte,16);
            }
        }*/
}

void Map::Afficher(bool alt,float alpha)
{
    coordonnee positionHero;

    Sprite sprite;
    String texte;

    //positionHero.y=(int)((hero->m_personnage.getCoordonneePixel().x+hero->m_personnage.getCoordonneePixel().y)*DIVISEUR_COTE_TILE*32);
    //positionHero.x=(int)(((hero->m_personnage.getCoordonneePixel().x-hero->m_personnage.getCoordonneePixel().y)*DIVISEUR_COTE_TILE-1)*64);

    coordonnee position;


    for (int couche=0;couche<NOMBRE_COUCHE_MAP;couche++)
    {
        if(m_afficher_couche0 && couche == 0 || m_afficher_couche1 && couche == 1)
        for (int j=eventManager->m_coordonnee.y-10*configuration->zoom;j<eventManager->m_coordonnee.y+10*configuration->zoom;++j)
        {
            for (int k=eventManager->m_coordonnee.x-10*configuration->zoom;k<eventManager->m_coordonnee.x+10*configuration->zoom;++k)
            {
                position.x=(float)(k-j-1)*64+48;
                position.y=(float)(k+j)*32+16;

                if (j>=0&&j<m_dimensions.y&&k>=0&&k<m_dimensions.x)
                for (int z=0;z<(int) m_decor[couche][j][k].size();++z)
                if( m_moduleAleatoireCur >= m_decor[couche][j][k][z].m_moduleAleatoireMin && m_moduleAleatoireCur <= m_decor[couche][j][k][z].m_moduleAleatoireMax)
                {
                    if (m_decor[couche][j][k][z].m_entite_graphique.m_sprite.GetSize().x>0)
                        if (m_decor[couche][j][k][z].m_entite_graphique.m_sprite.GetPosition().x+m_decor[couche][j][k][z].m_entite_graphique.m_sprite.GetSize().x-m_decor[couche][j][k][z].m_entite_graphique.m_sprite.GetOrigin().x>=GetViewRect(moteurGraphique->m_camera).Left
                                &&m_decor[couche][j][k][z].m_entite_graphique.m_sprite.GetPosition().x-m_decor[couche][j][k][z].m_entite_graphique.m_sprite.GetOrigin().x<GetViewRect(moteurGraphique->m_camera).Left + GetViewRect(moteurGraphique->m_camera).Width
                                &&m_decor[couche][j][k][z].m_entite_graphique.m_sprite.GetPosition().y+m_decor[couche][j][k][z].m_entite_graphique.m_sprite.GetSize().y-m_decor[couche][j][k][z].m_entite_graphique.m_sprite.GetOrigin().y>=GetViewRect(moteurGraphique->m_camera).Top
                                &&m_decor[couche][j][k][z].m_entite_graphique.m_sprite.GetPosition().y-m_decor[couche][j][k][z].m_entite_graphique.m_sprite.GetOrigin().y<GetViewRect(moteurGraphique->m_camera).Top + GetViewRect(moteurGraphique->m_camera).Height)
                        {
                          /*  if (m_decor[couche][j][k][z].m_entite_graphique.m_sprite.GetPosition().x + m_decor[couche][j][k][z].m_entite_graphique.m_sprite.GetSize().x - m_decor[couche][j][k][z].m_entite_graphique.m_sprite.GetOrigin().x > GetViewRect(moteurGraphique->m_camera).Right)
                                rectBuf.Right -= (int)m_decor[couche][j][k][z].m_entite_graphique.m_sprite.GetPosition().x + (int)m_decor[couche][j][k][z].m_entite_graphique.m_sprite.GetSize().x - (int)m_decor[couche][j][k][z].m_entite_graphique.m_sprite.GetOrigin().x - (int)GetViewRect(moteurGraphique->m_camera).Right;
                            if (m_decor[couche][j][k][z].m_entite_graphique.m_sprite.GetPosition().y + m_decor[couche][j][k][z].m_entite_graphique.m_sprite.GetSize().y - m_decor[couche][j][k][z].m_entite_graphique.m_sprite.GetOrigin().y > GetViewRect(moteurGraphique->m_camera).Bottom)
                                rectBuf.Bottom -= (int)m_decor[couche][j][k][z].m_entite_graphique.m_sprite.GetPosition().y + (int)m_decor[couche][j][k][z].m_entite_graphique.m_sprite.GetSize().y - (int)m_decor[couche][j][k][z].m_entite_graphique.m_sprite.GetOrigin().y - (int)GetViewRect(moteurGraphique->m_camera).Bottom;

                            if (m_decor[couche][j][k][z].m_entite_graphique.m_sprite.GetPosition().x - m_decor[couche][j][k][z].m_entite_graphique.m_sprite.GetOrigin().x < GetViewRect(moteurGraphique->m_camera).Left)
                            {
                                rectBuf.Left += (int)GetViewRect(moteurGraphique->m_camera).Left - (int)m_decor[couche][j][k][z].m_entite_graphique.m_sprite.GetPosition().x + (int)m_decor[couche][j][k][z].m_entite_graphique.m_sprite.GetOrigin().x;
                                m_decor[couche][j][k][z].m_entite_graphique.m_sprite.Move(rectBuf.Left - rect.Left, 0);
                            }
                            if (m_decor[couche][j][k][z].m_entite_graphique.m_sprite.GetPosition().y - m_decor[couche][j][k][z].m_entite_graphique.m_sprite.GetOrigin().y < GetViewRect(moteurGraphique->m_camera).Top)
                            {
                                rectBuf.Top += (int)GetViewRect(moteurGraphique->m_camera).Top - (int)m_decor[couche][j][k][z].m_entite_graphique.m_sprite.GetPosition().y + (int)m_decor[couche][j][k][z].m_entite_graphique.m_sprite.GetOrigin().y;
                                m_decor[couche][j][k][z].m_entite_graphique.m_sprite.Move(0, rectBuf.Top - rect.Top);
                            }

                            m_decor[couche][j][k][z].m_entite_graphique.m_sprite.SetSubRect(rectBuf);*/

                            //moteurGraphique->AjouterCommande(&m_decor[couche][j][k][z].m_entite_graphique.m_sprite,m_decor[couche][j][k][z].getCouche(),1);
                            m_decor[couche][j][k][z].m_entite_graphique.m_couche = m_decor[couche][j][k][z].getCouche();
                            moteurGraphique->AjouterEntiteGraphique(&m_decor[couche][j][k][z].m_entite_graphique);

                        }

                    if(k == 0 || j == 0 || k == (int)m_decor[couche][j].size() - 1 || j == (int)m_decor[couche].size() - 1)
                    {
                        sf::Sprite temp;
                        temp.SetImage(*moteurGraphique->getImage(eventManager->m_img_select));
                        temp.SetPosition(position.x-48, position.y-16);
                        temp.SetColor(sf::Color(32,32,32,64));
                        moteurGraphique->AjouterCommande(&temp,15,1);
                    }

                    if(m_afficher_collision)
                        if(getCollision(k,j))
                        {
                            sf::Sprite temp;
                            temp.SetImage(*moteurGraphique->getImage(eventManager->m_img_select));
                            temp.SetPosition(position.x-48, position.y-16);
                            temp.SetColor(sf::Color(255,0,0,32));
                            moteurGraphique->AjouterCommande(&temp,15,1);
                        }

                    if(m_afficher_relief && couche == 0)
                    {
                        for(int l = 0 ; l < m_decor[couche][j][k][z].getHauteur(); l++)
                        {
                            sf::Sprite temp;
                            temp.SetImage(*moteurGraphique->getImage(eventManager->m_img_select));
                            temp.SetPosition(position.x-48, position.y-16 - l);
                            temp.SetColor(sf::Color(128,128,128,16));
                            moteurGraphique->AjouterCommande(&temp,15,1);
                        }

                        sf::Sprite temp;
                        temp.SetImage(*moteurGraphique->getImage(eventManager->m_img_select2));
                        temp.SetPosition(position.x-48, position.y-16 - m_decor[couche][j][k][z].getHauteur());
                        temp.SetColor(sf::Color(255,255,255,64));
                        if(m_decor[couche][j][k][z].getHauteur() > 0)
                        moteurGraphique->AjouterCommande(&temp,15,1);

                    }

                    if (couche==1)
                    {
                        if (configuration->Herbes)
                            if(z == m_decor[couche][j][k].size() - 1)
                            for (int u=0;u<(int) m_decor[0][j][k].size();++u)
                            if( m_moduleAleatoireCur >= m_decor[0][j][k][u].m_moduleAleatoireMin && m_moduleAleatoireCur <= m_decor[0][j][k][u].m_moduleAleatoireMax)
                            if (m_decor[0][j][k][u].m_entite_herbe.m_sprite.GetSize().x>0)
                                if (m_decor[0][j][k][u].m_entite_herbe.m_sprite.GetPosition().x+m_decor[0][j][k][u].m_entite_herbe.m_sprite.GetSize().x>=GetViewRect(moteurGraphique->m_camera).Left
                                        &&m_decor[0][j][k][u].m_entite_herbe.m_sprite.GetPosition().x<GetViewRect(moteurGraphique->m_camera).Left + GetViewRect(moteurGraphique->m_camera).Width
                                        &&m_decor[0][j][k][u].m_entite_herbe.m_sprite.GetPosition().y+m_decor[0][j][k][u].m_entite_herbe.m_sprite.GetSize().y>=GetViewRect(moteurGraphique->m_camera).Top
                                        &&m_decor[0][j][k][u].m_entite_herbe.m_sprite.GetPosition().y<GetViewRect(moteurGraphique->m_camera).Top + GetViewRect(moteurGraphique->m_camera).Height)
                                    moteurGraphique->AjouterCommande(&m_decor[0][j][k][u].m_entite_herbe.m_sprite,10,1);

                        for (unsigned o = 0 ; o < m_decor[1][j][k][z].getMonstre().size() ; ++o)
                        {
                            if(eventManager->getCasePointee().x == k
                            && eventManager->getCasePointee().y == j)
                            {
                                std::ostringstream buf;
                                buf<<m_ModeleMonstre[m_monstre[m_decor[1][j][k][z].getMonstre()[o]].getModele()].m_chemin;
                                moteurGraphique->AjouterTexte(buf.str(), coordonnee(((float)position.x - (float)GetViewRect(moteurGraphique->m_camera).Left)/configuration->zoom ,
                                                                                    ((float)position.y - (float)GetViewRect(moteurGraphique->m_camera).Top)/configuration->zoom - 32 - 14*o), 15,0,12,sf::Color(0,255,0));
                            }

                            sf::Sprite temp;
                            temp.SetImage(*moteurGraphique->getImage(eventManager->m_img_select));
                            temp.SetPosition(position.x-48, position.y-16);
                            temp.SetColor(sf::Color(0,255,0,32));
                            moteurGraphique->AjouterCommande(&temp,10,1);

                            if(m_monstre[m_decor[1][j][k][z].getMonstre()[o]].m_ID >= 0)
                            {
                                std::ostringstream buf;
                                buf<<m_monstre[m_decor[1][j][k][z].getMonstre()[o]].m_ID;
                                moteurGraphique->AjouterTexte(buf.str(), coordonnee(((float)position.x - (float)GetViewRect(moteurGraphique->m_camera).Left)/configuration->zoom , ((float)position.y - (float)GetViewRect(moteurGraphique->m_camera).Top)/configuration->zoom  ), 15,0,24/configuration->zoom,sf::Color(0,255,0));
                            }

                            if (m_decor[1][j][k][z].getMonstre()[o]>=0&&m_decor[1][j][k][z].getMonstre()[o]<(int)m_monstre.size())
                            {
                                m_monstre[m_decor[1][j][k][z].getMonstre()[o]].m_positionPixel.h = m_decor[0][j][k][0].getHauteur();
                                m_monstre[m_decor[1][j][k][z].getMonstre()[o]].Afficher(getDimensions(),&m_ModeleMonstre[m_monstre[m_decor[1][j][k][z].getMonstre()[o]].getModele()],m_decor[1][j][k][z].getMonstre()[o]==m_monstreIllumine);
                            }
                        }


                        if (configuration->Herbes)
                            if(z == m_decor[couche][j][k].size() - 1)
                            for (int u=0;u<(int) m_decor[1][j][k].size();++u)
                            if( m_moduleAleatoireCur >= m_decor[1][j][k][u].m_moduleAleatoireMin && m_moduleAleatoireCur <= m_decor[1][j][k][u].m_moduleAleatoireMax)
                            if (m_decor[1][j][k][u].m_entite_herbe.m_sprite.GetSize().x>0)
                                if (m_decor[1][j][k][u].m_entite_herbe.m_sprite.GetPosition().x+m_decor[1][j][k][u].m_entite_herbe.m_sprite.GetSize().x>=GetViewRect(moteurGraphique->m_camera).Left
                                        &&m_decor[1][j][k][u].m_entite_herbe.m_sprite.GetPosition().x<GetViewRect(moteurGraphique->m_camera).Left + GetViewRect(moteurGraphique->m_camera).Width
                                        &&m_decor[1][j][k][u].m_entite_herbe.m_sprite.GetPosition().y+m_decor[1][j][k][u].m_entite_herbe.m_sprite.GetSize().y>=GetViewRect(moteurGraphique->m_camera).Top
                                        &&m_decor[1][j][k][u].m_entite_herbe.m_sprite.GetPosition().y<GetViewRect(moteurGraphique->m_camera).Top + GetViewRect(moteurGraphique->m_camera).Height)
                                    moteurGraphique->AjouterCommande(&m_decor[1][j][k][u].m_entite_herbe.m_sprite,10,1);


                        if (m_decor[0][j][k][z].getProjectile()>=0&&m_decor[0][j][k][z].getProjectile()<(int)m_projectile.size())
                            m_projectile[m_decor[0][j][k][z].getProjectile()].Afficher();
                        if (m_decor[1][j][k][z].getProjectile()>=0&&m_decor[1][j][k][z].getProjectile()<(int)m_projectile.size())
                            m_projectile[m_decor[1][j][k][z].getProjectile()].Afficher();

                        if (m_decor[0][j][k][z].getEffetGraphique()>=0&&m_decor[0][j][k][z].getEffetGraphique()<(int)m_effets.size())
                            m_effets[m_decor[0][j][k][z].getEffetGraphique()].Afficher();
                        if (m_decor[1][j][k][z].getEffetGraphique()>=0&&m_decor[1][j][k][z].getEffetGraphique()<(int)m_effets.size())
                            m_effets[m_decor[1][j][k][z].getEffetGraphique()].Afficher();


                        if (m_decor[1][j][k][z].getNombreObjets()>0&&couche==1)
                        {
                            if (m_decor[1][j][k][z].getNombreObjets()<=4)
                            {
                                for (int o=0;o<m_decor[1][j][k][z].getNombreObjets();o++)
                                {
                                    sprite.SetImage(*moteurGraphique->getImage(m_decor[1][j][k][z].getObjet(o)->getImage()));
                                    sprite.SetSubRect(IntRect(m_decor[1][j][k][z].getObjet(o)->getPositionImage().x, m_decor[1][j][k][z].getObjet(o)->getPositionImage().y, m_decor[1][j][k][z].getObjet(o)->getPositionImage().w, m_decor[1][j][k][z].getObjet(o)->getPositionImage().h));
                                    sprite.SetScale(0.8,0.4);

                                    sprite.SetX(position.x-32+m_decor[1][j][k][z].getObjet(o)->getPosition().x*32+16-(m_decor[1][j][k][z].getObjet(o)->getPositionImage().w*0.8)/2);
                                    sprite.SetY(position.y+m_decor[1][j][k][z].getObjet(o)->getPosition().y*32);

                                    sprite.SetColor(m_decor[1][j][k][z].getObjet(o)->m_color);

                                    moteurGraphique->AjouterCommande(&sprite,8,1);

                                    sprite.SetBlendMode(Blend::Alpha);
                                    sprite.SetColor(sf::Color(255,255,255));
                                }
                            }
                            else
                            {
                                sprite.SetImage(*moteurGraphique->getImage(IDImageSac));
                                sprite.SetSubRect(IntRect(0,0,32,32));
                                sprite.Resize(32 , 32);

                                sprite.SetX(position.x);
                                sprite.SetY(position.y);

                                if (eventManager->getCasePointee().x==k&&eventManager->getCasePointee().y==j&&m_monstreIllumine<0&&m_decor[1][j][k][z].getNombreObjets()>4&&!alt&&m_monstreIllumine<0)
                                    sprite.SetColor(sf::Color(255,128,128));
                                else
                                    sprite.SetColor(sf::Color(255,255,255));

                                moteurGraphique->AjouterCommande(&sprite,8,1);
                            }


                            sprite.SetScale(1,1);

                            sprite.SetColor(sf::Color(255,255,255));

                            int objetPointe=-1;

                            if (alt)
                                m_decor[1][j][k][z].AlphaObjets(255);

                            objetPointe=m_decor[1][j][k][z].AfficherTexteObjets(position,m_objetPointe);

                            if (!eventManager->getEvenement(sf::Mouse::Left,"CA")&&moteurGraphique->getPositionSouris().x>sprite.GetPosition().x&&moteurGraphique->getPositionSouris().x<sprite.GetPosition().x+32&&moteurGraphique->getPositionSouris().y>sprite.GetPosition().y&&moteurGraphique->getPositionSouris().y<sprite.GetPosition().y+32&&m_decor[1][j][k][z].getNombreObjets()>4)
                            {
                                m_sacPointe.x=k;
                                m_sacPointe.y=j;
                            }

                            if (objetPointe>=0&&!eventManager->getEvenement(sf::Mouse::Left,"CA") && alt)
                            {
                                m_sacPointe.x=k;
                                m_sacPointe.y=j;

                                m_objetPointe=objetPointe;
                            }
                        }
                    }
                }
                /*else if (couche==0)
                {

                    int w=j,z=k;

                    if (j>=m_dimensions.y)
                        w=m_dimensions.y-1;
                    if (k>=m_dimensions.x)
                        z=m_dimensions.x-1;
                    if (j<0)
                        w=0;
                    if (k<0)
                        z=0;

                    sf::Sprite buffer=m_decor[couche][w][z].m_sprite;
                    buffer.SetX(position.x+64-48);
                    buffer.SetY(position.y+32-16);

                    if (buffer.GetSize().x>0)
                        if (buffer.GetPosition().x+buffer.GetSize().x-buffer.GetOrigin().x>=GetViewRect(moteurGraphique->m_camera).Left
                                &&buffer.GetPosition().x-buffer.GetOrigin().x<GetViewRect(moteurGraphique->m_camera).Right
                                &&buffer.GetPosition().y+buffer.GetSize().y-buffer.GetOrigin().y>=GetViewRect(moteurGraphique->m_camera).Top
                                &&buffer.GetPosition().y-buffer.GetOrigin().y<GetViewRect(moteurGraphique->m_camera).Bottom)
                        {
                            if (m_decor[couche][w][z].getTileset()>=0 && m_decor[couche][w][z].getTileset()<(int)m_tileset.size())
                                if (m_tileset[m_decor[couche][w][z].getTileset()].getTransparentDuTile(m_decor[couche][w][z].getTile()))
                                {
                                    int alpha=(int)((positionHero.y)-position.y)+160;

                                    if (alpha<configuration->alpha)
                                        alpha=configuration->alpha;
                                    if (alpha>255)
                                        alpha=255;

                                    buffer.SetColor(sf::Color(255,255,255,alpha));
                                }

                            moteurGraphique->AjouterCommande(&buffer,m_decor[couche][w][z].getCouche(),1);
                        }
                }*/


                /*if (alpha>0&&couche==1)
                {
                    position.x=(((k-(eventManager->m_coordonnee.x-15))-(j-(eventManager->m_coordonnee.y-15))-1+40)*6*configuration->Resolution.w/800);
                    position.y=(((k-(eventManager->m_coordonnee.x-15))+(j-(eventManager->m_coordonnee.y-15)))*6*configuration->Resolution.w/800);

                    if (position.x+465*configuration->Resolution.w/800>605*configuration->Resolution.w/800&&position.x+465*configuration->Resolution.w/800<800*configuration->Resolution.w/800&&position.y*configuration->Resolution.w/800>0&&position.y-80*configuration->Resolution.w/800<146*configuration->Resolution.w/600)
                        AfficherMinimap(position,getTypeCase(k,j),alpha);
                }*/
            }
        }
    }

     if(m_mode_brush)
    {
        m_select_brush.m_entite_graphique.m_sprite.Move(-64*(int)(m_taillePinceau * 0.5), -32*m_taillePinceau * 0.5);
        m_select_brush.m_entite_graphique.m_sprite.Move(+64*(int)(m_taillePinceau * 0.5), -32*m_taillePinceau * 0.5);

        for (unsigned o = 0 ; o < m_select_brush.getMonstre().size() ; ++o)
            if (m_select_brush.getMonstre()[o]>=0&&m_select_brush.getMonstre()[o]<(int)m_monstre.size())
                m_monstre[m_select_brush.getMonstre()[o]].setCoordonnee(coordonnee( m_monstre[m_select_brush.getMonstre()[o]].getCoordonnee().x - (int)(m_taillePinceau * 0.5),
                                                                                    m_monstre[m_select_brush.getMonstre()[o]].getCoordonnee().y - (int)(m_taillePinceau * 0.5)));

        for(int i = -(int)(m_taillePinceau * 0.5) ; i < m_taillePinceau * 0.5 ; ++i)
        {
            for(int j = -(int)(m_taillePinceau * 0.5) ; j < m_taillePinceau * 0.5 ; ++j)
            {
                moteurGraphique->AjouterCommande(&m_select_brush.m_entite_graphique.m_sprite,15,1);
                m_select_brush.m_entite_graphique.m_sprite.Move(64,32);

                for (unsigned o = 0 ; o < m_select_brush.getMonstre().size() ; ++o)
                    if (m_select_brush.getMonstre()[o]>=0&&m_select_brush.getMonstre()[o]<(int)m_monstre.size())
                        m_monstre[m_select_brush.getMonstre()[o]].Afficher(getDimensions(),&m_ModeleMonstre[m_monstre[m_select_brush.getMonstre()[o]].getModele()],m_select_brush.getMonstre()[o]==m_monstreIllumine);

                for (unsigned o = 0 ; o < m_select_brush.getMonstre().size() ; ++o)
                    if (m_select_brush.getMonstre()[o]>=0&&m_select_brush.getMonstre()[o]<(int)m_monstre.size())
                        m_monstre[m_select_brush.getMonstre()[o]].setCoordonnee(coordonnee( m_monstre[m_select_brush.getMonstre()[o]].getCoordonnee().x + 1,
                                                                                            m_monstre[m_select_brush.getMonstre()[o]].getCoordonnee().y));
            }

            m_select_brush.m_entite_graphique.m_sprite.Move(-64*m_taillePinceau, -32*m_taillePinceau);
            m_select_brush.m_entite_graphique.m_sprite.Move(-64, 32);

            for (unsigned o = 0 ; o < m_select_brush.getMonstre().size() ; ++o)
                if (m_select_brush.getMonstre()[o]>=0&&m_select_brush.getMonstre()[o]<(int)m_monstre.size())
                    m_monstre[m_select_brush.getMonstre()[o]].setCoordonnee(coordonnee( m_monstre[m_select_brush.getMonstre()[o]].getCoordonnee().x - m_taillePinceau,
                                                                                        m_monstre[m_select_brush.getMonstre()[o]].getCoordonnee().y + 1));
        }
        m_select_brush.m_entite_graphique.m_sprite.Move(+64*m_taillePinceau*0.5, -32*m_taillePinceau*0.5);

        for (unsigned o = 0 ; o < m_select_brush.getMonstre().size() ; ++o)
            if (m_select_brush.getMonstre()[o]>=0&&m_select_brush.getMonstre()[o]<(int)m_monstre.size())
                m_monstre[m_select_brush.getMonstre()[o]].setCoordonnee(coordonnee( m_monstre[m_select_brush.getMonstre()[o]].getCoordonnee().x - m_taillePinceau * 0.5,
                                                                                    m_monstre[m_select_brush.getMonstre()[o]].getCoordonnee().y - m_taillePinceau * 0.5));
    }
    else
    {
        m_selectSprite.Move(-64*(int)(m_taillePinceau * 0.5), -32*(int)(m_taillePinceau * 0.5));
        m_selectSprite.Move(+64*(int)(m_taillePinceau * 0.5), -32*(int)(m_taillePinceau * 0.5));

        for(int i = -(int)(m_taillePinceau * 0.5) ; i < m_taillePinceau * 0.5 ; ++i)
        {
            for(int j = -(int)(m_taillePinceau * 0.5) ; j < m_taillePinceau * 0.5 ; ++j)
            {
                moteurGraphique->AjouterCommande(&m_selectSprite,15,1);
                m_selectSprite.Move(64,32);
            }
            m_selectSprite.Move(-64*m_taillePinceau, -32*m_taillePinceau);
            m_selectSprite.Move(-64, 32);
        }
        m_selectSprite.Move(+64*m_taillePinceau, -32*m_taillePinceau);
    }
}

void Map::AfficherMinimap(coordonnee position,int typeCase,float alpha)
{
    Sprite spriteMinimap;
    spriteMinimap.SetOrigin(4*configuration->Resolution.w/800,4*configuration->Resolution.w/800);
    spriteMinimap.SetRotation(45);
    spriteMinimap.SetImage(*moteurGraphique->getImage(0));
    spriteMinimap.SetSubRect(sf::IntRect(0,0,8 * configuration->Resolution.w/800,8 * configuration->Resolution.w/800));
    spriteMinimap.SetX((float)(position.x+465.0f*configuration->Resolution.w/800));
    spriteMinimap.SetY((float)(position.y-80.0f*configuration->Resolution.w/800));

    if (typeCase==1)
        spriteMinimap.SetColor(sf::Color(128,64,0,(int)alpha));
    else if (typeCase==2)
        spriteMinimap.SetColor(sf::Color(128,0,0,(int)alpha));
    else if (typeCase==3)
        spriteMinimap.SetColor(sf::Color(0,128,0,(int)alpha));
    else if (typeCase==4)
        spriteMinimap.SetColor(sf::Color(255,255,64,(int)alpha));
    else if (typeCase==5)
        spriteMinimap.SetColor(sf::Color(160,0,160,(int)alpha));
    else if (typeCase==6)
        spriteMinimap.SetColor(sf::Color(32,0,128,(int)alpha));
    else
        spriteMinimap.SetColor(sf::Color(0,0,0,0));

    if (spriteMinimap.GetColor().a>0)
        moteurGraphique->AjouterCommande(&spriteMinimap,14,0);
}

int Map::AjouterProjectile(coordonneeDecimal positionReel,coordonnee cible,coordonnee lanceur,int couche,float  vitesse,float decalageAngle,bool monstre,std::vector<Tile> &tiles)
{
  /*  m_projectile.push_back(Projectile ());

    m_projectile.back().m_position.x=positionReel.x;
    m_projectile.back().m_position.y=positionReel.y;
    m_projectile.back().m_position.h=0;

    m_projectile.back().m_depart = lanceur;

    m_projectile.back().m_monstre=monstre;

    m_projectile.back().m_actif     = true;
    m_projectile.back().m_supprime  = false;

    m_projectile.back().m_effet.m_couche=couche;

    coordonneeDecimal position,position2;

    double m=atan2(cible.y-lanceur.y,cible.x-lanceur.x);

    m+=decalageAngle;

    position.x=cos(m)*vitesse/10;
    position.y=sin(m)*vitesse/10;

    m_projectile.back().m_vecteur=position;

    m_projectile.back().m_rotationReelle=m;

    position.x=(lanceur.x-lanceur.y);
    position.y=(lanceur.x+lanceur.y)/2;

    position2.x=(cible.x-cible.y);
    position2.y=(cible.x+cible.y)/2;

    m=atan2(position2.y-position.y,position2.x-position.x);

    m+=decalageAngle;

    m_projectile.back().m_rotation=m;

    m_projectile.back().m_positionCase=lanceur;

    m_projectile.back().m_effet.m_tiles = tiles;


    if (couche>=0&&couche<2)
        if (lanceur.y>=0&&lanceur.y<m_dimensions.y)
            if (lanceur.x>=0&&lanceur.x<m_dimensions.x)
            {
                m_projectile.back().m_positionCase.x=lanceur.x;
                m_projectile.back().m_positionCase.y=lanceur.y;
                if (m_decor[1][lanceur.y][lanceur.x].getProjectile()!=-1)
                    m_decor[0][lanceur.y][lanceur.x].setProjectile(m_projectile.size()-1),m_projectile.back().m_positionCase.h=1;
                else
                    m_decor[1][lanceur.y][lanceur.x].setProjectile(m_projectile.size()-1),m_projectile.back().m_positionCase.h=0;
            }

    return m_projectile.size()-1;*/
}

void Map::Animer(float temps/*,Menu *menu*/)
{
    /*coordonnee positionHero;
    positionHero.x=(hero->m_personnage.getCoordonnee().x-hero->m_personnage.getCoordonnee().y-1)/5;
    positionHero.y=(hero->m_personnage.getCoordonnee().x+hero->m_personnage.getCoordonnee().y)/5;*/

    coordonnee vueMin,vueMax;

    vueMin.x=eventManager->m_coordonnee.x-10*configuration->zoom;
    vueMin.y=eventManager->m_coordonnee.y-10*configuration->zoom;
    vueMax.x=eventManager->m_coordonnee.x+10*configuration->zoom;
    vueMax.y=eventManager->m_coordonnee.y+10*configuration->zoom;

    if (vueMin.x<0)
        vueMin.x=0;
    if (vueMin.y<0)
        vueMin.y=0;
    if (vueMax.x>m_dimensions.x)
        vueMax.x=m_dimensions.x;
    if (vueMax.y>m_dimensions.y)
        vueMax.y=m_dimensions.y;

    bool animer = false;
    m_aleatoireTileset += temps;
    if(m_aleatoireTileset >= 1)
        animer = true, m_aleatoireTileset = 0;


    for (int i=0;i<2;++i)
        for (int j=vueMin.y;j<vueMax.y;j++)
            for (int k=vueMin.x;k<vueMax.x;k++)
            {
               // m_decor[i][j][k].AugmenterAnimation(temps);
               for(unsigned z = 0 ; z < m_decor[i][j][k].size() ; ++z)
               {
                if (m_decor[i][j][k][z].getTileset()>=0&&m_decor[i][j][k][z].getTileset()<(int)m_tileset.size())
                {
                    if(animer)
                    {
                        m_decor[i][j][k][z].random_animation++;

                        if(m_decor[i][j][k][z].random_animation >= m_decor[i][j][k][z].getTile().size())
                            m_decor[i][j][k][z].random_animation = 0;

                    }
                }
                else
                    m_decor[i][j][k][z].m_entite_graphique.m_sprite.SetSubRect(sf::IntRect(0,0,0,0));
               }

               CreerSprite(sf::Vector3f(k,j,i));
            }

            m_selectSprite.SetScale(1,1);
            m_selectSprite.SetRotation(0);
            m_selectSprite.SetColor(sf::Color(255,255,255,255));

            if(m_selectTileset - 1 >= 0 && m_selectTileset - 1 < (int) m_tileset.size())
            {
                coordonnee position,positionPartieDecor;

                position.x=((int)eventManager->getCasePointee().x-(int)eventManager->getCasePointee().y-1)*64;
                position.y=((int)eventManager->getCasePointee().x+(int)eventManager->getCasePointee().y)*32;

                positionPartieDecor=moteurGraphique->getTileset(m_tileset[m_selectTileset - 1])->getPositionDuTile(m_selectTile);

                m_selectSprite.SetImage(*moteurGraphique->getImage(moteurGraphique->getTileset(m_tileset[m_selectTileset - 1])->getImage(m_selectTile)));
                m_selectSprite.SetSubRect(IntRect(positionPartieDecor.x, positionPartieDecor.y, positionPartieDecor.w, positionPartieDecor.h));

                m_selectSprite.SetOrigin(moteurGraphique->getTileset(m_tileset[m_selectTileset - 1])->getCentreDuTile(m_selectTile).x,
                                         moteurGraphique->getTileset(m_tileset[m_selectTileset - 1])->getCentreDuTile(m_selectTile).y);

                m_selectSprite.SetX(position.x+64);
                m_selectSprite.SetY(position.y+32);
            }

            if(m_selectEntite - 1 >= 0 && m_selectEntite - 1 < (int) m_ModeleMonstre.size())
            {
                coordonnee position;

                position.x=((int)eventManager->getCasePointee().x-(int)eventManager->getCasePointee().y)*64;
                position.y=((int)eventManager->getCasePointee().x+(int)eventManager->getCasePointee().y)*32;

                m_selectSprite.SetImage(*moteurGraphique->getImage(m_ModeleMonstre[m_selectEntite - 1].m_tileset[0][m_entite_angle].getImage(0)));

                m_selectSprite.SetOrigin(m_ModeleMonstre[m_selectEntite - 1].m_tileset[0][m_entite_angle].getCentreDuTile(0).x,m_ModeleMonstre[m_selectEntite - 1].m_tileset[0][m_entite_angle].getCentreDuTile(0).y);

                m_selectSprite.SetSubRect(IntRect(  m_ModeleMonstre[m_selectEntite - 1].m_tileset[0][m_entite_angle].getPositionDuTile(0).x,
                                                    m_ModeleMonstre[m_selectEntite - 1].m_tileset[0][m_entite_angle].getPositionDuTile(0).y,
                                                    m_ModeleMonstre[m_selectEntite - 1].m_tileset[0][m_entite_angle].getPositionDuTile(0).w,
                                                    m_ModeleMonstre[m_selectEntite - 1].m_tileset[0][m_entite_angle].getPositionDuTile(0).h));

                m_selectSprite.SetX(position.x);
                m_selectSprite.SetY(position.y+32);

                m_selectSprite.SetColor(sf::Color(m_ModeleMonstre[m_selectEntite - 1].getPorteeLumineuse().rouge,
                                                  m_ModeleMonstre[m_selectEntite - 1].getPorteeLumineuse().vert,
                                                  m_ModeleMonstre[m_selectEntite - 1].getPorteeLumineuse().bleu));

                m_selectSprite.SetScale(m_ModeleMonstre[m_selectEntite - 1].getCaracteristique().modificateurTaille,
                                        m_ModeleMonstre[m_selectEntite - 1].getCaracteristique().modificateurTaille);
            }

            if(m_selectEntite < 0 && m_selectTileset < 0 || m_mode_brush)
                m_selectSprite.SetSubRect(sf::IntRect(0,0,0,0));

            if(m_mode_brush)
            {
                if(m_select_brush.getTileset() >= 0 && m_select_brush.getTileset() < (int)m_tileset.size())
                {
                    coordonnee position,positionPartieDecor;
                    if(animer)
                    {
                        m_select_brush.random_animation++;

                        if(m_select_brush.random_animation >= m_select_brush.getTile().size())
                            m_select_brush.random_animation = 0;

                        positionPartieDecor=moteurGraphique->getTileset(m_tileset[m_select_brush.getTileset()])->getPositionDuTile(m_select_brush.getTile()[m_select_brush.random_animation]);

                        m_select_brush.m_entite_graphique.m_sprite.SetImage(*moteurGraphique->getImage(moteurGraphique->getTileset(m_tileset[m_select_brush.getTileset()])->getImage(m_select_brush.getTile()[m_select_brush.random_animation])));
                        m_select_brush.m_entite_graphique.m_sprite.SetSubRect(IntRect(positionPartieDecor.x, positionPartieDecor.y, positionPartieDecor.w, positionPartieDecor.h));

                        m_select_brush.m_entite_graphique.m_sprite.SetOrigin(moteurGraphique->getTileset(m_tileset[m_select_brush.getTileset()])->getCentreDuTile(m_select_brush.getTile()[m_select_brush.random_animation]).x,
                                                                                                                     moteurGraphique->getTileset(m_tileset[m_select_brush.getTileset()])->getCentreDuTile(m_select_brush.getTile()[m_select_brush.random_animation]).y);
                    }

                    position.x=((int)eventManager->getCasePointee().x-(int)eventManager->getCasePointee().y-1)*64;
                    position.y=((int)eventManager->getCasePointee().x+(int)eventManager->getCasePointee().y)*32;

                    m_select_brush.m_entite_graphique.m_sprite.SetX(position.x+64);
                    m_select_brush.m_entite_graphique.m_sprite.SetY(position.y+64);
                }

                for(int z = 0 ; z < m_select_brush.m_monstre.size() ; ++z)
                    if(m_select_brush.m_monstre[z] >= 0 && m_select_brush.m_monstre[z] < m_monstre.size())
                        m_monstre[m_select_brush.m_monstre[z]].setCoordonnee(coordonnee (eventManager->getCasePointee().x, eventManager->getCasePointee().y));
            }
}

/*void Map::GererMiracle(Personnage *personnage,std::vector<Miracle> &miracles ,float temps,coordonnee positionHero,Hero *hero)
{
    bool continuerb = true;
    for (int i=0;i<(int)personnage->m_miracleEnCours.size() && continuerb;++i)
    {
        bool continuer=true;
        for (int o=0;o<(int)personnage->m_miracleEnCours[i].m_infos.size() && continuer && continuerb;o++)
        {
            int effetEnCours =      personnage->m_miracleEnCours[i].m_infos[o].m_effetEnCours;
            int IDObjet =           personnage->m_miracleEnCours[i].m_infos[o].m_IDObjet;

            coordonnee cible;

            if (personnage->m_miracleEnCours[i].m_cible != NULL)
                cible = personnage->m_miracleEnCours[i].m_cible->getProchaineCase();
            else
                cible = personnage->m_miracleEnCours[i].m_coordonneeCible;



            if (effetEnCours >= 0)
            {
                if (continuer)
                    if (miracles[personnage->m_miracleEnCours[i].m_modele].m_effets[effetEnCours].m_type == AURA)
                    {
                        if (IDObjet == -1)
                        {
                            personnage->m_miracleEnCours[i].m_infos[o].m_IDObjet = personnage->AjouterEffet(
                                        miracles[personnage->m_miracleEnCours[i].m_modele].m_tile[miracles[personnage->m_miracleEnCours[i].m_modele].m_effets[effetEnCours].m_sequence],
                                        miracles[personnage->m_miracleEnCours[i].m_modele].m_effets[effetEnCours].m_informations[0],
                                        -100,
                                        miracles[personnage->m_miracleEnCours[i].m_modele].m_effets[effetEnCours].m_informations[1],
                                        miracles[personnage->m_miracleEnCours[i].m_modele].m_effets[effetEnCours].m_informations[2],
                                        miracles[personnage->m_miracleEnCours[i].m_modele].m_effets[effetEnCours].m_informations[3]);
                        }
                        else
                        {
                            personnage->m_miracleEnCours[i].m_infos[o].m_position.x = (float)personnage->getCoordonneePixel().x;
                            personnage->m_miracleEnCours[i].m_infos[o].m_position.y = (float)personnage->getCoordonneePixel().y;
                        }
                    }



                if (continuer)
                    if (miracles[personnage->m_miracleEnCours[i].m_modele].m_effets[effetEnCours].m_type == EFFET)
                    {
                        if (miracles[personnage->m_miracleEnCours[i].m_modele].m_effets[effetEnCours].m_informations[0])
                        {
                            personnage->AjouterEffet(
                                miracles[personnage->m_miracleEnCours[i].m_modele].m_tile[miracles[personnage->m_miracleEnCours[i].m_modele].m_effets[effetEnCours].m_sequence],
                                miracles[personnage->m_miracleEnCours[i].m_modele].m_effets[effetEnCours].m_informations[1], // TYPE
                                miracles[personnage->m_miracleEnCours[i].m_modele].m_effets[effetEnCours].m_informations[2], // TEMPS
                                miracles[personnage->m_miracleEnCours[i].m_modele].m_effets[effetEnCours].m_informations[3], // INFOS
                                miracles[personnage->m_miracleEnCours[i].m_modele].m_effets[effetEnCours].m_informations[4],
                                miracles[personnage->m_miracleEnCours[i].m_modele].m_effets[effetEnCours].m_informations[5]);
                        }
                        else if(personnage->m_miracleEnCours[i].m_cible != NULL)
                        {
                                personnage->m_miracleEnCours[i].m_cible->AjouterEffet(
                                miracles[personnage->m_miracleEnCours[i].m_modele].m_tile[miracles[personnage->m_miracleEnCours[i].m_modele].m_effets[effetEnCours].m_sequence],
                                miracles[personnage->m_miracleEnCours[i].m_modele].m_effets[effetEnCours].m_informations[1], // TYPE
                                miracles[personnage->m_miracleEnCours[i].m_modele].m_effets[effetEnCours].m_informations[2], // TEMPS
                                miracles[personnage->m_miracleEnCours[i].m_modele].m_effets[effetEnCours].m_informations[3], // INFOS
                                miracles[personnage->m_miracleEnCours[i].m_modele].m_effets[effetEnCours].m_informations[4],
                                miracles[personnage->m_miracleEnCours[i].m_modele].m_effets[effetEnCours].m_informations[5]);
                        }

                        for (int p=0;p<(int)miracles[personnage->m_miracleEnCours[i].m_modele].m_effets[effetEnCours].m_lien.size();p++)
                        {
                            personnage->m_miracleEnCours[i].m_infos.push_back(InfosEntiteMiracle ());
                            personnage->m_miracleEnCours[i].m_infos.back().m_effetEnCours=miracles[personnage->m_miracleEnCours[i].m_modele].m_effets[effetEnCours].m_lien[p];
                            personnage->m_miracleEnCours[i].m_infos.back().m_position=personnage->m_miracleEnCours[i].m_infos[o].m_position;
                        }

                        personnage->m_miracleEnCours[i].m_infos.erase(personnage->m_miracleEnCours[i].m_infos.begin()+o);

                        continuer=false;
                    }

                if (continuer)
                    if (miracles[personnage->m_miracleEnCours[i].m_modele].m_effets[effetEnCours].m_type == CHARME && IDObjet == -1)
                    {
                        if (personnage->m_miracleEnCours[i].m_cible->getCaracteristique().maxVie <= miracles[personnage->m_miracleEnCours[i].m_modele].m_effets[effetEnCours].m_informations[0]
                                &&personnage->m_miracleEnCours[i].m_cible->getCaracteristique().niveau > 0 && personnage->m_miracleEnCours[i].m_cible->getCaracteristique().vitesse > 0)
                        {
                            personnage->m_miracleEnCours[i].m_cible->m_scriptAI     = Script (miracles[personnage->m_miracleEnCours[i].m_modele].m_effets[effetEnCours].m_chaine);

                            personnage->m_miracleEnCours[i].m_cible->m_friendly     = true;

                            personnage->m_miracleEnCours[i].m_cible->m_cible        = NULL;
                            personnage->m_miracleEnCours[i].m_cible->frappeEnCours  = false;

                            personnage->m_miracleEnCours[i].m_infos[o].m_IDObjet    = 1;

                            if(personnage == &hero->m_personnage)
                                hero->m_amis.push_back(personnage->m_miracleEnCours[i].m_cible);

                            for (int p=0;p<(int)miracles[personnage->m_miracleEnCours[i].m_modele].m_effets[effetEnCours].m_lien.size();p++)
                            {
                                personnage->m_miracleEnCours[i].m_infos.push_back(InfosEntiteMiracle ());
                                personnage->m_miracleEnCours[i].m_infos.back().m_effetEnCours=miracles[personnage->m_miracleEnCours[i].m_modele].m_effets[effetEnCours].m_lien[p];
                                personnage->m_miracleEnCours[i].m_infos.back().m_position=personnage->m_miracleEnCours[i].m_infos[o].m_position;
                            }

                            continuer=false;
                        }
                        else
                        {
                            personnage->m_miracleEnCours[i].m_infos.erase(personnage->m_miracleEnCours[i].m_infos.begin()+o);

                            continuer=false;
                        }
                    }

                if (continuer)
                    if (miracles[personnage->m_miracleEnCours[i].m_modele].m_effets[effetEnCours].m_type == CHARME && IDObjet == 1)
                    {
                        Lumiere temp;
                        temp.rouge = 255;
                        temp.vert = 0;
                        temp.bleu = 255;
                        temp.intensite = 255;
                        personnage->m_miracleEnCours[i].m_cible->setPorteeLumineuse(temp);

                        if (!personnage->m_miracleEnCours[i].m_cible->EnVie())
                        {
                            if(personnage == &hero->m_personnage)
                                for(unsigned o = 0 ; o < hero->m_amis.size() ;  ++o)
                                    if(hero->m_amis[o] == personnage->m_miracleEnCours[i].m_cible)
                                        hero->m_amis.erase(hero->m_amis.begin() + o);

                            personnage->m_miracleEnCours[i].m_infos.erase(personnage->m_miracleEnCours[i].m_infos.begin()+o);

                            continuer=false;
                        }

                        if (fabs(personnage->m_miracleEnCours[i].m_cible->getCoordonnee().x - personnage->getCoordonnee().x) > 10 || fabs(personnage->m_miracleEnCours[i].m_cible->getCoordonnee().y - personnage->getCoordonnee().y) > 10)
                        {
                            personnage->m_miracleEnCours[i].m_cible->setCoordonnee(personnage->getCoordonnee());
                            personnage->m_miracleEnCours[i].m_cible->setDepart();
                        }

                        if (!personnage->m_miracleEnCours[i].m_cible->m_friendly)
                        {
                            personnage->m_miracleEnCours[i].m_cible->DetruireEffets();
                            personnage->m_miracleEnCours[i].m_cible->m_scriptAI = m_ModeleMonstre[personnage->m_miracleEnCours[i].m_cible->getModele()].m_scriptAI;
                            personnage->m_miracleEnCours[i].m_cible->setDepart();

                            personnage->m_miracleEnCours[i].m_infos.erase(personnage->m_miracleEnCours[i].m_infos.begin()+o);

                            continuer=false;
                        }
                    }

                if (continuer)
                    if (miracles[personnage->m_miracleEnCours[i].m_modele].m_effets[effetEnCours].m_type==CORPS_A_CORPS)
                    {
                        if (personnage->m_miracleEnCours[i].m_cible != NULL)
                        {
                            if (fabs(personnage->m_miracleEnCours[i].m_cible->getCoordonnee().x - personnage->getCoordonnee().x) > 1 || fabs(personnage->m_miracleEnCours[i].m_cible->getCoordonnee().y - personnage->getCoordonnee().y) > 1 )
                                personnage->setArrivee(personnage->m_miracleEnCours[i].m_cible->getCoordonnee());
                            else
                            {
                                personnage->setArrivee(personnage->getProchaineCase());
                                personnage->m_miracleEnCours[i].m_infos[o].m_position.x = (float)personnage->getProchaineCase().x * COTE_TILE + 1;
                                personnage->m_miracleEnCours[i].m_infos[o].m_position.y = (float)personnage->getProchaineCase().y * COTE_TILE + 1;

                                if (personnage->getArrivee().x==personnage->getCoordonnee().x&&personnage->getArrivee().y==personnage->getCoordonnee().y)
                                {
                                    personnage->Frappe(personnage->getCoordonneePixel(),personnage->m_miracleEnCours[i].m_cible->getCoordonneePixel());

                                    if (effetEnCours == 0)
                                    {
                                        Caracteristique temp = personnage->getCaracteristique();
                                        temp.foi        -= miracles[personnage->m_miracleEnCours[i].m_modele].m_coutFoi + miracles[personnage->m_miracleEnCours[i].m_modele].m_reserveFoi;
                                        temp.vie        -= miracles[personnage->m_miracleEnCours[i].m_modele].m_coutVie + miracles[personnage->m_miracleEnCours[i].m_modele].m_reserveVie;
                                        temp.reserveFoi += miracles[personnage->m_miracleEnCours[i].m_modele].m_reserveFoi;
                                        temp.reserveVie += miracles[personnage->m_miracleEnCours[i].m_modele].m_reserveVie;
                                        personnage->setCaracteristique(temp);

                                        personnage->m_miracleEnCours[i].m_dejaConsommeFoi = true;
                                    }

                                    for (int p=0;p<(int)miracles[personnage->m_miracleEnCours[i].m_modele].m_effets[effetEnCours].m_lien.size();p++)
                                    {
                                        personnage->m_miracleEnCours[i].m_infos.push_back(InfosEntiteMiracle ());
                                        personnage->m_miracleEnCours[i].m_infos.back().m_effetEnCours=miracles[personnage->m_miracleEnCours[i].m_modele].m_effets[effetEnCours].m_lien[p];
                                        personnage->m_miracleEnCours[i].m_infos.back().m_position=personnage->m_miracleEnCours[i].m_infos[o].m_position;
                                    }

                                    personnage->m_miracleEnCours[i].m_infos.erase(personnage->m_miracleEnCours[i].m_infos.begin()+o);

                                    continuer=false;
                                }
                            }
                        }
                        else
                            personnage->m_miracleEnCours[i].m_infos.erase(personnage->m_miracleEnCours[i].m_infos.begin()+o);
                    }

                if (continuer)
                    if (miracles[personnage->m_miracleEnCours[i].m_modele].m_effets[effetEnCours].m_type==REPETITION)
                    {
                        personnage->m_miracleEnCours[i].m_infos[o].m_informations[3] += temps * 100;
                        if (personnage->m_miracleEnCours[i].m_infos[o].m_informations[3] > miracles[personnage->m_miracleEnCours[i].m_modele].m_effets[effetEnCours].m_informations[2])
                        {
                            if (miracles[personnage->m_miracleEnCours[i].m_modele].m_effets[effetEnCours].m_informations[0] >= 0)
                            {
                                personnage->m_miracleEnCours[i].m_infos.push_back(InfosEntiteMiracle ());
                                personnage->m_miracleEnCours[i].m_infos.back().m_effetEnCours = miracles[personnage->m_miracleEnCours[i].m_modele].m_effets[effetEnCours].m_informations[0];
                                personnage->m_miracleEnCours[i].m_infos.back().m_position = personnage->m_miracleEnCours[i].m_infos[o].m_position;
                            }

                            personnage->m_miracleEnCours[i].m_infos[o].m_informations[1]--;
                            personnage->m_miracleEnCours[i].m_infos[o].m_informations[3] = 0;
                        }

                        if (-personnage->m_miracleEnCours[i].m_infos[o].m_informations[1] >= miracles[personnage->m_miracleEnCours[i].m_modele].m_effets[effetEnCours].m_informations[1])
                        {
                            for (int p=0;p<(int)miracles[personnage->m_miracleEnCours[i].m_modele].m_effets[effetEnCours].m_lien.size();p++)
                            {
                                personnage->m_miracleEnCours[i].m_infos.push_back(InfosEntiteMiracle ());
                                personnage->m_miracleEnCours[i].m_infos.back().m_effetEnCours=miracles[personnage->m_miracleEnCours[i].m_modele].m_effets[effetEnCours].m_lien[p];
                                personnage->m_miracleEnCours[i].m_infos.back().m_position=personnage->m_miracleEnCours[i].m_infos[o].m_position;
                            }

                            personnage->m_miracleEnCours[i].m_infos.erase(personnage->m_miracleEnCours[i].m_infos.begin()+o);

                            continuer=false;
                        }
                    }

                if (continuer)
                    if (miracles[personnage->m_miracleEnCours[i].m_modele].m_effets[effetEnCours].m_type==PROJECTILE)
                    {
                        if (IDObjet>=0 && IDObjet<(int)m_projectile.size())
                        {
                            if (!m_projectile[IDObjet].m_effet.m_actif)
                                m_projectile[IDObjet].m_actif = false;

                            if (m_projectile[IDObjet].m_actif)
                                m_projectile[IDObjet].m_effet.Animer(temps);
                            else
                            {
                                m_projectile[IDObjet].m_supprime = true;
                                personnage->m_miracleEnCours[i].m_cible = m_projectile[IDObjet].m_entite_cible;

                                for (int p=0;p<(int)miracles[personnage->m_miracleEnCours[i].m_modele].m_effets[effetEnCours].m_lien.size();p++)
                                {
                                    personnage->m_miracleEnCours[i].m_infos.push_back(InfosEntiteMiracle ());
                                    personnage->m_miracleEnCours[i].m_infos.back().m_effetEnCours=miracles[personnage->m_miracleEnCours[i].m_modele].m_effets[effetEnCours].m_lien[p];
                                    personnage->m_miracleEnCours[i].m_infos.back().m_position = m_projectile[IDObjet].m_position;
                                }

                                personnage->m_miracleEnCours[i].m_infos.erase(personnage->m_miracleEnCours[i].m_infos.begin()+o);

                                continuer=false;
                            }

                        }
                        else
                        {
                            coordonnee pos( (int)((float)personnage->m_miracleEnCours[i].m_infos[o].m_position.x/COTE_TILE),
                                            (int)((float)personnage->m_miracleEnCours[i].m_infos[o].m_position.y/COTE_TILE));

                            personnage->m_miracleEnCours[i].m_infos[o].m_IDObjet = AjouterProjectile(personnage->m_miracleEnCours[i].m_infos[o].m_position,
                                                                  cible,personnage->getCoordonnee(),1,miracles[personnage->m_miracleEnCours[i].m_modele].m_effets[personnage->m_miracleEnCours[i].m_infos[o].m_effetEnCours].m_informations[0],
                                                                  (float)miracles[personnage->m_miracleEnCours[i].m_modele].m_effets[personnage->m_miracleEnCours[i].m_infos[o].m_effetEnCours].m_informations[1]*M_PI/180,!personnage->m_friendly,
                                                                  miracles[personnage->m_miracleEnCours[i].m_modele].m_tile[miracles[personnage->m_miracleEnCours[i].m_modele].m_effets[personnage->m_miracleEnCours[i].m_infos[o].m_effetEnCours].m_sequence]);

                            if (miracles[personnage->m_miracleEnCours[i].m_modele].m_effets[personnage->m_miracleEnCours[i].m_infos[o].m_effetEnCours].m_informations[1])
                                m_projectile.back().m_cible = cible;
                            else
                                m_projectile.back().m_cible = coordonnee (-1, -1);
                        }
                    }

                if (continuer)
                    if (miracles[personnage->m_miracleEnCours[i].m_modele].m_effets[effetEnCours].m_type == EFFET_GRAPHIQUE)
                    {
                        if (IDObjet>=0&&IDObjet<(int)m_effets.size())
                        {
                            m_effets[IDObjet].Animer(temps);
                            if (!m_effets[IDObjet].m_actif)
                            {
                                for (int p=0;p<(int)miracles[personnage->m_miracleEnCours[i].m_modele].m_effets[effetEnCours].m_lien.size();p++)
                                {
                                    personnage->m_miracleEnCours[i].m_infos.push_back(InfosEntiteMiracle ());
                                    personnage->m_miracleEnCours[i].m_infos.back().m_effetEnCours=miracles[personnage->m_miracleEnCours[i].m_modele].m_effets[effetEnCours].m_lien[p];
                                    personnage->m_miracleEnCours[i].m_infos.back().m_position = personnage->m_miracleEnCours[i].m_infos[o].m_position;
                                }

                                personnage->m_miracleEnCours[i].m_infos.erase(personnage->m_miracleEnCours[i].m_infos.begin()+o);

                                continuer=false;
                            }
                        }
                        else
                        {
                            m_effets.push_back(EffetGraphique ());

                            m_effets.back().m_position.x = personnage->m_miracleEnCours[i].m_infos[o].m_position.x;
                            m_effets.back().m_position.y = personnage->m_miracleEnCours[i].m_infos[o].m_position.y;

                            m_effets.back().m_compteur=miracles[personnage->m_miracleEnCours[i].m_modele].m_effets[personnage->m_miracleEnCours[i].m_infos[o].m_effetEnCours].m_informations[0];

                            m_effets.back().m_actif=true;

                            m_effets.back().m_couche=1;

                            m_effets.back().m_tiles = miracles[personnage->m_miracleEnCours[i].m_modele].m_tile[miracles[personnage->m_miracleEnCours[i].m_modele].m_effets[personnage->m_miracleEnCours[i].m_infos[o].m_effetEnCours].m_sequence];

                            personnage->m_miracleEnCours[i].m_infos[o].m_IDObjet=m_effets.size()-1;

                            if (m_effets.back().m_position.y / COTE_TILE>=0&&m_effets.back().m_position.y / COTE_TILE<m_dimensions.y)
                                if (m_effets.back().m_position.x / COTE_TILE>=0&&m_effets.back().m_position.x / COTE_TILE<m_dimensions.x)
                                    m_decor[1][(int)(m_effets.back().m_position.y / COTE_TILE)][(int)(m_effets.back().m_position.x / COTE_TILE)].setEffetGraphique(m_effets.size()-1);
                        }
                    }

                if (continuer)
                    if (miracles[personnage->m_miracleEnCours[i].m_modele].m_effets[personnage->m_miracleEnCours[i].m_infos[o].m_effetEnCours].m_type==INVOCATION)
                    {
                        if (personnage->m_miracleEnCours[i].m_infos[o].m_IDObjet==-1)
                        {
                            bool invoquer=true;
                            int x=0,y=0;
                            coordonnee positionCase;
                            positionCase.x=cible.x-1;
                            positionCase.y=cible.y-1;

                            if (positionCase.x<0)
                                positionCase.x=0;
                            if (positionCase.y<0)
                                positionCase.y=0;

                            while ((getCollision(positionCase.x,positionCase.y) || ( positionCase.x==hero->m_personnage.getCoordonnee().x && positionCase.y==hero->m_personnage.getCoordonnee().y )) && invoquer )
                            {
                                positionCase.x++;
                                x++;
                                if (x>=3)
                                    x=0,positionCase.x-=3,y++,positionCase.y++;
                                if (y>=3)
                                    invoquer=false;
                            }

                            if (invoquer)
                            {
                                personnage->m_miracleEnCours[i].m_infos[o].m_position.x=positionCase.x*COTE_TILE;
                                personnage->m_miracleEnCours[i].m_infos[o].m_position.y=positionCase.y*COTE_TILE;

                                for (unsigned p=0;p < miracles[personnage->m_miracleEnCours[i].m_modele].m_effets[personnage->m_miracleEnCours[i].m_infos[o].m_effetEnCours].m_lien.size();p++)
                                {
                                    personnage->m_miracleEnCours[i].m_infos.push_back(InfosEntiteMiracle ());
                                    personnage->m_miracleEnCours[i].m_infos.back().m_effetEnCours=miracles[personnage->m_miracleEnCours[i].m_modele].m_effets[personnage->m_miracleEnCours[i].m_infos[o].m_effetEnCours].m_lien[p];
                                    personnage->m_miracleEnCours[i].m_infos.back().m_position=personnage->m_miracleEnCours[i].m_infos[o].m_position;
                                }

                                int numero=-1;
                                for (int j=0;j<(int)m_ModeleMonstre.size();j++)
                                    if (miracles[personnage->m_miracleEnCours[i].m_modele].m_effets[personnage->m_miracleEnCours[i].m_infos[o].m_effetEnCours].m_chaine==m_ModeleMonstre[j].m_chemin)
                                    {
                                        numero=j;
                                        break;
                                    }

                                if (numero==-1)
                                {
                                    m_ModeleMonstre.push_back(Modele_Monstre ());
                                    m_ModeleMonstre.back().Charger(miracles[personnage->m_miracleEnCours[i].m_modele].m_effets[personnage->m_miracleEnCours[i].m_infos[o].m_effetEnCours].m_chaine);
                                    console->Ajouter("Chargement de : "+miracles[personnage->m_miracleEnCours[i].m_modele].m_effets[personnage->m_miracleEnCours[i].m_infos[o].m_effetEnCours].m_chaine+" terminé",0);
                                    numero=m_ModeleMonstre.size()-1;
                                }

                                m_monstre.push_back(Monstre ());

                                m_monstre.back().Charger(numero,&m_ModeleMonstre[numero]);
                                m_monstre.back().setCoordonnee(positionCase),m_monstre.back().setDepart();

                                m_monstre.back().m_light=moteurGraphique->LightManager->Add_Dynamic_Light();

                                moteurGraphique->LightManager->SetQuality(m_monstre.back().m_light,6);

                                sf::Vector2f pos;
                                pos.x=(((m_monstre.back().getCoordonneePixel().x-m_monstre.back().getCoordonneePixel().y)*64/COTE_TILE*64));
                                pos.y=(((m_monstre.back().getCoordonneePixel().x+m_monstre.back().getCoordonneePixel().y)*64/COTE_TILE)/2+32)*2;

                                moteurGraphique->LightManager->SetPosition(m_monstre.back().m_light,pos);

                                moteurGraphique->LightManager->SetColor(m_monstre.back().m_light,sf::Color(m_monstre.back().getPorteeLumineuse().rouge,m_monstre.back().getPorteeLumineuse().vert,m_monstre.back().getPorteeLumineuse().bleu));

                                m_decor[1][positionCase.y][positionCase.x].setMonstre(m_monstre.size()-1);

                                personnage->m_miracleEnCours[i].m_infos[o].m_IDObjet=m_monstre.size()-1;
                            }

                            continuer=false;
                        }
                        else if (personnage->m_miracleEnCours[i].m_infos[o].m_IDObjet>=0&&personnage->m_miracleEnCours[i].m_infos[o].m_IDObjet<(int)m_monstre.size())
                        {
                            personnage->m_nombreInvocation++;
                            if (!m_monstre[personnage->m_miracleEnCours[i].m_infos[o].m_IDObjet].EnVie())
                            {
                                personnage->m_miracleEnCours[i].m_infos.erase(personnage->m_miracleEnCours[i].m_infos.begin()+o);
                                continuer=false;
                            }
                        }
                        else
                        {
                            personnage->m_miracleEnCours[i].m_infos.erase(personnage->m_miracleEnCours[i].m_infos.begin()+o);
                            continuer=false;
                        }
                    }

                if (continuer)
                    if (miracles[personnage->m_miracleEnCours[i].m_modele].m_effets[effetEnCours].m_type == CHANGEMENT_POSE)
                    {
                        if (miracles[personnage->m_miracleEnCours[i].m_modele].m_effets[effetEnCours].m_informations[0] != -1)
                            personnage->setJustEtat(miracles[personnage->m_miracleEnCours[i].m_modele].m_effets[effetEnCours].m_informations[0]);

                        if (miracles[personnage->m_miracleEnCours[i].m_modele].m_effets[effetEnCours].m_informations[1] != -1)
                            personnage->setPose(miracles[personnage->m_miracleEnCours[i].m_modele].m_effets[effetEnCours].m_informations[1]);

                        personnage->addAngle(miracles[personnage->m_miracleEnCours[i].m_modele].m_effets[effetEnCours].m_informations[2]*45);

                        for (int p=0;p<(int)miracles[personnage->m_miracleEnCours[i].m_modele].m_effets[effetEnCours].m_lien.size();p++)
                        {
                            personnage->m_miracleEnCours[i].m_infos.push_back(InfosEntiteMiracle ());
                            personnage->m_miracleEnCours[i].m_infos.back().m_effetEnCours=miracles[personnage->m_miracleEnCours[i].m_modele].m_effets[effetEnCours].m_lien[p];
                            personnage->m_miracleEnCours[i].m_infos.back().m_position = personnage->m_miracleEnCours[i].m_infos[o].m_position;
                        }

                        personnage->m_miracleEnCours[i].m_infos.erase(personnage->m_miracleEnCours[i].m_infos.begin()+o);

                        continuer=false;
                    }


                if (continuer)
                    if (miracles[personnage->m_miracleEnCours[i].m_modele].m_effets[effetEnCours].m_type == CHARGE)
                    {

                        personnage->m_miracleEnCours[i].m_infos[o].m_informations[3] += temps * 100;
                        if (personnage->m_miracleEnCours[i].m_infos[o].m_informations[3] > miracles[personnage->m_miracleEnCours[i].m_modele].m_effets[effetEnCours].m_informations[2])
                        {
                            if (miracles[personnage->m_miracleEnCours[i].m_modele].m_effets[effetEnCours].m_informations[1] >= 0)
                            {
                                personnage->m_miracleEnCours[i].m_infos.push_back(InfosEntiteMiracle ());
                                personnage->m_miracleEnCours[i].m_infos.back().m_effetEnCours = miracles[personnage->m_miracleEnCours[i].m_modele].m_effets[effetEnCours].m_informations[1];
                                personnage->m_miracleEnCours[i].m_infos.back().m_position = personnage->m_miracleEnCours[i].m_infos[o].m_position;
                            }

                            personnage->m_miracleEnCours[i].m_infos[o].m_informations[3] = 0;
                        }

                        personnage->m_miracleEnCours[i].m_infos[o].m_position.x = (float)personnage->getCoordonneePixel().x;
                        personnage->m_miracleEnCours[i].m_infos[o].m_position.y = (float)personnage->getCoordonneePixel().y;

                        if (IDObjet == 1)
                        {
                            if (((personnage->m_miracleEnCours[i].m_coordonneeDepart.x  < personnage->m_miracleEnCours[i].m_coordonneeCible.x&&personnage->getCoordonneePixel().x>=personnage->m_miracleEnCours[i].m_coordonneeCible.x*COTE_TILE)
                               ||(personnage->m_miracleEnCours[i].m_coordonneeDepart.x  > personnage->m_miracleEnCours[i].m_coordonneeCible.x&&personnage->getCoordonneePixel().x<=personnage->m_miracleEnCours[i].m_coordonneeCible.x*COTE_TILE)
                                ||personnage->m_miracleEnCours[i].m_coordonneeDepart.x  ==personnage->m_miracleEnCours[i].m_coordonneeCible.x)

                              ||(personnage->m_miracleEnCours[i].m_coordonneeDepart.y   > personnage->m_miracleEnCours[i].m_coordonneeCible.y&&personnage->getCoordonneePixel().y<=personnage->m_miracleEnCours[i].m_coordonneeCible.y*COTE_TILE)
                            && ((personnage->m_miracleEnCours[i].m_coordonneeDepart.y   < personnage->m_miracleEnCours[i].m_coordonneeCible.y&&personnage->getCoordonneePixel().y>=personnage->m_miracleEnCours[i].m_coordonneeCible.y*COTE_TILE)
                               ||personnage->m_miracleEnCours[i].m_coordonneeDepart.y   ==personnage->m_miracleEnCours[i].m_coordonneeCible.y)

                                    ||(personnage->getPousse().x == 0 && personnage->getPousse().y == 0))
                            {

                                if(personnage->getCoordonnee().x == personnage->m_miracleEnCours[i].m_coordonneeCible.x
                                && personnage->getCoordonnee().y == personnage->m_miracleEnCours[i].m_coordonneeCible.y )
                                    personnage->setCoordonnee(coordonnee (personnage->m_miracleEnCours[i].m_coordonneeCible.x,
                                                                          personnage->m_miracleEnCours[i].m_coordonneeCible.y));

                                personnage->setPousse(coordonneeDecimal  (0,0));

                                personnage->m_miracleEnCours[i].m_infos[o].m_position.x = (float)personnage->getCoordonneePixel().x;
                                personnage->m_miracleEnCours[i].m_infos[o].m_position.y = (float)personnage->getCoordonneePixel().y;

                                for (int p=0;p<(int)miracles[personnage->m_miracleEnCours[i].m_modele].m_effets[effetEnCours].m_lien.size();p++)
                                {
                                    personnage->m_miracleEnCours[i].m_infos.push_back(InfosEntiteMiracle ());
                                    personnage->m_miracleEnCours[i].m_infos.back().m_effetEnCours=miracles[personnage->m_miracleEnCours[i].m_modele].m_effets[effetEnCours].m_lien[p];
                                    personnage->m_miracleEnCours[i].m_infos.back().m_position = personnage->m_miracleEnCours[i].m_infos[o].m_position;
                                }

                                personnage->m_miracleEnCours[i].m_infos.erase(personnage->m_miracleEnCours[i].m_infos.begin()+o);

                                continuer=false;
                            }
                        }

                        if ( continuer && !(personnage->getCoordonnee().x == personnage->m_miracleEnCours[i].m_coordonneeCible.x && personnage->getCoordonnee().y == personnage->m_miracleEnCours[i].m_coordonneeCible.y) )
                        {
                            float m = atan2((float)personnage->m_miracleEnCours[i].m_coordonneeCible.x * COTE_TILE - (float)personnage->getCoordonneePixel().x,
                                            (float)personnage->m_miracleEnCours[i].m_coordonneeCible.y * COTE_TILE - (float)personnage->getCoordonneePixel().y);

                            personnage->setPousse(coordonneeDecimal  (sin(m) * miracles[personnage->m_miracleEnCours[i].m_modele].m_effets[effetEnCours].m_informations[0],
                                                                      cos(m) * miracles[personnage->m_miracleEnCours[i].m_modele].m_effets[effetEnCours].m_informations[0]));
                        }

                        if(continuer)
                            personnage->m_miracleEnCours[i].m_infos[o].m_IDObjet= 1;
                    }

                if (continuer)
                    if (miracles[personnage->m_miracleEnCours[i].m_modele].m_effets[personnage->m_miracleEnCours[i].m_infos[o].m_effetEnCours].m_type == EFFET_ECRAN)
                    {
                        moteurGraphique->m_effetsEcran.push_back(Effet_ecran ());
                        moteurGraphique->m_effetsEcran.back().type      = miracles[personnage->m_miracleEnCours[i].m_modele].m_effets[personnage->m_miracleEnCours[i].m_infos[o].m_effetEnCours].m_informations[0];
                        moteurGraphique->m_effetsEcran.back().info1     = miracles[personnage->m_miracleEnCours[i].m_modele].m_effets[personnage->m_miracleEnCours[i].m_infos[o].m_effetEnCours].m_informations[1];


                        for (unsigned p=0;p<miracles[personnage->m_miracleEnCours[i].m_modele].m_effets[personnage->m_miracleEnCours[i].m_infos[o].m_effetEnCours].m_lien.size();p++)
                        {
                            personnage->m_miracleEnCours[i].m_infos.push_back(InfosEntiteMiracle ());
                            personnage->m_miracleEnCours[i].m_infos.back().m_effetEnCours=miracles[personnage->m_miracleEnCours[i].m_modele].m_effets[personnage->m_miracleEnCours[i].m_infos[o].m_effetEnCours].m_lien[p];
                            personnage->m_miracleEnCours[i].m_infos.back().m_position=personnage->m_miracleEnCours[i].m_infos[o].m_position;
                        }

                        continuer=false;
                        personnage->m_miracleEnCours[i].m_infos.erase(personnage->m_miracleEnCours[i].m_infos.begin()+o);
                    }


                if (continuer)
                    if (miracles[personnage->m_miracleEnCours[i].m_modele].m_effets[personnage->m_miracleEnCours[i].m_infos[o].m_effetEnCours].m_type==DEGATS)
                    {
                        int deg = rand() % (int)(miracles[personnage->m_miracleEnCours[i].m_modele].m_effets[personnage->m_miracleEnCours[i].m_infos[o].m_effetEnCours].m_informations[1] - miracles[personnage->m_miracleEnCours[i].m_modele].m_effets[personnage->m_miracleEnCours[i].m_infos[o].m_effetEnCours].m_informations[0] + 1) + miracles[personnage->m_miracleEnCours[i].m_modele].m_effets[personnage->m_miracleEnCours[i].m_infos[o].m_effetEnCours].m_informations[0];
                        if (personnage->m_miracleEnCours[i].m_cible != NULL)
                        {
                            if (personnage->m_miracleEnCours[i].m_cible->getCoordonnee().y >=0 && personnage->m_miracleEnCours[i].m_cible->getCoordonnee().y < (int)m_decor[0].size())
                                if (personnage->m_miracleEnCours[i].m_cible->getCoordonnee().x >=0 && personnage->m_miracleEnCours[i].m_cible->getCoordonnee().x < (int)m_decor[0][personnage->m_miracleEnCours[i].m_cible->getCoordonnee().y].size())
                                    InfligerDegats(personnage->m_miracleEnCours[i].m_cible, deg, hero, 0);
                        }

                        for (unsigned p=0;p<miracles[personnage->m_miracleEnCours[i].m_modele].m_effets[personnage->m_miracleEnCours[i].m_infos[o].m_effetEnCours].m_lien.size();p++)
                        {
                            personnage->m_miracleEnCours[i].m_infos.push_back(InfosEntiteMiracle ());
                            personnage->m_miracleEnCours[i].m_infos.back().m_effetEnCours=miracles[personnage->m_miracleEnCours[i].m_modele].m_effets[personnage->m_miracleEnCours[i].m_infos[o].m_effetEnCours].m_lien[p];
                            personnage->m_miracleEnCours[i].m_infos.back().m_position=personnage->m_miracleEnCours[i].m_infos[o].m_position;
                        }

                        personnage->m_miracleEnCours[i].m_infos.erase(personnage->m_miracleEnCours[i].m_infos.begin()+o);
                        continuer=false;
                    }



                if (continuer)
                    if (miracles[personnage->m_miracleEnCours[i].m_modele].m_effets[personnage->m_miracleEnCours[i].m_infos[o].m_effetEnCours].m_type==M_EXPLOSION)
                    {
                        int deg = rand() % (int)(miracles[personnage->m_miracleEnCours[i].m_modele].m_effets[personnage->m_miracleEnCours[i].m_infos[o].m_effetEnCours].m_informations[1] - miracles[personnage->m_miracleEnCours[i].m_modele].m_effets[personnage->m_miracleEnCours[i].m_infos[o].m_effetEnCours].m_informations[0] + 1) + miracles[personnage->m_miracleEnCours[i].m_modele].m_effets[personnage->m_miracleEnCours[i].m_infos[o].m_effetEnCours].m_informations[0];
                        coordonnee buf((int)(personnage->m_miracleEnCours[i].m_infos[o].m_position.x), (int)(personnage->m_miracleEnCours[i].m_infos[o].m_position.y));
                        InfligerDegatsMasse(buf, miracles[personnage->m_miracleEnCours[i].m_modele].m_effets[personnage->m_miracleEnCours[i].m_infos[o].m_effetEnCours].m_informations[3],
                                            deg, 1, hero, miracles[personnage->m_miracleEnCours[i].m_modele].m_effets[personnage->m_miracleEnCours[i].m_infos[o].m_effetEnCours].m_informations[2],
                                            miracles[personnage->m_miracleEnCours[i].m_modele].m_effets[personnage->m_miracleEnCours[i].m_infos[o].m_effetEnCours].m_informations[5] , !personnage->m_friendly );

                        if (miracles[personnage->m_miracleEnCours[i].m_modele].m_effets[personnage->m_miracleEnCours[i].m_infos[o].m_effetEnCours].m_informations[4])
                            VerifierDeclencheursDegats((buf.y+COTE_TILE*0.5)/COTE_TILE,(buf.x+COTE_TILE*0.5)/COTE_TILE);

                        for (unsigned p=0;p<miracles[personnage->m_miracleEnCours[i].m_modele].m_effets[personnage->m_miracleEnCours[i].m_infos[o].m_effetEnCours].m_lien.size();p++)
                        {
                            personnage->m_miracleEnCours[i].m_infos.push_back(InfosEntiteMiracle ());
                            personnage->m_miracleEnCours[i].m_infos.back().m_effetEnCours=miracles[personnage->m_miracleEnCours[i].m_modele].m_effets[personnage->m_miracleEnCours[i].m_infos[o].m_effetEnCours].m_lien[p];
                            personnage->m_miracleEnCours[i].m_infos.back().m_position=personnage->m_miracleEnCours[i].m_infos[o].m_position;
                        }

                        personnage->m_miracleEnCours[i].m_infos.erase(personnage->m_miracleEnCours[i].m_infos.begin()+o);
                        continuer=false;
                    }

                if (continuer)
                    if (miracles[personnage->m_miracleEnCours[i].m_modele].m_effets[personnage->m_miracleEnCours[i].m_infos[o].m_effetEnCours].m_type==-1)
                    {
                        for (unsigned p=0;p<miracles[personnage->m_miracleEnCours[i].m_modele].m_effets[personnage->m_miracleEnCours[i].m_infos[o].m_effetEnCours].m_lien.size();p++)
                        {
                            personnage->m_miracleEnCours[i].m_infos.push_back(InfosEntiteMiracle ());
                            personnage->m_miracleEnCours[i].m_infos.back().m_effetEnCours=miracles[personnage->m_miracleEnCours[i].m_modele].m_effets[personnage->m_miracleEnCours[i].m_infos[o].m_effetEnCours].m_lien[p];
                            personnage->m_miracleEnCours[i].m_infos.back().m_position=personnage->m_miracleEnCours[i].m_infos[o].m_position;
                        }

                        personnage->m_miracleEnCours[i].m_infos.erase(personnage->m_miracleEnCours[i].m_infos.begin()+o);
                        continuer=false;
                    }


                if (!continuer)
                    o --, continuer = true;
            }
            else
                personnage->m_miracleEnCours[i].m_infos.erase(personnage->m_miracleEnCours[i].m_infos.begin()+o);
        }

        if (personnage->m_miracleEnCours[i].m_infos.empty())
        {
            if (personnage->m_miracleEnCours[i].m_dejaConsommeFoi)
            {
                Caracteristique temp = personnage->getCaracteristique();
                temp.foi        += miracles[personnage->m_miracleEnCours[i].m_modele].m_reserveFoi;
                temp.vie        += miracles[personnage->m_miracleEnCours[i].m_modele].m_reserveVie;
                temp.reserveFoi -= miracles[personnage->m_miracleEnCours[i].m_modele].m_reserveFoi;
                temp.reserveVie -= miracles[personnage->m_miracleEnCours[i].m_modele].m_reserveVie;
                personnage->setCaracteristique(temp);
            }

            personnage->m_miracleEnCours.erase (personnage->m_miracleEnCours.begin()+i);
            continuerb = false;
        }
    }
}*/

/*void Map::GererEvenements(int evenement,int z,int couche,int x,int y)
{
    if (evenement>=0&&evenement<(int)m_evenement.size())
    {
        if (m_evenement[m_decor[couche][y][x].getEvenement()[z]].getType()==CHANGER_DECOR)
        {
            m_decor[couche][y][x].setTileset(m_evenement[evenement].getInformation(0));
            m_decor[couche][y][x].setTile(m_evenement[evenement].getInformation(1));

            if (m_decor[couche][y][x].getTileset()>=0&&m_decor[couche][y][x].getTileset()<(int)m_tileset.size())
            {
                sf::Vector2f pos;
                pos.x=(((x*COTE_TILE-y*COTE_TILE)*64/COTE_TILE));
                pos.y=(((x*COTE_TILE+y*COTE_TILE)*64/COTE_TILE)/2+32)*2;
                if (m_tileset[m_decor[couche][y][x].getTileset()].getLumiereDuTile(m_decor[couche][y][x].getTile()).intensite>0)
                {
                    m_decor[couche][y][x].m_light=moteurGraphique->LightManager->Add_Dynamic_Light(pos,m_tileset[m_decor[couche][y][x].getTileset()].getLumiereDuTile(m_decor[couche][y][x].getTile()).intensite,m_tileset[m_decor[couche][y][x].getTileset()].getLumiereDuTile(m_decor[couche][y][x].getTile()).intensite*3,8,sf::Color(m_tileset[m_decor[couche][y][x].getTileset()].getLumiereDuTile(m_decor[couche][y][x].getTile()).rouge,m_tileset[m_decor[couche][y][x].getTileset()].getLumiereDuTile(m_decor[couche][y][x].getTile()).vert,m_tileset[m_decor[couche][y][x].getTileset()].getLumiereDuTile(m_decor[couche][y][x].getTile()).bleu));

                    moteurGraphique->LightManager->Generate(m_decor[couche][y][x].m_light);
                }
            }
        }
        if (m_evenement[evenement].getType()==EXPLOSION)
        {
            m_decor[couche][y][x].setEvenement(-1,z);
            VerifierDeclencheursDegats(y,x);
        }
    }
}

void Map::InfligerDegatsMasse(coordonnee position,int rayon,int degats,bool sourceConcernee, Hero *hero, bool pousser, bool amisCompris, bool monstre)
{
    for (int y=(int)(((float)position.y+COTE_TILE*0.5)/COTE_TILE-rayon);y<=(int)(((float)position.y+COTE_TILE*0.5)/COTE_TILE+rayon);y++)
        for (int x=(int)(((float)position.x+COTE_TILE*0.5)/COTE_TILE-rayon);x<=(int)(((float)position.x+COTE_TILE*0.5)/COTE_TILE+rayon);x++)
            if (y>=0&&x>=0&&y<m_dimensions.y&&x<m_dimensions.x)
                if (sourceConcernee||!sourceConcernee&&!(y==position.y&&x==position.x))
                {
                    for (unsigned o = 0 ; o < m_decor[1][y][x].getMonstre().size() ; ++o)
                        if (m_decor[1][y][x].getMonstre()[o]>=0&&m_decor[1][y][x].getMonstre()[o]<(int)m_monstre.size())
                            if (m_monstre[m_decor[1][y][x].getMonstre()[o]].EnVie()
                            && ((m_monstre[m_decor[1][y][x].getMonstre()[o]].m_friendly == monstre) || (m_monstre[m_decor[1][y][x].getMonstre()[o]].m_friendly != monstre && amisCompris))
                            &&m_monstre[m_decor[1][y][x].getMonstre()[o]].getCaracteristique().rang>=0
                            &&m_monstre[m_decor[1][y][x].getMonstre()[o]].getCaracteristique().niveau>=0)
                            {
                                InfligerDegats(m_decor[1][y][x].getMonstre()[o], degats,hero,0);

                                if (pousser)
                                {
                                    coordonneeDecimal vecteur;

                                    if (position.x-m_monstre[m_decor[1][y][x].getMonstre()[o]].getCoordonneePixel().x<0)
                                        vecteur.x=1;
                                    else if (position.x-m_monstre[m_decor[1][y][x].getMonstre()[o]].getCoordonneePixel().x>0)
                                        vecteur.x=-1;
                                    else
                                        vecteur.x=0;

                                    if (position.y-m_monstre[m_decor[1][y][x].getMonstre()[o]].getCoordonneePixel().y<0)
                                        vecteur.y=1;
                                    else if (position.y-m_monstre[m_decor[1][y][x].getMonstre()[o]].getCoordonneePixel().y>0)
                                        vecteur.y=-1;
                                    else
                                        vecteur.y=0;

                                    if(vecteur.x == 0 && vecteur.y == 0)
                                        vecteur.x = 1, vecteur.y = 1;

                                    PousserMonstre(m_decor[1][y][x].getMonstre()[o],vecteur);
                                }
                            }

                    if (y==hero->m_personnage.getCoordonnee().y&&x==hero->m_personnage.getCoordonnee().x && (monstre || amisCompris))
                        hero->m_personnage.InfligerDegats(degats);
                }
}

void Map::VerifierDeclencheursDegats(int i, int j)
{
    for (int o=0;o<2;o++)
        for (int y=i-1;y<=i+1;y++)
            for (int x=j-1;x<=j+1;x++)
                if (y>0&&x>0&&y<m_dimensions.y&&x<m_dimensions.x)
                {
                    for (int z=0;z<(int)m_decor[o][y][x].getEvenement().size();z++)
                    {
                        if (m_decor[o][y][x].getEvenement()[z]>=0&&m_decor[o][y][x].getEvenement()[z]<(int)m_evenement.size())
                            if (m_evenement[m_decor[o][y][x].getEvenement()[z]].getType()==DECLENCHEUR_DEGAT_TO_EVENEMENT)
                            {
                                m_decor[o][y][x].setEvenement(m_evenement[m_decor[o][y][x].getEvenement()[z]].getInformation(0),z);
                                GererEvenements(m_decor[o][y][x].getEvenement()[z],z,o,x,y);
                            }
                    }
                }
}*/

void Map::MusiquePlay(coordonnee position)
{
    if (m_nombreMusique>0)
        if (m_musiqueEnCours>=0&&m_musiqueEnCours<MAX_MUSIQUE)
        {
            Sound::Status Status = m_musique[m_musiqueEnCours].GetStatus();

            if (Status==0)
            {
                m_musiqueEnCours++;
                if (m_musiqueEnCours>=m_nombreMusique)
                    m_musiqueEnCours=0;

                if (m_musiqueEnCours>=0&&m_musiqueEnCours<MAX_MUSIQUE&&m_musiqueEnCours<m_nombreMusique)
                    if (m_musique[m_musiqueEnCours].GetDuration()>0)
                        m_musique[m_musiqueEnCours].Play();
            }

            if (m_musiqueEnCours>=0&&m_musiqueEnCours<MAX_MUSIQUE&&m_musiqueEnCours<m_nombreMusique)
                m_musique[m_musiqueEnCours].SetPosition(-position.x, 0, position.y);
        }
        else
            m_musiqueEnCours=0;
}


/*void Map::Script_RandomDisplace(Jeu *jeu,Script *script,int noInstruction,int monstre,Hero *hero,float temps,Menu *menu, bool seDeplacer)
{
    if (m_monstre[monstre].EnVie())
    {
        if (m_monstre[monstre].getArrivee().x==m_monstre[monstre].getCoordonnee().x&&m_monstre[monstre].getArrivee().y==m_monstre[monstre].getCoordonnee().y)
        {
            coordonnee arrivee;
            arrivee=m_monstre[monstre].getDepart();
            arrivee.x+=(3-rand()%(6));
            arrivee.y+=(3-rand()%(6));
            m_monstre[monstre].setArrivee(arrivee);
        }
        else
        {
            if (seDeplacer)
            {
                coordonnee tempCoord(hero->m_personnage.getProchaineCase().x,hero->m_personnage.getProchaineCase().y,-1,-1);
                m_monstre[monstre].Pathfinding(getAlentourDuPersonnage(m_monstre[monstre].getCoordonnee()),tempCoord);
            }
        }
    }
}

void Map::Script_Evasion(Jeu *jeu,Script *script,int noInstruction,int monstre,Hero *hero,float temps,Menu *menu, bool seDeplacer)
{
    if (m_monstre[monstre].EnVie())
    {
        if (!m_monstre[monstre].frappeEnCours)
        {
            if (m_monstre[monstre].getArrivee().x==m_monstre[monstre].getCoordonnee().x&&m_monstre[monstre].getArrivee().y==m_monstre[monstre].getCoordonnee().y)
            {
                coordonnee arrivee;
                arrivee=m_monstre[monstre].getCoordonnee();
                arrivee.x+=m_monstre[monstre].getCoordonnee().x-hero->m_personnage.getCoordonnee().x;
                arrivee.y+=m_monstre[monstre].getCoordonnee().y-hero->m_personnage.getCoordonnee().y;
                m_monstre[monstre].setArrivee(arrivee);
            }
            else
            {
                if (seDeplacer)
                {
                    coordonnee tempCoord(hero->m_personnage.getProchaineCase().x,hero->m_personnage.getProchaineCase().y,-1,-1);
                    m_monstre[monstre].Pathfinding(getAlentourDuPersonnage(m_monstre[monstre].getCoordonnee()),tempCoord);
                }
            }
        }
    }
}

void Map::Script_Follow(Jeu *jeu,Script *script,int noInstruction,int monstre,Hero *hero,float temps,Menu *menu, bool seDeplacer)
{
    if (m_monstre[monstre].EnVie())
    {
        if (!m_monstre[monstre].frappeEnCours)
        {
            m_monstre[monstre].setArrivee(hero->m_personnage.getCoordonnee());

            if (m_monstre[monstre].getArrivee().x==m_monstre[monstre].getCoordonnee().x&&m_monstre[monstre].getArrivee().y==m_monstre[monstre].getCoordonnee().y)
            {
                m_monstre[monstre].setDepart();
            }
            else
            {
                if (seDeplacer)
                {
                    coordonnee tempCoord(hero->m_personnage.getProchaineCase().x,hero->m_personnage.getProchaineCase().y,-1,-1);
                    m_monstre[monstre].Pathfinding(getAlentourDuPersonnage(m_monstre[monstre].getCoordonnee()),tempCoord);
                }
            }
        }
    }
}

void Map::Script_Teleport(Jeu *jeu,Script *script,int noInstruction,int monstre,Hero *hero,float temps,Menu *menu, bool seDeplacer)
{
    if (m_monstre[monstre].EnVie())
    {
        if (seDeplacer && !m_monstre[monstre].frappeEnCours)
        {
            int temp = monstre;
            m_monstre[temp].setCoordonnee(m_monstre[temp].getArrivee());
            m_monstre[temp].setDepart();
            m_monstre[monstre].setArrivee(m_monstre[monstre].getCoordonnee());
        }
    }
}



void Map::Script_UseMiracle(Jeu *jeu,Script *script,int noInstruction,int monstre,Hero *hero,float temps,Menu *menu, bool seDeplacer)
{
    m_monstre[monstre].setArrivee(m_monstre[monstre].getProchaineCase());
    if (seDeplacer)
    {
        m_monstre[monstre].setArrivee(m_monstre[monstre].getCoordonnee());

        if (m_monstre[monstre].m_miracleALancer==-1)
        {
            m_monstre[monstre].m_miracleALancer=script->m_instructions[noInstruction].valeurs.at(0);
            if (m_monstre[monstre].EnVie())
                m_monstre[monstre].setEtat(2);
        }
        if (m_monstre[monstre].EnVie())
            m_monstre[monstre].Frappe(m_monstre[monstre].getCoordonnee(),m_monstre[monstre].m_cible->getCoordonnee());
    }
}

void Map::Script_SetState(Jeu *jeu,Script *script,int noInstruction,int monstre,Hero *hero,float temps,Menu *menu, bool seDeplacer)
{
    if (m_monstre[monstre].getEtat()!=script->m_instructions[noInstruction].valeurs.at(0))
        m_monstre[monstre].setJustEtat(script->m_instructions[noInstruction].valeurs.at(0));
}




void Map::Script_PlaySound(Jeu *jeu,Script *script,int noInstruction,int monstre,Hero *hero,float temps,Menu *menu, bool seDeplacer)
{
    if (m_monstre[monstre].getModele()>=0&&m_monstre[monstre].getModele()<(int)m_ModeleMonstre.size())
    {
        coordonnee position;
        position.x=(m_monstre[monstre].getCoordonnee().x-m_monstre[monstre].getCoordonnee().y-1)/5;
        position.y=(m_monstre[monstre].getCoordonnee().x+m_monstre[monstre].getCoordonnee().y)/5;

        coordonnee positionHero;
        positionHero.x=(hero->m_personnage.getCoordonnee().x-hero->m_personnage.getCoordonnee().y-1)/5;
        positionHero.y=(hero->m_personnage.getCoordonnee().x+hero->m_personnage.getCoordonnee().y)/5;

        m_ModeleMonstre[m_monstre[monstre].getModele()].JouerSon(script->m_instructions[noInstruction].valeurs.at(0),position,positionHero,true);
    }
}

void Map::Script_Shoot(Jeu *jeu,Script *script,int noInstruction,int monstre,Hero *hero,float temps,Menu *menu, bool seDeplacer)
{
    m_monstre[monstre].setArrivee(m_monstre[monstre].getProchaineCase());
    if (seDeplacer)
    {
        m_monstre[monstre].setArrivee(m_monstre[monstre].getCoordonnee());

        m_monstre[monstre].m_miracleALancer=-1;
        m_monstre[monstre].Frappe(m_monstre[monstre].getCoordonnee(),m_monstre[monstre].m_cible->getCoordonnee());
        m_monstre[monstre].m_shooter=true;
    }
}

void Map::Script_Fight(Jeu *jeu,Script *script,int noInstruction,int monstre,Hero *hero,float temps,Menu *menu, bool seDeplacer)
{
    if (!m_monstre[monstre].getErreurPathfinding())
    {
        if (m_monstre[monstre].EnVie())
        {
            coordonnee arrivee;

            if (seDeplacer)
            {
                m_monstre[monstre].setDepart();
                if (fabs(m_monstre[monstre].getCoordonnee().x-m_monstre[monstre].m_cible->getCoordonnee().x)>1
                        || fabs(m_monstre[monstre].getCoordonnee().y-m_monstre[monstre].m_cible->getCoordonnee().y)>1)
                {
                    m_monstre[monstre].setArrivee(m_monstre[monstre].m_cible->getProchaineCase());

                    coordonnee tempCoord(hero->m_personnage.getProchaineCase().x,hero->m_personnage.getProchaineCase().y,-1,-1);
                    m_monstre[monstre].Pathfinding(getAlentourDuPersonnage(m_monstre[monstre].getCoordonnee()),tempCoord);
                }
                else
                {
                    if (m_monstre[monstre].m_cible->EnVie()<=0)
                        m_monstre[monstre].setVu(0);
                    if (!m_monstre[monstre].frappeEnCours)
                        m_monstre[monstre].setEtat(2);
                    m_monstre[monstre].Frappe(m_monstre[monstre].getCoordonnee(),m_monstre[monstre].m_cible->getCoordonnee());
                    m_monstre[monstre].setArrivee(m_monstre[monstre].getCoordonnee());
                    m_monstre[monstre].m_shooter=false;
                }
            }

            if (fabs(m_monstre[monstre].getCoordonnee().x-m_monstre[monstre].m_cible->getCoordonnee().x)>1
                    || fabs(m_monstre[monstre].getCoordonnee().y-m_monstre[monstre].m_cible->getCoordonnee().y)>1)
            {
                if ( m_monstre[monstre].getArrivee().x==m_monstre[monstre].getCoordonnee().x
                        && m_monstre[monstre].getArrivee().y==m_monstre[monstre].getCoordonnee().y)
                    m_monstre[monstre].setArrivee(m_monstre[monstre].m_cible->getProchaineCase());

            }
        }
    }
}

void Map::Script_Trade(Jeu *jeu,Script *script,int noInstruction,int monstre,Hero *hero,float temps,Menu *menu, bool seDeplacer)
{
    hero->setMonstreVise(-1);
    jeu->m_inventaire->setTrader(m_monstre[monstre].getPointeurObjets(),&hero->m_classe);
    jeu->Clock.Reset();
    jeu->m_contexte=jeu->m_inventaire;
}*/

std::string DecouperTexte(std::string texte, int tailleCadran, int tailleTexte)
{
    sf::Text temp;
    temp.SetCharacterSize(tailleTexte);
    temp.SetFont(moteurGraphique->m_font);

    std::string buf;
    std::string bufMot;
    for (int p = 0;p < (int)texte.size();p++)
    {
        if (texte[p] != ' ' && texte[p] != '"')
        {
            bufMot += texte[p];
            temp.SetString(buf + bufMot);
            if (temp.GetRect().Width > tailleCadran)
                bufMot = '\n' + bufMot;
        }
        else
            buf += bufMot, bufMot = " ";
    }

    return buf;
}


/*void Map::GererInstructions(Jeu *jeu,Script *script,int noInstruction,int monstre,Hero *hero,float temps,Menu *menu, bool seDeplacer)
{
    if (noInstruction>=0&&noInstruction<(int)script->m_instructions.size())
    {
        if (script->m_instructions[noInstruction].nom=="fight")
            Script_Fight(jeu,script,noInstruction,monstre,hero,temps,menu,seDeplacer);
        else if (script->m_instructions[noInstruction].nom=="evasion")
            Script_Evasion(jeu,script,noInstruction,monstre,hero,temps,menu,seDeplacer);
        else if (script->m_instructions[noInstruction].nom=="followHero")
            Script_Follow(jeu,script,noInstruction,monstre,hero,temps,menu,seDeplacer);
        else if (script->m_instructions[noInstruction].nom=="teleport")
            Script_Teleport(jeu,script,noInstruction,monstre,hero,temps,menu,seDeplacer);
        else if (script->m_instructions[noInstruction].nom=="useMiracle")
            Script_UseMiracle(jeu,script,noInstruction,monstre,hero,temps,menu,seDeplacer); //USEMIRACLE(script->m_instructions[noInstruction].valeurs.at(0))
        else if (script->m_instructions[noInstruction].nom=="setState")
            Script_SetState(jeu,script,noInstruction,monstre,hero,temps,menu,seDeplacer); //SETSTATE(script->m_instructions[noInstruction].valeurs.at(0))
        else if (script->m_instructions[noInstruction].nom=="dammages")
            InfligerDegats(monstre, script->m_instructions[noInstruction].valeurs.at(0), hero, false);
        else if (script->m_instructions[noInstruction].nom=="shoot")
            Script_Shoot(jeu,script,noInstruction,monstre,hero,temps,menu,seDeplacer);
        else if (script->m_instructions[noInstruction].nom=="randomDisplace")
            Script_RandomDisplace(jeu,script,noInstruction,monstre,hero,temps,menu,seDeplacer);
        else if (script->m_instructions[noInstruction].nom=="playSound")
            Script_PlaySound(jeu,script,noInstruction,monstre,hero,temps,menu,seDeplacer); //PLAYSOUND(script->m_instructions[noInstruction].valeurs.at(0))
        else if (script->m_instructions[noInstruction].nom=="trade")
            Script_Trade(jeu,script,noInstruction,monstre,hero,temps,menu,seDeplacer);
        else if (script->m_instructions[noInstruction].nom=="if")
            GererConditions(jeu,script,noInstruction,monstre,hero,temps,menu,seDeplacer);
        else if (script->m_instructions[noInstruction].nom=="variable")
            script->variables[script->m_instructions[noInstruction].valeurs.at(0)]=script->m_instructions[noInstruction].valeurs.at(1);
        else if (script->m_instructions[noInstruction].nom=="incrementeVariable")
            script->variables[script->m_instructions[noInstruction].valeurs.at(0)]+=script->m_instructions[noInstruction].valeurs.at(1);

        else if (script->m_instructions[noInstruction].nom=="speak")
        {
            if (jeu->menu.m_dialogue == " ")
                jeu->menu.m_dialogue = DecouperTexte(script->m_instructions[noInstruction].valeurString, hero->m_classe.position_contenu_dialogue.w, 14);
        }
        else if (script->m_instructions[noInstruction].nom=="newQuest")
        {
            bool ok = true;
            for (int i = 0;i < (int)hero->m_quetes.size(); ++i)
                if (hero->m_quetes[i].m_id == script->m_instructions[noInstruction].valeurs.at(0))
                    ok = false;
            if (ok)
                hero->m_quetes.push_back(Quete (script->m_instructions[noInstruction].valeurs.at(0)));
        }
        else if (script->m_instructions[noInstruction].nom=="setQuestName")
        {
            for (int i = 0;i < (int)hero->m_quetes.size(); ++i)
                if (hero->m_quetes[i].m_id == script->m_instructions[noInstruction].valeurs.at(0))
                    hero->m_quetes[i].m_nom = DecouperTexte(script->m_instructions[noInstruction].valeurString, hero->m_classe.position_contenu_quetes.w, 12);
        }
        else if (script->m_instructions[noInstruction].nom=="setQuestState")
        {
            for (int i = 0;i < (int)hero->m_quetes.size(); ++i)
                if (hero->m_quetes[i].m_id == script->m_instructions[noInstruction].valeurs.at(0))
                {
                    hero->m_quetes[i].m_description = DecouperTexte(script->m_instructions[noInstruction].valeurString, hero->m_classe.position_contenu_description_quete.w, 12);
                    hero->m_quetes[i].m_statut = script->m_instructions[noInstruction].valeurs.at(1);
                }
        }
        else if (script->m_instructions[noInstruction].nom=="giftItem")
        {
            if (script->m_instructions[noInstruction].valeurs.at(0) >= 0 && script->m_instructions[noInstruction].valeurs.at(0) < (int)(*m_monstre[monstre].getPointeurObjets()).size())
                if (!hero->AjouterObjet((*m_monstre[monstre].getPointeurObjets())[script->m_instructions[noInstruction].valeurs.at(0)], false))
                    m_decor[1][hero->m_personnage.getCoordonnee().y][hero->m_personnage.getCoordonnee().x].AjouterObjet((*m_monstre[monstre].getPointeurObjets())[script->m_instructions[noInstruction].valeurs.at(0)]);
        }
    }
}

void Map::GererConditions(Jeu *jeu,Script *script,int noInstruction,int monstre,Hero *hero,float temps,Menu *menu, bool seDeplacer)
{
    if (noInstruction>=0&&noInstruction<(int)script->m_instructions.size())
    {
        int b=0;
        bool ok=true;
        for (;b<(int)script->m_instructions[noInstruction].valeurs.size()&&script->m_instructions[noInstruction].valeurs[b]!=-2;b++)
        {
            if (script->m_instructions[noInstruction].valeurs[b]>=0&&script->m_instructions[noInstruction].valeurs[b]<(int)script->m_instructions.size())
            {
                if (script->m_instructions[script->m_instructions[noInstruction].valeurs[b]].nom=="alive")
                {
                    if (!m_monstre[monstre].EnVie())
                        ok=false;
                }
                else if (script->m_instructions[script->m_instructions[noInstruction].valeurs[b]].nom=="see")
                {
                    if (!m_monstre[monstre].getVu())
                        ok=false;
                }
                else if (script->m_instructions[script->m_instructions[noInstruction].valeurs[b]].nom=="shooter")
                {
                    if (!m_monstre[monstre].m_shooter)
                        ok=false;
                }
                else if (script->m_instructions[script->m_instructions[noInstruction].valeurs[b]].nom=="touch")
                {
                    if (!m_monstre[monstre].m_touche)
                        ok=false;
                }
                else if (script->m_instructions[script->m_instructions[noInstruction].valeurs[b]].nom=="numberInvocation")
                {
                    if (m_monstre[monstre].m_nombreInvocation==script->m_instructions[script->m_instructions[noInstruction].valeurs[b]].valeurs.at(0))
                        ok=true;
                    else
                        ok=false;
                }
                else if (script->m_instructions[script->m_instructions[noInstruction].valeurs[b]].nom=="distance")
                {
                    if (((m_monstre[monstre].getCoordonnee().x-hero->m_personnage.getCoordonnee().x)*(m_monstre[monstre].getCoordonnee().x-hero->m_personnage.getCoordonnee().x) + (m_monstre[monstre].getCoordonnee().y-hero->m_personnage.getCoordonnee().y)*(m_monstre[monstre].getCoordonnee().y-hero->m_personnage.getCoordonnee().y)) < script->m_instructions[script->m_instructions[noInstruction].valeurs[b]].valeurs.at(0)*script->m_instructions[script->m_instructions[noInstruction].valeurs[b]].valeurs.at(0))
                        ok=true;
                    else
                        ok=false;
                }
                else if (script->m_instructions[script->m_instructions[noInstruction].valeurs[b]].nom=="rand")
                {
                    if (rand() % 100 <= script->m_instructions[script->m_instructions[noInstruction].valeurs[b]].valeurs.at(0))
                        ok=true;
                    else
                        ok=false;
                }
                else if (script->m_instructions[script->m_instructions[noInstruction].valeurs[b]].nom=="variable")
                {
                    if (script->variables[script->m_instructions[script->m_instructions[noInstruction].valeurs[b]].valeurs.at(0)]==script->m_instructions[script->m_instructions[noInstruction].valeurs[b]].valeurs.at(1))
                        ok=true;
                    else
                        ok=false;
                }
                else if (script->m_instructions[script->m_instructions[noInstruction].valeurs[b]].nom=="talk")
                {
                    if (hero->getMonstreVise()==monstre&&fabs(m_monstre[monstre].getCoordonnee().x-hero->m_personnage.getCoordonnee().x)<=1&&fabs(m_monstre[monstre].getCoordonnee().y-hero->m_personnage.getCoordonnee().y)<=1)
                        ok=true;
                    else
                        ok=false;
                }
                else if (script->m_instructions[script->m_instructions[noInstruction].valeurs[b]].nom=="quest")
                {
                    if (script->m_instructions[script->m_instructions[noInstruction].valeurs[b]].valeurs.at(1) == -1)
                    {
                        for (int i = 0;i < (int)hero->m_quetes.size(); ++i)
                            if (hero->m_quetes[i].m_id == script->m_instructions[script->m_instructions[noInstruction].valeurs[b]].valeurs.at(0))
                                ok = false;
                    }
                    else
                    {
                        ok = false;
                        for (int i = 0;i < (int)hero->m_quetes.size(); ++i)
                            if (hero->m_quetes[i].m_id == script->m_instructions[script->m_instructions[noInstruction].valeurs[b]].valeurs.at(0))
                                if (hero->m_quetes[i].m_statut == script->m_instructions[script->m_instructions[noInstruction].valeurs[b]].valeurs.at(1))
                                    ok = true;
                    }
                }
            }
        }
        for (;b<(int)script->m_instructions[noInstruction].valeurs.size()&&script->m_instructions[noInstruction].valeurs[b]!=-1&&script->m_instructions[noInstruction].valeurs[b]!=-3;b++)
            if (ok)
                if (script->m_instructions[noInstruction].valeurs[b]>=0&&script->m_instructions[noInstruction].valeurs[b]<(int)script->m_instructions.size())
                    GererInstructions(jeu,script,script->m_instructions[noInstruction].valeurs[b],monstre,hero,temps,menu,seDeplacer);
        if (!ok)
            for (;b<(int)script->m_instructions[noInstruction].valeurs.size()&&script->m_instructions[noInstruction].valeurs[b]!=-1;b++)
                if (script->m_instructions[noInstruction].valeurs[b]>=0&&script->m_instructions[noInstruction].valeurs[b]<(int)script->m_instructions.size())
                    GererInstructions(jeu,script,script->m_instructions[noInstruction].valeurs[b],monstre,hero,temps,menu,seDeplacer);
    }
}*/

/*void Map::GererProjectilesEtEffets(Hero *hero,float temps)
{
    coordonnee vueMin,vueMax;

    vueMin.x=hero->m_personnage.getCoordonnee().x-15;
    vueMin.y=hero->m_personnage.getCoordonnee().y-15;
    vueMax.x=hero->m_personnage.getCoordonnee().x+15;
    vueMax.y=hero->m_personnage.getCoordonnee().y+15;

    if (vueMin.x<0)
        vueMin.x=0;
    if (vueMin.y<0)
        vueMin.y=0;
    if (vueMax.x>=m_dimensions.x)
        vueMax.x=m_dimensions.x-1;
    if (vueMax.y>=m_dimensions.y)
        vueMax.y=m_dimensions.y-1;

    if (!m_projectile.empty())
    {
        int nombreInactif=0;
        for (int i=0;i<(int)m_projectile.size();++i)
        {
            m_projectile[i].m_dejaDeplace=false;
            if (m_projectile[i].m_supprime)
                nombreInactif++;

            if (!m_projectile[i].m_dejaDeplace)
            {
                if (m_projectile[i].m_actif)
                {
                    m_projectile[i].m_dejaDeplace=true;

                    int temp=i;
                    m_decor[m_projectile[temp].m_positionCase.h][m_projectile[i].m_positionCase.y][m_projectile[i].m_positionCase.x].setProjectile(-1);

                    if (m_projectile[temp].m_position.y/COTE_TILE>=vueMin.y&&m_projectile[temp].m_position.y/COTE_TILE<vueMax.y&&m_projectile[temp].m_position.x/COTE_TILE>=vueMin.x&&m_projectile[temp].m_position.x/COTE_TILE<vueMax.x)
                    {
                        if (m_projectile[temp].m_cible.x == -1 && m_projectile[temp].m_cible.y == -1)
                        {
                            if (!m_decor[1][(int)(m_projectile[temp].m_positionCase.y)][(int)(m_projectile[temp].m_positionCase.x)].getMonstre().empty())
                            {
                                bool collision = false;
                                for (unsigned o = 0 ; o < m_decor[1][(int)(m_projectile[temp].m_positionCase.y)][(int)(m_projectile[temp].m_positionCase.x)].getMonstre().size() && !collision ; ++o)
                                    if ( m_decor[1][(int)(m_projectile[temp].m_positionCase.y)][(int)(m_projectile[temp].m_positionCase.x)].getMonstre()[o] < (int)m_monstre.size()&&!m_projectile[temp].m_monstre)
                                        if (m_monstre[m_decor[1][(int)(m_projectile[temp].m_positionCase.y)][(int)(m_projectile[temp].m_positionCase.x)].getMonstre()[o]].EnVie())
                                        if(m_monstre[m_decor[1][(int)(m_projectile[temp].m_positionCase.y)][(int)(m_projectile[temp].m_positionCase.x)].getMonstre()[o]].m_friendly == m_projectile[temp].m_monstre)
                                        {
                                            collision                         = true;
                                            m_projectile[temp].m_actif        = false;
                                            m_projectile[temp].m_entite_cible = &m_monstre[m_decor[1][(int)(m_projectile[temp].m_positionCase.y)][(int)(m_projectile[temp].m_positionCase.x)].getMonstre()[o]];
                                           // InfligerDegats(m_decor[1][(int)(m_projectile[temp].m_positionCase.y)][(int)(m_projectile[temp].m_positionCase.x)].getMonstre()[o],m_projectile[temp].m_degats,hero,false);
                                        }
                            }

                            if (hero->m_personnage.getCoordonnee().x==(int)((m_projectile[temp].m_position.x+32)/COTE_TILE)&&hero->m_personnage.getCoordonnee().y==(int)((m_projectile[temp].m_position.y+32)/COTE_TILE)&&m_projectile[temp].m_monstre)
                            {
                                m_projectile[temp].m_actif=false;
                                m_projectile[temp].m_entite_cible = &hero->m_personnage;
                               // hero->m_personnage.InfligerDegats(m_projectile[temp].m_degats);
                            }
                        }
                        else
                        {
                            bool x = false;
                            bool y = false;

                            if ( m_projectile[temp].m_positionCase.x >= m_projectile[temp].m_cible.x && m_projectile[temp].m_cible.x >= m_projectile[temp].m_depart.x)
                                x = true;
                            else if ( m_projectile[temp].m_positionCase.x <= m_projectile[temp].m_cible.x && m_projectile[temp].m_cible.x <= m_projectile[temp].m_depart.x)
                                x = true;

                            if ( m_projectile[temp].m_positionCase.y >= m_projectile[temp].m_cible.y && m_projectile[temp].m_cible.y >= m_projectile[temp].m_depart.y)
                                y = true;
                            else if ( m_projectile[temp].m_positionCase.y <= m_projectile[temp].m_cible.y && m_projectile[temp].m_cible.y <= m_projectile[temp].m_depart.y)
                                y = true;

                            if (x && y)
                                m_projectile[temp].m_actif = false;
                        }

                        if (getTypeCase((int)(m_projectile[temp].m_positionCase.x),(int)(m_projectile[temp].m_positionCase.y))==1)
                            m_projectile[temp].m_actif=false;



                        if (m_projectile[temp].m_actif)
                        {
                            m_projectile[i].m_positionCase.y=(int)((m_projectile[temp].m_position.y+32)/COTE_TILE);
                            m_projectile[i].m_positionCase.x=(int)((m_projectile[temp].m_position.x+32)/COTE_TILE);
                            if (m_decor[m_projectile[i].m_positionCase.h][m_projectile[i].m_positionCase.y][m_projectile[i].m_positionCase.x].getProjectile()!=-1)
                            {
                                if (m_projectile[i].m_positionCase.h==1)
                                    m_projectile[i].m_positionCase.h=0;
                                else
                                    m_projectile[i].m_positionCase.h=1;
                            }

                            m_decor[m_projectile[i].m_positionCase.h][m_projectile[i].m_positionCase.y][m_projectile[i].m_positionCase.x].setProjectile(temp);
                        }
                    }
                    else
                    {
                        m_projectile[temp].m_actif=false;
                        if (m_decor[1][(int)(m_projectile[i].m_position.y/COTE_TILE)][(int)(m_projectile[i].m_position.x/COTE_TILE)].getProjectile()==i)
                            m_decor[1][(int)(m_projectile[temp].m_position.y/COTE_TILE)][(int)(m_projectile[temp].m_position.x/COTE_TILE)].setProjectile(-1);
                        if (m_decor[0][(int)(m_projectile[i].m_position.y/COTE_TILE)][(int)(m_projectile[i].m_position.x/COTE_TILE)].getProjectile()==i)
                            m_decor[0][(int)(m_projectile[temp].m_position.y/COTE_TILE)][(int)(m_projectile[temp].m_position.x/COTE_TILE)].setProjectile(-1);
                    }

                    m_projectile[i].Deplacer(temps);
                }
                else
                {
                    moteurGraphique->LightManager->Delete_Light(m_projectile[i].m_effet.m_light);
                    m_decor[m_projectile[i].m_positionCase.h][m_projectile[i].m_positionCase.y][m_projectile[i].m_positionCase.x].setProjectile(-1);
                }
            }

        }
        if (nombreInactif==(int)m_projectile.size())
            m_projectile.clear();
    }

    if (!m_effets.empty())
    {
        int nombreInactif=0;
        for (int i=0;i<(int)m_effets.size();++i)
            if (!m_effets[i].m_actif)
                nombreInactif++,moteurGraphique->LightManager->Delete_Light(m_effets[i].m_light);
        if (nombreInactif==(int)m_effets.size())
            m_effets.clear();
    }
}

void Map::GererMonstres(Jeu *jeu,Hero *hero,float temps,Menu *menu)
{
    coordonnee vueMin,vueMax;

    vueMin.x=hero->m_personnage.getCoordonnee().x-15;
    vueMin.y=hero->m_personnage.getCoordonnee().y-15;
    vueMax.x=hero->m_personnage.getCoordonnee().x+15;
    vueMax.y=hero->m_personnage.getCoordonnee().y+15;

    if (vueMin.x<0)
        vueMin.x=0;
    if (vueMin.y<0)
        vueMin.y=0;
    if (vueMax.x>=m_dimensions.x)
        vueMax.x=m_dimensions.x-1;
    if (vueMax.y>=m_dimensions.y)
        vueMax.y=m_dimensions.y-1;


    for (int j=vueMin.y;j<vueMax.y;j++)
        for (int k=vueMin.x;k<vueMax.x;k++)
            for (unsigned o = 0 ; o < m_decor[1][j][k].getMonstre().size() ; o++)
            {
                int monstre = m_decor[1][j][k].getMonstre()[o];
                if (monstre >= 0 && monstre < (int)m_monstre.size())
                {
                    if (m_monstre[monstre].m_cible != NULL)
                    {
                        if (m_monstre[monstre].m_cible->EnVie())
                        {
                            if (m_monstre[monstre].m_friendly == m_monstre[monstre].m_cible->m_friendly)
                                m_monstre[monstre].setVu(0);
                        }
                        else
                            m_monstre[monstre].setVu(0);
                    }
                    else
                        m_monstre[monstre].setVu(0);

                    if (m_monstre[monstre].getVu() == 0)
                        TestVisionMonstre(monstre, hero);

                    if (m_monstre[monstre].m_cible != NULL)
                        m_monstre[monstre].TesterVision(m_monstre[monstre].m_cible->getCoordonnee());

                    if (m_monstre[monstre].m_attente<=0)
                    {
                        bool seDeplacer = m_monstre[monstre].SeDeplacer(temps*100,getDimensions(),
                                          (!getCollision((int)((m_monstre[monstre].getCoordonneePixel().x + m_monstre[monstre].getPousse().x * temps * COTE_TILE * 5 + COTE_TILE * 0.5)/COTE_TILE),
                                                         (int)((m_monstre[monstre].getCoordonneePixel().y + m_monstre[monstre].getPousse().y * temps * COTE_TILE * 5 + COTE_TILE * 0.5)/COTE_TILE),
                                                         monstre)));

                        if (m_monstre[monstre].getPousse().x == 0 && m_monstre[monstre].getPousse().y == 0)
                        {
                            Script *script=&m_monstre[monstre].m_scriptAI;
                            if ((int)script->m_instructions.size()>0)
                                for (int a=0;a<(int)script->m_instructions[0].valeurs.size();a++)
                                    if (script->m_instructions[0].valeurs[a]>=0&&script->m_instructions[0].valeurs[a]<(int)script->m_instructions.size())
                                        GererInstructions(jeu,script,script->m_instructions[0].valeurs[a],monstre,hero,temps,menu,seDeplacer);

                            if (m_monstre[monstre].getErreurPathfinding())
                                Script_RandomDisplace(jeu,script,0,monstre,hero,temps,menu,seDeplacer);
                        }

                        m_monstre[monstre].m_touche = false;
                    }
                    else
                        m_monstre[monstre].m_attente-=temps*100;

                    if (m_monstre[monstre].getErreurPathfinding())
                        m_monstre[monstre].m_compteur++;
                    else
                        m_monstre[monstre].m_compteur=0;

                   // if (m_monstre[monstre].m_compteur>=2)
                    // m_monstre[monstre].m_attente=1,m_monstre[monstre].m_compteur=0,m_monstre[monstre].setArrivee(m_monstre[monstre].getCoordonnee()),m_monstre[monstre].setErreurPathfinding(false);

                    ///GESTION DES EVENEMENTS SUR LES MONSTRES
                    for (int l=0;l<2;l++)
                        if (m_monstre[monstre].getCoordonnee().y>=0&&m_monstre[monstre].getCoordonnee().y<m_dimensions.y
                                &&m_monstre[monstre].getCoordonnee().x>=0&&m_monstre[monstre].getCoordonnee().x<m_dimensions.x)
                            for (int z=0;z<(int)m_decor[l][m_monstre[monstre].getCoordonnee().y][m_monstre[monstre].getCoordonnee().x].getEvenement().size();z++)
                                if (m_decor[l][m_monstre[monstre].getCoordonnee().y][m_monstre[monstre].getCoordonnee().x].getEvenement()[z]>=0&&m_decor[l][m_monstre[monstre].getCoordonnee().y][m_monstre[monstre].getCoordonnee().x].getEvenement()[z]<(int)m_evenement.size())
                                    if (m_evenement[m_decor[l][m_monstre[monstre].getCoordonnee().y][m_monstre[monstre].getCoordonnee().x].getEvenement()[z]].getType()==INFLIGER_DEGATS)
                                        if (m_monstre[monstre].EnVie())
                                            InfligerDegats(monstre,m_evenement[m_decor[l][m_monstre[monstre].getCoordonnee().y][m_monstre[monstre].getCoordonnee().x].getEvenement()[z]].getInformation(0)*temps*10,hero,0);

                    if (m_monstre[monstre].EnVie())
                        if (m_monstre[monstre].getCoordonnee().y!=j || m_monstre[monstre].getCoordonnee().x != k)
                            if ( m_monstre[monstre].getCoordonnee().x >= 0 && m_monstre[monstre].getCoordonnee().x < m_dimensions.x
                                    && m_monstre[monstre].getCoordonnee().y >= 0 && m_monstre[monstre].getCoordonnee().y < m_dimensions.y)
                            {
                                m_decor[1][j][k].delMonstre(monstre);
                                m_decor[1][m_monstre[monstre].getCoordonnee().y][m_monstre[monstre].getCoordonnee().x].setMonstre(monstre);
                            }

                }

                //if (m_decor[i][j][k].getProjectile()<0||m_decor[i][j][k].getProjectile()>=(int)m_projectile.size())
                //   m_decor[i][j][k].setProjectile(-1);

                //if (m_decor[i][j][k].getEffetGraphique()>=0&&m_decor[i][j][k].getEffetGraphique()<(int)m_effets.size())
                //{
                //  if (!m_effets[m_decor[i][j][k].getEffetGraphique()].m_actif)
                //      m_decor[i][j][k].setEffetGraphique(-1);
                //}
                //else
                //  m_decor[i][j][k].setEffetGraphique(-1);
            }
}

void Map::TestVisionMonstre(int numero, Hero *hero)
{
    if (numero >= 0 && numero < (int)m_monstre.size())
        //for (int couche = 0 ; couche < 2 ; ++couche)
        for (int y = m_monstre[numero].getCoordonnee().y - 5 ;
                y < m_monstre[numero].getCoordonnee().y + 5 ; ++y)
            for (int x = m_monstre[numero].getCoordonnee().x - 5 ;
                    x < m_monstre[numero].getCoordonnee().x + 5 ; ++x)
                if (y >= 0 && x >= 0 && y < (int)m_decor[0].size() && x < (int)m_decor[0][0].size())
                {

                    if (m_monstre[numero].m_friendly)
                    {
                        for (unsigned o = 0 ; o < m_decor[1][y][x].getMonstre().size() ; ++o )
                        {
                            if (m_decor[1][y][x].getMonstre()[o] >= 0)
                                if (!m_monstre[m_decor[1][y][x].getMonstre()[o]].m_friendly && m_monstre[m_decor[1][y][x].getMonstre()[o]].EnVie() && m_monstre[m_decor[1][y][x].getMonstre()[o]].getCaracteristique().niveau > 0)
                                {
                                    if (m_monstre[numero].m_cible == NULL)
                                        m_monstre[numero].m_cible = &m_monstre[m_decor[1][y][x].getMonstre()[o]];
                                    else
                                    {
                                        int x1 = m_monstre[numero].m_cible->getCoordonnee().x - m_monstre[numero].getCoordonnee().x;
                                        int y1 = m_monstre[numero].m_cible->getCoordonnee().y - m_monstre[numero].getCoordonnee().y;

                                        int x2 = m_monstre[m_decor[1][y][x].getMonstre()[o]].getCoordonnee().x - m_monstre[numero].getCoordonnee().x;
                                        int y2 = m_monstre[m_decor[1][y][x].getMonstre()[o]].getCoordonnee().y - m_monstre[numero].getCoordonnee().y;
                                        if ( x1 * x1 + y1 * y1 > x2 * x2 + y2 * y2)
                                            m_monstre[numero].m_cible = &m_monstre[m_decor[1][y][x].getMonstre()[o]];
                                    }
                                }
                        }
                    }
                    else
                    {
                        if(hero->m_personnage.EnVie())
                            if (hero->m_personnage.getCoordonnee().y == y && hero->m_personnage.getCoordonnee().x == x)
                            {
                                if (m_monstre[numero].m_cible == NULL)
                                    m_monstre[numero].m_cible = &hero->m_personnage;
                                else
                                {
                                    int x1 = m_monstre[numero].m_cible->getCoordonnee().x - m_monstre[numero].getCoordonnee().x;
                                    int y1 = m_monstre[numero].m_cible->getCoordonnee().y - m_monstre[numero].getCoordonnee().y;

                                    int x2 = x - m_monstre[numero].getCoordonnee().x;
                                    int y2 = y - m_monstre[numero].getCoordonnee().y;
                                    if ( x1 * x1 + y1 * y1 > x2 * x2 + y2 * y2)
                                        m_monstre[numero].m_cible = &hero->m_personnage;
                                }
                            }

                        for (unsigned o = 0 ; o < m_decor[1][y][x].getMonstre().size() ; ++o )
                        {
                            if (m_decor[1][y][x].getMonstre()[o] >= 0)
                                if (m_monstre[m_decor[1][y][x].getMonstre()[o]].m_friendly  && m_monstre[m_decor[1][y][x].getMonstre()[o]].EnVie() && m_monstre[m_decor[1][y][x].getMonstre()[o]].getCaracteristique().niveau > 0)
                                {
                                    if (m_monstre[numero].m_cible == NULL)
                                        m_monstre[numero].m_cible = &m_monstre[m_decor[1][y][x].getMonstre()[o]];
                                    else
                                    {
                                        int x1 = m_monstre[numero].m_cible->getCoordonnee().x - m_monstre[numero].getCoordonnee().x;
                                        int y1 = m_monstre[numero].m_cible->getCoordonnee().y - m_monstre[numero].getCoordonnee().y;

                                        int x2 = x - m_monstre[numero].getCoordonnee().x;
                                        int y2 = y - m_monstre[numero].getCoordonnee().y;
                                        if ( x1 * x1 + y1 * y1 > x2 * x2 + y2 * y2)
                                            m_monstre[numero].m_cible = &m_monstre[m_decor[1][y][x].getMonstre()[o]];
                                    }
                                }
                        }
                    }

                }
}

bool Map::InfligerDegats(int numero, float degats, Hero *hero,bool pousser)
{
    if (numero >= 0 && numero < (int)m_monstre.size())
    {
        return InfligerDegats(&m_monstre[numero], degats, hero, pousser);

        if (!m_monstre[numero].EnVie())
            if (m_monstreIllumine == numero || hero->getMonstreVise() == numero)
                m_monstreIllumine=-1,hero->setMonstreVise(-1);
    }
    return (false);
}

bool Map::InfligerDegats(Personnage *monstre, float degats, Hero *hero,bool pousser)
{

    float viePrecedente = monstre->getCaracteristique().vie;


    monstre->InfligerDegats(degats);

    hero->m_personnage.InfligerDegats(-degats * hero->m_caracteristiques.volVie);
    hero->m_caracteristiques.foi += degats * hero->m_caracteristiques.volFoi;

    for (int x=monstre->getCoordonnee().x;x<10+monstre->getCoordonnee().x;x++)
        for (int y=monstre->getCoordonnee().y;y<10+monstre->getCoordonnee().y;y++)
            if (x>=0&&y>=0&&x<m_dimensions.x&&y<m_dimensions.y)
                for (unsigned o = 0 ; o < m_decor[1][y][x].getMonstre().size() ; ++o)
                    if (m_decor[1][y][x].getMonstre()[o]>=0&&m_decor[1][y][x].getMonstre()[o]<(int)m_monstre.size())
                        m_monstre[m_decor[1][y][x].getMonstre()[o]].setVu(1);

    if (!monstre->EnVie() && viePrecedente > 0 && monstre != &hero->m_personnage)
    {
        for (int i=0;i<(int)monstre->m_miracleEnCours.size();++i)
        {
            for (int o=0;o<(int)monstre->m_miracleEnCours[i].m_infos.size();o++)
                if (monstre->m_miracleEnCours[i].m_infos[o].m_effetEnCours>=0)
                    if (m_ModeleMonstre[monstre->getModele()].m_miracles[monstre->m_miracleEnCours[i].m_modele].m_effets[monstre->m_miracleEnCours[i].m_infos[o].m_effetEnCours].m_type==INVOCATION)
                        if (monstre->m_miracleEnCours[i].m_infos[o].m_IDObjet>=0&&monstre->m_miracleEnCours[i].m_infos[o].m_IDObjet<(int)m_monstre.size())
                            InfligerDegats(monstre->m_miracleEnCours[i].m_infos[o].m_IDObjet, m_monstre[monstre->m_miracleEnCours[i].m_infos[o].m_IDObjet].getCaracteristique().vie ,hero,false);
        }

        if (monstre->getCaracteristique().pointAme>0)
        {
            hero->m_personnage.AjouterPointAme(monstre->getCaracteristique().pointAme);
        }

        float force=((degats*2)/monstre->getCaracteristique().maxVie)*10,angle;

        double m=atan2(monstre->getCoordonneePixel().y-hero->m_personnage.getCoordonneePixel().y,monstre->getCoordonneePixel().x-hero->m_personnage.getCoordonneePixel().x);

            angle=(int)(0+(m*180)/M_PI);
            if (angle>=360)
                angle=0;
            if (angle<0)
                angle=360+angle;

            angle+=45;

            if (force<7)
                force=7;
            if (force>20)
                force=20;

        if (configuration->particules&&m_ModeleMonstre[monstre->getModele()].m_particules>=0)
        {
            coordonnee position2;
            position2.x=(int)(((monstre->getCoordonneePixel().x-monstre->getCoordonneePixel().y)*64/COTE_TILE));
            position2.y=(int)(((monstre->getCoordonneePixel().x+monstre->getCoordonneePixel().y)*64/COTE_TILE)/2);

            sf::Color buffer(255,255,255);
            if (monstre->getPorteeLumineuse().intensite>0)
            {
                buffer.r=monstre->getPorteeLumineuse().rouge;
                buffer.g=monstre->getPorteeLumineuse().vert;
                buffer.b=monstre->getPorteeLumineuse().bleu;
            }
            moteurGraphique->AjouterSystemeParticules(m_ModeleMonstre[monstre->getModele()].m_particules,position2,buffer,force,angle);
        }

        monstre->Pousser(coordonneeDecimal(cos(m) * force * 0.1, sin(m) * force * 0.1));

        if (monstre->getCoordonnee().x>=0&&monstre->getCoordonnee().x<m_dimensions.x&&monstre->getCoordonnee().y>=0&&monstre->getCoordonnee().y<m_dimensions.y)
            for (int i=0;i<(int)monstre->getObjets().size();++i)
                m_decor[1][monstre->getCoordonnee().y][monstre->getCoordonnee().x].AjouterObjet(monstre->getObjets()[i]);

        return 1;
    }

    return 0;
}

void Map::PousserMonstre(int numeroMonstre, coordonneeDecimal vecteur)
{
    if (numeroMonstre>=0&&numeroMonstre<(int)m_monstre.size())
        m_monstre[numeroMonstre].Pousser(vecteur);
}

bool Map::RamasserObjet(Hero *hero,bool enMain)
{
    coordonnee position;

    if (enMain)
        position=hero->m_personnage.getCoordonnee();
    else
        position=hero->getChercherSac();


    if (position.x>=0&&position.x<m_dimensions.x
            &&position.y>=0&&position.y<m_dimensions.y)
        if (m_objetPointe>=0&&m_objetPointe<m_decor[1][position.y][position.x].getNombreObjets())
        {
            if (hero->AjouterObjet(*m_decor[1][position.y][position.x].getObjet(m_objetPointe),enMain))
            {
                m_decor[1][position.y][position.x].supprimerObjet(m_objetPointe);
                return true;
            }
        }
    return false;
}*/


void Map::AjouterObjet(Objet objet)
{
    /*if (objet.getPosition().x>=0&&objet.getPosition().x<m_dimensions.x&&objet.getPosition().y>=0&&objet.getPosition().y<m_dimensions.y)
    {
        m_decor[1][objet.getPosition().y][objet.getPosition().x].AjouterObjet(objet);
    }*/
}

void Map::AjouterMonstre(Monstre monstre)
{
  /*  m_monstre.push_back(monstre);
    if(m_monstre.back().getCoordonnee().x >= 0 && m_monstre.back().getCoordonnee().x < (int)m_decor[0][0].size()
    && m_monstre.back().getCoordonnee().y >= 0 && m_monstre.back().getCoordonnee().y < (int)m_decor[0].size())
        m_decor[1][m_monstre.back().getCoordonnee().y][m_monstre.back().getCoordonnee().x].setMonstre(m_monstre.size()-1);*/
}


void Map::AjouterModeleMonstre(Modele_Monstre monstre)
{
    m_ModeleMonstre.push_back(monstre);
}

void Map::setVolumeMusique(int volume)
{
    for (int i=0;i<MAX_MUSIQUE;++i)
        m_musique[i].SetVolume(volume);
}

const coordonnee &Map::getSacPointe()
{
    return m_sacPointe;
}
int Map::getObjetPointe()
{
    return m_objetPointe;
}
int Map::getNombreObjets(coordonnee position)
{
    /*if (position.x>=0&&position.x<m_dimensions.x
            &&position.y>=0&&position.y<m_dimensions.y)
        return m_decor[1][position.y][position.x].getNombreObjets();
    else
        return 0;*/
}

int Map::getNombreMonstres()
{
    return m_monstre.size();
}

int Map::getNombreModeleMonstres()
{
    return m_ModeleMonstre.size();
}

const coordonnee &Map::getPositionMonstre(int numeroMonstre)
{
    if (numeroMonstre>=0&&numeroMonstre<(int)m_monstre.size())
        return m_monstre[numeroMonstre].getCoordonnee();

    return m_monstre[0].getCoordonnee();
}

Monstre *Map::getEntiteMonstre(int numeroMonstre)
{
    if (numeroMonstre>=0&&numeroMonstre<(int)m_monstre.size())
        return &m_monstre[numeroMonstre];
    else
        return (NULL);
}

Modele_Monstre Map::getModeleMonstre(int numeroMonstre)
{
    if (numeroMonstre>=0&&numeroMonstre<(int)m_ModeleMonstre.size())
        return m_ModeleMonstre[numeroMonstre];
    else
        return m_ModeleMonstre[0];
}

bool Map::getMonstreEnVie(int numeroMonstre)
{
    if (numeroMonstre>=0&&numeroMonstre<(int)m_monstre.size())
        return m_monstre[numeroMonstre].EnVie();
    else
        return 0;
}

casePathfinding** Map::getAlentourDuPersonnage(coordonnee positionPersonnage)
{
    /*casePathfinding **grille = new casePathfinding* [10];

    for (int y=positionPersonnage.y-5;y<positionPersonnage.y+5;y++)
    {
        grille[y-positionPersonnage.y+5] = new casePathfinding[10];
        for (int x=positionPersonnage.x-5;x<positionPersonnage.x+5;x++)
        {
            grille[y-positionPersonnage.y+5][x-positionPersonnage.x+5].hauteur=0;
            if (y>=0&&x>=0&&x<m_dimensions.x&&y<m_dimensions.y)
            {
                grille[y-positionPersonnage.y+5][x-positionPersonnage.x+5].collision=getCollision(x,y);
                //if(m_decor[0][y][x].getHauteur()>m_decor[1][y][x].getHauteur())
                grille[y-positionPersonnage.y+5][x-positionPersonnage.x+5].hauteur=m_decor[0][y][x].getHauteur();
                // else
                // grille[y-positionPersonnage.y+10][x-positionPersonnage.x+10].hauteur=m_decor[1][y][x].getHauteur();
            }
            else
                grille[y-positionPersonnage.y+5][x-positionPersonnage.x+5].collision=1;
        }
    }

    return grille;*/
}

int Map::getMonstreIllumine()
{
    return m_monstreIllumine;
}

const coordonnee &Map::getDimensions()
{
    return m_dimensions;
}

bool Map::getCollision(int positionX,int positionY, int exception)
{
    if(positionX >= 0 && positionX < m_decor[0][0].size()
    && positionY >= 0 && positionY < m_decor[0].size())
    for (int i=0;i<2;++i)
    for (int z=0;z<m_decor[i][positionY][positionX].size();++z)
    if(m_moduleAleatoireCur >= m_decor[i][positionY][positionX][z].m_moduleAleatoireMin)
    if(m_moduleAleatoireCur <= m_decor[i][positionY][positionX][z].m_moduleAleatoireMax)
    {
        if (m_decor[i][positionY][positionX][z].getTileset()>=0&&m_decor[i][positionY][positionX][z].getTileset()<(int)m_tileset.size())
            if (moteurGraphique->getTileset(m_tileset[m_decor[i][positionY][positionX][z].getTileset()])->getCollisionTile(m_decor[i][positionY][positionX][z].getTile()[m_decor[i][positionY][positionX][z].random_animation]))
                return 1;

        if (i == 1)
        {

            for (unsigned o = 0 ; o < m_decor[i][positionY][positionX][z].getMonstre().size() ; ++o)
                if (m_decor[i][positionY][positionX][z].getMonstre()[o]>-1&&m_decor[i][positionY][positionX][z].getMonstre()[o]<(int)m_monstre.size())
                        return 1;
        }
    }
    return 0;
}

int Map::getTypeCase(int positionX,int positionY)
{
    /*if (positionY>=0&&positionY<m_dimensions.y&&positionX>=0&&positionX<m_dimensions.x)
    {
        if (m_decor[0][positionY][positionX].getTileset()>=0&&m_decor[0][positionY][positionX].getTileset()<(int)m_tileset.size())
            //if(m_tileset[m_decor[0][positionY][positionX].getTileset()].getCollisionTile(m_decor[0][positionY][positionX].getTile()))
            if ((int)m_tileset[m_decor[0][positionY][positionX].getTileset()].getLumiereDuTile(m_decor[0][positionY][positionX].getTile()).hauteur>32)
                return 1;

        if (m_decor[0][positionY][positionX].getNombreObjets())
            return 4;

        for (int z=0;z<(int)m_decor[0][positionY][positionX].getEvenement().size();z++)
            if (m_decor[0][positionY][positionX].getEvenement()[z]>=0&&m_decor[0][positionY][positionX].getEvenement()[z]<(int)m_evenement.size())
                if (m_evenement[m_decor[0][positionY][positionX].getEvenement()[z]].getType()==CHANGEMENT_DE_MAP)
                    return 3;
    }
    else
        return 1;

    if (positionY>=0&&positionY<m_dimensions.y&&positionX>=0&&positionX<m_dimensions.x)
    {
        if (m_decor[1][positionY][positionX].getTileset()>=0&&m_decor[1][positionY][positionX].getTileset()<(int)m_tileset.size())
            //if(m_tileset[m_decor[1][positionY][positionX].getTileset()].getCollisionTile(m_decor[1][positionY][positionX].getTile()))
            if ((int)m_tileset[m_decor[1][positionY][positionX].getTileset()].getLumiereDuTile(m_decor[1][positionY][positionX].getTile()).hauteur>32)
                return 1;

        for (unsigned o = 0 ; o < m_decor[1][positionY][positionX].getMonstre().size() ; ++o)
            if (m_decor[1][positionY][positionX].getMonstre()[o]>-1&&m_decor[1][positionY][positionX].getMonstre()[o]<(int)m_monstre.size())
                if (m_monstre[m_decor[1][positionY][positionX].getMonstre()[o]].EnVie())
                {
                    if (m_monstre[m_decor[1][positionY][positionX].getMonstre()[o]].m_friendly)
                        return 5;
                    else if (m_ModeleMonstre[m_monstre[m_decor[1][positionY][positionX].getMonstre()[o]].getModele()].m_minimap)
                        return 2;
                }

        if (m_decor[1][positionY][positionX].getNombreObjets())
            return 4;

        for (int z=0;z<(int)m_decor[1][positionY][positionX].getEvenement().size();z++)
            if (m_decor[1][positionY][positionX].getEvenement()[z]>=0&&m_decor[1][positionY][positionX].getEvenement()[z]<(int)m_evenement.size())
                if (m_evenement[m_decor[1][positionY][positionX].getEvenement()[z]].getType()==CHANGEMENT_DE_MAP)
                    return 3;
    }
    else
        return 1;


    return 0;*/
}

bool Map::getCollisionPousse(int positionX,int positionY)
{
  /* if (positionY>=0&&positionY<m_dimensions.y&&positionX>=0&&positionX<m_dimensions.x)
    {
        if (m_decor[0][positionY][positionX].getTileset()>=0&&m_decor[0][positionY][positionX].getTileset()<(int)m_tileset.size())
            if(m_tileset[m_decor[0][positionY][positionX].getTileset()].getCollisionTile(m_decor[0][positionY][positionX].getTile()))
                return 1;

        if (m_decor[1][positionY][positionX].getTileset()>=0&&m_decor[1][positionY][positionX].getTileset()<(int)m_tileset.size())
            if(m_tileset[m_decor[1][positionY][positionX].getTileset()].getCollisionTile(m_decor[1][positionY][positionX].getTile()))
                return 1;

        for (unsigned o = 0 ; o < m_decor[1][positionY][positionX].getMonstre().size() ; ++o)
            if (m_decor[1][positionY][positionX].getMonstre()[o]>-1&&m_decor[1][positionY][positionX].getMonstre()[o]<(int)m_monstre.size())
                if (m_monstre[m_decor[1][positionY][positionX].getMonstre()[o]].EnVie() && m_monstre[m_decor[1][positionY][positionX].getMonstre()[o]].m_impenetrable)
                    return 1;
    }
    else
        return 1;


    return 0;*/
}

int Map::getMonstre(Hero *hero,coordonnee positionSouris,coordonnee casePointee)
{
   /* float distance=100000;
    int meilleur=-1;
    bool meilleurIsFriendly = true;
    coordonnee vueMin,vueMax;

    vueMin.x=casePointee.x-2;
    vueMin.y=casePointee.y-2;
    vueMax.x=casePointee.x+2;
    vueMax.y=casePointee.y+2;

    if (vueMin.x<0)
        vueMin.x=0;
    if (vueMin.y<0)
        vueMin.y=0;
    if (vueMax.x>m_dimensions.x)
        vueMax.x=m_dimensions.x;
    if (vueMax.y>m_dimensions.y)
        vueMax.y=m_dimensions.y;

    m_sacPointe.x=-1;
    m_sacPointe.y=-1;

    if (casePointee.y>=0&&casePointee.y<m_dimensions.y&&casePointee.x>=0&&casePointee.x<m_dimensions.x)
    {
        for (int j=vueMin.y;j<vueMax.y;j++)
            for (int k=vueMin.x;k<vueMax.x;k++)
            {
                for (unsigned o = 0 ; o < m_decor[1][j][k].getMonstre().size() ; ++o)
                    if (m_decor[1][j][k].getMonstre()[o]>=0&&m_decor[1][j][k].getMonstre()[o]<(int)m_monstre.size())
                        if (m_monstre[m_decor[1][j][k].getMonstre()[o]].EnVie()&&m_monstre[m_decor[1][j][k].getMonstre()[o]].getCaracteristique().rang>=0)
                        {
                            coordonneeDecimal temp;
                            temp.x=(((m_monstre[m_decor[1][j][k].getMonstre()[o]].getCoordonneePixel().x-m_monstre[m_decor[1][j][k].getMonstre()[o]].getCoordonneePixel().y)*64/COTE_TILE));
                            temp.y=(((m_monstre[m_decor[1][j][k].getMonstre()[o]].getCoordonneePixel().x+m_monstre[m_decor[1][j][k].getMonstre()[o]].getCoordonneePixel().y)*64/COTE_TILE)/2+32);

                            coordonnee positionSourisTotale=moteurGraphique->getPositionSouris();

                            if (positionSourisTotale.x>temp.x-96
                                    && positionSourisTotale.x<temp.x+96
                                    && positionSourisTotale.y>temp.y-128
                                    && positionSourisTotale.y<temp.y+32)
                            {
                                float temp2=0;
                                temp2=((temp.x-(positionSourisTotale.x))
                                       *(temp.x-(positionSourisTotale.x))
                                       +(temp.y-(positionSourisTotale.y+24))
                                       *(temp.y-(positionSourisTotale.y+24)));

                                if ( (distance>temp2 && ((!m_monstre[m_decor[1][j][k].getMonstre()[o]].m_friendly && !meilleurIsFriendly) || (m_monstre[m_decor[1][j][k].getMonstre()[o]].m_friendly && meilleurIsFriendly))) ||  (!m_monstre[m_decor[1][j][k].getMonstre()[o]].m_friendly && meilleurIsFriendly) )
                                {
                                    if (m_monstre[m_decor[1][j][k].getMonstre()[o]].m_friendly)
                                        meilleurIsFriendly = true;
                                    else
                                        meilleurIsFriendly = false;
                                    meilleur=m_decor[1][j][k].getMonstre()[o],distance=temp2;
                                }
                            }
                        }
            }
    }

    m_monstreIllumine=meilleur;
    return meilleur;*/
}
