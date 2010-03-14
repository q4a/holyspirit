

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


using namespace std;
using namespace sf;

#include "../globale.h"
#include "../jeu.h"

inline sf::Vector2f AutoScreenAdjust(float x, float y, float decalage = 0)
{
    sf::Vector2f temp;
    temp.x = x + (configuration->Resolution.x - 800) * 0.5;
    temp.y = y + (configuration->Resolution.y - 600) - decalage * configuration->Resolution.h/600;
    return temp;
}

Map::Map()
{
    console->Ajouter("");
    console->Ajouter("Chargements d'images diverses :");
    IDImageSac=moteurGraphique->AjouterImage(configuration->chemin_menus+configuration->nom_sac,-1);
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

    /*if (m_decor)
    {
        for (int i=0;i<NOMBRE_COUCHE_MAP;++i)
        {
            if (m_decor[i])
            {
                for (int j=0;j<m_dimensions.y;j++)
                {
                    if (configuration->debug)
                        console->Ajouter(j);
                    if (m_decor[i][j])
                        delete[] m_decor[i][j];
                }
                if (configuration->debug)
                    console->Ajouter("-");
                delete[] m_decor[i];
            }
        }
        delete[] m_decor;
    }*/

    if (configuration->debug)
        console->Ajouter("Cases détruites !");

    m_ModeleMonstre.clear();

    if (configuration->debug)
        console->Ajouter("Modèles monstres détruits !");

    m_monstre.clear();

    if (configuration->debug)
        console->Ajouter("Monstres détruits !");

    m_evenement.clear();
    if (configuration->debug)
        console->Ajouter("Evénements détruits !");

    m_fond.clear();
    if (configuration->debug)
        console->Ajouter("Fonds détruits !");

    m_musiques.clear();
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

bool Map::Charger(std::string nomMap,Hero *hero)
{
    m_dimensions.x=0;
    m_dimensions.y=0;

    int numeroModuleAleatoire=rand()%10;

    bool entite_map_existante=0,mapExistante=0;

    m_nom_fichier=nomMap;
    string chemin = nomMap,chemin2 = configuration->chemin_temps+nomMap;

    m_lumiere[0].intensite=1;
    m_lumiere[0].rouge=0;
    m_lumiere[0].vert=0;
    m_lumiere[0].bleu=0;

    m_musiqueEnCours=0;

    console->Ajouter("",0);
    console->Ajouter("Chargement de la map : "+nomMap);

    cDAT reader,reader2;

    reader.Read(configuration->chemin_saves+"hero.sav.hs");

    if (reader.IsFileExist(configuration->chemin_temps+nomMap))
        mapExistante=true,entite_map_existante=true,console->Ajouter("Map sauvée existante.");

    ifstream *fichier=NULL;
    ifstream *fichier2=NULL;

    if (mapExistante==true)
    {
        fichier=reader.GetInfos(configuration->chemin_temps+nomMap);
        fichier2=reader.GetInfos(configuration->chemin_temps+"entites_map_"+nomMap+".emap.hs");
    }
    else
    {
        reader2.Read(configuration->chemin_maps);
        fichier=reader2.GetInfos(nomMap);
    }

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
                throw ("Erreur : Map \" "+chemin+" \" Invalide");
            }
        }
        while (caractere!='$');

        if (configuration->debug)
            console->Ajouter("/Lectures des fonds.");

        m_musiqueEnCours = 0;
        do
        {
            //Chargement des musiques
            fichier->get(caractere);
            if (caractere=='*')
            {
                std::string temp2;
                *fichier>>temp2;
                m_musiques.push_back(temp2);
            }
            if (fichier->eof())
            {
                console->Ajouter("Erreur : Map \" "+chemin+" \" Invalide",1);
                throw ("Erreur : Map \" "+chemin+" \" Invalide");
            }
        }
        while (caractere!='$');

        if (configuration->debug)
            console->Ajouter("/Lectures des musiques.");

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
                throw ("Erreur : Map \" "+chemin+" \" Invalide");
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
                throw ("Erreur : Map \" "+chemin+" \" Invalide");
            }
        }
        while (caractere!='$');

        if (configuration->debug)
            console->Ajouter("/Lectures des monstres.");

        console->Ajouter("");

        if (entite_map_existante==true)
        {
            console->Ajouter("Une map des entités est déjà existante.");
            if (fichier2)
            {
                char caractere;
                do
                {
                    //Chargement du nom
                    fichier2->get(caractere);
                    if (caractere=='*')
                    {

                        Caracteristique tempCaract;
                        Lumiere lumiere;
                        int numeroModele=-1,vieMin=0,vieMax=1,degatsMin=0,degatsMax=0,rang=0,ame=0,pose=0,etat=0,angle=0;
                        float taille=1;
                        vector <Objet> objets;
                        std::vector <int> variables;
                        m_monstre.push_back(Monstre ());

                        int pos;


                        char caractere;
                        do
                        {
                            //Chargement du nom
                            fichier2->get(caractere);
                            switch (caractere)
                            {
                            case 'm':
                                *fichier2>>numeroModele;
                                break;
                            case 'v':
                                fichier2->get(caractere);
                                if (caractere=='i')
                                {
                                    *fichier2>>vieMin;
                                }
                                else if (caractere=='a')
                                {
                                    *fichier2>>vieMax;
                                }
                                break;
                            case 'd':
                                fichier2->get(caractere);
                                if (caractere=='i')
                                {
                                    *fichier2>>degatsMin;
                                }
                                else if (caractere=='a')
                                {
                                    *fichier2>>degatsMax;
                                }
                                break;
                            case 'r':
                                *fichier2>>rang;
                                break;
                            case 'a':
                                *fichier2>>ame;
                                break;
                            case 't':
                                *fichier2>>taille;
                                break;

                            case 'p':
                                *fichier2>>pose;
                                break;
                            case 'e':
                                *fichier2>>etat;
                                break;
                            case 'g':
                                *fichier2>>angle;
                                break;

                            case 'l':
                                fichier2->get(caractere);
                                switch (caractere)
                                {
                                case 'r':
                                    *fichier2>>lumiere.rouge;
                                    break;
                                case 'v':
                                    *fichier2>>lumiere.vert;
                                    break;
                                case 'b':
                                    *fichier2>>lumiere.bleu;
                                    break;
                                case 'i':
                                    *fichier2>>lumiere.intensite;
                                    break;
                                }
                                break;

                            case 'o':
                                pos=fichier2->tellg();
                                objets.push_back(Objet ());
                                objets.back().ChargerTexte(fichier2,tempCaract,true);
                                objets.back().Charger(objets.back().getChemin(),tempCaract,true);
                                fichier2->seekg(pos, ios::beg);
                                objets.back().m_benedictions.clear();
                                objets.back().ChargerTexte(fichier2,tempCaract);
                                break;

                            case 's':
                                int no;
                                *fichier2>>no;
                                if(no >= variables.size())
                                    variables.resize(no + 1);
                                *fichier2>>variables[no];
                                break;


                            }

                            if (fichier2->eof())
                            {
                                console->Ajouter("Erreur : Map \" "+chemin+" \" Invalide",1);
                                throw ("Erreur : Map \" "+chemin+" \" Invalide");
                            }
                        }
                        while (caractere!='$');

                        if ((int)m_monstre.size()>0)
                        {
                            if (numeroModele>=0&&numeroModele<(int)m_ModeleMonstre.size())
                                m_monstre.back().Charger(numeroModele,&m_ModeleMonstre[numeroModele]);
                            Caracteristique caracteristique = m_monstre.back().getCaracteristique();
                            caracteristique.vie=vieMin;
                            caracteristique.maxVie=vieMax;
                            caracteristique.degatsMin[PHYSIQUE]=degatsMin;
                            caracteristique.degatsMax[PHYSIQUE]=degatsMax;
                            caracteristique.rang=rang;
                            caracteristique.pointAme=ame;
                            caracteristique.modificateurTaille=taille;
                            m_monstre.back().setCaracteristique(caracteristique);
                            m_monstre.back().setPorteeLumineuse(lumiere);
                            m_monstre.back().setEtat(etat);
                            m_monstre.back().setPose(pose);
                            m_monstre.back().setAngle(angle);
                            m_monstre.back().setObjets(objets);
                            for(int z=0;z<variables.size();++z)
                                m_monstre.back().m_scriptAI.setVariable(z,variables[z]);
                        }

                        objets.clear();
                    }

                    if (fichier2->eof())
                    {
                        console->Ajouter("Erreur : Map \" "+chemin+" \" Invalide",1);
                        throw ("Erreur : Map \" "+chemin+" \" Invalide");
                    }
                }
                while (caractere!='$');
                fichier2->close();
            }

        }

        m_evenement.clear();

        do
        {
            fichier->get(caractere);
            if (caractere=='*')
            {
                int numeroEvenement;
                do
                {
                    fichier->get(caractere);
                    switch (caractere)
                    {
                    case 'e':
                        *fichier>>numeroEvenement;
                        break;
                    }
                    if (fichier->eof())
                    {
                        console->Ajouter("Erreur : Map \" "+chemin+" \" Invalide",1);
                        throw ("Erreur : Map \" "+chemin+" \" Invalide");
                    }
                }
                while (caractere!='$');
                m_evenement.push_back(numeroEvenement);

                //AjouterEvenement(numeroEvenement);

                int information;
                do
                {
                    fichier->get(caractere);
                    if (caractere=='i')
                    {
                        *fichier>>information;

                        m_evenement.back().AjouterInformation(information);
                    }

                    if (caractere=='m')
                    {
                        std::string temp;
                        *fichier>>temp;

                        m_evenement.back().setString(temp);
                    }
                    if (fichier->eof())
                    {
                        console->Ajouter("Erreur : Map \" "+chemin+" \" Invalide",1);
                        throw ("Erreur : Map \" "+chemin+" \" Invalide");
                    }
                }
                while (caractere!='$');

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
            console->Ajouter("/Lectures des événements.");

        Decor decorTemp(-1,-1,std::vector<int> (),std::vector<int> (),-1,0,0);

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
                    m_decor[couche].push_back(std::vector<Decor> ());
                    do
                    {
                        Caracteristique tempCaract;

                        int pos;
                        std::vector<int> evenement;
                        int tileset=-1,tileFinal=-1,herbe=-1,layer=0,hauteur=0;
                        int temp, id = -1;
                        vector <int>tile;
                        vector <int>monstre, monstreFinal;
                        vector <Objet> objets;
                        bool added_minimap = false;

                        do
                        {

                            switch (caractere)
                            {

                            case '+':
                                fichier->get(caractere);
                                if(caractere == 'm')
                                {
                                    if (entite_map_existante!=true)
                                    {
                                        if (!monstre.empty())
                                        {
                                            int random = rand() % (int)monstre.size();
                                            if (random>=0&&random<(int)monstre.size())
                                                monstreFinal.push_back(monstre[random]);

                                            monstre.clear();
                                        }

                                        if (!monstreFinal.empty())
                                        {
                                            if (monstreFinal.back()>=0&&monstreFinal.back()<(int)m_ModeleMonstre.size())
                                            {
                                                m_monstre.push_back(Monstre ());
                                                m_monstre.back().Charger(monstreFinal.back(),&m_ModeleMonstre[monstreFinal.back()]);
                                                m_monstre.back().setCoordonnee(position),m_monstre.back().setDepart();
                                                m_monstre.back().m_ID = id;

                                                if(id >= 0)
                                                {
                                                    if(m_listID.size() <= id)
                                                        m_listID.resize(id + 1);
                                                    m_listID[id].push_back(m_monstre.size() - 1);
                                                }

                                                TrierInventaire(m_monstre.back().getPointeurObjets(),hero->m_classe.position_contenu_marchand.w);

                                                monstreFinal.back()=m_monstre.size()-1;
                                            }
                                            else
                                                monstreFinal.clear();
                                        }

                                        monstre.clear();
                                    }

                                    int temp3;
                                    *fichier>>temp3;
                                    monstre.push_back(temp3);
                                }
                                break;

                            case 's':
                                *fichier>>tileset;
                                break;
                            case 't':
                                *fichier>>temp;
                                tile.push_back(temp);
                                break;
                            case 'e':
                                int temp2;
                                *fichier>>temp2;
                                evenement.push_back(temp2);
                                break;
                            case 'm':
                                *fichier>>temp;
                                monstre.push_back(temp);
                                break;
                            case 'd':
                               *fichier>>id;
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
                            case 'p':
                                *fichier>>added_minimap;
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

                            case 'r':
                                fichier->get(caractere);
                                int noModuleCaseMin=-1,noModuleCaseMax=-1;
                                do
                                {
                                    if (caractere=='i')
                                        *fichier>>noModuleCaseMin;
                                    else if (caractere=='a')
                                        *fichier>>noModuleCaseMax;
                                    else if (caractere=='*')
                                        do
                                        {
                                            fichier->get(caractere);

                                            if (numeroModuleAleatoire>=noModuleCaseMin&&numeroModuleAleatoire<=noModuleCaseMax)
                                                switch (caractere)
                                                {
                                                case 's':
                                                    *fichier>>tileset;
                                                    break;
                                                case 't':
                                                    *fichier>>temp;
                                                    tile.push_back(temp);
                                                    break;
                                                case 'e':
                                                    int temp2;
                                                    *fichier>>temp2;
                                                    evenement.push_back(temp2);
                                                    break;
                                                case 'm':
                                                    *fichier>>temp;
                                                    monstre.push_back(temp);
                                                    break;
                                                case 'd':
                                                   *fichier>>id;
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

                                                case 'o':
                                                    pos=fichier->tellg();
                                                    objets.push_back(Objet ());
                                                    objets.back().ChargerTexte(fichier,tempCaract,true);
                                                    objets.back().Charger(objets.back().getChemin(),tempCaract,true);
                                                    fichier->seekg(pos, ios::beg);
                                                    objets.back().m_benedictions.clear();
                                                    objets.back().ChargerTexte(fichier,tempCaract);
                                                    break;
                                                }

                                            if (fichier->eof())
                                            {
                                                console->Ajouter("Erreur : Map \" "+chemin+" \" Invalide",1);
                                                throw ("Erreur : Map \" "+chemin+" \" Invalide");
                                            }
                                        }
                                        while (caractere!='$');

                                    fichier->get(caractere);

                                    if (fichier->eof())
                                    {
                                        console->Ajouter("Erreur : Map \" "+chemin+" \" Invalide",1);
                                        throw ("Erreur : Map \" "+chemin+" \" Invalide");
                                    }
                                }
                                while (caractere!='$');
                                break;
                            }
                            fichier->get(caractere);
                            if (fichier->eof())
                            {
                                console->Ajouter("Erreur : Map \" "+chemin+" \" Invalide",1);
                                throw ("Erreur : Map \" "+chemin+" \" Invalide");
                            }
                        }
                        while (caractere!='|' && caractere!='$');

                        if (caractere!='$')
                        {
                            if (couche==1)
                                if (m_decor[0][position.y][position.x].getHerbe()>=0&&herbe<0)
                                    herbe=m_decor[0][position.y][position.x].getHerbe();

                            if (entite_map_existante!=true)
                            {
                                if (!monstre.empty())
                                {
                                    int random = rand() % (int)monstre.size();
                                    if (random>=0&&random<(int)monstre.size())
                                        monstreFinal.push_back(monstre[random]);

                                    monstre.clear();
                                }

                                if (!monstreFinal.empty())
                                {
                                    if (monstreFinal.back()>=0&&monstreFinal.back()<(int)m_ModeleMonstre.size())
                                    {
                                        m_monstre.push_back(Monstre ());
                                        m_monstre.back().Charger(monstreFinal.back(),&m_ModeleMonstre[monstreFinal.back()]);
                                        m_monstre.back().setCoordonnee(position),m_monstre.back().setDepart();
                                        m_monstre.back().m_ID = id;

                                        if(id >= 0)
                                        {
                                            if(m_listID.size() <= id)
                                                m_listID.resize(id + 1);
                                            m_listID[id].push_back(m_monstre.size() - 1);
                                        }


                                        TrierInventaire(m_monstre.back().getPointeurObjets(),hero->m_classe.position_contenu_marchand.w);

                                        monstreFinal.back()=m_monstre.size()-1;
                                    }
                                    else
                                        monstreFinal.clear();
                                }
                            }
                            else
                            {
                                for (unsigned i = 0 ; i < monstre.size() ; ++i)
                                {

                                    if (monstre[i]>=0&&monstre[i]<(int)m_monstre.size())
                                    {
                                        int etat,pose,angle;
                                        etat=m_monstre[monstre[i]].getEtat();
                                        pose=m_monstre[monstre[i]].getPose();
                                        angle=m_monstre[monstre[i]].getAngle();
                                        m_monstre[monstre[i]].setCoordonnee(position);
                                        m_monstre[monstre[i]].setDepart();
                                        m_monstre[monstre[i]].setEtat(etat);
                                        m_monstre[monstre[i]].setPose(pose);
                                        m_monstre[monstre[i]].setAngle(angle);

                                        m_monstre[monstre[i]].m_ID = id;

                                        if(id >= 0)
                                        {
                                            if(m_listID.size() <= id)
                                                m_listID.resize(id + 1);
                                            m_listID[id].push_back(monstre[i]);
                                        }

                                        monstreFinal.push_back(monstre[i]);
                                    }
                                }
                            }

                            if (tile.size()>0)
                            {
                                int random = (rand() % (tile.size() -1 - 0 + 1)) + 0;
                                if (random>=0&&random<(int)tile.size())
                                    tileFinal = tile[random];

                                tile.clear();
                            }

                            if (fichier->eof())
                            {
                                console->Ajouter("Erreur : Map \" "+chemin+" \" Invalide",1);
                                throw ("Erreur : Map \" "+chemin+" \" Invalide");
                            }

                            if (couche==0)
                                layer+=1;
                            else
                                layer+=10;

                            m_decor[couche][position.y].push_back(Decor (tileset,tileFinal,evenement,monstreFinal,herbe,layer,hauteur,objets));
                            m_decor[couche][position.y].back().added_minimap = added_minimap;

                            tileset=-1,tile.clear(),tileFinal=-1,evenement.clear(),monstreFinal.clear(),herbe=-1,layer=0,hauteur=0;
                            objets.clear();
                            position.x++;
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
                    throw ("Erreur : Map \" "+chemin+" \" Invalide");
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
        throw "";
    }



    for (int i=0;i<24;++i)
        if (m_lumiere[i].intensite<1)
            m_lumiere[i].intensite=1;

    delete fichier;
    delete fichier2;

    m_dimensions.x=(int)m_decor[0][0].size();
    m_dimensions.y=(int)m_decor[0].size();

    console->Ajouter("Chargement de la map terminé.");

    Initialiser(hero);

    if (configuration->debug)
        console->Ajouter("Initialisation terminée.");
    console->Ajouter("");

    return 1;
}


void Map::Initialiser(Hero *hero)
{
    CalculerOmbresEtLumieres();

    for (int couche=0;couche<2;couche++)
        for (int i=0;i<m_dimensions.y;++i)
            for (int j=0;j<m_dimensions.x;j++)
            {
                if (m_decor[couche][i][j].getHerbe()>=0&&m_decor[couche][i][j].getHerbe()<(int)m_herbe.size())
                {
                    if (m_herbe[m_decor[couche][i][j].getHerbe()] >= 0)
                    {
                        coordonnee position,positionPartieDecor;

                        int numeroHerbe=0;
                        if (moteurGraphique->getTileset(m_herbe[m_decor[couche][i][j].getHerbe()])->getTaille()>0)
                            numeroHerbe = (rand() % (moteurGraphique->getTileset(m_herbe[m_decor[couche][i][j].getHerbe()])->getTaille()));
                        m_decor[couche][i][j].setNumeroHerbe(numeroHerbe);

                        position.x=(j-i)*64;
                        position.y=(j+i+1)*32;

                        if (couche==0)
                            position.y-=32;
                        position.x+=m_decor[0][i][j].getDecalageHerbe().x;

                        m_decor[couche][i][j].m_entite_herbe = moteurGraphique->getEntiteGraphique(m_herbe[m_decor[couche][i][j].getHerbe()], numeroHerbe, 10);
                        m_decor[couche][i][j].m_entite_herbe.m_sprite.SetPosition(position.x, position.y - m_decor[0][i][j].getHauteur());
                        m_decor[couche][i][j].m_entite_herbe.m_sprite.SetScale((float)m_decor[couche][i][j].getTailleHerbe()/100,(float)m_decor[couche][i][j].getTailleHerbe()/100);
                        m_decor[couche][i][j].m_entite_herbe.m_sprite.SetColor(m_decor[couche][i][j].getCouleurHerbe());

                        m_decor[couche][i][j].m_entite_herbe.Initialiser(coordonnee ());
                    }
                }
            }

    if (configuration->debug)
        console->Ajouter("Initialisation de l'herbe terminée.");

    for (int i=0;i<(int)m_monstre.size();++i)
        if (m_monstre[i].getCaracteristique().rang>=0)
        {
           // m_monstre[i].m_light=moteurGraphique->LightManager->Add_Dynamic_Light();
          //  moteurGraphique->LightManager->SetQuality(m_monstre[i].m_light,6);

            coordonnee pos;
            pos.x=(int)(((m_monstre[i].getCoordonneePixel().x-m_monstre[i].getCoordonneePixel().y)*64/COTE_TILE));
            pos.y=(int)(((m_monstre[i].getCoordonneePixel().x+m_monstre[i].getCoordonneePixel().y)*64/COTE_TILE)/2+32)*2;

            m_monstre[i].m_entite_graphique.Initialiser(pos);

            if(m_monstre[i].m_entite_graphique.m_light.ID() == -1)
            {
                m_monstre[i].m_entite_graphique.m_light=moteurGraphique->LightManager->Add_Dynamic_Light();
                moteurGraphique->LightManager->SetQuality(m_monstre[i].m_entite_graphique.m_light,6);
            }


            //moteurGraphique->LightManager->SetPosition(m_monstre[i].m_light,pos);
            moteurGraphique->LightManager->SetColor(m_monstre[i].m_entite_graphique.m_light,sf::Color(m_monstre[i].getPorteeLumineuse().rouge,m_monstre[i].getPorteeLumineuse().vert,m_monstre[i].getPorteeLumineuse().bleu));
        }

    if (configuration->debug)
        console->Ajouter("Initialisation des entités terminée.");

    sf::Vector2f pos;

    for (int i=0;i<NOMBRE_COUCHE_MAP;++i)
        for (int j=0;j<m_dimensions.y;j++)
            for (int k=0;k<m_dimensions.x;k++)
                if (m_decor[i][j][k].getTileset()>=0&&m_decor[i][j][k].getTileset()<(int)m_tileset.size())
                {
                    coordonnee position, pos;

                    position.x=(k-j)*64;
                    position.y=(k+j+1)*32;

                    pos.x = (int)(((k*COTE_TILE-j*COTE_TILE)*64/COTE_TILE));
                    pos.y = (int)(((k*COTE_TILE+j*COTE_TILE)*64/COTE_TILE)+64);

                    m_decor[i][j][k].m_entite_graphique = moteurGraphique->getEntiteGraphique(m_tileset[m_decor[i][j][k].getTileset()], m_decor[i][j][k].getTile(), m_decor[i][j][k].getCouche());
                    m_decor[i][j][k].m_entite_graphique.m_sprite.SetPosition(position.x, position.y);
                    m_decor[i][j][k].m_entite_graphique.Initialiser(pos);

                    CreerSprite(sf::Vector3f(k,j,i));
                }

    if (configuration->debug)
        console->Ajouter("Initialisation des décors terminée.");

    for (int i=0;i<NOMBRE_COUCHE_MAP;++i)
        for (int j=0;j<m_dimensions.y;j++)
            for (int k=0;k<m_dimensions.x;k++)
                if (m_decor[i][j][k].getTileset()>=0&&m_decor[i][j][k].getTileset()<(int)m_tileset.size())
                {
                    if (m_decor[i][j][k].added_minimap)
                        if(m_decor[i][j][k].m_spriteMinimap.GetSize().x> 1)
                            hero->m_minimap.push_back(m_decor[i][j][k].m_spriteMinimap);
                }

    moteurGraphique->LightManager->Generate();
}

void Map::CreerSprite(sf::Vector3f position_case)
{
    int x = (int)position_case.x;
    int y = (int)position_case.y;
    int z = (int)position_case.z;

    coordonnee position,positionPartieDecor;

    position.x=(x-y-1)*64;
    position.y=(x+y)*32;

    if(m_decor[z][y][x].getTileset() != -1)
    {
        positionPartieDecor=moteurGraphique->getTileset(m_tileset[m_decor[z][y][x].getTileset()])->getPositionMinimap(m_decor[z][y][x].getTile());
        m_decor[z][y][x].m_spriteMinimap.SetImage(*moteurGraphique->getImage(moteurGraphique->getTileset(m_tileset[m_decor[z][y][x].getTileset()])->getMinimap(m_decor[z][y][x].getTile())));
        m_decor[z][y][x].m_spriteMinimap.SetSubRect(IntRect(positionPartieDecor.x, positionPartieDecor.y, positionPartieDecor.x+positionPartieDecor.w, positionPartieDecor.y+positionPartieDecor.h));
        m_decor[z][y][x].m_spriteMinimap.SetX(position.x);
        m_decor[z][y][x].m_spriteMinimap.SetY(position.y);
        m_decor[z][y][x].m_spriteMinimap.SetColor(sf::Color(255,255,255,128));
    }
}


void Map::Sauvegarder(Hero *hero)
{
    string chemin = configuration->chemin_temps+m_nom_fichier;

    console->Ajouter("",0);
    console->Ajouter("Sauvegarde de la map : "+chemin,0);

    ofstream fichier(chemin.c_str(), ios::out | ios::trunc);

    bool ajouter=true;
    for (int i=0;i<(int)hero->m_contenuSave.size();++i)
        if (hero->m_contenuSave[i]==chemin)
            ajouter=false;
    if (ajouter)
        hero->m_contenuSave.push_back(chemin);

    if (fichier)
    {
        fichier<<"* n"<<m_no_nom<<" $ \n$\n";
        for (int i=0;i<(int)m_fond.size();++i)
            fichier<<"*"<<m_fond[i]<<"\n";
        fichier<<"$\n";
        for (int i=0;i<(int)m_musiques.size();++i)
            fichier<<"*"<<m_musiques[i]<<"\n";
        fichier<<"$\n";

        for (int i=0;i<24;++i)
            fichier<<"*"<<m_lumiere[i].rouge<<" "<<m_lumiere[i].vert<<" "<<m_lumiere[i].bleu<<" "<<m_lumiere[i].intensite<<" "<<m_lumiere[i].hauteur<<"\n";

        fichier<<"$\n";

        if(!m_nom_img_sky.empty())
            fichier<<" *"<<m_nom_img_sky<<endl;
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

        for (int i=0;i<(int)m_evenement.size();++i)
        {
            fichier<<"* e"<<m_evenement[i].getType()<<" $ * ";
            if (m_evenement[i].getString()!="")
                fichier<<"m"<<m_evenement[i].getString()<<" ";
            for (int j=0;j<m_evenement[i].getNombreInformation();j++)
                fichier<<"i"<<m_evenement[i].getInformation(j)<<" ";
            fichier<<"$\n";
        }

        fichier<<"$\n";

        for (int couche=0;couche<2;couche++)
        {
            for (int i=0;i<m_dimensions.y;++i)
            {
                fichier<<"* ";
                for (int j=0;j<m_dimensions.x;j++)
                {
                    fichier<<" s"<<m_decor[couche][i][j].getTileset()<<" ";
                    fichier<<"t"<<m_decor[couche][i][j].getTile()<<" ";
                    for (unsigned k = 0 ; k < m_decor[couche][i][j].getEvenement().size() ; ++k)
                        fichier<<"e"<<m_decor[couche][i][j].getEvenement()[k]<<" ";

                    if(m_decor[couche][i][j].added_minimap)
                        fichier<<"p1";

                    for (unsigned k = 0 ; k < m_decor[couche][i][j].getMonstre().size() ; ++k)
                        if (m_decor[couche][i][j].getMonstre()[k] >= 0 && m_decor[couche][i][j].getMonstre()[k] < (int)m_monstre.size())
                        {
                            fichier<<"m"<<m_decor[couche][i][j].getMonstre()[k]<<" ";
                            if(m_monstre[m_decor[couche][i][j].getMonstre()[k]].m_ID >= 0)
                                fichier<<"d"<<m_monstre[m_decor[couche][i][j].getMonstre()[k]].m_ID<<" ";
                        }

                    fichier<<"h"<<m_decor[couche][i][j].getHerbe()<<" ";

                    fichier<<"i"<<m_decor[couche][i][j].getHauteur()<<" ";

                    if (couche==0)
                        fichier<<"l"<<m_decor[couche][i][j].getCouche()-1<<" ";
                    else
                        fichier<<"l"<<m_decor[couche][i][j].getCouche()-10<<" ";

                    for (int o=0;o<(int)m_decor[couche][i][j].getNombreObjets();o++)
                        m_decor[couche][i][j].getObjet(o)->SauvegarderTexte(&fichier);


                    fichier<<"|";
                }
                fichier<<"$\n";
            }
            fichier<<"$\n";
        }

        m_script.Sauvegarder(fichier);

        fichier.close();
    }
    else
    {
        console->Ajouter("Impossible d'ouvrir le fichier : "+chemin,1);
        throw "";
    }




    chemin = configuration->chemin_temps+"entites_map_"+m_nom_fichier+".emap.hs";

    console->Ajouter("Sauvegarde de la map_entite : "+chemin,0);

    ofstream fichier2(chemin.c_str(), ios::out | ios::trunc);

    ajouter=true;
    for (int i=0;i<(int)hero->m_contenuSave.size();++i)
        if (hero->m_contenuSave[i]==chemin)
            ajouter=false;
    if (ajouter)
        hero->m_contenuSave.push_back(chemin);

    if (fichier2)
    {
        for (std::vector<Monstre>::iterator iter = m_monstre.begin();iter!=m_monstre.end();++iter)
            iter->Sauvegarder(fichier2);

        fichier2<<"\n$";

        fichier2.close();
    }

    console->Ajouter("Sauvegarde de la map terminée !");
}





void Map::CalculerOmbresEtLumieres()
{
    if (configuration->heure+1<24)
    {
        moteurGraphique->m_soleil.intensite=(int)(((float)m_lumiere[configuration->heure].intensite*(60-configuration->minute)+((float)m_lumiere[configuration->heure+1].intensite*configuration->minute))*0.016666666666666666666666666666667f);
        moteurGraphique->m_soleil.rouge=(int)(((float)m_lumiere[configuration->heure].rouge*(60-configuration->minute)+((float)m_lumiere[configuration->heure+1].rouge*configuration->minute))*0.016666666666666666666666666666667f);
        moteurGraphique->m_soleil.vert=(int)(((float)m_lumiere[configuration->heure].vert*(60-configuration->minute)+((float)m_lumiere[configuration->heure+1].vert*configuration->minute))*0.016666666666666666666666666666667f);
        moteurGraphique->m_soleil.bleu=(int)(((float)m_lumiere[configuration->heure].bleu*(60-configuration->minute)+((float)m_lumiere[configuration->heure+1].bleu*configuration->minute))*0.016666666666666666666666666666667f);
        moteurGraphique->m_soleil.hauteur=((float)m_lumiere[configuration->heure].hauteur*(60-configuration->minute)+((float)m_lumiere[configuration->heure+1].hauteur*configuration->minute))*0.016666666666666666666666666666667f;
    }
    else
    {
        moteurGraphique->m_soleil.intensite=(int)(((float)m_lumiere[configuration->heure].intensite*(60-configuration->minute)+((float)m_lumiere[0].intensite*configuration->minute))*0.016666666666666666666666666666667f);
        moteurGraphique->m_soleil.rouge=(int)(((float)m_lumiere[configuration->heure].rouge*(60-configuration->minute)+((float)m_lumiere[0].rouge*configuration->minute))*0.016666666666666666666666666666667f);
        moteurGraphique->m_soleil.vert=(int)(((float)m_lumiere[configuration->heure].vert*(60-configuration->minute)+((float)m_lumiere[0].vert*configuration->minute))*0.016666666666666666666666666666667f);
        moteurGraphique->m_soleil.bleu=(int)(((float)m_lumiere[configuration->heure].bleu*(60-configuration->minute)+((float)m_lumiere[0].bleu*configuration->minute))*0.016666666666666666666666666666667f);
        moteurGraphique->m_soleil.hauteur=((float)m_lumiere[configuration->heure].hauteur*(60-configuration->minute)+((float)m_lumiere[0].hauteur*configuration->minute))*0.016666666666666666666666666666667f;
    }

    moteurGraphique->m_angleOmbreSoleil=((float)configuration->heure*60+configuration->minute)*180/720+180;

}

void Map::AfficherSac(coordonnee positionSac,float decalage,coordonnee position_sac_inventaire,Caracteristique caract)
{
    Sprite Sprite;
    Text texte;

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
                if(eventManager->getPositionSouris().x > AutoScreenAdjust(position_sac_inventaire.x,0).x
                && eventManager->getPositionSouris().x < AutoScreenAdjust(position_sac_inventaire.x + position_sac_inventaire.w * 32,0).x
                && eventManager->getPositionSouris().y > AutoScreenAdjust(0,position_sac_inventaire.y  + (z - m_defilerObjets) * 20).y
                && eventManager->getPositionSouris().y < AutoScreenAdjust(0,position_sac_inventaire.y  + (z - m_defilerObjets) * 20 + 20).y)
                {
                    Sprite.SetImage(*moteurGraphique->getImage(0));
                    Sprite.SetColor(sf::Color(255,255,255,128));
                    Sprite.Resize(position_sac_inventaire.w,20);
                    Sprite.SetPosition(AutoScreenAdjust(position_sac_inventaire.x,
                                                        position_sac_inventaire.y+(z-m_defilerObjets)*20, decalage));

                    moteurGraphique->AjouterCommande(&Sprite,16,0);

                    if (!(z-m_defilerObjets==0&&m_defilerObjets>0)&&!((z-m_defilerObjets==position_sac_inventaire.h-1&&z+m_defilerObjets<=m_decor[1][positionSac.y][positionSac.x].getNombreObjets()+1)))
                    {
                        m_objetPointe=z;
                        m_decor[1][positionSac.y][positionSac.x].getObjet(z)->AfficherCaracteristiques(eventManager->getPositionSouris(),caract,NULL);
                    }
                }

                texte.SetColor(GetItemColor(m_decor[1][positionSac.y][positionSac.x].getObjet(z)->getRarete()));
                texte.SetFont(moteurGraphique->m_font);

                if (z-m_defilerObjets==0&&m_defilerObjets>0)
                    texte.SetString("...");
                else if ((z-m_defilerObjets==position_sac_inventaire.h-1&&z+m_defilerObjets<=m_decor[1][positionSac.y][positionSac.x].getNombreObjets()+1))
                    texte.SetString("...");
                else
                    texte.SetString(m_decor[1][positionSac.y][positionSac.x].getObjet(z)->getNom());
                texte.SetCharacterSize(16);

                texte.SetPosition(AutoScreenAdjust(position_sac_inventaire.x,
                                                   position_sac_inventaire.y+(z-m_defilerObjets)*20, decalage));
                texte.Move(((position_sac_inventaire.w/2))-(texte.GetRect().Right-texte.GetRect().Left)/2,0);

                moteurGraphique->AjouterTexte(&texte,16);
            }
        }
}

void Map::Afficher(Hero *hero,bool alt,float alpha)
{
    coordonnee positionHero;

    Sprite sprite;
    String texte;

    positionHero.y=(int)((hero->m_personnage.getCoordonneePixel().x+hero->m_personnage.getCoordonneePixel().y)*DIVISEUR_COTE_TILE*32);
    positionHero.x=(int)(((hero->m_personnage.getCoordonneePixel().x-hero->m_personnage.getCoordonneePixel().y)*DIVISEUR_COTE_TILE-1)*64);

    coordonnee position;

    if (alpha>0)
    {
        sf::Sprite fond;
        fond.SetImage(*moteurGraphique->getImage(0));
        fond.SetColor(sf::Color(0,0,0,(int)(alpha * 0.25f)));
        fond.Resize(configuration->Resolution.x,configuration->Resolution.y);
        moteurGraphique->AjouterCommande(&fond,12,0);

        for(std::list<sf::Sprite>::iterator i = hero->m_minimap.begin() ; i != hero->m_minimap.end(); ++i)
        {
            sf::Sprite minimap = *i;

            minimap.SetColor(sf::Color(255,255,255,(int)(alpha * 0.5f)));
            minimap.SetPosition(configuration->Resolution.x*0.5f + (minimap.GetPosition().x - positionHero.x) * 0.125f,
                                configuration->Resolution.y*0.5f + (minimap.GetPosition().y - positionHero.y) * 0.125f);

            moteurGraphique->AjouterCommande(&minimap,12,0);
        }

        sf::Sprite minimap;
        minimap.SetImage(*moteurGraphique->getImage(hero->m_classe.icone_mm.image));
        minimap.SetSubRect(sf::IntRect(hero->m_classe.icone_mm.position.x, hero->m_classe.icone_mm.position.y,
                                       hero->m_classe.icone_mm.position.x + hero->m_classe.icone_mm.position.w,
                                       hero->m_classe.icone_mm.position.y + hero->m_classe.icone_mm.position.h));
        minimap.SetColor(sf::Color(255,255,255,(int)(alpha * 0.5f)));
        minimap.SetPosition(configuration->Resolution.x*0.5f ,
                            configuration->Resolution.y*0.5f);
        moteurGraphique->AjouterCommande(&minimap,12,0);

        for(int i = 0 ; i < hero->m_amis.size() ; ++i)
        {
            coordonnee pos;

            pos.y=(int)((hero->m_amis[i]->getCoordonneePixel().x+hero->m_amis[i]->getCoordonneePixel().y)*DIVISEUR_COTE_TILE*32);
            pos.x=(int)(((hero->m_amis[i]->getCoordonneePixel().x-hero->m_amis[i]->getCoordonneePixel().y)*DIVISEUR_COTE_TILE-1)*64);

            minimap.SetColor(sf::Color(255,0,192));
            minimap.SetPosition(configuration->Resolution.x*0.5f + (pos.x - positionHero.x) * 0.125f,
                                configuration->Resolution.y*0.5f + (pos.y - positionHero.y) * 0.125f);
            moteurGraphique->AjouterCommande(&minimap,12,0);
        }

    }

    sf::Sprite sky;
    sky.SetImage(*moteurGraphique->getImage(m_img_sky));
    moteurGraphique->AjouterCommande(&sky,0,0);

    int maxY = hero->m_personnage.getCoordonnee().y + (int)(17 * configuration->zoom);
    int maxX = hero->m_personnage.getCoordonnee().x + (int)(17 * configuration->zoom);

    for (int couche=0;couche<NOMBRE_COUCHE_MAP;couche++)
    {
        for (int j=hero->m_personnage.getCoordonnee().y - (int)(12 * configuration->zoom) ;j<maxY;++j)
        {
            for (int k=hero->m_personnage.getCoordonnee().x - (int)(12 * configuration->zoom) ;k<maxX ;++k)
            {
                if (j>=0&&j<m_dimensions.y&&k>=0&&k<m_dimensions.x)
                {
                    if(m_decor[couche][j][k].m_entite_graphique.m_tileset != NULL)
                    {
                        if (m_decor[couche][j][k].m_entite_graphique.m_tileset->getTransparentDuTile(m_decor[couche][j][k].m_entite_graphique.m_noAnimation))
                        {
                            position.x=(k-j-1)*64+48;
                            position.y=(k+j)*32+16;

                            int alpha=(int)((positionHero.y)-position.y)+160;

                            if (alpha<configuration->alpha)
                                alpha=configuration->alpha;
                            if (alpha>255)
                                alpha=255;

                            m_decor[couche][j][k].m_entite_graphique.m_sprite.SetColor(sf::Color(255,255,255,alpha));
                        }
                        moteurGraphique->AjouterEntiteGraphique(&m_decor[couche][j][k].m_entite_graphique);
                    }

                    if(!m_decor[couche][j][k].added_minimap)
                        if((j-hero->m_personnage.getCoordonnee().y) < 8 && (k-hero->m_personnage.getCoordonnee().x) < 8)
                            if(TileVisible(k,j,hero->m_personnage.getCoordonnee()))
                            {
                                if(m_decor[couche][j][k].m_spriteMinimap.GetSize().x> 1)
                                    hero->m_minimap.push_back(m_decor[couche][j][k].m_spriteMinimap);
                                m_decor[couche][j][k].added_minimap = true;
                            }

                    if (couche==1)
                    {
                        if(configuration->Herbes)
                            moteurGraphique->AjouterEntiteGraphique(&m_decor[0][j][k].m_entite_herbe);

                        for (unsigned o = 0 ; o < m_decor[1][j][k].getMonstre().size() ; ++o)
                            if (m_decor[1][j][k].getMonstre()[o]>=0&&m_decor[1][j][k].getMonstre()[o]<(int)m_monstre.size())
                                m_monstre[m_decor[1][j][k].getMonstre()[o]].Afficher(getDimensions(),&m_ModeleMonstre[m_monstre[m_decor[1][j][k].getMonstre()[o]].getModele()],m_decor[1][j][k].getMonstre()[o]==m_monstreIllumine);

                        if ((int)((hero->m_personnage.getCoordonneePixel().x + COTE_TILE * 0.5f) / COTE_TILE) == k
                        &&  (int)((hero->m_personnage.getCoordonneePixel().y + COTE_TILE * 0.5f) / COTE_TILE) == j)
                                hero->Afficher(getDimensions());

                        if(configuration->Herbes)
                            moteurGraphique->AjouterEntiteGraphique(&m_decor[1][j][k].m_entite_herbe);

                        for (unsigned o = 0 ; o < m_decor[1][j][k].getProjectile().size() ; ++o)
                            if (m_decor[1][j][k].getProjectile()[o]>=0&&m_decor[1][j][k].getProjectile()[o]<(int)m_projectile.size())
                                m_projectile[m_decor[1][j][k].getProjectile()[o]].Afficher();

                        for (unsigned o = 0 ; o < m_decor[1][j][k].getEffetGraphique().size() ; ++o)
                            if (m_decor[1][j][k].getEffetGraphique()[o]>=0&&m_decor[1][j][k].getEffetGraphique()[o]<(int)m_effets.size())
                                m_effets[m_decor[1][j][k].getEffetGraphique()[o]].Afficher();


                        if (m_decor[1][j][k].getNombreObjets()>0&&couche==1)
                        {
                            position.x=(k-j-1)*64+48;
                            position.y=(k+j)*32+16;

                            if (m_decor[1][j][k].getNombreObjets()<=4)
                            {
                                for (int o=0;o<m_decor[1][j][k].getNombreObjets();o++)
                                {
                                    sprite.SetImage(*moteurGraphique->getImage(m_decor[1][j][k].getObjet(o)->getImage()));
                                    sprite.SetSubRect(IntRect(m_decor[1][j][k].getObjet(o)->getPositionImage().x, m_decor[1][j][k].getObjet(o)->getPositionImage().y, m_decor[1][j][k].getObjet(o)->getPositionImage().x+m_decor[1][j][k].getObjet(o)->getPositionImage().w, m_decor[1][j][k].getObjet(o)->getPositionImage().y+m_decor[1][j][k].getObjet(o)->getPositionImage().h));
                                    sprite.SetScale(0.8f,0.4f);

                                    sprite.SetX(position.x-32+m_decor[1][j][k].getObjet(o)->getPosition().x*32+16-(m_decor[1][j][k].getObjet(o)->getPositionImage().w*0.8f)/2);
                                    sprite.SetY(position.y+m_decor[1][j][k].getObjet(o)->getPosition().y*32);

                                    //sprite.SetColor(m_decor[1][j][k].getObjet(o)->m_color);

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

                                if (eventManager->getCasePointee().x==k&&eventManager->getCasePointee().y==j&&m_monstreIllumine<0&&m_decor[1][j][k].getNombreObjets()>4&&!alt&&m_monstreIllumine<0)
                                    sprite.SetColor(sf::Color(255,128,128));
                                else
                                    sprite.SetColor(sf::Color(255,255,255));

                                moteurGraphique->AjouterCommande(&sprite,8,1);
                            }

                            sprite.SetScale(1,1);
                            sprite.SetColor(sf::Color(255,255,255));

                            int objetPointe=-1;

                            if (alt)
                                m_decor[1][j][k].AlphaObjets(255);

                            //position.x = position.x + (configuration->Resolution.x - 800) * 0.5;
                            //position.y = position.y + configuration->Resolution.y - 600;
                            objetPointe=m_decor[1][j][k].AfficherTexteObjets(position,m_objetPointe);

                            if (objetPointe>=0&&!eventManager->getEvenement(sf::Mouse::Left,EventClicA) && alt)
                            {
                                m_sacPointe.x=k;
                                m_sacPointe.y=j;

                                if (eventManager->getEvenement(sf::Key::LControl,EventKey))
                                    m_decor[1][j][k].getObjet(objetPointe)->AfficherCaracteristiques(eventManager->getPositionSouris(),hero->m_caracteristiques,&hero->m_coffre);

                                m_objetPointe=objetPointe;
                            }
                        }
                    }
                }
                else if (couche==0)
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

                    sf::Sprite buffer=m_decor[couche][w][z].m_entite_graphique.m_sprite;

                    buffer.SetX((k-j)*64);
                    buffer.SetY((k+j+1)*32);

                    if (buffer.GetSize().x>0)
                        if (buffer.GetPosition().x+buffer.GetSize().x-buffer.GetOrigin().x>=GetViewRect(moteurGraphique->m_camera).Left
                                &&buffer.GetPosition().x-buffer.GetOrigin().x<GetViewRect(moteurGraphique->m_camera).Right
                                &&buffer.GetPosition().y+buffer.GetSize().y-buffer.GetOrigin().y>=GetViewRect(moteurGraphique->m_camera).Top
                                &&buffer.GetPosition().y-buffer.GetOrigin().y<GetViewRect(moteurGraphique->m_camera).Bottom)
                            moteurGraphique->AjouterCommande(&buffer,m_decor[couche][w][z].getCouche(),1);
                }
            }
        }
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

void Map::AfficherNomEvenement(coordonnee casePointee,coordonnee positionSouris)
{
    int evenement=-1;
    for (int i=0;i<2;++i)
        for (int z=0;z<(int)m_decor[i][casePointee.y][casePointee.x].getEvenement().size();z++)
            if (m_decor[i][casePointee.y][casePointee.x].getEvenement()[z]>-1)
            {
                evenement=m_decor[i][casePointee.y][casePointee.x].getEvenement()[z];

                if (evenement>=0)
                {
                    if (m_evenement[evenement].getType()==CHANGEMENT_DE_MAP)
                    {
                        string nom;

                        cDAT reader;
                        reader.Read(configuration->chemin_maps);

                        ifstream *fichier=reader.GetInfos(m_evenement[evenement].getString());
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
                                            nom = configuration->getText(5, no);
                                        }
                                    }
                                    while (caractere!='$');
                                    fichier->get(caractere);
                                }
                            }
                            while (caractere!='$');
                        }
                        fichier->close();

                        delete fichier;

                        sf::Text texte;
                        texte.SetString(nom);
                        texte.SetCharacterSize(16);
                        if (configuration->Resolution.y>0)
                            texte.SetY((positionSouris.y-16)*configuration->Resolution.h/configuration->Resolution.y);
                        if (configuration->Resolution.x>0)
                            texte.SetX(positionSouris.x*configuration->Resolution.w/configuration->Resolution.x);
                        moteurGraphique->AjouterTexte(&texte,15);
                    }
                }
            }
}

bool Map::TestEvenement(Jeu *jeu,float temps)
{
    for (int i=0;i<2;++i)
        for (int z=0;z<(int)m_decor[i][jeu->hero.m_personnage.getCoordonnee().y][jeu->hero.m_personnage.getCoordonnee().x].getEvenement().size();z++)
            if (m_decor[i][jeu->hero.m_personnage.getCoordonnee().y][jeu->hero.m_personnage.getCoordonnee().x].getEvenement()[z]>=0)
            {
                if (m_evenement[m_decor[i][jeu->hero.m_personnage.getCoordonnee().y][jeu->hero.m_personnage.getCoordonnee().x].getEvenement()[z]].getType()==CHANGEMENT_DE_MAP)
                {
                    string nomMap=m_evenement[m_decor[i][jeu->hero.m_personnage.getCoordonnee().y][jeu->hero.m_personnage.getCoordonnee().x].getEvenement()[z]].getString();

                    console->Ajouter("",0);
                    console->Ajouter("---------------------------------------------------------------------------------",0);
                    console->Ajouter("EVENEMENT : Changement de map",0);
                    console->Ajouter("---------------------------------------------------------------------------------",0);

                    coordonnee coordonneePerso;
                    coordonneePerso.x=m_evenement[m_decor[i][jeu->hero.m_personnage.getCoordonnee().y][jeu->hero.m_personnage.getCoordonnee().x].getEvenement()[z]].getInformation(0);
                    coordonneePerso.y=m_evenement[m_decor[i][jeu->hero.m_personnage.getCoordonnee().y][jeu->hero.m_personnage.getCoordonnee().x].getEvenement()[z]].getInformation(1);

                    sf::Clock Clock;
                    Clock.Reset();

                    jeu->m_chargement->setC_Chargement(nomMap,coordonneePerso);
                    jeu->m_contexte = jeu->m_chargement;
                }

                if (m_evenement[m_decor[i][jeu->hero.m_personnage.getCoordonnee().y][jeu->hero.m_personnage.getCoordonnee().x].getEvenement()[z]].getType()==INFLIGER_DEGATS)
                {
                    jeu->hero.m_personnage.InfligerDegats(m_evenement[m_decor[i][jeu->hero.m_personnage.getCoordonnee().y][jeu->hero.m_personnage.getCoordonnee().x].getEvenement()[z]].getInformation(0)*temps*50,
                                                          m_evenement[m_decor[i][jeu->hero.m_personnage.getCoordonnee().y][jeu->hero.m_personnage.getCoordonnee().x].getEvenement()[z]].getInformation(1), NULL);
                }
            }
    return 1;
}

int Map::AjouterProjectile(coordonneeDecimal positionReel,coordonnee cible,coordonnee lanceur,int couche,float  vitesse,float decalageAngle,bool monstre,Tileset *tileset)
{
    m_projectile.push_back(Projectile ());

    m_projectile.back().m_position.x = positionReel.x;
    m_projectile.back().m_position.y = positionReel.y;
    m_projectile.back().m_position.h=0;

    m_projectile.back().m_depart = lanceur;

    m_projectile.back().m_monstre=monstre;

    m_projectile.back().m_actif     = true;
    m_projectile.back().m_supprime  = false;

    m_projectile.back().m_effet.m_couche = couche;

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

    position.x=(positionReel.x-positionReel.y) * 64 / COTE_TILE;
    position.y=(positionReel.x+positionReel.y) * 64 / COTE_TILE;

    m_projectile.back().m_effet.m_tileset = tileset;
    m_projectile.back().m_effet.Initialiser(position);

    if (couche>=0&&couche<2)
        if (lanceur.y>=0&&lanceur.y<m_dimensions.y)
            if (lanceur.x>=0&&lanceur.x<m_dimensions.x)
            {
                m_projectile.back().m_positionCase.x=lanceur.x;
                m_projectile.back().m_positionCase.y=lanceur.y;
                m_decor[1][lanceur.y][lanceur.x].setProjectile(m_projectile.size()-1);
            }

    return m_projectile.size()-1;
}

void Map::Animer(Hero *hero,float temps,Menu *menu)
{
    coordonnee positionHero;
    positionHero.x=(hero->m_personnage.getCoordonnee().x-hero->m_personnage.getCoordonnee().y-1)/5;
    positionHero.y=(hero->m_personnage.getCoordonnee().x+hero->m_personnage.getCoordonnee().y)/5;

    coordonnee vueMin,vueMax;

    vueMin.x=hero->m_personnage.getCoordonnee().x-10;
    vueMin.y=hero->m_personnage.getCoordonnee().y-10;
    vueMax.x=hero->m_personnage.getCoordonnee().x+10;
    vueMax.y=hero->m_personnage.getCoordonnee().y+10;

    if (vueMin.x<0)
        vueMin.x=0;
    if (vueMin.y<0)
        vueMin.y=0;
    if (vueMax.x>m_dimensions.x)
        vueMax.x=m_dimensions.x;
    if (vueMax.y>m_dimensions.y)
        vueMax.y=m_dimensions.y;

    for (int i=0;i<2;++i)
        for (int j=vueMin.y;j<vueMax.y;j++)
            for (int k=vueMin.x;k<vueMax.x;k++)
            {
                for (int z=0;z<(int)m_decor[i][j][k].getEvenement().size();z++)
                    if (m_decor[i][j][k].getEvenement()[z]>=0&&m_decor[i][j][k].getEvenement()[z]<(int)m_evenement.size())
                        if (m_evenement[m_decor[i][j][k].getEvenement()[z]].getType()==TIMER)
                        {
                            m_evenement[m_decor[i][j][k].getEvenement()[z]].setInformation((int)((float)m_evenement[m_decor[i][j][k].getEvenement()[z]].getInformation(1)+(float)temps*1000),1);

                            if (m_evenement[m_decor[i][j][k].getEvenement()[z]].getInformation(1)>=m_evenement[m_decor[i][j][k].getEvenement()[z]].getInformation(0))
                            {
                                m_decor[i][j][k].setEvenement(m_evenement[m_decor[i][j][k].getEvenement()[z]].getInformation(2),z);
                                GererEvenements(m_decor[i][j][k].getEvenement()[z],z,i,k,j);
                            }
                        }

                m_decor[i][j][k].m_entite_graphique.Animer(temps);

                //if(i == 1)
                   // m_decor[i][j][k].m_entite_graphique.m_sprite.Move(0,-m_decor[0][j][k].getHauteur());
                //m_decor[i][j][k].m_entite_herbe.Animer(temps);

                for(int z = 0; z < m_decor[i][j][k].getNombreObjets() ; ++z)
                {
                    m_decor[i][j][k].getObjet(z)->m_alpha -= temps*200;
                    if(m_decor[i][j][k].getObjet(z)->m_alpha < 0)
                        m_decor[i][j][k].getObjet(z)->m_alpha = 0;
                }

                for (int z=0;z<(int)m_decor[i][j][k].getMonstre().size();z++)
                {
                    int monstre=m_decor[i][j][k].getMonstre()[z];
                    if (monstre>=0&&monstre<(int)m_monstre.size())
                    {
                        m_monstre[monstre].m_vientDeFrapper = NULL;
                        m_monstre[monstre].m_degatsInflige  = 0;

                        int degats = m_monstre[monstre].Animer(&m_ModeleMonstre[m_monstre[monstre].getModele()],temps);
                        if (degats>0)
                        {
                            if (m_monstre[monstre].m_miracleALancer == -1)
                            {
                                if (m_monstre[monstre].m_cible != NULL)
                                {
                                    if (m_monstre[monstre].m_shooter||!m_monstre[monstre].m_shooter&&fabs(m_monstre[monstre].getCoordonnee().x-m_monstre[monstre].m_cible->getCoordonnee().x)<=1&&fabs(m_monstre[monstre].getCoordonnee().y-m_monstre[monstre].m_cible->getCoordonnee().y)<=1)
                                        if (rand() % 100 < (float)((float)(m_monstre[monstre].getCaracteristique().dexterite + 100)/(float)(m_monstre[monstre].m_cible->getCaracteristique().dexterite + 100))*25 )
                                        {
                                            m_monstre[monstre].m_vientDeFrapper = m_monstre[monstre].m_cible;
                                            m_monstre[monstre].m_degatsInflige  = degats;

                                            m_monstre[monstre].m_cible->m_vientDetreTouche = &m_monstre[monstre];
                                            InfligerDegats(m_monstre[monstre].m_cible, degats, 0, hero, 0);

                                            m_monstre[monstre].InfligerDegats(-degats * m_monstre[monstre].getCaracteristique().volVie, 4, NULL);
                                        }
                                }
                            }
                            else
                            {
                                m_monstre[monstre].m_miracleEnCours.push_back(EntiteMiracle ());
                                m_monstre[monstre].m_miracleEnCours.back().m_infos.push_back(new InfosEntiteMiracle ());

                                m_monstre[monstre].m_miracleEnCours.back().m_modele=m_monstre[monstre].m_miracleALancer;

                                m_monstre[monstre].m_miracleALancer = -1;

                                m_monstre[monstre].m_miracleEnCours.back().m_infos.back()->m_position.x=m_monstre[monstre].getCoordonneePixel().x;
                                m_monstre[monstre].m_miracleEnCours.back().m_infos.back()->m_position.y=m_monstre[monstre].getCoordonneePixel().y;

                                m_monstre[monstre].m_miracleEnCours.back().m_coordonneeCible = hero->m_personnage.getProchaineCase();
                                m_monstre[monstre].m_miracleEnCours.back().m_infos.back()->m_cible = m_monstre[monstre].m_cible;
                            }

                        }
                        m_monstre[monstre].m_nombreInvocation=0;

                        if(configuration->Lumiere)
                            moteurGraphique->LightManager->Generate(m_monstre[monstre].m_entite_graphique.m_light);

                        GererMiracle(&m_monstre[monstre],m_ModeleMonstre[m_monstre[monstre].getModele()].m_miracles,temps,positionHero,hero);

                        if(m_monstre[monstre].m_doitMourir && m_monstre[monstre].EnVie() )
                            InfligerDegats(&m_monstre[monstre], m_monstre[monstre].getCaracteristique().vie, 4, hero, 0);
                    }
                }
            }
}

bool Map::Miracle_Aura           (Hero *hero, Personnage *personnage, Miracle &modele, Effet &effet, EntiteMiracle &miracleEnCours, InfosEntiteMiracle &info, float temps, int o)
{
    if (info.m_IDObjet == -1)
    {
        info.m_IDObjet = 1;

        miracleEnCours.m_infos.push_back(new InfosEntiteMiracle ());
        miracleEnCours.m_infos.back()->m_effetEnCours    = effet.m_informations[0];
        miracleEnCours.m_infos.back()->m_position        = info.m_position;
        miracleEnCours.m_infos.back()->m_cible           = info.m_cible;
    }
    else
        info.m_position = personnage->getCoordonneePixel();

    return 1;
}

bool Map::Miracle_Declencheur    (Hero *hero, Personnage *personnage, Miracle &modele, Effet &effet, EntiteMiracle &miracleEnCours, InfosEntiteMiracle &info, float temps, int o)
{
    if(effet.m_informations[0] == D_PERMANENT)
    {
        info.m_informations[4] += temps * 100;
        if (info.m_informations[4] > effet.m_informations[4])
        {
            if (effet.m_informations[3] >= 0)
            {
                miracleEnCours.m_infos.push_back(new InfosEntiteMiracle ());
                miracleEnCours.m_infos.back()->m_effetEnCours    = effet.m_informations[3];
                miracleEnCours.m_infos.back()->m_position        = info.m_position;

                if(effet.m_informations[2] == 0)
                    miracleEnCours.m_infos.back()->m_cible = info.m_cible;
                else
                    miracleEnCours.m_infos.back()->m_cible = personnage;
            }
            info.m_informations[4] = 0;
        }
    }
    if(effet.m_informations[0] == D_FRAPPE)
    {
        if(personnage->m_vientDeFrapper != NULL)
        {
            if (effet.m_informations[3] >= 0)
            {
                miracleEnCours.m_infos.push_back(new InfosEntiteMiracle ());
                miracleEnCours.m_infos.back()->m_effetEnCours    = effet.m_informations[3];

                miracleEnCours.m_infos.back()->m_position.x      = personnage->getCoordonneePixel().x/*+cos(-(personnage->getAngle()+22.5)*M_PI/180)*96*/;
                miracleEnCours.m_infos.back()->m_position.y      = personnage->getCoordonneePixel().y/*+sin(-(personnage->getAngle()+22.5)*M_PI/180)*96*/;

                if(effet.m_informations[2] == 0)
                    miracleEnCours.m_infos.back()->m_cible = personnage->m_vientDeFrapper;
                else
                    miracleEnCours.m_infos.back()->m_cible = personnage;
            }
        }
    }
    if(effet.m_informations[0] == D_TOUCHE)
    {
        if(personnage->m_vientDetreTouche != NULL)
        {
            if (effet.m_informations[3] >= 0)
            {
                miracleEnCours.m_infos.push_back(new InfosEntiteMiracle ());
                miracleEnCours.m_infos.back()->m_effetEnCours    = effet.m_informations[3];
                miracleEnCours.m_infos.back()->m_position        = info.m_position;

                if(effet.m_informations[2] == 0)
                    miracleEnCours.m_infos.back()->m_cible = personnage->m_vientDetreTouche;
                else
                    miracleEnCours.m_infos.back()->m_cible = personnage;
            }
        }
    }

    if(effet.m_informations[1] != -100)
    {
        info.m_informations[3] += temps * 100;
        if (info.m_informations[3] > effet.m_informations[1])
        {

            for (int p=0;p<(int)effet.m_lien.size();p++)
            {
                miracleEnCours.m_infos.push_back(new InfosEntiteMiracle ());
                miracleEnCours.m_infos.back()->m_effetEnCours    = effet.m_lien[p];
                miracleEnCours.m_infos.back()->m_position        = info.m_position;
                miracleEnCours.m_infos.back()->m_cible           = info.m_cible;
            }

            miracleEnCours.m_infos.erase(miracleEnCours.m_infos.begin() + o);

            return 0;
        }
    }

    return 1;
}

bool Map::Miracle_Effet  (Hero *hero, Personnage *personnage, Miracle &modele, Effet &effet, EntiteMiracle &miracleEnCours, InfosEntiteMiracle &info, float temps, int o)
{
    if(info.m_IDObjet == -1)
    {
        if (effet.m_informations[0])
        {
            info.m_IDObjet = personnage->AjouterEffet(
                moteurGraphique->getTileset(modele.m_tileset[effet.m_sequence]),
                effet.m_informations[1], // TYPE
                effet.m_informations[2], // TEMPS
                effet.m_informations[3], // INFOS
                effet.m_informations[4],
                effet.m_informations[5]);
            info.m_cible = personnage;
        }
        else if(info.m_cible != NULL)
        {
            info.m_IDObjet = info.m_cible->AjouterEffet(
                moteurGraphique->getTileset(modele.m_tileset[effet.m_sequence]),
                effet.m_informations[1], // TYPE
                effet.m_informations[2], // TEMPS
                effet.m_informations[3], // INFOS
                effet.m_informations[4],
                effet.m_informations[5]);

            info.m_position = info.m_cible->getCoordonneePixel();
        }
        else
        {
            miracleEnCours.m_infos.erase(miracleEnCours.m_infos.begin()+o);

            return 0;
        }


        for (int p=0;p<(int)effet.m_lien.size();p++)
        {
            miracleEnCours.m_infos.push_back(new InfosEntiteMiracle ());
            miracleEnCours.m_infos.back()->m_effetEnCours    = effet.m_lien[p];
            miracleEnCours.m_infos.back()->m_position        = info.m_position;
            miracleEnCours.m_infos.back()->m_cible           = info.m_cible;
        }
    }
    else
    {
        Personnage *temp;
        if (effet.m_informations[0])
            temp = personnage;
        else
            temp = info.m_cible;

        if(!temp->m_effets[info.m_IDObjet].m_effet.m_actif)
        {
            miracleEnCours.m_infos.erase(miracleEnCours.m_infos.begin()+o);
            return 0;
        }
    }

    return 1;
}

bool Map::Miracle_Charme (Hero *hero, Personnage *personnage, Miracle &modele, Effet &effet, EntiteMiracle &miracleEnCours, InfosEntiteMiracle &info, float temps, int o)
{
    if(info.m_IDObjet == -1)
    {
        if (info.m_cible->m_friendly != personnage->m_friendly
            &&info.m_cible->getCaracteristique().niveau > 0 && info.m_cible->getCaracteristique().vitesse > 0)
        {
            info.m_cible->m_scriptAI     = Script (effet.m_chaine);

            info.m_cible->m_friendly     = true;

            info.m_cible->m_cible        = NULL;
            info.m_cible->frappeEnCours  = false;
            info.m_cible->setEtat(0);

            info.m_IDObjet    = 1;

            if(personnage == &hero->m_personnage)
                hero->m_amis.push_back(info.m_cible);

            for (int p=0;p<(int)effet.m_lien.size();p++)
            {
                miracleEnCours.m_infos.push_back(new InfosEntiteMiracle ());
                miracleEnCours.m_infos.back()->m_effetEnCours=effet.m_lien[p];
                miracleEnCours.m_infos.back()->m_position=info.m_position;
                miracleEnCours.m_infos.back()->m_cible=info.m_cible;
            }

            return 0;
        }
        else
        {
            miracleEnCours.m_infos.erase(miracleEnCours.m_infos.begin()+o);
            return 0;
        }
    }
    else
    {
        Lumiere temp;
        temp.rouge = 255;
        temp.vert = 0;
        temp.bleu = 255;
        temp.intensite = 255;
        info.m_cible->setPorteeLumineuse(temp);

        if (!info.m_cible->EnVie())
        {
            if(personnage == &hero->m_personnage)
                for(unsigned o = 0 ; o < hero->m_amis.size() ;  ++o)
                    if(hero->m_amis[o] == info.m_cible)
                        hero->m_amis.erase(hero->m_amis.begin() + o);

            miracleEnCours.m_infos.erase(miracleEnCours.m_infos.begin()+o);

            return 0;
        }

        if (fabs(info.m_cible->getCoordonnee().x - personnage->getCoordonnee().x) > 10 || fabs(info.m_cible->getCoordonnee().y - personnage->getCoordonnee().y) > 10)
        {
            coordonnee temp;
            bool ok = false;
            int id = -1;
            for(int i = 0 ; i < m_monstre.size() ; ++i)
                if(&m_monstre[i] == info.m_cible)
                    id = i, m_decor[1][m_monstre[i].getCoordonnee().y][m_monstre[i].getCoordonnee().x].delMonstre(i);

           /* temp.x = personnage->getCoordonnee().x;
            temp.y = personnage->getCoordonnee().y;
            if(!getCollision(temp.x, temp.y))
                info.m_cible->setCoordonnee(temp);*/
            temp.x = personnage->getCoordonnee().x - 1;
            temp.y = personnage->getCoordonnee().y - 1;
            if(!getCollision(temp.x, temp.y))
                info.m_cible->setCoordonnee(temp), ok = true;
            temp.x = personnage->getCoordonnee().x - 1;
            temp.y = personnage->getCoordonnee().y;
            if(!getCollision(temp.x, temp.y))
                info.m_cible->setCoordonnee(temp), ok = true;
            temp.x = personnage->getCoordonnee().x - 1;
            temp.y = personnage->getCoordonnee().y + 1;
            if(!getCollision(temp.x, temp.y))
                info.m_cible->setCoordonnee(temp), ok = true;
            temp.x = personnage->getCoordonnee().x;
            temp.y = personnage->getCoordonnee().y - 1;
            if(!getCollision(temp.x, temp.y))
                info.m_cible->setCoordonnee(temp), ok = true;
            temp.x = personnage->getCoordonnee().x;
            temp.y = personnage->getCoordonnee().y + 1;
            if(!getCollision(temp.x, temp.y))
                info.m_cible->setCoordonnee(temp), ok = true;
            temp.x = personnage->getCoordonnee().x + 1;
            temp.y = personnage->getCoordonnee().y - 1;
            if(!getCollision(temp.x, temp.y))
                info.m_cible->setCoordonnee(temp), ok = true;
            temp.x = personnage->getCoordonnee().x + 1;
            temp.y = personnage->getCoordonnee().y + 1;
            if(!getCollision(temp.x, temp.y))
                info.m_cible->setCoordonnee(temp), ok = true;


            if(ok)
            {
                m_decor[1][info.m_cible->getCoordonnee().y][info.m_cible->getCoordonnee().x].setMonstre(id);

                info.m_cible->setDepart();
                info.m_cible->frappeEnCours = false;
            }
        }

        if (!info.m_cible->m_friendly)
        {
            info.m_cible->DetruireEffets();
            info.m_cible->m_scriptAI = m_ModeleMonstre[info.m_cible->getModele()].m_scriptAI;
            info.m_cible->setDepart();

            Lumiere temp;
            temp = m_ModeleMonstre[info.m_cible->getModele()].getPorteeLumineuse();
            info.m_cible->setPorteeLumineuse(temp);

            miracleEnCours.m_infos.erase(miracleEnCours.m_infos.begin()+o);

            return 0;
        }
    }

    return 1;
}

bool Map::Miracle_CorpsACorps (Hero *hero, Personnage *personnage, Miracle &modele, Effet &effet, EntiteMiracle &miracleEnCours, InfosEntiteMiracle &info, float temps, int o)
{
    if (info.m_cible != NULL)
    {
        if (fabs(info.m_cible->getCoordonnee().x - personnage->getCoordonnee().x) > effet.m_informations[0]
         || fabs(info.m_cible->getCoordonnee().y - personnage->getCoordonnee().y) > effet.m_informations[0] )
            personnage->setArrivee(info.m_cible->getCoordonnee());
        else
        {
            personnage->setArrivee(personnage->getProchaineCase());
            info.m_position.x = (float)personnage->getProchaineCase().x * COTE_TILE + 1;
            info.m_position.y = (float)personnage->getProchaineCase().y * COTE_TILE + 1;

            if( personnage->getArrivee().x == personnage->getCoordonnee().x
             && personnage->getArrivee().y == personnage->getCoordonnee().y)
            {
                personnage->Frappe(personnage->getCoordonneePixel(),info.m_cible->getCoordonneePixel());

                if (info.m_effetEnCours == 0)
                {
                    Caracteristique temp = personnage->getCaracteristique();
                    temp.foi        -= modele.m_coutFoi + modele.m_reserveFoi;
                    temp.vie        -= modele.m_coutVie + modele.m_reserveVie;
                    temp.reserveFoi += modele.m_reserveFoi;
                    temp.reserveVie += modele.m_reserveVie;
                    personnage->setCaracteristique(temp);

                    miracleEnCours.m_dejaConsommeFoi = true;
                }

                for (int p=0;p<(int)effet.m_lien.size();p++)
                {
                    miracleEnCours.m_infos.push_back(new InfosEntiteMiracle ());
                    miracleEnCours.m_infos.back()->m_effetEnCours    =   effet.m_lien[p];
                    miracleEnCours.m_infos.back()->m_position        =   info.m_position;
                    miracleEnCours.m_infos.back()->m_cible           =   info.m_cible;
                }

                miracleEnCours.m_infos.erase(miracleEnCours.m_infos.begin()+o);

                return 0;
            }
        }
    }
    else
        miracleEnCours.m_infos.erase(miracleEnCours.m_infos.begin()+o);

    return 1;
}

bool Map::Miracle_Repetition(Hero *hero, Personnage *personnage, Miracle &modele, Effet &effet, EntiteMiracle &miracleEnCours, InfosEntiteMiracle &info, float temps, int o)
{
    info.m_informations[3] += temps * 100;
    if (info.m_informations[3] > effet.m_informations[2])
    {
        if (effet.m_informations[0] >= 0)
        {
            miracleEnCours.m_infos.push_back(new InfosEntiteMiracle ());
            miracleEnCours.m_infos.back()->m_effetEnCours    = effet.m_informations[0];
            miracleEnCours.m_infos.back()->m_position        = info.m_position;
            miracleEnCours.m_infos.back()->m_cible           = info.m_cible;
        }

        info.m_informations[1]--;
        info.m_informations[3] = 0;
    }

    if (-info.m_informations[1] >= effet.m_informations[1])
    {
        for (int p=0;p<(int)effet.m_lien.size();p++)
        {
            miracleEnCours.m_infos.push_back(new InfosEntiteMiracle ());
            miracleEnCours.m_infos.back()->m_effetEnCours    = effet.m_lien[p];
            miracleEnCours.m_infos.back()->m_position        = info.m_position;
            miracleEnCours.m_infos.back()->m_cible           = info.m_cible;
        }

        miracleEnCours.m_infos.erase(miracleEnCours.m_infos.begin()+o);

        return 0;
    }

    return 1;
}

bool Map::Miracle_Projectile(Hero *hero, Personnage *personnage, Miracle &modele, Effet &effet, EntiteMiracle &miracleEnCours, InfosEntiteMiracle &info, float temps, int o)
{
    if (info.m_IDObjet >= 0 && info.m_IDObjet < (int)m_projectile.size())
    {
        if (!m_projectile[info.m_IDObjet].m_effet.m_actif)
            m_projectile[info.m_IDObjet].m_actif = false;

        if (m_projectile[info.m_IDObjet].m_actif)
            m_projectile[info.m_IDObjet].m_effet.Animer(temps);
        else
        {
            m_projectile[info.m_IDObjet].m_supprime = true;
            info.m_cible = m_projectile[info.m_IDObjet].m_entite_cible;

            for (int p=0;p<(int)effet.m_lien.size();p++)
            {
                miracleEnCours.m_infos.push_back(new InfosEntiteMiracle ());
                miracleEnCours.m_infos.back()->m_effetEnCours    = effet.m_lien[p];
                miracleEnCours.m_infos.back()->m_position        = m_projectile[info.m_IDObjet].m_position;
                miracleEnCours.m_infos.back()->m_cible           = info.m_cible;
            }

            miracleEnCours.m_infos.erase(miracleEnCours.m_infos.begin()+o);

            return 0;
        }
    }
    else
    {
        coordonnee pos( (int)((float)info.m_position.x/COTE_TILE),
                        (int)((float)info.m_position.y/COTE_TILE));

        coordonnee cible;

        if (info.m_cible != NULL)
            cible = info.m_cible->getProchaineCase();
        else
            cible = miracleEnCours.m_coordonneeCible;


        info.m_IDObjet = AjouterProjectile( info.m_position,
                                            cible,personnage->getCoordonnee(),1,effet.m_informations[0],
                                            (float)effet.m_informations[1]*M_PI/180,!personnage->m_friendly,
                                            moteurGraphique->getTileset(modele.m_tileset[effet.m_sequence]));

        if (effet.m_informations[1])
            m_projectile.back().m_cible = cible;
        else
            m_projectile.back().m_cible = coordonnee (-1, -1);
    }

    return 1;
}

bool Map::Miracle_EffetGraphique(Hero *hero, Personnage *personnage, Miracle &modele, Effet &effet, EntiteMiracle &miracleEnCours, InfosEntiteMiracle &info, float temps, int o)
{
    if (info.m_IDObjet >= 0 && info.m_IDObjet < (int)m_effets.size())
    {
        m_effets[info.m_IDObjet].Animer(temps);
        if (!m_effets[info.m_IDObjet].m_actif)
        {
            for (int p=0;p<(int)effet.m_lien.size();p++)
            {
                miracleEnCours.m_infos.push_back(new InfosEntiteMiracle ());
                miracleEnCours.m_infos.back()->m_effetEnCours    = effet.m_lien[p];
                miracleEnCours.m_infos.back()->m_position        = info.m_position;
                miracleEnCours.m_infos.back()->m_cible           = info.m_cible;
            }

            miracleEnCours.m_infos.erase(miracleEnCours.m_infos.begin()+o);

            return 0;
        }
    }
    else
    {
        m_effets.push_back(EffetGraphique ());

        m_effets.back().m_position.x = (int)info.m_position.x;
        m_effets.back().m_position.y = (int)info.m_position.y;

        m_effets.back().m_compteur   = effet.m_informations[0];

        m_effets.back().m_actif      = true;

        m_effets.back().m_tileset    = moteurGraphique->getTileset(modele.m_tileset[effet.m_sequence]);
        m_effets.back().m_couche     = 10;
        m_effets.back().Initialiser(coordonnee((int)(((float)info.m_position.x - (float)info.m_position.y) * 64 / COTE_TILE),
                                               (int)(((float)info.m_position.x + (float)info.m_position.y) * 64 / COTE_TILE)));

        info.m_IDObjet               = m_effets.size()-1;

        if (m_effets.back().m_position.y / COTE_TILE>=0&&m_effets.back().m_position.y / COTE_TILE<m_dimensions.y)
            if (m_effets.back().m_position.x / COTE_TILE>=0&&m_effets.back().m_position.x / COTE_TILE<m_dimensions.x)
                m_decor[1][(int)(m_effets.back().m_position.y / COTE_TILE)][(int)(m_effets.back().m_position.x / COTE_TILE)].setEffetGraphique(m_effets.size()-1);
    }
    return 1;
}

bool Map::Miracle_Invocation(Hero *hero, Personnage *personnage, Miracle &modele, Effet &effet, EntiteMiracle &miracleEnCours, InfosEntiteMiracle &info, float temps, int o)
{
    if (info.m_IDObjet==-1)
    {
        bool invoquer=true;
        int x=0,y=0;

        coordonnee cible;

        if(effet.m_informations[0] == 1)
        {
            cible.x = (int)(info.m_position.x/COTE_TILE);
            cible.y = (int)(info.m_position.y/COTE_TILE);
        }
        else
        {
            if (info.m_cible != NULL)
                cible = info.m_cible->getProchaineCase();
            else
                cible = miracleEnCours.m_coordonneeCible;
        }

        coordonnee positionCase;
        positionCase.x = cible.x-1;
        positionCase.y = cible.y-1;

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
                invoquer = false;
        }

        if(effet.m_informations[0] == 1)
        {
            positionCase.x = (int)(info.m_position.x/COTE_TILE);
            positionCase.y = (int)(info.m_position.y/COTE_TILE);
        }

        if (invoquer)
        {
            for (unsigned p=0;p < effet.m_lien.size();p++)
            {
                miracleEnCours.m_infos.push_back(new InfosEntiteMiracle ());
                miracleEnCours.m_infos.back()->m_effetEnCours    = effet.m_lien[p];
                miracleEnCours.m_infos.back()->m_position        = info.m_position;
                miracleEnCours.m_infos.back()->m_cible           = info.m_cible;
            }

            info.m_position.x = positionCase.x*COTE_TILE;
            info.m_position.y = positionCase.y*COTE_TILE;



            int numero=-1;
            for (int j=0;j<(int)m_ModeleMonstre.size();j++)
                if (effet.m_chaine==m_ModeleMonstre[j].m_chemin)
                {
                    numero=j;
                    break;
                }

            if (numero==-1)
            {
                m_ModeleMonstre.push_back(Modele_Monstre ());
                m_ModeleMonstre.back().Charger(effet.m_chaine);
                console->Ajouter("Chargement de : "+effet.m_chaine+" terminé",0);
                numero=m_ModeleMonstre.size()-1;
            }

            m_monstre.push_back(Monstre ());

            m_monstre.back().Charger(numero,&m_ModeleMonstre[numero]);
            m_monstre.back().setCoordonnee(positionCase),m_monstre.back().setDepart();

            coordonnee pos;
            pos.x=(int)(((m_monstre.back().getCoordonneePixel().x-m_monstre.back().getCoordonneePixel().y)*64/COTE_TILE*64));
            pos.y=(int)(((m_monstre.back().getCoordonneePixel().x+m_monstre.back().getCoordonneePixel().y)*64/COTE_TILE)/2+32)*2;

            m_monstre.back().m_entite_graphique.Initialiser(pos);


            m_decor[1][positionCase.y][positionCase.x].setMonstre(m_monstre.size()-1);

            info.m_IDObjet=m_monstre.size()-1;
        }
    }
    else if (info.m_IDObjet>=0&&info.m_IDObjet<(int)m_monstre.size())
    {
        personnage->m_nombreInvocation++;
        if (!m_monstre[info.m_IDObjet].EnVie())
        {
            miracleEnCours.m_infos.erase(miracleEnCours.m_infos.begin()+o);
            return 0;
        }
    }
    else
    {
        miracleEnCours.m_infos.erase(miracleEnCours.m_infos.begin()+o);
        return 0;
    }
    return 1;
}

bool Map::Miracle_Pose(Hero *hero, Personnage *personnage, Miracle &modele, Effet &effet, EntiteMiracle &miracleEnCours, InfosEntiteMiracle &info, float temps, int o)
{
    if (effet.m_informations[0] != -1)
        personnage->setJustEtat(effet.m_informations[0]);

    if (effet.m_informations[1] != -1)
        personnage->setPose(effet.m_informations[1]);

    personnage->addAngle(effet.m_informations[2]*45);

    for (int p=0;p<(int)effet.m_lien.size();p++)
    {
        miracleEnCours.m_infos.push_back(new InfosEntiteMiracle ());
        miracleEnCours.m_infos.back()->m_effetEnCours    = effet.m_lien[p];
        miracleEnCours.m_infos.back()->m_position        = info.m_position;
        miracleEnCours.m_infos.back()->m_cible           = info.m_cible;
    }

    miracleEnCours.m_infos.erase(miracleEnCours.m_infos.begin()+o);

    return 0;
}

bool Map::Miracle_Charge(Hero *hero, Personnage *personnage, Miracle &modele, Effet &effet, EntiteMiracle &miracleEnCours, InfosEntiteMiracle &info, float temps, int o)
{
    info.m_informations[3] += temps * 1000;
    if (info.m_informations[3] > effet.m_informations[2])
    {
        if (effet.m_informations[1] >= 0)
        {
            miracleEnCours.m_infos.push_back(new InfosEntiteMiracle ());
            miracleEnCours.m_infos.back()->m_effetEnCours    = effet.m_informations[1];
            miracleEnCours.m_infos.back()->m_position        = info.m_position;
            miracleEnCours.m_infos.back()->m_cible           = info.m_cible;
        }

        info.m_informations[3] -= effet.m_informations[2];
    }

    info.m_position.x = (float)personnage->getCoordonneePixel().x;
    info.m_position.y = (float)personnage->getCoordonneePixel().y;

    if (info.m_IDObjet == 1)
    {

        if(miracleEnCours.m_coordonneeDepart.x != miracleEnCours.m_coordonneeCible.x && personnage->getPousse().x == 0)
        {
            miracleEnCours.m_coordonneeDepart.x = personnage->getCoordonnee().x;
            miracleEnCours.m_coordonneeCible.x  = personnage->getCoordonnee().x;
        }
        if(miracleEnCours.m_coordonneeDepart.y != miracleEnCours.m_coordonneeCible.y && personnage->getPousse().y == 0)
        {
            miracleEnCours.m_coordonneeDepart.y = personnage->getCoordonnee().y;
            miracleEnCours.m_coordonneeCible.y  = personnage->getCoordonnee().y;
        }

        if (((  miracleEnCours.m_coordonneeDepart.x  < miracleEnCours.m_coordonneeCible.x&&personnage->getCoordonneePixel().x>miracleEnCours.m_coordonneeCible.x*COTE_TILE)
            ||( miracleEnCours.m_coordonneeDepart.x  > miracleEnCours.m_coordonneeCible.x&&personnage->getCoordonneePixel().x<miracleEnCours.m_coordonneeCible.x*COTE_TILE)
            ||  miracleEnCours.m_coordonneeDepart.x  == miracleEnCours.m_coordonneeCible.x)

           &&((  miracleEnCours.m_coordonneeDepart.y   > miracleEnCours.m_coordonneeCible.y&&personnage->getCoordonneePixel().y<miracleEnCours.m_coordonneeCible.y*COTE_TILE)
           ||(   miracleEnCours.m_coordonneeDepart.y   < miracleEnCours.m_coordonneeCible.y&&personnage->getCoordonneePixel().y>miracleEnCours.m_coordonneeCible.y*COTE_TILE)
           ||    miracleEnCours.m_coordonneeDepart.y   == miracleEnCours.m_coordonneeCible.y)

            ||(personnage->getPousse().x == 0 && personnage->getPousse().y == 0))
        {

          /*  if(personnage->getCoordonnee().x == miracleEnCours.m_coordonneeCible.x
            && personnage->getCoordonnee().y == miracleEnCours.m_coordonneeCible.y )
                personnage->setCoordonnee(coordonnee (miracleEnCours.m_coordonneeCible.x,
                                                      miracleEnCours.m_coordonneeCible.y));*/

            personnage->setPousse(coordonneeDecimal  (0,0));

            info.m_position.x = (float)personnage->getCoordonneePixel().x;
            info.m_position.y = (float)personnage->getCoordonneePixel().y;

            for (int p=0;p<(int)effet.m_lien.size();p++)
            {
                miracleEnCours.m_infos.push_back(new InfosEntiteMiracle ());
                miracleEnCours.m_infos.back()->m_effetEnCours=effet.m_lien[p];
                miracleEnCours.m_infos.back()->m_position = info.m_position;
                miracleEnCours.m_infos.back()->m_cible=info.m_cible;
            }

            miracleEnCours.m_infos.erase(miracleEnCours.m_infos.begin()+o);

            return 0;
        }
    }
    else
    {
        float m=atan2((double)(personnage->getCoordonnee().x-miracleEnCours.m_coordonneeCible.x),(double)(personnage->getCoordonnee().y-miracleEnCours.m_coordonneeCible.y));
        m+=M_PI/3;

        m=(int)(m*180/M_PI);
        if (m>=360)
            m=0;
        if (m<0)
            m=360+m;
        personnage->setAngle((int)m);
    }

    if ( !(personnage->getCoordonnee().x == miracleEnCours.m_coordonneeCible.x && personnage->getCoordonnee().y == miracleEnCours.m_coordonneeCible.y) )
    {
        float m = atan2((float)miracleEnCours.m_coordonneeCible.x * COTE_TILE - (float)personnage->getCoordonneePixel().x,
                        (float)miracleEnCours.m_coordonneeCible.y * COTE_TILE - (float)personnage->getCoordonneePixel().y);

        personnage->setPousse(coordonneeDecimal  (sin(m) * effet.m_informations[0],
                                                  cos(m) * effet.m_informations[0]));
    }

    info.m_IDObjet= 1;

    return 1;
}

bool Map::Miracle_EffetEcran(Hero *hero, Personnage *personnage, Miracle &modele, Effet &effet, EntiteMiracle &miracleEnCours, InfosEntiteMiracle &info, float temps, int o)
{
    moteurGraphique->m_effetsEcran.push_back(Effet_ecran ());
    moteurGraphique->m_effetsEcran.back().type      = effet.m_informations[0];
    moteurGraphique->m_effetsEcran.back().info1     = effet.m_informations[1];


    for (unsigned p=0;p<effet.m_lien.size();p++)
    {
        miracleEnCours.m_infos.push_back(new InfosEntiteMiracle ());
        miracleEnCours.m_infos.back()->m_effetEnCours    = effet.m_lien[p];
        miracleEnCours.m_infos.back()->m_position        = info.m_position;
        miracleEnCours.m_infos.back()->m_cible           = info.m_cible;
    }

    miracleEnCours.m_infos.erase(miracleEnCours.m_infos.begin()+o);

    return 0;
}

bool Map::Miracle_Degats(Hero *hero, Personnage *personnage, Miracle &modele, Effet &effet, EntiteMiracle &miracleEnCours, InfosEntiteMiracle &info, float temps, int o)
{
    int deg = rand() % (int)(effet.m_informations[1] - effet.m_informations[0] + 1) + effet.m_informations[0];
    if (info.m_cible != NULL)
    {
        if (info.m_cible->getCoordonnee().y >=0 && info.m_cible->getCoordonnee().y < (int)m_decor[0].size())
            if (info.m_cible->getCoordonnee().x >=0 && info.m_cible->getCoordonnee().x < (int)m_decor[0][info.m_cible->getCoordonnee().y].size())
            {
                info.m_position = info.m_cible->getCoordonneePixel();
                if(deg != 0)
                    InfligerDegats(info.m_cible, deg, effet.m_informations[2], hero, 0);
            }
    }

    for (unsigned p=0;p<effet.m_lien.size();p++)
    {
        miracleEnCours.m_infos.push_back(new InfosEntiteMiracle ());
        miracleEnCours.m_infos.back()->m_effetEnCours    = effet.m_lien[p];
        miracleEnCours.m_infos.back()->m_position        = info.m_position;
        miracleEnCours.m_infos.back()->m_cible           = info.m_cible;
    }

    miracleEnCours.m_infos.erase(miracleEnCours.m_infos.begin()+o);

    return 0;
}

bool Map::Miracle_Souffle(Hero *hero, Personnage *personnage, Miracle &modele, Effet &effet, EntiteMiracle &miracleEnCours, InfosEntiteMiracle &info, float temps, int o)
{
    if (info.m_cible != NULL)
    {
        coordonneeDecimal vecteur;

        if      ((int)(info.m_position.x*0.1f)-(int)(info.m_cible->getCoordonneePixel().x*0.1f)<0)
            vecteur.x =  effet.m_informations[0];
        else if ((int)(info.m_position.x*0.1f)-(int)(info.m_cible->getCoordonneePixel().x*0.1f)>0)
            vecteur.x = -effet.m_informations[0];
        else
            vecteur.x = 0;

        if      ((int)(info.m_position.y*0.1f)-(int)(info.m_cible->getCoordonneePixel().y*0.1f)<0)
            vecteur.y =  effet.m_informations[0];
        else if ((int)(info.m_position.y*0.1f)-(int)(info.m_cible->getCoordonneePixel().y*0.1f)>0)
            vecteur.y = -effet.m_informations[0];
        else
            vecteur.y = 0;

        if(vecteur.x == 0 && vecteur.y == 0)
            vecteur.x = effet.m_informations[0],
            vecteur.y = effet.m_informations[0];

        info.m_cible->Pousser(vecteur);
    }

    for (unsigned p=0;p<effet.m_lien.size();p++)
    {
        miracleEnCours.m_infos.push_back(new InfosEntiteMiracle ());
        miracleEnCours.m_infos.back()->m_effetEnCours    = effet.m_lien[p];
        miracleEnCours.m_infos.back()->m_position        = info.m_position;
        miracleEnCours.m_infos.back()->m_cible           = info.m_cible;
    }

    miracleEnCours.m_infos.erase(miracleEnCours.m_infos.begin()+o);

    return 0;
}

bool Map::Miracle_Zone(Hero *hero, Personnage *personnage, Miracle &modele, Effet &effet, EntiteMiracle &miracleEnCours, InfosEntiteMiracle &info, float temps, int o)
{
    coordonnee buf( (int)((int)(info.m_position.x+COTE_TILE*0.5f)/COTE_TILE),
                    (int)((int)(info.m_position.y+COTE_TILE*0.5f)/COTE_TILE));

    for(int x = buf.x - effet.m_informations[3];
            x <= buf.x + effet.m_informations[3]; ++x)
    for(int y = buf.y - effet.m_informations[3];
            y <= buf.y + effet.m_informations[3]; ++y)
        if (y>=0&&x>=0&&y<m_dimensions.y&&x<m_dimensions.x)
            {

                for (unsigned z = 0 ; z < m_decor[1][y][x].getMonstre().size() ; ++z)
                    if (m_decor[1][y][x].getMonstre()[z]>=0&&m_decor[1][y][x].getMonstre()[z]<(int)m_monstre.size())
                        if (m_monstre[m_decor[1][y][x].getMonstre()[z]].EnVie()
                        && ((m_monstre[m_decor[1][y][x].getMonstre()[z]].m_friendly != personnage->m_friendly)
                         || (m_monstre[m_decor[1][y][x].getMonstre()[z]].m_friendly == personnage->m_friendly && effet.m_informations[5]/*&& y==hero->m_personnage.getCoordonnee().y&&x==hero->m_personnage.getCoordonnee().x*/ /*&& ((!personnage->m_friendly) || effet.m_informations[5]))*/))
                        &&m_monstre[m_decor[1][y][x].getMonstre()[z]].getCaracteristique().rang>=0
                        &&m_monstre[m_decor[1][y][x].getMonstre()[z]].m_actif
                        &&m_monstre[m_decor[1][y][x].getMonstre()[z]].getCaracteristique().niveau>=0)
                        {
                            miracleEnCours.m_infos.push_back(new InfosEntiteMiracle ());
                            miracleEnCours.m_infos.back()->m_effetEnCours    =  effet.m_informations[0];
                            miracleEnCours.m_infos.back()->m_position        =  info.m_position;
                            miracleEnCours.m_infos.back()->m_cible           = &m_monstre[m_decor[1][y][x].getMonstre()[z]];
                        }

                if (y==hero->m_personnage.getCoordonnee().y&&x==hero->m_personnage.getCoordonnee().x && ((!personnage->m_friendly) || effet.m_informations[5]))
                {
                    miracleEnCours.m_infos.push_back(new InfosEntiteMiracle ());
                    miracleEnCours.m_infos.back()->m_effetEnCours    =  effet.m_informations[0];
                    miracleEnCours.m_infos.back()->m_position        =  info.m_position;
                    miracleEnCours.m_infos.back()->m_cible           = &hero->m_personnage;
                }
            }


    if (effet.m_informations[4])
        VerifierDeclencheursDegats(buf.y,buf.x);


    for (unsigned p=0;p<effet.m_lien.size();p++)
    {
        miracleEnCours.m_infos.push_back(new InfosEntiteMiracle ());
        miracleEnCours.m_infos.back()->m_effetEnCours    = effet.m_lien[p];
        miracleEnCours.m_infos.back()->m_position        = info.m_position;
        miracleEnCours.m_infos.back()->m_cible           = info.m_cible;
    }

    miracleEnCours.m_infos.erase(miracleEnCours.m_infos.begin()+o);

    return 0;
}

bool Map::Miracle_Conditions(Hero *hero, Personnage *personnage, Miracle &modele, Effet &effet, EntiteMiracle &miracleEnCours, InfosEntiteMiracle &info, float temps, int o)
{
    bool oui = false;
    if(effet.m_informations[1] == C_ANGLE)
        if((int)(personnage->getAngle()/45) == effet.m_informations[2])
            oui = true;

    if(effet.m_informations[1] == C_CIBLELIFE)
        if(info.m_cible != NULL)
            if(info.m_cible->getCaracteristique().maxVie <= effet.m_informations[2]
            && info.m_cible->getCaracteristique().niveau > 0 && info.m_cible->getCaracteristique().vitesse > 0
            && info.m_cible->m_friendly != personnage->m_friendly)
                oui = true;

    if(oui)
    {
        if (info.m_effetEnCours == 0)
        {
            Caracteristique temp = personnage->getCaracteristique();
            temp.foi        -= modele.m_coutFoi + modele.m_reserveFoi;
            temp.vie        -= modele.m_coutVie + modele.m_reserveVie;
            temp.reserveFoi += modele.m_reserveFoi;
            temp.reserveVie += modele.m_reserveVie;
            personnage->setCaracteristique(temp);

            miracleEnCours.m_dejaConsommeFoi = true;
        }

        miracleEnCours.m_infos.push_back(new InfosEntiteMiracle ());
        miracleEnCours.m_infos.back()->m_effetEnCours    =  effet.m_informations[0];
        miracleEnCours.m_infos.back()->m_position        =  info.m_position;
        miracleEnCours.m_infos.back()->m_cible           =  info.m_cible;
    }

    for (unsigned p=0;p<effet.m_lien.size();p++)
    {
        miracleEnCours.m_infos.push_back(new InfosEntiteMiracle ());
        miracleEnCours.m_infos.back()->m_effetEnCours    = effet.m_lien[p];
        miracleEnCours.m_infos.back()->m_position        = info.m_position;
        miracleEnCours.m_infos.back()->m_cible           = info.m_cible;
    }

    miracleEnCours.m_infos.erase(miracleEnCours.m_infos.begin()+o);

    return 0;
}

bool Map::Miracle_Bloquer(Hero *hero, Personnage *personnage, Miracle &modele, Effet &effet, EntiteMiracle &miracleEnCours, InfosEntiteMiracle &info, float temps, int o)
{
    personnage->m_miracleBloquant = effet.m_informations[0];

    for (unsigned p=0;p<effet.m_lien.size();p++)
    {
        miracleEnCours.m_infos.push_back(new InfosEntiteMiracle ());
        miracleEnCours.m_infos.back()->m_effetEnCours    = effet.m_lien[p];
        miracleEnCours.m_infos.back()->m_position        = info.m_position;
        miracleEnCours.m_infos.back()->m_cible           = info.m_cible;
    }

    miracleEnCours.m_infos.erase(miracleEnCours.m_infos.begin()+o);

    return 0;
}

void Map::GererMiracle(Personnage *personnage,std::vector<Miracle> &miracles ,float temps,coordonnee positionHero,Hero *hero)
{
    bool continuerb = true;
    for (int i=0;i<(int)personnage->m_miracleEnCours.size() && continuerb;++i)
    {
        bool continuer=true;
        for (int o=0;o<(int)personnage->m_miracleEnCours[i].m_infos.size() && continuerb;o++)
        {
            if (personnage->m_miracleEnCours[i].m_infos[o]->m_effetEnCours >= 0)
            {
                int effetEnCours = personnage->m_miracleEnCours[i].m_infos[o]->m_effetEnCours;
                int type = miracles[personnage->m_miracleEnCours[i].m_modele].m_effets[personnage->m_miracleEnCours[i].m_infos[o]->m_effetEnCours].m_type;


                if (type == AURA)
                    continuer = Miracle_Aura(       hero,personnage, miracles[personnage->m_miracleEnCours[i].m_modele],
                                                    miracles[personnage->m_miracleEnCours[i].m_modele].m_effets[effetEnCours],
                                                    personnage->m_miracleEnCours[i], *personnage->m_miracleEnCours[i].m_infos[o],
                                                    temps, o);

                else if (type == DECLENCHEUR)
                    continuer = Miracle_Declencheur(hero,personnage, miracles[personnage->m_miracleEnCours[i].m_modele],
                                                    miracles[personnage->m_miracleEnCours[i].m_modele].m_effets[effetEnCours],
                                                    personnage->m_miracleEnCours[i], *personnage->m_miracleEnCours[i].m_infos[o],
                                                    temps, o);

                else if (type == EFFET)
                    continuer = Miracle_Effet(      hero,personnage, miracles[personnage->m_miracleEnCours[i].m_modele],
                                                    miracles[personnage->m_miracleEnCours[i].m_modele].m_effets[effetEnCours],
                                                    personnage->m_miracleEnCours[i], *personnage->m_miracleEnCours[i].m_infos[o],
                                                    temps, o);

                else if (type == CHARME)
                    continuer = Miracle_Charme(     hero,personnage, miracles[personnage->m_miracleEnCours[i].m_modele],
                                                    miracles[personnage->m_miracleEnCours[i].m_modele].m_effets[effetEnCours],
                                                    personnage->m_miracleEnCours[i], *personnage->m_miracleEnCours[i].m_infos[o],
                                                    temps, o);

                else if (type==CORPS_A_CORPS)
                    continuer = Miracle_CorpsACorps(hero,personnage, miracles[personnage->m_miracleEnCours[i].m_modele],
                                                    miracles[personnage->m_miracleEnCours[i].m_modele].m_effets[effetEnCours],
                                                    personnage->m_miracleEnCours[i], *personnage->m_miracleEnCours[i].m_infos[o],
                                                    temps, o);

                else if (type==REPETITION)
                    continuer = Miracle_Repetition( hero,personnage, miracles[personnage->m_miracleEnCours[i].m_modele],
                                                    miracles[personnage->m_miracleEnCours[i].m_modele].m_effets[effetEnCours],
                                                    personnage->m_miracleEnCours[i], *personnage->m_miracleEnCours[i].m_infos[o],
                                                    temps, o);

                else if (type==PROJECTILE)
                    continuer = Miracle_Projectile( hero,personnage, miracles[personnage->m_miracleEnCours[i].m_modele],
                                                    miracles[personnage->m_miracleEnCours[i].m_modele].m_effets[effetEnCours],
                                                    personnage->m_miracleEnCours[i], *personnage->m_miracleEnCours[i].m_infos[o],
                                                    temps, o);

                else if (type == EFFET_GRAPHIQUE)
                    continuer = Miracle_EffetGraphique( hero,personnage, miracles[personnage->m_miracleEnCours[i].m_modele],
                                                    miracles[personnage->m_miracleEnCours[i].m_modele].m_effets[effetEnCours],
                                                    personnage->m_miracleEnCours[i], *personnage->m_miracleEnCours[i].m_infos[o],
                                                    temps, o);

                else if (type==INVOCATION)
                {
                    int no = 0;
                    for(unsigned k = 0 ; k < m_monstre.size() ; ++k)
                        if(&m_monstre[k] == personnage)
                            no = k;
                    continuer = Miracle_Invocation( hero,personnage, miracles[personnage->m_miracleEnCours[i].m_modele],
                                                    miracles[personnage->m_miracleEnCours[i].m_modele].m_effets[effetEnCours],
                                                    personnage->m_miracleEnCours[i], *personnage->m_miracleEnCours[i].m_infos[o],
                                                    temps, o);
                    personnage = &m_monstre[no];
                }

                else if (type == CHANGEMENT_POSE)
                    continuer = Miracle_Pose(       hero,personnage, miracles[personnage->m_miracleEnCours[i].m_modele],
                                                    miracles[personnage->m_miracleEnCours[i].m_modele].m_effets[effetEnCours],
                                                    personnage->m_miracleEnCours[i], *personnage->m_miracleEnCours[i].m_infos[o],
                                                    temps, o);

                else if (type == CHARGE)
                    continuer = Miracle_Charge(     hero,personnage, miracles[personnage->m_miracleEnCours[i].m_modele],
                                                    miracles[personnage->m_miracleEnCours[i].m_modele].m_effets[effetEnCours],
                                                    personnage->m_miracleEnCours[i], *personnage->m_miracleEnCours[i].m_infos[o],
                                                    temps, o);

                else if (type == EFFET_ECRAN)
                    continuer = Miracle_EffetEcran( hero,personnage, miracles[personnage->m_miracleEnCours[i].m_modele],
                                                    miracles[personnage->m_miracleEnCours[i].m_modele].m_effets[effetEnCours],
                                                    personnage->m_miracleEnCours[i], *personnage->m_miracleEnCours[i].m_infos[o],
                                                    temps, o);

                else if (type == DEGATS)
                    continuer = Miracle_Degats( hero,personnage, miracles[personnage->m_miracleEnCours[i].m_modele],
                                                    miracles[personnage->m_miracleEnCours[i].m_modele].m_effets[effetEnCours],
                                                    personnage->m_miracleEnCours[i], *personnage->m_miracleEnCours[i].m_infos[o],
                                                    temps, o);

                else if (type == SOUFFLE)
                    continuer = Miracle_Souffle( hero,personnage, miracles[personnage->m_miracleEnCours[i].m_modele],
                                                    miracles[personnage->m_miracleEnCours[i].m_modele].m_effets[effetEnCours],
                                                    personnage->m_miracleEnCours[i], *personnage->m_miracleEnCours[i].m_infos[o],
                                                    temps, o);

                else if (type == ZONE)
                    continuer = Miracle_Zone(       hero,personnage, miracles[personnage->m_miracleEnCours[i].m_modele],
                                                    miracles[personnage->m_miracleEnCours[i].m_modele].m_effets[effetEnCours],
                                                    personnage->m_miracleEnCours[i], *personnage->m_miracleEnCours[i].m_infos[o],
                                                    temps, o);

                else if (type == CONDITION)
                    continuer = Miracle_Conditions( hero,personnage, miracles[personnage->m_miracleEnCours[i].m_modele],
                                                    miracles[personnage->m_miracleEnCours[i].m_modele].m_effets[effetEnCours],
                                                    personnage->m_miracleEnCours[i], *personnage->m_miracleEnCours[i].m_infos[o],
                                                    temps, o);

                else if (type == BLOQUER)
                    continuer = Miracle_Bloquer   ( hero,personnage, miracles[personnage->m_miracleEnCours[i].m_modele],
                                                    miracles[personnage->m_miracleEnCours[i].m_modele].m_effets[effetEnCours],
                                                    personnage->m_miracleEnCours[i], *personnage->m_miracleEnCours[i].m_infos[o],
                                                    temps, o);

                else if (type == -1)
                {
                    for (unsigned p=0;p<miracles[personnage->m_miracleEnCours[i].m_modele].m_effets[personnage->m_miracleEnCours[i].m_infos[o]->m_effetEnCours].m_lien.size();p++)
                    {
                        personnage->m_miracleEnCours[i].m_infos.push_back(new InfosEntiteMiracle ());
                        personnage->m_miracleEnCours[i].m_infos.back()->m_effetEnCours   = miracles[personnage->m_miracleEnCours[i].m_modele].m_effets[personnage->m_miracleEnCours[i].m_infos[o]->m_effetEnCours].m_lien[p];
                        personnage->m_miracleEnCours[i].m_infos.back()->m_position       = personnage->m_miracleEnCours[i].m_infos[o]->m_position;
                        personnage->m_miracleEnCours[i].m_infos.back()->m_cible          = personnage->m_miracleEnCours[i].m_infos[o]->m_cible;
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

    personnage->m_vientDetreTouche  = NULL;
}

void Map::GererEvenements(int evenement,int z,int couche,int x,int y)
{
    if (evenement>=0&&evenement<(int)m_evenement.size())
    {
        if (m_evenement[m_decor[couche][y][x].getEvenement()[z]].getType()==CHANGER_DECOR)
        {
            m_decor[couche][y][x].setTileset(m_evenement[evenement].getInformation(0));
            m_decor[couche][y][x].setTile(m_evenement[evenement].getInformation(1));

            if (m_decor[couche][y][x].getTileset()>=0&&m_decor[couche][y][x].getTileset()<(int)m_tileset.size())
            {
                coordonnee pos;
                pos.x=(int)(((x*COTE_TILE-y*COTE_TILE)*64/COTE_TILE));
                pos.y=(int)(((x*COTE_TILE+y*COTE_TILE)*64/COTE_TILE)+64);

                m_decor[couche][y][x].m_entite_graphique = moteurGraphique->getEntiteGraphique(m_tileset[m_decor[couche][y][x].getTileset()], m_decor[couche][y][x].getTile(), m_decor[couche][y][x].getCouche());
                m_decor[couche][y][x].m_entite_graphique.m_sprite.SetPosition(pos.x, pos.y * 0.5f);
                m_decor[couche][y][x].m_entite_graphique.Initialiser(pos);
            }
        }
        if (m_evenement[evenement].getType()==EXPLOSION)
        {
            m_decor[couche][y][x].setEvenement(-1,z);
            VerifierDeclencheursDegats(y,x);
        }
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
}

void Map::MusiquePlay(coordonnee position)
{
    if (m_musiqueEnCours>=0&&m_musiqueEnCours<m_musiques.size())
    {
        Sound::Status Status = moteurSons->GetMusicStatus();

        if (Status==0)
        {
            m_musiqueEnCours++;
            if (m_musiqueEnCours>=m_musiques.size())
                m_musiqueEnCours=0;

            if (m_musiqueEnCours>=0&&m_musiqueEnCours<m_musiques.size())
                moteurSons->PlayNewMusic(m_musiques[m_musiqueEnCours]);
        }
    }
    else
        m_musiqueEnCours=0;
}


void Map::Script_RandomDisplace(Jeu *jeu,Script *script,int noInstruction,int monstre,Hero *hero,float temps,Menu *menu, bool seDeplacer)
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
      /*  else
        {
            if (seDeplacer)
            {
                //coordonnee tempCoord(hero->m_personnage.getProchaineCase().x,hero->m_personnage.getProchaineCase().y,-1,-1);
               // m_monstre[monstre].Pathfinding(getAlentourDuPersonnage(m_monstre[monstre].getCoordonnee()),tempCoord);
            }
        }*/
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
            /*else
            {
                if (seDeplacer)
                {
                    coordonnee tempCoord(hero->m_personnage.getProchaineCase().x,hero->m_personnage.getProchaineCase().y,-1,-1);
                    m_monstre[monstre].Pathfinding(getAlentourDuPersonnage(m_monstre[monstre].getCoordonnee()),tempCoord);
                }
            }*/
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
            /*else
            {
                if (seDeplacer)
                {
                    coordonnee tempCoord(hero->m_personnage.getProchaineCase().x,hero->m_personnage.getProchaineCase().y,-1,-1);
                    m_monstre[monstre].Pathfinding(getAlentourDuPersonnage(m_monstre[monstre].getCoordonnee()),tempCoord);
                }
            }*/
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
    if (m_monstre[monstre].EnVie())
    {
        if (seDeplacer)
        {
            m_monstre[monstre].setArrivee(m_monstre[monstre].getCoordonnee());

            if (m_monstre[monstre].m_miracleALancer == -1)
                m_monstre[monstre].setEtat(2);

            m_monstre[monstre].Frappe(m_monstre[monstre].getCoordonnee(),m_monstre[monstre].m_cible->getCoordonnee());
        }
        else
            m_monstre[monstre].setArrivee(m_monstre[monstre].getProchaineCase());
    }

    if (m_monstre[monstre].m_miracleALancer == -1)
        m_monstre[monstre].m_miracleALancer = script->getValeur(noInstruction, 0);
}

void Map::Script_SetState(Jeu *jeu,Script *script,int noInstruction,int monstre,Hero *hero,float temps,Menu *menu, bool seDeplacer)
{
    if (m_monstre[monstre].getEtat()!=script->getValeur(noInstruction, 0))
        m_monstre[monstre].setJustEtat(script->getValeur(noInstruction, 0)), m_monstre[monstre].m_etatForce = true;
    if(script->m_instructions[noInstruction].m_valeurs.size() >= 2)
        m_monstre[monstre].setPose(script->getValeur(noInstruction, 1));
}




void Map::Script_PlaySound(Jeu *jeu,Script *script,int noInstruction,int monstre,Hero *hero,float temps,Menu *menu, bool seDeplacer)
{
    if (m_monstre[monstre].getModele()>=0&&m_monstre[monstre].getModele()<(int)m_ModeleMonstre.size())
    {
        coordonnee position;
        position.x=(m_monstre[monstre].getCoordonnee().x-m_monstre[monstre].getCoordonnee().y-1)/5;
        position.y=(m_monstre[monstre].getCoordonnee().x+m_monstre[monstre].getCoordonnee().y)/5;
        m_monstre[monstre].m_entite_graphique.m_tileset->JouerSon(script->getValeur(noInstruction, 0),position,true);
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
        if (m_monstre[monstre].m_miracleALancer == -1)
        {
            coordonnee arrivee;

            if (seDeplacer)
            {
                m_monstre[monstre].setDepart();
                if (fabs(m_monstre[monstre].getCoordonnee().x-m_monstre[monstre].m_cible->getCoordonnee().x)>1
                        || fabs(m_monstre[monstre].getCoordonnee().y-m_monstre[monstre].m_cible->getCoordonnee().y)>1)
                {
                    m_monstre[monstre].setArrivee(m_monstre[monstre].m_cible->getProchaineCase());

                    //coordonnee tempCoord(hero->m_personnage.getProchaineCase().x,hero->m_personnage.getProchaineCase().y,-1,-1);
                    //m_monstre[monstre].Pathfinding(getAlentourDuPersonnage(m_monstre[monstre].getCoordonnee()),tempCoord);
                }
                else
                {
                    if (m_monstre[monstre].m_cible->EnVie()<=0)
                        m_monstre[monstre].setVu(0);
                    if (!m_monstre[monstre].frappeEnCours && !m_monstre[monstre].m_etatForce)
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
    if(!script->m_instructions[noInstruction].m_valeurs.empty())
        if(script->getValeur(noInstruction, 0) == 1)
            jeu->m_inventaire->setTrader(&hero->m_coffre,&hero->m_classe);

    eventManager->StopEvenement(sf::Mouse::Left, EventClic);
    eventManager->StopEvenement(sf::Mouse::Left, EventClicA);
    jeu->Clock.Reset();
    jeu->m_contexte=jeu->m_inventaire;
    jeu->m_jeu->alpha_dialog = 0;
}

void Map::Script_Potale(Jeu *jeu,Script *script,int noInstruction,int monstre,Hero *hero,float temps,Menu *menu, bool seDeplacer)
{
    hero->setMonstreVise(-1);
   // jeu->m_inventaire->setTrader(m_monstre[monstre].getPointeurObjets(),&hero->m_classe);
    eventManager->StopEvenement(sf::Mouse::Left, EventClic);
    eventManager->StopEvenement(sf::Mouse::Left, EventClicA);
    jeu->Clock.Reset();
    jeu->m_contexte=jeu->m_potales;
    jeu->m_jeu->alpha_dialog = 0;
}

void Map::Script_Craft(Jeu *jeu,Script *script,int noInstruction,int monstre,Hero *hero,float temps,Menu *menu, bool seDeplacer)
{
    hero->setMonstreVise(-1);
   // jeu->m_inventaire->setTrader(m_monstre[monstre].getPointeurObjets(),&hero->m_classe);
    eventManager->StopEvenement(sf::Mouse::Left, EventClic);
    eventManager->StopEvenement(sf::Mouse::Left, EventClicA);
    jeu->Clock.Reset();
    jeu->m_contexte=jeu->m_craft;
    jeu->m_jeu->alpha_dialog = 0;
}

std::string DecouperTexte(std::string texte, int tailleCadran, int tailleTexte)
{
    sf::Text temp;
    temp.SetCharacterSize(tailleTexte);
    temp.SetFont(moteurGraphique->m_font);

    std::string buf;
    std::string bufMot = " ";
    for (int p = 0;p < (int)texte.size();p++)
    {
        if (texte[p] != ' ' && texte[p] != '\0' && texte[p] != '\n')
        {
            if(texte[p] == '\\')
                texte[p] = '\n';
            bufMot += texte[p];
            if(texte[p] == '\n')
                bufMot += " ";

            temp.SetString(buf + bufMot);
            if (temp.GetRect().Right - temp.GetRect().Left > tailleCadran)
                bufMot = '\n' + bufMot;
        }
        else
            buf += bufMot, bufMot = " ";
    }
    buf += bufMot;

    return buf;
}


void Map::GererInstructions(Jeu *jeu,Script *script,int noInstruction,int monstre,Hero *hero,float temps,Menu *menu, bool seDeplacer)
{
    if (noInstruction>=0&&noInstruction<(int)script->m_instructions.size())
    {
        if (script->m_instructions[noInstruction].nom=="fight" && monstre != -1)
            Script_Fight(jeu,script,noInstruction,monstre,hero,temps,menu,seDeplacer);
        else if (script->m_instructions[noInstruction].nom=="evasion" && monstre != -1)
            Script_Evasion(jeu,script,noInstruction,monstre,hero,temps,menu,seDeplacer);
        else if (script->m_instructions[noInstruction].nom=="followHero" && monstre != -1)
            Script_Follow(jeu,script,noInstruction,monstre,hero,temps,menu,seDeplacer);
        else if (script->m_instructions[noInstruction].nom=="teleport" && monstre != -1)
            Script_Teleport(jeu,script,noInstruction,monstre,hero,temps,menu,seDeplacer);
        else if (script->m_instructions[noInstruction].nom=="useMiracle" && monstre != -1)
            Script_UseMiracle(jeu,script,noInstruction,monstre,hero,temps,menu,seDeplacer); //USEMIRACLE(script->getValeur(noInstruction, 0))
        else if (script->m_instructions[noInstruction].nom=="setState" && monstre != -1)
            Script_SetState(jeu,script,noInstruction,monstre,hero,temps,menu,seDeplacer);
        else if (script->m_instructions[noInstruction].nom=="dammages" && monstre != -1)
            InfligerDegats(monstre, script->getValeur(noInstruction, 0), 4, hero, false);
        else if (script->m_instructions[noInstruction].nom=="shoot" && monstre != -1)
            Script_Shoot(jeu,script,noInstruction,monstre,hero,temps,menu,seDeplacer);
        else if (script->m_instructions[noInstruction].nom=="randomDisplace" && monstre != -1)
            Script_RandomDisplace(jeu,script,noInstruction,monstre,hero,temps,menu,seDeplacer);
        else if (script->m_instructions[noInstruction].nom=="setInvocationID" && monstre != -1)
        {
            if(script->getValeur(noInstruction, 0) >= 0)
            for (int i=0;i<(int)m_monstre[monstre].m_miracleEnCours.size();++i)
            {
                for (int o=0;o<(int)m_monstre[monstre].m_miracleEnCours[i].m_infos.size();o++)
                    if (m_monstre[monstre].m_miracleEnCours[i].m_infos[o]->m_effetEnCours>=0)
                        if (m_ModeleMonstre[m_monstre[monstre].getModele()].m_miracles[m_monstre[monstre].m_miracleEnCours[i].m_modele].m_effets[m_monstre[monstre].m_miracleEnCours[i].m_infos[o]->m_effetEnCours].m_type==INVOCATION)
                            if (m_monstre[monstre].m_miracleEnCours[i].m_infos[o]->m_IDObjet>=0&&m_monstre[monstre].m_miracleEnCours[i].m_infos[o]->m_IDObjet<(int)m_monstre.size())
                            {
                                if(m_listID.size() <= script->getValeur(noInstruction, 0))
                                    m_listID.resize(script->getValeur(noInstruction, 0) + 1);
                                m_listID[script->getValeur(noInstruction, 0)].push_back(m_monstre[monstre].m_miracleEnCours[i].m_infos[o]->m_IDObjet);
                            }
            }
        }
        else if (script->m_instructions[noInstruction].nom=="setActif" && monstre != -1)
        {
            m_monstre[monstre].m_actif = script->getValeur(noInstruction, 0);
        }
        else if (script->m_instructions[noInstruction].nom=="set_entityActif" && monstre == -1)
        {
            if(script->getValeur(noInstruction, 0) >= 0 && script->getValeur(noInstruction, 0) < m_monstre.size())
                m_monstre[script->getValeur(noInstruction, 0)].m_actif = script->getValeur(noInstruction, 1);
        }
        else if (script->m_instructions[noInstruction].nom=="playSound" && monstre != -1)
            Script_PlaySound(jeu,script,noInstruction,monstre,hero,temps,menu,seDeplacer); //PLAYSOUND(script->getValeur(noInstruction, 0))
        else if (script->m_instructions[noInstruction].nom=="trade" && monstre != -1)
            Script_Trade(jeu,script,noInstruction,monstre,hero,temps,menu,seDeplacer);
        else if (script->m_instructions[noInstruction].nom=="craft" && monstre != -1)
            Script_Craft(jeu,script,noInstruction,monstre,hero,temps,menu,seDeplacer);
        else if (script->m_instructions[noInstruction].nom=="teleportation_menu")
            Script_Potale(jeu,script,noInstruction,monstre,hero,temps,menu,seDeplacer);
        else if (script->m_instructions[noInstruction].nom=="add_checkpoint")
        {
            hero->addPotale(script->getValeur(noInstruction, 0),
                            script->getValeur(noInstruction, 1),
                            script->getValeur(noInstruction, 2),
                            script->m_instructions[noInstruction].valeurString);
        }
        else if (script->m_instructions[noInstruction].nom=="if")
            GererConditions(jeu,script,noInstruction,monstre,hero,temps,menu,seDeplacer);
        else if (script->m_instructions[noInstruction].nom=="variable")
            script->setVariable(script->getValeur(noInstruction, 0), script->getValeur(noInstruction, 1));
        else if (script->m_instructions[noInstruction].nom=="incrementVariable")
            script->setVariable(script->getValeur(noInstruction, 0), script->getVariable(script->getValeur(noInstruction, 0)) + script->getValeur(noInstruction, 1));
        else if (script->m_instructions[noInstruction].nom=="setCollision" && monstre != -1)
        {
            m_monstre[monstre].m_collision = script->getValeur(noInstruction, 0);
            m_monstre[monstre].m_impenetrable = script->getValeur(noInstruction, 0);
        }
        else if (script->m_instructions[noInstruction].nom=="speak")
        {
            if (jeu->menu.m_dialogue.empty())
            {
                jeu->menu.m_dialogue = DecouperTexte(configuration->getText(4, script->getValeur(noInstruction, 0)), hero->m_classe.position_contenu_dialogue.w, 14);
                eventManager->StopEvenement(Mouse::Left,EventClic);
                hero->m_personnage.setArrivee(hero->m_personnage.getProchaineCase());

                m_monstre[monstre].setArrivee(m_monstre[monstre].getProchaineCase());
            }
        }
        else if (script->m_instructions[noInstruction].nom=="stop_speak")
        {
            jeu->menu.m_dialogue.clear();
            jeu->menu.ClearSpeakChoice();
        }
        else if (script->m_instructions[noInstruction].nom=="speak_choice")
        {
            jeu->menu.AddSpeakChoice(   DecouperTexte(configuration->getText(4, script->getValeur(noInstruction, 0)), hero->m_classe.position_contenu_dialogue.w, 14),
                                        script->getValeur(noInstruction, 1));
            eventManager->StopEvenement(Mouse::Left,EventClic);
            hero->m_personnage.setArrivee(hero->m_personnage.getProchaineCase());

            m_monstre[monstre].setArrivee(m_monstre[monstre].getProchaineCase());
        }
        else if (script->m_instructions[noInstruction].nom=="newQuest")
        {
            bool ok = true;
            for (int i = 0;i < (int)hero->m_quetes.size(); ++i)
                if (hero->m_quetes[i].m_id == script->getValeur(noInstruction, 0))
                    ok = false;
            if (ok)
            {
                hero->m_quetes.push_back(Quete (script->getValeur(noInstruction, 0)));
                hero->m_queteSelectionnee = hero->m_quetes.size() - 1;
            }
        }
        else if (script->m_instructions[noInstruction].nom=="setQuestName")
        {
            for (int i = 0;i < (int)hero->m_quetes.size(); ++i)
                if (hero->m_quetes[i].m_id == script->getValeur(noInstruction, 0))
                    hero->m_quetes[i].m_nom = DecouperTexte(configuration->getText(4, script->getValeur(noInstruction, 1)), hero->m_classe.position_contenu_quetes.w, 16);
        }
        else if (script->m_instructions[noInstruction].nom=="setQuestState")
        {
            for (int i = 0;i < (int)hero->m_quetes.size(); ++i)
                if (hero->m_quetes[i].m_id == script->getValeur(noInstruction, 0))
                {
                    hero->m_quetes[i].m_description = DecouperTexte(configuration->getText(4, script->getValeur(noInstruction, 2)), hero->m_classe.position_contenu_description_quete.w, 14);
                    hero->m_quetes[i].m_statut = script->getValeur(noInstruction, 1);
                }
        }
        else if (script->m_instructions[noInstruction].nom=="setQuestPosition")
        {
            for (int i = 0;i < (int)hero->m_quetes.size(); ++i)
                if (hero->m_quetes[i].m_id == script->getValeur(noInstruction, 0))
                {
                    hero->m_quetes[i].m_position.x = script->getValeur(noInstruction, 1);
                    hero->m_quetes[i].m_position.y = script->getValeur(noInstruction, 2);
                    hero->m_quetes[i].m_map = script->m_instructions[noInstruction].valeurString;
                }
        }
        else if (script->m_instructions[noInstruction].nom=="giftItem" && monstre != -1)
        {
            if (script->getValeur(noInstruction, 0) >= 0 && script->getValeur(noInstruction, 0) < (int)(*m_monstre[monstre].getPointeurObjets()).size())
                if (!hero->AjouterObjet((*m_monstre[monstre].getPointeurObjets())[script->getValeur(noInstruction, 0)], false))
                    m_decor[1][hero->m_personnage.getCoordonnee().y][hero->m_personnage.getCoordonnee().x].AjouterObjet((*m_monstre[monstre].getPointeurObjets())[script->getValeur(noInstruction, 0)]);
        }
        else if (script->m_instructions[noInstruction].nom=="noTalk")
        {
            hero->setMonstreVise(-1);
        }
        else if (script->m_instructions[noInstruction].nom=="addCash")
        {
            hero->m_argent += script->getValeur(noInstruction, 0);
        }
        else if (script->m_instructions[noInstruction].nom=="entity_variable" && monstre == -1)
        {
            if(script->getValeur(noInstruction, 0) < m_listID.size())
                for(unsigned i = 0 ; i < m_listID[script->getValeur(noInstruction, 0)].size() ; ++i)
                    m_monstre[m_listID[script->getValeur(noInstruction, 0)][i]].m_scriptAI.setVariable(script->getValeur(noInstruction, 1), script->getValeur(noInstruction, 2));
        }
        else if (script->m_instructions[noInstruction].nom=="setTile" && monstre == -1)
        {
            if(script->getValeur(noInstruction, 0) >= 0 && script->getValeur(noInstruction, 0) < 2)
                if(script->getValeur(noInstruction, 1) >= 0 && script->getValeur(noInstruction, 1) < (int)m_decor[0][0].size())
                    if(script->getValeur(noInstruction, 2) >= 0 && script->getValeur(noInstruction, 2) < (int)m_decor[0].size())
                    {
                        int x = script->getValeur(noInstruction, 1);
                        int y = script->getValeur(noInstruction, 2);
                        int z = script->getValeur(noInstruction, 0);
                        m_decor[z][y][x].setTile(script->getValeur(noInstruction, 3));
                        m_decor[z][y][x].m_entite_graphique.m_noAnimation = script->getValeur(noInstruction, 3);
                    }
        }

    }
}

void Map::GererConditions(Jeu *jeu,Script *script,int noInstruction,int monstre,Hero *hero,float temps,Menu *menu, bool seDeplacer)
{
    if (noInstruction>=0&&noInstruction<(int)script->m_instructions.size())
    {
        int b=0;
        bool ok=true;
        for (;b<(int)script->m_instructions[noInstruction].m_valeurs.size()&&script->m_instructions[noInstruction].m_valeurs[b]!=-2;b++)
        {
            if (script->m_instructions[noInstruction].m_valeurs[b]>=0&&script->m_instructions[noInstruction].m_valeurs[b]<(int)script->m_instructions.size())
            {
                if (script->m_instructions[script->m_instructions[noInstruction].m_valeurs[b]].nom=="alive" && monstre != -1)
                {
                    if (!m_monstre[monstre].EnVie())
                        ok=false;
                }
                else if (script->m_instructions[script->m_instructions[noInstruction].m_valeurs[b]].nom=="see" && monstre != -1)
                {
                    if (!m_monstre[monstre].getVu())
                        ok=false;
                }
                else if (script->m_instructions[script->m_instructions[noInstruction].m_valeurs[b]].nom=="shooter" && monstre != -1)
                {
                    if (!m_monstre[monstre].m_shooter)
                        ok=false;
                }
                else if (script->m_instructions[script->m_instructions[noInstruction].m_valeurs[b]].nom=="touch" && monstre != -1)
                {
                    if (!m_monstre[monstre].m_touche)
                        ok=false;
                }
                else if (script->m_instructions[script->m_instructions[noInstruction].m_valeurs[b]].nom=="getState" && monstre != -1)
                {
                    if (m_monstre[monstre].getEtat() != script->getValeur(script->m_instructions[noInstruction].m_valeurs[b], 0))
                        ok=false;
                }
                else if (script->m_instructions[script->m_instructions[noInstruction].m_valeurs[b]].nom=="numberInvocation" && monstre != -1)
                {
                    if (m_monstre[monstre].m_nombreInvocation!=script->getValeur(script->m_instructions[noInstruction].m_valeurs[b], 0))
                        ok=false;
                }
                else if (script->m_instructions[script->m_instructions[noInstruction].m_valeurs[b]].nom=="distance" && monstre != -1)
                {
                    if (((m_monstre[monstre].getCoordonnee().x-hero->m_personnage.getCoordonnee().x)*(m_monstre[monstre].getCoordonnee().x-hero->m_personnage.getCoordonnee().x) + (m_monstre[monstre].getCoordonnee().y-hero->m_personnage.getCoordonnee().y)*(m_monstre[monstre].getCoordonnee().y-hero->m_personnage.getCoordonnee().y)) >= script->getValeur(script->m_instructions[noInstruction].m_valeurs[b], 0)*script->getValeur(script->m_instructions[noInstruction].m_valeurs[b], 0))
                        ok=false;
                }
                else if (script->m_instructions[script->m_instructions[noInstruction].m_valeurs[b]].nom=="exist_item")
                {
                    int nbr = 0;
                    for(unsigned i = 0 ; i < hero->getNombreObjet() ; ++i)
                        if(hero->getNomObjet(i) == script->m_instructions[script->m_instructions[noInstruction].m_valeurs[b]].valeurString)
                            nbr++;


                    if(nbr < script->getValeur(script->m_instructions[noInstruction].m_valeurs[b], 0))
                         ok = false;
                    else if(script->getValeur(script->m_instructions[noInstruction].m_valeurs[b], 1))
                        for(unsigned i = 0 ; i < hero->getNombreObjet() ; ++i)
                            if(hero->getNomObjet(i) == script->m_instructions[script->m_instructions[noInstruction].m_valeurs[b]].valeurString)
                                hero->delObjet(i), i--;

                }
                else if (script->m_instructions[script->m_instructions[noInstruction].m_valeurs[b]].nom=="rand")
                {
                    if (rand() % 100 < script->getValeur(script->m_instructions[noInstruction].m_valeurs[b], 0))
                        ok=false;
                }
                else if (script->m_instructions[script->m_instructions[noInstruction].m_valeurs[b]].nom=="variable")
                {
                    if (script->getVariable(script->getValeur(script->m_instructions[noInstruction].m_valeurs[b], 0))!=script->getValeur(script->m_instructions[noInstruction].m_valeurs[b], 1))
                        ok=false;
                }
                else if (script->m_instructions[script->m_instructions[noInstruction].m_valeurs[b]].nom=="talk" && monstre != -1)
                {
                    if (!(hero->getMonstreVise()==monstre&&fabs(m_monstre[monstre].getCoordonnee().x-hero->m_personnage.getCoordonnee().x)<=1&&fabs(m_monstre[monstre].getCoordonnee().y-hero->m_personnage.getCoordonnee().y)<=1))
                        ok=false;
                }
                else if (script->m_instructions[script->m_instructions[noInstruction].m_valeurs[b]].nom=="clicOver" && monstre != -1)
                {
                    if (!(hero->getMonstreVise()==monstre))
                        ok=false;
                }
                else if (script->m_instructions[script->m_instructions[noInstruction].m_valeurs[b]].nom=="speak_choice" && monstre != -1)
                {
                    if (menu->getSpeakChoice() != script->getValeur(script->m_instructions[noInstruction].m_valeurs[b], 0))
                        ok=false;
                }
                else if (script->m_instructions[script->m_instructions[noInstruction].m_valeurs[b]].nom=="no_speak")
                {
                    if (!jeu->menu.m_dialogue.empty())
                        ok=false;
                }
                else if (script->m_instructions[script->m_instructions[noInstruction].m_valeurs[b]].nom=="quest")
                {
                    if (script->getValeur(script->m_instructions[noInstruction].m_valeurs[b], 1) == -1)
                    {
                        for (int i = 0;i < (int)hero->m_quetes.size(); ++i)
                            if (hero->m_quetes[i].m_id == script->getValeur(script->m_instructions[noInstruction].m_valeurs[b], 0))
                                ok = false;
                    }
                    else if(ok)
                    {
                        ok = false;
                        for (int i = 0;i < (int)hero->m_quetes.size(); ++i)
                            if (hero->m_quetes[i].m_id == script->getValeur(script->m_instructions[noInstruction].m_valeurs[b], 0))
                                if (hero->m_quetes[i].m_statut == script->getValeur(script->m_instructions[noInstruction].m_valeurs[b], 1))
                                    ok = true;
                    }
                }
                else if (script->m_instructions[script->m_instructions[noInstruction].m_valeurs[b]].nom=="entity_variable" && monstre == -1)
                {
                    if(script->getValeur(script->m_instructions[noInstruction].m_valeurs[b], 0) < m_listID.size())
                    {
                        for(unsigned i = 0 ; i < m_listID[script->getValeur(script->m_instructions[noInstruction].m_valeurs[b], 0)].size() ; ++i)
                            if (m_monstre[m_listID[script->getValeur(script->m_instructions[noInstruction].m_valeurs[b], 0)][i]].m_scriptAI.getVariable(script->getValeur(script->m_instructions[noInstruction].m_valeurs[b], 1))!=script->getValeur(script->m_instructions[noInstruction].m_valeurs[b], 2))
                                ok = false;
                    }
                    else
                        ok = false;
                }
                else if (script->m_instructions[script->m_instructions[noInstruction].m_valeurs[b]].nom=="entity_dead" && monstre == -1)
                {
                    if(script->getValeur(script->m_instructions[noInstruction].m_valeurs[b], 0) < m_listID.size())
                    {
                        for(unsigned i = 0 ; i < m_listID[script->getValeur(script->m_instructions[noInstruction].m_valeurs[b], 0)].size() ; ++i)
                            if (m_monstre[m_listID[script->getValeur(script->m_instructions[noInstruction].m_valeurs[b], 0)][i]].EnVie())
                                if(m_monstre[m_listID[script->getValeur(script->m_instructions[noInstruction].m_valeurs[b], 0)][i]].m_friendly ==
                                   m_ModeleMonstre[m_monstre[m_listID[script->getValeur(script->m_instructions[noInstruction].m_valeurs[b], 0)][i]].getModele()].m_friendly)
                                    ok = false;
                    }
                    else
                        ok = false;
                }
            }
        }
        for (;b<(int)script->m_instructions[noInstruction].m_valeurs.size()&&script->m_instructions[noInstruction].m_valeurs[b]!=-1&&script->m_instructions[noInstruction].m_valeurs[b]!=-3;b++)
            if (ok)
                if (script->m_instructions[noInstruction].m_valeurs[b]>=0&&script->m_instructions[noInstruction].m_valeurs[b]<(int)script->m_instructions.size())
                    GererInstructions(jeu,script,script->m_instructions[noInstruction].m_valeurs[b],monstre,hero,temps,menu,seDeplacer);
        if (!ok)
            for (;b<(int)script->m_instructions[noInstruction].m_valeurs.size()&&script->m_instructions[noInstruction].m_valeurs[b]!=-1;b++)
                if (script->m_instructions[noInstruction].m_valeurs[b]>=0&&script->m_instructions[noInstruction].m_valeurs[b]<(int)script->m_instructions.size())
                    GererInstructions(jeu,script,script->m_instructions[noInstruction].m_valeurs[b],monstre,hero,temps,menu,seDeplacer);
    }
}

void Map::GererProjectilesEtEffets(Hero *hero,float temps)
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
        int i=0;
        for (std::vector <Projectile>::iterator projectile = m_projectile.begin();
                projectile != m_projectile.end();++i, ++projectile)
        {
            projectile->m_dejaDeplace=false;
            if (projectile->m_supprime)
                nombreInactif++;

            if (!projectile->m_dejaDeplace)
            {
                if (projectile->m_actif)
                {
                    projectile->m_dejaDeplace=true;

                    int temp=i;
                    m_decor[1][projectile->m_positionCase.y][projectile->m_positionCase.x].delProjectile(i);

                    if (projectile->m_position.y/COTE_TILE>=vueMin.y&&projectile->m_position.y/COTE_TILE<vueMax.y&&projectile->m_position.x/COTE_TILE>=vueMin.x&&projectile->m_position.x/COTE_TILE<vueMax.x)
                    {
                        if (projectile->m_cible.x == -1 && projectile->m_cible.y == -1)
                        {
                            if (!m_decor[1][(int)(projectile->m_positionCase.y)][(int)(projectile->m_positionCase.x)].getMonstre().empty())
                            {
                                bool collision = false;
                                for (unsigned o = 0 ; o < m_decor[1][(int)(projectile->m_positionCase.y)][(int)(projectile->m_positionCase.x)].getMonstre().size() && !collision ; ++o)
                                    if ( m_decor[1][(int)(projectile->m_positionCase.y)][(int)(projectile->m_positionCase.x)].getMonstre()[o] < (int)m_monstre.size())
                                        if (m_monstre[m_decor[1][(int)(projectile->m_positionCase.y)][(int)(projectile->m_positionCase.x)].getMonstre()[o]].EnVie())
                                        if(m_monstre[m_decor[1][(int)(projectile->m_positionCase.y)][(int)(projectile->m_positionCase.x)].getMonstre()[o]].m_friendly == projectile->m_monstre)
                                        if(m_monstre[m_decor[1][(int)(projectile->m_positionCase.y)][(int)(projectile->m_positionCase.x)].getMonstre()[o]].getCaracteristique().niveau >= 0)
                                        {
                                            collision                         = true;
                                            projectile->m_actif        = false;
                                            projectile->m_entite_cible = &m_monstre[m_decor[1][(int)(projectile->m_positionCase.y)][(int)(projectile->m_positionCase.x)].getMonstre()[o]];
                                           // InfligerDegats(m_decor[1][(int)(projectile->m_positionCase.y)][(int)(projectile->m_positionCase.x)].getMonstre()[o],projectile->m_degats,hero,false);
                                        }
                            }

                            if (hero->m_personnage.getCoordonnee().x==(int)((projectile->m_position.x+32)/COTE_TILE)&&hero->m_personnage.getCoordonnee().y==(int)((projectile->m_position.y+32)/COTE_TILE)&&projectile->m_monstre)
                            {
                                projectile->m_actif=false;
                                projectile->m_entite_cible = &hero->m_personnage;
                               // hero->m_personnage.InfligerDegats(projectile->m_degats);
                            }
                        }
                        else
                        {
                            bool x = false;
                            bool y = false;

                            if ( projectile->m_positionCase.x >= projectile->m_cible.x && projectile->m_cible.x >= projectile->m_depart.x)
                                x = true;
                            else if ( projectile->m_positionCase.x <= projectile->m_cible.x && projectile->m_cible.x <= projectile->m_depart.x)
                                x = true;

                            if ( projectile->m_positionCase.y >= projectile->m_cible.y && projectile->m_cible.y >= projectile->m_depart.y)
                                y = true;
                            else if ( projectile->m_positionCase.y <= projectile->m_cible.y && projectile->m_cible.y <= projectile->m_depart.y)
                                y = true;

                            if (x && y)
                                projectile->m_actif = false;
                        }

                        if (getTypeCase((int)(projectile->m_positionCase.x),(int)(projectile->m_positionCase.y))==1)
                            projectile->m_actif=false;



                        if (projectile->m_actif)
                        {
                            projectile->m_positionCase.y=(int)((projectile->m_position.y+32)/COTE_TILE);
                            projectile->m_positionCase.x=(int)((projectile->m_position.x+32)/COTE_TILE);

                            m_decor[1][projectile->m_positionCase.y][projectile->m_positionCase.x].setProjectile(temp);
                        }
                    }
                    else
                    {
                        projectile->m_actif=false;
                        m_decor[1][(int)(projectile->m_position.y/COTE_TILE)][(int)(projectile->m_position.x/COTE_TILE)].delProjectile(temp);
                    }

                    projectile->Deplacer(temps);
                }
                else
                {
                    moteurGraphique->LightManager->Delete_Light(projectile->m_effet.m_light);
                    m_decor[1][projectile->m_positionCase.y][projectile->m_positionCase.x].delProjectile(i);
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
            {
                m_decor[1][(int)(m_effets[i].m_position.y / COTE_TILE)][(int)(m_effets[i].m_position.x / COTE_TILE)].delEffetGraphique(i);
                nombreInactif++,moteurGraphique->LightManager->Delete_Light(m_effets[i].m_light);
            }
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
        vueMax.x=m_dimensions.x;
    if (vueMax.y>=m_dimensions.y)
        vueMax.y=m_dimensions.y;

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
                        TesterPoussable(m_monstre[monstre], temps, monstre);

                        bool seDeplacer = m_monstre[monstre].SeDeplacer(temps*100,getDimensions());



                        //if (m_monstre[monstre].getPousse().x == 0 && m_monstre[monstre].getPousse().y == 0)
                        {
                            Script *script=&m_monstre[monstre].m_scriptAI;
                            if ((int)script->m_instructions.size()>0)
                                for (int a=0;a<(int)script->m_instructions[0].m_valeurs.size();a++)
                                    if (script->m_instructions[0].m_valeurs[a]>=0&&script->m_instructions[0].m_valeurs[a]<(int)script->m_instructions.size())
                                        GererInstructions(jeu,script,script->m_instructions[0].m_valeurs[a],monstre,hero,temps,menu,seDeplacer);

                            if (m_monstre[monstre].getErreurPathfinding())
                                Script_RandomDisplace(jeu,script,0,monstre,hero,temps,menu,seDeplacer);
                        }

                        if (seDeplacer)
                        {
                            coordonnee tempCoord(hero->m_personnage.getProchaineCase().x,hero->m_personnage.getProchaineCase().y,-1,-1);
                             m_monstre[monstre].Pathfinding(getAlentourDuPersonnage(m_monstre[monstre].getCoordonnee()),tempCoord);
                        }

                        m_monstre[monstre].m_touche = false;
                    }
                    else
                        m_monstre[monstre].m_attente-=temps*100;

                    if (m_monstre[monstre].getErreurPathfinding())
                        m_monstre[monstre].m_compteur++;
                    else
                        m_monstre[monstre].m_compteur=0;

                    ///GESTION DES EVENEMENTS SUR LES MONSTRES
                    for (int l=0;l<2;l++)
                        if (m_monstre[monstre].getCoordonnee().y>=0&&m_monstre[monstre].getCoordonnee().y<m_dimensions.y
                                &&m_monstre[monstre].getCoordonnee().x>=0&&m_monstre[monstre].getCoordonnee().x<m_dimensions.x)
                            for (int z=0;z<(int)m_decor[l][m_monstre[monstre].getCoordonnee().y][m_monstre[monstre].getCoordonnee().x].getEvenement().size();z++)
                                if (m_decor[l][m_monstre[monstre].getCoordonnee().y][m_monstre[monstre].getCoordonnee().x].getEvenement()[z]>=0&&m_decor[l][m_monstre[monstre].getCoordonnee().y][m_monstre[monstre].getCoordonnee().x].getEvenement()[z]<(int)m_evenement.size())
                                    if (m_evenement[m_decor[l][m_monstre[monstre].getCoordonnee().y][m_monstre[monstre].getCoordonnee().x].getEvenement()[z]].getType()==INFLIGER_DEGATS)
                                        if (m_monstre[monstre].EnVie())
                                            InfligerDegats(monstre,m_evenement[m_decor[l][m_monstre[monstre].getCoordonnee().y][m_monstre[monstre].getCoordonnee().x].getEvenement()[z]].getInformation(0)*temps*10,
                                                                   m_evenement[m_decor[l][m_monstre[monstre].getCoordonnee().y][m_monstre[monstre].getCoordonnee().x].getEvenement()[z]].getInformation(1),hero,0);

                    if (m_monstre[monstre].EnVie())
                        if (m_monstre[monstre].getCoordonnee().y!=j || m_monstre[monstre].getCoordonnee().x != k)
                            if ( m_monstre[monstre].getCoordonnee().x >= 0 && m_monstre[monstre].getCoordonnee().x < m_dimensions.x
                                    && m_monstre[monstre].getCoordonnee().y >= 0 && m_monstre[monstre].getCoordonnee().y < m_dimensions.y)
                            {
                                m_decor[1][j][k].delMonstre(monstre);
                                m_decor[1][m_monstre[monstre].getCoordonnee().y][m_monstre[monstre].getCoordonnee().x].setMonstre(monstre);
                            }
                }
            }
}

void Map::GererScript(Jeu *jeu,Hero *hero,float temps,Menu *menu)
{
    if ((int)m_script.m_instructions.size()>0)
        for (int a=0;a<(int)m_script.m_instructions[0].m_valeurs.size();a++)
            if (m_script.m_instructions[0].m_valeurs[a]>=0&&m_script.m_instructions[0].m_valeurs[a]<(int)m_script.m_instructions.size())
                GererInstructions(jeu,&m_script,m_script.m_instructions[0].m_valeurs[a],-1,hero,temps,menu,0);
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
                                if (!m_monstre[m_decor[1][y][x].getMonstre()[o]].m_friendly
                                    && m_monstre[m_decor[1][y][x].getMonstre()[o]].EnVie()
                                    && m_monstre[m_decor[1][y][x].getMonstre()[o]].getCaracteristique().niveau > 0
                                    && m_monstre[m_decor[1][y][x].getMonstre()[o]].m_actif)
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
                                if (m_monstre[m_decor[1][y][x].getMonstre()[o]].m_friendly
                                    && m_monstre[m_decor[1][y][x].getMonstre()[o]].EnVie()
                                    && m_monstre[m_decor[1][y][x].getMonstre()[o]].getCaracteristique().niveau > 0
                                    && m_monstre[m_decor[1][y][x].getMonstre()[o]].m_actif)
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

bool Map::TileVisible(int x,int y, coordonnee pos)
{
    return true;
}

bool Map::InfligerDegats(int numero, float degats, int type, Hero *hero,bool pousser)
{
    if (numero >= 0 && numero < (int)m_monstre.size())
    {
        return InfligerDegats(&m_monstre[numero], degats, type, hero, pousser);

        if (!m_monstre[numero].EnVie())
            if (m_monstreIllumine == numero || hero->getMonstreVise() == numero)
                m_monstreIllumine=-1,hero->setMonstreVise(-1);
    }
    return (false);
}

bool Map::InfligerDegats(Personnage *monstre, float degats, int type, Hero *hero,bool pousser)
{
    /*if (configuration->sang&&monstre->EnVie())
        for (int i=0;i<5;++i)
            if (rand()%(100)>25&&monstre->getCaracteristique().sang)
            {
                coordonneeDecimal position;
                position.x=rand()%(600 - 100) + 100;
                position.y=rand()%(400 - 100 ) + 100;

                menu->AjouterSang(position);
            }*/

    float viePrecedente = monstre->getCaracteristique().vie;

    /*if(monstre->EnVie()&&pousser)
    {
        double m=atan(((double)hero->m_personnage.getCoordonnee().y-(double)monstre->getCoordonnee().y)/(double)((double)hero->m_personnage.getCoordonnee().x-(double)monstre->getCoordonnee().x));
        if(hero->m_personnage.getCoordonnee().x-monstre->getCoordonnee().x<0)
            m-=M_PI;
        coordonnee vecteur;
        vecteur.x=(int)(-cos(m)*20);
        vecteur.y=(int)(-sin(m)*20);
        PousserMonstre(numeroMonstre,vecteur);

        for(int o=0;o<2;o++)
            for(int x=0;x<10;x++)
                for(int y=0;y<10;y++)
                    if(x>=0&&y>=0&&x<m_dimensions.x&&y<m_dimensions.y)
                        if(m_decor[o][y][x].getMonstre()>=0&&m_decor[o][y][x].getMonstre()<m_monstre.size())
                            m_monstre[m_decor[o][y][x].getMonstre()].setVu(1);
    }*/


        if(monstre != &hero->m_personnage)
            monstre->InfligerDegats(degats, type, &m_ModeleMonstre[monstre->getModele()]);
        else
            monstre->InfligerDegats(degats, type, &hero->m_modelePersonnage[0]);

        /*hero->m_personnage.InfligerDegats(-degats * hero->m_caracteristiques.volVie, NULL);
        hero->m_caracteristiques.foi += degats * hero->m_caracteristiques.volFoi;*/

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
                if (monstre->m_miracleEnCours[i].m_infos[o]->m_effetEnCours>=0)
                    if (m_ModeleMonstre[monstre->getModele()].m_miracles[monstre->m_miracleEnCours[i].m_modele].m_effets[monstre->m_miracleEnCours[i].m_infos[o]->m_effetEnCours].m_type==INVOCATION)
                        if (monstre->m_miracleEnCours[i].m_infos[o]->m_IDObjet>=0&&monstre->m_miracleEnCours[i].m_infos[o]->m_IDObjet<(int)m_monstre.size())
                            InfligerDegats(monstre->m_miracleEnCours[i].m_infos[o]->m_IDObjet, m_monstre[monstre->m_miracleEnCours[i].m_infos[o]->m_IDObjet].getCaracteristique().vie, 4,hero,false);
        }

        if (monstre->getCaracteristique().pointAme>0)
        {
            /*coordonneeDecimal position;
            position.x=(((monstre->getCoordonnee().x-monstre->getCoordonnee().y-1)*64)-GetViewRect(moteurGraphique->m_camera).Left+48-(configuration->Resolution.w/configuration->zoom/2-400));
            position.y=(((monstre->getCoordonnee().x+monstre->getCoordonnee().y)*32)-GetViewRect(moteurGraphique->m_camera).Top-96);
            menu->AjouterAme(position,monstre->getCaracteristique().pointAme);*/
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

        monstre->Pousser(coordonneeDecimal(cos(m) * force * 0.1f, sin(m) * force * 0.1f));

        if (monstre->getCoordonnee().x>=0&&monstre->getCoordonnee().x<m_dimensions.x&&monstre->getCoordonnee().y>=0&&monstre->getCoordonnee().y<m_dimensions.y)
            for (int i=0;i<(int)monstre->getObjets().size();++i)
                m_decor[1][monstre->getCoordonnee().y][monstre->getCoordonnee().x].AjouterObjet(monstre->getObjets()[i]);

        return 1;
    }

    return 0;
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
}


void Map::AjouterObjet(Objet objet)
{
    if (objet.getPosition().x>=0&&objet.getPosition().x<m_dimensions.x&&objet.getPosition().y>=0&&objet.getPosition().y<m_dimensions.y)
    {
        m_decor[1][objet.getPosition().y][objet.getPosition().x].AjouterObjet(objet);
    }
}

void Map::AjouterMonstre(Monstre monstre)
{
    m_monstre.push_back(monstre);
    if(m_monstre.back().getCoordonnee().x >= 0 && m_monstre.back().getCoordonnee().x < (int)m_decor[0][0].size()
    && m_monstre.back().getCoordonnee().y >= 0 && m_monstre.back().getCoordonnee().y < (int)m_decor[0].size())
        m_decor[1][m_monstre.back().getCoordonnee().y][m_monstre.back().getCoordonnee().x].setMonstre(m_monstre.size()-1);
}


void Map::AjouterModeleMonstre(Modele_Monstre monstre)
{
    m_ModeleMonstre.push_back(monstre);
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
    if (position.x>=0&&position.x<m_dimensions.x
            &&position.y>=0&&position.y<m_dimensions.y)
        return m_decor[1][position.y][position.x].getNombreObjets();
    else
        return 0;
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

const Modele_Monstre &Map::getModeleMonstre(int numeroMonstre)
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
    casePathfinding **grille = new casePathfinding* [10];

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

    return grille;
}

int Map::getMonstreIllumine()
{
    return m_monstreIllumine;
}

const coordonnee &Map::getDimensions()
{
    return m_dimensions;
}

int Map::getEvenement(coordonnee casePointee)
{
    int temp=-1;
    if (casePointee.y>=0&&casePointee.y<m_dimensions.y&&casePointee.x>=0&&casePointee.x<m_dimensions.x)
        for (int i=0;i<2;++i)
            if ((int)m_decor[i][casePointee.y][casePointee.x].getEvenement().size()>0)
                if (m_decor[i][casePointee.y][casePointee.x].getEvenement()[0]>-1)
                    temp=m_decor[i][casePointee.y][casePointee.x].getEvenement()[0];

    return temp;
}

bool Map::getCollision(int positionX,int positionY, int exception)
{
    if(positionX >= 0 && positionX < (int)m_decor[0][0].size()
    && positionY >= 0 && positionY < (int)m_decor[0].size())
    for (int i=0;i<2;++i)
    {
        if (m_decor[i][positionY][positionX].getTileset()>=0&&m_decor[i][positionY][positionX].getTileset()<(int)m_tileset.size())
            if (moteurGraphique->getTileset(m_tileset[m_decor[i][positionY][positionX].getTileset()])->getCollisionTile(m_decor[i][positionY][positionX].getTile()))
                return 1;

        if (i == 1)
        {

            for (unsigned o = 0 ; o < m_decor[i][positionY][positionX].getMonstre().size() ; ++o)
                if (m_decor[i][positionY][positionX].getMonstre()[o]>-1&&m_decor[i][positionY][positionX].getMonstre()[o]<(int)m_monstre.size() && m_decor[i][positionY][positionX].getMonstre()[o] != exception)
                    if (m_monstre[m_decor[i][positionY][positionX].getMonstre()[o]].EnVie()&&m_monstre[m_decor[i][positionY][positionX].getMonstre()[o]].m_collision)
                        return 1;

            coordonnee enCours;

            enCours.x=positionX+1;
            enCours.y=positionY+1;
            if (enCours.x>=0&&enCours.y>=0&&enCours.x<m_dimensions.x&&enCours.y<m_dimensions.y)
                for (unsigned o = 0 ; o < m_decor[i][ enCours.y][enCours.x].getMonstre().size() ; ++o)
                    if (m_decor[i][ enCours.y][enCours.x].getMonstre()[o]>-1&&m_decor[i][ enCours.y][enCours.x].getMonstre()[o]<(int)m_monstre.size() && m_decor[i][enCours.y][enCours.x].getMonstre()[o] != exception)
                        if (m_monstre[m_decor[i][ enCours.y][enCours.x].getMonstre()[o]].EnVie()&&m_monstre[m_decor[i][ enCours.y][enCours.x].getMonstre()[o]].getCaracteristique().vitesse>0)
                            if (m_decor[i][enCours.y][enCours.x].getMonstre()[o]<(int)m_monstre.size())
                                if ((m_monstre[m_decor[i][enCours.y][enCours.x].getMonstre()[o]].getProchaineCase().x==positionX&&m_monstre[m_decor[i][enCours.y][enCours.x].getMonstre()[o]].getProchaineCase().y==positionY)||(m_monstre[m_decor[i][enCours.y][enCours.x].getMonstre()[o]].getCoordonnee().x==positionX&&m_monstre[m_decor[i][enCours.y][enCours.x].getMonstre()[o]].getCoordonnee().y==positionY))
                                    return 1;
            enCours.x=positionX-1;
            enCours.y=positionY-1;
            if (enCours.x>=0&&enCours.y>=0&&enCours.x<m_dimensions.x&&enCours.y<m_dimensions.y)
                for (unsigned o = 0 ; o < m_decor[i][ enCours.y][enCours.x].getMonstre().size() ; ++o)
                    if (m_decor[i][ enCours.y][enCours.x].getMonstre()[o]>-1&&m_decor[i][ enCours.y][enCours.x].getMonstre()[o]<(int)m_monstre.size() && m_decor[i][enCours.y][enCours.x].getMonstre()[o] != exception)
                        if (m_monstre[m_decor[i][ enCours.y][enCours.x].getMonstre()[o]].EnVie()&&m_monstre[m_decor[i][ enCours.y][enCours.x].getMonstre()[o]].getCaracteristique().vitesse>0)
                            if (m_decor[i][enCours.y][enCours.x].getMonstre()[o]<(int)m_monstre.size())
                                if ((m_monstre[m_decor[i][enCours.y][enCours.x].getMonstre()[o]].getProchaineCase().x==positionX&&m_monstre[m_decor[i][enCours.y][enCours.x].getMonstre()[o]].getProchaineCase().y==positionY)||(m_monstre[m_decor[i][enCours.y][enCours.x].getMonstre()[o]].getCoordonnee().x==positionX&&m_monstre[m_decor[i][enCours.y][enCours.x].getMonstre()[o]].getCoordonnee().y==positionY))
                                    return 1;
            enCours.x=positionX+1;
            enCours.y=positionY-1;
            if (enCours.x>=0&&enCours.y>=0&&enCours.x<m_dimensions.x&&enCours.y<m_dimensions.y)
                for (unsigned o = 0 ; o < m_decor[i][ enCours.y][enCours.x].getMonstre().size() ; ++o)
                    if (m_decor[i][ enCours.y][enCours.x].getMonstre()[o]>-1&&m_decor[i][ enCours.y][enCours.x].getMonstre()[o]<(int)m_monstre.size() && m_decor[i][enCours.y][enCours.x].getMonstre()[o] != exception)
                        if (m_monstre[m_decor[i][ enCours.y][enCours.x].getMonstre()[o]].EnVie()&&m_monstre[m_decor[i][ enCours.y][enCours.x].getMonstre()[o]].getCaracteristique().vitesse>0)
                            if (m_decor[i][enCours.y][enCours.x].getMonstre()[o]<(int)m_monstre.size())
                                if ((m_monstre[m_decor[i][enCours.y][enCours.x].getMonstre()[o]].getProchaineCase().x==positionX&&m_monstre[m_decor[i][enCours.y][enCours.x].getMonstre()[o]].getProchaineCase().y==positionY)||(m_monstre[m_decor[i][enCours.y][enCours.x].getMonstre()[o]].getCoordonnee().x==positionX&&m_monstre[m_decor[i][enCours.y][enCours.x].getMonstre()[o]].getCoordonnee().y==positionY))
                                    return 1;
            enCours.x=positionX-1;
            enCours.y=positionY+1;
            if (enCours.x>=0&&enCours.y>=0&&enCours.x<m_dimensions.x&&enCours.y<m_dimensions.y)
                for (unsigned o = 0 ; o < m_decor[i][ enCours.y][enCours.x].getMonstre().size() ; ++o)
                    if (m_decor[i][ enCours.y][enCours.x].getMonstre()[o]>-1&&m_decor[i][ enCours.y][enCours.x].getMonstre()[o]<(int)m_monstre.size() && m_decor[i][enCours.y][enCours.x].getMonstre()[o] != exception)
                        if (m_monstre[m_decor[i][ enCours.y][enCours.x].getMonstre()[o]].EnVie()&&m_monstre[m_decor[i][ enCours.y][enCours.x].getMonstre()[o]].getCaracteristique().vitesse>0)
                            if (m_decor[i][enCours.y][enCours.x].getMonstre()[o]<(int)m_monstre.size())
                                if ((m_monstre[m_decor[i][enCours.y][enCours.x].getMonstre()[o]].getProchaineCase().x==positionX&&m_monstre[m_decor[i][enCours.y][enCours.x].getMonstre()[o]].getProchaineCase().y==positionY)||(m_monstre[m_decor[i][enCours.y][enCours.x].getMonstre()[o]].getCoordonnee().x==positionX&&m_monstre[m_decor[i][enCours.y][enCours.x].getMonstre()[o]].getCoordonnee().y==positionY))
                                    return 1;
            enCours.x=positionX;
            enCours.y=positionY+1;
            if (enCours.x>=0&&enCours.y>=0&&enCours.x<m_dimensions.x&&enCours.y<m_dimensions.y)
                for (unsigned o = 0 ; o < m_decor[i][ enCours.y][enCours.x].getMonstre().size() ; ++o)
                    if (m_decor[i][ enCours.y][enCours.x].getMonstre()[o]>-1&&m_decor[i][ enCours.y][enCours.x].getMonstre()[o]<(int)m_monstre.size() && m_decor[i][enCours.y][enCours.x].getMonstre()[o] != exception)
                        if (m_monstre[m_decor[i][ enCours.y][enCours.x].getMonstre()[o]].EnVie()&&m_monstre[m_decor[i][ enCours.y][enCours.x].getMonstre()[o]].getCaracteristique().vitesse>0)
                            if (m_decor[i][enCours.y][enCours.x].getMonstre()[o]<(int)m_monstre.size())
                                if ((m_monstre[m_decor[i][enCours.y][enCours.x].getMonstre()[o]].getProchaineCase().x==positionX&&m_monstre[m_decor[i][enCours.y][enCours.x].getMonstre()[o]].getProchaineCase().y==positionY)||(m_monstre[m_decor[i][enCours.y][enCours.x].getMonstre()[o]].getCoordonnee().x==positionX&&m_monstre[m_decor[i][enCours.y][enCours.x].getMonstre()[o]].getCoordonnee().y==positionY))
                                    return 1;
            enCours.x=positionX+1;
            enCours.y=positionY;
            if (enCours.x>=0&&enCours.y>=0&&enCours.x<m_dimensions.x&&enCours.y<m_dimensions.y)
                for (unsigned o = 0 ; o < m_decor[i][ enCours.y][enCours.x].getMonstre().size() ; ++o)
                    if (m_decor[i][ enCours.y][enCours.x].getMonstre()[o]>-1&&m_decor[i][ enCours.y][enCours.x].getMonstre()[o]<(int)m_monstre.size() && m_decor[i][enCours.y][enCours.x].getMonstre()[o] != exception)
                        if (m_monstre[m_decor[i][ enCours.y][enCours.x].getMonstre()[o]].EnVie()&&m_monstre[m_decor[i][ enCours.y][enCours.x].getMonstre()[o]].getCaracteristique().vitesse>0)
                            if (m_decor[i][enCours.y][enCours.x].getMonstre()[o]<(int)m_monstre.size())
                                if ((m_monstre[m_decor[i][enCours.y][enCours.x].getMonstre()[o]].getProchaineCase().x==positionX&&m_monstre[m_decor[i][enCours.y][enCours.x].getMonstre()[o]].getProchaineCase().y==positionY)||(m_monstre[m_decor[i][enCours.y][enCours.x].getMonstre()[o]].getCoordonnee().x==positionX&&m_monstre[m_decor[i][enCours.y][enCours.x].getMonstre()[o]].getCoordonnee().y==positionY))
                                    return 1;
            enCours.x=positionX-1;
            enCours.y=positionY;
            if (enCours.x>=0&&enCours.y>=0&&enCours.x<m_dimensions.x&&enCours.y<m_dimensions.y)
                for (unsigned o = 0 ; o < m_decor[i][ enCours.y][enCours.x].getMonstre().size() ; ++o)
                    if (m_decor[i][ enCours.y][enCours.x].getMonstre()[o]>-1&&m_decor[i][ enCours.y][enCours.x].getMonstre()[o]<(int)m_monstre.size() && m_decor[i][enCours.y][enCours.x].getMonstre()[o] != exception)
                        if (m_monstre[m_decor[i][ enCours.y][enCours.x].getMonstre()[o]].EnVie()&&m_monstre[m_decor[i][ enCours.y][enCours.x].getMonstre()[o]].getCaracteristique().vitesse>0)
                            if (m_decor[i][enCours.y][enCours.x].getMonstre()[o]<(int)m_monstre.size())
                                if ((m_monstre[m_decor[i][enCours.y][enCours.x].getMonstre()[o]].getProchaineCase().x==positionX&&m_monstre[m_decor[i][enCours.y][enCours.x].getMonstre()[o]].getProchaineCase().y==positionY)||(m_monstre[m_decor[i][enCours.y][enCours.x].getMonstre()[o]].getCoordonnee().x==positionX&&m_monstre[m_decor[i][enCours.y][enCours.x].getMonstre()[o]].getCoordonnee().y==positionY))
                                    return 1;
            enCours.x=positionX;
            enCours.y=positionY-1;
            if (enCours.x>=0&&enCours.y>=0&&enCours.x<m_dimensions.x&&enCours.y<m_dimensions.y)
                for (unsigned o = 0 ; o < m_decor[i][ enCours.y][enCours.x].getMonstre().size() ; ++o)
                    if (m_decor[i][ enCours.y][enCours.x].getMonstre()[o]>-1&&m_decor[i][ enCours.y][enCours.x].getMonstre()[o]<(int)m_monstre.size() && m_decor[i][enCours.y][enCours.x].getMonstre()[o] != exception)
                        if (m_monstre[m_decor[i][ enCours.y][enCours.x].getMonstre()[o]].EnVie()&&m_monstre[m_decor[i][ enCours.y][enCours.x].getMonstre()[o]].getCaracteristique().vitesse>0)
                            if (m_decor[i][enCours.y][enCours.x].getMonstre()[o]<(int)m_monstre.size())
                                if ((m_monstre[m_decor[i][enCours.y][enCours.x].getMonstre()[o]].getProchaineCase().x==positionX&&m_monstre[m_decor[i][enCours.y][enCours.x].getMonstre()[o]].getProchaineCase().y==positionY)||(m_monstre[m_decor[i][enCours.y][enCours.x].getMonstre()[o]].getCoordonnee().x==positionX&&m_monstre[m_decor[i][enCours.y][enCours.x].getMonstre()[o]].getCoordonnee().y==positionY))
                                    return 1;
        }
    }
    return 0;
}

int Map::getTypeCase(int positionX,int positionY)
{
    if (positionY>=0&&positionY<m_dimensions.y&&positionX>=0&&positionX<m_dimensions.x)
    {
        if (m_decor[0][positionY][positionX].getTileset()>=0&&m_decor[0][positionY][positionX].getTileset()<(int)m_tileset.size())
            //if(m_tileset[m_decor[0][positionY][positionX].getTileset()].getCollisionTile(m_decor[0][positionY][positionX].getTile()))
            if ((int)moteurGraphique->getTileset(m_tileset[m_decor[0][positionY][positionX].getTileset()])->getLumiereDuTile(m_decor[0][positionY][positionX].getTile()).hauteur>32)
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
            if ((int)moteurGraphique->getTileset(m_tileset[m_decor[1][positionY][positionX].getTileset()])->getLumiereDuTile(m_decor[1][positionY][positionX].getTile()).hauteur>32)
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


    return 0;
}

bool Map::getCollisionPousse(int positionX,int positionY, int id)
{
    if (positionY>=0&&positionY<m_dimensions.y&&positionX>=0&&positionX<m_dimensions.x)
    {
        if (m_decor[0][positionY][positionX].getTileset()>=0&&m_decor[0][positionY][positionX].getTileset()<(int)m_tileset.size())
            if(moteurGraphique->getTileset(m_tileset[m_decor[0][positionY][positionX].getTileset()])->getCollisionTile(m_decor[0][positionY][positionX].getTile()))
                return 1;

        if (m_decor[1][positionY][positionX].getTileset()>=0&&m_decor[1][positionY][positionX].getTileset()<(int)m_tileset.size())
            if(moteurGraphique->getTileset(m_tileset[m_decor[1][positionY][positionX].getTileset()])->getCollisionTile(m_decor[1][positionY][positionX].getTile()))
                return 1;

        for (unsigned o = 0 ; o < m_decor[1][positionY][positionX].getMonstre().size() ; ++o)
            if (m_decor[1][positionY][positionX].getMonstre()[o]>-1&&m_decor[1][positionY][positionX].getMonstre()[o]<(int)m_monstre.size())
                if (m_monstre[m_decor[1][positionY][positionX].getMonstre()[o]].EnVie()
                 && m_monstre[m_decor[1][positionY][positionX].getMonstre()[o]].m_impenetrable
                 && m_decor[1][positionY][positionX].getMonstre()[o] != id)
                    return 1;
    }
    else
        return 1;
    return 0;
}

void Map::TesterPoussable(Personnage &personnage, float temps, int id)
{
    bool pousser = true;

    if(personnage.getPousse().x != 0 || personnage.getPousse().y != 0)
    if(getCollisionPousse((int)((personnage.getCoordonneePixel().x + personnage.getPousse().x * temps * COTE_TILE * 5 + COTE_TILE * 0.5f * (personnage.getPousse().x >= 0))/COTE_TILE),
                          (int)((personnage.getCoordonneePixel().y + personnage.getPousse().y * temps * COTE_TILE * 5 + COTE_TILE * 0.5f * (personnage.getPousse().y >= 0))/COTE_TILE),
                           id))
    {
        pousser = false;
        if(personnage.getPousse().x != 0
        && !getCollisionPousse((int)((personnage.getCoordonneePixel().x + personnage.getPousse().x * temps * COTE_TILE * 5 + COTE_TILE * 0.5f * (personnage.getPousse().x >= 0))/COTE_TILE),
                               (int)((personnage.getCoordonneePixel().y + COTE_TILE * 0.5f * (personnage.getPousse().y >= 0))/COTE_TILE),
                                id))
            pousser = true, personnage.setPousse(coordonneeDecimal(personnage.getPousse().x, 0));
        else if(personnage.getPousse().y != 0
             && !getCollisionPousse((int)((personnage.getCoordonneePixel().x + COTE_TILE * 0.5f * (personnage.getPousse().x >= 0))/COTE_TILE),
                                    (int)((personnage.getCoordonneePixel().y + personnage.getPousse().y * temps * COTE_TILE * 5 + COTE_TILE * 0.5f * (personnage.getPousse().y >= 0) )/COTE_TILE),
                                    id))
            pousser = true, personnage.setPousse(coordonneeDecimal(0, personnage.getPousse().y));
        else if(personnage.getPousse().x != 0)
        {
            if(!getCollisionPousse((int)((personnage.getCoordonneePixel().x + COTE_TILE * 0.5f * (personnage.getPousse().x >= 0))/COTE_TILE),
                                    (int)((personnage.getCoordonneePixel().y + personnage.getPousse().x * temps * COTE_TILE * 5 + COTE_TILE * 0.5f * (personnage.getPousse().x >= 0))/COTE_TILE),
                                    id))
                pousser = true, personnage.setPousse(coordonneeDecimal(0, personnage.getPousse().x));
            else if(!getCollisionPousse((int)((personnage.getCoordonneePixel().x + COTE_TILE * 0.5f * (personnage.getPousse().x >= 0))/COTE_TILE),
                                        (int)((personnage.getCoordonneePixel().y - personnage.getPousse().x * temps * COTE_TILE * 5 + COTE_TILE * 0.5f * (personnage.getPousse().x >= 0))/COTE_TILE),
                                        id))
                pousser = true, personnage.setPousse(coordonneeDecimal(0, -personnage.getPousse().x));
        }
        else if(personnage.getPousse().y != 0)
        {
            if(!getCollisionPousse((int)((personnage.getCoordonneePixel().x + personnage.getPousse().y * temps * COTE_TILE * 5 + COTE_TILE * 0.5f * (personnage.getPousse().y >= 0))/COTE_TILE),
                                   (int)((personnage.getCoordonneePixel().y + COTE_TILE * 0.5f * (personnage.getPousse().y >= 0))/COTE_TILE),
                                    id))
                pousser = true, personnage.setPousse(coordonneeDecimal(personnage.getPousse().y, 0));
            else if(!getCollisionPousse((int)((personnage.getCoordonneePixel().x - personnage.getPousse().y * temps * COTE_TILE * 5 + COTE_TILE * 0.5f * (personnage.getPousse().y >= 0))/COTE_TILE),
                                        (int)((personnage.getCoordonneePixel().y + COTE_TILE * 0.5f * (personnage.getPousse().y >= 0))/COTE_TILE),
                                        id))
                pousser = true, personnage.setPousse(coordonneeDecimal(-personnage.getPousse().y, 0));
        }
    }
}

int Map::getMonstre(Hero *hero,coordonnee positionSouris,coordonnee casePointee)
{
    float distance=100000;
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
                        if (m_monstre[m_decor[1][j][k].getMonstre()[o]].EnVie()
                            &&m_monstre[m_decor[1][j][k].getMonstre()[o]].getCaracteristique().rang>=0
                            &&m_monstre[m_decor[1][j][k].getMonstre()[o]].m_actif)
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
    return meilleur;
}

const  std::string  &Map::getNom()
{
    return m_nom_fichier;
}
