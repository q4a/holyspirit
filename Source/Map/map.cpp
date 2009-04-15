

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


#include "Map.h"

#include <iostream.h>
#include <fstream>
#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <dirent.h>


using namespace std;
using namespace sf;

#include "../Globale.h"
#include "../Jeu.h"

Map::Map()
{
    carreBrun.Create(8*configuration->Resolution.x/800, 8*configuration->Resolution.x/800, Color(128, 64, 0)),carreBleu.Create(8*configuration->Resolution.x/800, 8*configuration->Resolution.x/800, Color(32, 0, 128));
    carreRouge.Create(8*configuration->Resolution.x/800,8*configuration->Resolution.x/800, Color(128, 0, 0)),carreVert.Create(8*configuration->Resolution.x/800, 8*configuration->Resolution.x/800, Color(0, 128, 0));
    carreJaune.Create(8*configuration->Resolution.x/800, 8*configuration->Resolution.x/800, Color(255, 255, 64));

    carreMauve.Create(8*configuration->Resolution.x/800, 8*configuration->Resolution.x/800, Color(160, 0, 160));

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

    if(configuration->debug)
        console->Ajouter("Destruction des cases...");


    if(m_decor!=NULL)
    {
        for(int i=0;i<NOMBRE_COUCHE_MAP;i++)
        {
            if(m_decor[i]!=NULL)
            {
                for(int j=0;j<m_dimensions.y;j++)
                {
                    if(configuration->debug)
                        console->Ajouter(j);
                    if(m_decor[i][j]!=NULL)
                        delete[] m_decor[i][j];
                }
                if(configuration->debug)
                        console->Ajouter("-");
                delete[] m_decor[i];
            }
        }
        delete[] m_decor;
    }


     if(configuration->debug)
        console->Ajouter("Cases détruites !");


    //for(int i=0;i<(int)m_ModeleMonstre.size();i++)
     //  m_ModeleMonstre.erase(m_ModeleMonstre.begin()+1);
    m_ModeleMonstre.clear();

    if(configuration->debug)
        console->Ajouter("Modèles monstres détruits !");

    m_monstre.clear();

    if(configuration->debug)
        console->Ajouter("Monstres détruits !");

    m_evenement.clear();
    if(configuration->debug)
        console->Ajouter("Evénements détruits !");

    for(int i=0;i<MAX_MUSIQUE;i++)
        m_musique[i].Stop();

    m_fond.clear();
    if(configuration->debug)
        console->Ajouter("Fonds détruits !");

    m_cheminMusique.clear();
    if(configuration->debug)
        console->Ajouter("Chemins musiques détruits !");

    m_projectile.clear();
    if(configuration->debug)
        console->Ajouter("Projectiles détruits !");

    m_effets.clear();
    if(configuration->debug)
        console->Ajouter("Effets détruits !");

    console->Ajouter("Map détruite !");
}

bool Map::Charger(int numeroMap,Hero *hero)
{
    m_dimensions.x=0;
    m_dimensions.y=0;

    int numeroModuleAleatoire=rand()%10;

    bool entite_map_existante=0,mapExistante=0;

    m_numero=numeroMap;
	char numero[7];
	string chemin = configuration->chemin_maps,chemin2 = configuration->chemin_temps+"map";

	chemin += numero;
	chemin2 += numero;

	m_lumiere[0].intensite=1;
	m_lumiere[0].rouge=0;
	m_lumiere[0].vert=0;
	m_lumiere[0].bleu=0;

    m_musiqueEnCours=0;

    sprintf(numero,"Chargement de la map : map%i.map.hs",numeroMap);

    console->Ajouter("",0);
	console->Ajouter(numero);

	sprintf(numero,"map%i.map.hs",numeroMap);

    cDAT reader,reader2;

    reader.Read(configuration->chemin_saves+"hero.sav.hs");

    if(reader.IsFileExist(configuration->chemin_temps+numero))
       mapExistante=true,entite_map_existante=true,console->Ajouter("Map sauvée existante.");

	ifstream *fichier=NULL;
	ifstream *fichier2=NULL;

	if(mapExistante==true)
	{
        fichier=reader.GetInfos(configuration->chemin_temps+numero);
	    sprintf(numero,"entites_map%i.emap.hs",numeroMap);
        fichier2=reader.GetInfos(configuration->chemin_temps+numero);
	}
	else
	{
	    reader2.Read(configuration->chemin_maps);
	    sprintf(numero,"map%i.map.hs",numeroMap);
        fichier=reader2.GetInfos(numero);
	}

    if(*fichier)
    {
    	char caractere;
    	do
    	{
    	    //Chargement du nom
    		fichier->get(caractere);
    		if(caractere=='*')
                *fichier>>m_nom;

    		if(fichier->eof()){ char temp[1000]; sprintf(temp,"Erreur : Map \" %s \" Invalide",chemin.c_str());console->Ajouter(temp,1); throw (&temp); }
    	}while(caractere!='$');
    	if(configuration->debug)
            console->Ajouter("/Lectures du nom.");

    	do
        {
            fichier->get(caractere);

            if(caractere=='*')
    		{
    			string nom;
                *fichier>>nom;
                m_fond.push_back(nom);
    		}

            if(fichier->eof()){ char temp[1000]; sprintf(temp,"Erreur : Map \" %s \" Invalide",chemin.c_str());console->Ajouter(temp,1); throw (&temp); }
        }while(caractere!='$');

        if(configuration->debug)
            console->Ajouter("/Lectures des fonds.");

        for(int i=0;i<MAX_MUSIQUE;i++)
                m_musique[i].Stop();

        m_nombreMusique=0;
    	do
    	{
    	    //Chargement des musiques
    		fichier->get(caractere);
    		if(caractere=='*'&&m_musiqueEnCours<MAX_MUSIQUE)
    		{
    		    std::string temp2;

                *fichier>>temp2;

                if(!m_musique[m_musiqueEnCours].OpenFromFile(temp2.c_str()))
                    console->Ajouter("Impossible de charger : "+temp2,1);
                else
                console->Ajouter("Chargement de : "+temp2,0);

                m_cheminMusique.push_back(temp2);

                m_musique[m_musiqueEnCours].SetLoop(false);
                m_musique[m_musiqueEnCours].SetVolume(0);

                m_musiqueEnCours++;
                m_nombreMusique++;
    		}
    		if(fichier->eof()){ char temp[1000]; sprintf(temp,"Erreur : Map \" %s \" Invalide",chemin.c_str()); throw (&temp); }
    	}while(caractere!='$');

    	if(configuration->debug)
            console->Ajouter("/Lectures des musiques.");

    	m_musique[0].Play();

    	m_musiqueEnCours=0;

        int heureEnCours=0;
    	do
    	{

    	    //Chargement de la lumière ambiante
    		fichier->get(caractere);
    		if(caractere=='*')
    		{
    			*fichier>>m_lumiere[heureEnCours].rouge;
    			*fichier>>m_lumiere[heureEnCours].vert;
    			*fichier>>m_lumiere[heureEnCours].bleu;
    			*fichier>>m_lumiere[heureEnCours].intensite;
    			*fichier>>m_lumiere[heureEnCours].hauteur;
    			heureEnCours++;

    			if(heureEnCours>23)
                    heureEnCours=23;
    		}
    		if(fichier->eof()){ char temp[1000]; sprintf(temp,"Erreur : Map \" %s \" Invalide",chemin.c_str());console->Ajouter(temp,1); throw (&temp); }
    	}while(caractere!='$');

    	if(configuration->debug)
            console->Ajouter("/Lectures des lumières.");

    	while(heureEnCours<24)
    	{
    	    m_lumiere[heureEnCours].rouge=m_lumiere[0].rouge;
            m_lumiere[heureEnCours].vert=m_lumiere[0].vert;
            m_lumiere[heureEnCours].bleu=m_lumiere[0].bleu;
            m_lumiere[heureEnCours].intensite=m_lumiere[0].intensite;
            m_lumiere[heureEnCours].hauteur=m_lumiere[0].hauteur;
            heureEnCours++;
    	}

    	Tileset tilesetTemp;
    	do
    	{
    	    //Chargement des tileset
    		fichier->get(caractere);
    		if(caractere=='*')
    		{
    			string cheminDuTileset;
    			*fichier>>cheminDuTileset;
                m_tileset.push_back(tilesetTemp);
                if(!m_tileset.back().Charger(cheminDuTileset))
                    return 0;

    		}
    		if(fichier->eof()){ char temp[1000]; sprintf(temp,"Erreur : Map \" %s \" Invalide",chemin.c_str());console->Ajouter(temp,1); throw (&temp); }

    	}while(caractere!='$');

    	if(configuration->debug)
            console->Ajouter("/Lectures des tilesets.");

    	do
    	{
    	    //Chargement des tileset
    		fichier->get(caractere);
    		if(caractere=='*')
    		{
    			string cheminDuTileset;
                *fichier>>cheminDuTileset;
                m_herbe.push_back(Herbe ());
                if(!m_herbe.back().Charger(cheminDuTileset))
                    return 0;

    		}
    		if(fichier->eof()){ char temp[1000]; sprintf(temp,"Erreur : Map \" %s \" Invalide",chemin.c_str());console->Ajouter(temp,1); throw (&temp); }

    	}while(caractere!='$');

    	if(configuration->debug)
            console->Ajouter("/Lectures des herbes.");

    	if(numeroMap==0)
            for(int i=0;i<5;i++)
                m_tileset.push_back(tilesetTemp);

    	do
    	{

    	    //Chargement des tileset
    		fichier->get(caractere);
    		if(caractere=='*')
    		{
    			string cheminDuMonstre;
                //getline(fichier, cheminDuMonstre);
                *fichier>>cheminDuMonstre;
                m_ModeleMonstre.push_back(Modele_Monstre ());
                m_ModeleMonstre.back().Charger(cheminDuMonstre);
                console->Ajouter("Chargement de : "+cheminDuMonstre+" terminé",0);

    		}
    		if(fichier->eof()){ char temp[1000]; sprintf(temp,"Erreur : Map \" %s \" Invalide",chemin.c_str());console->Ajouter(temp,1); throw (&temp); }
    	}while(caractere!='$');

    	if(configuration->debug)
            console->Ajouter("/Lectures des monstres.");

    	console->Ajouter("");

    	if(entite_map_existante==true)
        {
            console->Ajouter("Une map des entités est déjà existante.");
            if(fichier2)
            {
                char caractere;
                do
                {
                    //Chargement du nom
                    fichier2->get(caractere);
                    if(caractere=='*')
                    {
                        Lumiere lumiere;
                        int numeroModele=-1,vieMin=0,vieMax=1,degatsMin=0,degatsMax=0,rang=0,ame=0,pose=0,etat=0,angle=0;
                        float taille=1;
                        m_monstre.push_back(Monstre ());

                        char caractere;
                        do
                        {
                            //Chargement du nom
                            fichier2->get(caractere);
                            switch(caractere)
                            {
                                case 'm': *fichier2>>numeroModele; break;
                                case 'v': fichier2->get(caractere); if(caractere=='i') { *fichier2>>vieMin; } else if(caractere=='a') { *fichier2>>vieMax; } break;
                                case 'd': fichier2->get(caractere); if(caractere=='i') { *fichier2>>degatsMin; } else if(caractere=='a') { *fichier2>>degatsMax; } break;
                                case 'r': *fichier2>>rang; break;
                                case 'a': *fichier2>>ame; break;
                                case 't': *fichier2>>taille; break;

                                case 'p': *fichier2>>pose; break;
                                case 'e': *fichier2>>etat; break;
                                case 'g': *fichier2>>angle; break;

                                case 'l':
                                    fichier2->get(caractere);
                                    switch(caractere)
                                    {
                                        case 'r': *fichier2>>lumiere.rouge; break;
                                        case 'v': *fichier2>>lumiere.vert; break;
                                        case 'b': *fichier2>>lumiere.bleu; break;
                                        case 'i': *fichier2>>lumiere.intensite; break;
                                    }
                                break;
                            }

                            if(fichier2->eof()){ char temp[1000]; sprintf(temp,"Erreur : Map \" %s \" Invalide",chemin.c_str());console->Ajouter(temp,1); throw (&temp); }
                        }while(caractere!='$');

                        if((int)m_monstre.size()>0)
                        {
                            if(numeroModele>=0&&numeroModele<(int)m_ModeleMonstre.size())
                                m_monstre.back().Charger(numeroModele,&m_ModeleMonstre[numeroModele]);
                            Caracteristique caracteristique = m_monstre.back().getCaracteristique();
                            caracteristique.vie=vieMin;
                            caracteristique.maxVie=vieMax;
                            caracteristique.degatsMin=degatsMin;
                            caracteristique.degatsMax=degatsMax;
                            caracteristique.rang=rang;
                            caracteristique.pointAme=ame;
                            caracteristique.modificateurTaille=taille;
                            m_monstre.back().setCaracteristique(caracteristique);
                            m_monstre.back().setPorteeLumineuse(lumiere);
                            m_monstre.back().setEtat(etat);
                            m_monstre.back().setPose(pose);
                            m_monstre.back().setAngle(angle);
                        }
                    }

                    if(fichier2->eof()){ char temp[1000]; sprintf(temp,"Erreur : Map \" %s \" Invalide",chemin.c_str());console->Ajouter(temp,1); throw (&temp); }
                }while(caractere!='$');
                fichier2->close();
            }

        }

    	m_evenement.clear();

    	do
    	{
    		fichier->get(caractere);
    		if(caractere=='*')
    		{
    		    int numeroEvenement;
    			do
    			{
    				fichier->get(caractere);
    				switch (caractere)
    				{
    					case 'e': *fichier>>numeroEvenement; break;
    				}
    				if(fichier->eof()){ char temp[1000]; sprintf(temp,"Erreur : Map \" %s \" Invalide",chemin.c_str());console->Ajouter(temp,1); throw (&temp); }
    			}while(caractere!='$');
    			m_evenement.push_back(numeroEvenement);

    			//AjouterEvenement(numeroEvenement);

    			int information;
    			do
    			{
    				fichier->get(caractere);
    				if(caractere=='i')
    				{
                        *fichier>>information;

    				    m_evenement.back().AjouterInformation(information);
    				}
    				if(fichier->eof()){ char temp[1000]; sprintf(temp,"Erreur : Map \" %s \" Invalide",chemin.c_str());console->Ajouter(temp,1); throw (&temp); }
    			}while(caractere!='$');

    			fichier->get(caractere);
    		}
    		if(fichier->eof()){ char temp[1000]; sprintf(temp,"Erreur : Map \" %s \" Invalide",chemin.c_str());console->Ajouter(temp,1); throw (&temp); }
    	}while(caractere!='$');

    	if(configuration->debug)
            console->Ajouter("/Lectures des événements.");


    	std::vector<int> evenementTemp;
    	Decor decorTemp(-1,-1,evenementTemp,-1,-1,0,0);

    	coordonnee position;
    	for(int couche=0;couche<NOMBRE_COUCHE_MAP;couche++)
    	{
            position.x=0;
            position.y=0;

            do
            {
                //Chargement des "décors", des cases de la map
                fichier->get(caractere);
                if(caractere=='*')
                {
                    decorBuffer[couche].push_back(std::vector<Decor> ());
                    do
                    {
                        std::vector<int> evenement;
                        int tileset=-1,tileFinal=-1,herbe=-1,monstreFinal=-1,layer=0,hauteur=0;
                        int rarete=0;
                        int temp;
                        vector <int>tile;
                        vector <int>monstre;
                        vector <Objet> objets;

                        do
                        {

                            switch (caractere)
                            {
                                case 's': *fichier>>tileset; break;
                                case 't': *fichier>>temp; tile.push_back(temp); break;
                                case 'e': int temp2; *fichier>>temp2; evenement.push_back(temp2); break;
                                case 'm': if(entite_map_existante!=true) { *fichier>>temp; monstre.push_back(temp);  } else {  *fichier>>monstreFinal; } break;
                                case 'h': *fichier>>herbe; break;
                                case 'l': *fichier>>layer; break;
                                case 'i': *fichier>>hauteur; break;

                                case 'o':
                                    objets.push_back(Objet ());
                                    objets.back().ChargerTexte(fichier);
                                    rarete=objets.back().getRarete();
                                    objets.back().Charger(objets.back().getChemin());
                                    objets.back().setRarete(rarete);

                                break;

                                case 'r':
                                    fichier->get(caractere);
                                    int noModuleCaseMin=-1,noModuleCaseMax=-1;
                                    do
                                    {
                                        if(caractere=='i')
                                            *fichier>>noModuleCaseMin;
                                        else if(caractere=='a')
                                            *fichier>>noModuleCaseMax;

                                        else if(caractere=='*')
                                                do
                                                {
                                                    fichier->get(caractere);

                                                    if(numeroModuleAleatoire>=noModuleCaseMin&&numeroModuleAleatoire<=noModuleCaseMax)
                                                        switch (caractere)
                                                        {
                                                            case 's': *fichier>>tileset; break;
                                                            case 't': *fichier>>temp; tile.push_back(temp); break;
                                                            case 'e': int temp2; *fichier>>temp2; evenement.push_back(temp2); break;
                                                            case 'm': if(entite_map_existante!=true) { *fichier>>temp; monstre.push_back(temp);  } else {  *fichier>>monstreFinal; } break;
                                                            case 'h': *fichier>>herbe; break;
                                                            case 'l': *fichier>>layer; break;
                                                        }

                                                    if(fichier->eof()){ char temp[1000]; sprintf(temp,"Erreur : Map \" %s \" Invalide",chemin.c_str());console->Ajouter(temp,1); throw (&temp); }
                                                }while(caractere!='$');

                                        fichier->get(caractere);

                                        if(fichier->eof()){ char temp[1000]; sprintf(temp,"Erreur : Map \" %s \" Invalide",chemin.c_str());console->Ajouter(temp,1); throw (&temp); }
                                    }while(caractere!='$');
                                break;
                            }
                            fichier->get(caractere);
                            if(fichier->eof()){ char temp[1000]; sprintf(temp,"Erreur : Map \" %s \" Invalide",chemin.c_str());console->Ajouter(temp,1); throw (&temp); }
                        }while(caractere!='|' && caractere!='$');

                        if(caractere!='$')
                        {
                            if(couche==1)
                                if(decorBuffer[0][position.y][position.x].getHerbe()>=0&&herbe<0)
                                    herbe=decorBuffer[0][position.y][position.x].getHerbe();

                            if(entite_map_existante!=true)
                            {
                                if((int)monstre.size()>0)
                                {
                                    int random = rand() % (int)monstre.size();
                                    if(random>=0&&random<(int)monstre.size())
                                        monstreFinal = monstre[random];

                                    monstre.clear();
                                }

                                if(monstreFinal>=0&&monstreFinal<(int)m_ModeleMonstre.size())
                                {
                                    m_monstre.push_back(Monstre ());
                                    m_monstre.back().Charger(monstreFinal,&m_ModeleMonstre[monstreFinal]);
                                    m_monstre.back().setCoordonnee(position),m_monstre.back().setDepart();
                                    monstreFinal=m_monstre.size()-1;
                                }
                                else
                                monstreFinal=-1;
                            }
                            else
                            {
                                if(monstreFinal>=0&&monstreFinal<(int)m_monstre.size())
                                {
                                    int etat,pose,angle;
                                    etat=m_monstre[monstreFinal].getEtat();
                                    pose=m_monstre[monstreFinal].getPose();
                                    angle=m_monstre[monstreFinal].getAngle();
                                    m_monstre[monstreFinal].setCoordonnee(position);
                                    m_monstre[monstreFinal].setDepart();
                                    m_monstre[monstreFinal].setEtat(etat);
                                    m_monstre[monstreFinal].setPose(pose);
                                    m_monstre[monstreFinal].setAngle(angle);
                                }
                            }

                            if(tile.size()>0)
                            {
                                int random = (rand() % (tile.size() -1 - 0 + 1)) + 0;
                                if(random>=0&&random<(int)tile.size())
                                    tileFinal = tile[random];

                                tile.clear();
                            }

                            if(fichier->eof()){ char temp[1000]; sprintf(temp,"Erreur : Map \" %s \" Invalide",chemin.c_str());console->Ajouter(temp,1); throw (&temp); }

                            decorBuffer[couche][position.y].push_back(Decor (tileset,tileFinal,evenement,monstreFinal,herbe,layer,hauteur,objets));

                            tileset=-1,tile.clear(),tileFinal=-1,evenement.clear(),monstreFinal=-1,herbe=-1,layer=0,hauteur=0;
                            objets.clear();
                            position.x++;
                        }

                    }while(caractere!='$');

                    fichier->get(caractere);

                    position.x=0;
                    position.y++;
                }
                if(fichier->eof()){ char temp[1000]; sprintf(temp,"Erreur : Map \" %s \" Invalide",chemin.c_str());console->Ajouter(temp,1);  throw (&temp); }
            }while(caractere!='$');
    	}
    	if(configuration->debug)
            console->Ajouter("/Lectures des cases.");

    	fichier->close();
    }
    else
    {
        console->Ajouter("Impossible d'ouvrir le fichier : "+chemin,1);
        throw "";
    }


    for(int i=0;i<24;i++)
    if(m_lumiere[i].intensite<1)
        m_lumiere[i].intensite=1;

    delete fichier;
    delete fichier2;

    m_dimensions.x=(int)decorBuffer[0][0].size();
    m_dimensions.y=(int)decorBuffer[0].size();

    m_decor = new Decor** [NOMBRE_COUCHE_MAP];
    for(int i=0;i<NOMBRE_COUCHE_MAP;i++)
    {
        m_decor[i] = new Decor* [(int)decorBuffer[i].size()];
        for(int j=0;j<(int)decorBuffer[i].size();j++)
        {
            m_decor[i][j] = new Decor[(int)decorBuffer[i][j].size()];
            for(int k=0;k<(int)decorBuffer[i][j].size();k++)
                m_decor[i][j][k]=decorBuffer[i][j][k];
        }
    }

    console->Ajouter("Chargement de la map terminé.");
    console->Ajouter("");

    //calculerOmbresEtLumieres();

    Initialiser();

    for(int i=0;i<NOMBRE_COUCHE_MAP;i++)
    {
        for(int j=0;j<(int)decorBuffer[i].size();j++)
            decorBuffer[i][j].clear();
        decorBuffer[i].clear();
    }

    return 1;
}


void Map::Initialiser()
{
    calculerOmbresEtLumieres();

    for(int couche=0;couche<2;couche++)
        for(int i=0;i<m_dimensions.y;i++)
            for(int j=0;j<m_dimensions.x;j++)
            {
                if(m_decor[couche][i][j].getHerbe()>=0&&m_decor[couche][i][j].getHerbe()<(int)m_herbe.size())
                {
                    if(m_herbe[m_decor[couche][i][j].getHerbe()].getTaille()>0)
                    {
                        coordonnee position,positionPartieDecor;

                        int numeroHerbe=0;
                        if(m_herbe[m_decor[couche][i][j].getHerbe()].getTaille()>0)
                            numeroHerbe = (rand() % (m_herbe[m_decor[couche][i][j].getHerbe()].getTaille()));
                        m_decor[couche][i][j].setNumeroHerbe(numeroHerbe);

                        position.x=(j-i-1+m_dimensions.y)*64;
                        position.y=(j+i)*32;

                        positionPartieDecor=m_herbe[m_decor[couche][i][j].getHerbe()].getPositionDuTile(m_decor[couche][i][j].getNumeroHerbe());

                        if(couche==0)
                            position.y-=32;
                        position.x+=m_decor[0][i][j].getDecalageHerbe().x;

                        m_decor[couche][i][j].m_spriteHerbe.SetImage(*moteurGraphique->getImage(m_herbe[m_decor[couche][i][j].getHerbe()].getImage(m_decor[couche][i][j].getNumeroHerbe())));
                        m_decor[couche][i][j].m_spriteHerbe.SetSubRect(IntRect(positionPartieDecor.x, positionPartieDecor.y, positionPartieDecor.x+positionPartieDecor.w, positionPartieDecor.y+positionPartieDecor.h));
                        m_decor[couche][i][j].m_spriteHerbe.SetScale((float)m_decor[couche][i][j].getTailleHerbe()/100,(float)m_decor[couche][i][j].getTailleHerbe()/100);
                        m_decor[couche][i][j].m_spriteHerbe.SetX(position.x+64-positionPartieDecor.w/2);
                        m_decor[couche][i][j].m_spriteHerbe.SetY(position.y-positionPartieDecor.h+64);
                        m_decor[couche][i][j].m_spriteHerbe.SetColor(m_decor[couche][i][j].getCouleurHerbe());
                    }
                }
            }

    for(int i=0;i<(int)m_monstre.size();i++)
        if(m_monstre[i].getCaracteristique().rang>=0)
        {
            m_monstre[i].m_light=moteurGraphique->LightManager->Add_Dynamic_Light();
            moteurGraphique->LightManager->SetQuality(m_monstre[i].m_light,6);

            sf::Vector2f pos;
            pos.x=(((m_monstre[i].getCoordonneePixel().x-m_monstre[i].getCoordonneePixel().y)*64/COTE_TILE+getDimensions().y*64));
            pos.y=(((m_monstre[i].getCoordonneePixel().x+m_monstre[i].getCoordonneePixel().y)*64/COTE_TILE)/2+32)*2;

            moteurGraphique->LightManager->SetPosition(m_monstre[i].m_light,pos);

            moteurGraphique->LightManager->SetColor(m_monstre[i].m_light,sf::Color(m_monstre[i].getPorteeLumineuse().rouge,m_monstre[i].getPorteeLumineuse().vert,m_monstre[i].getPorteeLumineuse().bleu));
        }

    sf::Vector2f pos;

    for(int i=0;i<NOMBRE_COUCHE_MAP;i++)
        for(int j=0;j<m_dimensions.y;j++)
            for(int k=0;k<m_dimensions.x;k++)
                if(m_decor[i][j][k].getTileset()>=0&&m_decor[i][j][k].getTileset()<(int)m_tileset.size())
                {
                    CreerSprite(sf::Vector3f(k,j,i));

                    pos.x=(((k*COTE_TILE-j*COTE_TILE)*64/COTE_TILE+getDimensions().y*64));
                    pos.y=(((k*COTE_TILE+j*COTE_TILE)*64/COTE_TILE)/2+32)*2;

                    if(m_tileset[m_decor[i][j][k].getTileset()].getLumiereDuTile(m_decor[i][j][k].getTile()).intensite<0)
                        {
                            if(m_tileset[m_decor[i][j][k].getTileset()].getOrientationDuTile(m_decor[i][j][k].getTile())=='x'||m_tileset[m_decor[i][j][k].getTileset()].getOrientationDuTile(m_decor[i][j][k].getTile())=='c')
                                moteurGraphique->LightManager->Add_Wall(sf::Vector2f(pos.x-32, pos.y-32), sf::Vector2f(pos.x+32 , pos.y+32),(int)m_tileset[m_decor[i][j][k].getTileset()].getLumiereDuTile(m_decor[i][j][k].getTile()).hauteur);

                            if(m_tileset[m_decor[i][j][k].getTileset()].getOrientationDuTile(m_decor[i][j][k].getTile())=='y'||m_tileset[m_decor[i][j][k].getTileset()].getOrientationDuTile(m_decor[i][j][k].getTile())=='c')
                                moteurGraphique->LightManager->Add_Wall(sf::Vector2f(pos.x-32, pos.y+32), sf::Vector2f(pos.x+32 , pos.y-32),(int)m_tileset[m_decor[i][j][k].getTileset()].getLumiereDuTile(m_decor[i][j][k].getTile()).hauteur);

                            if(m_tileset[m_decor[i][j][k].getTileset()].getOrientationDuTile(m_decor[i][j][k].getTile())=='b'||m_tileset[m_decor[i][j][k].getTileset()].getOrientationDuTile(m_decor[i][j][k].getTile())=='g')
                                moteurGraphique->LightManager->Add_Wall(sf::Vector2f(pos.x, pos.y), sf::Vector2f(pos.x+32 , pos.y-32),(int)m_tileset[m_decor[i][j][k].getTileset()].getLumiereDuTile(m_decor[i][j][k].getTile()).hauteur);

                            if(m_tileset[m_decor[i][j][k].getTileset()].getOrientationDuTile(m_decor[i][j][k].getTile())=='b')
                                moteurGraphique->LightManager->Add_Wall(sf::Vector2f(pos.x-32, pos.y-32), sf::Vector2f(pos.x-0.1 , pos.y-0.1),(int)m_tileset[m_decor[i][j][k].getTileset()].getLumiereDuTile(m_decor[i][j][k].getTile()).hauteur);

                            if(m_tileset[m_decor[i][j][k].getTileset()].getOrientationDuTile(m_decor[i][j][k].getTile())=='d')
                                moteurGraphique->LightManager->Add_Wall(sf::Vector2f(pos.x-32, pos.y-32), sf::Vector2f(pos.x-0.1 , pos.y+0.1),(int)m_tileset[m_decor[i][j][k].getTileset()].getLumiereDuTile(m_decor[i][j][k].getTile()).hauteur);

                            if(m_tileset[m_decor[i][j][k].getTileset()].getOrientationDuTile(m_decor[i][j][k].getTile())=='h'||m_tileset[m_decor[i][j][k].getTileset()].getOrientationDuTile(m_decor[i][j][k].getTile())=='g')
                                moteurGraphique->LightManager->Add_Wall(sf::Vector2f(pos.x+0.1, pos.y+0.1), sf::Vector2f(pos.x+32 , pos.y+32),(int)m_tileset[m_decor[i][j][k].getTileset()].getLumiereDuTile(m_decor[i][j][k].getTile()).hauteur);

                            if(m_tileset[m_decor[i][j][k].getTileset()].getOrientationDuTile(m_decor[i][j][k].getTile())=='h'||m_tileset[m_decor[i][j][k].getTileset()].getOrientationDuTile(m_decor[i][j][k].getTile())=='d')
                                moteurGraphique->LightManager->Add_Wall(sf::Vector2f(pos.x-32, pos.y+32), sf::Vector2f(pos.x , pos.y),(int)m_tileset[m_decor[i][j][k].getTileset()].getLumiereDuTile(m_decor[i][j][k].getTile()).hauteur);
                        }
                }

    for(int i=0;i<NOMBRE_COUCHE_MAP;i++)
        for(int j=0;j<m_dimensions.y;j++)
            for(int k=0;k<m_dimensions.x;k++)
                if(m_decor[i][j][k].getTileset()>=0&&m_decor[i][j][k].getTileset()<(int)m_tileset.size())
                {
                    pos.x=(((k*COTE_TILE-j*COTE_TILE)*64/COTE_TILE+getDimensions().y*64));
                    pos.y=(((k*COTE_TILE+j*COTE_TILE)*64/COTE_TILE)/2+32)*2;
                    if(m_tileset[m_decor[i][j][k].getTileset()].getLumiereDuTile(m_decor[i][j][k].getTile()).intensite>0)
                    {
                        if(m_tileset[m_decor[i][j][k].getTileset()].getAnimationTile(m_decor[i][j][k].getTile())!=-1)
                            m_decor[i][j][k].m_light=moteurGraphique->LightManager->Add_Dynamic_Light(pos,m_tileset[m_decor[i][j][k].getTileset()].getLumiereDuTile(m_decor[i][j][k].getTile()).intensite,m_tileset[m_decor[i][j][k].getTileset()].getLumiereDuTile(m_decor[i][j][k].getTile()).intensite*3,8,sf::Color(m_tileset[m_decor[i][j][k].getTileset()].getLumiereDuTile(m_decor[i][j][k].getTile()).rouge,m_tileset[m_decor[i][j][k].getTileset()].getLumiereDuTile(m_decor[i][j][k].getTile()).vert,m_tileset[m_decor[i][j][k].getTileset()].getLumiereDuTile(m_decor[i][j][k].getTile()).bleu));
                        else
                            m_decor[i][j][k].m_light=moteurGraphique->LightManager->Add_Static_Light(pos,m_tileset[m_decor[i][j][k].getTileset()].getLumiereDuTile(m_decor[i][j][k].getTile()).intensite,m_tileset[m_decor[i][j][k].getTileset()].getLumiereDuTile(m_decor[i][j][k].getTile()).intensite*3,8,sf::Color(m_tileset[m_decor[i][j][k].getTileset()].getLumiereDuTile(m_decor[i][j][k].getTile()).rouge,m_tileset[m_decor[i][j][k].getTileset()].getLumiereDuTile(m_decor[i][j][k].getTile()).vert,m_tileset[m_decor[i][j][k].getTileset()].getLumiereDuTile(m_decor[i][j][k].getTile()).bleu));
                    }
                }
    moteurGraphique->LightManager->Generate();
}

void Map::CreerSprite(sf::Vector3f position_case)
{
    coordonnee position,positionPartieDecor;

    position.x=((int)position_case.x-(int)position_case.y-1+m_dimensions.y)*64;
    position.y=((int)position_case.x+(int)position_case.y)*32;
    positionPartieDecor=m_tileset[m_decor[(int)position_case.z][(int)position_case.y][(int)position_case.x].getTileset()].getPositionDuTile(m_decor[(int)position_case.z][(int)position_case.y][(int)position_case.x].getTile());

    m_decor[(int)position_case.z][(int)position_case.y][(int)position_case.x].m_sprite.SetImage(*moteurGraphique->getImage(m_tileset[m_decor[(int)position_case.z][(int)position_case.y][(int)position_case.x].getTileset()].getImage(m_decor[(int)position_case.z][(int)position_case.y][(int)position_case.x].getTile())));
    m_decor[(int)position_case.z][(int)position_case.y][(int)position_case.x].m_sprite.SetSubRect(IntRect(positionPartieDecor.x, positionPartieDecor.y, positionPartieDecor.x+positionPartieDecor.w, positionPartieDecor.y+positionPartieDecor.h));

    m_decor[(int)position_case.z][(int)position_case.y][(int)position_case.x].m_sprite.SetCenter(m_tileset[m_decor[(int)position_case.z][(int)position_case.y][(int)position_case.x].getTileset()].getCentreDuTile(m_decor[(int)position_case.z][(int)position_case.y][(int)position_case.x].getTile()).x,m_tileset[m_decor[(int)position_case.z][(int)position_case.y][(int)position_case.x].getTileset()].getCentreDuTile(m_decor[(int)position_case.z][(int)position_case.y][(int)position_case.x].getTile()).y);

    m_decor[(int)position_case.z][(int)position_case.y][(int)position_case.x].m_spriteOmbre=m_decor[(int)position_case.z][(int)position_case.y][(int)position_case.x].m_sprite;

    if(configuration->Ombre)
    {
        if(m_tileset[m_decor[(int)position_case.z][(int)position_case.y][(int)position_case.x].getTileset()].getOmbreDuTile(m_decor[(int)position_case.z][(int)position_case.y][(int)position_case.x].getTile()))
        {
            m_decor[(int)position_case.z][(int)position_case.y][(int)position_case.x].m_spriteOmbre.SetScale(1, (100-(float)moteurGraphique->m_soleil.hauteur)/50);
            m_decor[(int)position_case.z][(int)position_case.y][(int)position_case.x].m_spriteOmbre.SetRotation(moteurGraphique->m_angleOmbreSoleil);
            m_decor[(int)position_case.z][(int)position_case.y][(int)position_case.x].m_spriteOmbre.SetX(position.x+64);
            m_decor[(int)position_case.z][(int)position_case.y][(int)position_case.x].m_spriteOmbre.SetY(position.y+32);
        }
        else
            m_decor[(int)position_case.z][(int)position_case.y][(int)position_case.x].m_spriteOmbre.SetSubRect(sf::IntRect(0,0,0,0));
    }
    else
        m_decor[(int)position_case.z][(int)position_case.y][(int)position_case.x].m_spriteOmbre.SetSubRect(sf::IntRect(0,0,0,0));

    m_decor[(int)position_case.z][(int)position_case.y][(int)position_case.x].m_sprite.SetX(position.x+64);
    m_decor[(int)position_case.z][(int)position_case.y][(int)position_case.x].m_sprite.SetY(position.y+32);

    int layer=6;

    if(position_case.z==0)
        layer=m_decor[(int)position_case.z][(int)position_case.y][(int)position_case.x].getCouche();
    if(position_case.z==1)
        layer=10;

    m_decor[(int)position_case.z][(int)position_case.y][(int)position_case.x].setCouche(layer);
}


void Map::Sauvegarder(Hero *hero)
{
	char numero[7];
	string chemin = configuration->chemin_temps;

	sprintf(numero,"map%i.map.hs",m_numero);
	chemin += numero;

	console->Ajouter("",0);
	console->Ajouter("Sauvegarde de la map : "+chemin,0);


	ofstream fichier(chemin.c_str(), ios::out | ios::trunc);

	bool ajouter=true;
	for(int i=0;i<(int)hero->m_contenuSave.size();i++)
        if(hero->m_contenuSave[i]==chemin)
            ajouter=false;
    if(ajouter)
        hero->m_contenuSave.push_back(chemin);

    if(fichier)
    {
        fichier<<"*"<<m_nom<<"\n$\n";
        for(int i=0;i<(int)m_fond.size();i++)
            fichier<<"*"<<m_fond[i]<<"\n";
        fichier<<"$\n";
        for(int i=0;i<(int)m_cheminMusique.size();i++)
            fichier<<"*"<<m_cheminMusique[i]<<"\n";
        fichier<<"$\n";

        for(int i=0;i<24;i++)
            fichier<<"*"<<m_lumiere[i].rouge<<" "<<m_lumiere[i].vert<<" "<<m_lumiere[i].bleu<<" "<<m_lumiere[i].intensite<<" "<<m_lumiere[i].hauteur<<"\n";

        fichier<<"$\n";

        for(int i=0;i<(int)m_tileset.size();i++)
            fichier<<"*"<<m_tileset[i].getChemin()<<"\n";

        fichier<<"$\n";

        for(int i=0;i<(int)m_herbe.size();i++)
            fichier<<"*"<<m_herbe[i].getChemin()<<"\n";

        fichier<<"$\n";

        for(int i=0;i<(int)m_ModeleMonstre.size();i++)
            fichier<<"*"<<m_ModeleMonstre[i].m_chemin<<"\n";

        fichier<<"$\n";

        for(int i=0;i<(int)m_evenement.size();i++)
        {
            fichier<<"* e"<<m_evenement[i].getType()<<" $ * ";
            for(int j=0;j<m_evenement[i].getNombreInformation();j++)
                fichier<<"i"<<m_evenement[i].getInformation(j)<<" ";
            fichier<<"$\n";
        }

        fichier<<"$\n";

        for(int couche=0;couche<2;couche++)
    	{
    	    for(int i=0;i<(int)m_dimensions.y;i++)
    	    {
    	        fichier<<"* ";
                for(int j=0;j<m_dimensions.x;j++)
                {
                    fichier<<" s"<<m_decor[couche][i][j].getTileset()<<" ";
                    fichier<<"t"<<m_decor[couche][i][j].getTile()<<" ";
                    for(int k=0;k<(int)m_decor[couche][i][j].getEvenement().size();k++)
                        fichier<<"e"<<m_decor[couche][i][j].getEvenement()[k]<<" ";
                    if(m_decor[couche][i][j].getMonstre()>=0&&m_decor[couche][i][j].getMonstre()<(int)m_monstre.size())
                        fichier<<"m"<<m_decor[couche][i][j].getMonstre()<<" ";
                            //fichier<<"m"<<m_monstre[m_decor[couche][i][j].getMonstre()].getModele()<<" ";
                    fichier<<"h"<<m_decor[couche][i][j].getHerbe()<<" ";
                    fichier<<"i"<<m_decor[couche][i][j].getHauteur()<<" ";

                    fichier<<"l"<<m_decor[couche][i][j].getCouche()<<" ";

                    for(int o=0;o<(int)m_decor[couche][i][j].getNombreObjets();o++)
                        m_decor[couche][i][j].getObjet(o)->SauvegarderTexte(&fichier);

                    fichier<<"|";
                }
                fichier<<"$\n";
    	    }
    	    fichier<<"$\n";
    	}

        fichier.close();
    }
    else
    {
        console->Ajouter("Impossible d'ouvrir le fichier : "+chemin,1);
        throw "";
    }




    chemin = configuration->chemin_temps;
    sprintf(numero,"entites_map%i.emap.hs",m_numero);
	chemin += numero;

	console->Ajouter("Sauvegarde de la map_entite : "+chemin,0);

    ofstream fichier2(chemin.c_str(), ios::out | ios::trunc);

    ajouter=true;
	for(int i=0;i<(int)hero->m_contenuSave.size();i++)
        if(hero->m_contenuSave[i]==chemin)
            ajouter=false;
    if(ajouter)
        hero->m_contenuSave.push_back(chemin);

    if(fichier2)
    {
        for(int i=0;i<(int)m_monstre.size();i++)
        {
            fichier2<<"* m"<<m_monstre[i].getModele()<<" vi"<<m_monstre[i].getCaracteristique().vie<<" va"<<m_monstre[i].getCaracteristique().maxVie<<" di"<<m_monstre[i].getCaracteristique().degatsMin<<" da"<<m_monstre[i].getCaracteristique().degatsMax<<" r"<<m_monstre[i].getCaracteristique().rang<<" a"<<m_monstre[i].getCaracteristique().pointAme<<" t"<<m_monstre[i].getCaracteristique().modificateurTaille
            <<" p"<<m_monstre[i].getPose()<<" e"<<m_monstre[i].getEtat()<<" g"<<m_monstre[i].getAngle()
            <<" lr"<<m_monstre[i].getPorteeLumineuse().rouge<<" lv"<<m_monstre[i].getPorteeLumineuse().vert<<" lb"<<m_monstre[i].getPorteeLumineuse().bleu<<" li"<<m_monstre[i].getPorteeLumineuse().intensite<<" $\n";
        }
        fichier2<<"\n$";

        fichier2.close();
    }


    /*chemin = configuration->chemin_temps;
    sprintf(numero,"item_map%i.imap.hs",m_numero);
	chemin += numero;

	console->Ajouter("Sauvegarde de la map_item : "+chemin,0);

    ofstream fichier2(chemin.c_str(), ios::out | ios::trunc);
    if(fichier2)
    {
        for(int i=0;i<(int)m_monstre.size();i++)
        {
            fichier2<<"* m"<<m_monstre[i].getModele()<<" vi"<<m_monstre[i].getCaracteristique().vie<<" va"<<m_monstre[i].getCaracteristique().maxVie<<" di"<<m_monstre[i].getCaracteristique().degatsMin<<" da"<<m_monstre[i].getCaracteristique().degatsMax<<" r"<<m_monstre[i].getCaracteristique().rang<<" a"<<m_monstre[i].getCaracteristique().pointAme<<" t"<<m_monstre[i].getCaracteristique().modificateurTaille
            <<" p"<<m_monstre[i].getPose()<<" e"<<m_monstre[i].getEtat()<<" g"<<m_monstre[i].getAngle()
            <<" lr"<<m_monstre[i].getPorteeLumineuse().rouge<<" lv"<<m_monstre[i].getPorteeLumineuse().vert<<" lb"<<m_monstre[i].getPorteeLumineuse().bleu<<" li"<<m_monstre[i].getPorteeLumineuse().intensite<<" $\n";
        }
        fichier2<<"\n$";

        fichier2.close();
    }*/


    console->Ajouter("Sauvegarde terminée !");
}





void Map::calculerOmbresEtLumieres()
{
    if(configuration->heure+1<24)
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

void Map::AfficherSacInventaire(RenderWindow* ecran,coordonnee positionSac,float decalage,coordonnee positionSouris,Classe *classe)
{
    Sprite Sprite;
	String texte;

	    m_objetPointe=-1;
	    coordonnee position;

	    if(positionSac.x>=0&&positionSac.x<m_dimensions.x
	     &&positionSac.y>=0&&positionSac.y<(int)m_dimensions.y)
	    if(m_decor[1][positionSac.y][positionSac.x].getNombreObjets()>0)
        {
            if(m_defilerObjets>m_decor[1][positionSac.y][positionSac.x].getNombreObjets()-classe->position_sac_inventaire.h+1)
                m_defilerObjets=m_decor[1][positionSac.y][positionSac.x].getNombreObjets()-classe->position_sac_inventaire.h+1;
            if(m_defilerObjets<0)
                m_defilerObjets=0;

            for(int z=m_defilerObjets;z<m_decor[1][positionSac.y][positionSac.x].getNombreObjets()&&z<classe->position_sac_inventaire.h+m_defilerObjets;z++)
            {
                if(positionSouris.x>classe->position_sac_inventaire.x*configuration->Resolution.w/800&&positionSouris.x<(classe->position_sac_inventaire.x+classe->position_sac_inventaire.w)*configuration->Resolution.w/800
                 &&positionSouris.y>classe->position_sac_inventaire.y*configuration->Resolution.h/600+(z-m_defilerObjets)*20*configuration->Resolution.w/800
                 &&positionSouris.y<classe->position_sac_inventaire.y*configuration->Resolution.h/600+(z-m_defilerObjets)*20*configuration->Resolution.w/800+20*configuration->Resolution.w/800)
                 {
                     Sprite.SetImage(*moteurGraphique->getImage(0));
                     Sprite.SetColor(sf::Color(255,255,255,128));
                     Sprite.Resize(320*configuration->Resolution.w/800,20*configuration->Resolution.h/600);
                     Sprite.SetX(classe->position_sac_inventaire.x*configuration->Resolution.w/800);
                     Sprite.SetY(classe->position_sac_inventaire.y*configuration->Resolution.h/600+(z-m_defilerObjets)*20*configuration->Resolution.w/800 - decalage*configuration->Resolution.h/600);
                     moteurGraphique->AjouterCommande(&Sprite,16,0);

                    if(!(z-m_defilerObjets==0&&m_defilerObjets>0)&&!((z-m_defilerObjets==classe->position_sac_inventaire.h-1&&z+m_defilerObjets<=m_decor[1][positionSac.y][positionSac.x].getNombreObjets()+1)))
                        m_objetPointe=z;
                 }


                int rarete=m_decor[1][positionSac.y][positionSac.x].getObjet(z)->getRarete();
                if(rarete==NORMAL)
                    texte.SetColor(sf::Color(224,224,224,255));
                if(rarete==BONNEFACTURE)
                    texte.SetColor(sf::Color(128,0,128,255));
                if(rarete==BENI)
                    texte.SetColor(sf::Color(0,64,128,255));
                if(rarete==SACRE)
                    texte.SetColor(sf::Color(255,255,128,255));
                if(rarete==SANCTIFIE)
                    texte.SetColor(sf::Color(128,255,255,255));
                if(rarete==DIVIN)
                    texte.SetColor(sf::Color(255,164,32,255));
                if(rarete==INFERNAL)
                    texte.SetColor(sf::Color(224,0,0,255));
                if(rarete==CRAFT)
                    texte.SetColor(sf::Color(128,64,0,255));

                if(z-m_defilerObjets==0&&m_defilerObjets>0)
                    texte.SetText("...");
                else if((z-m_defilerObjets==classe->position_sac_inventaire.h-1&&z+m_defilerObjets<=m_decor[1][positionSac.y][positionSac.x].getNombreObjets()+1))
                    texte.SetText("...");
                else
                    texte.SetText(m_decor[1][positionSac.y][positionSac.x].getObjet(z)->getNom());
                texte.SetSize(16*configuration->Resolution.w/800);

                position.x=(int)((classe->position_sac_inventaire.x*configuration->Resolution.w/800)+((classe->position_sac_inventaire.w/2)*configuration->Resolution.w/800)-(texte.GetRect().Right-texte.GetRect().Left)/2);
                position.y=(int)(classe->position_sac_inventaire.y*configuration->Resolution.h/600+(z-m_defilerObjets)*20*configuration->Resolution.w/800 - decalage*configuration->Resolution.h/600 );

                texte.SetY(position.y);
                texte.SetX(position.x);

                moteurGraphique->AjouterTexte(&texte,16);
            }
        }
}

void Map::Afficher(RenderWindow* ecran,View *camera,int type,Hero *hero,coordonnee positionSouris,bool alt,float alpha)
{
    coordonnee positionHero;

	Sprite sprite,spriteMinimap;
	String texte;

	spriteMinimap.SetColor(sf::Color(255,255,255,(int)alpha));
	spriteMinimap.SetCenter(4*configuration->Resolution.w/800,4*configuration->Resolution.w/800);
    spriteMinimap.SetRotation(45);
    spriteMinimap.SetSubRect(sf::IntRect(0,0,8,8));

	positionHero.y=(int)((hero->m_personnage.getCoordonneePixel().x+hero->m_personnage.getCoordonneePixel().y)*DIVISEUR_COTE_TILE*32);
    positionHero.x=(int)(((hero->m_personnage.getCoordonneePixel().x-hero->m_personnage.getCoordonneePixel().y)*DIVISEUR_COTE_TILE-1+m_dimensions.y)*64);


    if(type==1)
    {
        coordonnee position;

        for(int couche=0;couche<NOMBRE_COUCHE_MAP;couche++)
        {
            for(int j=hero->m_personnage.getCoordonnee().y-15;j<hero->m_personnage.getCoordonnee().y+15;++j)
            {
                for(int k=hero->m_personnage.getCoordonnee().x-15;k<hero->m_personnage.getCoordonnee().x+15;++k)
                {
                    position.x=(k-j-1+m_dimensions.y)*64+48;
                    position.y=(k+j)*32+16;

                    if(j>=0&&j<(int)m_dimensions.y&&k>=0&&k<m_dimensions.x)
                    {
                        if(couche==1)
                        {
                            if(configuration->Herbes)
                                if(m_decor[0][j][k].m_spriteHerbe.GetSize().x>0)
                                     if(m_decor[0][j][k].m_spriteHerbe.GetPosition().x+m_decor[0][j][k].m_spriteHerbe.GetSize().x>=camera->GetRect().Left
                                      &&m_decor[0][j][k].m_spriteHerbe.GetPosition().x<camera->GetRect().Right
                                      &&m_decor[0][j][k].m_spriteHerbe.GetPosition().y+m_decor[0][j][k].m_spriteHerbe.GetSize().y>=camera->GetRect().Top
                                      &&m_decor[0][j][k].m_spriteHerbe.GetPosition().y<camera->GetRect().Bottom)
                                        moteurGraphique->AjouterCommande(&m_decor[0][j][k].m_spriteHerbe,10,1);

                                if(hero->m_personnage.getCoordonnee().x==hero->m_personnage.getProchaineCase().x&&hero->m_personnage.getCoordonnee().y==hero->m_personnage.getProchaineCase().y)
                                {
                                    if(hero->m_personnage.getCoordonnee().x==k&&hero->m_personnage.getCoordonnee().y==j)
                                        hero->Afficher(ecran,camera,getDimensions());
                                }
                                else
                                {
                                    coordonnee decalage={0,0,0,0};
                                    if(hero->m_personnage.getCoordonnee().x>hero->m_personnage.getProchaineCase().x)
                                        decalage.x=1;
                                    if(hero->m_personnage.getCoordonnee().y>hero->m_personnage.getProchaineCase().y)
                                        decalage.y=1;

                                    if(hero->m_personnage.getProchaineCase().x+decalage.x==k&&hero->m_personnage.getProchaineCase().y+decalage.y==j)
                                            hero->Afficher(ecran,camera,getDimensions());
                                }

                                if(m_decor[0][j][k].getMonstre()>=0&&m_decor[0][j][k].getMonstre()<(int)m_monstre.size())
                                    m_monstre[m_decor[0][j][k].getMonstre()].Afficher(ecran,camera,getDimensions(),&m_ModeleMonstre[m_monstre[m_decor[0][j][k].getMonstre()].getModele()]);
                                if(m_decor[1][j][k].getMonstre()>=0&&m_decor[1][j][k].getMonstre()<(int)m_monstre.size())
                                    m_monstre[m_decor[1][j][k].getMonstre()].Afficher(ecran,camera,getDimensions(),&m_ModeleMonstre[m_monstre[m_decor[1][j][k].getMonstre()].getModele()]);

                                if(configuration->Herbes)
                                    if(m_decor[1][j][k].m_spriteHerbe.GetSize().x>0)
                                        if(m_decor[1][j][k].m_spriteHerbe.GetPosition().x+m_decor[1][j][k].m_spriteHerbe.GetSize().x>=camera->GetRect().Left
                                         &&m_decor[1][j][k].m_spriteHerbe.GetPosition().x<camera->GetRect().Right
                                         &&m_decor[1][j][k].m_spriteHerbe.GetPosition().y+m_decor[1][j][k].m_spriteHerbe.GetSize().y>=camera->GetRect().Top
                                         &&m_decor[1][j][k].m_spriteHerbe.GetPosition().y<camera->GetRect().Bottom)
                                            moteurGraphique->AjouterCommande(&m_decor[1][j][k].m_spriteHerbe,10,1);


                                if(m_decor[0][j][k].getProjectile()>=0&&m_decor[0][j][k].getProjectile()<(int)m_projectile.size())
                                    m_projectile[m_decor[0][j][k].getProjectile()].Afficher(ecran,camera,position,getDimensions());
                                if(m_decor[1][j][k].getProjectile()>=0&&m_decor[1][j][k].getProjectile()<(int)m_projectile.size())
                                    m_projectile[m_decor[1][j][k].getProjectile()].Afficher(ecran,camera,position,getDimensions());

                                if(m_decor[0][j][k].getEffetGraphique()>=0&&m_decor[0][j][k].getEffetGraphique()<(int)m_effets.size())
                                    m_effets[m_decor[0][j][k].getEffetGraphique()].Afficher(ecran,camera,position,getDimensions());
                                if(m_decor[1][j][k].getEffetGraphique()>=0&&m_decor[1][j][k].getEffetGraphique()<(int)m_effets.size())
                                    m_effets[m_decor[1][j][k].getEffetGraphique()].Afficher(ecran,camera,position,getDimensions());


                                if(m_decor[1][j][k].getNombreObjets()>0&&couche==1)
                                {
                                    sprite.SetImage(*moteurGraphique->getImage(IDImageSac));
                                    sprite.SetSubRect(IntRect(0,0,32,32));
                                    sprite.Resize(32 , 32);
                                    sprite.SetX(position.x);
                                    sprite.SetY(position.y);

                                    if(m_sacPointe.x==k&&m_sacPointe.y==j&&m_monstreIllumine<0)
                                        sprite.SetColor(sf::Color(255,128,128));
                                    else
                                        sprite.SetColor(sf::Color(192,192,192));

                                    moteurGraphique->AjouterCommande(&sprite,10,1);

                                    sprite.SetColor(sf::Color(255,255,255));

                                    if(m_sacPointe.x==k&&m_sacPointe.y==j&&m_monstreIllumine<0||alt)
                                    {
                                        for(int z=0;z<m_decor[1][j][k].getNombreObjets();z++)
                                        {
                                            int rarete=m_decor[1][j][k].getObjet(z)->getRarete();
                                            if(rarete==NORMAL)
                                                texte.SetColor(sf::Color(224,224,224));
                                            if(rarete==BONNEFACTURE)
                                                texte.SetColor(sf::Color(128,0,128));
                                            if(rarete==BENI)
                                                texte.SetColor(sf::Color(0,64,128));
                                            if(rarete==SACRE)
                                                texte.SetColor(sf::Color(255,255,128));
                                            if(rarete==SANCTIFIE)
                                                texte.SetColor(sf::Color(128,255,255));
                                            if(rarete==DIVIN)
                                                texte.SetColor(sf::Color(255,164,32));
                                            if(rarete==INFERNAL)
                                                texte.SetColor(sf::Color(224,0,0));
                                            if(rarete==CRAFT)
                                                texte.SetColor(sf::Color(128,64,0));

                                            texte.SetText(m_decor[1][j][k].getObjet(z)->getNom());
                                            texte.SetSize(16*configuration->Resolution.w/800);
                                            texte.SetY((position.y-camera->GetRect().Top)*configuration->zoom-20*configuration->Resolution.w/800*(z+1));
                                            texte.SetX((position.x-camera->GetRect().Left)*configuration->zoom);

                                            moteurGraphique->AjouterTexte(&texte,14);

                                            sprite.SetImage(*moteurGraphique->getImage(0));

                                            sprite.SetY((position.y-camera->GetRect().Top)*configuration->zoom-20*configuration->Resolution.w/800*(z+1));
                                            sprite.SetX((position.x-camera->GetRect().Left)*configuration->zoom-4);
                                            sprite.SetColor(sf::Color(0,0,0,128));
                                            sprite.Resize(texte.GetRect().Right-texte.GetRect().Left +8 , texte.GetRect().Bottom-texte.GetRect().Top +6);

                                            moteurGraphique->AjouterCommande(&sprite,13,0);
                                        }
                                    }
                                }
                        }


                        if(m_decor[couche][j][k].m_sprite.GetSize().x>0)
                            if(m_decor[couche][j][k].m_sprite.GetPosition().x+m_decor[couche][j][k].m_sprite.GetSize().x-m_decor[couche][j][k].m_sprite.GetCenter().x>=camera->GetRect().Left
                             &&m_decor[couche][j][k].m_sprite.GetPosition().x-m_decor[couche][j][k].m_sprite.GetCenter().x<camera->GetRect().Right
                             &&m_decor[couche][j][k].m_sprite.GetPosition().y+m_decor[couche][j][k].m_sprite.GetSize().y-m_decor[couche][j][k].m_sprite.GetCenter().y>=camera->GetRect().Top
                             &&m_decor[couche][j][k].m_sprite.GetPosition().y-m_decor[couche][j][k].m_sprite.GetCenter().y<camera->GetRect().Bottom)
                             {
                                if(m_tileset[m_decor[couche][j][k].getTileset()].getTransparentDuTile(m_decor[couche][j][k].getTile()))
                                {
                                    int alpha=(int)((positionHero.y)-position.y)+160;

                                    if(alpha<configuration->alpha)
                                        alpha=configuration->alpha;
                                    if(alpha>255)
                                        alpha=255;

                                    m_decor[couche][j][k].m_sprite.SetColor(sf::Color(255,255,255,alpha));
                                }
                                moteurGraphique->AjouterCommande(&m_decor[couche][j][k].m_sprite,m_decor[couche][j][k].getCouche(),1);
                             }

                        if(m_decor[couche][j][k].m_spriteOmbre.GetSize().x>0)
                                moteurGraphique->AjouterCommande(&m_decor[couche][j][k].m_spriteOmbre,9,1);

                    }
                    else if(couche==0)
                    {

                        int w=j,z=k;

                        if(j>=(int)m_dimensions.y)
                            w=m_dimensions.y-1;
                        if(k>=m_dimensions.x)
                            z=m_dimensions.x-1;
                        if(j<0)
                            w=0;
                        if(k<0)
                            z=0;

                        sf::Sprite buffer=m_decor[couche][w][z].m_sprite;
                        buffer.SetX(position.x+64-48);
                        buffer.SetY(position.y+32-16);

                        if(buffer.GetSize().x>0)
                            if(buffer.GetPosition().x+buffer.GetSize().x-buffer.GetCenter().x>=camera->GetRect().Left
                             &&buffer.GetPosition().x-buffer.GetCenter().x<camera->GetRect().Right
                             &&buffer.GetPosition().y+buffer.GetSize().y-buffer.GetCenter().y>=camera->GetRect().Top
                             &&buffer.GetPosition().y-buffer.GetCenter().y<camera->GetRect().Bottom)
                             {
                                if(m_decor[couche][w][z].getTileset()>=0 && m_decor[couche][w][z].getTileset()<(int)m_tileset.size())
                                    if(m_tileset[m_decor[couche][w][z].getTileset()].getTransparentDuTile(m_decor[couche][w][z].getTile()))
                                    {
                                        int alpha=(int)((positionHero.y)-position.y)+160;

                                        if(alpha<configuration->alpha)
                                                alpha=configuration->alpha;
                                        if(alpha>255)
                                            alpha=255;

                                        buffer.SetColor(sf::Color(255,255,255,alpha));
                                    }

                                moteurGraphique->AjouterCommande(&buffer,m_decor[couche][w][z].getCouche(),1);
                             }
                    }


                    if(configuration->Minimap&&couche==1)
                    {
                        position.x=(((k-(hero->m_personnage.getCoordonnee().x-15))-(j-(hero->m_personnage.getCoordonnee().y-15))-1+40)*6*configuration->Resolution.w/800);
                        position.y=(((k-(hero->m_personnage.getCoordonnee().x-15))+(j-(hero->m_personnage.getCoordonnee().y-15)))*6*configuration->Resolution.w/800);

                        if(position.x+465*configuration->Resolution.w/800>605*configuration->Resolution.w/800&&position.x+465*configuration->Resolution.w/800<800*configuration->Resolution.w/800&&position.y*configuration->Resolution.y/800>0&&position.y-80*configuration->Resolution.w/800<195*configuration->Resolution.h/600)
                        {
                            int typeCase=getTypeCase(k,j);
                            if(typeCase==1)
                            {
                                spriteMinimap.SetImage(carreBrun);
                                spriteMinimap.SetX((float)(position.x+465*configuration->Resolution.w/800));
                                spriteMinimap.SetY((float)(position.y-80*configuration->Resolution.w/800));
                                moteurGraphique->AjouterCommande(&spriteMinimap,14,0);
                            }

                            if(typeCase==2)
                            {
                                spriteMinimap.SetImage(carreRouge);
                                spriteMinimap.SetX((float)(position.x+465*configuration->Resolution.w/800));
                                spriteMinimap.SetY((float)(position.y-80*configuration->Resolution.w/800));
                                moteurGraphique->AjouterCommande(&spriteMinimap,14,0);
                            }

                            if(typeCase==3)
                            {
                                spriteMinimap.SetImage(carreVert);
                                spriteMinimap.SetX((float)(position.x+465*configuration->Resolution.w/800));
                                spriteMinimap.SetY((float)(position.y-80*configuration->Resolution.w/800));
                                moteurGraphique->AjouterCommande(&spriteMinimap,14,0);
                            }

                            if(typeCase==4)
                            {
                                spriteMinimap.SetImage(carreJaune);
                                spriteMinimap.SetX((float)(position.x+465*configuration->Resolution.w/800));
                                spriteMinimap.SetY((float)(position.y-80*configuration->Resolution.w/800));
                                moteurGraphique->AjouterCommande(&spriteMinimap,14,0);
                            }

                            if(typeCase==5)
                            {
                                spriteMinimap.SetImage(carreMauve);
                                spriteMinimap.SetX((float)(position.x+465*configuration->Resolution.w/800));
                                spriteMinimap.SetY((float)(position.y-80*configuration->Resolution.w/800));
                                moteurGraphique->AjouterCommande(&spriteMinimap,14,0);
                            }

                            if(hero->m_personnage.getCoordonnee().x==k&&hero->m_personnage.getCoordonnee().y==j)
                            {
                                spriteMinimap.SetImage(carreBleu);
                                spriteMinimap.SetX((float)(position.x+465*configuration->Resolution.w/800));
                                spriteMinimap.SetY((float)(position.y-80*configuration->Resolution.w/800));
                                moteurGraphique->AjouterCommande(&spriteMinimap,14,0);
                            }
                        }
                    }
                }
            }
        }
    }

	if(type==3)
	{
	    m_objetPointe=-1;
	    coordonnee position;

	    if(hero->getChercherSac().x>=0&&hero->getChercherSac().x<m_dimensions.x
	     &&hero->getChercherSac().y>=0&&hero->getChercherSac().y<(int)m_dimensions.y)
	    if(m_decor[1][hero->getChercherSac().y][hero->getChercherSac().x].getNombreObjets()>0)
        {
            if(m_defilerObjets>m_decor[1][hero->getChercherSac().y][hero->getChercherSac().x].getNombreObjets()-11)
                m_defilerObjets=m_decor[1][hero->getChercherSac().y][hero->getChercherSac().x].getNombreObjets()-11;
            if(m_defilerObjets<0)
                m_defilerObjets=0;

            for(int z=m_defilerObjets;z<m_decor[1][hero->getChercherSac().y][hero->getChercherSac().x].getNombreObjets()&&z<12+m_defilerObjets;z++)
            {

                if(positionSouris.x>configuration->Resolution.w-configuration->Resolution.w*0.25
                 &&positionSouris.y>configuration->Resolution.w*0.265+(z-m_defilerObjets)*20*configuration->Resolution.w/800
                 &&positionSouris.y<configuration->Resolution.w*0.265+(z-m_defilerObjets)*20*configuration->Resolution.w/800+20*configuration->Resolution.w/800)
                 {
                     sprite.SetImage(*moteurGraphique->getImage(0));
                     sprite.SetColor(sf::Color(255,255,255,128));
                     sprite.Resize(configuration->Resolution.w*0.24,20*configuration->Resolution.w/800);
                     sprite.SetX(configuration->Resolution.w-configuration->Resolution.w*0.24*alpha/255);
                     sprite.SetY(configuration->Resolution.w*0.265+(z-m_defilerObjets)*20*configuration->Resolution.w/800);
                     moteurGraphique->AjouterCommande(&sprite,14,0);

                    if(!(z-m_defilerObjets==0&&m_defilerObjets>0)&&!((z-m_defilerObjets==11&&z+m_defilerObjets<=m_decor[1][hero->getChercherSac().y][hero->getChercherSac().x].getNombreObjets()+1)))
                        m_objetPointe=z;
                 }


                int rarete=m_decor[1][hero->getChercherSac().y][hero->getChercherSac().x].getObjet(z)->getRarete();
                if(rarete==NORMAL)
                    texte.SetColor(sf::Color(224,224,224,255));
                if(rarete==BONNEFACTURE)
                    texte.SetColor(sf::Color(128,0,128,255));
                if(rarete==BENI)
                    texte.SetColor(sf::Color(0,64,128,255));
                if(rarete==SACRE)
                    texte.SetColor(sf::Color(255,255,128,255));
                if(rarete==SANCTIFIE)
                    texte.SetColor(sf::Color(128,255,255,255));
                if(rarete==DIVIN)
                    texte.SetColor(sf::Color(255,164,32,255));
                if(rarete==INFERNAL)
                    texte.SetColor(sf::Color(224,0,0,255));
                if(rarete==CRAFT)
                    texte.SetColor(sf::Color(128,64,0,255));

                if(z-m_defilerObjets==0&&m_defilerObjets>0)
                    texte.SetText("...");
                else if((z-m_defilerObjets==11&&z+m_defilerObjets<=m_decor[1][hero->getChercherSac().y][hero->getChercherSac().x].getNombreObjets()+1))
                    texte.SetText("...");
                else
                    texte.SetText(m_decor[1][hero->getChercherSac().y][hero->getChercherSac().x].getObjet(z)->getNom());
                texte.SetSize(16*configuration->Resolution.w/800);

                position.x=(int)((configuration->Resolution.w-configuration->Resolution.w*0.25*alpha/255)+(configuration->Resolution.w*0.25)/2-(texte.GetRect().Right-texte.GetRect().Left)/2);
                position.y=(int)(configuration->Resolution.w*0.265+(z-m_defilerObjets)*20*configuration->Resolution.w/800);

                texte.SetY(position.y);
                texte.SetX(position.x);

                moteurGraphique->AjouterTexte(&texte,14);
            }
        }
	}
}

void Map::AfficherNomEvenement(sf::RenderWindow* ecran,coordonnee casePointee,coordonnee positionSouris)
{
    int evenement=-1;
    for(int i=0;i<2;i++)
        for(int z=0;z<(int)m_decor[i][casePointee.y][casePointee.x].getEvenement().size();z++)
            if(m_decor[i][casePointee.y][casePointee.x].getEvenement()[z]>-1)
            {
                evenement=m_decor[i][casePointee.y][casePointee.x].getEvenement()[z];

                if(evenement>=0)
                {
                    if(m_evenement[evenement].getType()==CHANGEMENT_DE_MAP)
                    {
                        string nom;
                        char chemin[128];

                        cDAT reader;
                        reader.Read(configuration->chemin_maps);
                        sprintf(chemin,"map%i.map.hs",m_evenement[evenement].getInformation(0));

                        ifstream *fichier=reader.GetInfos(chemin);
                        if(fichier)
                        {
                            char caractere;
                            do
                            {
                                //Chargement du nom
                                fichier->get(caractere);
                                if(caractere=='*')
                                {
                                    *fichier>>nom;
                                    for(int i=0;i<(int)nom.size();i++)
                                        if(nom[i]=='_')
                                            nom[i]=' ';
                                }
                            }while(caractere!='$');
                        }
                        fichier->close();

                        delete fichier;

                        sprintf(chemin,"Vers %s",nom.c_str());

                        sf::String texte;
                        texte.SetText(chemin);
                        texte.SetSize(16.f);
                        if(configuration->Resolution.y>0)
                            texte.SetY((positionSouris.y-16)*configuration->Resolution.h/configuration->Resolution.y);
                        if(configuration->Resolution.x>0)
                            texte.SetX(positionSouris.x*configuration->Resolution.w/configuration->Resolution.x);
                        moteurGraphique->AjouterTexte(&texte,15);
                    }
                }
            }
}

bool Map::testEvenement(sf::View *camera, Jeu *jeu,float temps)
{
    for(int i=0;i<2;i++)
    for(int z=0;z<(int)m_decor[i][jeu->hero.m_personnage.getCoordonnee().y][jeu->hero.m_personnage.getCoordonnee().x].getEvenement().size();z++)
    if(m_decor[i][jeu->hero.m_personnage.getCoordonnee().y][jeu->hero.m_personnage.getCoordonnee().x].getEvenement()[z]>=0)
    {
        if(m_evenement[m_decor[i][jeu->hero.m_personnage.getCoordonnee().y][jeu->hero.m_personnage.getCoordonnee().x].getEvenement()[z]].getType()==CHANGEMENT_DE_MAP)
        {
            int numeroMap=m_evenement[m_decor[i][jeu->hero.m_personnage.getCoordonnee().y][jeu->hero.m_personnage.getCoordonnee().x].getEvenement()[z]].getInformation(0);

            console->Ajouter("",0);
            console->Ajouter("---------------------------------------------------------------------------------",0);
            console->Ajouter("EVENEMENT : Changement de map",0);
            console->Ajouter("---------------------------------------------------------------------------------",0);

            coordonnee coordonneePerso;
            coordonneePerso.x=m_evenement[m_decor[i][jeu->hero.m_personnage.getCoordonnee().y][jeu->hero.m_personnage.getCoordonnee().x].getEvenement()[z]].getInformation(1);
            coordonneePerso.y=m_evenement[m_decor[i][jeu->hero.m_personnage.getCoordonnee().y][jeu->hero.m_personnage.getCoordonnee().x].getEvenement()[z]].getInformation(2);

            sf::Clock Clock;
             Clock.Reset();

            jeu->m_chargement->setC_Chargement(numeroMap,coordonneePerso);
            jeu->m_contexte = jeu->m_chargement;
        }

        if(m_evenement[m_decor[i][jeu->hero.m_personnage.getCoordonnee().y][jeu->hero.m_personnage.getCoordonnee().x].getEvenement()[z]].getType()==INFLIGER_DEGATS)
        {
            jeu->hero.infligerDegats(m_evenement[m_decor[i][jeu->hero.m_personnage.getCoordonnee().y][jeu->hero.m_personnage.getCoordonnee().x].getEvenement()[z]].getInformation(0)*temps*10);
        }
    }
    return 1;
}

int Map::gererMiracle(EntiteMiracle *entiteMiracle,Miracle *modeleMiracle,Hero *hero,bool monstre,coordonnee lanceur, coordonnee cible,int couche=0)
{
    couche=0;
    int retour=0;
    bool continuer=true;

    for(int o=0;o<(int)entiteMiracle->m_infos.size()&&continuer;o++)
    {
        if(entiteMiracle->m_infos[o].m_effetEnCours>=0&&entiteMiracle->m_infos[o].m_effetEnCours<(int)modeleMiracle->m_effets.size())
        {
            if(continuer)
            if(modeleMiracle->m_effets[entiteMiracle->m_infos[o].m_effetEnCours].m_type==PROJECTILE&&entiteMiracle->m_infos[o].m_IDObjet==-1)
            {
                m_projectile.push_back(Projectile ());

                m_projectile.back().m_position.x=entiteMiracle->m_infos[o].m_position.x;
                m_projectile.back().m_position.y=entiteMiracle->m_infos[o].m_position.y;

                m_projectile.back().m_degats=0;

                for(int p=0;p<(int)modeleMiracle->m_effets[entiteMiracle->m_infos[o].m_effetEnCours].m_lien.size();p++)
                    if(modeleMiracle->m_effets[entiteMiracle->m_infos[o].m_effetEnCours].m_lien[p]>=0&&modeleMiracle->m_effets[entiteMiracle->m_infos[o].m_effetEnCours].m_lien[p]<(int)modeleMiracle->m_effets.size())
                        if(modeleMiracle->m_effets[modeleMiracle->m_effets[entiteMiracle->m_infos[o].m_effetEnCours].m_lien[p]].m_type==DEGATS)
                            m_projectile.back().m_degats+=modeleMiracle->m_effets[modeleMiracle->m_effets[entiteMiracle->m_infos[o].m_effetEnCours].m_lien[p]].m_informations[0];

                m_projectile.back().m_monstre=monstre;

                m_projectile.back().m_actif=true;

                m_projectile.back().m_couche=couche;

                coordonneeDecimal position,position2;

                double m=atan2(cible.y-lanceur.y,cible.x-lanceur.x);

                m+=(float)modeleMiracle->m_effets[entiteMiracle->m_infos[o].m_effetEnCours].m_informations[1]*M_PI/180;

                position.x=cos(m)*modeleMiracle->m_effets[entiteMiracle->m_infos[o].m_effetEnCours].m_informations[0]/10;
                position.y=sin(m)*modeleMiracle->m_effets[entiteMiracle->m_infos[o].m_effetEnCours].m_informations[0]/10;

                m_projectile.back().m_vecteur=position;

                m_projectile.back().m_rotationReelle=m;

                position.x=(lanceur.x-lanceur.y)+m_dimensions.y;
                position.y=(lanceur.x+lanceur.y)/2;

                position2.x=(cible.x-cible.y)+m_dimensions.y;
                position2.y=(cible.x+cible.y)/2;

                m=atan2(position2.y-position.y,position2.x-position.x);

                m+=(float)modeleMiracle->m_effets[entiteMiracle->m_infos[o].m_effetEnCours].m_informations[1]*M_PI/180;

                m_projectile.back().m_rotation=m;

                m_projectile.back().m_positionCase=lanceur;


                m_projectile.back().m_positionImage= modeleMiracle->m_tile[modeleMiracle->m_effets[entiteMiracle->m_infos[o].m_effetEnCours].m_sequence][entiteMiracle->m_infos[o].m_imageEnCours].getCoordonnee();
                m_projectile.back().m_centre= modeleMiracle->m_tile[modeleMiracle->m_effets[entiteMiracle->m_infos[o].m_effetEnCours].m_sequence][entiteMiracle->m_infos[o].m_imageEnCours].getCentre();
                m_projectile.back().m_lumiere=modeleMiracle->m_tile[modeleMiracle->m_effets[entiteMiracle->m_infos[o].m_effetEnCours].m_sequence][entiteMiracle->m_infos[o].m_imageEnCours].getLumiere();

                m_projectile.back().m_light=moteurGraphique->LightManager->Add_Dynamic_Light(sf::Vector2f(position.x*64,position.y*32),m_projectile.back().m_lumiere.intensite,m_projectile.back().m_lumiere.intensite,4,sf::Color(m_projectile.back().m_lumiere.rouge,m_projectile.back().m_lumiere.vert,m_projectile.back().m_lumiere.bleu));

                if(modeleMiracle->m_tile[modeleMiracle->m_effets[entiteMiracle->m_infos[o].m_effetEnCours].m_sequence][entiteMiracle->m_infos[o].m_imageEnCours].getImage()>=0&&modeleMiracle->m_tile[modeleMiracle->m_effets[entiteMiracle->m_infos[o].m_effetEnCours].m_sequence][entiteMiracle->m_infos[o].m_imageEnCours].getImage()<(int)modeleMiracle->m_image.size())
                    m_projectile.back().m_image=modeleMiracle->m_image[modeleMiracle->m_tile[modeleMiracle->m_effets[entiteMiracle->m_infos[o].m_effetEnCours].m_sequence][entiteMiracle->m_infos[o].m_imageEnCours].getImage()];
                m_projectile.back().m_positionImage= modeleMiracle->m_tile[modeleMiracle->m_effets[entiteMiracle->m_infos[o].m_effetEnCours].m_sequence][entiteMiracle->m_infos[o].m_imageEnCours].getCoordonnee();

                entiteMiracle->m_infos[o].m_IDObjet=m_projectile.size()-1;

                entiteMiracle->m_infos[o].m_imageEnCours=0;

                if(couche>=0&&couche<2)
                    if(lanceur.y>=0&&lanceur.y<m_dimensions.y)
                        if(lanceur.x>=0&&lanceur.x<m_dimensions.x)
                        {
                            m_projectile.back().m_positionCase.x=lanceur.x;
                            m_projectile.back().m_positionCase.y=lanceur.y;
                            if(m_decor[1][lanceur.y][lanceur.x].getProjectile()!=-1)
                                m_decor[0][lanceur.y][lanceur.x].setProjectile(m_projectile.size()-1);
                            else
                                m_decor[1][lanceur.y][lanceur.x].setProjectile(m_projectile.size()-1);
                        }
            }

            if(continuer)
            if(modeleMiracle->m_effets[entiteMiracle->m_infos[o].m_effetEnCours].m_type==EFFET_GRAPHIQUE&&entiteMiracle->m_infos[o].m_IDObjet==-1)
            {

                m_effets.push_back(EffetGraphique ());

                m_effets.back().m_position.x=entiteMiracle->m_infos[o].m_position.x;
                 m_effets.back().m_position.y=entiteMiracle->m_infos[o].m_position.y;

                sf::Vector2f position;

                 m_effets.back().m_compteur=modeleMiracle->m_effets[entiteMiracle->m_infos[o].m_effetEnCours].m_informations[0];

                 m_effets.back().m_actif=true;

                 m_effets.back().m_couche=couche;

                 m_effets.back().m_lumiere.intensite=128;
                 m_effets.back().m_lumiere.rouge=255;
                 m_effets.back().m_lumiere.vert=128;
                 m_effets.back().m_lumiere.bleu=128;


                position.x=((( m_effets.back().m_position.x- m_effets.back().m_position.y)*64/COTE_TILE+m_dimensions.y*64));
                position.y=((( m_effets.back().m_position.x+ m_effets.back().m_position.y)*64/COTE_TILE)/2+32)*2;

                 m_effets.back().m_light=moteurGraphique->LightManager->Add_Dynamic_Light(position, m_effets.back().m_lumiere.intensite, m_effets.back().m_lumiere.intensite,4,sf::Color( m_effets.back().m_lumiere.rouge, m_effets.back().m_lumiere.vert, m_effets.back().m_lumiere.bleu));

                entiteMiracle->m_infos[o].m_imageEnCours=0;

                if(modeleMiracle->m_tile[modeleMiracle->m_effets[entiteMiracle->m_infos[o].m_effetEnCours].m_sequence][entiteMiracle->m_infos[o].m_imageEnCours].getImage()>=0&&modeleMiracle->m_tile[modeleMiracle->m_effets[entiteMiracle->m_infos[o].m_effetEnCours].m_sequence][entiteMiracle->m_infos[o].m_imageEnCours].getImage()<(int)modeleMiracle->m_image.size())
                     m_effets.back().m_image=modeleMiracle->m_image[modeleMiracle->m_tile[modeleMiracle->m_effets[entiteMiracle->m_infos[o].m_effetEnCours].m_sequence][entiteMiracle->m_infos[o].m_imageEnCours].getImage()];
                 m_effets.back().m_positionImage= modeleMiracle->m_tile[modeleMiracle->m_effets[entiteMiracle->m_infos[o].m_effetEnCours].m_sequence][entiteMiracle->m_infos[o].m_imageEnCours].getCoordonnee();

                entiteMiracle->m_infos[o].m_IDObjet=m_effets.size()-1;

                if(couche>=0&&couche<2)
                    if(lanceur.y>=0&&lanceur.y<m_dimensions.y)
                        if(lanceur.x>=0&&lanceur.x<m_dimensions.x)
                            m_decor[couche][lanceur.y][lanceur.x].setEffetGraphique(m_effets.size()-1);
            }

            if(continuer)
            if(modeleMiracle->m_effets[entiteMiracle->m_infos[o].m_effetEnCours].m_type==INVOCATION)
            {
                if(entiteMiracle->m_infos[o].m_IDObjet==-1)
                {
                    bool invoquer=true;
                    int x=0,y=0;
                    coordonnee positionCase;
                    positionCase.x=cible.x+rand()%2-4;
                    positionCase.y=cible.y+rand()%2-4;

                    if(positionCase.x<0)
                        positionCase.x=0;
                    if(positionCase.y<0)
                        positionCase.y=0;

                    while((getCollision(positionCase.x,positionCase.y) || ( positionCase.x==hero->m_personnage.getCoordonnee().x && positionCase.y==hero->m_personnage.getCoordonnee().y )) && invoquer )
                    {
                        positionCase.x++;
                        positionCase.y++;
                        x++; y++;
                        if(x>10)
                            invoquer=false;
                    }

                    if(invoquer)
                    {
                        entiteMiracle->m_infos[o].m_position.x=positionCase.x*COTE_TILE;
                        entiteMiracle->m_infos[o].m_position.y=positionCase.y*COTE_TILE;

                        int numero=-1;
                        for(int j=0;j<(int)m_ModeleMonstre.size();j++)
                            if(modeleMiracle->m_effets[entiteMiracle->m_infos[o].m_effetEnCours].m_chaine==m_ModeleMonstre[j].m_chemin)
                            {
                                numero=j;
                                break;
                            }

                        if(numero==-1)
                        {
                            m_ModeleMonstre.push_back(Modele_Monstre ());
                            m_ModeleMonstre.back().Charger(modeleMiracle->m_effets[entiteMiracle->m_infos[o].m_effetEnCours].m_chaine);
                            console->Ajouter("Chargement de : "+modeleMiracle->m_effets[entiteMiracle->m_infos[o].m_effetEnCours].m_chaine+" terminé",0);
                            numero=m_ModeleMonstre.size()-1;
                        }

                        m_monstre.push_back(Monstre ());
                        m_monstre.back().Charger(numero,&m_ModeleMonstre[numero]);
                        m_monstre.back().setCoordonnee(positionCase),m_monstre.back().setDepart();

                        m_monstre.back().m_light=moteurGraphique->LightManager->Add_Dynamic_Light();

                        moteurGraphique->LightManager->SetQuality(m_monstre.back().m_light,6);

                        sf::Vector2f pos;
                        pos.x=(((m_monstre.back().getCoordonneePixel().x-m_monstre.back().getCoordonneePixel().y)*64/COTE_TILE+getDimensions().y*64));
                        pos.y=(((m_monstre.back().getCoordonneePixel().x+m_monstre.back().getCoordonneePixel().y)*64/COTE_TILE)/2+32)*2;

                        moteurGraphique->LightManager->SetPosition(m_monstre.back().m_light,pos);

                        moteurGraphique->LightManager->SetColor(m_monstre.back().m_light,sf::Color(m_monstre.back().getPorteeLumineuse().rouge,m_monstre.back().getPorteeLumineuse().vert,m_monstre.back().getPorteeLumineuse().bleu));

                        m_decor[1][positionCase.y][positionCase.x].setMonstre(m_monstre.size()-1);

                        entiteMiracle->m_infos[o].m_IDObjet=m_monstre.size()-1;
                    }

                    for(int p=0;p<(int)modeleMiracle->m_effets[entiteMiracle->m_infos[o].m_effetEnCours].m_lien.size();p++)
                    {
                        entiteMiracle->m_infos.push_back(infosEntiteMiracle ());
                        entiteMiracle->m_infos.back().m_effetEnCours=modeleMiracle->m_effets[entiteMiracle->m_infos[o].m_effetEnCours].m_lien[p];
                        entiteMiracle->m_infos.back().m_position=entiteMiracle->m_infos[o].m_position;
                    }

                    continuer=false;
                    gererMiracle(entiteMiracle,modeleMiracle,hero,monstre,lanceur,cible,couche);
                }
                else if(entiteMiracle->m_infos[o].m_IDObjet>=0&&entiteMiracle->m_infos[o].m_IDObjet<(int)m_monstre.size())
                {
                    if(!m_monstre[entiteMiracle->m_infos[o].m_IDObjet].enVie())
                    {
                        entiteMiracle->m_infos.erase(entiteMiracle->m_infos.begin()+o);
                        continuer=false;
                        gererMiracle(entiteMiracle,modeleMiracle,hero,monstre,lanceur,cible,couche);
                    }
                }
                else
                {
                    entiteMiracle->m_infos.erase(entiteMiracle->m_infos.begin()+o);
                    continuer=false;
                    gererMiracle(entiteMiracle,modeleMiracle,hero,monstre,lanceur,cible,couche);
                }
            }


            if(continuer)
            if(modeleMiracle->m_effets[entiteMiracle->m_infos[o].m_effetEnCours].m_type==DEGATS)
            {
                retour+=modeleMiracle->m_effets[entiteMiracle->m_infos[o].m_effetEnCours].m_informations[0];
                for(int p=0;p<(int)modeleMiracle->m_effets[entiteMiracle->m_infos[o].m_effetEnCours].m_lien.size();p++)
                {
                    entiteMiracle->m_infos.push_back(infosEntiteMiracle ());
                    entiteMiracle->m_infos.back().m_effetEnCours=modeleMiracle->m_effets[entiteMiracle->m_infos[o].m_effetEnCours].m_lien[p];
                    entiteMiracle->m_infos.back().m_position=entiteMiracle->m_infos[o].m_position;
                }

                entiteMiracle->m_infos.erase(entiteMiracle->m_infos.begin()+o);
                continuer=false;
                gererMiracle(entiteMiracle,modeleMiracle,hero,monstre,lanceur,cible,couche);

            }

            if(modeleMiracle->m_effets[entiteMiracle->m_infos[o].m_effetEnCours].m_type==-1)
            {
                for(int p=0;p<(int)modeleMiracle->m_effets[entiteMiracle->m_infos[o].m_effetEnCours].m_lien.size();p++)
                {
                    entiteMiracle->m_infos.push_back(infosEntiteMiracle ());
                    entiteMiracle->m_infos.back().m_effetEnCours=modeleMiracle->m_effets[entiteMiracle->m_infos[o].m_effetEnCours].m_lien[p];
                    entiteMiracle->m_infos.back().m_position=entiteMiracle->m_infos[o].m_position;
                }

                entiteMiracle->m_infos.erase(entiteMiracle->m_infos.begin()+o);
                continuer=false;
                gererMiracle(entiteMiracle,modeleMiracle,hero,monstre,lanceur,cible,couche);
            }
        }
        else
        {
            for(int p=0;p<(int)modeleMiracle->m_effets[entiteMiracle->m_infos[o].m_effetEnCours].m_lien.size();p++)
            {
                entiteMiracle->m_infos.push_back(infosEntiteMiracle ());
                entiteMiracle->m_infos.back().m_effetEnCours=modeleMiracle->m_effets[entiteMiracle->m_infos[o].m_effetEnCours].m_lien[p];
                entiteMiracle->m_infos.back().m_position=entiteMiracle->m_infos[o].m_position;
            }

            entiteMiracle->m_infos.erase(entiteMiracle->m_infos.begin()+o);
            continuer=false;
            gererMiracle(entiteMiracle,modeleMiracle,hero,monstre,lanceur,cible,couche);
        }
    }

    return retour;
}

void Map::animer(Hero *hero,float temps,Menu *menu,sf::View *camera)
{
    coordonnee positionHero;
    positionHero.x=(hero->m_personnage.getCoordonnee().x-hero->m_personnage.getCoordonnee().y-1+m_dimensions.y)/5;
    positionHero.y=(hero->m_personnage.getCoordonnee().x+hero->m_personnage.getCoordonnee().y)/5;

    coordonnee vueMin,vueMax;

    vueMin.x=hero->m_personnage.getCoordonnee().x-10;
    vueMin.y=hero->m_personnage.getCoordonnee().y-10;
    vueMax.x=hero->m_personnage.getCoordonnee().x+10;
    vueMax.y=hero->m_personnage.getCoordonnee().y+10;

    if(vueMin.x<0) { vueMin.x=0; }
    if(vueMin.y<0) { vueMin.y=0; }
    if(vueMax.x>m_dimensions.x) { vueMax.x=m_dimensions.x; }
    if(vueMax.y>(int)m_dimensions.y) { vueMax.y=m_dimensions.y; }

	for(int i=0;i<2;i++)
        for(int j=vueMin.y;j<vueMax.y;j++)
            for(int k=vueMin.x;k<vueMax.x;k++)
            {
                for(int z=0;z<(int)m_decor[i][j][k].getEvenement().size();z++)
                    if(m_decor[i][j][k].getEvenement()[z]>=0&&m_decor[i][j][k].getEvenement()[z]<(int)m_evenement.size())
                        if(m_evenement[m_decor[i][j][k].getEvenement()[z]].getType()==TIMER)
                        {
                            m_evenement[m_decor[i][j][k].getEvenement()[z]].setInformation((int)((float)m_evenement[m_decor[i][j][k].getEvenement()[z]].getInformation(1)+(float)temps*1000),1);

                            if(m_evenement[m_decor[i][j][k].getEvenement()[z]].getInformation(1)>=m_evenement[m_decor[i][j][k].getEvenement()[z]].getInformation(0))
                            {
                                m_decor[i][j][k].setEvenement(m_evenement[m_decor[i][j][k].getEvenement()[z]].getInformation(2),z);
                                gererEvenements(m_decor[i][j][k].getEvenement()[z],z,i,k,j);
                            }
                        }
                    m_decor[i][j][k].augmenterAnimation(temps);

                    if(m_decor[i][j][k].getTileset()>=0&&m_decor[i][j][k].getTileset()<(int)m_tileset.size())
                    {
                        float tempsAnimation=m_tileset[m_decor[i][j][k].getTileset()].getTempsDuTile(m_decor[i][j][k].getTile());

                        if(m_tileset[m_decor[i][j][k].getTileset()].getAnimationTile(m_decor[i][j][k].getTile())>=0)
                            while(m_decor[i][j][k].getAnimation()>=tempsAnimation)
                            {
                                m_decor[i][j][k].setDecor(m_decor[i][j][k].getTileset(),m_tileset[m_decor[i][j][k].getTileset()].getAnimationTile(m_decor[i][j][k].getTile()),m_decor[i][j][k].getEvenement(),m_decor[i][j][k].getMonstre(),m_decor[i][j][k].getHerbe(),m_decor[i][j][k].getCouche(),m_decor[i][j][k].getHauteur());

                                CreerSprite(sf::Vector3f(k,j,i));

                                if(m_decor[i][j][k].getTileset()>=0&&m_decor[i][j][k].getTileset()<(int)m_tileset.size())
                                {
                                    moteurGraphique->LightManager->SetIntensity(m_decor[i][j][k].m_light,m_tileset[m_decor[i][j][k].getTileset()].getLumiereDuTile(m_decor[i][j][k].getTile()).intensite);
                                    moteurGraphique->LightManager->SetRadius(m_decor[i][j][k].m_light,m_tileset[m_decor[i][j][k].getTileset()].getLumiereDuTile(m_decor[i][j][k].getTile()).intensite*3);
                                    moteurGraphique->LightManager->SetColor(m_decor[i][j][k].m_light,sf::Color(m_tileset[m_decor[i][j][k].getTileset()].getLumiereDuTile(m_decor[i][j][k].getTile()).rouge,m_tileset[m_decor[i][j][k].getTileset()].getLumiereDuTile(m_decor[i][j][k].getTile()).vert,m_tileset[m_decor[i][j][k].getTileset()].getLumiereDuTile(m_decor[i][j][k].getTile()).bleu));

                                    moteurGraphique->LightManager->Generate(m_decor[i][j][k].m_light);

                                }

                                coordonnee position;
                                position.x=(k-j-1+m_dimensions.y)/5;
                                position.y=(k+j)/5;

                                m_tileset[m_decor[i][j][k].getTileset()].jouerSon(m_tileset[m_decor[i][j][k].getTileset()].getSonTile(m_decor[i][j][k].getTile()),position,positionHero);

                                m_decor[i][j][k].decrementerAnimation(tempsAnimation);
                            }
                    }

                    int monstre=m_decor[i][j][k].getMonstre();
                    if(monstre>=0&&monstre<(int)m_monstre.size())
                    {
                        moteurGraphique->LightManager->Generate(m_monstre[monstre].m_light);

                        bool explosif=false;
                        int degats = m_monstre[monstre].animer(&m_ModeleMonstre[m_monstre[monstre].getModele()],m_dimensions.y,temps,&explosif,positionHero);
                        if(degats>0&&!explosif)
                        {
                            if(m_monstre[monstre].m_miracleALancer==-1)
                                hero->infligerDegats(degats);
                            else
                            {
                                m_monstre[monstre].m_miracleEnCours.push_back(EntiteMiracle ());
                                m_monstre[monstre].m_miracleEnCours.back().m_infos.push_back(infosEntiteMiracle ());

                                m_monstre[monstre].m_miracleEnCours.back().m_modele=m_monstre[monstre].m_miracleALancer;

                                m_monstre[monstre].m_miracleEnCours.back().m_infos.back().m_position.x=m_monstre[monstre].getCoordonneePixel().x;
                                m_monstre[monstre].m_miracleEnCours.back().m_infos.back().m_position.y=m_monstre[monstre].getCoordonneePixel().y;

                                if(m_monstre[monstre].m_miracleEnCours.back().m_modele>=0&&m_monstre[monstre].m_miracleEnCours.back().m_modele<(int)m_ModeleMonstre[m_monstre[monstre].getModele()].m_miracles.size())
                                    gererMiracle(&m_monstre[monstre].m_miracleEnCours.back(),&m_ModeleMonstre[m_monstre[monstre].getModele()].m_miracles[m_monstre[monstre].m_miracleEnCours.back().m_modele],hero,1,m_monstre[monstre].getCoordonnee(),hero->m_personnage.getProchaineCase(),i);
                            }
                            m_monstre[monstre].m_miracleALancer=-1;
                        }
                        if(explosif&&degats>0)
                        {
                            coordonnee temp={k,j,-1,-1};

                            verifierDeclencheursDegats(j,k);

                            infligerDegatsMasse(temp,1,degats,1,hero,menu,camera);
                        }

                        m_monstre[monstre].m_nombreInvocation=0;

                        for(int i=0;i<(int)m_monstre[monstre].m_miracleEnCours.size();i++)
                        {
                            bool continuer=true;
                            for(int o=0;o<(int)m_monstre[monstre].m_miracleEnCours[i].m_infos.size()&&continuer;o++)
                            {
                                if(m_monstre[monstre].m_miracleEnCours[i].m_infos[o].m_effetEnCours>=0)
                                {
                                    if(m_ModeleMonstre[m_monstre[monstre].getModele()].m_miracles[m_monstre[monstre].m_miracleEnCours[i].m_modele].m_effets[m_monstre[monstre].m_miracleEnCours[i].m_infos[o].m_effetEnCours].m_sequence>=0&&m_ModeleMonstre[m_monstre[monstre].getModele()].m_miracles[m_monstre[monstre].m_miracleEnCours[i].m_modele].m_effets[m_monstre[monstre].m_miracleEnCours[i].m_infos[o].m_effetEnCours].m_sequence<(int)m_ModeleMonstre[m_monstre[monstre].getModele()].m_miracles[m_monstre[monstre].m_miracleEnCours[i].m_modele].m_tile.size())
                                        if(m_monstre[monstre].m_miracleEnCours[i].m_infos[o].m_imageEnCours>=0&&m_monstre[monstre].m_miracleEnCours[i].m_infos[o].m_imageEnCours<(int)m_ModeleMonstre[m_monstre[monstre].getModele()].m_miracles[m_monstre[monstre].m_miracleEnCours[i].m_modele].m_tile[m_ModeleMonstre[m_monstre[monstre].getModele()].m_miracles[m_monstre[monstre].m_miracleEnCours[i].m_modele].m_effets[m_monstre[monstre].m_miracleEnCours[i].m_infos[o].m_effetEnCours].m_sequence].size())
                                            m_monstre[monstre].m_miracleEnCours[i].m_infos[o].m_imageEnCours=m_ModeleMonstre[m_monstre[monstre].getModele()].m_miracles[m_monstre[monstre].m_miracleEnCours[i].m_modele].m_tile[m_ModeleMonstre[m_monstre[monstre].getModele()].m_miracles[m_monstre[monstre].m_miracleEnCours[i].m_modele].m_effets[m_monstre[monstre].m_miracleEnCours[i].m_infos[o].m_effetEnCours].m_sequence][m_monstre[monstre].m_miracleEnCours[i].m_infos[o].m_imageEnCours].getAnimation();

                                    if(continuer)
                                    if(m_ModeleMonstre[m_monstre[monstre].getModele()].m_miracles[m_monstre[monstre].m_miracleEnCours[i].m_modele].m_effets[m_monstre[monstre].m_miracleEnCours[i].m_infos[o].m_effetEnCours].m_type==PROJECTILE)
                                        if(m_monstre[monstre].m_miracleEnCours[i].m_infos[o].m_IDObjet>=0 && m_monstre[monstre].m_miracleEnCours[i].m_infos[o].m_IDObjet<(int)m_projectile.size())
                                        {
                                            float tempsAnimation = m_ModeleMonstre[m_monstre[monstre].getModele()].m_miracles[m_monstre[monstre].m_miracleEnCours[i].m_modele].m_tile[m_ModeleMonstre[m_monstre[monstre].getModele()].m_miracles[m_monstre[monstre].m_miracleEnCours[i].m_modele].m_effets[m_monstre[monstre].m_miracleEnCours[i].m_infos[o].m_effetEnCours].m_sequence][m_monstre[monstre].m_miracleEnCours[i].m_infos[o].m_imageEnCours].getTemps();
                                            m_projectile[m_monstre[monstre].m_miracleEnCours[i].m_infos[o].m_IDObjet].m_animation+=temps;

                                            moteurGraphique->LightManager->Generate(m_projectile[m_monstre[monstre].m_miracleEnCours[i].m_infos[o].m_IDObjet].m_light);

                                            if(m_projectile[m_monstre[monstre].m_miracleEnCours[i].m_infos[o].m_IDObjet].m_animation>=tempsAnimation)
                                            {
                                                m_projectile[m_monstre[monstre].m_miracleEnCours[i].m_infos[o].m_IDObjet].m_animation-=tempsAnimation;
                                                if(m_projectile[m_monstre[monstre].m_miracleEnCours[i].m_infos[o].m_IDObjet].m_actif)
                                                {
                                                    m_projectile[m_monstre[monstre].m_miracleEnCours[i].m_infos[o].m_IDObjet].m_image=m_ModeleMonstre[m_monstre[monstre].getModele()].m_miracles[m_monstre[monstre].m_miracleEnCours[i].m_modele].m_image[m_ModeleMonstre[m_monstre[monstre].getModele()].m_miracles[m_monstre[monstre].m_miracleEnCours[i].m_modele].m_tile[m_ModeleMonstre[m_monstre[monstre].getModele()].m_miracles[m_monstre[monstre].m_miracleEnCours[i].m_modele].m_effets[m_monstre[monstre].m_miracleEnCours[i].m_infos[o].m_effetEnCours].m_sequence][m_monstre[monstre].m_miracleEnCours[i].m_infos[o].m_imageEnCours].getImage()];
                                                    m_projectile[m_monstre[monstre].m_miracleEnCours[i].m_infos[o].m_IDObjet].m_positionImage= m_ModeleMonstre[m_monstre[monstre].getModele()].m_miracles[m_monstre[monstre].m_miracleEnCours[i].m_modele].m_tile[m_ModeleMonstre[m_monstre[monstre].getModele()].m_miracles[m_monstre[monstre].m_miracleEnCours[i].m_modele].m_effets[m_monstre[monstre].m_miracleEnCours[i].m_infos[o].m_effetEnCours].m_sequence][m_monstre[monstre].m_miracleEnCours[i].m_infos[o].m_imageEnCours].getCoordonnee();
                                                    m_projectile[m_monstre[monstre].m_miracleEnCours[i].m_infos[o].m_IDObjet].m_centre= m_ModeleMonstre[m_monstre[monstre].getModele()].m_miracles[m_monstre[monstre].m_miracleEnCours[i].m_modele].m_tile[m_ModeleMonstre[m_monstre[monstre].getModele()].m_miracles[m_monstre[monstre].m_miracleEnCours[i].m_modele].m_effets[m_monstre[monstre].m_miracleEnCours[i].m_infos[o].m_effetEnCours].m_sequence][m_monstre[monstre].m_miracleEnCours[i].m_infos[o].m_imageEnCours].getCentre();

                                                    m_projectile[m_monstre[monstre].m_miracleEnCours[i].m_infos[o].m_IDObjet].m_lumiere=m_ModeleMonstre[m_monstre[monstre].getModele()].m_miracles[m_monstre[monstre].m_miracleEnCours[i].m_modele].m_tile[m_ModeleMonstre[m_monstre[monstre].getModele()].m_miracles[m_monstre[monstre].m_miracleEnCours[i].m_modele].m_effets[m_monstre[monstre].m_miracleEnCours[i].m_infos[o].m_effetEnCours].m_sequence][m_monstre[monstre].m_miracleEnCours[i].m_infos[o].m_imageEnCours].getLumiere();

                                                    moteurGraphique->LightManager->SetIntensity(m_projectile[m_monstre[monstre].m_miracleEnCours[i].m_infos[o].m_IDObjet].m_light,m_projectile[m_monstre[monstre].m_miracleEnCours[i].m_infos[o].m_IDObjet].m_lumiere.intensite);
                                                    moteurGraphique->LightManager->SetRadius(m_projectile[m_monstre[monstre].m_miracleEnCours[i].m_infos[o].m_IDObjet].m_light,m_projectile[m_monstre[monstre].m_miracleEnCours[i].m_infos[o].m_IDObjet].m_lumiere.intensite*5);

                                                    m_monstre[monstre].m_miracleEnCours[i].m_infos[o].m_position=m_projectile[m_monstre[monstre].m_miracleEnCours[i].m_infos[o].m_IDObjet].m_position;
                                                }
                                                else
                                                {
                                                    for(int p=0;p<(int)m_ModeleMonstre[m_monstre[monstre].getModele()].m_miracles[m_monstre[monstre].m_miracleEnCours[i].m_modele].m_effets[m_monstre[monstre].m_miracleEnCours[i].m_infos[o].m_effetEnCours].m_lien.size();p++)
                                                    {
                                                        m_monstre[monstre].m_miracleEnCours[i].m_infos.push_back(infosEntiteMiracle ());
                                                        m_monstre[monstre].m_miracleEnCours[i].m_infos.back().m_effetEnCours=m_ModeleMonstre[m_monstre[monstre].getModele()].m_miracles[m_monstre[monstre].m_miracleEnCours[i].m_modele].m_effets[m_monstre[monstre].m_miracleEnCours[i].m_infos[o].m_effetEnCours].m_lien[p];
                                                        m_monstre[monstre].m_miracleEnCours[i].m_infos.back().m_position=m_monstre[monstre].m_miracleEnCours[i].m_infos[o].m_position;
                                                    }

                                                    m_monstre[monstre].m_miracleEnCours[i].m_infos.erase(m_monstre[monstre].m_miracleEnCours[i].m_infos.begin()+o);

                                                    continuer=false;
                                                }

                                            }

                                        }
                                    if(continuer)
                                    if(m_ModeleMonstre[m_monstre[monstre].getModele()].m_miracles[m_monstre[monstre].m_miracleEnCours[i].m_modele].m_effets[m_monstre[monstre].m_miracleEnCours[i].m_infos[o].m_effetEnCours].m_type==INVOCATION)
                                            m_monstre[monstre].m_nombreInvocation++;



                                    if(continuer)
                                    if(m_ModeleMonstre[m_monstre[monstre].getModele()].m_miracles[m_monstre[monstre].m_miracleEnCours[i].m_modele].m_effets[m_monstre[monstre].m_miracleEnCours[i].m_infos[o].m_effetEnCours].m_type==EFFET_GRAPHIQUE)
                                        if(m_monstre[monstre].m_miracleEnCours[i].m_infos[o].m_IDObjet>=0&&m_monstre[monstre].m_miracleEnCours[i].m_infos[o].m_IDObjet<(int)m_effets.size())
                                        {
                                            float tempsAnimation = m_ModeleMonstre[m_monstre[monstre].getModele()].m_miracles[m_monstre[monstre].m_miracleEnCours[i].m_modele].m_tile[m_ModeleMonstre[m_monstre[monstre].getModele()].m_miracles[m_monstre[monstre].m_miracleEnCours[i].m_modele].m_effets[m_monstre[monstre].m_miracleEnCours[i].m_infos[o].m_effetEnCours].m_sequence][m_monstre[monstre].m_miracleEnCours[i].m_infos[o].m_imageEnCours].getTemps();
                                            m_effets[m_monstre[monstre].m_miracleEnCours[i].m_infos[o].m_IDObjet].m_animation+=temps;

                                            if(m_effets[m_monstre[monstre].m_miracleEnCours[i].m_infos[o].m_IDObjet].m_animation>=tempsAnimation)
                                            {
                                                m_effets[m_monstre[monstre].m_miracleEnCours[i].m_infos[o].m_IDObjet].m_animation-=tempsAnimation;
                                                if(m_effets[m_monstre[monstre].m_miracleEnCours[i].m_infos[o].m_IDObjet].m_compteur>0)
                                                {
                                                    m_effets[m_monstre[monstre].m_miracleEnCours[i].m_infos[o].m_IDObjet].m_image=m_ModeleMonstre[m_monstre[monstre].getModele()].m_miracles[m_monstre[monstre].m_miracleEnCours[i].m_modele].m_image[m_ModeleMonstre[m_monstre[monstre].getModele()].m_miracles[m_monstre[monstre].m_miracleEnCours[i].m_modele].m_tile[m_ModeleMonstre[m_monstre[monstre].getModele()].m_miracles[m_monstre[monstre].m_miracleEnCours[i].m_modele].m_effets[m_monstre[monstre].m_miracleEnCours[i].m_infos[o].m_effetEnCours].m_sequence][m_monstre[monstre].m_miracleEnCours[i].m_infos[o].m_imageEnCours].getImage()];
                                                    m_effets[m_monstre[monstre].m_miracleEnCours[i].m_infos[o].m_IDObjet].m_positionImage= m_ModeleMonstre[m_monstre[monstre].getModele()].m_miracles[m_monstre[monstre].m_miracleEnCours[i].m_modele].m_tile[m_ModeleMonstre[m_monstre[monstre].getModele()].m_miracles[m_monstre[monstre].m_miracleEnCours[i].m_modele].m_effets[m_monstre[monstre].m_miracleEnCours[i].m_infos[o].m_effetEnCours].m_sequence][m_monstre[monstre].m_miracleEnCours[i].m_infos[o].m_imageEnCours].getCoordonnee();
                                                    m_effets[m_monstre[monstre].m_miracleEnCours[i].m_infos[o].m_IDObjet].m_compteur--;

                                                    m_effets[m_monstre[monstre].m_miracleEnCours[i].m_infos[o].m_IDObjet].m_lumiere=m_ModeleMonstre[m_monstre[monstre].getModele()].m_miracles[m_monstre[monstre].m_miracleEnCours[i].m_modele].m_tile[m_ModeleMonstre[m_monstre[monstre].getModele()].m_miracles[m_monstre[monstre].m_miracleEnCours[i].m_modele].m_effets[m_monstre[monstre].m_miracleEnCours[i].m_infos[o].m_effetEnCours].m_sequence][m_monstre[monstre].m_miracleEnCours[i].m_infos[o].m_imageEnCours].getLumiere();

                                                    moteurGraphique->LightManager->SetIntensity(m_effets[m_monstre[monstre].m_miracleEnCours[i].m_infos[o].m_IDObjet].m_light,m_effets[m_monstre[monstre].m_miracleEnCours[i].m_infos[o].m_IDObjet].m_lumiere.intensite);
                                                    moteurGraphique->LightManager->SetRadius(m_effets[m_monstre[monstre].m_miracleEnCours[i].m_infos[o].m_IDObjet].m_light,m_effets[m_monstre[monstre].m_miracleEnCours[i].m_infos[o].m_IDObjet].m_lumiere.intensite*5);
                                                    moteurGraphique->LightManager->SetColor(m_effets[m_monstre[monstre].m_miracleEnCours[i].m_infos[o].m_IDObjet].m_light,sf::Color(m_effets[m_monstre[monstre].m_miracleEnCours[i].m_infos[o].m_IDObjet].m_lumiere.rouge,m_effets[m_monstre[monstre].m_miracleEnCours[i].m_infos[o].m_IDObjet].m_lumiere.vert,m_effets[m_monstre[monstre].m_miracleEnCours[i].m_infos[o].m_IDObjet].m_lumiere.bleu));
                                                }

                                                moteurGraphique->LightManager->Generate(m_effets[m_monstre[monstre].m_miracleEnCours[i].m_infos[o].m_IDObjet].m_light);

                                                if(m_effets[m_monstre[monstre].m_miracleEnCours[i].m_infos[o].m_IDObjet].m_compteur<=0)
                                                {
                                                    m_effets[m_monstre[monstre].m_miracleEnCours[i].m_infos[o].m_IDObjet].m_actif=false;

                                                    for(int p=0;p<(int)m_ModeleMonstre[m_monstre[monstre].getModele()].m_miracles[m_monstre[monstre].m_miracleEnCours[i].m_modele].m_effets[m_monstre[monstre].m_miracleEnCours[i].m_infos[o].m_effetEnCours].m_lien.size();p++)
                                                    {
                                                        m_monstre[monstre].m_miracleEnCours[i].m_infos.push_back(infosEntiteMiracle ());
                                                        m_monstre[monstre].m_miracleEnCours[i].m_infos.back().m_effetEnCours=m_ModeleMonstre[m_monstre[monstre].getModele()].m_miracles[m_monstre[monstre].m_miracleEnCours[i].m_modele].m_effets[m_monstre[monstre].m_miracleEnCours[i].m_infos[o].m_effetEnCours].m_lien[p];
                                                        m_monstre[monstre].m_miracleEnCours[i].m_infos.back().m_position=m_monstre[monstre].m_miracleEnCours[i].m_infos[o].m_position;
                                                    }

                                                    m_monstre[monstre].m_miracleEnCours[i].m_infos.erase(m_monstre[monstre].m_miracleEnCours[i].m_infos.begin()+o);

                                                    continuer=false;
                                                }
                                            }
                                        }

                                        if(m_monstre[monstre].m_miracleEnCours[i].m_infos[o].m_effetEnCours>=0)
                                        {
                                            if(m_ModeleMonstre[m_monstre[monstre].getModele()].m_miracles[m_monstre[monstre].m_miracleEnCours[i].m_modele].m_effets[m_monstre[monstre].m_miracleEnCours[i].m_infos[o].m_effetEnCours].m_sequence>=0&&m_ModeleMonstre[m_monstre[monstre].getModele()].m_miracles[m_monstre[monstre].m_miracleEnCours[i].m_modele].m_effets[m_monstre[monstre].m_miracleEnCours[i].m_infos[o].m_effetEnCours].m_sequence<(int)m_ModeleMonstre[m_monstre[monstre].getModele()].m_miracles[m_monstre[monstre].m_miracleEnCours[i].m_modele].m_tile.size())
                                                if(m_monstre[monstre].m_miracleEnCours[i].m_infos[o].m_imageEnCours>=0&&m_monstre[monstre].m_miracleEnCours[i].m_infos[o].m_imageEnCours<(int)m_ModeleMonstre[m_monstre[monstre].getModele()].m_miracles[m_monstre[monstre].m_miracleEnCours[i].m_modele].m_tile[m_ModeleMonstre[m_monstre[monstre].getModele()].m_miracles[m_monstre[monstre].m_miracleEnCours[i].m_modele].m_effets[m_monstre[monstre].m_miracleEnCours[i].m_infos[o].m_effetEnCours].m_sequence].size())
                                                    if(m_ModeleMonstre[m_monstre[monstre].getModele()].m_miracles[m_monstre[monstre].m_miracleEnCours[i].m_modele].m_tile[m_ModeleMonstre[m_monstre[monstre].getModele()].m_miracles[m_monstre[monstre].m_miracleEnCours[i].m_modele].m_effets[m_monstre[monstre].m_miracleEnCours[i].m_infos[o].m_effetEnCours].m_sequence][m_monstre[monstre].m_miracleEnCours[i].m_infos[o].m_imageEnCours].getSon()>=0)
                                                    {
                                                        coordonnee position;
                                                        position.x=(k-j-1+m_dimensions.y)/5;
                                                        position.y=(k+j)/5;

                                                        m_ModeleMonstre[m_monstre[monstre].getModele()].m_miracles[m_monstre[monstre].m_miracleEnCours[i].m_modele].jouerSon(m_ModeleMonstre[m_monstre[monstre].getModele()].m_miracles[m_monstre[monstre].m_miracleEnCours[i].m_modele].m_tile[m_ModeleMonstre[m_monstre[monstre].getModele()].m_miracles[m_monstre[monstre].m_miracleEnCours[i].m_modele].m_effets[m_monstre[monstre].m_miracleEnCours[i].m_infos[o].m_effetEnCours].m_sequence][m_monstre[monstre].m_miracleEnCours[i].m_infos[o].m_imageEnCours].getSon(),position,positionHero);
                                                    }
                                        }

                                    gererMiracle(&m_monstre[monstre].m_miracleEnCours[i],&m_ModeleMonstre[m_monstre[monstre].getModele()].m_miracles[m_monstre[monstre].m_miracleEnCours[i].m_modele],hero,true,m_monstre[monstre].getCoordonnee(),hero->m_personnage.getProchaineCase(),i);
                                }
                                else
                                   m_monstre[monstre].m_miracleEnCours[i].m_infos.erase(m_monstre[monstre].m_miracleEnCours[i].m_infos.begin()+o);
                            }

                            if(m_monstre[monstre].m_miracleEnCours[i].m_infos.empty())
                               m_monstre[monstre].m_miracleEnCours.erase (m_monstre[monstre].m_miracleEnCours.begin()+i);
                        }
                   }
           }
}

void Map::gererEvenements(int evenement,int z,int couche,int x,int y)
{
    if(evenement>=0&&evenement<(int)m_evenement.size())
    {
        if(m_evenement[m_decor[couche][y][x].getEvenement()[z]].getType()==CHANGER_DECOR)
        {
            m_decor[couche][y][x].setTileset(m_evenement[evenement].getInformation(0));
            m_decor[couche][y][x].setTile(m_evenement[evenement].getInformation(1));

                if(m_decor[couche][y][x].getTileset()>=0&&m_decor[couche][y][x].getTileset()<(int)m_tileset.size())
                {
                    sf::Vector2f pos;
                    pos.x=(((x*COTE_TILE-y*COTE_TILE)*64/COTE_TILE+getDimensions().y*64));
                    pos.y=(((x*COTE_TILE+y*COTE_TILE)*64/COTE_TILE)/2+32)*2;
                    if(m_tileset[m_decor[couche][y][x].getTileset()].getLumiereDuTile(m_decor[couche][y][x].getTile()).intensite>0)
                    {
                        m_decor[couche][y][x].m_light=moteurGraphique->LightManager->Add_Dynamic_Light(pos,m_tileset[m_decor[couche][y][x].getTileset()].getLumiereDuTile(m_decor[couche][y][x].getTile()).intensite,m_tileset[m_decor[couche][y][x].getTileset()].getLumiereDuTile(m_decor[couche][y][x].getTile()).intensite*3,8,sf::Color(m_tileset[m_decor[couche][y][x].getTileset()].getLumiereDuTile(m_decor[couche][y][x].getTile()).rouge,m_tileset[m_decor[couche][y][x].getTileset()].getLumiereDuTile(m_decor[couche][y][x].getTile()).vert,m_tileset[m_decor[couche][y][x].getTileset()].getLumiereDuTile(m_decor[couche][y][x].getTile()).bleu));

                        moteurGraphique->LightManager->Generate(m_decor[couche][y][x].m_light);
                    }
                }
        }
        if(m_evenement[evenement].getType()==EXPLOSION)
        {
            m_decor[couche][y][x].setEvenement(-1,z);
            verifierDeclencheursDegats(y,x);
        }
    }
}

void Map::infligerDegatsMasse(coordonnee position,int rayon,int degats,bool sourceConcernee, Hero *hero,Menu *menu,sf::View *camera)
{
    for(int couche=0;couche<2;couche++)
        for(int y=position.y-rayon;y<=position.y+rayon;y++)
            for(int x=position.x-rayon;x<=position.x+rayon;x++)
                if(y>=0&&x>=0&&y<m_dimensions.y&&x<m_dimensions.x)
                    if(sourceConcernee||!sourceConcernee&&!(y==position.y&&x==position.x))
                    {
                        if(m_decor[couche][y][x].getMonstre()>=0&&m_decor[couche][y][x].getMonstre()<(int)m_monstre.size())
                            if(m_monstre[m_decor[couche][y][x].getMonstre()].enVie()&&m_monstre[m_decor[couche][y][x].getMonstre()].m_friendly!=true&&m_monstre[m_decor[couche][y][x].getMonstre()].getCaracteristique().rang>=0)
                            {
                                infligerDegats(m_decor[couche][y][x].getMonstre(), degats,menu,camera,hero,0);

                                coordonnee vecteur;

                                if(position.x-m_monstre[m_decor[couche][y][x].getMonstre()].getCoordonnee().x<0)
                                    vecteur.x=1;
                                else if(position.x-m_monstre[m_decor[couche][y][x].getMonstre()].getCoordonnee().x>0)
                                    vecteur.x=-1;
                                else
                                    vecteur.x=0;

                                if(position.y-m_monstre[m_decor[couche][y][x].getMonstre()].getCoordonnee().y<0)
                                    vecteur.y=1;
                                else if(position.y-m_monstre[m_decor[couche][y][x].getMonstre()].getCoordonnee().y>0)
                                    vecteur.y=-1;
                                else
                                    vecteur.y=0;

                                PousserMonstreCase(m_decor[couche][y][x].getMonstre(),vecteur);
                            }

                        if(y==hero->m_personnage.getCoordonnee().y&&x==hero->m_personnage.getCoordonnee().x)
                            hero->infligerDegats(degats);
                    }
}

void Map::verifierDeclencheursDegats(int i, int j)
{
    for(int o=0;o<2;o++)
        for(int y=i-1;y<=i+1;y++)
            for(int x=j-1;x<=j+1;x++)
                if(y>0&&x>0&&y<(int)m_dimensions.y&&x<m_dimensions.x)
                {
                    for(int z=0;z<(int)m_decor[o][y][x].getEvenement().size();z++)
                    {
                        if(m_decor[o][y][x].getEvenement()[z]>=0&&m_decor[o][y][x].getEvenement()[z]<(int)m_evenement.size())
                            if(m_evenement[m_decor[o][y][x].getEvenement()[z]].getType()==DECLENCHEUR_DEGAT_TO_EVENEMENT)
                            {
                                m_decor[o][y][x].setEvenement(m_evenement[m_decor[o][y][x].getEvenement()[z]].getInformation(0),z);
                                gererEvenements(m_decor[o][y][x].getEvenement()[z],z,o,x,y);
                            }
                    }
                }
}

void Map::musiquePlay(coordonnee position)
{
    if(m_nombreMusique>0)
        if(m_musiqueEnCours>=0&&m_musiqueEnCours<MAX_MUSIQUE)
        {
            Sound::Status Status = m_musique[m_musiqueEnCours].GetStatus();

            if(Status==0)
            {
                m_musiqueEnCours++;
                if(m_musiqueEnCours>=0&&m_musiqueEnCours<MAX_MUSIQUE&&m_musiqueEnCours<m_nombreMusique)
                    m_musique[m_musiqueEnCours].Play();
             }

            if(m_musiqueEnCours>=0&&m_musiqueEnCours<MAX_MUSIQUE&&m_musiqueEnCours<m_nombreMusique)
                m_musique[m_musiqueEnCours].SetPosition(-position.x, 0, position.y);
        }
        else
        m_musiqueEnCours=0;
}


#define RANDOMDISPLACE( )       if(m_monstre[m_decor[i][j][k].getMonstre()].enVie())   \
                                {   \
                                    if(m_monstre[m_decor[i][j][k].getMonstre()].getArrivee().x==m_monstre[m_decor[i][j][k].getMonstre()].getCoordonnee().x&&m_monstre[m_decor[i][j][k].getMonstre()].getArrivee().y==m_monstre[m_decor[i][j][k].getMonstre()].getCoordonnee().y) \
                                    {   \
                                        coordonnee arrivee;   \
                                        arrivee=m_monstre[m_decor[i][j][k].getMonstre()].getDepart();   \
                                        arrivee.x+=(3-rand()%(6));   \
                                        arrivee.y+=(3-rand()%(6));   \
                                        m_monstre[m_decor[i][j][k].getMonstre()].setArrivee(arrivee);   \
                                    }   \
                                    else   \
                                    {   \
                                        if(m_monstre[m_decor[i][j][k].getMonstre()].seDeplacer(temps*100,getDimensions()))   \
                                        {   \
                                            coordonnee tempCoord={hero->m_personnage.getProchaineCase().x,hero->m_personnage.getProchaineCase().y,-1,-1};   \
                                            m_monstre[m_decor[i][j][k].getMonstre()].pathfinding(getAlentourDuPersonnage(m_monstre[m_decor[i][j][k].getMonstre()].getCoordonnee()),tempCoord);   \
                                        }   \
                                    }   \
                                }

#define EVASION()               if(m_monstre[m_decor[i][j][k].getMonstre()].enVie())   \
                                {   \
                                    if(!m_monstre[m_decor[i][j][k].getMonstre()].frappeEnCours)\
                                    {\
                                        if(m_monstre[m_decor[i][j][k].getMonstre()].getArrivee().x==m_monstre[m_decor[i][j][k].getMonstre()].getCoordonnee().x&&m_monstre[m_decor[i][j][k].getMonstre()].getArrivee().y==m_monstre[m_decor[i][j][k].getMonstre()].getCoordonnee().y) \
                                        {   \
                                            coordonnee arrivee;   \
                                            arrivee=m_monstre[m_decor[i][j][k].getMonstre()].getCoordonnee();   \
                                            arrivee.x+=m_monstre[m_decor[i][j][k].getMonstre()].getCoordonnee().x-hero->m_personnage.getCoordonnee().x;   \
                                            arrivee.y+=m_monstre[m_decor[i][j][k].getMonstre()].getCoordonnee().y-hero->m_personnage.getCoordonnee().y;   \
                                            m_monstre[m_decor[i][j][k].getMonstre()].setArrivee(arrivee);   \
                                        }   \
                                        else   \
                                        {   \
                                            if(m_monstre[m_decor[i][j][k].getMonstre()].seDeplacer(temps*100,getDimensions()))   \
                                            {   \
                                                coordonnee tempCoord={hero->m_personnage.getProchaineCase().x,hero->m_personnage.getProchaineCase().y,-1,-1};   \
                                                m_monstre[m_decor[i][j][k].getMonstre()].pathfinding(getAlentourDuPersonnage(m_monstre[m_decor[i][j][k].getMonstre()].getCoordonnee()),tempCoord); \
                                            }   \
                                        }   \
                                    }\
                                }

#define USEMIRACLE(numero)  m_monstre[m_decor[i][j][k].getMonstre()].setArrivee(m_monstre[m_decor[i][j][k].getMonstre()].getProchaineCase()); \
                            if(m_monstre[m_decor[i][j][k].getMonstre()].seDeplacer(temps*100,getDimensions()))   \
                            { \
                                m_monstre[m_decor[i][j][k].getMonstre()].setArrivee(m_monstre[m_decor[i][j][k].getMonstre()].getCoordonnee()); \
                                \
                                if(m_monstre[m_decor[i][j][k].getMonstre()].m_miracleALancer==-1) \
                                    m_monstre[m_decor[i][j][k].getMonstre()].m_miracleALancer=numero;\
                                if(m_monstre[m_decor[i][j][k].getMonstre()].getEtat()!=2) \
                                    m_monstre[m_decor[i][j][k].getMonstre()].setEtat(2); \
                                m_monstre[m_decor[i][j][k].getMonstre()].frappe(m_monstre[m_decor[i][j][k].getMonstre()].getCoordonnee(),hero->m_personnage.getCoordonnee());   \
                            }

#define PLAYSOUND(numero)   if(m_monstre[m_decor[i][j][k].getMonstre()].getModele()>=0&&m_monstre[m_decor[i][j][k].getMonstre()].getModele()<(int)m_ModeleMonstre.size()) \
                            { \
                                coordonnee position; \
                                position.x=(m_monstre[m_decor[i][j][k].getMonstre()].getCoordonnee().x-m_monstre[m_decor[i][j][k].getMonstre()].getCoordonnee().y-1+m_dimensions.y)/5; \
                                position.y=(m_monstre[m_decor[i][j][k].getMonstre()].getCoordonnee().x+m_monstre[m_decor[i][j][k].getMonstre()].getCoordonnee().y)/5; \
                                 \
                                coordonnee positionHero; \
                                positionHero.x=(hero->m_personnage.getCoordonnee().x-hero->m_personnage.getCoordonnee().y-1+m_dimensions.y)/5; \
                                positionHero.y=(hero->m_personnage.getCoordonnee().x+hero->m_personnage.getCoordonnee().y)/5; \
                                 \
                                m_ModeleMonstre[m_monstre[m_decor[i][j][k].getMonstre()].getModele()].jouerSon(numero,position,positionHero,true); \
                            }

#define SHOOT()             m_monstre[m_decor[i][j][k].getMonstre()].setArrivee(m_monstre[m_decor[i][j][k].getMonstre()].getProchaineCase()); \
                            if(m_monstre[m_decor[i][j][k].getMonstre()].seDeplacer(temps*100,getDimensions()))   \
                            { \
                                m_monstre[m_decor[i][j][k].getMonstre()].setArrivee(m_monstre[m_decor[i][j][k].getMonstre()].getCoordonnee()); \
                                \
                                m_monstre[m_decor[i][j][k].getMonstre()].m_miracleALancer=-1;\
                                m_monstre[m_decor[i][j][k].getMonstre()].frappe(m_monstre[m_decor[i][j][k].getMonstre()].getCoordonnee(),hero->m_personnage.getCoordonnee());   \
                            }

#define FIGHT()                 if(!m_monstre[m_decor[i][j][k].getMonstre()].getErreurPathfinding())   \
                                {   \
                                    if(m_monstre[m_decor[i][j][k].getMonstre()].enVie())   \
                                    {   \
                                        coordonnee arrivee;   \
                                        \
                                        if(m_monstre[m_decor[i][j][k].getMonstre()].seDeplacer(temps*100,getDimensions()))   \
                                        {   \
                                            m_monstre[m_decor[i][j][k].getMonstre()].setDepart(); \
                                            if(fabs(m_monstre[m_decor[i][j][k].getMonstre()].getCoordonnee().x-hero->m_personnage.getCoordonnee().x)>1||fabs(m_monstre[m_decor[i][j][k].getMonstre()].getCoordonnee().y-hero->m_personnage.getCoordonnee().y)>1)   \
                                            {   \
                                                arrivee=hero->m_personnage.getProchaineCase();   \
                                                m_monstre[m_decor[i][j][k].getMonstre()].setArrivee(arrivee);   \
                                                \
                                                coordonnee tempCoord={hero->m_personnage.getProchaineCase().x,hero->m_personnage.getProchaineCase().y,-1,-1};   \
                                                m_monstre[m_decor[i][j][k].getMonstre()].pathfinding(getAlentourDuPersonnage(m_monstre[m_decor[i][j][k].getMonstre()].getCoordonnee()),tempCoord);   \
                                            }   \
                                            else  \
                                            {   \
                                                if(hero->m_personnage.enVie()<=0)   \
                                                    m_monstre[m_decor[i][j][k].getMonstre()].setVu(0);   \
                                                if(m_monstre[m_decor[i][j][k].getMonstre()].getEtat()!=2)   \
                                                    m_monstre[m_decor[i][j][k].getMonstre()].setEtat(2);   \
                                                m_monstre[m_decor[i][j][k].getMonstre()].frappe(m_monstre[m_decor[i][j][k].getMonstre()].getCoordonnee(),hero->m_personnage.getCoordonnee());  \
                                                m_monstre[m_decor[i][j][k].getMonstre()].setArrivee(m_monstre[m_decor[i][j][k].getMonstre()].getCoordonnee());   \
                                            }   \
                                        }   \
                                        \
                                        if(fabs(m_monstre[m_decor[i][j][k].getMonstre()].getCoordonnee().x-hero->m_personnage.getCoordonnee().x)>1||fabs(m_monstre[m_decor[i][j][k].getMonstre()].getCoordonnee().y-hero->m_personnage.getCoordonnee().y)>1)   \
                                        {   \
                                                if(m_monstre[m_decor[i][j][k].getMonstre()].getArrivee().x==m_monstre[m_decor[i][j][k].getMonstre()].getCoordonnee().x&&m_monstre[m_decor[i][j][k].getMonstre()].getArrivee().y==m_monstre[m_decor[i][j][k].getMonstre()].getCoordonnee().y)   \
                                                {   \
                                                    arrivee=hero->m_personnage.getProchaineCase();   \
                                                    m_monstre[m_decor[i][j][k].getMonstre()].setArrivee(arrivee);   \
                                                }   \
                                                \
                                        }   \
                                    }   \
                                } \


#define LISTE_INSTRUCTIONS(noInstruction) if(noInstruction>=0&&noInstruction<(int)script->m_instructions.size()) \
                                             { \
                                                if(script->m_instructions[noInstruction].nom=="fight") { FIGHT() } \
                                                if(script->m_instructions[noInstruction].nom=="evasion") { EVASION() } \
                                                if(script->m_instructions[noInstruction].nom=="useMiracle") { USEMIRACLE(script->m_instructions[noInstruction].valeurs.at(0)) } \
                                                if(script->m_instructions[noInstruction].nom=="shoot") { SHOOT() } \
                                                if(script->m_instructions[noInstruction].nom=="randomDisplace") { RANDOMDISPLACE() } \
                                                if(script->m_instructions[noInstruction].nom=="playSound") { PLAYSOUND(script->m_instructions[noInstruction].valeurs.at(0)) } \
                                                if(script->m_instructions[noInstruction].nom=="if") { gererConditions(script,noInstruction,i,j,k,hero,temps,camera,menu); } \
                                                if(script->m_instructions[noInstruction].nom=="variable") { script->variables[script->m_instructions[noInstruction].valeurs.at(0)]=script->m_instructions[noInstruction].valeurs.at(1); }  \
                                                if(script->m_instructions[noInstruction].nom=="incrementeVariable") { script->variables[script->m_instructions[noInstruction].valeurs.at(0)]+=script->m_instructions[noInstruction].valeurs.at(1); }  \
                                             }

#define LISTE_CONDITIONS(noInstruction) if(script->m_instructions[noInstruction].nom=="see")\
                                        {\
                                            if(!m_monstre[m_decor[i][j][k].getMonstre()].getVu()) \
                                                ok=false;\
                                        }\
                                        else if(script->m_instructions[noInstruction].nom=="numberInvocation")\
                                        {\
                                            if(m_monstre[m_decor[i][j][k].getMonstre()].m_nombreInvocation==script->m_instructions[noInstruction].valeurs.at(0)) \
                                                ok=true;\
                                            else\
                                                ok=false;\
                                        }\
                                        else if(script->m_instructions[noInstruction].nom=="distance")\
                                        {\
                                            if(((m_monstre[m_decor[i][j][k].getMonstre()].getCoordonnee().x-hero->m_personnage.getCoordonnee().x)*(m_monstre[m_decor[i][j][k].getMonstre()].getCoordonnee().x-hero->m_personnage.getCoordonnee().x) + (m_monstre[m_decor[i][j][k].getMonstre()].getCoordonnee().y-hero->m_personnage.getCoordonnee().y)*(m_monstre[m_decor[i][j][k].getMonstre()].getCoordonnee().y-hero->m_personnage.getCoordonnee().y)) < script->m_instructions[noInstruction].valeurs.at(0)*script->m_instructions[noInstruction].valeurs.at(0)) \
                                                ok=true;\
                                            else \
                                                ok=false;\
                                        }\
                                        else if(script->m_instructions[noInstruction].nom=="rand")\
                                        {\
                                            if(rand() % 100 <= script->m_instructions[noInstruction].valeurs.at(0)) \
                                                ok=true;\
                                            else \
                                                ok=false;\
                                        } \
                                        else if(script->m_instructions[noInstruction].nom=="variable")\
                                        {\
                                            if(script->variables[script->m_instructions[noInstruction].valeurs.at(0)]==script->m_instructions[noInstruction].valeurs.at(1)) \
                                                ok=true;\
                                            else \
                                                ok=false;\
                                        }


void Map::gererConditions(Script *script,int noInstruction,int i, int j, int k,Hero *hero,float temps,sf::View *camera,Menu *menu)
{
    if(noInstruction>=0&&noInstruction<(int)script->m_instructions.size())
    {
        int b=0;
        bool ok=true;
        for(;b<(int)script->m_instructions[noInstruction].valeurs.size()&&script->m_instructions[noInstruction].valeurs[b]!=-2;b++)
        {
            if(script->m_instructions[noInstruction].valeurs[b]>=0&&script->m_instructions[noInstruction].valeurs[b]<(int)script->m_instructions.size())
                LISTE_CONDITIONS(script->m_instructions[noInstruction].valeurs[b]);
        }
        for(;b<(int)script->m_instructions[noInstruction].valeurs.size()&&script->m_instructions[noInstruction].valeurs[b]!=-1&&script->m_instructions[noInstruction].valeurs[b]!=-3;b++)
            if(ok)
                if(script->m_instructions[noInstruction].valeurs[b]>=0&&script->m_instructions[noInstruction].valeurs[b]<(int)script->m_instructions.size())
                    LISTE_INSTRUCTIONS(script->m_instructions[noInstruction].valeurs[b])
        if(!ok)
            for(;b<(int)script->m_instructions[noInstruction].valeurs.size()&&script->m_instructions[noInstruction].valeurs[b]!=-1;b++)
                if(script->m_instructions[noInstruction].valeurs[b]>=0&&script->m_instructions[noInstruction].valeurs[b]<(int)script->m_instructions.size())
                    LISTE_INSTRUCTIONS(script->m_instructions[noInstruction].valeurs[b])
    }
}

void Map::gererMonstres(Hero *hero,float temps,sf::View *camera,Menu *menu)
{
    coordonnee vueMin,vueMax;

    vueMin.x=hero->m_personnage.getCoordonnee().x-10;
    vueMin.y=hero->m_personnage.getCoordonnee().y-10;
    vueMax.x=hero->m_personnage.getCoordonnee().x+10;
    vueMax.y=hero->m_personnage.getCoordonnee().y+10;

    if(vueMin.x<0) { vueMin.x=0; }
    if(vueMin.y<0) { vueMin.y=0; }
    if(vueMax.x>=m_dimensions.x) { vueMax.x=m_dimensions.x-1; }
    if(vueMax.y>=(int)m_dimensions.y) { vueMax.y=m_dimensions.y-1; }

    if(!m_projectile.empty())
    {
        int nombreInactif=0;
        for(int i=0;i<(int)m_projectile.size();i++)
        {
            m_projectile[i].m_dejaDeplace=false;
            if(!m_projectile[i].m_actif)
                nombreInactif++;

                        if(!m_projectile[i].m_dejaDeplace)
                        {
                            if(m_projectile[i].m_actif)
                            {
                                m_projectile[i].m_dejaDeplace=true;

                                int temp=i;
                                m_decor[m_projectile[temp].m_positionCase.h][m_projectile[i].m_positionCase.y][m_projectile[i].m_positionCase.x].setProjectile(-1);

                                if(m_projectile[temp].m_position.y/COTE_TILE>vueMin.y+1&&m_projectile[temp].m_position.y/COTE_TILE<vueMax.y-1&&m_projectile[temp].m_position.x/COTE_TILE>vueMin.x+1&&m_projectile[temp].m_position.x/COTE_TILE<vueMax.x-1)
                                {
                                    int typeCase=getTypeCase((int)(m_projectile[temp].m_positionCase.x),(int)(m_projectile[temp].m_positionCase.y));
                                    if(typeCase==2&&!m_projectile[temp].m_monstre)
                                    {}
                                    else if(hero->m_personnage.getCoordonnee().x==(int)((m_projectile[temp].m_position.x+32)/COTE_TILE)&&hero->m_personnage.getCoordonnee().y==(int)((m_projectile[temp].m_position.y+32)/COTE_TILE)&&m_projectile[temp].m_monstre)
                                    {
                                        m_projectile[temp].m_actif=false;
                                        hero->infligerDegats(m_projectile[temp].m_degats);
                                    }
                                    else if(typeCase==1)
                                        m_projectile[temp].m_actif=false;
                                    else
                                    {
                                        m_projectile[i].m_positionCase.y=(int)(m_projectile[temp].m_position.y/COTE_TILE);
                                        m_projectile[i].m_positionCase.x=(int)(m_projectile[temp].m_position.x/COTE_TILE);
                                        if(m_decor[m_projectile[i].m_positionCase.h][m_projectile[i].m_positionCase.y][m_projectile[i].m_positionCase.x].getProjectile()!=-1)
                                            if(m_projectile[i].m_positionCase.h==1)
                                                m_projectile[i].m_positionCase.h=0;
                                            else
                                                m_projectile[i].m_positionCase.h=1;

                                        m_decor[m_projectile[i].m_positionCase.h][m_projectile[i].m_positionCase.y][m_projectile[i].m_positionCase.x].setProjectile(temp);
                                    }
                                }
                                else
                                {
                                    m_projectile[temp].m_actif=false;
                                    if(m_decor[1][(int)(m_projectile[i].m_position.y/COTE_TILE)][(int)(m_projectile[i].m_position.x/COTE_TILE)].getProjectile()==i)
                                        m_decor[1][(int)(m_projectile[temp].m_position.y/COTE_TILE)][(int)(m_projectile[temp].m_position.x/COTE_TILE)].setProjectile(-1);
                                    if(m_decor[0][(int)(m_projectile[i].m_position.y/COTE_TILE)][(int)(m_projectile[i].m_position.x/COTE_TILE)].getProjectile()==i)
                                        m_decor[0][(int)(m_projectile[temp].m_position.y/COTE_TILE)][(int)(m_projectile[temp].m_position.x/COTE_TILE)].setProjectile(-1);
                                }

                                m_projectile[i].Deplacer(temps,getDimensions().y);
                            }
                            else
                            {
                                moteurGraphique->LightManager->Delete_Light(m_projectile[i].m_light);
                                m_decor[m_projectile[i].m_positionCase.h][m_projectile[i].m_positionCase.y][m_projectile[i].m_positionCase.x].setProjectile(-1);
                            }
                        }

        }
        if(nombreInactif==(int)m_projectile.size())
            m_projectile.clear();
    }

    if(!m_effets.empty())
    {
        int nombreInactif=0;
        for(int i=0;i<(int)m_effets.size();i++)
            if(!m_effets[i].m_actif)
                nombreInactif++,moteurGraphique->LightManager->Delete_Light(m_effets[i].m_light);
        if(nombreInactif==(int)m_effets.size())
            m_effets.clear();
    }

	for(int i=0;i<2;i++)
        for(int j=vueMin.y;j<vueMax.y;j++)
            for(int k=vueMin.x;k<vueMax.x;k++)
            {
                    if(m_decor[i][j][k].getMonstre()>=0&&m_decor[i][j][k].getMonstre()<(int)m_monstre.size())
                    if(m_monstre[m_decor[i][j][k].getMonstre()].enVie())
                    {
                        if(hero->m_personnage.enVie())
                            m_monstre[m_decor[i][j][k].getMonstre()].testerVision(hero->m_personnage.getCoordonnee());
                        else
                            m_monstre[m_decor[i][j][k].getMonstre()].setVu(0);


                        if(m_monstre[m_decor[i][j][k].getMonstre()].m_attente<=0)
                        {
                            Script *script=&m_ModeleMonstre[m_monstre[m_decor[i][j][k].getMonstre()].getModele()].m_scriptAI;
                            if((int)script->m_instructions.size()>0)
                                for(int a=0;a<(int)script->m_instructions[0].valeurs.size();a++)
                                    if(script->m_instructions[0].valeurs[a]>=0&&script->m_instructions[0].valeurs[a]<(int)script->m_instructions.size())
                                        LISTE_INSTRUCTIONS(script->m_instructions[0].valeurs[a])
                                        //if(script->m_instructions[script->m_instructions[0].valeurs[a]].nom=="if")
                                            //gererConditions(script,script->m_instructions[0].valeurs[a],i,j,k,hero,temps,camera,menu);

                            if(m_monstre[m_decor[i][j][k].getMonstre()].getErreurPathfinding())
                                RANDOMDISPLACE()
                        }
                        else
                            m_monstre[m_decor[i][j][k].getMonstre()].m_attente-=temps*100;

                        if(m_monstre[m_decor[i][j][k].getMonstre()].getErreurPathfinding())
                            m_monstre[m_decor[i][j][k].getMonstre()].m_compteur++;
                        else
                            m_monstre[m_decor[i][j][k].getMonstre()].m_compteur=0;

                        if(m_monstre[m_decor[i][j][k].getMonstre()].m_compteur>3)
                            m_monstre[m_decor[i][j][k].getMonstre()].m_attente=2,m_monstre[m_decor[i][j][k].getMonstre()].m_compteur=0,m_monstre[m_decor[i][j][k].getMonstre()].setArrivee(m_monstre[m_decor[i][j][k].getMonstre()].getCoordonnee());


                        //if(!m_monstre[m_decor[i][j][k].getMonstre()].enVie()&&m_monstre[m_decor[i][j][k].getMonstre()].getEtat()!=3)
                           // m_monstre[m_decor[i][j][k].getMonstre()].setEtat(3);


                        ///GESTION DES EVENEMENTS SUR LES MONSTRES
                        for(int l=0;l<2;l++)
                            if(m_monstre[m_decor[i][j][k].getMonstre()].getCoordonnee().y>=0&&m_monstre[m_decor[i][j][k].getMonstre()].getCoordonnee().y<(int)m_dimensions.y
                             &&m_monstre[m_decor[i][j][k].getMonstre()].getCoordonnee().x>=0&&m_monstre[m_decor[i][j][k].getMonstre()].getCoordonnee().x<m_dimensions.x)
                             for(int z=0;z<(int)m_decor[l][m_monstre[m_decor[i][j][k].getMonstre()].getCoordonnee().y][m_monstre[m_decor[i][j][k].getMonstre()].getCoordonnee().x].getEvenement().size();z++)
                                if(m_decor[l][m_monstre[m_decor[i][j][k].getMonstre()].getCoordonnee().y][m_monstre[m_decor[i][j][k].getMonstre()].getCoordonnee().x].getEvenement()[z]>=0&&m_decor[l][m_monstre[m_decor[i][j][k].getMonstre()].getCoordonnee().y][m_monstre[m_decor[i][j][k].getMonstre()].getCoordonnee().x].getEvenement()[z]<(int)m_evenement.size())
                                    if(m_evenement[m_decor[l][m_monstre[m_decor[i][j][k].getMonstre()].getCoordonnee().y][m_monstre[m_decor[i][j][k].getMonstre()].getCoordonnee().x].getEvenement()[z]].getType()==INFLIGER_DEGATS)
                                        infligerDegats(m_decor[i][j][k].getMonstre(),m_evenement[m_decor[l][m_monstre[m_decor[i][j][k].getMonstre()].getCoordonnee().y][m_monstre[m_decor[i][j][k].getMonstre()].getCoordonnee().x].getEvenement()[z]].getInformation(0)*temps*10,menu,camera,hero,0);

                        if(!(m_monstre[m_decor[i][j][k].getMonstre()].getCoordonnee().y==j&&m_monstre[m_decor[i][j][k].getMonstre()].getCoordonnee().x==k))
                            if(m_monstre[m_decor[i][j][k].getMonstre()].getCoordonnee().x>0&&m_monstre[m_decor[i][j][k].getMonstre()].getCoordonnee().x<m_dimensions.x&&m_monstre[m_decor[i][j][k].getMonstre()].getCoordonnee().y>0&&m_monstre[m_decor[i][j][k].getMonstre()].getCoordonnee().y<(int)m_dimensions.y)
                            {
                                if(m_decor[0][m_monstre[m_decor[i][j][k].getMonstre()].getCoordonnee().y][m_monstre[m_decor[i][j][k].getMonstre()].getCoordonnee().x].getMonstre()<0&&m_decor[1][m_monstre[m_decor[i][j][k].getMonstre()].getCoordonnee().y][m_monstre[m_decor[i][j][k].getMonstre()].getCoordonnee().x].getMonstre()<0)
                                    m_decor[i][m_monstre[m_decor[i][j][k].getMonstre()].getCoordonnee().y][m_monstre[m_decor[i][j][k].getMonstre()].getCoordonnee().x].setMonstre(m_decor[i][j][k].getMonstre());
                                else
                                {
                                    int temp=m_decor[i][j][k].getMonstre();
                                    if(m_decor[1][m_monstre[m_decor[i][j][k].getMonstre()].getCoordonnee().y][m_monstre[m_decor[i][j][k].getMonstre()].getCoordonnee().x].getMonstre()>=0)
                                        m_decor[0][m_monstre[m_decor[i][j][k].getMonstre()].getCoordonnee().y][m_monstre[m_decor[i][j][k].getMonstre()].getCoordonnee().x].setMonstre(m_decor[1][m_monstre[m_decor[i][j][k].getMonstre()].getCoordonnee().y][m_monstre[m_decor[i][j][k].getMonstre()].getCoordonnee().x].getMonstre());
                                    m_decor[1][m_monstre[m_decor[i][j][k].getMonstre()].getCoordonnee().y][m_monstre[m_decor[i][j][k].getMonstre()].getCoordonnee().x].setMonstre(temp);
                                }
                                m_decor[i][j][k].setMonstre(-1);
                            }

                    }

                    if(m_decor[i][j][k].getProjectile()<0||m_decor[i][j][k].getProjectile()>=(int)m_projectile.size())
                        m_decor[i][j][k].setProjectile(-1);

                    if(m_decor[i][j][k].getEffetGraphique()>=0&&m_decor[i][j][k].getEffetGraphique()<(int)m_effets.size())
                    {
                        if(!m_effets[m_decor[i][j][k].getEffetGraphique()].m_actif)
                            m_decor[i][j][k].setEffetGraphique(-1);
                    }
                    else
                        m_decor[i][j][k].setEffetGraphique(-1);
            }
}

bool Map::infligerDegats(int numeroMonstre, float degats,Menu *menu,sf::View *camera, Hero *hero,bool pousser)
{
    if(numeroMonstre>=0&&numeroMonstre<(int)m_monstre.size())
    {
        if(configuration->sang&&m_monstre[numeroMonstre].enVie())
            for(int i=0;i<5;i++)
                if(rand()%(100)>25&&m_monstre[numeroMonstre].getCaracteristique().sang)
                {
                    coordonneeDecimal position;
                    position.x=rand()%(600 - 100) + 100;
                    position.y=rand()%(400 - 100 ) + 100;

                    menu->AjouterSang(position);
                }

        float viePrecedente=m_monstre[numeroMonstre].getCaracteristique().vie;

        /*if(m_monstre[numeroMonstre].enVie()&&pousser)
        {
            double m=atan(((double)hero->m_personnage.getCoordonnee().y-(double)m_monstre[numeroMonstre].getCoordonnee().y)/(double)((double)hero->m_personnage.getCoordonnee().x-(double)m_monstre[numeroMonstre].getCoordonnee().x));
            if(hero->m_personnage.getCoordonnee().x-m_monstre[numeroMonstre].getCoordonnee().x<0)
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


        m_monstre[numeroMonstre].infligerDegats(degats);

        if(!m_monstre[numeroMonstre].enVie())
            if(m_monstreIllumine==numeroMonstre||hero->getMonstreVise()==numeroMonstre)
                m_monstreIllumine=-1,hero->setMonstreVise(-1);

        if(!m_monstre[numeroMonstre].enVie()&&viePrecedente>0)
        {
            for(int i=0;i<(int)m_monstre[numeroMonstre].m_miracleEnCours.size();i++)
            {
                for(int o=0;o<(int)m_monstre[numeroMonstre].m_miracleEnCours[i].m_infos.size();o++)
                    if(m_monstre[numeroMonstre].m_miracleEnCours[i].m_infos[o].m_effetEnCours>=0)
                        if(m_ModeleMonstre[m_monstre[numeroMonstre].getModele()].m_miracles[m_monstre[numeroMonstre].m_miracleEnCours[i].m_modele].m_effets[m_monstre[numeroMonstre].m_miracleEnCours[i].m_infos[o].m_effetEnCours].m_type==INVOCATION)
                            if(m_monstre[numeroMonstre].m_miracleEnCours[i].m_infos[o].m_IDObjet>=0&&m_monstre[numeroMonstre].m_miracleEnCours[i].m_infos[o].m_IDObjet<(int)m_monstre.size())
                                infligerDegats(m_monstre[numeroMonstre].m_miracleEnCours[i].m_infos[o].m_IDObjet, m_monstre[m_monstre[numeroMonstre].m_miracleEnCours[i].m_infos[o].m_IDObjet].getCaracteristique().vie ,menu,camera,hero,false);
            }

            //m_monstre[numeroMonstre].m_miracleEnCours.clear();

            if(m_monstre[numeroMonstre].getCaracteristique().pointAme>0)
            {
                coordonneeDecimal position;
                position.x=(((m_monstre[numeroMonstre].getCoordonnee().x-m_monstre[numeroMonstre].getCoordonnee().y-1+m_dimensions.y)*64)-camera->GetRect().Left+48-(configuration->Resolution.w/configuration->zoom/2-400));
                position.y=(((m_monstre[numeroMonstre].getCoordonnee().x+m_monstre[numeroMonstre].getCoordonnee().y)*32)-camera->GetRect().Top-96);
                menu->AjouterAme(position,m_monstre[numeroMonstre].getCaracteristique().pointAme);
            }
            if(configuration->particules&&m_ModeleMonstre[m_monstre[numeroMonstre].getModele()].m_particules>=0)
            {
                coordonnee position2;
                position2.x=(int)(((m_monstre[numeroMonstre].getCoordonneePixel().x-m_monstre[numeroMonstre].getCoordonneePixel().y)*64/COTE_TILE+m_dimensions.y*64));
                position2.y=(int)(((m_monstre[numeroMonstre].getCoordonneePixel().x+m_monstre[numeroMonstre].getCoordonneePixel().y)*64/COTE_TILE)/2);

                float force=((degats*2)/m_monstre[numeroMonstre].getCaracteristique().maxVie)*5,angle;

                if(force<7)
                    force=7;
                if(force>20)
                    force=20;


                double m=atan2(m_monstre[numeroMonstre].getCoordonneePixel().y-hero->m_personnage.getCoordonneePixel().y,m_monstre[numeroMonstre].getCoordonneePixel().x-hero->m_personnage.getCoordonneePixel().x);

                angle=(int)(0+(m*360)/(2*M_PI));
                if(angle>=360)
                    angle=0;
                if(angle<0)
                    angle=360+angle;

                angle+=45;

                sf::Color buffer(255,255,255);
                if(m_monstre[numeroMonstre].getPorteeLumineuse().intensite>0)
                {
                    buffer.r=m_monstre[numeroMonstre].getPorteeLumineuse().rouge;
                    buffer.g=m_monstre[numeroMonstre].getPorteeLumineuse().vert;
                    buffer.b=m_monstre[numeroMonstre].getPorteeLumineuse().bleu;
                }
                moteurGraphique->AjouterSystemeParticules(m_ModeleMonstre[m_monstre[numeroMonstre].getModele()].m_particules,position2,buffer,force,angle);
            }

            if(m_monstre[numeroMonstre].getCoordonnee().x>=0&&m_monstre[numeroMonstre].getCoordonnee().x<m_dimensions.x&&m_monstre[numeroMonstre].getCoordonnee().y>=0&&m_monstre[numeroMonstre].getCoordonnee().y<(int)m_dimensions.y)
                if(m_monstre[numeroMonstre].getModele()>=0&&m_monstre[numeroMonstre].getModele()<(int)m_ModeleMonstre.size())
                        for(int i=0;i<(int)m_ModeleMonstre[m_monstre[numeroMonstre].getModele()].getObjets().size();i++)
                            if(rand()%100000/(m_monstre[numeroMonstre].getCaracteristique().rang*3+1)<=m_ModeleMonstre[m_monstre[numeroMonstre].getModele()].getObjets()[i].getChanceTrouver())
                            {
                                Objet temp;
                                temp=m_ModeleMonstre[m_monstre[numeroMonstre].getModele()].getObjets()[i];
                                temp.Generer((m_monstre[numeroMonstre].getCaracteristique().rang*5+1));
                                m_decor[1][m_monstre[numeroMonstre].getCoordonnee().y][m_monstre[numeroMonstre].getCoordonnee().x].ajouterObjet(temp);
                            }

            return 1;
        }
    }
    return 0;
}

void Map::PousserMonstre(int numeroMonstre, coordonnee vecteur)
{
    if(numeroMonstre>=0&&numeroMonstre<(int)m_monstre.size())
        if(1!=getTypeCase((int)((m_monstre[numeroMonstre].getCoordonneePixel().x+(float)vecteur.x)/COTE_TILE),(int)((m_monstre[numeroMonstre].getCoordonneePixel().y+(float)vecteur.y)/COTE_TILE)))
            m_monstre[numeroMonstre].Pousser(vecteur);
}

void Map::PousserMonstreCase(int numeroMonstre, coordonnee vecteur)
{
    if(numeroMonstre>=0&&numeroMonstre<(int)m_monstre.size())
        if(1!=getTypeCase((int)((m_monstre[numeroMonstre].getCoordonnee().x+vecteur.x)),(int)((m_monstre[numeroMonstre].getCoordonnee().y+vecteur.y))))
            m_monstre[numeroMonstre].PousserCase(vecteur);
}

bool Map::ramasserObjet(Hero *hero,bool enMain)
{
    coordonnee position;

    if(enMain)
        position=hero->m_personnage.getCoordonnee();
    else
        position=hero->getChercherSac();


    if(position.x>=0&&position.x<m_dimensions.x
     &&position.y>=0&&position.y<(int)m_dimensions.y)
	    if(m_objetPointe>=0&&m_objetPointe<m_decor[1][position.y][position.x].getNombreObjets())
        {
            if(hero->ajouterObjet(*m_decor[1][position.y][position.x].getObjet(m_objetPointe),enMain))
            {
                m_decor[1][position.y][position.x].supprimerObjet(m_objetPointe);
                return true;
            }
        }
    return false;
}


void Map::AjouterObjet(Objet objet)
{
    if(objet.getPosition().x>=0&&objet.getPosition().x<m_dimensions.x&&objet.getPosition().y>=0&&objet.getPosition().y<(int)m_dimensions.y)
    {
        m_decor[1][objet.getPosition().y][objet.getPosition().x].ajouterObjet(objet);
    }
}

void Map::setVolumeMusique(int volume)
{
    for(int i=0;i<MAX_MUSIQUE;++i)
        m_musique[i].SetVolume(volume);
}

coordonnee Map::getSacPointe(){return m_sacPointe;}
int Map::getObjetPointe(){return m_objetPointe;}
int Map::getNombreObjets(coordonnee position)
{
    if(position.x>=0&&position.x<m_dimensions.x
     &&position.y>=0&&position.y<(int)m_dimensions.y)
        return m_decor[1][position.y][position.x].getNombreObjets();
    else
        return 0;
}

coordonnee Map::getPositionMonstre(int numeroMonstre)
{
    if(numeroMonstre>=0&&numeroMonstre<(int)m_monstre.size())
        return m_monstre[numeroMonstre].getCoordonnee();

    coordonnee temp={-1,-1,-1,-1};
    return temp;
}

Monstre *Map::getEntiteMonstre(int numeroMonstre)
{
    if(numeroMonstre>=0&&numeroMonstre<(int)m_monstre.size())
        return &m_monstre[numeroMonstre];
    else
    {
        return NULL;
    }
}

bool Map::getMonstreEnVie(int numeroMonstre)
{
    if(numeroMonstre>=0&&numeroMonstre<(int)m_monstre.size())
        return m_monstre[numeroMonstre].enVie();
    else
        return 0;
}

casePathfinding** Map::getAlentourDuPersonnage(coordonnee positionPersonnage)
{
    casePathfinding **grille = new casePathfinding* [20];

	for(int y=positionPersonnage.y-10;y<positionPersonnage.y+10;y++)
	{
	    grille[y-positionPersonnage.y+10] = new casePathfinding[20];
		for(int x=positionPersonnage.x-10;x<positionPersonnage.x+10;x++)
		{
		    grille[y-positionPersonnage.y+10][x-positionPersonnage.x+10].hauteur=0;
			if(y>=0&&x>=0&&x<m_dimensions.x&&y<(int)m_dimensions.y)
			{
				grille[y-positionPersonnage.y+10][x-positionPersonnage.x+10].collision=getCollision(x,y);
				//if(m_decor[0][y][x].getHauteur()>m_decor[1][y][x].getHauteur())
                    grille[y-positionPersonnage.y+10][x-positionPersonnage.x+10].hauteur=m_decor[0][y][x].getHauteur();
               // else
                   // grille[y-positionPersonnage.y+10][x-positionPersonnage.x+10].hauteur=m_decor[1][y][x].getHauteur();
			}
			else
				grille[y-positionPersonnage.y+10][x-positionPersonnage.x+10].collision=1;
		}
	}

	return grille;
}

int Map::getMonstreIllumine(){return m_monstreIllumine;}

coordonnee Map::getDimensions()
{
	coordonnee dimensions;

	dimensions.x=m_dimensions.x;
	dimensions.y=m_dimensions.y;

	return dimensions;
}

int Map::getEvenement(coordonnee casePointee)
{
    int temp=-1;
    if(casePointee.y>=0&&casePointee.y<(int)m_dimensions.y&&casePointee.x>=0&&casePointee.x<m_dimensions.x)
        for(int i=0;i<2;i++)
            if((int)m_decor[i][casePointee.y][casePointee.x].getEvenement().size()>0)
                if(m_decor[i][casePointee.y][casePointee.x].getEvenement()[0]>-1)
                    temp=m_decor[i][casePointee.y][casePointee.x].getEvenement()[0];

    return temp;
}

bool Map::getCollision(int positionX,int positionY)
{
	for(int i=0;i<2;i++)
	{
        if(m_decor[i][positionY][positionX].getTileset()>=0&&m_decor[i][positionY][positionX].getTileset()<(int)m_tileset.size())
            if(m_tileset[m_decor[i][positionY][positionX].getTileset()].getCollisionTile(m_decor[i][positionY][positionX].getTile()))
                return 1;

        if(m_decor[i][positionY][positionX].getMonstre()>-1&&m_decor[i][positionY][positionX].getMonstre()<(int)m_monstre.size())
            if(m_monstre[m_decor[i][positionY][positionX].getMonstre()].enVie()&&m_monstre[m_decor[i][positionY][positionX].getMonstre()].getCaracteristique().rang>=0)
                return 1;

        coordonnee enCours;

        enCours.x=positionX+1;
        enCours.y=positionY+1;
        if(enCours.x>=0&&enCours.y>=0&&enCours.x<m_dimensions.x&&enCours.y<(int)m_dimensions.y)
            if(m_decor[i][ enCours.y][enCours.x].getMonstre()>-1&&m_decor[i][ enCours.y][enCours.x].getMonstre()<(int)m_monstre.size())
                if(m_monstre[m_decor[i][ enCours.y][enCours.x].getMonstre()].enVie()&&m_monstre[m_decor[i][ enCours.y][enCours.x].getMonstre()].getCaracteristique().vitesse>0)
                    if(m_decor[i][enCours.y][enCours.x].getMonstre()<(int)m_monstre.size())
                        if((m_monstre[m_decor[i][enCours.y][enCours.x].getMonstre()].getProchaineCase().x==positionX&&m_monstre[m_decor[i][enCours.y][enCours.x].getMonstre()].getProchaineCase().y==positionY)||(m_monstre[m_decor[i][enCours.y][enCours.x].getMonstre()].getCoordonnee().x==positionX&&m_monstre[m_decor[i][enCours.y][enCours.x].getMonstre()].getCoordonnee().y==positionY))
                            return 1;
        enCours.x=positionX-1;
        enCours.y=positionY-1;
        if(enCours.x>=0&&enCours.y>=0&&enCours.x<m_dimensions.x&&enCours.y<(int)m_dimensions.y)
            if(m_decor[i][ enCours.y][enCours.x].getMonstre()>-1&&m_decor[i][ enCours.y][enCours.x].getMonstre()<(int)m_monstre.size())
                if(m_monstre[m_decor[i][ enCours.y][enCours.x].getMonstre()].enVie()&&m_monstre[m_decor[i][ enCours.y][enCours.x].getMonstre()].getCaracteristique().vitesse>0)
                    if(m_decor[i][enCours.y][enCours.x].getMonstre()<(int)m_monstre.size())
                        if((m_monstre[m_decor[i][enCours.y][enCours.x].getMonstre()].getProchaineCase().x==positionX&&m_monstre[m_decor[i][enCours.y][enCours.x].getMonstre()].getProchaineCase().y==positionY)||(m_monstre[m_decor[i][enCours.y][enCours.x].getMonstre()].getCoordonnee().x==positionX&&m_monstre[m_decor[i][enCours.y][enCours.x].getMonstre()].getCoordonnee().y==positionY))
                            return 1;
        enCours.x=positionX+1;
        enCours.y=positionY-1;
        if(enCours.x>=0&&enCours.y>=0&&enCours.x<m_dimensions.x&&enCours.y<(int)m_dimensions.y)
            if(m_decor[i][ enCours.y][enCours.x].getMonstre()>-1&&m_decor[i][ enCours.y][enCours.x].getMonstre()<(int)m_monstre.size())
                if(m_monstre[m_decor[i][ enCours.y][enCours.x].getMonstre()].enVie()&&m_monstre[m_decor[i][ enCours.y][enCours.x].getMonstre()].getCaracteristique().vitesse>0)
                    if(m_decor[i][enCours.y][enCours.x].getMonstre()<(int)m_monstre.size())
                        if((m_monstre[m_decor[i][enCours.y][enCours.x].getMonstre()].getProchaineCase().x==positionX&&m_monstre[m_decor[i][enCours.y][enCours.x].getMonstre()].getProchaineCase().y==positionY)||(m_monstre[m_decor[i][enCours.y][enCours.x].getMonstre()].getCoordonnee().x==positionX&&m_monstre[m_decor[i][enCours.y][enCours.x].getMonstre()].getCoordonnee().y==positionY))
                            return 1;
        enCours.x=positionX-1;
        enCours.y=positionY+1;
        if(enCours.x>=0&&enCours.y>=0&&enCours.x<m_dimensions.x&&enCours.y<(int)m_dimensions.y)
            if(m_decor[i][ enCours.y][enCours.x].getMonstre()>-1&&m_decor[i][ enCours.y][enCours.x].getMonstre()<(int)m_monstre.size())
                if(m_monstre[m_decor[i][ enCours.y][enCours.x].getMonstre()].enVie()&&m_monstre[m_decor[i][ enCours.y][enCours.x].getMonstre()].getCaracteristique().vitesse>0)
                    if(m_decor[i][enCours.y][enCours.x].getMonstre()<(int)m_monstre.size())
                        if((m_monstre[m_decor[i][enCours.y][enCours.x].getMonstre()].getProchaineCase().x==positionX&&m_monstre[m_decor[i][enCours.y][enCours.x].getMonstre()].getProchaineCase().y==positionY)||(m_monstre[m_decor[i][enCours.y][enCours.x].getMonstre()].getCoordonnee().x==positionX&&m_monstre[m_decor[i][enCours.y][enCours.x].getMonstre()].getCoordonnee().y==positionY))
                            return 1;
            enCours.x=positionX;
        enCours.y=positionY+1;
        if(enCours.x>=0&&enCours.y>=0&&enCours.x<m_dimensions.x&&enCours.y<(int)m_dimensions.y)
            if(m_decor[i][ enCours.y][enCours.x].getMonstre()>-1&&m_decor[i][ enCours.y][enCours.x].getMonstre()<(int)m_monstre.size())
                if(m_monstre[m_decor[i][ enCours.y][enCours.x].getMonstre()].enVie()&&m_monstre[m_decor[i][ enCours.y][enCours.x].getMonstre()].getCaracteristique().vitesse>0)
                    if(m_decor[i][enCours.y][enCours.x].getMonstre()<(int)m_monstre.size())
                        if((m_monstre[m_decor[i][enCours.y][enCours.x].getMonstre()].getProchaineCase().x==positionX&&m_monstre[m_decor[i][enCours.y][enCours.x].getMonstre()].getProchaineCase().y==positionY)||(m_monstre[m_decor[i][enCours.y][enCours.x].getMonstre()].getCoordonnee().x==positionX&&m_monstre[m_decor[i][enCours.y][enCours.x].getMonstre()].getCoordonnee().y==positionY))
                            return 1;
        enCours.x=positionX+1;
        enCours.y=positionY;
        if(enCours.x>=0&&enCours.y>=0&&enCours.x<m_dimensions.x&&enCours.y<(int)m_dimensions.y)
            if(m_decor[i][ enCours.y][enCours.x].getMonstre()>-1&&m_decor[i][ enCours.y][enCours.x].getMonstre()<(int)m_monstre.size())
                if(m_monstre[m_decor[i][ enCours.y][enCours.x].getMonstre()].enVie()&&m_monstre[m_decor[i][ enCours.y][enCours.x].getMonstre()].getCaracteristique().vitesse>0)
                    if(m_decor[i][enCours.y][enCours.x].getMonstre()<(int)m_monstre.size())
                        if((m_monstre[m_decor[i][enCours.y][enCours.x].getMonstre()].getProchaineCase().x==positionX&&m_monstre[m_decor[i][enCours.y][enCours.x].getMonstre()].getProchaineCase().y==positionY)||(m_monstre[m_decor[i][enCours.y][enCours.x].getMonstre()].getCoordonnee().x==positionX&&m_monstre[m_decor[i][enCours.y][enCours.x].getMonstre()].getCoordonnee().y==positionY))
                            return 1;
        enCours.x=positionX-1;
        enCours.y=positionY;
        if(enCours.x>=0&&enCours.y>=0&&enCours.x<m_dimensions.x&&enCours.y<(int)m_dimensions.y)
            if(m_decor[i][ enCours.y][enCours.x].getMonstre()>-1&&m_decor[i][ enCours.y][enCours.x].getMonstre()<(int)m_monstre.size())
                if(m_monstre[m_decor[i][ enCours.y][enCours.x].getMonstre()].enVie()&&m_monstre[m_decor[i][ enCours.y][enCours.x].getMonstre()].getCaracteristique().vitesse>0)
                    if(m_decor[i][enCours.y][enCours.x].getMonstre()<(int)m_monstre.size())
                        if((m_monstre[m_decor[i][enCours.y][enCours.x].getMonstre()].getProchaineCase().x==positionX&&m_monstre[m_decor[i][enCours.y][enCours.x].getMonstre()].getProchaineCase().y==positionY)||(m_monstre[m_decor[i][enCours.y][enCours.x].getMonstre()].getCoordonnee().x==positionX&&m_monstre[m_decor[i][enCours.y][enCours.x].getMonstre()].getCoordonnee().y==positionY))
                            return 1;
        enCours.x=positionX;
        enCours.y=positionY-1;
        if(enCours.x>=0&&enCours.y>=0&&enCours.x<m_dimensions.x&&enCours.y<(int)m_dimensions.y)
            if(m_decor[i][ enCours.y][enCours.x].getMonstre()>-1&&m_decor[i][ enCours.y][enCours.x].getMonstre()<(int)m_monstre.size())
                if(m_monstre[m_decor[i][ enCours.y][enCours.x].getMonstre()].enVie()&&m_monstre[m_decor[i][ enCours.y][enCours.x].getMonstre()].getCaracteristique().vitesse>0)
                    if(m_decor[i][enCours.y][enCours.x].getMonstre()<(int)m_monstre.size())
                        if((m_monstre[m_decor[i][enCours.y][enCours.x].getMonstre()].getProchaineCase().x==positionX&&m_monstre[m_decor[i][enCours.y][enCours.x].getMonstre()].getProchaineCase().y==positionY)||(m_monstre[m_decor[i][enCours.y][enCours.x].getMonstre()].getCoordonnee().x==positionX&&m_monstre[m_decor[i][enCours.y][enCours.x].getMonstre()].getCoordonnee().y==positionY))
                            return 1;
	}
	return 0;
}

int Map::getTypeCase(int positionX,int positionY)
{

	    if(positionY>=0&&positionY<(int)m_dimensions.y&&positionX>=0&&positionX<m_dimensions.x)
	    {
            if(m_decor[0][positionY][positionX].getTileset()>=0&&m_decor[0][positionY][positionX].getTileset()<(int)m_tileset.size())
                if(m_tileset[m_decor[0][positionY][positionX].getTileset()].getCollisionTile(m_decor[0][positionY][positionX].getTile()))
                    return 1;

            if(m_decor[0][positionY][positionX].getMonstre()>-1&&m_decor[0][positionY][positionX].getMonstre()<(int)m_monstre.size())
                if(m_monstre[m_decor[0][positionY][positionX].getMonstre()].enVie())
                    if(m_ModeleMonstre[m_monstre[m_decor[0][positionY][positionX].getMonstre()].getModele()].m_minimap)
                        if(m_monstre[m_decor[0][positionY][positionX].getMonstre()].m_friendly)
                            return 5;
                        else
                            return 2;

            if(m_decor[0][positionY][positionX].getNombreObjets())
                return 4;

            for(int z=0;z<(int)m_decor[0][positionY][positionX].getEvenement().size();z++)
                if(m_decor[0][positionY][positionX].getEvenement()[z]>=0&&m_decor[0][positionY][positionX].getEvenement()[z]<(int)m_evenement.size())
                    if(m_evenement[m_decor[0][positionY][positionX].getEvenement()[z]].getType()==CHANGEMENT_DE_MAP)
                        return 3;
	    }
	    else
            return 1;

        if(positionY>=0&&positionY<(int)m_dimensions.y&&positionX>=0&&positionX<m_dimensions.x)
	    {
            if(m_decor[1][positionY][positionX].getTileset()>=0&&m_decor[1][positionY][positionX].getTileset()<(int)m_tileset.size())
                if(m_tileset[m_decor[1][positionY][positionX].getTileset()].getCollisionTile(m_decor[1][positionY][positionX].getTile()))
                    return 1;

            if(m_decor[1][positionY][positionX].getMonstre()>-1&&m_decor[1][positionY][positionX].getMonstre()<(int)m_monstre.size())
                if(m_monstre[m_decor[1][positionY][positionX].getMonstre()].enVie())
                    if(m_ModeleMonstre[m_monstre[m_decor[1][positionY][positionX].getMonstre()].getModele()].m_minimap)
                        if(m_monstre[m_decor[1][positionY][positionX].getMonstre()].m_friendly)
                            return 5;
                        else
                            return 2;

            if(m_decor[1][positionY][positionX].getNombreObjets())
                return 4;

            for(int z=0;z<(int)m_decor[1][positionY][positionX].getEvenement().size();z++)
                if(m_decor[1][positionY][positionX].getEvenement()[z]>=0&&m_decor[1][positionY][positionX].getEvenement()[z]<(int)m_evenement.size())
                    if(m_evenement[m_decor[1][positionY][positionX].getEvenement()[z]].getType()==CHANGEMENT_DE_MAP)
                        return 3;
	    }
	    else
            return 1;


	return 0;
}

int Map::getMonstre(Hero *hero,View *camera,RenderWindow *ecran,coordonnee positionSouris,coordonnee casePointee)
{
    float distance=100000;
    int meilleur=-1;
    coordonnee vueMin,vueMax;

    vueMin.x=casePointee.x-2;
    vueMin.y=casePointee.y-2;
    vueMax.x=casePointee.x+2;
    vueMax.y=casePointee.y+2;


    if(vueMin.x<0) { vueMin.x=0; }
    if(vueMin.y<0) { vueMin.y=0; }
    if(vueMax.x>m_dimensions.x) { vueMax.x=m_dimensions.x; }
    if(vueMax.y>(int)m_dimensions.y) { vueMax.y=m_dimensions.y; }


    m_sacPointe.x=-1;
    m_sacPointe.y=-1;

	for(int i=0;i<2;i++)
	{
	    if(m_decor[i][casePointee.y][casePointee.x].getNombreObjets()>0)
        {
            m_sacPointe.x=casePointee.x;
            m_sacPointe.y=casePointee.y;
        }

        for(int j=vueMin.y;j<vueMax.y;j++)
            for(int k=vueMin.x;k<vueMax.x;k++)
            {
                    if(m_decor[i][j][k].getMonstre()>=0&&m_decor[i][j][k].getMonstre()<(int)m_monstre.size())
                    if(m_monstre[m_decor[i][j][k].getMonstre()].enVie()&&m_monstre[m_decor[i][j][k].getMonstre()].getCaracteristique().rang>=0)
                    {
                        coordonneeDecimal temp;
                        temp.x=(((m_monstre[m_decor[i][j][k].getMonstre()].getCoordonneePixel().x-m_monstre[m_decor[i][j][k].getMonstre()].getCoordonneePixel().y)*64/COTE_TILE+m_dimensions.y*64));
                        temp.y=(((m_monstre[m_decor[i][j][k].getMonstre()].getCoordonneePixel().x+m_monstre[m_decor[i][j][k].getMonstre()].getCoordonneePixel().y)*64/COTE_TILE)/2+32);


                        coordonnee positionSourisTotale;
                        positionSourisTotale.x=(int)ecran->ConvertCoords(ecran->GetInput().GetMouseX(),ecran->GetInput().GetMouseY()).x;
                        positionSourisTotale.y=(int)ecran->ConvertCoords(ecran->GetInput().GetMouseX(), ecran->GetInput().GetMouseY()).y;
                        if(positionSourisTotale.x>temp.x-64
                        &&positionSourisTotale.x<temp.x+64
                        &&positionSourisTotale.y>temp.y-128
                        &&positionSourisTotale.y<temp.y+32)
                        {
                            float temp2=0;
                            temp2=gpl::sqrt((temp.x-(positionSourisTotale.x))
                            *(temp.x-(positionSourisTotale.x))
                            +(temp.y-(positionSourisTotale.y+24))
                            *(temp.y-(positionSourisTotale.y+24)));

                            if(distance>temp2)
                                meilleur=m_decor[i][j][k].getMonstre(),distance=temp2;
                        }
                    }
            }
	}

                    m_monstreIllumine=meilleur;
                    return meilleur;
}
