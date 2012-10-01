
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
#include <dirent.h>


using namespace std;
using namespace sf;

#include "../globale.h"
#include "../jeu.h"


void Map::Detruire()
{
    moteurGraphique->LightManager->Delete_All_Light();
    moteurGraphique->LightManager->Delete_All_Wall();

    console->Ajouter("");
    console->Ajouter("Destroying the map...");

    m_tileset.clear();
    m_herbe.clear();

    if (configuration->debug)
        console->Ajouter("Destroying tiles...");

    for (int i=0;i<NOMBRE_COUCHE_MAP;++i)
    {
        for (int j=0;j<(int)m_decor[i].size();j++)
            m_decor[i][j].clear();
        m_decor[i].clear();
    }

    if (configuration->debug)
        console->Ajouter("Tiles destroyed !");

    m_ModeleMonstre.clear();

    if (configuration->debug)
        console->Ajouter("Monster's models destroyed !");

    for(int i = 0 ; i < m_monstre.size() ; ++i)
        delete m_monstre[i];
    m_monstre.clear();

    if (configuration->debug)
        console->Ajouter("Monsters destroyed !");

    m_fond.clear();
    if (configuration->debug)
        console->Ajouter("Funds destroyed !");

    m_musiques.clear();
    if (configuration->debug)
        console->Ajouter("Pathes to music content destroyed !");

    m_ambiances.clear();

    m_projectile.clear();
    if (configuration->debug)
        console->Ajouter("Projectiles destroyed !");

    m_effets.clear();
    if (configuration->debug)
        console->Ajouter("Effets destroyed !");

    console->Ajouter("Map destroyed !");
}

bool Map::Charger(std::string nomMap,Hero *hero)
{
    for(int y = 0 ; y < MINIMAP_SIZE ; ++y)
    for(int x = 0 ; x < MINIMAP_SIZE ; ++x)
        m_render_minimap[y][x].clear(sf::Color(0,0,0,0));

    m_dimensions.x=0;
    m_dimensions.y=0;

   // int numeroModuleAleatoire=rand()%10;

    bool entite_map_existante=false,mapExistante=false;

    m_nom_fichier=nomMap;
    string chemin = nomMap,chemin2 = configuration->chemin_temps+nomMap;

    m_lumiere[0].intensite=1;
    m_lumiere[0].rouge=0;
    m_lumiere[0].vert=0;
    m_lumiere[0].bleu=0;

    m_musiqueEnCours=0;

    m_etat_chargement = 0;

    console->Ajouter("",0);
    console->Ajouter("Loading the map : "+nomMap);

    cDAT reader,reader2;

    reader.Read(configuration->chemin_saves+hero->m_chemin_save);

    if (reader.IsFileExist(configuration->chemin_temps+nomMap))
        mapExistante=true,entite_map_existante=true,console->Ajouter("Map does exist in savegame.");

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
        ChargerInfos(fichier, chemin);

        if (entite_map_existante==true)
            ChargerEntites(fichier2, chemin);

        m_etat_chargement = 3;

        ChargerCases(fichier, chemin, entite_map_existante, hero);

        m_script.Charger(*fichier);

        fichier->close();
    }
    else
    {
        console->Ajouter("Can not open file : "+chemin,1);
        throw "";
    }

    m_etat_chargement = 4;


    for (int i=0;i<24;++i)
        if (m_lumiere[i].intensite<1)
            m_lumiere[i].intensite=1;

    delete fichier;
    delete fichier2;

    m_dimensions.x=(int)m_decor[0][0].size();
    m_dimensions.y=(int)m_decor[0].size();

    console->Ajouter("Loading the map ended.");

    m_loaded = true;

    return 1;
}

void Map::ChargerInfos(ifstream *fichier, const std::string &chemin)
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
                    console->Ajouter("Error : Map \" "+chemin+" \" is invalid",1);
                    throw ("Error : Map \" "+chemin+" \" is invalid");
                }
            }
            while (caractere!='$');

            m_nom = gettext(configuration->getText(5,m_no_nom).c_str());
            fichier->get(caractere);
        }

        if (fichier->eof())
        {
            console->Ajouter("Error : Map \" "+chemin+" \" is invalid",1);
            throw ("Error : Map \" "+chemin+" \" is invalid");
        }
    }
    while (caractere!='$');
    if (configuration->debug)
        console->Ajouter("/Reading name.");

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
            console->Ajouter("Error : Map \" "+chemin+" \" is invalid",1);
            throw ("Error : Map \" "+chemin+" \" is invalid");
        }
    }
    while (caractere!='$');

    if (configuration->debug)
        console->Ajouter("/Reading funds.");

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
            console->Ajouter("Error : Map \" "+chemin+" \" is invalid",1);
            throw ("Error : Map \" "+chemin+" \" is invalid");
        }
    }
    while (caractere!='$');

    if (configuration->debug)
        console->Ajouter("/Reading music.");

    do
    {
        fichier->get(caractere);
        if (caractere=='*')
        {
            do
            {
                fichier->get(caractere);
                if (caractere=='m')
                {
                    string cheminSon;
                    *fichier>>cheminSon;

                    m_ambiancesPath.push_back(cheminSon);
                    m_ambiances.push_back(moteurSons->AjouterBuffer(cheminSon));
                    m_volumeAmbiances.push_back(0);
                    m_playAmbiances.push_back(true);
                    m_startPlayAmbiances.push_back(true);
                }
                else if (caractere=='p')
                {
                    int temp;
                    *fichier>>temp;
                    m_playAmbiances.back() = temp;
                    m_startPlayAmbiances.back() = temp;
                }

                if (fichier->eof())
                    caractere='$';
            }
            while (caractere!='$');

            fichier->get(caractere);
        }
        if (fichier->eof())
            caractere='$';
    }
    while (caractere!='$');

    if (configuration->debug)
        console->Ajouter("/Reading environments.");


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
            console->Ajouter("Error : Map \" "+chemin+" \" is invalid",1);
            throw ("Error : Map \" "+chemin+" \" is invalid");
        }
    }
    while (caractere!='$');

    if (configuration->debug)
        console->Ajouter("/Reading lights.");

    m_img_sky = -1;

    do
    {
        fichier->get(caractere);
        if (caractere=='*')
        {
            *fichier>>m_nom_img_sky;
            m_img_sky = moteurGraphique->AjouterImage(m_nom_img_sky, 3);
        }
        if (fichier->eof())
        {
            console->Ajouter("Error : Map \" "+chemin+" \" is invalid",1);
            throw ("Error : Map \" "+chemin+" \" is invalid");
        }
    }
    while (caractere!='$');

    do
    {
        fichier->get(caractere);
        if (caractere=='*')
        {
            std::string temp;
            *fichier>>temp;
            m_climates.push_back(temp);
        }
        if (fichier->eof())
        {
            console->Ajouter("Error : Map \" "+chemin+" \" is invalid",1);
            throw ("Error : Map \" "+chemin+" \" is invalid");
        }
    }
    while (caractere!='$');

    if (configuration->debug)
        console->Ajouter("/Reading skies.");



    while (heureEnCours<24)
    {
        m_lumiere[heureEnCours].rouge=m_lumiere[0].rouge;
        m_lumiere[heureEnCours].vert=m_lumiere[0].vert;
        m_lumiere[heureEnCours].bleu=m_lumiere[0].bleu;
        m_lumiere[heureEnCours].intensite=m_lumiere[0].intensite;
        m_lumiere[heureEnCours].hauteur=m_lumiere[0].hauteur;
        heureEnCours++;
    }

    m_etat_chargement = 1;

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
            console->Ajouter("Error : Map \" "+chemin+" \" is invalid",1);
            throw ("Error : Map \" "+chemin+" \" is invalid");
        }

    }
    while (caractere!='$');

    if (configuration->debug)
        console->Ajouter("/Reading tilesets.");

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
            console->Ajouter("Error : Map \" "+chemin+" \" is invalid",1);
            throw ("Error : Map \" "+chemin+" \" is invalid");
        }

    }
    while (caractere!='$');

    if (configuration->debug)
        console->Ajouter("/Reading herbs.");

    m_etat_chargement = 2;

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
                console->Ajouter("Loading : "+cheminDuMonstre+" ended",0);

            }
            if (fichier->eof())
            {
                console->Ajouter("Error : Map \" "+chemin+" \" is invalid",1);
                throw ("Error : Map \" "+chemin+" \" is invalid");
            }
        }
        while (caractere!='$');

        if (configuration->debug)
            console->Ajouter("/Reading monsters.");

        console->Ajouter("");
}

void Map::ChargerEntites(ifstream *fichier2, const std::string &chemin)
{
    console->Ajouter("A map of already existing entities.");
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
                int numeroModele=-1,vieMin=0,vieMax=1,degatsMin=0,degatsMax=0,rang=0,ame=0,pose=0,etat=0,angle=0;
                float taille=1;
                bool etat_force = false;
                vector <Objet> objets;
                std::vector <int> variables;
                m_monstre.push_back(new Monstre ());
                m_monstre.back()->m_no = m_monstre.size() - 1;

                int pos;

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
                            *fichier2>>vieMin;
                        else if (caractere=='a')
                            *fichier2>>vieMax;
                        break;
                    case 'd':
                        fichier2->get(caractere);
                        if (caractere=='i')
                            *fichier2>>degatsMin;
                        else if (caractere=='a')
                            *fichier2>>degatsMax;
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

                    case 'i':
                        *fichier2>>pose;
                        break;
                    case 'e':
                        *fichier2>>etat;
                        break;
                    case 'f':
                        *fichier2>>etat_force;
                        break;
                    case 'g':
                        *fichier2>>angle;
                        break;



                    case 'p':
                        m_monstre.back()->m_entite_graphique.LoadParameters(*fichier2);
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
                        if(no >= (int)variables.size())
                            variables.resize(no + 1);
                        *fichier2>>variables[no];
                        break;


                    }

                    if (fichier2->eof())
                    {
                        console->Ajouter("Error : Map \" "+chemin+" \" is invalid",1);
                        throw ("Error : Map \" "+chemin+" \" is invalid");
                    }
                }
                while (caractere!='$');

                if ((int)m_monstre.size()>0)
                {
                    if (numeroModele>=0&&numeroModele<(int)m_ModeleMonstre.size())
                        m_monstre.back()->Charger(numeroModele,&m_ModeleMonstre[numeroModele]);
                    Caracteristique caracteristique = m_monstre.back()->getCaracteristique();
                    caracteristique.vie=vieMin;
                    caracteristique.maxVie=vieMax;
                    caracteristique.degatsMin[PHYSIQUE]=degatsMin;
                    caracteristique.degatsMax[PHYSIQUE]=degatsMax;
                    caracteristique.rang=rang;
                    caracteristique.pointAme=ame;
                    caracteristique.modificateurTaille=taille;
                    m_monstre.back()->setCaracteristique(caracteristique);
                    m_monstre.back()->setEtat(etat);
                    m_monstre.back()->setPose(pose);
                    m_monstre.back()->setForcedAngle(angle);
                    m_monstre.back()->setObjets(objets);
                    m_monstre.back()->m_etatForce = etat_force;
                    for(unsigned z=0;z<variables.size();++z)
                        m_monstre.back()->m_scriptAI.setVariable(z,variables[z]);
                }

                objets.clear();

                fichier2->get(caractere);
            }

            if (fichier2->eof())
            {
                console->Ajouter("Error : Map \" "+chemin+" \" is invalid",1);
                throw ("Error : Map \" "+chemin+" \" is invalid");
            }
        }
        while (caractere!='$');
        fichier2->close();
    }
    if (configuration->debug)
        console->Ajouter("/Reading monsters if existing.");
}

void Map::ChargerCases(ifstream *fichier, const std::string &chemin, bool entite_map_existante, Hero *hero)
{
    //Decor decorTemp(-1,-1,std::vector<int> (),-1,0,0);
    char caractere;

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
                    int tileset=-1,tileFinal=-1,herbe=-1,layer=0,hauteur=0;
                    int temp, id = -1;
                    vector <int>tile;
                    vector <int>monstre, monstreFinal;
                    vector <Objet> objets;
                    bool added_minimap = false;
                    int entite_angle = 0;

                    Entite_graphique entite_decor;
                    vector <Entite_graphique> entite_monstre;
                    Entite_graphique entite_monstre_final;
                    bool dernierEtaitMonstre = false;

                    if(couche > 0)
                    if(position.y < (int)m_decor[0].size())
                    if(position.x < (int)m_decor[0][position.y].size())
                    position.h = m_decor[0][position.y][position.x].getHauteur();

                    do
                    {

                        switch (caractere)
                        {

                        case '+':
                            fichier->get(caractere);
                            if(caractere == 'm')
                            {
                                if (entite_map_existante!=true)
                                    AddMonstre(monstre, monstreFinal, entite_monstre, entite_monstre_final, position,
                                               id, entite_angle, hero->m_classe.position_contenu_marchand.w);

                                int temp3;
                                *fichier>>temp3;
                                monstre.push_back(temp3);
                                entite_monstre.push_back(Entite_graphique ());
                                dernierEtaitMonstre = true;
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
                            *fichier>>temp;
                            monstre.push_back(temp);
                            entite_monstre.push_back(Entite_graphique ());
                            dernierEtaitMonstre = true;
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
                            position.h = hauteur;
                            break;
                        case 'c':
                            *fichier>>added_minimap;
                            break;
                        case 'a':
                            *fichier>>entite_angle;
                            break;
                        case 'p':
                            if(dernierEtaitMonstre)
                                entite_monstre.back().LoadParameters(*fichier);
                            else
                                entite_decor.LoadParameters(*fichier);
                            break;


                        case 'o':
                            pos=fichier->tellg();
                            objets.push_back(Objet ());
                            objets.back().ChargerTexte(fichier,tempCaract,true);
                            objets.back().Charger(objets.back().getChemin(),tempCaract,true);
                            fichier->seekg(pos, ios::beg);
                            objets.back().m_benedictions.clear();
                            objets.back().ChargerTexte(fichier,tempCaract);
                            objets.back().m_alpha = 0;
                            objets.back().m_hauteur = 0;
                            objets.back().m_monter = 0;
                            objets.back().m_rotation = 0;

                            break;
                        }
                        fichier->get(caractere);
                        if (fichier->eof())
                        {
                            console->Ajouter("Error : Map \" "+chemin+" \" is invalid",1);
                            throw ("Error : Map \" "+chemin+" \" is invalid");
                        }
                    }
                    while (caractere!='|' && caractere!='$');

                    if (caractere!='$')
                    {
                        if (couche==1)
                            if (m_decor[0][position.y][position.x].getHerbe()>=0&&herbe<0)
                                herbe=m_decor[0][position.y][position.x].getHerbe();

                        if (entite_map_existante!=true)
                            AddMonstre(monstre, monstreFinal, entite_monstre, entite_monstre_final, position,
                                       id, entite_angle, hero->m_classe.position_contenu_marchand.w);
                        else
                        {
                            for (unsigned i = 0 ; i < monstre.size() ; ++i)
                            {
                                if (monstre[i]>=0&&monstre[i]<(int)m_monstre.size())
                                {
                                    int etat,pose,angle;
                                    etat=m_monstre[monstre[i]]->getEtat();
                                    pose=m_monstre[monstre[i]]->getPose();
                                    angle=m_monstre[monstre[i]]->getAngle();
                                    m_monstre[monstre[i]]->setCoordonnee(position);
                                    m_monstre[monstre[i]]->setDepart();
                                    m_monstre[monstre[i]]->setEtat(etat);
                                    m_monstre[monstre[i]]->setPose(pose);
                                    m_monstre[monstre[i]]->setAngle(angle);

                                    m_monstre[monstre[i]]->m_ID = id;

                                    if(id >= 0)
                                    {
                                        if((int)m_listID.size() <= id)
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
                            console->Ajouter("Error : Map \" "+chemin+" \" is invalid",1);
                            throw ("Error : Map \" "+chemin+" \" is invalid");
                        }

                        if (couche==0)
                            layer+=1;
                        else
                            layer+=10;

                        m_decor[couche][position.y].push_back(Decor (tileset,tileFinal,monstreFinal,herbe,layer,hauteur,objets));
                        m_decor[couche][position.y].back().added_minimap = added_minimap;
                        m_decor[couche][position.y].back().m_entite_graphique = entite_decor;


                        tileset=-1,tile.clear(),tileFinal=-1,monstreFinal.clear(),herbe=-1,layer=0,hauteur=0;
                        position.h = 0;
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
                console->Ajouter("Error : Map \" "+chemin+" \" is invslid",1);
                throw ("Error : Map \" "+chemin+" \" is invalid");
            }
        }
        while (caractere!='$');
    }
    if (configuration->debug)
        console->Ajouter("/Reading the boxes.");
}

void Map::AddMonstre(vector <int> &monstre, vector <int> &monstreFinal, std::vector <Entite_graphique> &entite_monstre,
                     Entite_graphique &entite_monstre_final, coordonnee &position, int &id, int &entite_angle, int w)
{
    if (!monstre.empty())
    {
        int random = rand() % (int)monstre.size();
        if (random>=0&&random<(int)monstre.size())
            monstreFinal.push_back(monstre[random]), entite_monstre_final = entite_monstre[random];

        monstre.clear();
        entite_monstre.clear();
    }

    if (!monstreFinal.empty())
    {
        if (monstreFinal.back()>=0&&monstreFinal.back()<(int)m_ModeleMonstre.size())
        {
            m_monstre.push_back(new Monstre ());
            m_monstre.back()->m_no = m_monstre.size() - 1;
            m_monstre.back()->Charger(monstreFinal.back(),&m_ModeleMonstre[monstreFinal.back()]);
            m_monstre.back()->setCoordonnee(position),m_monstre.back()->setDepart();
            m_monstre.back()->m_ID = id;
            m_monstre.back()->m_entite_graphique = entite_monstre_final;
            m_monstre.back()->setForcedAngle(entite_angle);
            entite_angle = 0;

            if(id >= 0)
            {
                if((int)m_listID.size() <= id)
                    m_listID.resize(id + 1);
                m_listID[id].push_back(m_monstre.size() - 1);
            }

            TrierInventaire(m_monstre.back()->getPointeurObjets(),w/*hero->m_classe.position_contenu_marchand.w*/);

            monstreFinal.back()=m_monstre.size()-1;
        }
        else
            monstreFinal.clear();
    }

    monstre.clear();
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
                        m_decor[couche][i][j].m_entite_herbe.setPosition(position.x, position.y - m_decor[0][i][j].getHauteur());
                        m_decor[couche][i][j].m_entite_herbe.setScale((float)m_decor[couche][i][j].getTailleHerbe()/100,(float)m_decor[couche][i][j].getTailleHerbe()/100);
                        m_decor[couche][i][j].m_entite_herbe.m_color = m_decor[couche][i][j].getCouleurHerbe();

                        m_decor[couche][i][j].m_entite_herbe.Initialiser(coordonnee ());
                    }
                    else
                        m_decor[couche][i][j].m_entite_herbe.m_tileset = NULL;
                }
            }

    if (configuration->debug)
        console->Ajouter("Initializing the grass ended.");

    for (unsigned i=0;i<NOMBRE_COUCHE_MAP;++i)
        for (unsigned j=0;j<m_decor[i].size();j++)
            for (unsigned k=0;k<m_decor[i][j].size();k++)
                if (m_decor[i][j][k].getTileset()>=0&&m_decor[i][j][k].getTileset()<(int)m_tileset.size())
                {
                    coordonnee position, pos;

                    position.x=(k-j)*64;
                    position.y=(k+j+1)*32;

                    pos.x = position.x;
                    pos.y = position.y * 2;

                    Entite_graphique temp = m_decor[i][j][k].m_entite_graphique;
                    m_decor[i][j][k].m_entite_graphique = moteurGraphique->getEntiteGraphique(m_tileset[m_decor[i][j][k].getTileset()], m_decor[i][j][k].getTile(), m_decor[i][j][k].getCouche());
                    m_decor[i][j][k].m_entite_graphique.setPosition(position.x, position.y);

                    m_decor[i][j][k].m_entite_graphique.setColor(
                            sf::Color(m_decor[i][j][k].m_entite_graphique.getColor().r * m_decor[i][j][k].m_entite_graphique.m_color.r / 255,
                                      m_decor[i][j][k].m_entite_graphique.getColor().g * m_decor[i][j][k].m_entite_graphique.m_color.g / 255,
                                      m_decor[i][j][k].m_entite_graphique.getColor().b * m_decor[i][j][k].m_entite_graphique.m_color.b / 255,
                                      m_decor[i][j][k].m_entite_graphique.getColor().a * m_decor[i][j][k].m_entite_graphique.m_color.a / 255));
                    m_decor[i][j][k].m_entite_graphique.SetParameters(temp);
                    m_decor[i][j][k].m_entite_graphique.Initialiser(pos);
                    m_decor[i][j][k].m_entite_graphique.Generer();

                    CreerSprite(sf::Vector3f(k,j,i));
                }
                else
                    m_decor[i][j][k].m_entite_graphique.m_tileset = NULL;

    if (configuration->debug)
        console->Ajouter("Initializing scenery ended.");

    for (int i=0;i<(int)m_monstre.size();++i)
       // if (m_monstre[i]->getCaracteristique().rang>=0)
        {
            coordonnee pos;

            pos.x = (int)(((m_monstre[i]->getCoordonnee().x-m_monstre[i]->getCoordonnee().y)*64));
            pos.y = (int)(((m_monstre[i]->getCoordonnee().x+m_monstre[i]->getCoordonnee().y+1)*32));

            m_monstre[i]->m_entite_graphique.setPosition(pos.x, pos.y);

            if(m_monstre[i]->getModele() >= 0
            && m_monstre[i]->getModele() < (int)m_ModeleMonstre.size())
                m_monstre[i]->Animer(&m_ModeleMonstre[m_monstre[i]->getModele()],0);
            m_monstre[i]->m_entite_graphique.Initialiser(pos);
        }

    if (configuration->debug)
        console->Ajouter("Initializing entities ended.");


    moteurGraphique->LightManager->Generate();
}

void Map::InitialiserMinimap()
{
    for (int i=0;i<NOMBRE_COUCHE_MAP;++i)
        for (int j=0;j<m_dimensions.y;j++)
            for (int k=0;k<m_dimensions.x;k++)
                if (m_decor[i][j][k].getTileset()>=0&&m_decor[i][j][k].getTileset()<(int)m_tileset.size())
                {
                    if (m_decor[i][j][k].added_minimap)
                    {
                        int cur_y = 0;
                        int cur_x = MINIMAP_SIZE / 2 ;

                        sf::Sprite minimap = m_decor[i][j][k].m_spriteMinimap;

                        minimap.setPosition((minimap.getPosition().x) * 0.125f,
                                            (minimap.getPosition().y) * 0.125f);

                        while(minimap.getPosition().y > 1024)
                            minimap.move(0, -1024), cur_y ++;
                        while(minimap.getPosition().x > 1024)
                            minimap.move(-1024, 0), cur_x ++;
                        while(minimap.getPosition().x - minimap.getGlobalBounds().width / 2 < 0)
                            minimap.move(1024, 0), cur_x --;
                        while(minimap.getPosition().y - minimap.getGlobalBounds().height / 2 < 0)
                            minimap.move(0, 1024), cur_y --;


                        if(m_render_minimap[cur_y][cur_x].getSize().y != 1024)
                        {
                            m_render_minimap[cur_y][cur_x].create(1024,1024);
                            m_render_minimap[cur_y][cur_x].clear(sf::Color(0,0,0,0));
                        }
                        m_render_minimap[cur_y][cur_x].draw(minimap);

                        bool redraw = false;

                        if(minimap.getPosition().y + minimap.getGlobalBounds().height > 1024)
                            cur_y++, minimap.move(0,-1024), redraw = true;
                        if(minimap.getPosition().x + minimap.getGlobalBounds().width > 1024)
                            cur_x++, minimap.move(-1024,0), redraw = true;


                        if(m_render_minimap[cur_y][cur_x].getSize().y != 1024)
                        {
                            m_render_minimap[cur_y][cur_x].create(1024,1024);
                            m_render_minimap[cur_y][cur_x].clear(sf::Color(0,0,0,0));
                        }
                        if(redraw)
                            m_render_minimap[cur_y][cur_x].draw(minimap);
                    }
                }

    for(int y = 0 ; y < MINIMAP_SIZE ; ++y)
    for(int x = 0 ; x < MINIMAP_SIZE ; ++x)
        m_render_minimap[y][x].display();
}

void Map::CreerSprite(sf::Vector3f position_case)
{
    int x = (int)position_case.x;
    int y = (int)position_case.y;
    int z = (int)position_case.z;

    coordonnee position,positionPartieDecor;

    position.x=(x-y-1)*64;
    position.y=(x+y)*32;

    if(m_decor[z][y][x].getTileset() >= 0
     &&m_decor[z][y][x].getTileset() < (int)m_tileset.size())
    {
        positionPartieDecor=moteurGraphique->getTileset(m_tileset[m_decor[z][y][x].getTileset()])->getPositionMinimap(m_decor[z][y][x].getTile());
        m_decor[z][y][x].m_spriteMinimap.setTexture(*moteurGraphique->getImage(moteurGraphique->getTileset(m_tileset[m_decor[z][y][x].getTileset()])->getMinimap(m_decor[z][y][x].getTile())));
        m_decor[z][y][x].m_spriteMinimap.setTextureRect(IntRect(positionPartieDecor.x, positionPartieDecor.y,
                                                            positionPartieDecor.w, positionPartieDecor.h));
        m_decor[z][y][x].m_spriteMinimap.setPosition(position.x, position.y);
        m_decor[z][y][x].m_spriteMinimap.setOrigin(positionPartieDecor.w/2, positionPartieDecor.h/2);
        m_decor[z][y][x].m_spriteMinimap.setColor(sf::Color(255,255,255,255));
        if(m_decor[z][y][x].m_entite_graphique.m_scale.x < 0)
            m_decor[z][y][x].m_spriteMinimap.setScale(-1,1);
    }
}


void Map::Sauvegarder(Hero *hero)
{
    string chemin = configuration->chemin_temps+m_nom_fichier;

    console->Ajouter("",0);
    console->Ajouter("Saving the map : "+chemin,0);

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
        for (unsigned i=0;i< m_fond.size();++i)
            fichier<<"*"<<m_fond[i]<<"\n";
        fichier<<"$\n";
        for (unsigned i=0;i< m_musiques.size();++i)
            fichier<<"*"<<m_musiques[i]<<"\n";
        fichier<<"$\n";
        for (unsigned i=0;i< m_ambiancesPath.size();++i)
            fichier<<"* m"<<m_ambiancesPath[i]<<" p"<<m_startPlayAmbiances[i]<<" $\n";
        fichier<<"$\n";

        for (int i=0;i<24;++i)
            fichier<<"*"<<m_lumiere[i].rouge<<" "<<m_lumiere[i].vert<<" "<<m_lumiere[i].bleu<<" "<<m_lumiere[i].intensite<<" "<<m_lumiere[i].hauteur<<"\n";

        fichier<<"$\n";

        if(!m_nom_img_sky.empty())
            fichier<<" *"<<m_nom_img_sky<<endl;
        fichier<<"$\n";

        for(unsigned i = 0 ; i < m_climates.size() ; ++i)
            if(!m_climates[i].m_passif)
                fichier<<" *"<<m_climates[i].m_chemin<<endl;
        fichier<<"$\n";

        for (unsigned i=0;i<m_tileset.size();++i)
            fichier<<"*"<<moteurGraphique->getTileset(m_tileset[i])->getChemin()<<"\n";

        fichier<<"$\n";

        for (unsigned i=0;i<m_herbe.size();++i)
            fichier<<"*"<<moteurGraphique->getTileset(m_herbe[i])->getChemin()<<"\n";

        fichier<<"$\n";

        for (unsigned i=0;i<m_ModeleMonstre.size();++i)
            fichier<<"*"<<m_ModeleMonstre[i].m_chemin<<"\n";

        fichier<<"$\n";

        for (int couche=0;couche<2;couche++)
        {
            for (int i=0;i<m_dimensions.y;++i)
            {
                fichier<<"* ";
                for (int j=0;j<m_dimensions.x;j++)
                {
                    if(m_decor[couche][i][j].getTileset() >= 0)
                        fichier<<" s"<<m_decor[couche][i][j].getTileset()<<" ";
                    if(m_decor[couche][i][j].getTile() >= 0)
                        fichier<<"t"<<m_decor[couche][i][j].getTile()<<" ";

                    m_decor[couche][i][j].m_entite_graphique.SaveParameters(fichier);

                    if(m_decor[couche][i][j].added_minimap)
                        fichier<<"c1 ";

                    if(m_decor[couche][i][j].getHauteur() > 0)
                        fichier<<"i"<<m_decor[couche][i][j].getHauteur()<<" ";

                    for (unsigned k = 0 ; k < m_decor[couche][i][j].getMonstre().size() ; ++k)
                        if (m_decor[couche][i][j].getMonstre()[k] >= 0 && m_decor[couche][i][j].getMonstre()[k] < (int)m_monstre.size())
                            if(!m_monstre[m_decor[couche][i][j].getMonstre()[k]]->m_inexistant)
                            {
                                bool ok = true;

                                for(unsigned o = 0 ; o < hero->m_amis.size(); ++o)
                                    if(hero->m_amis[o] == m_monstre[m_decor[couche][i][j].getMonstre()[k]])
                                        ok = false;

                                if(ok)
                                {
                                    if(m_monstre[m_decor[couche][i][j].getMonstre()[k]]->m_ID >= 0)
                                        fichier<<"d"<<m_monstre[m_decor[couche][i][j].getMonstre()[k]]->m_ID<<" ";

                                    fichier<<"m"<<m_decor[couche][i][j].getMonstre()[k]<<" ";
                                }

                            }

                    if(m_decor[couche][i][j].getHerbe() >= 0)
                        fichier<<"h"<<m_decor[couche][i][j].getHerbe()<<" ";

                    if (couche==0)
                    {
                        if(m_decor[couche][i][j].getCouche()-1 > 0)
                            fichier<<"l"<<m_decor[couche][i][j].getCouche()-1<<" ";
                    }
                    else if(m_decor[couche][i][j].getCouche()-10 > 0)
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
        console->Ajouter("Can not open file : "+chemin,1);
        throw "";
    }




    chemin = configuration->chemin_temps+"entites_map_"+m_nom_fichier+".emap.hs";

    console->Ajouter("Saving the map_entite : "+chemin,0);

    ofstream fichier2(chemin.c_str(), ios::out | ios::trunc);

    ajouter=true;
    for (int i=0;i<(int)hero->m_contenuSave.size();++i)
        if (hero->m_contenuSave[i]==chemin)
            ajouter=false;
    if (ajouter)
        hero->m_contenuSave.push_back(chemin);

    if (fichier2)
    {
        for (std::vector<Monstre*>::iterator iter = m_monstre.begin();iter!=m_monstre.end();++iter)
            (*iter)->Sauvegarder(fichier2);

        fichier2<<"\n$";

        fichier2.close();
    }

    console->Ajouter("Saving the map ended !");
}


