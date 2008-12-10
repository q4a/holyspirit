#include "Map.h"

#include <iostream>
#include <fstream>
#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <dirent.h>

using namespace std;
using namespace sf;

#include "Globale.h"
#include "Jeu.h"

Map::Map()
{
    carreBrun.SetSmooth(true);
    carreBleu.SetSmooth(true);
    carreRouge.SetSmooth(true);
    carreVert.SetSmooth(true);
    carreJaune.SetSmooth(true);
    carreBrun.Create(8*configuration.Resolution.x/800, 8*configuration.Resolution.x/800, Color(128, 64, 0)),carreBleu.Create(8*configuration.Resolution.x/800, 8*configuration.Resolution.x/800, Color(32, 0, 128));
    carreRouge.Create(8*configuration.Resolution.x/800,8*configuration.Resolution.x/800, Color(128, 0, 0)),carreVert.Create(8*configuration.Resolution.x/800, 8*configuration.Resolution.x/800, Color(0, 128, 0));
    carreJaune.Create(8*configuration.Resolution.x/800, 8*configuration.Resolution.x/800, Color(255, 255, 64));
    IDImageSac=moteurGraphique.AjouterImage(configuration.chemin_menus+configuration.nom_sac);
}

Map::~Map()
{
    Detruire();
}

void Map::Detruire()
{
    m_tileset.clear();
    m_herbe.clear();

    for(int i=0;i<m_decor.size();i++)
    {
            for(int j=0;j<m_decor[0].size();j++)
                m_decor[i][j].clear();
        m_decor[i].clear();
    }
    m_decor.clear();

    m_ModeleMonstre.clear();
    m_monstre.clear();
    for(int i=0;i<m_evenement.size();i++)
        m_evenement[i].deleteInformations();
    m_evenement.clear();

    for(int i=0;i<MAX_MUSIQUE;i++)
        m_musique[i].Stop();

    m_fond.clear();
    m_cheminMusique.clear();
}

bool Map::Charger(int numeroMap)
{
    bool entite_map_existante=false;

    m_numero=numeroMap;
	char numero[7];
	string chemin = configuration.chemin_maps,chemin2 = configuration.chemin_temps+"map";

	sprintf(numero,"%ld.map.hs",numeroMap);
	chemin += numero;
	chemin2 += numero;

	console.Ajouter("",0);
	console.Ajouter("Chargement de la map : "+chemin);

	m_lumiere[0].intensite=1;
	m_lumiere[0].rouge=0;
	m_lumiere[0].vert=0;
	m_lumiere[0].bleu=0;

    m_musiqueEnCours=0;

	ifstream fichier,fichier2;
    fichier.open(chemin.c_str(), ios::in);

    struct dirent *lecture;

	DIR *repertoire;
    repertoire = opendir(configuration.chemin_temps.c_str());
    while ((lecture = readdir(repertoire)))
    {
        char temp2[255];
        string temp;
        sprintf(temp2,"map%ld.map.hs",numeroMap);
        temp=temp2;
        if(lecture->d_name==temp)
            fichier.close(),fichier.open(chemin2.c_str(), ios::in);


        sprintf(temp2,"entites_map%ld.emap.hs",numeroMap);
        temp=temp2;
        if(lecture->d_name==temp)
        {
            string cheminEntites = configuration.chemin_temps + temp;
            entite_map_existante=true,fichier2.open(cheminEntites.c_str(), ios::in);
        }
    }
    closedir(repertoire);

    Monstre monstreTemp;

    if(fichier)
    {
    	char caractere;
    	do
    	{
    	    //Chargement du nom
    		fichier.get(caractere);
    		if(caractere=='*')
    		{
    			string nom;
                getline(fichier, nom);
                m_nom=nom;
    		}

    		if(fichier.eof()){ char temp[1000]; sprintf(temp,"Erreur : Map \" %s \" Invalide",chemin.c_str());console.Ajouter(temp,1); throw (&temp); }
    	}while(caractere!='$');

    	do
        {
            fichier.get(caractere);

            if(caractere=='*')
    		{
    			string nom;
                getline(fichier, nom);
                m_fond.push_back(nom);
    		}

            if(fichier.eof()){ char temp[1000]; sprintf(temp,"Erreur : Map \" %s \" Invalide",chemin.c_str());console.Ajouter(temp,1); throw (&temp); }
        }while(caractere!='$');

        for(int i=0;i<MAX_MUSIQUE;i++)
                m_musique[i].Stop();

    	do
    	{
    	    //Chargement des musiques
    		fichier.get(caractere);
    		if(caractere=='*'&&m_musiqueEnCours<MAX_MUSIQUE)
    		{
    		    std::string temp2;

                getline(fichier, temp2);

                if(!m_musique[m_musiqueEnCours].OpenFromFile(temp2.c_str()))
                    console.Ajouter("Impossible de charger : "+temp2,1);
                else
                console.Ajouter("Chargement de : "+temp2,0);

                m_cheminMusique.push_back(temp2);

                m_musique[m_musiqueEnCours].SetLoop(false);
                m_musique[m_musiqueEnCours].SetVolume(0);

                m_musiqueEnCours++;
    		}
    		if(fichier.eof()){ char temp[1000]; sprintf(temp,"Erreur : Map \" %s \" Invalide",chemin.c_str()); throw (&temp); }
    	}while(caractere!='$');

    	m_musique[0].Play();

    	m_musiqueEnCours=0;

        int heureEnCours=0;
    	do
    	{

    	    //Chargement de la lumière ambiante
    		fichier.get(caractere);
    		if(caractere=='*')
    		{
    			fichier>>m_lumiere[heureEnCours].rouge;
    			fichier>>m_lumiere[heureEnCours].vert;
    			fichier>>m_lumiere[heureEnCours].bleu;
    			fichier>>m_lumiere[heureEnCours].intensite;
    			fichier>>m_lumiere[heureEnCours].hauteur;
    			heureEnCours++;
    		}
    		if(fichier.eof()){ char temp[1000]; sprintf(temp,"Erreur : Map \" %s \" Invalide",chemin.c_str());console.Ajouter(temp,1); throw (&temp); }
    	}while(caractere!='$');

    	while(heureEnCours<24)
    	{
    	    m_lumiere[heureEnCours].rouge=m_lumiere[0].rouge;
            m_lumiere[heureEnCours].vert=m_lumiere[0].vert;
            m_lumiere[heureEnCours].bleu=m_lumiere[0].bleu;
            m_lumiere[heureEnCours].intensite=m_lumiere[0].intensite;
            m_lumiere[heureEnCours].hauteur=m_lumiere[0].hauteur;
            heureEnCours++;
    	}

    	for(int i=0;i<24;i++)
            if(m_lumiere[i].intensite<1)
                m_lumiere[i].intensite=1;


    	Tileset tilesetTemp;
    	do
    	{
    	    //Chargement des tileset
    		fichier.get(caractere);
    		if(caractere=='*')
    		{
    			string cheminDuTileset;
                getline(fichier, cheminDuTileset);
                m_tileset.push_back(tilesetTemp);
                if(!m_tileset[m_tileset.size()-1].Charger(cheminDuTileset))
                    return 0;

    		}
    		if(fichier.eof()){ char temp[1000]; sprintf(temp,"Erreur : Map \" %s \" Invalide",chemin.c_str());console.Ajouter(temp,1); throw (&temp); }

    	}while(caractere!='$');

    	do
    	{
    	    //Chargement des tileset
    		fichier.get(caractere);
    		if(caractere=='*')
    		{
    			string cheminDuTileset;
                getline(fichier, cheminDuTileset);
                m_herbe.push_back(tilesetTemp);
                if(!m_herbe[m_herbe.size()-1].Charger(cheminDuTileset))
                    return 0;

    		}
    		if(fichier.eof()){ char temp[1000]; sprintf(temp,"Erreur : Map \" %s \" Invalide",chemin.c_str());console.Ajouter(temp,1); throw (&temp); }

    	}while(caractere!='$');

    	if(numeroMap==0)
            for(int i=0;i<5;i++)
                m_tileset.push_back(tilesetTemp);

    	Modele_Monstre monstreModeleTemp;
    	do
    	{

    	    //Chargement des tileset
    		fichier.get(caractere);
    		if(caractere=='*')
    		{
    			string cheminDuMonstre;
                getline(fichier, cheminDuMonstre);
                m_ModeleMonstre.push_back(monstreModeleTemp);
                if(!m_ModeleMonstre[m_ModeleMonstre.size()-1].Charger(cheminDuMonstre))
                    return 0;
                else
                console.Ajouter("Chargement de : "+cheminDuMonstre+" terminé",0);

    		}
    		if(fichier.eof()){ char temp[1000]; sprintf(temp,"Erreur : Map \" %s \" Invalide",chemin.c_str());console.Ajouter(temp,1); throw (&temp); }
    	}while(caractere!='$');

    	if(entite_map_existante)
        {
            if(fichier2)
            {
                char caractere;
                do
                {
                    //Chargement du nom
                    fichier2.get(caractere);
                    if(caractere=='*')
                    {
                        Lumiere lumiere;
                        int numeroModele=-1,vieMin=0,vieMax=1,degatsMin=0,degatsMax=0,rang=0,ame=0,pose=0,etat=0,angle=0;
                        float taille=1;
                        m_monstre.push_back(monstreTemp);

                        char caractere;
                        do
                        {
                            //Chargement du nom
                            fichier2.get(caractere);
                            switch(caractere)
                            {
                                case 'm': fichier2>>numeroModele; break;
                                case 'v': fichier2.get(caractere); if(caractere=='i') { fichier2>>vieMin; } else if(caractere=='a') { fichier2>>vieMax; } break;
                                case 'd': fichier2.get(caractere); if(caractere=='i') { fichier2>>degatsMin; } else if(caractere=='a') { fichier2>>degatsMax; } break;
                                case 'r': fichier2>>rang; break;
                                case 'a': fichier2>>ame; break;
                                case 't': fichier2>>taille; break;

                                case 'p': fichier2>>pose; break;
                                case 'e': fichier2>>etat; break;
                                case 'g': fichier2>>angle; break;

                                case 'l':
                                    fichier2.get(caractere);
                                    switch(caractere)
                                    {
                                        case 'r': fichier2>>lumiere.rouge; break;
                                        case 'v': fichier2>>lumiere.vert; break;
                                        case 'b': fichier2>>lumiere.bleu; break;
                                        case 'i': fichier2>>lumiere.intensite; break;
                                    }
                                break;
                            }

                            if(fichier2.eof()){ char temp[1000]; sprintf(temp,"Erreur : Map \" %s \" Invalide",chemin.c_str());console.Ajouter(temp,1); throw (&temp); }
                        }while(caractere!='$');

                        if(m_monstre.size()>0)
                        {
                            if(numeroModele>=0&&numeroModele<m_ModeleMonstre.size())
                                m_monstre[m_monstre.size()-1].Charger(numeroModele,&m_ModeleMonstre[numeroModele]);
                            Caracteristique caracteristique = m_monstre[m_monstre.size()-1].getCaracteristique();
                            caracteristique.vie=vieMin;
                            caracteristique.maxVie=vieMax;
                            caracteristique.degatsMin=degatsMin;
                            caracteristique.degatsMax=degatsMax;
                            caracteristique.rang=rang;
                            caracteristique.pointAme=ame;
                            caracteristique.modificateurTaille=taille;
                            m_monstre[m_monstre.size()-1].setCaracteristique(caracteristique);
                            m_monstre[m_monstre.size()-1].setPorteeLumineuse(lumiere);
                            m_monstre[m_monstre.size()-1].setEtat(etat);
                            m_monstre[m_monstre.size()-1].setPose(pose);
                            m_monstre[m_monstre.size()-1].setAngle(angle);
                        }
                    }

                    if(fichier2.eof()){ char temp[1000]; sprintf(temp,"Erreur : Map \" %s \" Invalide",chemin.c_str());console.Ajouter(temp,1); throw (&temp); }
                }while(caractere!='$');
            }
            fichier2.close();
        }

    	m_evenement.clear();

    	do
    	{
    		fichier.get(caractere);
    		if(caractere=='*')
    		{
    		    int numeroEvenement;
    			do
    			{
    				fichier.get(caractere);
    				switch (caractere)
    				{
    					case 'e': fichier>>numeroEvenement; break;
    				}
    				if(fichier.eof()){ char temp[1000]; sprintf(temp,"Erreur : Map \" %s \" Invalide",chemin.c_str());console.Ajouter(temp,1); throw (&temp); }
    			}while(caractere!='$');
    			m_evenement.push_back(numeroEvenement);

    			//AjouterEvenement(numeroEvenement);

    			int information;
    			do
    			{
    				fichier.get(caractere);
    				if(caractere=='i')
    				{
                        fichier>>information;
    				    m_evenement[m_evenement.size()-1].AjouterInformation(information);
    				}
    				if(fichier.eof()){ char temp[1000]; sprintf(temp,"Erreur : Map \" %s \" Invalide",chemin.c_str());console.Ajouter(temp,1); throw (&temp); }
    			}while(caractere!='$');

    			fichier.get(caractere);
    		}
    		if(fichier.eof()){ char temp[1000]; sprintf(temp,"Erreur : Map \" %s \" Invalide",chemin.c_str());console.Ajouter(temp,1); throw (&temp); }
    	}while(caractere!='$');


    	std::vector<int> evenementTemp;
    	Decor decorTemp(-1,-1,evenementTemp,-1,-1);

    	m_decor.resize(2,vector<vector<Decor> >(0,vector<Decor>(0,decorTemp)));

    	coordonnee position;
    	for(int couche=0;couche<2;couche++)
    	{
            position.x=0;
            position.y=0;


            do
            {
                //Chargement des "décors", des cases de la map
                fichier.get(caractere);
                if(caractere=='*')
                {
                   // if(couche==0)
                        m_decor[couche].push_back(vector<Decor> (0,decorTemp));
                    do
                    {
                        std::vector<int> evenement;
                        int tileset=-1,tileFinal=-1,herbe=-1,monstreFinal=-1;
                        int temp;
                        vector <int>tile;
                        vector <int>monstre;
                        do
                        {

                            fichier.get(caractere);
                            switch (caractere)
                            {
                                case 's': fichier>>tileset; break;
                                case 't': fichier>>temp; tile.push_back(temp); break;
                                case 'e': int temp2; fichier>>temp2; evenement.push_back(temp2); break;
                                 case 'm': if(!entite_map_existante) { fichier>>temp; monstre.push_back(temp);  } else {  fichier>>monstreFinal; } break;
                                case 'h': fichier>>herbe; break;
                            }
                            if(fichier.eof()){ char temp[1000]; sprintf(temp,"Erreur : Map \" %s \" Invalide",chemin.c_str());console.Ajouter(temp,1); throw (&temp); }
                        }while(caractere!='|');
                        //AjouterDecor(tileset,tile,evenement,position,couche,monstre);
                        //decorTemp2.setDecor(tileset,tile,evenement,monstre);

                        if(couche==1)
                            if(m_decor[0][position.y][position.x].getHerbe()>=0&&herbe<0)
                                herbe=m_decor[0][position.y][position.x].getHerbe();

                        if(!entite_map_existante)
                        {
                            if(monstre.size()>0)
                            {
                                int random = (rand() % (monstre.size() -1 - 0 + 1)) + 0;
                                if(random>=0&&random<monstre.size())
                                    monstreFinal = monstre[random];

                                monstre.clear();
                            }

                            if(monstreFinal>=0&&monstreFinal<m_ModeleMonstre.size())
                            {
                                m_monstre.push_back(monstreTemp);
                                m_monstre[m_monstre.size()-1].Charger(monstreFinal,&m_ModeleMonstre[monstreFinal]);
                                m_monstre[m_monstre.size()-1].setCoordonnee(position),m_monstre[m_monstre.size()-1].setDepart();
                                monstreFinal=m_monstre.size()-1;
                            }
                            else
                            monstreFinal=-1;
                        }
                        else
                        {
                            if(monstreFinal>=0&&monstreFinal<m_monstre.size())
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
                            if(random>=0&&random<tile.size())
                                tileFinal = tile[random];

                            tile.clear();
                        }

                        if(fichier.eof()){ char temp[1000]; sprintf(temp,"Erreur : Map \" %s \" Invalide",chemin.c_str());console.Ajouter(temp,1); throw (&temp); }


                        m_decor[couche][position.y].push_back(decorTemp);
                        m_decor[couche][position.y][position.x].setDecor(tileset,tileFinal,evenement,monstreFinal,herbe);
                        tileset=-1,tile.clear(),tileFinal=-1,evenement.clear(),monstreFinal=-1,herbe=-1;
                        position.x++;
                        fichier.get(caractere);
                    }while(caractere!='\n');
                    position.x=0;
                    position.y++;
                }
                if(fichier.eof()){ char temp[1000]; sprintf(temp,"Erreur : Map \" %s \" Invalide",chemin.c_str());console.Ajouter(temp,1);  throw (&temp); }

            }while(caractere!='$');

    	}
    }
    else
    {
        console.Ajouter("Impossible d'ouvrir le fichier : "+chemin,1);
        throw "";
    }
    fichier.close();

    for(int i=0;i<24;i++)
    if(m_lumiere[i].intensite<0)
        m_lumiere[i].intensite=0;
    for(int couche=0;couche<2;couche++)
        for(int i=0;i<m_decor[0].size();i++)
            for(int j=0;j<m_decor[0][0].size();j++)
            {
                if(m_decor[couche][i][j].getHerbe()>=0&&m_decor[couche][i][j].getHerbe()<m_herbe.size())
                    if(m_herbe[m_decor[couche][i][j].getHerbe()].getTaille()>0)
                    {
                        int numeroHerbe = (rand() % (m_herbe[m_decor[couche][i][j].getHerbe()].getTaille()));
                        m_decor[couche][i][j].setNumeroHerbe(numeroHerbe);
                    }
            }
    return 1;
}







void Map::Sauvegarder()
{
	char numero[7];
	string chemin = configuration.chemin_temps;

	sprintf(numero,"map%ld.map.hs",m_numero);
	chemin += numero;

	console.Ajouter("",0);
	console.Ajouter("Sauvegarde de la map : "+chemin,0);


	ofstream fichier;
    fichier.open(chemin.c_str(), ios::out);
    if(fichier)
    {
        fichier<<"*"<<m_nom<<"\n$\n";
        for(int i=0;i<m_fond.size();i++)
            fichier<<"*"<<m_fond[i]<<"\n";
        fichier<<"$\n";
        for(int i=0;i<m_cheminMusique.size();i++)
            fichier<<"*"<<m_cheminMusique[i]<<"\n";
        fichier<<"$\n";

        for(int i=0;i<24;i++)
            fichier<<"*"<<m_lumiere[i].rouge<<" "<<m_lumiere[i].vert<<" "<<m_lumiere[i].bleu<<" "<<m_lumiere[i].intensite<<" "<<m_lumiere[i].hauteur<<"\n";

        fichier<<"$\n";

        for(int i=0;i<m_tileset.size();i++)
            fichier<<"*"<<m_tileset[i].getChemin()<<"\n";

        fichier<<"$\n";

        for(int i=0;i<m_herbe.size();i++)
            fichier<<"*"<<m_herbe[i].getChemin()<<"\n";

        fichier<<"$\n";

        for(int i=0;i<m_ModeleMonstre.size();i++)
            fichier<<"*"<<m_ModeleMonstre[i].m_chemin<<"\n";

       /* fichier<<"$\n";

        for(int i=0;i<m_monstre.size();i++)
            fichier<<"* m"<<m_monstre[i].getModele()<<" $\n";*/

        fichier<<"$\n";

        for(int i=0;i<m_evenement.size();i++)
        {
            fichier<<"* e"<<m_evenement[i].getType()<<" $ * ";
            for(int j=0;j<m_evenement[i].getNombreInformation();j++)
                fichier<<"i"<<m_evenement[i].getInformation(j)<<" ";
            fichier<<"$\n";
        }

        fichier<<"$\n";

        for(int couche=0;couche<2;couche++)
    	{
    	    for(int i=0;i<m_decor[0].size();i++)
    	    {
    	        fichier<<"* ";
                for(int j=0;j<m_decor[0][0].size();j++)
                {
                    fichier<<" s"<<m_decor[couche][i][j].getTileset()<<" ";
                    fichier<<"t"<<m_decor[couche][i][j].getTile()<<" ";
                    for(int k=0;k<m_decor[couche][i][j].getEvenement().size();k++)
                        fichier<<"e"<<m_decor[couche][i][j].getEvenement()[k]<<" ";
                    if(m_decor[couche][i][j].getMonstre()>=0&&m_decor[couche][i][j].getMonstre()<m_monstre.size())
                        fichier<<"m"<<m_decor[couche][i][j].getMonstre()<<" ";
                            //fichier<<"m"<<m_monstre[m_decor[couche][i][j].getMonstre()].getModele()<<" ";
                    fichier<<"h"<<m_decor[couche][i][j].getHerbe()<<" ";

                    fichier<<"|";
                }
                fichier<<"\n";
    	    }
    	    fichier<<"$\n";
    	}
    }
    else
    {
        console.Ajouter("Impossible d'ouvrir le fichier : "+chemin,1);
        throw "";
    }
    fichier.close();



    chemin = configuration.chemin_temps;
    sprintf(numero,"entites_map%ld.emap.hs",m_numero);
	chemin += numero;
    fichier.open(chemin.c_str(), ios::out);
    if(fichier)
    {
        for(int i=0;i<m_monstre.size();i++)
        {
            fichier<<"* m"<<m_monstre[i].getModele()<<" vi"<<m_monstre[i].getCaracteristique().vie<<" va"<<m_monstre[i].getCaracteristique().maxVie<<" di"<<m_monstre[i].getCaracteristique().degatsMin<<" da"<<m_monstre[i].getCaracteristique().degatsMax<<" r"<<m_monstre[i].getCaracteristique().rang<<" a"<<m_monstre[i].getCaracteristique().pointAme<<" t"<<m_monstre[i].getCaracteristique().modificateurTaille
            <<" p"<<m_monstre[i].getPose()<<" e"<<m_monstre[i].getEtat()<<" g"<<m_monstre[i].getAngle()
            <<" lr"<<m_monstre[i].getPorteeLumineuse().rouge<<" lv"<<m_monstre[i].getPorteeLumineuse().vert<<" lb"<<m_monstre[i].getPorteeLumineuse().bleu<<" li"<<m_monstre[i].getPorteeLumineuse().intensite<<" $\n";
        }
        fichier<<"\n$";
    }
    fichier.close();
}





void Map::calculerOmbresEtLumieres(sf::RenderWindow* ecran,Hero *hero,sf::View *camera)
{
    hero->m_personnage.m_lumiere.detruire();

    //La, ça se complique, je vais essayer d'être clair, mais il ne faut pas hésiter à me redemander des explications ^^
    Lumiere lumiere,lumiereTile,lumiereMap;
    float angleOmbreMap;

    coordonnee vueMin,vueMax,lampesMin,lampesMax,distanceActuelle,position,positionPartieDecor;

     double lumiereTemp,Xtemp,Ytemp,angle=0;

     //Calcul des sources de lumière qui peuvent se trouver les plus loin du perso, tout en influant sur des tiles dans le champs de vision
    lampesMin.x=(int)((float)hero->m_personnage.getCoordonnee().x-15);
    lampesMin.y=(int)((float)hero->m_personnage.getCoordonnee().y-15);
    lampesMax.x=(int)((float)hero->m_personnage.getCoordonnee().x+15);
    lampesMax.y=(int)((float)hero->m_personnage.getCoordonnee().y+15);

    //Vérification que ces tiles soit bien dans la map
    if(lampesMin.x<0) { lampesMin.x=0; }
    if(lampesMin.y<0) { lampesMin.y=0; }
    if(lampesMax.x>getDimensions().x) { lampesMax.x=getDimensions().x; }
    if(lampesMax.y>getDimensions().y) { lampesMax.y=getDimensions().y; }


    //Calcul des tile qui peuvent se trouver les plus loin du perso, tout en restant dans le champs de vision
    vueMin.x=hero->m_personnage.getCoordonnee().x-15;
    vueMin.y=hero->m_personnage.getCoordonnee().y-15;
    vueMax.x=hero->m_personnage.getCoordonnee().x+15;
    vueMax.y=hero->m_personnage.getCoordonnee().y+15;

    //Vérification que ces tiles soit bien dans la map
    if(vueMin.x<0) { vueMin.x=0; }
    if(vueMin.y<0) { vueMin.y=0; }
    if(vueMax.x>getDimensions().x) { vueMax.x=getDimensions().x; }
    if(vueMax.y>getDimensions().y) { vueMax.y=getDimensions().y; }


    if(configuration.heure+1<24)
    {
        lumiereMap.intensite=(int)(((float)m_lumiere[configuration.heure].intensite*(60-configuration.minute)+((float)m_lumiere[configuration.heure+1].intensite*configuration.minute))*0.016666666666666666666666666666667);
        lumiereMap.rouge=(int)(((float)m_lumiere[configuration.heure].rouge*(60-configuration.minute)+((float)m_lumiere[configuration.heure+1].rouge*configuration.minute))*0.016666666666666666666666666666667);
        lumiereMap.vert=(int)(((float)m_lumiere[configuration.heure].vert*(60-configuration.minute)+((float)m_lumiere[configuration.heure+1].vert*configuration.minute))*0.016666666666666666666666666666667);
        lumiereMap.bleu=(int)(((float)m_lumiere[configuration.heure].bleu*(60-configuration.minute)+((float)m_lumiere[configuration.heure+1].bleu*configuration.minute))*0.016666666666666666666666666666667);
        lumiereMap.hauteur=((float)m_lumiere[configuration.heure].hauteur*(60-configuration.minute)+((float)m_lumiere[configuration.heure+1].hauteur*configuration.minute))*0.016666666666666666666666666666667;
    }
    else
    {
        lumiereMap.intensite=(int)(((float)m_lumiere[configuration.heure].intensite*(60-configuration.minute)+((float)m_lumiere[0].intensite*configuration.minute))*0.016666666666666666666666666666667);
        lumiereMap.rouge=(int)(((float)m_lumiere[configuration.heure].rouge*(60-configuration.minute)+((float)m_lumiere[0].rouge*configuration.minute))*0.016666666666666666666666666666667);
        lumiereMap.vert=(int)(((float)m_lumiere[configuration.heure].vert*(60-configuration.minute)+((float)m_lumiere[0].vert*configuration.minute))*0.016666666666666666666666666666667);
        lumiereMap.bleu=(int)(((float)m_lumiere[configuration.heure].bleu*(60-configuration.minute)+((float)m_lumiere[0].bleu*configuration.minute))*0.016666666666666666666666666666667);
        lumiereMap.hauteur=((float)m_lumiere[configuration.heure].hauteur*(60-configuration.minute)+((float)m_lumiere[0].hauteur*configuration.minute))*0.016666666666666666666666666666667;
    }

    angleOmbreMap=((float)configuration.heure*60+configuration.minute)*180/720;

    hero->m_personnage.m_lumiere=hero->m_modelePersonnage.getPorteeLumineuse();

    hero->m_personnage.m_lumiere.rouge=(lumiereMap.rouge*lumiereMap.intensite+hero->m_personnage.m_lumiere.rouge*hero->m_personnage.m_lumiere.intensite)/(hero->m_personnage.m_lumiere.intensite+lumiereMap.intensite);
    hero->m_personnage.m_lumiere.vert=(lumiereMap.vert*lumiereMap.intensite+hero->m_personnage.m_lumiere.vert*hero->m_personnage.m_lumiere.intensite)/(hero->m_personnage.m_lumiere.intensite+lumiereMap.intensite);
    hero->m_personnage.m_lumiere.bleu=(lumiereMap.bleu*lumiereMap.intensite+hero->m_personnage.m_lumiere.bleu*hero->m_personnage.m_lumiere.intensite)/(hero->m_personnage.m_lumiere.intensite+lumiereMap.intensite);

    hero->m_personnage.m_lumiere.intensite+=lumiereMap.intensite;
    if(hero->m_personnage.m_lumiere.intensite>255) hero->m_personnage.m_lumiere.intensite=255;
    if(hero->m_personnage.m_lumiere.intensite<0) hero->m_personnage.m_lumiere.intensite=0;

    if(configuration.Ombre)
        hero->m_personnage.m_lumiere.AjouterOmbre(lumiereMap.intensite/4,angleOmbreMap,lumiereMap.hauteur);


        // Je fonctionne comme pour en bas, juste que je prend le héro comme source de lumière, pour la portée lumineuse
    for(int i=0;i<30;++i)
        for(int j=0;j<30;++j)
        {
            m_tableauDesLampes[i][j].detruire();

             m_tableauDesLampes[i][j]=lumiereMap;
            if(configuration.Ombre)
                m_tableauDesLampes[i][j].AjouterOmbre((int)((float)lumiereMap.intensite*0.25),angleOmbreMap,lumiereMap.hauteur);

            if(i>=0&&i<vueMax.y-vueMin.y&&j>=0&&j<vueMax.x-vueMin.x)
            {

                lumiereTemp=0;
                lumiereTemp=(float)hero->m_modelePersonnage.getPorteeLumineuse().intensite-((((float)gpl::sqrt((hero->m_personnage.getCoordonneePixel().x-(j+vueMin.x)*COTE_TILE)*(hero->m_personnage.getCoordonneePixel().x-(j+vueMin.x)*COTE_TILE)+(hero->m_personnage.getCoordonneePixel().y-(i+vueMin.y)*COTE_TILE)*(hero->m_personnage.getCoordonneePixel().y-(i+vueMin.y)*COTE_TILE))))*DIVISEUR_COTE_TILE)*50;

                lumiere=hero->m_modelePersonnage.getPorteeLumineuse();

                lumiere.intensite=(int)lumiereTemp;

                float lumiereTemp2=0;

                 bool aMoitieSombre=true;

                lumiereTemp=0;
                lumiereTemp2=0;
                if(configuration.Lumiere==2)
                if(lumiere.intensite>0)
                    for(int o=1;o<10;++o)
                    {
                        coordonneeDecimal positionHero;

                        positionHero.x=(hero->m_personnage.getCoordonneePixel().x+1)*DIVISEUR_COTE_TILE;
                        positionHero.y=(hero->m_personnage.getCoordonneePixel().y+1)*DIVISEUR_COTE_TILE;

                        Xtemp=((float)((hero->m_personnage.getCoordonneePixel().x+(hero->m_personnage.getProchaineCase().x-hero->m_personnage.getCoordonnee().x)*8)*DIVISEUR_COTE_TILE-(j+vueMin.x))*0.1)*(float)o;
                        Ytemp=((float)((hero->m_personnage.getCoordonneePixel().y-(hero->m_personnage.getProchaineCase().y-hero->m_personnage.getCoordonnee().y)*8)*DIVISEUR_COTE_TILE-(i+vueMin.y))*0.1)*(float)o;

                        if(Xtemp-(int)Xtemp>0.5)
                            Xtemp=(int)Xtemp+1;

                        if(Ytemp-(int)Ytemp>0.5)
                            Ytemp=(int)Ytemp+1;


                        distanceActuelle.x=(j+vueMin.x)+(int)Xtemp;
                        distanceActuelle.y=(i+vueMin.y)+(int)Ytemp;

                        if(distanceActuelle.x>0&&distanceActuelle.x<m_decor[0][0].size()&&distanceActuelle.y>0&&distanceActuelle.y<m_decor[0].size())
                                if(m_decor[1][distanceActuelle.y][distanceActuelle.x].getTileset()>=0&&m_decor[1][distanceActuelle.y][distanceActuelle.x].getTileset()<m_tileset.size())
                                   if(m_tileset[m_decor[1][distanceActuelle.y][distanceActuelle.x].getTileset()].getLumiereDuTile(m_decor[1][distanceActuelle.y][distanceActuelle.x].getTile()).intensite<0&&m_tileset[m_decor[1][distanceActuelle.y][distanceActuelle.x].getTileset()].getLumiereDuTile(m_decor[1][distanceActuelle.y][distanceActuelle.x].getTile()).intensite<lumiereTemp)
                                    {


                                        bool erreur=false;
                                        if(m_tileset[m_decor[1][distanceActuelle.y][distanceActuelle.x].getTileset()].getOrientationDuTile(m_decor[1][distanceActuelle.y][distanceActuelle.x].getTile())=='y'||m_tileset[m_decor[1][distanceActuelle.y][distanceActuelle.x].getTileset()].getOrientationDuTile(m_decor[1][distanceActuelle.y][distanceActuelle.x].getTile())=='d'||m_tileset[m_decor[1][distanceActuelle.y][distanceActuelle.x].getTileset()].getOrientationDuTile(m_decor[1][distanceActuelle.y][distanceActuelle.x].getTile())=='b')
                                            if((j+vueMin.x)==distanceActuelle.x&&(j+vueMin.x)<positionHero.x)
                                                erreur=true;

                                        if(m_tileset[m_decor[1][distanceActuelle.y][distanceActuelle.x].getTileset()].getOrientationDuTile(m_decor[1][distanceActuelle.y][distanceActuelle.x].getTile())=='x'||m_tileset[m_decor[1][distanceActuelle.y][distanceActuelle.x].getTileset()].getOrientationDuTile(m_decor[1][distanceActuelle.y][distanceActuelle.x].getTile())=='g'||m_tileset[m_decor[1][distanceActuelle.y][distanceActuelle.x].getTileset()].getOrientationDuTile(m_decor[1][distanceActuelle.y][distanceActuelle.x].getTile())=='b')
                                            if((i+vueMin.y)==distanceActuelle.y&&(i+vueMin.y)<=positionHero.y)
                                                erreur=true;

                                        if(!erreur)
                                        if(!((i+vueMin.y)==distanceActuelle.y&&(j+vueMin.x)==distanceActuelle.x))
                                            lumiereTemp=m_tileset[m_decor[1][distanceActuelle.y][distanceActuelle.x].getTileset()].getLumiereDuTile(m_decor[1][distanceActuelle.y][distanceActuelle.x].getTile()).intensite,aMoitieSombre=true;
                                        else
                                        {

                                            if(distanceActuelle.x>positionHero.x&&distanceActuelle.y>positionHero.y)
                                                lumiereTemp=m_tileset[m_decor[1][distanceActuelle.y][distanceActuelle.x].getTileset()].getLumiereDuTile(m_decor[1][distanceActuelle.y][distanceActuelle.x].getTile()).intensite,aMoitieSombre=true;

                                            if(distanceActuelle.x>positionHero.x&&distanceActuelle.y<=positionHero.y)
                                            {
                                                if(m_tileset[m_decor[1][distanceActuelle.y][distanceActuelle.x].getTileset()].getOrientationDuTile(m_decor[1][distanceActuelle.y][distanceActuelle.x].getTile())=='y'||m_tileset[m_decor[1][distanceActuelle.y][distanceActuelle.x].getTileset()].getOrientationDuTile(m_decor[1][distanceActuelle.y][distanceActuelle.x].getTile())=='d'||m_tileset[m_decor[1][distanceActuelle.y][distanceActuelle.x].getTileset()].getOrientationDuTile(m_decor[1][distanceActuelle.y][distanceActuelle.x].getTile())=='h')
                                                    lumiereTemp=m_tileset[m_decor[1][distanceActuelle.y][distanceActuelle.x].getTileset()].getLumiereDuTile(m_decor[1][distanceActuelle.y][distanceActuelle.x].getTile()).intensite,aMoitieSombre=true;
                                            }
                                            if(distanceActuelle.x<=positionHero.x&&distanceActuelle.y>positionHero.y)
                                            {
                                                if(m_tileset[m_decor[1][distanceActuelle.y][distanceActuelle.x].getTileset()].getOrientationDuTile(m_decor[1][distanceActuelle.y][distanceActuelle.x].getTile())=='x'||m_tileset[m_decor[1][distanceActuelle.y][distanceActuelle.x].getTileset()].getOrientationDuTile(m_decor[1][distanceActuelle.y][distanceActuelle.x].getTile())=='g'||m_tileset[m_decor[1][distanceActuelle.y][distanceActuelle.x].getTileset()].getOrientationDuTile(m_decor[1][distanceActuelle.y][distanceActuelle.x].getTile())=='h')
                                                    lumiereTemp=m_tileset[m_decor[1][distanceActuelle.y][distanceActuelle.x].getTileset()].getLumiereDuTile(m_decor[1][distanceActuelle.y][distanceActuelle.x].getTile()).intensite,aMoitieSombre=true;
                                            }

                                        }

                                    }
                    }


                if((int)lumiereTemp<0||lumiereTemp2<0)
                {
                    lumiere.intensite+=(int)(lumiereTemp);
                }

                if(lumiere.intensite>255)
                    lumiere.intensite=255;
                if(lumiere.intensite<0)
                    lumiere.intensite=0;


                if(lumiere.intensite>0)
                {
                    m_tableauDesLampes[i][j].intensite+=lumiere.intensite;
                }

                if(lumiere.intensite>0)
                {
                    m_tableauDesLampes[i][j].rouge=(lumiere.rouge*lumiere.intensite/(m_tableauDesLampes[i][j].intensite+lumiere.intensite)+m_tableauDesLampes[i][j].rouge*m_tableauDesLampes[i][j].intensite/(m_tableauDesLampes[i][j].intensite+lumiere.intensite));
                    m_tableauDesLampes[i][j].vert=(lumiere.vert*lumiere.intensite/(m_tableauDesLampes[i][j].intensite+lumiere.intensite)+m_tableauDesLampes[i][j].vert*m_tableauDesLampes[i][j].intensite/(m_tableauDesLampes[i][j].intensite+lumiere.intensite));
                    m_tableauDesLampes[i][j].bleu=(lumiere.bleu*lumiere.intensite/(m_tableauDesLampes[i][j].intensite+lumiere.intensite)+m_tableauDesLampes[i][j].bleu*m_tableauDesLampes[i][j].intensite/(m_tableauDesLampes[i][j].intensite+lumiere.intensite));

                }

                if(m_tableauDesLampes[i][j].intensite>255)
                    m_tableauDesLampes[i][j].intensite=255;
                if(m_tableauDesLampes[i][j].intensite<0)
                    m_tableauDesLampes[i][j].intensite=0;
                if(m_tableauDesLampes[i][j].rouge>255)
                    m_tableauDesLampes[i][j].rouge=255;
                if(m_tableauDesLampes[i][j].vert>255)
                    m_tableauDesLampes[i][j].vert=255;
                if(m_tableauDesLampes[i][j].bleu>255)
                    m_tableauDesLampes[i][j].bleu=255;

                if(i==hero->m_personnage.getCoordonnee().y&&j==hero->m_personnage.getCoordonnee().x)
                {
                    if(lumiere.intensite>0)
                        hero->m_personnage.m_lumiere.intensite+=lumiere.intensite;

                    if(lumiere.intensite>0)
                    {
                        hero->m_personnage.m_lumiere.rouge=(lumiere.rouge*lumiere.intensite/(hero->m_personnage.m_lumiere.intensite+lumiere.intensite)+hero->m_personnage.m_lumiere.rouge*hero->m_personnage.m_lumiere.intensite/(hero->m_personnage.m_lumiere.intensite+lumiere.intensite));
                        hero->m_personnage.m_lumiere.vert=(lumiere.vert*lumiere.intensite/(hero->m_personnage.m_lumiere.intensite+lumiere.intensite)+hero->m_personnage.m_lumiere.vert*hero->m_personnage.m_lumiere.intensite/(hero->m_personnage.m_lumiere.intensite+lumiere.intensite));
                        hero->m_personnage.m_lumiere.bleu=(lumiere.bleu*lumiere.intensite/(hero->m_personnage.m_lumiere.intensite+lumiere.intensite)+hero->m_personnage.m_lumiere.bleu*hero->m_personnage.m_lumiere.intensite/(hero->m_personnage.m_lumiere.intensite+lumiere.intensite));

                    }

                    if(hero->m_personnage.m_lumiere.intensite>255)
                        hero->m_personnage.m_lumiere.intensite=255;
                    if(hero->m_personnage.m_lumiere.intensite<0)
                        hero->m_personnage.m_lumiere.intensite=0;
                    if(hero->m_personnage.m_lumiere.rouge>255)
                        hero->m_personnage.m_lumiere.rouge=255;
                    if(hero->m_personnage.m_lumiere.vert>255)
                        hero->m_personnage.m_lumiere.vert=255;
                    if(hero->m_personnage.m_lumiere.bleu>255)
                        hero->m_personnage.m_lumiere.bleu=255;
                }

                if(m_decor[0][i+vueMin.y][j+vueMin.x].getMonstre()>=0&&m_decor[0][i+vueMin.y][j+vueMin.x].getMonstre()<m_monstre.size())
                {
                    m_monstre[m_decor[0][i+vueMin.y][j+vueMin.x].getMonstre()].m_lumiere.detruire();

                    m_monstre[m_decor[0][i+vueMin.y][j+vueMin.x].getMonstre()].m_lumiere.intensite=m_tableauDesLampes[i][j].intensite;
                    m_monstre[m_decor[0][i+vueMin.y][j+vueMin.x].getMonstre()].m_lumiere.rouge=m_tableauDesLampes[i][j].rouge;
                    m_monstre[m_decor[0][i+vueMin.y][j+vueMin.x].getMonstre()].m_lumiere.vert=m_tableauDesLampes[i][j].vert;
                    m_monstre[m_decor[0][i+vueMin.y][j+vueMin.x].getMonstre()].m_lumiere.bleu=m_tableauDesLampes[i][j].bleu;
                    m_monstre[m_decor[0][i+vueMin.y][j+vueMin.x].getMonstre()].m_lumiere.hauteur=m_tableauDesLampes[i][j].hauteur;

                    if(configuration.Ombre)
                        m_monstre[m_decor[0][i+vueMin.y][j+vueMin.x].getMonstre()].m_lumiere.AjouterOmbre(lumiereMap.intensite/4,angleOmbreMap,lumiereMap.hauteur);
                }
                if(m_decor[1][i+vueMin.y][j+vueMin.x].getMonstre()>=0&&m_decor[1][i+vueMin.y][j+vueMin.x].getMonstre()<m_monstre.size())
                {
                    m_monstre[m_decor[1][i+vueMin.y][j+vueMin.x].getMonstre()].m_lumiere.detruire();

                    m_monstre[m_decor[1][i+vueMin.y][j+vueMin.x].getMonstre()].m_lumiere.intensite=m_tableauDesLampes[i][j].intensite;
                    m_monstre[m_decor[1][i+vueMin.y][j+vueMin.x].getMonstre()].m_lumiere.rouge=m_tableauDesLampes[i][j].rouge;
                    m_monstre[m_decor[1][i+vueMin.y][j+vueMin.x].getMonstre()].m_lumiere.vert=m_tableauDesLampes[i][j].vert;
                    m_monstre[m_decor[1][i+vueMin.y][j+vueMin.x].getMonstre()].m_lumiere.bleu=m_tableauDesLampes[i][j].bleu;
                    m_monstre[m_decor[1][i+vueMin.y][j+vueMin.x].getMonstre()].m_lumiere.hauteur=m_tableauDesLampes[i][j].hauteur;

                    if(configuration.Ombre)
                        m_monstre[m_decor[1][i+vueMin.y][j+vueMin.x].getMonstre()].m_lumiere.AjouterOmbre(lumiereMap.intensite/4,angleOmbreMap,lumiereMap.hauteur);
                }

                if(configuration.Ombre&&!((i+vueMin.y)==hero->m_personnage.getCoordonnee().y&&(j+vueMin.x)==hero->m_personnage.getCoordonnee().x))
                {
                    coordonnee coord1,coord2;

                    coord1.x=(int)(hero->m_personnage.getCoordonneePixel().x-hero->m_personnage.getCoordonneePixel().y-1*COTE_TILE+m_decor[0].size()*COTE_TILE)*2;
                    coord1.y=(int)(hero->m_personnage.getCoordonneePixel().x+hero->m_personnage.getCoordonneePixel().y);
                    coord2.x=(int)((j+vueMin.x)*COTE_TILE-(i+vueMin.y)*COTE_TILE-1*COTE_TILE+m_decor[0].size()*COTE_TILE)*2;
                    coord2.y=(int)((j+vueMin.x)*COTE_TILE+(i+vueMin.y)*COTE_TILE);

                    angle=atan(valeurAbsolue((float)coord2.y-(float)coord1.y)/valeurAbsolue((float)coord2.x-(float)coord1.x))*360/(2*M_PI)-90;

                    if(coord2.y>=coord1.y&&coord2.x>=coord1.x)
                        angle=180-angle;
                    else if(coord2.y>=coord1.y&&coord2.x<coord1.x)
                        angle+=180;
                    else if(coord2.y<coord1.y&&coord2.x<coord1.x)
                        angle=-angle;

                    m_tableauDesLampes[i][j].AjouterOmbre(lumiere.intensite/2,(int)angle,lumiere.hauteur);



                    /*if(m_decor[0][i+vueMin.y][j+vueMin.x].getMonstre()>=0&&m_decor[0][i+vueMin.y][j+vueMin.x].getMonstre()<m_monstre.size())
                    {
                        coord1.x=(int)(hero->m_personnage.getCoordonneePixel().x-hero->m_personnage.getCoordonneePixel().y-1*COTE_TILE+m_decor[0].size()*COTE_TILE)*2;
                        coord1.y=(int)(hero->m_personnage.getCoordonneePixel().x+hero->m_personnage.getCoordonneePixel().y);
                        coord1.x=(int)(m_monstre[m_decor[0][i+vueMin.y][j+vueMin.x].getMonstre()].getCoordonneePixel().x-m_monstre[m_decor[0][i+vueMin.y][j+vueMin.x].getMonstre()].getCoordonneePixel().y-1*COTE_TILE+m_decor[0].size()*COTE_TILE)*2;
                        coord1.y=(int)(m_monstre[m_decor[0][i+vueMin.y][j+vueMin.x].getMonstre()].getCoordonneePixel().x+m_monstre[m_decor[0][i+vueMin.y][j+vueMin.x].getMonstre()].getCoordonneePixel().y);

                        angle=atan(valeurAbsolue((float)coord2.y-(float)coord1.y)/valeurAbsolue((float)coord2.x-(float)coord1.x))*360/(2*M_PI)-90;

                        if(coord2.y>=coord1.y&&coord2.x>=coord1.x)
                            angle=180-angle;
                        else if(coord2.y>=coord1.y&&coord2.x<coord1.x)
                            angle+=180;
                        else if(coord2.y<coord1.y&&coord2.x<coord1.x)
                            angle=-angle;

                        m_monstre[m_decor[0][i+vueMin.y][j+vueMin.x].getMonstre()].m_lumiere.AjouterOmbre(lumiere.intensite/2,(int)angle,lumiere.hauteur);
                    }

                    if(m_decor[1][i+vueMin.y][j+vueMin.x].getMonstre()>=0&&m_decor[1][i+vueMin.y][j+vueMin.x].getMonstre()<m_monstre.size())
                    {
                        coord1.x=(int)(hero->m_personnage.getCoordonneePixel().x-hero->m_personnage.getCoordonneePixel().y-1*COTE_TILE+m_decor[0].size()*COTE_TILE)*2;
                        coord1.y=(int)(hero->m_personnage.getCoordonneePixel().x+hero->m_personnage.getCoordonneePixel().y);
                        coord1.x=(int)(m_monstre[m_decor[1][i+vueMin.y][j+vueMin.x].getMonstre()].getCoordonneePixel().x-m_monstre[m_decor[1][i+vueMin.y][j+vueMin.x].getMonstre()].getCoordonneePixel().y-1*COTE_TILE+m_decor[0].size()*COTE_TILE)*2;
                        coord1.y=(int)(m_monstre[m_decor[1][i+vueMin.y][j+vueMin.x].getMonstre()].getCoordonneePixel().x+m_monstre[m_decor[1][i+vueMin.y][j+vueMin.x].getMonstre()].getCoordonneePixel().y);

                        angle=atan(valeurAbsolue((float)coord2.y-(float)coord1.y)/valeurAbsolue((float)coord2.x-(float)coord1.x))*360/(2*M_PI)-90;

                        if(coord2.y>=coord1.y&&coord2.x>=coord1.x)
                            angle=180-angle;
                        else if(coord2.y>=coord1.y&&coord2.x<coord1.x)
                            angle+=180;
                        else if(coord2.y<coord1.y&&coord2.x<coord1.x)
                            angle=-angle;

                        //m_monstre[m_decor[1][i+vueMin.y][j+vueMin.x].getMonstre()].m_lumiere.AjouterOmbre(lumiere.intensite/2,(int)angle,lumiere.hauteur);
                    }*/
                }


            }
        }






        for(int couche=1;couche==1;couche++)
        {
            for(int j=lampesMin.y;j<lampesMax.y;++j)
            {
                for(int k=lampesMin.x;k<lampesMax.x;++k)
                {
                    bool ok=false;
                    if(m_decor[0][j][k].getMonstre()>=0&&m_decor[0][j][k].getMonstre()<m_monstre.size())// Je vérifie que le tile est initialisé ^^
                            if(m_monstre[m_decor[0][j][k].getMonstre()].getPorteeLumineuse().intensite>0)
                                ok=true;
                    if(m_decor[1][j][k].getMonstre()>=0&&m_decor[1][j][k].getMonstre()<m_monstre.size())// Je vérifie que le tile est initialisé ^^
                            if(m_monstre[m_decor[1][j][k].getMonstre()].getPorteeLumineuse().intensite>0)
                                ok=true;

                     for(int z=0;z<m_decor[1][j][k].getEvenement().size();z++)
                        if(m_decor[1][j][k].getEvenement()[z]>=0&&m_decor[1][j][k].getEvenement()[z]<m_evenement.size())// Je vérifie que le tile est initialisé ^^
                            if(m_evenement[m_decor[1][j][k].getEvenement()[z]].getType()==LUMIERE)
                                ok=true;
                    for(int z=0;z<m_decor[0][j][k].getEvenement().size();z++)
                        if(m_decor[0][j][k].getEvenement()[z]>=0&&m_decor[0][j][k].getEvenement()[z]<m_evenement.size())// Je vérifie que le tile est initialisé ^^
                            if(m_evenement[m_decor[0][j][k].getEvenement()[z]].getType()==LUMIERE)
                                ok=true;

                    if(m_decor[couche][j][k].getTileset()>=0&&m_decor[couche][j][k].getTileset()<m_tileset.size())
                        if(m_tileset[m_decor[couche][j][k].getTileset()].getLumiereDuTile(m_decor[couche][j][k].getTile()).intensite>0)// Je ragarde si le tile est une source de lumière
                            ok=true;
                    if(ok)
                    {
                        lumiereTile.intensite=0;
                        lumiereTile.rouge=0;
                        lumiereTile.vert=0;
                        lumiereTile.bleu=0;
                        lumiereTile.hauteur=50;

                        if(m_decor[couche][j][k].getTileset()>=0&&m_decor[couche][j][k].getTileset()<m_tileset.size())
                            if(m_tileset[m_decor[couche][j][k].getTileset()].getLumiereDuTile(m_decor[couche][j][k].getTile()).intensite>0)
                                lumiereTile=m_tileset[m_decor[couche][j][k].getTileset()].getLumiereDuTile(m_decor[couche][j][k].getTile());// Je dis que lumièreTiles à la valeur de la lumière de la source de lumière


                        for(int z=0;z<m_decor[0][j][k].getEvenement().size();z++)
                            if(m_decor[0][j][k].getEvenement()[z]>=0&&m_decor[0][j][k].getEvenement()[z]<m_evenement.size())// Je vérifie que le tile est initialisé ^^
                                if(m_evenement[m_decor[0][j][k].getEvenement()[z]].getType()==LUMIERE)
                                {
                                    lumiereTile.rouge=(lumiereTile.rouge*lumiereTile.intensite+m_evenement[m_decor[0][j][k].getEvenement()[z]].getInformation(0)*m_evenement[m_decor[0][j][k].getEvenement()[z]].getInformation(3))/(m_evenement[m_decor[0][j][k].getEvenement()[z]].getInformation(3)+lumiereTile.intensite);
                                    lumiereTile.vert=(lumiereTile.vert*lumiereTile.intensite+m_evenement[m_decor[0][j][k].getEvenement()[z]].getInformation(1)*m_evenement[m_decor[0][j][k].getEvenement()[z]].getInformation(3))/(m_evenement[m_decor[0][j][k].getEvenement()[z]].getInformation(3)+lumiereTile.intensite);
                                    lumiereTile.bleu=(lumiereTile.bleu*lumiereTile.intensite+m_evenement[m_decor[0][j][k].getEvenement()[z]].getInformation(2)*m_evenement[m_decor[0][j][k].getEvenement()[z]].getInformation(3))/(m_evenement[m_decor[0][j][k].getEvenement()[z]].getInformation(3)+lumiereTile.intensite);

                                    lumiereTile.hauteur=(lumiereTile.hauteur*lumiereTile.intensite+m_evenement[m_decor[0][j][k].getEvenement()[z]].getInformation(4)*m_evenement[m_decor[0][j][k].getEvenement()[z]].getInformation(3))/(m_evenement[m_decor[0][j][k].getEvenement()[z]].getInformation(3)+lumiereTile.intensite);

                                    lumiereTile.intensite+=m_evenement[m_decor[0][j][k].getEvenement()[z]].getInformation(3);
                                    if(lumiereTile.intensite>255)
                                        lumiereTile.intensite=255;
                                    if(lumiereTile.intensite<0)
                                        lumiereTile.intensite=0;
                                }
                        for(int z=0;z<m_decor[1][j][k].getEvenement().size();z++)
                            if(m_decor[1][j][k].getEvenement()[z]>=0&&m_decor[1][j][k].getEvenement()[z]<m_evenement.size())// Je vérifie que le tile est initialisé ^^
                                if(m_evenement[m_decor[1][j][k].getEvenement()[z]].getType()==LUMIERE)
                                {
                                    lumiereTile.rouge=(lumiereTile.rouge*lumiereTile.intensite+m_evenement[m_decor[1][j][k].getEvenement()[z]].getInformation(0)*m_evenement[m_decor[1][j][k].getEvenement()[z]].getInformation(3))/(m_evenement[m_decor[1][j][k].getEvenement()[z]].getInformation(3)+lumiereTile.intensite);
                                    lumiereTile.vert=(lumiereTile.vert*lumiereTile.intensite+m_evenement[m_decor[1][j][k].getEvenement()[z]].getInformation(1)*m_evenement[m_decor[1][j][k].getEvenement()[z]].getInformation(3))/(m_evenement[m_decor[1][j][k].getEvenement()[z]].getInformation(3)+lumiereTile.intensite);
                                    lumiereTile.bleu=(lumiereTile.bleu*lumiereTile.intensite+m_evenement[m_decor[1][j][k].getEvenement()[z]].getInformation(2)*m_evenement[m_decor[1][j][k].getEvenement()[z]].getInformation(3))/(m_evenement[m_decor[1][j][k].getEvenement()[z]].getInformation(3)+lumiereTile.intensite);

                                    lumiereTile.hauteur=(lumiereTile.hauteur*lumiereTile.intensite+m_evenement[m_decor[1][j][k].getEvenement()[z]].getInformation(4)*m_evenement[m_decor[1][j][k].getEvenement()[z]].getInformation(3))/(m_evenement[m_decor[1][j][k].getEvenement()[z]].getInformation(3)+lumiereTile.intensite);

                                    lumiereTile.intensite+=m_evenement[m_decor[1][j][k].getEvenement()[z]].getInformation(3);
                                    if(lumiereTile.intensite>255)
                                        lumiereTile.intensite=255;
                                    if(lumiereTile.intensite<0)
                                        lumiereTile.intensite=0;
                                }


                        //Maintenant, je vais calculer la lumière à ajouter à tout les tiles affiché à l'écran
                        for(int l=j-7;l<j+7;++l)
                            for(int m=k-7;m<k+7;++m)
                            if(l>=vueMin.y&&l<vueMax.y&&m>=vueMin.x&&m<vueMax.x)
                                {
                                    bool ombre=false;
                                    position.x=(m-l-1+m_decor[0].size())*64;
                                    position.y=(m+l)*32; // Conversion des coord iso en cartésienne
                                    const sf::FloatRect& ViewRect = camera->GetRect();
                                    if(m_decor[0][l][m].getTileset()>-1)
                                        positionPartieDecor=m_tileset[m_decor[0][l][m].getTileset()].getPositionDuTile(m_decor[0][l][m].getTile());
                                    if(m_decor[1][l][m].getTileset()>-1)
                                    {
                                        positionPartieDecor=m_tileset[m_decor[1][l][m].getTileset()].getPositionDuTile(m_decor[1][l][m].getTile());
                                        if(m_tileset[m_decor[1][l][m].getTileset()].getOmbreDuTile(m_decor[1][l][m].getTile())&&configuration.Ombre)
                                            ombre=true;
                                    }

                                    if(position.x+positionPartieDecor.h*2+positionPartieDecor.w>=ViewRect.Left&&position.x-positionPartieDecor.h*2-positionPartieDecor.w-64<ViewRect.Right&&position.y+positionPartieDecor.h+positionPartieDecor.w>=ViewRect.Top&&position.y-positionPartieDecor.h-32<ViewRect.Bottom+100&&ombre
                                    ||position.x+positionPartieDecor.w/configuration.zoom>=ViewRect.Left&&position.x-128/configuration.zoom<ViewRect.Right&&position.y+64+positionPartieDecor.h/configuration.zoom>=ViewRect.Top&&position.y-positionPartieDecor.h/configuration.zoom+64<ViewRect.Bottom)
                                  //  if(position.x>=ViewRect.Left-384&&position.x<ViewRect.Right+384&&position.y>=ViewRect.Top-384&&position.y<ViewRect.Bottom+384)// Je test si le tile va êtrz affiché à l'écran ou pas
                                    {

                                    //lumiere=m_lumiere;
                                    lumiere.intensite=0;
                                    lumiereTemp=0;
                                    // Je réduit la quantité de lumière reçue, en fonction de la distance, qui est calculée avec un simple pythagore ^^
                                    lumiereTemp=(float)lumiereTile.intensite-(float)(gpl::sqrt((k*COTE_TILE-m*COTE_TILE)*(k*COTE_TILE-m*COTE_TILE)+(j*COTE_TILE-l*COTE_TILE)*(j*COTE_TILE-l*COTE_TILE)))/4;
                                    lumiere.hauteur=lumiereTile.hauteur;

                                    if(lumiereTemp>0)
                                    lumiere.intensite=(int)lumiereTemp;
                                    else
                                    lumiere.intensite=0;

                                    //Je vais tester s'il y a un obstacle entre la source de lumière et le tile
                                    if(configuration.Lumiere==2)
                                    if(!(l==j&&m==k))
                                    if(lumiere.intensite>0)
                                    for(int o=1;o<10;o++)
                                    {
                                        Xtemp=((double)(k-m)*0.1)*(double)o;
                                        Ytemp=((double)(j-l)*0.1)*(double)o;

                                        distanceActuelle.x=m+(int)Xtemp;
                                        distanceActuelle.y=l+(int)Ytemp;

                                        if(distanceActuelle.x>0&&distanceActuelle.x<m_decor[0][0].size()&&distanceActuelle.y>0&&distanceActuelle.y<m_decor[0].size())
                                            if(m_decor[1][distanceActuelle.y][distanceActuelle.x].getTileset()>=0&&m_decor[1][distanceActuelle.y][distanceActuelle.x].getTileset()<m_tileset.size())
                                            if(m_tileset[m_decor[1][distanceActuelle.y][distanceActuelle.x].getTileset()].getLumiereDuTile(m_decor[1][distanceActuelle.y][distanceActuelle.x].getTile()).intensite<0&&m_tileset[m_decor[1][distanceActuelle.y][distanceActuelle.x].getTileset()].getLumiereDuTile(m_decor[1][distanceActuelle.y][distanceActuelle.x].getTile()).intensite<lumiereTemp)
                                                    {
                                                        bool erreur=false;
                                                        //if(m_decor[1][l][m].getTileset()>0)
                                                            if(m_tileset[m_decor[1][distanceActuelle.y][distanceActuelle.x].getTileset()].getOrientationDuTile(m_decor[1][distanceActuelle.y][distanceActuelle.x].getTile())=='y'||m_tileset[m_decor[1][distanceActuelle.y][distanceActuelle.x].getTileset()].getOrientationDuTile(m_decor[1][distanceActuelle.y][distanceActuelle.x].getTile())=='d')
                                                                //if(m_tileset[m_decor[1][l][m].getTileset()].getOrientationDuTile(m_decor[1][l][m].getTile())!='x')
                                                                    if(m==distanceActuelle.x&&m<k)
                                                                        erreur=true;


                                                       // if(m_decor[1][l][m].getTileset()>0)
                                                            if(m_tileset[m_decor[1][distanceActuelle.y][distanceActuelle.x].getTileset()].getOrientationDuTile(m_decor[1][distanceActuelle.y][distanceActuelle.x].getTile())=='x'||m_tileset[m_decor[1][distanceActuelle.y][distanceActuelle.x].getTileset()].getOrientationDuTile(m_decor[1][distanceActuelle.y][distanceActuelle.x].getTile())=='g')
                                                                //if(m_tileset[m_decor[1][l][m].getTileset()].getOrientationDuTile(m_decor[1][l][m].getTile())!='y')
                                                                    if(l==distanceActuelle.y&&l<j)
                                                                        erreur=true;

                                                        if(!erreur)
                                                        if(!(l==distanceActuelle.y&&m==distanceActuelle.x))
                                                            lumiereTemp=m_tileset[m_decor[1][distanceActuelle.y][distanceActuelle.x].getTileset()].getLumiereDuTile(m_decor[1][distanceActuelle.y][distanceActuelle.x].getTile()).intensite;
                                                        else
                                                        {
                                                            if(distanceActuelle.x>=k&&distanceActuelle.y>=j)
                                                               lumiereTemp=m_tileset[m_decor[1][distanceActuelle.y][distanceActuelle.x].getTileset()].getLumiereDuTile(m_decor[1][distanceActuelle.y][distanceActuelle.x].getTile()).intensite;

                                                            if(distanceActuelle.x>=k&&distanceActuelle.y<j)
                                                            {
                                                                if(m_tileset[m_decor[1][distanceActuelle.y][distanceActuelle.x].getTileset()].getOrientationDuTile(m_decor[1][distanceActuelle.y][distanceActuelle.x].getTile())=='y'||m_tileset[m_decor[1][distanceActuelle.y][distanceActuelle.x].getTileset()].getOrientationDuTile(m_decor[1][distanceActuelle.y][distanceActuelle.x].getTile())=='d')
                                                                        lumiereTemp=m_tileset[m_decor[1][distanceActuelle.y][distanceActuelle.x].getTileset()].getLumiereDuTile(m_decor[1][distanceActuelle.y][distanceActuelle.x].getTile()).intensite;
                                                            }
                                                            if(distanceActuelle.x<k&&distanceActuelle.y>=j)
                                                            {

                                                                if(m_tileset[m_decor[1][distanceActuelle.y][distanceActuelle.x].getTileset()].getOrientationDuTile(m_decor[1][distanceActuelle.y][distanceActuelle.x].getTile())=='x'||m_tileset[m_decor[1][distanceActuelle.y][distanceActuelle.x].getTileset()].getOrientationDuTile(m_decor[1][distanceActuelle.y][distanceActuelle.x].getTile())=='g')
                                                                    lumiereTemp=m_tileset[m_decor[1][distanceActuelle.y][distanceActuelle.x].getTileset()].getLumiereDuTile(m_decor[1][distanceActuelle.y][distanceActuelle.x].getTile()).intensite;
                                                            }
                                                        }
                                                    }

                                    }
                                    // SI oui, je retire de la lumière au tile
                                    if((int)lumiereTemp<0)
                                    lumiere.intensite+=(int)lumiereTemp;

                                    lumiere.rouge=lumiereTile.rouge;
                                    lumiere.vert=lumiereTile.vert;
                                    lumiere.bleu=lumiereTile.bleu;

                                    if(lumiere.intensite>255)
                                        lumiere.intensite=255;
                                    if(lumiere.intensite<0)
                                        lumiere.intensite=0;

                                    if(lumiere.intensite>0)
                                    {
                                        // Je calcule la couleur de la lumière, par rapport à l'ancienne lumière et à la nouvelle, pour faire un fondu
                                        m_tableauDesLampes[l-vueMin.y][m-vueMin.x].rouge=(lumiere.rouge*lumiere.intensite+m_tableauDesLampes[l-vueMin.y][m-vueMin.x].rouge*m_tableauDesLampes[l-vueMin.y][m-vueMin.x].intensite)/(m_tableauDesLampes[l-vueMin.y][m-vueMin.x].intensite+lumiere.intensite);
                                            m_tableauDesLampes[l-vueMin.y][m-vueMin.x].vert=(lumiere.vert*lumiere.intensite+m_tableauDesLampes[l-vueMin.y][m-vueMin.x].vert*m_tableauDesLampes[l-vueMin.y][m-vueMin.x].intensite)/(m_tableauDesLampes[l-vueMin.y][m-vueMin.x].intensite+lumiere.intensite);
                                            m_tableauDesLampes[l-vueMin.y][m-vueMin.x].bleu=(lumiere.bleu*lumiere.intensite+m_tableauDesLampes[l-vueMin.y][m-vueMin.x].bleu*m_tableauDesLampes[l-vueMin.y][m-vueMin.x].intensite)/(m_tableauDesLampes[l-vueMin.y][m-vueMin.x].intensite+lumiere.intensite);

                                        if(m_tableauDesLampes[l-vueMin.y][m-vueMin.x].rouge>255)
                                            m_tableauDesLampes[l-vueMin.y][m-vueMin.x].rouge=255;
                                        if(m_tableauDesLampes[l-vueMin.y][m-vueMin.x].vert>255)
                                            m_tableauDesLampes[l-vueMin.y][m-vueMin.x].vert=255;
                                        if(m_tableauDesLampes[l-vueMin.y][m-vueMin.x].bleu>255)
                                            m_tableauDesLampes[l-vueMin.y][m-vueMin.x].bleu=255;

                                        m_tableauDesLampes[l-vueMin.y][m-vueMin.x].intensite+=lumiere.intensite;

                                        if(m_tableauDesLampes[l-vueMin.y][m-vueMin.x].intensite>255)
                                            m_tableauDesLampes[l-vueMin.y][m-vueMin.x].intensite=255;
                                        if(m_tableauDesLampes[l-vueMin.y][m-vueMin.x].intensite<0)
                                            m_tableauDesLampes[l-vueMin.y][m-vueMin.x].intensite=0;

                                        // Je calcule les ombres
                                        if(!(l==j&&m==k))
                                        if(configuration.Ombre)
                                        {
                                            coordonnee coord1,coord2;

                                            coord1.x=(k-j-1+m_decor[0].size())*64;
                                            coord1.y=(k+j)*32;

                                            coord2.x=(m-l-1+m_decor[0].size())*64;
                                            coord2.y=(m+l)*32;

                                            // L'angle de l'ombre, merci Mme la trigonométrie :D
                                            angle=atan(valeurAbsolue((float)coord2.y-(float)coord1.y)/valeurAbsolue((float)coord2.x-(float)coord1.x))*360/(2*M_PI)-90;

                                            if(coord2.y>=coord1.y&&coord2.x>=coord1.x)
                                                angle=180-angle;
                                            else if(coord2.y>=coord1.y&&coord2.x<coord1.x)
                                                angle+=180;
                                            else if(coord2.y<coord1.y&&coord2.x<coord1.x)
                                                angle=-angle;

                                            // Et j'ajoute cette ombre
                                            m_tableauDesLampes[l-vueMin.y][m-vueMin.x].AjouterOmbre(lumiere.intensite,(int)angle,lumiere.hauteur);

                                            if(m_decor[0][l][m].getMonstre()>=0&&m_decor[0][l][m].getMonstre()<m_monstre.size())
                                            {
                                                coord1.x=(int)(k*COTE_TILE-j*COTE_TILE-1+m_decor[0].size())*64;
                                                coord1.y=(int)(k*COTE_TILE+j*COTE_TILE)*32;

                                                coord2.x=((int)m_monstre[m_decor[0][l][m].getMonstre()].getCoordonneePixel().x-(int)m_monstre[m_decor[0][l][m].getMonstre()].getCoordonneePixel().y-1+m_decor[0].size())*64;
                                                coord2.y=((int)m_monstre[m_decor[0][l][m].getMonstre()].getCoordonneePixel().x+(int)m_monstre[m_decor[0][l][m].getMonstre()].getCoordonneePixel().y)*32;

                                                angle=atan(valeurAbsolue((float)coord2.y-(float)coord1.y)/valeurAbsolue((float)coord2.x-(float)coord1.x))*360/(2*M_PI)-90;

                                                if(coord2.y>=coord1.y&&coord2.x>=coord1.x)
                                                    angle=180-angle;
                                                else if(coord2.y>=coord1.y&&coord2.x<coord1.x)
                                                    angle+=180;
                                                else if(coord2.y<coord1.y&&coord2.x<coord1.x)
                                                    angle=-angle;

                                                lumiereTemp=lumiere.intensite/2;
                                                if(lumiereTemp<0)
                                                    lumiereTemp=0;
                                               m_monstre[m_decor[0][l][m].getMonstre()].m_lumiere.AjouterOmbre((int)lumiereTemp,(int)angle,lumiere.hauteur);
                                            }
                                            if(m_decor[1][l][m].getMonstre()>=0&&m_decor[1][l][m].getMonstre()<m_monstre.size())
                                            {
                                                coord1.x=(int)(k*COTE_TILE-j*COTE_TILE-1+m_decor[0].size())*64;
                                                coord1.y=(int)(k*COTE_TILE+j*COTE_TILE)*32;

                                                coord2.x=((int)m_monstre[m_decor[1][l][m].getMonstre()].getCoordonneePixel().x-(int)m_monstre[m_decor[1][l][m].getMonstre()].getCoordonneePixel().y-1+m_decor[0].size())*64;
                                                coord2.y=((int)m_monstre[m_decor[1][l][m].getMonstre()].getCoordonneePixel().x+(int)m_monstre[m_decor[1][l][m].getMonstre()].getCoordonneePixel().y)*32;

                                                angle=atan(valeurAbsolue((float)coord2.y-(float)coord1.y)/valeurAbsolue((float)coord2.x-(float)coord1.x))*360/(2*M_PI)-90;

                                                if(coord2.y>=coord1.y&&coord2.x>=coord1.x)
                                                    angle=180-angle;
                                                else if(coord2.y>=coord1.y&&coord2.x<coord1.x)
                                                    angle+=180;
                                                else if(coord2.y<coord1.y&&coord2.x<coord1.x)
                                                    angle=-angle;

                                                lumiereTemp=lumiere.intensite/2;
                                                if(lumiereTemp<0)
                                                    lumiereTemp=0;
                                               m_monstre[m_decor[1][l][m].getMonstre()].m_lumiere.AjouterOmbre((int)lumiereTemp,(int)angle,lumiere.hauteur);
                                            }
                                        }

                                        if(m_decor[0][l][m].getMonstre()>=0&&m_decor[0][l][m].getMonstre()<m_monstre.size())
                                        {
                                            m_monstre[m_decor[0][l][m].getMonstre()].m_lumiere.intensite=m_tableauDesLampes[l-vueMin.y][m-vueMin.x].intensite;
                                            m_monstre[m_decor[0][l][m].getMonstre()].m_lumiere.rouge=m_tableauDesLampes[l-vueMin.y][m-vueMin.x].rouge;
                                            m_monstre[m_decor[0][l][m].getMonstre()].m_lumiere.vert=m_tableauDesLampes[l-vueMin.y][m-vueMin.x].vert;
                                            m_monstre[m_decor[0][l][m].getMonstre()].m_lumiere.bleu=m_tableauDesLampes[l-vueMin.y][m-vueMin.x].bleu;
                                            m_monstre[m_decor[0][l][m].getMonstre()].m_lumiere.hauteur=m_tableauDesLampes[l-vueMin.y][m-vueMin.x].hauteur;
                                        }

                                        if(m_decor[1][l][m].getMonstre()>=0&&m_decor[1][l][m].getMonstre()<m_monstre.size())
                                        {
                                            m_monstre[m_decor[1][l][m].getMonstre()].m_lumiere.intensite=m_tableauDesLampes[l-vueMin.y][m-vueMin.x].intensite;
                                            m_monstre[m_decor[1][l][m].getMonstre()].m_lumiere.rouge=m_tableauDesLampes[l-vueMin.y][m-vueMin.x].rouge;
                                            m_monstre[m_decor[1][l][m].getMonstre()].m_lumiere.vert=m_tableauDesLampes[l-vueMin.y][m-vueMin.x].vert;
                                            m_monstre[m_decor[1][l][m].getMonstre()].m_lumiere.bleu=m_tableauDesLampes[l-vueMin.y][m-vueMin.x].bleu;
                                            m_monstre[m_decor[1][l][m].getMonstre()].m_lumiere.hauteur=m_tableauDesLampes[l-vueMin.y][m-vueMin.x].hauteur;
                                        }


                                        // J'applique la même chose au héro
                                        if(l==hero->m_personnage.getCoordonnee().y&&m==hero->m_personnage.getCoordonnee().x)
                                        {
                                           hero->m_personnage.m_lumiere.rouge=(lumiere.rouge*lumiere.intensite+hero->m_personnage.m_lumiere.rouge*hero->m_personnage.m_lumiere.intensite)/(hero->m_personnage.m_lumiere.intensite+lumiere.intensite);
                                           hero->m_personnage.m_lumiere.vert=(lumiere.vert*lumiere.intensite+hero->m_personnage.m_lumiere.vert*hero->m_personnage.m_lumiere.intensite)/(hero->m_personnage.m_lumiere.intensite+lumiere.intensite);
                                           hero->m_personnage.m_lumiere.bleu=(lumiere.bleu*lumiere.intensite+hero->m_personnage.m_lumiere.bleu*hero->m_personnage.m_lumiere.intensite)/(hero->m_personnage.m_lumiere.intensite+lumiere.intensite);

                                            if(hero->m_personnage.m_lumiere.rouge>255)
                                               hero->m_personnage.m_lumiere.rouge=255;
                                            if(hero->m_personnage.m_lumiere.vert>255)
                                               hero->m_personnage.m_lumiere.vert=255;
                                            if(hero->m_personnage.m_lumiere.bleu>255)
                                               hero->m_personnage.m_lumiere.bleu=255;

                                            if(configuration.Ombre&&!(j==l&&k==m))
                                            {
                                                coordonnee coord1,coord2;

                                                coord1.x=(int)(k*COTE_TILE-j*COTE_TILE-1+m_decor[0].size())*64;
                                                coord1.y=(int)(k*COTE_TILE+j*COTE_TILE)*32;

                                                coord2.x=((int)hero->m_personnage.getCoordonneePixel().x-(int)hero->m_personnage.getCoordonneePixel().y-1+m_decor[0].size())*64;
                                                coord2.y=((int)hero->m_personnage.getCoordonneePixel().x+(int)hero->m_personnage.getCoordonneePixel().y)*32;

                                                angle=atan(valeurAbsolue((float)coord2.y-(float)coord1.y)/valeurAbsolue((float)coord2.x-(float)coord1.x))*360/(2*M_PI)-90;

                                                if(coord2.y>=coord1.y&&coord2.x>=coord1.x)
                                                    angle=180-angle;
                                                else if(coord2.y>=coord1.y&&coord2.x<coord1.x)
                                                    angle+=180;
                                                else if(coord2.y<coord1.y&&coord2.x<coord1.x)
                                                    angle=-angle;

                                                lumiereTemp=lumiere.intensite/2;
                                                if(lumiereTemp<0)
                                                    lumiereTemp=0;
                                               hero->m_personnage.m_lumiere.AjouterOmbre((int)lumiereTemp,(int)angle,lumiere.hauteur);
                                            }

                                            if(lumiere.intensite>0)
                                            {
                                                hero->m_personnage.m_lumiere.intensite+=lumiere.intensite;

                                                if(hero->m_personnage.m_lumiere.intensite>255)
                                                   hero->m_personnage.m_lumiere.intensite=255;
                                                if(hero->m_personnage.m_lumiere.intensite<0)
                                                   hero->m_personnage.m_lumiere.intensite=0;
                                            }
                                        }
                                    }


                                        if(m_decor[0][j][k].getMonstre()>=0&&m_decor[0][j][k].getMonstre()<m_monstre.size())
                                        //if(m_monstre[m_decor[0][j][k].getMonstre()].getCaracteristique().vie>0)
                                        if(m_monstre[m_decor[0][j][k].getMonstre()].getPorteeLumineuse().intensite>0)
                                        {
                                            //lumiere=m_lumiere;
                                            lumiere.intensite=m_monstre[m_decor[0][j][k].getMonstre()].getPorteeLumineuse().intensite;
                                            lumiereTemp=0;

                                            if(lumiere.intensite>255)
                                                lumiere.intensite=255;

                                            // Je réduit la quantité de lumière reçue, en fonction de la distance, qui est calculée avec un simple pythagore ^^
                                           // lumiereTemp=(float)lumiereTile.intensite-(float)(gpl::sqrt((k*COTE_TILE-m*COTE_TILE)*(k*COTE_TILE-m*COTE_TILE)+(j*COTE_TILE-l*COTE_TILE)*(j*COTE_TILE-l*COTE_TILE)))/5;
                                            lumiereTemp=(float)lumiere.intensite-((((float)gpl::sqrt((m_monstre[m_decor[0][j][k].getMonstre()].getCoordonneePixel().x-((float)m)*COTE_TILE)*(m_monstre[m_decor[0][j][k].getMonstre()].getCoordonneePixel().x-((float)m)*COTE_TILE)+(m_monstre[m_decor[0][j][k].getMonstre()].getCoordonneePixel().y-((float)l)*COTE_TILE)*(m_monstre[m_decor[0][j][k].getMonstre()].getCoordonneePixel().y-((float)l)*COTE_TILE))))*DIVISEUR_COTE_TILE)*60;
                                            lumiere.hauteur=m_monstre[m_decor[0][j][k].getMonstre()].getPorteeLumineuse().hauteur;

                                            if(lumiereTemp>0)
                                            lumiere.intensite=(int)lumiereTemp;
                                            else
                                            lumiere.intensite=0;


                                            //Je vais tester s'il y a un obstacle entre la source de lumière et le tile
                                            if(configuration.Lumiere==2)
                                            if(!(l==j&&m==k))
                                            if(lumiere.intensite>0)
                                            for(int o=1;o<10;++o)
                                            {
                                                Xtemp=((double)(k-m)*0.1)*(double)o;
                                                Ytemp=((double)(j-l)*0.1)*(double)o;

                                                distanceActuelle.x=m+(int)Xtemp;
                                                distanceActuelle.y=l+(int)Ytemp;

                                                if(distanceActuelle.x>0&&distanceActuelle.x<m_decor[0][0].size()&&distanceActuelle.y>0&&distanceActuelle.y<m_decor[0].size())
                                                    if(m_decor[1][distanceActuelle.y][distanceActuelle.x].getTileset()>=0&&m_decor[1][distanceActuelle.y][distanceActuelle.x].getTileset()<m_tileset.size())
                                                    if(m_tileset[m_decor[1][distanceActuelle.y][distanceActuelle.x].getTileset()].getLumiereDuTile(m_decor[1][distanceActuelle.y][distanceActuelle.x].getTile()).intensite<0&&m_tileset[m_decor[1][distanceActuelle.y][distanceActuelle.x].getTileset()].getLumiereDuTile(m_decor[1][distanceActuelle.y][distanceActuelle.x].getTile()).intensite<lumiereTemp)
                                                            {
                                                                bool erreur=false;
                                                                //if(m_decor[1][l][m].getTileset()>0)
                                                                    if(m_tileset[m_decor[1][distanceActuelle.y][distanceActuelle.x].getTileset()].getOrientationDuTile(m_decor[1][distanceActuelle.y][distanceActuelle.x].getTile())=='y'||m_tileset[m_decor[1][distanceActuelle.y][distanceActuelle.x].getTileset()].getOrientationDuTile(m_decor[1][distanceActuelle.y][distanceActuelle.x].getTile())=='d')
                                                                        //if(m_tileset[m_decor[1][l][m].getTileset()].getOrientationDuTile(m_decor[1][l][m].getTile())!='x')
                                                                            if(m==distanceActuelle.x&&m<k)
                                                                                erreur=true;


                                                               // if(m_decor[1][l][m].getTileset()>0)
                                                                    if(m_tileset[m_decor[1][distanceActuelle.y][distanceActuelle.x].getTileset()].getOrientationDuTile(m_decor[1][distanceActuelle.y][distanceActuelle.x].getTile())=='x'||m_tileset[m_decor[1][distanceActuelle.y][distanceActuelle.x].getTileset()].getOrientationDuTile(m_decor[1][distanceActuelle.y][distanceActuelle.x].getTile())=='g')
                                                                        //if(m_tileset[m_decor[1][l][m].getTileset()].getOrientationDuTile(m_decor[1][l][m].getTile())!='y')
                                                                            if(l==distanceActuelle.y&&l<j)
                                                                                erreur=true;

                                                                if(!erreur)
                                                                if(!(l==distanceActuelle.y&&m==distanceActuelle.x))
                                                                    lumiereTemp=m_tileset[m_decor[1][distanceActuelle.y][distanceActuelle.x].getTileset()].getLumiereDuTile(m_decor[1][distanceActuelle.y][distanceActuelle.x].getTile()).intensite;
                                                                else
                                                                {
                                                                    if(distanceActuelle.x>=k&&distanceActuelle.y>=j)
                                                                       lumiereTemp=m_tileset[m_decor[1][distanceActuelle.y][distanceActuelle.x].getTileset()].getLumiereDuTile(m_decor[1][distanceActuelle.y][distanceActuelle.x].getTile()).intensite;

                                                                    if(distanceActuelle.x>=k&&distanceActuelle.y<j)
                                                                    {
                                                                        if(m_tileset[m_decor[1][distanceActuelle.y][distanceActuelle.x].getTileset()].getOrientationDuTile(m_decor[1][distanceActuelle.y][distanceActuelle.x].getTile())=='y'||m_tileset[m_decor[1][distanceActuelle.y][distanceActuelle.x].getTileset()].getOrientationDuTile(m_decor[1][distanceActuelle.y][distanceActuelle.x].getTile())=='d')
                                                                                lumiereTemp=m_tileset[m_decor[1][distanceActuelle.y][distanceActuelle.x].getTileset()].getLumiereDuTile(m_decor[1][distanceActuelle.y][distanceActuelle.x].getTile()).intensite;
                                                                    }
                                                                    if(distanceActuelle.x<k&&distanceActuelle.y>=j)
                                                                    {

                                                                        if(m_tileset[m_decor[1][distanceActuelle.y][distanceActuelle.x].getTileset()].getOrientationDuTile(m_decor[1][distanceActuelle.y][distanceActuelle.x].getTile())=='x'||m_tileset[m_decor[1][distanceActuelle.y][distanceActuelle.x].getTileset()].getOrientationDuTile(m_decor[1][distanceActuelle.y][distanceActuelle.x].getTile())=='g')
                                                                            lumiereTemp=m_tileset[m_decor[1][distanceActuelle.y][distanceActuelle.x].getTileset()].getLumiereDuTile(m_decor[1][distanceActuelle.y][distanceActuelle.x].getTile()).intensite;
                                                                    }
                                                                }
                                                            }

                                            }
                                            // SI oui, je retire de la lumière au tile
                                            if((int)lumiereTemp<0)
                                            lumiere.intensite+=(int)lumiereTemp;


                                            if(lumiere.intensite>0)
                                            {

                                                lumiere.rouge=m_monstre[m_decor[0][j][k].getMonstre()].getPorteeLumineuse().rouge;
                                                lumiere.vert=m_monstre[m_decor[0][j][k].getMonstre()].getPorteeLumineuse().vert;
                                                lumiere.bleu=m_monstre[m_decor[0][j][k].getMonstre()].getPorteeLumineuse().bleu;

                                                lumiereTemp=m_monstre[m_decor[0][j][k].getMonstre()].getPorteeLumineuse().intensite;

                                                if(lumiereTemp>255)
                                                    lumiereTemp=255;

                                                    // Je calcule la couleur de la lumière, par rapport à l'ancienne lumière et à la nouvelle, pour faire un fondu
                                                m_tableauDesLampes[l-vueMin.y][m-vueMin.x].rouge=(int)(lumiere.rouge*lumiere.intensite+m_tableauDesLampes[l-vueMin.y][m-vueMin.x].rouge*(int)lumiereTemp)/((int)lumiereTemp+lumiere.intensite);
                                                m_tableauDesLampes[l-vueMin.y][m-vueMin.x].vert=(int)(lumiere.vert*lumiere.intensite+m_tableauDesLampes[l-vueMin.y][m-vueMin.x].vert*(int)lumiereTemp)/((int)lumiereTemp+lumiere.intensite);
                                                m_tableauDesLampes[l-vueMin.y][m-vueMin.x].bleu=(int)(lumiere.bleu*lumiere.intensite+m_tableauDesLampes[l-vueMin.y][m-vueMin.x].bleu*(int)lumiereTemp)/((int)lumiereTemp+lumiere.intensite);

                                                if(m_tableauDesLampes[l-vueMin.y][m-vueMin.x].rouge>255)
                                                    m_tableauDesLampes[l-vueMin.y][m-vueMin.x].rouge=255;
                                                if(m_tableauDesLampes[l-vueMin.y][m-vueMin.x].vert>255)
                                                    m_tableauDesLampes[l-vueMin.y][m-vueMin.x].vert=255;
                                                if(m_tableauDesLampes[l-vueMin.y][m-vueMin.x].bleu>255)
                                                    m_tableauDesLampes[l-vueMin.y][m-vueMin.x].bleu=255;

                                                m_tableauDesLampes[l-vueMin.y][m-vueMin.x].intensite+=lumiere.intensite;

                                                if(m_tableauDesLampes[l-vueMin.y][m-vueMin.x].intensite>255)
                                                    m_tableauDesLampes[l-vueMin.y][m-vueMin.x].intensite=255;
                                                if(m_tableauDesLampes[l-vueMin.y][m-vueMin.x].intensite<0)
                                                    m_tableauDesLampes[l-vueMin.y][m-vueMin.x].intensite=0;
                                            }
                                        }

                                        if(m_decor[1][j][k].getMonstre()>=0&&m_decor[1][j][k].getMonstre()<m_monstre.size())
                                       // if(m_monstre[m_decor[1][j][k].getMonstre()].getCaracteristique().vie>0)
                                        if(m_monstre[m_decor[1][j][k].getMonstre()].getPorteeLumineuse().intensite>0)
                                        {
                                            //lumiere=m_lumiere;
                                            lumiere.intensite=m_monstre[m_decor[1][j][k].getMonstre()].getPorteeLumineuse().intensite;
                                            if(lumiere.intensite>255)
                                                lumiere.intensite=255;
                                            lumiereTemp=0;
                                            // Je réduit la quantité de lumière reçue, en fonction de la distance, qui est calculée avec un simple pythagore ^^
                                            lumiereTemp=lumiere.intensite-((((float)gpl::sqrt((m_monstre[m_decor[1][j][k].getMonstre()].getCoordonneePixel().x-(m)*COTE_TILE)*(m_monstre[m_decor[1][j][k].getMonstre()].getCoordonneePixel().x-(m)*COTE_TILE)+(m_monstre[m_decor[1][j][k].getMonstre()].getCoordonneePixel().y-(l)*COTE_TILE)*(m_monstre[m_decor[1][j][k].getMonstre()].getCoordonneePixel().y-(l)*COTE_TILE))))*DIVISEUR_COTE_TILE)*60;
                                            lumiere.hauteur=m_monstre[m_decor[1][j][k].getMonstre()].getPorteeLumineuse().hauteur;

                                            if(lumiereTemp>0)
                                            lumiere.intensite=(int)lumiereTemp;
                                            else
                                            lumiere.intensite=0;



                                            //Je vais tester s'il y a un obstacle entre la source de lumière et le tile
                                            if(configuration.Lumiere==2)
                                            if(!(l==j&&m==k))
                                            if(lumiere.intensite>0)
                                            for(int o=1;o<10;++o)
                                            {
                                                Xtemp=((double)(k-m)*0.1)*(double)o;
                                                Ytemp=((double)(j-l)*0.1)*(double)o;

                                                distanceActuelle.x=m+(int)Xtemp;
                                                distanceActuelle.y=l+(int)Ytemp;

                                                if(distanceActuelle.x>0&&distanceActuelle.x<m_decor[0][0].size()&&distanceActuelle.y>0&&distanceActuelle.y<m_decor[0].size())
                                                    if(m_decor[1][distanceActuelle.y][distanceActuelle.x].getTileset()>=0&&m_decor[1][distanceActuelle.y][distanceActuelle.x].getTileset()<m_tileset.size())
                                                    if(m_tileset[m_decor[1][distanceActuelle.y][distanceActuelle.x].getTileset()].getLumiereDuTile(m_decor[1][distanceActuelle.y][distanceActuelle.x].getTile()).intensite<0&&m_tileset[m_decor[1][distanceActuelle.y][distanceActuelle.x].getTileset()].getLumiereDuTile(m_decor[1][distanceActuelle.y][distanceActuelle.x].getTile()).intensite<lumiereTemp)
                                                            {
                                                                bool erreur=false;
                                                                //if(m_decor[1][l][m].getTileset()>0)
                                                                    if(m_tileset[m_decor[1][distanceActuelle.y][distanceActuelle.x].getTileset()].getOrientationDuTile(m_decor[1][distanceActuelle.y][distanceActuelle.x].getTile())=='y'||m_tileset[m_decor[1][distanceActuelle.y][distanceActuelle.x].getTileset()].getOrientationDuTile(m_decor[1][distanceActuelle.y][distanceActuelle.x].getTile())=='d')
                                                                        //if(m_tileset[m_decor[1][l][m].getTileset()].getOrientationDuTile(m_decor[1][l][m].getTile())!='x')
                                                                            if(m==distanceActuelle.x&&m<k)
                                                                                erreur=true;


                                                               // if(m_decor[1][l][m].getTileset()>0)
                                                                    if(m_tileset[m_decor[1][distanceActuelle.y][distanceActuelle.x].getTileset()].getOrientationDuTile(m_decor[1][distanceActuelle.y][distanceActuelle.x].getTile())=='x'||m_tileset[m_decor[1][distanceActuelle.y][distanceActuelle.x].getTileset()].getOrientationDuTile(m_decor[1][distanceActuelle.y][distanceActuelle.x].getTile())=='g')
                                                                        //if(m_tileset[m_decor[1][l][m].getTileset()].getOrientationDuTile(m_decor[1][l][m].getTile())!='y')
                                                                            if(l==distanceActuelle.y&&l<j)
                                                                                erreur=true;

                                                                if(!erreur)
                                                                if(!(l==distanceActuelle.y&&m==distanceActuelle.x))
                                                                    lumiereTemp=m_tileset[m_decor[1][distanceActuelle.y][distanceActuelle.x].getTileset()].getLumiereDuTile(m_decor[1][distanceActuelle.y][distanceActuelle.x].getTile()).intensite;
                                                                else
                                                                {
                                                                    if(distanceActuelle.x>=k&&distanceActuelle.y>=j)
                                                                       lumiereTemp=m_tileset[m_decor[1][distanceActuelle.y][distanceActuelle.x].getTileset()].getLumiereDuTile(m_decor[1][distanceActuelle.y][distanceActuelle.x].getTile()).intensite;

                                                                    if(distanceActuelle.x>=k&&distanceActuelle.y<j)
                                                                    {
                                                                        if(m_tileset[m_decor[1][distanceActuelle.y][distanceActuelle.x].getTileset()].getOrientationDuTile(m_decor[1][distanceActuelle.y][distanceActuelle.x].getTile())=='y'||m_tileset[m_decor[1][distanceActuelle.y][distanceActuelle.x].getTileset()].getOrientationDuTile(m_decor[1][distanceActuelle.y][distanceActuelle.x].getTile())=='d')
                                                                                lumiereTemp=m_tileset[m_decor[1][distanceActuelle.y][distanceActuelle.x].getTileset()].getLumiereDuTile(m_decor[1][distanceActuelle.y][distanceActuelle.x].getTile()).intensite;
                                                                    }
                                                                    if(distanceActuelle.x<k&&distanceActuelle.y>=j)
                                                                    {

                                                                        if(m_tileset[m_decor[1][distanceActuelle.y][distanceActuelle.x].getTileset()].getOrientationDuTile(m_decor[1][distanceActuelle.y][distanceActuelle.x].getTile())=='x'||m_tileset[m_decor[1][distanceActuelle.y][distanceActuelle.x].getTileset()].getOrientationDuTile(m_decor[1][distanceActuelle.y][distanceActuelle.x].getTile())=='g')
                                                                            lumiereTemp=m_tileset[m_decor[1][distanceActuelle.y][distanceActuelle.x].getTileset()].getLumiereDuTile(m_decor[1][distanceActuelle.y][distanceActuelle.x].getTile()).intensite;
                                                                    }
                                                                }
                                                            }

                                            }
                                            // SI oui, je retire de la lumière au tile
                                            if((int)lumiereTemp<0)
                                            lumiere.intensite+=(int)lumiereTemp;


                                            if(lumiere.intensite>0)
                                            {
                                                lumiere.rouge=m_monstre[m_decor[1][j][k].getMonstre()].getPorteeLumineuse().rouge;
                                                lumiere.vert=m_monstre[m_decor[1][j][k].getMonstre()].getPorteeLumineuse().vert;
                                                lumiere.bleu=m_monstre[m_decor[1][j][k].getMonstre()].getPorteeLumineuse().bleu;
                                                    // Je calcule la couleur de la lumière, par rapport à l'ancienne lumière et à la nouvelle, pour faire un fondu
                                                lumiereTemp=m_monstre[m_decor[1][j][k].getMonstre()].getPorteeLumineuse().intensite;

                                                if(lumiereTemp>255)
                                                    lumiereTemp=255;

                                                    // Je calcule la couleur de la lumière, par rapport à l'ancienne lumière et à la nouvelle, pour faire un fondu
                                                m_tableauDesLampes[l-vueMin.y][m-vueMin.x].rouge=(int)(lumiere.rouge*lumiere.intensite+m_tableauDesLampes[l-vueMin.y][m-vueMin.x].rouge*(int)lumiereTemp)/((int)lumiereTemp+lumiere.intensite);
                                                m_tableauDesLampes[l-vueMin.y][m-vueMin.x].vert=(int)(lumiere.vert*lumiere.intensite+m_tableauDesLampes[l-vueMin.y][m-vueMin.x].vert*(int)lumiereTemp)/((int)lumiereTemp+lumiere.intensite);
                                                m_tableauDesLampes[l-vueMin.y][m-vueMin.x].bleu=(int)(lumiere.bleu*lumiere.intensite+m_tableauDesLampes[l-vueMin.y][m-vueMin.x].bleu*(int)lumiereTemp)/((int)lumiereTemp+lumiere.intensite);


                                                if(m_tableauDesLampes[l-vueMin.y][m-vueMin.x].rouge>255)
                                                    m_tableauDesLampes[l-vueMin.y][m-vueMin.x].rouge=255;
                                                if(m_tableauDesLampes[l-vueMin.y][m-vueMin.x].vert>255)
                                                    m_tableauDesLampes[l-vueMin.y][m-vueMin.x].vert=255;
                                                if(m_tableauDesLampes[l-vueMin.y][m-vueMin.x].bleu>255)
                                                    m_tableauDesLampes[l-vueMin.y][m-vueMin.x].bleu=255;

                                                m_tableauDesLampes[l-vueMin.y][m-vueMin.x].intensite+=lumiere.intensite;

                                                if(m_tableauDesLampes[l-vueMin.y][m-vueMin.x].intensite>255)
                                                    m_tableauDesLampes[l-vueMin.y][m-vueMin.x].intensite=255;
                                                if(m_tableauDesLampes[l-vueMin.y][m-vueMin.x].intensite<0)
                                                    m_tableauDesLampes[l-vueMin.y][m-vueMin.x].intensite=0;
                                            }
                                        }
                                    }
                                }
                    }
                }
            }
        }

        // Bah voilà, j'espère que j'ai été clair ^^'
        //for(int z=0;z<configuration.anticrenelage_lumieres;z++)

}

void Map::Afficher(RenderWindow* ecran,View *camera,int type,Hero *hero,coordonnee positionSouris,bool alt,float alpha)
{
    coordonnee positionPartieDecor,vueMin,vueMax,positionHero;

	Sprite Sprite;
	String texte;

	positionPartieDecor.x=0;
	positionPartieDecor.y=0;
	positionPartieDecor.w=128;
	positionPartieDecor.h=64;

	for(int i=0;i<m_tileset.size();i++)
		m_tileset[i].remiseAZeroDesSons();

    if(type==1)
    {
        m_sacPointe.x=-1;
        m_sacPointe.y=-1;
        coordonnee position;

        vueMin.x=hero->m_personnage.getCoordonnee().x-15;
        vueMin.y=hero->m_personnage.getCoordonnee().y-15;
        vueMax.x=hero->m_personnage.getCoordonnee().x+15;
        vueMax.y=hero->m_personnage.getCoordonnee().y+15;

        const sf::FloatRect& ViewRect = camera->GetRect();

        if(vueMin.x<0) { vueMin.x=0; }
        if(vueMin.y<0) { vueMin.y=0; }
        if(vueMax.x>=m_decor[0][0].size()) { vueMax.x=m_decor[0][0].size()-1; }
        if(vueMax.y>=m_decor[0].size()) { vueMax.y=m_decor[0].size()-1; }

        for(int couche=0;couche<2;couche++)
        {
            for(int j=hero->m_personnage.getCoordonnee().y-15;j<hero->m_personnage.getCoordonnee().y+15;++j)
            {
                for(int k=hero->m_personnage.getCoordonnee().x-15;k<hero->m_personnage.getCoordonnee().x+15;++k)
                {
                    position.x=(k-j-1+m_decor[0].size())*64;
                    position.y=(k+j)*32;

                    if(j>=0&&j<m_decor[0].size()&&k>=0&&k<m_decor[0][0].size())
                    {

                        if(couche==1)
                        {
                            if(j>=0&&j<m_decor[0].size()&&k>=0&&k<m_decor[0][0].size())
                            if(configuration.Herbes)
                                if(m_decor[0][j][k].getHerbe()>=0&&m_decor[0][j][k].getHerbe()<m_herbe.size())
                                {
                                    positionPartieDecor=m_herbe[m_decor[0][j][k].getHerbe()].getPositionDuTile(m_decor[0][j][k].getNumeroHerbe());

                                    position.y-=32;

                                    Sprite.SetImage(*moteurGraphique.getImage(m_herbe[m_decor[0][j][k].getHerbe()].getImage(m_decor[0][j][k].getNumeroHerbe())));
                                    if((position.x+64-positionPartieDecor.w/2)-ViewRect.Left<0)
                                    {
                                        int temp=(int)((position.x+64-positionPartieDecor.w/2)-ViewRect.Left);
                                        position.x+=temp/2;
                                        positionPartieDecor.x+=temp;
                                        positionPartieDecor.w-=temp;
                                    }
                                    if((position.x+64-positionPartieDecor.w/2)+positionPartieDecor.w>ViewRect.Right)
                                    {
                                        int temp=(int)((ViewRect.Right-((position.x+64-positionPartieDecor.w/2)+positionPartieDecor.w)));
                                        position.x-=temp/2;
                                        positionPartieDecor.w-=temp;
                                    }
                                    if((position.y-positionPartieDecor.h+64)-ViewRect.Top<0)
                                    {
                                        int temp=(int)(((position.y-positionPartieDecor.h+64)-ViewRect.Top));
                                        positionPartieDecor.y+=temp;
                                        positionPartieDecor.h-=temp;
                                    }
                                    if((position.y-positionPartieDecor.h+64)+positionPartieDecor.h>ViewRect.Bottom)
                                    {
                                        int temp=(int)((ViewRect.Bottom-(position.y-positionPartieDecor.h+64+positionPartieDecor.h)));
                                        position.y-=temp;
                                        positionPartieDecor.h-=temp;
                                    }

                                    Sprite.SetSubRect(IntRect(positionPartieDecor.x, positionPartieDecor.y, positionPartieDecor.x+positionPartieDecor.w, positionPartieDecor.y+positionPartieDecor.h));

                                    if(position.x+positionPartieDecor.w>=ViewRect.Left&&position.x<ViewRect.Right&&position.y+positionPartieDecor.h>=ViewRect.Top&&position.y-positionPartieDecor.h<ViewRect.Bottom)
                                        {
                                            Sprite.SetX(position.x+64-positionPartieDecor.w/2);
                                            Sprite.SetY(position.y-positionPartieDecor.h+64);

                                            if(configuration.Lumiere)
                                            {
                                                Sprite.SetColor(sf::Color(
                                                    (m_tableauDesLampes[j-vueMin.y][k-vueMin.x].intensite*m_tableauDesLampes[j-vueMin.y][k-vueMin.x].rouge)/255,
                                                    (m_tableauDesLampes[j-vueMin.y][k-vueMin.x].intensite*m_tableauDesLampes[j-vueMin.y][k-vueMin.x].vert)/255,
                                                    (m_tableauDesLampes[j-vueMin.y][k-vueMin.x].intensite*m_tableauDesLampes[j-vueMin.y][k-vueMin.x].bleu)/255,
                                                    255));
                                            }
                                            else
                                            {
                                                Sprite.SetColor(sf::Color(
                                                    255,
                                                    255,
                                                    255,
                                                    255));
                                            }
                                            moteurGraphique.AjouterCommande(&Sprite,8,1);
                                             //ecran->Draw(Sprite);
                                        }
                                }

                        if(j>=0&&j<m_decor[0].size()&&k>=0&&k<m_decor[0][0].size())
                        {
                                if(m_monstreIllumine==m_decor[0][j][k].getMonstre()&&m_monstreIllumine!=-1||m_monstreIllumine==m_decor[1][j][k].getMonstre()&&m_monstreIllumine!=-1)
                                {
                                    if(m_decor[0][j][k].getMonstre()>=0&&m_decor[0][j][k].getMonstre()<m_monstre.size())
                                    {
                                        if(m_monstre[m_decor[0][j][k].getMonstre()].enVie())
                                        {
                                            m_monstre[m_decor[0][j][k].getMonstre()].m_lumiere.rouge=255;
                                            m_monstre[m_decor[0][j][k].getMonstre()].m_lumiere.vert=32;
                                            m_monstre[m_decor[0][j][k].getMonstre()].m_lumiere.bleu=32;
                                            m_monstre[m_decor[0][j][k].getMonstre()].m_lumiere.intensite=255;
                                        }

                                        m_monstre[m_decor[0][j][k].getMonstre()].Afficher(ecran,camera,position,getDimensions(),&m_ModeleMonstre[m_monstre[m_decor[0][j][k].getMonstre()].getModele()]);
                                    }

                                    if(m_decor[1][j][k].getMonstre()>=0&&m_decor[1][j][k].getMonstre()<m_monstre.size())
                                    {
                                        if(m_monstre[m_decor[1][j][k].getMonstre()].enVie())
                                        {
                                            m_monstre[m_decor[1][j][k].getMonstre()].m_lumiere.rouge=255;
                                            m_monstre[m_decor[1][j][k].getMonstre()].m_lumiere.vert=32;
                                            m_monstre[m_decor[1][j][k].getMonstre()].m_lumiere.bleu=32;
                                            m_monstre[m_decor[1][j][k].getMonstre()].m_lumiere.intensite=255;
                                        }
                                        m_monstre[m_decor[1][j][k].getMonstre()].Afficher(ecran,camera,position,getDimensions(),&m_ModeleMonstre[m_monstre[m_decor[1][j][k].getMonstre()].getModele()]);
                                    }
                                }
                                else
                                {
                                    if(m_decor[0][j][k].getMonstre()>=0&&m_decor[0][j][k].getMonstre()<m_monstre.size())
                                        m_monstre[m_decor[0][j][k].getMonstre()].Afficher(ecran,camera,position,getDimensions(),&m_ModeleMonstre[m_monstre[m_decor[0][j][k].getMonstre()].getModele()]);
                                    if(m_decor[1][j][k].getMonstre()>=0&&m_decor[1][j][k].getMonstre()<m_monstre.size())
                                        m_monstre[m_decor[1][j][k].getMonstre()].Afficher(ecran,camera,position,getDimensions(),&m_ModeleMonstre[m_monstre[m_decor[1][j][k].getMonstre()].getModele()]);
                                }
                        }

                        if(hero->m_personnage.getCoordonnee().x==hero->m_personnage.getProchaineCase().x&&hero->m_personnage.getCoordonnee().y==hero->m_personnage.getProchaineCase().y)
                        {
                            if(hero->m_personnage.getCoordonnee().x==k&&hero->m_personnage.getCoordonnee().y==j)
                                hero->m_personnage.Afficher(ecran,camera,position,getDimensions(),&hero->m_modelePersonnage);
                        }
                        else
                        {
                            if(hero->m_personnage.getCoordonnee().x>hero->m_personnage.getProchaineCase().x&&hero->m_personnage.getCoordonnee().y>hero->m_personnage.getProchaineCase().y)
                            {
                                if(hero->m_personnage.getProchaineCase().x+1==k&&hero->m_personnage.getProchaineCase().y+1==j)
                                    hero->m_personnage.Afficher(ecran,camera,position,getDimensions(),&hero->m_modelePersonnage);
                            }
                            else
                                if(hero->m_personnage.getProchaineCase().x==k&&hero->m_personnage.getProchaineCase().y==j)
                                    hero->m_personnage.Afficher(ecran,camera,position,getDimensions(),&hero->m_modelePersonnage);
                        }





                        if(j>=0&&j<m_decor[0].size()&&k>=0&&k<m_decor[0][0].size())
                            if(configuration.Herbes)
                                if(m_decor[1][j][k].getHerbe()>=0&&m_decor[1][j][k].getHerbe()<m_herbe.size())
                                {
                                    position.x=(k-j-1+m_decor[1].size())*64;
                                    position.y=(k+j)*32;
                                    positionPartieDecor=m_herbe[m_decor[1][j][k].getHerbe()].getPositionDuTile(m_decor[1][j][k].getNumeroHerbe());

                                    position.y-=32;
                                    position.y+=positionPartieDecor.h/2;

                                    Sprite.SetImage(*moteurGraphique.getImage(m_herbe[m_decor[1][j][k].getHerbe()].getImage(m_decor[1][j][k].getNumeroHerbe())));
                                    if((position.x+64-positionPartieDecor.w/2)-ViewRect.Left<0)
                                    {
                                        int temp=(int)((position.x+64-positionPartieDecor.w/2)-ViewRect.Left);
                                        position.x+=temp/2;
                                        positionPartieDecor.x+=temp;
                                        positionPartieDecor.w-=temp;
                                    }
                                    if((position.x+64-positionPartieDecor.w/2)+positionPartieDecor.w>ViewRect.Right)
                                    {
                                        int temp=(int)((ViewRect.Right-((position.x+64-positionPartieDecor.w/2)+positionPartieDecor.w)));
                                        position.x-=temp/2;
                                        positionPartieDecor.w-=temp;
                                    }
                                    if((position.y-positionPartieDecor.h+64)-ViewRect.Top<0)
                                    {
                                        int temp=(int)(((position.y-positionPartieDecor.h+64)-ViewRect.Top));
                                        positionPartieDecor.y+=temp;
                                        positionPartieDecor.h-=temp;
                                    }
                                    if((position.y-positionPartieDecor.h+64)+positionPartieDecor.h>ViewRect.Bottom)
                                    {
                                        int temp=(int)((ViewRect.Bottom-(position.y-positionPartieDecor.h+64+positionPartieDecor.h)));
                                        position.y-=temp;
                                        positionPartieDecor.h-=temp;
                                    }
                                    Sprite.SetSubRect(IntRect(positionPartieDecor.x, positionPartieDecor.y, positionPartieDecor.x+positionPartieDecor.w, positionPartieDecor.y+positionPartieDecor.h));

                                    if(position.x+positionPartieDecor.w>=ViewRect.Left&&position.x<ViewRect.Right&&position.y+positionPartieDecor.h>=ViewRect.Top&&position.y-positionPartieDecor.h+64<ViewRect.Bottom)
                                        {
                                            Sprite.SetX(position.x+64-positionPartieDecor.w/2);
                                            Sprite.SetY(position.y-positionPartieDecor.h+64);

                                            if(configuration.Lumiere)
                                            {
                                                Sprite.SetColor(sf::Color(
                                                    (m_tableauDesLampes[j-vueMin.y][k-vueMin.x].intensite*m_tableauDesLampes[j-vueMin.y][k-vueMin.x].rouge)/255,
                                                    (m_tableauDesLampes[j-vueMin.y][k-vueMin.x].intensite*m_tableauDesLampes[j-vueMin.y][k-vueMin.x].vert)/255,
                                                    (m_tableauDesLampes[j-vueMin.y][k-vueMin.x].intensite*m_tableauDesLampes[j-vueMin.y][k-vueMin.x].bleu)/255,
                                                    255));
                                            }
                                            else
                                            {
                                                Sprite.SetColor(sf::Color(
                                                    255,
                                                    255,
                                                    255,
                                                    255));
                                            }

                                            moteurGraphique.AjouterCommande(&Sprite,10,1);
                                             //ecran->Draw(Sprite);
                                        }
                                }
                    }

                        if(m_decor[1][j][k].getNombreObjets()>0&&couche==1)
                        {
                            position.x=(k-j-1+m_decor[1].size())*64+48;
                            position.y=(k+j)*32+32;

                            Sprite.SetImage(*moteurGraphique.getImage(IDImageSac));
                            Sprite.SetSubRect(IntRect(0,0,32,32));
                            Sprite.SetX(position.x);
                            Sprite.SetY(position.y);

                            if(configuration.Lumiere)
                            {
                                Sprite.SetColor(sf::Color(
                                (m_tableauDesLampes[j-vueMin.y][k-vueMin.x].intensite*m_tableauDesLampes[j-vueMin.y][k-vueMin.x].rouge)/255,
                                (m_tableauDesLampes[j-vueMin.y][k-vueMin.x].intensite*m_tableauDesLampes[j-vueMin.y][k-vueMin.x].vert)/255,
                                (m_tableauDesLampes[j-vueMin.y][k-vueMin.x].intensite*m_tableauDesLampes[j-vueMin.y][k-vueMin.x].bleu)/255,
                                255));
                            }
                            else
                            {
                                Sprite.SetColor(sf::Color(
                                    255,
                                    255,
                                    255,
                                    255));
                            }

                            coordonnee positionSourisTotale;
                            positionSourisTotale.x=(int)ecran->ConvertCoords(ecran->GetInput().GetMouseX(),ecran->GetInput().GetMouseY()).x;
                            positionSourisTotale.y=(int)ecran->ConvertCoords(ecran->GetInput().GetMouseX(), ecran->GetInput().GetMouseY()).y;

                            if(positionSourisTotale.x>position.x-16*configuration.Resolution.w/800&&positionSourisTotale.x<position.x+48*configuration.Resolution.w/800&&positionSourisTotale.y>position.y-16*configuration.Resolution.w/800&&positionSourisTotale.y<position.y+48*configuration.Resolution.w/800)
                                Sprite.SetColor(sf::Color(
                                    255,
                                    128,
                                    128,
                                    255));

                            moteurGraphique.AjouterCommande(&Sprite,10,1);





                            if(positionSourisTotale.x>position.x-16*configuration.Resolution.w/800&&positionSourisTotale.x<position.x+48*configuration.Resolution.w/800&&positionSourisTotale.y>position.y-16*configuration.Resolution.w/800&&positionSourisTotale.y<position.y+48*configuration.Resolution.w/800||alt)
                            {
                                if(positionSourisTotale.x>position.x-16*configuration.Resolution.w/800&&positionSourisTotale.x<position.x+48*configuration.Resolution.w/800&&positionSourisTotale.y>position.y-16*configuration.Resolution.w/800&&positionSourisTotale.y<position.y+48*configuration.Resolution.w/800)
                                {
                                    m_sacPointe.x=k;
                                    m_sacPointe.y=j;
                                }
                                for(int z=0;z<m_decor[1][j][k].getNombreObjets();z++)
                                {
                                    int rarete=m_decor[1][j][k].getObjet(z).getRarete();
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

                                    texte.SetText(m_decor[1][j][k].getObjet(z).getNom());
                                    texte.SetSize(16*configuration.Resolution.w/800);
                                    texte.SetY((position.y-ViewRect.Top)*configuration.zoom-20*configuration.Resolution.w/800*(z+1));
                                    texte.SetX((position.x-ViewRect.Left)*configuration.zoom);

                                    moteurGraphique.AjouterTexte(&texte,18);

                                }
                            }
                        }




                        if(m_decor[couche][j][k].getTileset()>=0&&m_decor[couche][j][k].getTileset()<m_tileset.size())
                        {
                            position.x=(k-j-1+m_decor[1].size())*64;
                            position.y=(k+j)*32;
                            positionPartieDecor=m_tileset[m_decor[couche][j][k].getTileset()].getPositionDuTile(m_decor[couche][j][k].getTile());
                            if(position.x+positionPartieDecor.h*2+positionPartieDecor.w>=ViewRect.Left&&position.x-positionPartieDecor.h*2-positionPartieDecor.w-64<ViewRect.Right&&position.y+positionPartieDecor.h+positionPartieDecor.w>=ViewRect.Top&&position.y-positionPartieDecor.h-32<ViewRect.Bottom+100&&m_tileset[m_decor[couche][j][k].getTileset()].getOmbreDuTile(m_decor[couche][j][k].getTile()&&configuration.Ombre&&m_tileset[m_decor[couche][j][k].getTileset()].getOmbreDuTile(m_decor[couche][j][k].getTile()))
                            ||position.x+positionPartieDecor.w>=ViewRect.Left&&position.x<ViewRect.Right&&position.y+positionPartieDecor.h>=ViewRect.Top&&position.y-positionPartieDecor.h+64<ViewRect.Bottom)
                            {
                                Sprite.SetImage(*moteurGraphique.getImage(m_tileset[m_decor[couche][j][k].getTileset()].getImage(m_decor[couche][j][k].getTile())));
                                Sprite.SetSubRect(IntRect(positionPartieDecor.x, positionPartieDecor.y, positionPartieDecor.x+positionPartieDecor.w, positionPartieDecor.y+positionPartieDecor.h));

                                if(configuration.Ombre)
                                {
                                    if(m_tileset[m_decor[couche][j][k].getTileset()].getOmbreDuTile(m_decor[couche][j][k].getTile()))
                                    {
                                        for(int o=0;o<m_tableauDesLampes[j-vueMin.y][k-vueMin.x].m_ombre.size();o++)
                                        {
                                            Sprite.SetColor(sf::Color(0,0,0,m_tableauDesLampes[j-vueMin.y][k-vueMin.x].m_ombre[o].intensite));

                                            Sprite.SetColor(sf::Color(0,0,0,m_tableauDesLampes[j-vueMin.y][k-vueMin.x].m_ombre[o].intensite));
                                            Sprite.SetX(position.x+64-positionPartieDecor.w/2+positionPartieDecor.w/2);
                                            Sprite.SetY(position.y-positionPartieDecor.h+64+positionPartieDecor.h-32);
                                           // Sprite.SetX(((m_positionPixel.x-m_positionPixel.y)*64/COTE_TILE+dimensionsMap.y*64)-64+(64-modele->m_pose[m_etat][(int)(m_angle/45)][m_poseEnCours].getCoordonnee().w/2)+modele->m_pose[m_etat][(int)(m_angle/45)][m_poseEnCours].getCoordonnee().w/2);
                                            //Sprite.SetY(((m_positionPixel.x+m_positionPixel.y)*64/COTE_TILE)/2+(64-modele->m_pose[m_etat][(int)(m_angle/45)][m_poseEnCours].getCoordonnee().h)+modele->m_pose[m_etat][(int)(m_angle/45)][m_poseEnCours].getCoordonnee().h-32);
                                            Sprite.SetCenter((positionPartieDecor.w/2),(positionPartieDecor.h-32));
                                            Sprite.SetScale(1, m_tableauDesLampes[j-vueMin.y][k-vueMin.x].m_ombre[o].taille);
                                            Sprite.SetRotation(m_tableauDesLampes[j-vueMin.y][k-vueMin.x].m_ombre[o].angle);
                                             if(m_tableauDesLampes[j-vueMin.y][k-vueMin.x].m_ombre[o].angle>90&&m_tableauDesLampes[j-vueMin.y][k-vueMin.x].m_ombre[o].angle<270)
                                                Sprite.FlipX(true);
                                            if(Sprite.GetPosition().x+Sprite.GetSize().x>=camera->GetRect().Left)
                                            if(Sprite.GetPosition().x-Sprite.GetSize().y<camera->GetRect().Right)
                                            if(Sprite.GetPosition().y+Sprite.GetSize().y>=camera->GetRect().Top)
                                            if(Sprite.GetPosition().y-Sprite.GetSize().x<camera->GetRect().Bottom)
                                            moteurGraphique.AjouterCommande(&Sprite,9,1);
                                            //ecran->Draw(Sprite);
                                            Sprite.SetCenter(0,0);
                                            Sprite.SetScale(1, 1);
                                            Sprite.FlipX(false);
                                            Sprite.SetRotation(0);
                                        }
                                    }
                                }

                                positionHero.y=(int)((hero->m_personnage.getCoordonneePixel().x+hero->m_personnage.getCoordonneePixel().y)*DIVISEUR_COTE_TILE*32);
                                positionHero.x=(int)(((hero->m_personnage.getCoordonneePixel().x-hero->m_personnage.getCoordonneePixel().y)*DIVISEUR_COTE_TILE-1+m_decor[0].size())*64);

                                if(position.x+positionPartieDecor.w>=ViewRect.Left&&position.x<ViewRect.Right&&position.y+positionPartieDecor.h>=ViewRect.Top&&position.y-positionPartieDecor.h+64<ViewRect.Bottom)
                                {
                                    if(couche==1)
                                    {
                                        Sprite.SetX(position.x+64-positionPartieDecor.w/2);
                                        Sprite.SetY(position.y-positionPartieDecor.h+64);
                                    }
                                    else
                                    {
                                        Sprite.SetX(position.x+64-positionPartieDecor.w/2);
                                        Sprite.SetY(position.y-positionPartieDecor.h/2+64);
                                    }

                                    int alpha=255;

                                    if(m_tileset[m_decor[couche][j][k].getTileset()].getTransparentDuTile(m_decor[couche][j][k].getTile()))
                                    {
                                        alpha=(int)((positionHero.y)-position.y)+160;
                                        //if(position.y>positionHero.y+96)
                                          //  alpha=32;

                                        if(alpha<configuration.alpha)
                                            alpha=configuration.alpha;
                                        if(alpha>255)
                                            alpha=255;
                                    }



                                    if(configuration.Lumiere)
                                    {
                                        Sprite.SetColor(sf::Color(
                                            (m_tableauDesLampes[j-vueMin.y][k-vueMin.x].intensite*m_tableauDesLampes[j-vueMin.y][k-vueMin.x].rouge)/255,
                                            (m_tableauDesLampes[j-vueMin.y][k-vueMin.x].intensite*m_tableauDesLampes[j-vueMin.y][k-vueMin.x].vert)/255,
                                            (m_tableauDesLampes[j-vueMin.y][k-vueMin.x].intensite*m_tableauDesLampes[j-vueMin.y][k-vueMin.x].bleu)/255,
                                            alpha));
                                    }
                                    else
                                    {
                                        Sprite.SetColor(sf::Color(
                                            255,
                                            255,
                                            255,
                                            alpha));
                                    }
                                    if(Sprite.GetPosition().x+Sprite.GetSize().x>=camera->GetRect().Left)
                                    if(Sprite.GetPosition().x<camera->GetRect().Right)
                                    if(Sprite.GetPosition().y+Sprite.GetSize().y>=camera->GetRect().Top)
                                    if(Sprite.GetPosition().y<camera->GetRect().Bottom)
                                    moteurGraphique.AjouterCommande(&Sprite,7+couche*3,1);
                                     //ecran->Draw(Sprite);
                                }
                            }

                            positionHero.x=(hero->m_personnage.getCoordonnee().x-hero->m_personnage.getCoordonnee().y-1+m_decor[0].size())/5;
                            positionHero.y=(hero->m_personnage.getCoordonnee().x+hero->m_personnage.getCoordonnee().y)/5;

                            position.x=(k-j-1+m_decor[0].size())/5;
                            position.y=(k+j)/5;

                                if(m_tileset[m_decor[couche][j][k].getTileset()].getSonTile(m_decor[couche][j][k].getTile())>=0)
                                    m_tileset[m_decor[couche][j][k].getTileset()].jouerSon(m_tileset[m_decor[couche][j][k].getTileset()].getSonTile(m_decor[couche][j][k].getTile()),
                                    gpl::sqrt((hero->m_personnage.getCoordonnee().x-k)*(hero->m_personnage.getCoordonnee().x-k)+(hero->m_personnage.getCoordonnee().y-j)*(hero->m_personnage.getCoordonnee().y-j)),position,positionHero);
                        }

                    }
                    else
                    {
                        int w=j,z=k;

                        if(j>=m_decor[0].size())
                            w=m_decor[0].size()-1;
                        if(k>=m_decor[0][0].size())
                            z=m_decor[0][0].size()-1;
                        if(j<0)
                            w=0;
                        if(k<0)
                            z=0;

                        if(m_decor[couche][w][z].getTileset()>=0&&m_decor[couche][w][z].getTileset()<m_tileset.size())
                        {
                            if(couche==0)
                            {

                                positionPartieDecor=m_tileset[m_decor[couche][w][z].getTileset()].getPositionDuTile(m_decor[couche][w][z].getTile());
                                //positionPartieDecor.h/=2;
                                if(position.x+positionPartieDecor.w>=ViewRect.Left&&position.x<ViewRect.Right&&position.y+positionPartieDecor.h>=ViewRect.Top&&position.y-positionPartieDecor.h+64<ViewRect.Bottom)
                                {
                                    int alpha=255;
                                    if(couche==1)
                                    alpha=128;
                                    if(configuration.Lumiere)
                                    {
                                        Sprite.SetColor(sf::Color(
                                                (int)(((double)m_tableauDesLampes[w-vueMin.y][z-vueMin.x].intensite*(double)m_tableauDesLampes[w-vueMin.y][z-vueMin.x].rouge)*0.7/255),
                                                (int)(((double)m_tableauDesLampes[w-vueMin.y][z-vueMin.x].intensite*(double)m_tableauDesLampes[w-vueMin.y][z-vueMin.x].vert)*0.7/255),
                                                (int)(((double)m_tableauDesLampes[w-vueMin.y][z-vueMin.x].intensite*(double)m_tableauDesLampes[w-vueMin.y][z-vueMin.x].bleu)*0.7/255),
                                                alpha));
                                    }
                                    else
                                    {
                                        Sprite.SetColor(sf::Color(
                                                128,
                                                128,
                                                128,
                                                255));
                                    }
                                    Sprite.SetImage(*moteurGraphique.getImage(m_tileset[m_decor[couche][w][z].getTileset()].getImage(m_decor[couche][w][z].getTile())));
                                    Sprite.SetSubRect(IntRect(positionPartieDecor.x, positionPartieDecor.y, positionPartieDecor.x+positionPartieDecor.w, positionPartieDecor.y+positionPartieDecor.h));
                                    if(couche==1)
                                    {
                                        Sprite.SetX(position.x+64-positionPartieDecor.w/2);
                                        Sprite.SetY(position.y-positionPartieDecor.h+64);
                                    }
                                    else
                                    {
                                        Sprite.SetX(position.x+64-positionPartieDecor.w/2);
                                        Sprite.SetY(position.y-positionPartieDecor.h/2+64);
                                    }
                                    moteurGraphique.AjouterCommande(&Sprite,7+couche*3,1);
                                    //ecran->Draw(Sprite);
                                }
                            }
                        }
                    }




                }
            }
        }

    }

	if(type==2)
	{
	    coordonneeDecimal position;
        vueMin.x=hero->m_personnage.getCoordonnee().x-15;
        vueMin.y=hero->m_personnage.getCoordonnee().y-15;
        vueMax.x=hero->m_personnage.getCoordonnee().x+16;
        vueMax.y=hero->m_personnage.getCoordonnee().y+15;

        Sprite.SetCenter(4*configuration.Resolution.w/800,4*configuration.Resolution.w/800);
        Sprite.SetRotation(45);

        Sprite.SetColor(sf::Color(255,255,255,128*(int)alpha/255));

            for(int j=vueMin.y;j<vueMax.y;j++)
            {
                for(int k=vueMin.x;k<vueMax.x;k++)
                {
                    position.x=(float)(((k-vueMin.x)-(j-vueMin.y)-1+40)*6*(float)configuration.Resolution.w/800);
                    position.y=(float)(((k-vueMin.x)+(j-vueMin.y))*6*(float)configuration.Resolution.w/800);
                    if(position.x+465*configuration.Resolution.w/800>605*configuration.Resolution.w/800&&position.x+465*configuration.Resolution.w/800<800*configuration.Resolution.w/800&&position.y*configuration.Resolution.y/600>0&&position.y-80*configuration.Resolution.w/800<195*configuration.Resolution.h/600)
                    {
                        if(getTypeCase(k,j)==1)
                        {
                            Sprite.SetImage(carreBrun);
                            Sprite.SetX((float)(position.x+465*configuration.Resolution.w/800));
                            Sprite.SetY((float)(position.y-80*configuration.Resolution.w/800));
                            moteurGraphique.AjouterCommande(&Sprite,14,0);
                            //ecran->Draw(Sprite);
                        }

                        if(getTypeCase(k,j)==2)
                        {
                            Sprite.SetImage(carreRouge);

                            Sprite.SetX((float)(position.x+465*configuration.Resolution.w/800));
                            Sprite.SetY((float)(position.y-80*configuration.Resolution.w/800));
                            moteurGraphique.AjouterCommande(&Sprite,14,0);
                            //ecran->Draw(Sprite);
                        }

                        if(getTypeCase(k,j)==3)
                        {
                                Sprite.SetImage(carreVert);
                                Sprite.SetX((float)(position.x+465*configuration.Resolution.w/800));
                                Sprite.SetY((float)(position.y-80*configuration.Resolution.w/800));
                                moteurGraphique.AjouterCommande(&Sprite,14,0);
                                //ecran->Draw(Sprite);
                        }

                        if(getTypeCase(k,j)==4)
                        {
                                Sprite.SetImage(carreJaune);
                                Sprite.SetX((float)(position.x+465*configuration.Resolution.w/800));
                                Sprite.SetY((float)(position.y-80*configuration.Resolution.w/800));
                                moteurGraphique.AjouterCommande(&Sprite,14,0);
                                //ecran->Draw(Sprite);
                        }

                        if(hero->m_personnage.getCoordonnee().x==k&&hero->m_personnage.getCoordonnee().y==j)
                        {
                                Sprite.SetImage(carreBleu);
                                Sprite.SetX((float)(position.x+465*configuration.Resolution.w/800));
                                Sprite.SetY((float)(position.y-80*configuration.Resolution.w/800));
                                moteurGraphique.AjouterCommande(&Sprite,14,0);
                                //ecran->Draw(Sprite);
                        }
                    }
                }
            }
	}

	if(type==3)
	{


	    m_objetPointe=-1;
	    coordonnee position;

	    if(hero->getChercherSac().x>=0&&hero->getChercherSac().x<m_decor[1][0].size()
	     &&hero->getChercherSac().y>=0&&hero->getChercherSac().y<m_decor[1].size())
	    if(m_decor[1][hero->getChercherSac().y][hero->getChercherSac().x].getNombreObjets()>0)
        {
            if(m_defilerObjets>m_decor[1][hero->getChercherSac().y][hero->getChercherSac().x].getNombreObjets()-11)
                m_defilerObjets=m_decor[1][hero->getChercherSac().y][hero->getChercherSac().x].getNombreObjets()-11;
            if(m_defilerObjets<0)
                m_defilerObjets=0;

            for(int z=m_defilerObjets;z<m_decor[1][hero->getChercherSac().y][hero->getChercherSac().x].getNombreObjets()&&z<12+m_defilerObjets;z++)
            {

                if(positionSouris.x>configuration.Resolution.w-configuration.Resolution.w*0.25
                 &&positionSouris.y>configuration.Resolution.w*0.265+(z-m_defilerObjets)*20*configuration.Resolution.w/800
                 &&positionSouris.y<configuration.Resolution.w*0.265+(z-m_defilerObjets)*20*configuration.Resolution.w/800+20*configuration.Resolution.w/800)
                 {
                     Sprite.SetImage(*moteurGraphique.getImage(0));
                     Sprite.SetColor(sf::Color(255,255,255,128));
                     Sprite.Resize(configuration.Resolution.w*0.24,20*configuration.Resolution.w/800);
                     Sprite.SetX(configuration.Resolution.w-configuration.Resolution.w*0.24*alpha/255);
                     Sprite.SetY(configuration.Resolution.w*0.265+(z-m_defilerObjets)*20*configuration.Resolution.w/800);
                     moteurGraphique.AjouterCommande(&Sprite,14,0);

                    if(!(z-m_defilerObjets==0&&m_defilerObjets>0)&&!((z-m_defilerObjets==11&&z+m_defilerObjets<=m_decor[1][hero->getChercherSac().y][hero->getChercherSac().x].getNombreObjets()+1)))
                        m_objetPointe=z;
                 }


                int rarete=m_decor[1][hero->getChercherSac().y][hero->getChercherSac().x].getObjet(z).getRarete();
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
                    texte.SetText(m_decor[1][hero->getChercherSac().y][hero->getChercherSac().x].getObjet(z).getNom());
                texte.SetSize(16*configuration.Resolution.w/800);

                position.x=(int)((configuration.Resolution.w-configuration.Resolution.w*0.25*alpha/255)+(configuration.Resolution.w*0.25)/2-(texte.GetRect().Right-texte.GetRect().Left)/2);
                position.y=(int)(configuration.Resolution.w*0.265+(z-m_defilerObjets)*20*configuration.Resolution.w/800);

                texte.SetY(position.y);
                texte.SetX(position.x);

                moteurGraphique.AjouterTexte(&texte,14);
            }


        }
	}
}

void Map::AfficherNomEvenement(sf::RenderWindow* ecran,coordonnee casePointee,coordonnee positionSouris)
{

    int evenement=-1;
    for(int i=0;i<2;i++)
        for(int z=0;z<m_decor[i][casePointee.y][casePointee.x].getEvenement().size();z++)
            if(m_decor[i][casePointee.y][casePointee.x].getEvenement()[z]>-1)
            {
                evenement=m_decor[i][casePointee.y][casePointee.x].getEvenement()[z];

                if(evenement>=0)
                {
                    if(m_evenement[evenement].getType()==CHANGEMENT_DE_MAP)
                    {
                        string nom;
                        char chemin[128];
                        sprintf(chemin,"Data/Maps/map%ld.map.hs",m_evenement[evenement].getInformation(0));


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
                                    getline(fichier, nom);
                                }
                            }while(caractere!='$');
                        }
                        fichier.close();

                        sprintf(chemin,"Vers %s",nom.c_str());

                        sf::String texte;
                        texte.SetText(chemin);
                        texte.SetSize(16.f);
                        if(configuration.Resolution.y>0)
                            texte.SetY((positionSouris.y-16)*configuration.Resolution.h/configuration.Resolution.y);
                        if(configuration.Resolution.x>0)
                            texte.SetX(positionSouris.x*configuration.Resolution.w/configuration.Resolution.x);
                        moteurGraphique.AjouterTexte(&texte,15);
                        //ecran->Draw(texte);
                    }
                }
            }
}

bool Map::testEvenement(sf::View *camera, Jeu *jeu,float temps)
{
    for(int i=0;i<2;i++)
    for(int z=0;z<m_decor[i][jeu->hero.m_personnage.getCoordonnee().y][jeu->hero.m_personnage.getCoordonnee().x].getEvenement().size();z++)
    if(m_decor[i][jeu->hero.m_personnage.getCoordonnee().y][jeu->hero.m_personnage.getCoordonnee().x].getEvenement()[z]>=0)
    {
        if(m_evenement[m_decor[i][jeu->hero.m_personnage.getCoordonnee().y][jeu->hero.m_personnage.getCoordonnee().x].getEvenement()[z]].getType()==CHANGEMENT_DE_MAP)
        {
            int numeroMap=m_evenement[m_decor[i][jeu->hero.m_personnage.getCoordonnee().y][jeu->hero.m_personnage.getCoordonnee().x].getEvenement()[z]].getInformation(0);

            console.Ajouter("",0);
            console.Ajouter("---------------------------------------------------------------------------------",0);
            console.Ajouter("EVENEMENT : Changement de map",0);
            console.Ajouter("---------------------------------------------------------------------------------",0);

            coordonnee coordonneePerso;
            coordonneePerso.x=m_evenement[m_decor[i][jeu->hero.m_personnage.getCoordonnee().y][jeu->hero.m_personnage.getCoordonnee().x].getEvenement()[z]].getInformation(1);
            coordonneePerso.y=m_evenement[m_decor[i][jeu->hero.m_personnage.getCoordonnee().y][jeu->hero.m_personnage.getCoordonnee().x].getEvenement()[z]].getInformation(2);

            sf::Clock Clock;
             Clock.Reset();
            float Time = 0,temps_ecoule=0,tempsEcouleDepuisDernierAffichage=0;

            jeu->m_chargement->setC_Chargement(numeroMap,coordonneePerso);
            jeu->m_contexte = jeu->m_chargement;
          //  jeu->m_contexte->CopierCamera(jeu->m_chargement->camera);
        }

        if(m_evenement[m_decor[i][jeu->hero.m_personnage.getCoordonnee().y][jeu->hero.m_personnage.getCoordonnee().x].getEvenement()[z]].getType()==INFLIGER_DEGATS)
        {
            jeu->hero.m_personnage.infligerDegats(m_evenement[m_decor[i][jeu->hero.m_personnage.getCoordonnee().y][jeu->hero.m_personnage.getCoordonnee().x].getEvenement()[z]].getInformation(0)*temps*10);
        }
    }
    return 1;
}


void Map::animer(Hero *hero,float temps,Menu *menu,sf::View *camera)
{
    coordonnee vueMin,vueMax;

    vueMin.x=hero->m_personnage.getCoordonnee().x-10;
    vueMin.y=hero->m_personnage.getCoordonnee().y-10;
    vueMax.x=hero->m_personnage.getCoordonnee().x+10;
    vueMax.y=hero->m_personnage.getCoordonnee().y+10;

    if(vueMin.x<0) { vueMin.x=0; }
    if(vueMin.y<0) { vueMin.y=0; }
    if(vueMax.x>m_decor[0][0].size()) { vueMax.x=m_decor[0][0].size(); }
    if(vueMax.y>m_decor[0].size()) { vueMax.y=m_decor[0].size(); }

	for(int i=0;i<2;i++)
        for(int j=vueMin.y;j<vueMax.y;j++)
            for(int k=vueMin.x;k<vueMax.x;k++)
            {
                for(int z=0;z<m_decor[i][j][k].getEvenement().size();z++)
                    if(m_decor[i][j][k].getEvenement()[z]>=0&&m_decor[i][j][k].getEvenement()[z]<m_evenement.size())
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
                while(m_decor[i][j][k].getAnimation()>=0.075)
                {
                    if(m_decor[i][j][k].getTileset()>=0&&m_decor[i][j][k].getTileset()<m_tileset.size())
                        if(m_tileset[m_decor[i][j][k].getTileset()].getAnimationTile(m_decor[i][j][k].getTile())>=0)
                            m_decor[i][j][k].setDecor(m_decor[i][j][k].getTileset(),m_tileset[m_decor[i][j][k].getTileset()].getAnimationTile(m_decor[i][j][k].getTile()),m_decor[i][j][k].getEvenement(),m_decor[i][j][k].getMonstre(),m_decor[i][j][k].getHerbe());
                    m_decor[i][j][k].decrementerAnimation();
                }
                    if(m_decor[i][j][k].getMonstre()>=0&&m_decor[i][j][k].getMonstre()<m_monstre.size())
                    {
                        bool explosif=false;
                        int degats = m_monstre[m_decor[i][j][k].getMonstre()].animer(&m_ModeleMonstre[m_monstre[m_decor[i][j][k].getMonstre()].getModele()],m_decor[0].size(),temps,&explosif);
                        if(explosif&&degats>0)
                        {
                            verifierDeclencheursDegats(j,k);

                            for(int couche=0;couche<2;couche++)
                                for(int y=j-1;y<=j+1;y++)
                                    for(int x=k-1;x<=k+1;x++)
                                        if(y>0&&x>0&&y<m_decor[0].size()&&x<m_decor[0][0].size())
                                        {
                                            infligerDegats(m_decor[couche][y][x].getMonstre(), degats,menu,camera);

                                            if(y==hero->m_personnage.getCoordonnee().y&&x==hero->m_personnage.getCoordonnee().x)
                                                hero->m_personnage.infligerDegats(degats);


                                        }
                        }
                        else
                        hero->m_personnage.infligerDegats(degats);
                   }

           }

}

void Map::gererEvenements(int evenement,int z,int couche,int x,int y)
{
    if(evenement>=0&&evenement<m_evenement.size())
    {
        if(m_evenement[m_decor[couche][y][x].getEvenement()[z]].getType()==CHANGER_DECOR)
        {
            m_decor[couche][y][x].setTileset(m_evenement[evenement].getInformation(0));
            m_decor[couche][y][x].setTile(m_evenement[evenement].getInformation(1));
        }
        if(m_evenement[evenement].getType()==EXPLOSION)
        {
            m_decor[couche][y][x].setEvenement(-1,z);
            verifierDeclencheursDegats(y,x);
        }
    }
}

void Map::verifierDeclencheursDegats(int i, int j)
{
    for(int o=0;o<2;o++)
        for(int y=i-1;y<=i+1;y++)
            for(int x=j-1;x<=j+1;x++)
                if(y>0&&x>0&&y<m_decor[0].size()&&x<m_decor[0][0].size())
                {
                    for(int z=0;z<m_decor[o][y][x].getEvenement().size();z++)
                    {
                        if(m_decor[o][y][x].getEvenement()[z]>=0&&m_decor[o][y][x].getEvenement()[z]<m_evenement.size())
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
    if(m_musiqueEnCours>=0&&m_musiqueEnCours<MAX_MUSIQUE)
    {
      //  if(m_musique[m_musiqueEnCours].GetPlayingOffset)

        Sound::Status Status = m_musique[m_musiqueEnCours].GetStatus();

        if(Status==0)
        {
            m_musiqueEnCours++;
            if(m_musiqueEnCours>=0&&m_musiqueEnCours<MAX_MUSIQUE)
                m_musique[m_musiqueEnCours].Play();
         }

        if(m_musiqueEnCours>=0&&m_musiqueEnCours<MAX_MUSIQUE)
            m_musique[m_musiqueEnCours].SetPosition(-position.x, 0, position.y);
    }
    else
    m_musiqueEnCours=0;
}


#define RANDOMDISPLACE() if(m_monstre[m_decor[i][j][k].getMonstre()].enVie())   \
                                {   \
                                    if(m_monstre[m_decor[i][j][k].getMonstre()].getArrivee().x==m_monstre[m_decor[i][j][k].getMonstre()].getCoordonnee().x&&m_monstre[m_decor[i][j][k].getMonstre()].getArrivee().y==m_monstre[m_decor[i][j][k].getMonstre()].getCoordonnee().y) \
                                    {   \
                                        coordonnee arrivee;   \
                                        arrivee=m_monstre[m_decor[i][j][k].getMonstre()].getDepart();   \
                                        arrivee.x+=(rand()%(3 + 3) - 3);   \
                                        arrivee.y+=(rand()%(3 + 3) - 3);   \
                                        m_monstre[m_decor[i][j][k].getMonstre()].setArrivee(arrivee);   \
                                    }   \
                                    else   \
                                    {   \
                                        if(m_monstre[m_decor[i][j][k].getMonstre()].seDeplacer(temps*100))   \
                                        {   \
                                            coordonnee tempCoord={-1,-1,-1,-1};   \
                                            m_monstre[m_decor[i][j][k].getMonstre()].pathfinding(getAlentourDuPersonnage(m_monstre[m_decor[i][j][k].getMonstre()].getCoordonnee()),tempCoord);   \
                                            if(!(m_monstre[m_decor[i][j][k].getMonstre()].getCoordonnee().y==j&&m_monstre[m_decor[i][j][k].getMonstre()].getCoordonnee().x==k))   \
                                                if(m_monstre[m_decor[i][j][k].getMonstre()].getCoordonnee().x>0&&m_monstre[m_decor[i][j][k].getMonstre()].getCoordonnee().x<m_decor[0][0].size()&&m_monstre[m_decor[i][j][k].getMonstre()].getCoordonnee().y>0&&m_monstre[m_decor[i][j][k].getMonstre()].getCoordonnee().y<m_decor[0].size())   \
                                                {   \
                                                    m_decor[i][m_monstre[m_decor[i][j][k].getMonstre()].getCoordonnee().y][m_monstre[m_decor[i][j][k].getMonstre()].getCoordonnee().x].setMonstre(m_decor[i][j][k].getMonstre());   \
                                                    m_decor[i][j][k].setMonstre(-1);   \
                                                }   \
                                            }   \
                                    }   \
                                }

#define FIGHT() if(!m_monstre[m_decor[i][j][k].getMonstre()].getErreurPathfinding())   \
                                {   \
                                    if(m_monstre[m_decor[i][j][k].getMonstre()].enVie())   \
                                    {   \
                                        if(fabs(m_monstre[m_decor[i][j][k].getMonstre()].getCoordonnee().x-hero->m_personnage.getProchaineCase().x)>1||fabs(m_monstre[m_decor[i][j][k].getMonstre()].getCoordonnee().y-hero->m_personnage.getProchaineCase().y)>1)   \
                                        {   \
                                                coordonnee arrivee,enCours;   \
                                                if(m_monstre[m_decor[i][j][k].getMonstre()].getArrivee().x==m_monstre[m_decor[i][j][k].getMonstre()].getCoordonnee().x&&m_monstre[m_decor[i][j][k].getMonstre()].getArrivee().y==m_monstre[m_decor[i][j][k].getMonstre()].getCoordonnee().y)   \
                                                {   \
                                                    arrivee=hero->m_personnage.getProchaineCase();   \
                                                    m_monstre[m_decor[i][j][k].getMonstre()].setArrivee(arrivee);   \
                                                }   \
                                                if(m_monstre[m_decor[i][j][k].getMonstre()].seDeplacer(temps*100))   \
                                                {   \
                                                    coordonnee tempCoord={-1,-1,-1,-1};   \
                                                    m_monstre[m_decor[i][j][k].getMonstre()].pathfinding(getAlentourDuPersonnage(m_monstre[m_decor[i][j][k].getMonstre()].getCoordonnee()),tempCoord);   \
                                                    if(m_monstre[m_decor[i][j][k].getMonstre()].getErreurPathfinding())   \
                                                    {   \
                                                        coordonnee arrivee;   \
                                                        arrivee=m_monstre[m_decor[i][j][k].getMonstre()].getDepart();   \
                                                        arrivee.x+=(rand()%(3 + 3) - 3);   \
                                                        arrivee.y+=(rand()%(3 + 3) - 3);   \
                                                        m_monstre[m_decor[i][j][k].getMonstre()].setArrivee(arrivee);   \
                                                    }   \
                                                    else   \
                                                    {   \
                                                         arrivee=hero->m_personnage.getProchaineCase();   \
                                                         m_monstre[m_decor[i][j][k].getMonstre()].setArrivee(arrivee);   \
                                                    }   \
                                                    if(!(m_monstre[m_decor[i][j][k].getMonstre()].getCoordonnee().y==j&&m_monstre[m_decor[i][j][k].getMonstre()].getCoordonnee().x==k))   \
                                                    if(m_monstre[m_decor[i][j][k].getMonstre()].getCoordonnee().x>0&&m_monstre[m_decor[i][j][k].getMonstre()].getCoordonnee().x<m_decor[0][0].size()&&m_monstre[m_decor[i][j][k].getMonstre()].getCoordonnee().y>0&&m_monstre[m_decor[i][j][k].getMonstre()].getCoordonnee().y<m_decor[0].size())   \
                                                    {   \
                                                        if(m_decor[0][m_monstre[m_decor[i][j][k].getMonstre()].getCoordonnee().y][m_monstre[m_decor[i][j][k].getMonstre()].getCoordonnee().x].getMonstre()<0&&m_decor[1][m_monstre[m_decor[i][j][k].getMonstre()].getCoordonnee().y][m_monstre[m_decor[i][j][k].getMonstre()].getCoordonnee().x].getMonstre()<0)   \
                                                            m_decor[i][m_monstre[m_decor[i][j][k].getMonstre()].getCoordonnee().y][m_monstre[m_decor[i][j][k].getMonstre()].getCoordonnee().x].setMonstre(m_decor[i][j][k].getMonstre());   \
                                                        else   \
                                                        {   \
                                                            int temp=m_decor[i][j][k].getMonstre();   \
                                                            if(m_decor[1][m_monstre[m_decor[i][j][k].getMonstre()].getCoordonnee().y][m_monstre[m_decor[i][j][k].getMonstre()].getCoordonnee().x].getMonstre()>=0)   \
                                                                m_decor[0][m_monstre[m_decor[i][j][k].getMonstre()].getCoordonnee().y][m_monstre[m_decor[i][j][k].getMonstre()].getCoordonnee().x].setMonstre(m_decor[1][m_monstre[m_decor[i][j][k].getMonstre()].getCoordonnee().y][m_monstre[m_decor[i][j][k].getMonstre()].getCoordonnee().x].getMonstre());   \
                                                            m_decor[1][m_monstre[m_decor[i][j][k].getMonstre()].getCoordonnee().y][m_monstre[m_decor[i][j][k].getMonstre()].getCoordonnee().x].setMonstre(temp);   \
                                                        }   \
                                                        m_decor[i][j][k].setMonstre(-1);   \
                                                    }   \
                                                }   \
                                        }   \
                                        else   \
                                        {   \
                                            if(hero->m_personnage.enVie()<=0)   \
                                                m_monstre[m_decor[i][j][k].getMonstre()].setVu(0);   \
                                            if(m_monstre[m_decor[i][j][k].getMonstre()].getEtat()!=2)   \
                                                m_monstre[m_decor[i][j][k].getMonstre()].setEtat(2);   \
                                            coordonnee temp,temp2;   \
                                            temp.x=(hero->m_personnage.getCoordonneePixel().x-hero->m_personnage.getCoordonneePixel().y-1+m_decor[1].size())*64;   \
                                            temp.y=(hero->m_personnage.getCoordonneePixel().x+hero->m_personnage.getCoordonneePixel().y)*32;   \
                                            temp2.x=(m_monstre[m_decor[i][j][k].getMonstre()].getCoordonneePixel().x-m_monstre[m_decor[i][j][k].getMonstre()].getCoordonneePixel().y-1+m_decor[1].size())*64;   \
                                            temp2.y=(m_monstre[m_decor[i][j][k].getMonstre()].getCoordonneePixel().x+m_monstre[m_decor[i][j][k].getMonstre()].getCoordonneePixel().y)*32;   \
                                            m_monstre[m_decor[i][j][k].getMonstre()].frappe(temp,temp2);   \
                                            m_monstre[m_decor[i][j][k].getMonstre()].setArrivee(m_monstre[m_decor[i][j][k].getMonstre()].getCoordonnee());   \
                                        }   \
                                    }   \
                                }




#define LISTE_INSTRUCTIONS(noInstruction) if(noInstruction>=0&&noInstruction<script.m_instructions.size()) \
                                             { \
                                                if(script.m_instructions[noInstruction].nom=="fight") { FIGHT() } \
                                                if(script.m_instructions[noInstruction].nom=="randomDisplace") { RANDOMDISPLACE() } \
                                                if(script.m_instructions[noInstruction].nom=="if") { gererConditions(script,noInstruction,i,j,k,hero,temps,camera,menu); } \
                                             }

#define LISTE_CONDITIONS(noInstruction) if(script.m_instructions[noInstruction].nom=="see") \
                                                    if(!m_monstre[m_decor[i][j][k].getMonstre()].getVu()) \
                                                        ok=false;


void Map::gererConditions(Script script,int noInstruction,int i, int j, int k,Hero *hero,float temps,sf::View *camera,Menu *menu)
{
    if(noInstruction>=0&&noInstruction<script.m_instructions.size())
    {
        int b=0;
        bool ok=true;
        for(;b<script.m_instructions[noInstruction].valeurs.size()&&script.m_instructions[noInstruction].valeurs[b]!=-2;b++)
        {
            if(script.m_instructions[noInstruction].valeurs[b]>=0&&script.m_instructions[noInstruction].valeurs[b]<script.m_instructions.size())
                LISTE_CONDITIONS(script.m_instructions[noInstruction].valeurs[b]);
        }
        for(;b<script.m_instructions[noInstruction].valeurs.size()&&script.m_instructions[noInstruction].valeurs[b]!=-1&&script.m_instructions[noInstruction].valeurs[b]!=-3;b++)
            if(ok)
                if(script.m_instructions[noInstruction].valeurs[b]>=0&&script.m_instructions[noInstruction].valeurs[b]<script.m_instructions.size())   \
                    LISTE_INSTRUCTIONS(script.m_instructions[noInstruction].valeurs[b])
        if(!ok)
            for(;b<script.m_instructions[noInstruction].valeurs.size()&&script.m_instructions[noInstruction].valeurs[b]!=-1;b++)
                if(script.m_instructions[noInstruction].valeurs[b]>=0&&script.m_instructions[noInstruction].valeurs[b]<script.m_instructions.size())
                    LISTE_INSTRUCTIONS(script.m_instructions[noInstruction].valeurs[b])
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
    if(vueMax.x>=m_decor[0][0].size()) { vueMax.x=m_decor[0][0].size()-1; }
    if(vueMax.y>=m_decor[0].size()) { vueMax.y=m_decor[0].size()-1; }

	for(int i=0;i<2;i++)
        for(int j=vueMin.y;j<vueMax.y;j++)
            for(int k=vueMin.x;k<vueMax.x;k++)
                    if(m_decor[i][j][k].getMonstre()>=0&&m_decor[i][j][k].getMonstre()<m_monstre.size())
                    {
                        if(hero->m_personnage.enVie())
                            m_monstre[m_decor[i][j][k].getMonstre()].testerVision(hero->m_personnage.getCoordonnee());


                        if(m_monstre[m_decor[i][j][k].getMonstre()].m_attente<=0)
                        {
                            Script script=m_ModeleMonstre[m_monstre[m_decor[i][j][k].getMonstre()].getModele()].m_scriptAI;
                            for(int a=0;a<script.m_instructions[0].valeurs.size();a++)
                                if(script.m_instructions[0].valeurs[a]>=0&&script.m_instructions[0].valeurs[a]<script.m_instructions.size())
                                    if(script.m_instructions[script.m_instructions[0].valeurs[a]].nom=="if")
                                        gererConditions(script,script.m_instructions[0].valeurs[a],i,j,k,hero,temps,camera,menu);

                            if(m_monstre[m_decor[i][j][k].getMonstre()].getErreurPathfinding())
                                RANDOMDISPLACE()
                        }
                        else
                            m_monstre[m_decor[i][j][k].getMonstre()].m_attente-=temps;

                       if(m_monstre[m_decor[i][j][k].getMonstre()].getErreurPathfinding())
                            m_monstre[m_decor[i][j][k].getMonstre()].m_attente=0.1;

                        if(!m_monstre[m_decor[i][j][k].getMonstre()].enVie()&&m_monstre[m_decor[i][j][k].getMonstre()].getEtat()!=3)
                                    m_monstre[m_decor[i][j][k].getMonstre()].setEtat(3);



                        ///GESTION DES EVENEMENTS SUR LES MONSTRES
                        for(int l=0;l<2;l++)
                            if(m_monstre[m_decor[i][j][k].getMonstre()].getCoordonnee().y>=0&&m_monstre[m_decor[i][j][k].getMonstre()].getCoordonnee().y<m_decor[0].size()
                             &&m_monstre[m_decor[i][j][k].getMonstre()].getCoordonnee().x>=0&&m_monstre[m_decor[i][j][k].getMonstre()].getCoordonnee().x<m_decor[0][0].size())
                             for(int z=0;z<m_decor[l][m_monstre[m_decor[i][j][k].getMonstre()].getCoordonnee().y][m_monstre[m_decor[i][j][k].getMonstre()].getCoordonnee().x].getEvenement().size();z++)
                             if(m_decor[l][m_monstre[m_decor[i][j][k].getMonstre()].getCoordonnee().y][m_monstre[m_decor[i][j][k].getMonstre()].getCoordonnee().x].getEvenement()[z]>=0&&m_decor[l][m_monstre[m_decor[i][j][k].getMonstre()].getCoordonnee().y][m_monstre[m_decor[i][j][k].getMonstre()].getCoordonnee().x].getEvenement()[z]<m_evenement.size())
                                if(m_evenement[m_decor[l][m_monstre[m_decor[i][j][k].getMonstre()].getCoordonnee().y][m_monstre[m_decor[i][j][k].getMonstre()].getCoordonnee().x].getEvenement()[z]].getType()==INFLIGER_DEGATS)
                                {
                                    //m_monstre[m_decor[i][j][k].getMonstre()].infligerDegats(m_evenement[m_decor[l][m_monstre[m_decor[i][j][k].getMonstre()].getCoordonnee().y][m_monstre[m_decor[i][j][k].getMonstre()].getCoordonnee().x].getEvenement()].getInformation(0)*temps*10);
                                    infligerDegats(m_decor[i][j][k].getMonstre(),m_evenement[m_decor[l][m_monstre[m_decor[i][j][k].getMonstre()].getCoordonnee().y][m_monstre[m_decor[i][j][k].getMonstre()].getCoordonnee().x].getEvenement()[z]].getInformation(0)*temps*10,menu,camera);
                                }
                    }
}

bool Map::infligerDegats(int numeroMonstre, float degats,Menu *menu,sf::View *camera)
{
    if(numeroMonstre>=0&&numeroMonstre<m_monstre.size())
    {
        if(configuration.sang&&m_monstre[numeroMonstre].enVie())
            for(int i=0;i<5;i++)
                if(rand()%(100)>25&&m_monstre[numeroMonstre].getCaracteristique().sang)
                {
                    coordonneeDecimal position;
                    position.x=rand()%(600 - 100) + 100;
                    position.y=rand()%(400 - 100 ) + 100;

                    menu->AjouterSang(position);
                }

        float viePrecedente=m_monstre[numeroMonstre].getCaracteristique().vie;

        m_monstre[numeroMonstre].infligerDegats(degats);
        if(!m_monstre[numeroMonstre].enVie()&&viePrecedente>0)
        {
            if(m_monstre[numeroMonstre].getCaracteristique().pointAme>0)
            {
                coordonneeDecimal position;
                position.x=(((m_monstre[numeroMonstre].getCoordonnee().x-m_monstre[numeroMonstre].getCoordonnee().y-1+m_decor[0].size())*64)-camera->GetRect().Left+48-(configuration.Resolution.x/configuration.zoom/2-400));
                position.y=(((m_monstre[numeroMonstre].getCoordonnee().x+m_monstre[numeroMonstre].getCoordonnee().y)*32)-camera->GetRect().Top-96);
                menu->AjouterAme(position,m_monstre[numeroMonstre].getCaracteristique().pointAme);
            }

            if(m_monstre[numeroMonstre].getCoordonnee().x>=0&&m_monstre[numeroMonstre].getCoordonnee().x<m_decor[0][0].size()&&m_monstre[numeroMonstre].getCoordonnee().y>=0&&m_monstre[numeroMonstre].getCoordonnee().y<m_decor[0].size())
            {
                /*for(int i=0;i<(m_monstre[numeroMonstre].getCaracteristique().rang+1)*3;i++)
                {
                    if(rand()%1000<500)
                    {
                        int rarete=NORMAL;
                        int random=rand()%10000;
                        if(random<=2811)
                            rarete=BONNEFACTURE;
                        if(random<=811)
                            rarete=BENI;
                        if(random<=311)
                            rarete=SACRE;
                        if(random<111)
                            rarete=SANCTIFIE;
                        if(random<11)
                            rarete=DIVIN;
                        if(random==1)
                            rarete=INFERNAL;


                        Objet temp("Un Objet Merveilleux",rarete);
                        m_decor[1][m_monstre[numeroMonstre].getCoordonnee().y][m_monstre[numeroMonstre].getCoordonnee().x].ajouterObjet(temp);
                    }
                }*/
                if(m_monstre[numeroMonstre].getModele()>=0&&m_monstre[numeroMonstre].getModele()<m_ModeleMonstre.size())
                for(int j=0;j<(m_monstre[numeroMonstre].getCaracteristique().rang+1)*3;j++)
                for(int i=0;i<m_ModeleMonstre[m_monstre[numeroMonstre].getModele()].getObjets().size();i++)
                    if(rand()%1000<m_ModeleMonstre[m_monstre[numeroMonstre].getModele()].getObjets()[i].getChanceTrouver())
                    {
                        int rarete=m_ModeleMonstre[m_monstre[numeroMonstre].getModele()].getObjets()[i].getRarete();

                        if(rarete==0)
                        {
                            int random=rand()%10000;
                            if(random<=3000)
                                rarete=BONNEFACTURE;
                            if(random<=300)
                                rarete=BENI;
                            if(random<=50)
                                rarete=SACRE;
                            if(random<20)
                                rarete=SANCTIFIE;
                            if(random<3)
                                rarete=DIVIN;
                            if(random==1)
                                rarete=INFERNAL;
                        }

                        Objet temp;
                        temp=m_ModeleMonstre[m_monstre[numeroMonstre].getModele()].getObjets()[i];
                        temp.setRarete(rarete);
                        m_decor[1][m_monstre[numeroMonstre].getCoordonnee().y][m_monstre[numeroMonstre].getCoordonnee().x].ajouterObjet(temp);
                    }
            }

            return 1;
        }
    }
    return 0;
}

void Map::ramasserObjet(Hero *hero)
{
    if(hero->getChercherSac().x>=0&&hero->getChercherSac().x<m_decor[1][0].size()
    &&hero->getChercherSac().y>=0&&hero->getChercherSac().y<m_decor[1].size())
	    if(m_objetPointe>=0&&m_objetPointe<m_decor[1][hero->getChercherSac().y][hero->getChercherSac().x].getNombreObjets())
        {
            if(hero->ajouterObjet(m_decor[1][hero->getChercherSac().y][hero->getChercherSac().x].getObjet(m_objetPointe)))
                m_decor[1][hero->getChercherSac().y][hero->getChercherSac().x].supprimerObjet(m_objetPointe);
        }
}
void Map::AjouterObjet(Objet objet)
{
    if(objet.getPosition().x>=0&&objet.getPosition().x<m_decor[0][0].size()&&objet.getPosition().y>=0&&objet.getPosition().y<m_decor[0].size())
    {
        m_decor[1][objet.getPosition().y][objet.getPosition().x].ajouterObjet(objet);
    }
}

void Map::setVolumeMusique(int volume)
{
    for(int i=0;i<MAX_MUSIQUE;++i)
        m_musique[i].SetVolume(volume);
    /*if(m_musiqueEnCours>=0&&m_musiqueEnCours<MAX_MUSIQUE)
        m_musique[m_musiqueEnCours].SetVolume(volume);*/
}

coordonnee Map::getSacPointe(){return m_sacPointe;}
int Map::getObjetPointe(){return m_objetPointe;}
int Map::getNombreObjets(coordonnee position)
{
    if(position.x>=0&&position.x<m_decor[0][0].size()
     &&position.y>=0&&position.y<m_decor[0].size())
        return m_decor[1][position.y][position.x].getNombreObjets();
    else
        return 0;
}

coordonnee Map::getPositionMonstre(int numeroMonstre)
{
    if(numeroMonstre>=0&&numeroMonstre<m_monstre.size())
        return m_monstre[numeroMonstre].getCoordonnee();
}

Monstre *Map::getEntiteMonstre(int numeroMonstre)
{
    if(numeroMonstre>=0&&numeroMonstre<m_monstre.size())
        return &m_monstre[numeroMonstre];
    else
    {
        return NULL;
    }
}

vector<vector<bool> > Map::getAlentourDuPersonnage(coordonnee positionPersonnage)
{
	vector<vector<bool> > grille(20,vector<bool>(20,0));

	for(int y=positionPersonnage.y-10;y<positionPersonnage.y+10;y++)
		for(int x=positionPersonnage.x-10;x<positionPersonnage.x+10;x++)
			if(y>=0&&x>=0&&x<m_decor[0][0].size()&&y<m_decor[0].size())
				grille[y-positionPersonnage.y+10][x-positionPersonnage.x+10]=getCollision(x,y);
			else
				grille[y-positionPersonnage.y+10][x-positionPersonnage.x+10]=1;

	return grille;
}

int Map::getMonstreIllumine(){return m_monstreIllumine;}

coordonnee Map::getDimensions()
{
	coordonnee dimensions;

	dimensions.x=m_decor[0][0].size();
	dimensions.y=m_decor[0].size();

	return dimensions;
}

int Map::getEvenement(coordonnee casePointee)
{
    int temp=-1;
    if(casePointee.y>=0&&casePointee.y<m_decor[0].size()&&casePointee.x>=0&&casePointee.x<m_decor[0][0].size())
        for(int i=0;i<2;i++)
            if(m_decor[i][casePointee.y][casePointee.x].getEvenement().size()>0)
                if(m_decor[i][casePointee.y][casePointee.x].getEvenement()[0]>-1)
                    temp=m_decor[i][casePointee.y][casePointee.x].getEvenement()[0];

    return temp;
}

bool Map::getCollision(int positionX,int positionY)
{
	for(int i=0;i<2;i++)
	{
        if(m_decor[i][positionY][positionX].getTileset()>=0&&m_decor[i][positionY][positionX].getTileset()<m_tileset.size())
            if(m_tileset[m_decor[i][positionY][positionX].getTileset()].getCollisionTile(m_decor[i][positionY][positionX].getTile()))
                return 1;

         if(m_decor[i][positionY][positionX].getMonstre()>-1&&m_decor[i][positionY][positionX].getMonstre()<m_monstre.size())
                if(m_monstre[m_decor[i][positionY][positionX].getMonstre()].enVie())
                    return 1;

        coordonnee enCours;

        enCours.x=positionX+1;
        enCours.y=positionY+1;
        if(enCours.x>=0&&enCours.y>=0&&enCours.x<m_decor[0][0].size()&&enCours.y<m_decor[0].size())
            if(m_decor[i][ enCours.y][enCours.x].getMonstre()>-1&&m_decor[i][ enCours.y][enCours.x].getMonstre()<m_monstre.size())
                if(m_monstre[m_decor[i][ enCours.y][enCours.x].getMonstre()].enVie()&&m_monstre[m_decor[i][ enCours.y][enCours.x].getMonstre()].getCaracteristique().vitesse>0)
                    if(m_decor[i][enCours.y][enCours.x].getMonstre()<m_monstre.size())
                        if(m_monstre[m_decor[i][enCours.y][enCours.x].getMonstre()].getProchaineCase().x==positionX&&m_monstre[m_decor[i][enCours.y][enCours.x].getMonstre()].getProchaineCase().y==positionY)
                            return 1;
        enCours.x=positionX-1;
        enCours.y=positionY-1;
        if(enCours.x>=0&&enCours.y>=0&&enCours.x<m_decor[0][0].size()&&enCours.y<m_decor[0].size())
            if(m_decor[i][ enCours.y][enCours.x].getMonstre()>-1&&m_decor[i][ enCours.y][enCours.x].getMonstre()<m_monstre.size())
                if(m_monstre[m_decor[i][ enCours.y][enCours.x].getMonstre()].enVie()&&m_monstre[m_decor[i][ enCours.y][enCours.x].getMonstre()].getCaracteristique().vitesse>0)
                    if(m_decor[i][enCours.y][enCours.x].getMonstre()<m_monstre.size())
                        if(m_monstre[m_decor[i][enCours.y][enCours.x].getMonstre()].getProchaineCase().x==positionX&&m_monstre[m_decor[i][enCours.y][enCours.x].getMonstre()].getProchaineCase().y==positionY)
                            return 1;
        enCours.x=positionX+1;
        enCours.y=positionY-1;
        if(enCours.x>=0&&enCours.y>=0&&enCours.x<m_decor[0][0].size()&&enCours.y<m_decor[0].size())
            if(m_decor[i][ enCours.y][enCours.x].getMonstre()>-1&&m_decor[i][ enCours.y][enCours.x].getMonstre()<m_monstre.size())
                if(m_monstre[m_decor[i][ enCours.y][enCours.x].getMonstre()].enVie()&&m_monstre[m_decor[i][ enCours.y][enCours.x].getMonstre()].getCaracteristique().vitesse>0)
                    if(m_decor[i][enCours.y][enCours.x].getMonstre()<m_monstre.size())
                        if(m_monstre[m_decor[i][enCours.y][enCours.x].getMonstre()].getProchaineCase().x==positionX&&m_monstre[m_decor[i][enCours.y][enCours.x].getMonstre()].getProchaineCase().y==positionY)
                            return 1;
        enCours.x=positionX-1;
        enCours.y=positionY+1;
        if(enCours.x>=0&&enCours.y>=0&&enCours.x<m_decor[0][0].size()&&enCours.y<m_decor[0].size())
            if(m_decor[i][ enCours.y][enCours.x].getMonstre()>-1&&m_decor[i][ enCours.y][enCours.x].getMonstre()<m_monstre.size())
                if(m_monstre[m_decor[i][ enCours.y][enCours.x].getMonstre()].enVie()&&m_monstre[m_decor[i][ enCours.y][enCours.x].getMonstre()].getCaracteristique().vitesse>0)
                    if(m_decor[i][enCours.y][enCours.x].getMonstre()<m_monstre.size())
                        if(m_monstre[m_decor[i][enCours.y][enCours.x].getMonstre()].getProchaineCase().x==positionX&&m_monstre[m_decor[i][enCours.y][enCours.x].getMonstre()].getProchaineCase().y==positionY)
                            return 1;
            enCours.x=positionX;
        enCours.y=positionY+1;
        if(enCours.x>=0&&enCours.y>=0&&enCours.x<m_decor[0][0].size()&&enCours.y<m_decor[0].size())
            if(m_decor[i][ enCours.y][enCours.x].getMonstre()>-1&&m_decor[i][ enCours.y][enCours.x].getMonstre()<m_monstre.size())
                if(m_monstre[m_decor[i][ enCours.y][enCours.x].getMonstre()].enVie()&&m_monstre[m_decor[i][ enCours.y][enCours.x].getMonstre()].getCaracteristique().vitesse>0)
                    if(m_decor[i][enCours.y][enCours.x].getMonstre()<m_monstre.size())
                        if(m_monstre[m_decor[i][enCours.y][enCours.x].getMonstre()].getProchaineCase().x==positionX&&m_monstre[m_decor[i][enCours.y][enCours.x].getMonstre()].getProchaineCase().y==positionY)
                            return 1;
        enCours.x=positionX+1;
        enCours.y=positionY;
        if(enCours.x>=0&&enCours.y>=0&&enCours.x<m_decor[0][0].size()&&enCours.y<m_decor[0].size())
            if(m_decor[i][ enCours.y][enCours.x].getMonstre()>-1&&m_decor[i][ enCours.y][enCours.x].getMonstre()<m_monstre.size())
                if(m_monstre[m_decor[i][ enCours.y][enCours.x].getMonstre()].enVie()&&m_monstre[m_decor[i][ enCours.y][enCours.x].getMonstre()].getCaracteristique().vitesse>0)
                    if(m_decor[i][enCours.y][enCours.x].getMonstre()<m_monstre.size())
                        if(m_monstre[m_decor[i][enCours.y][enCours.x].getMonstre()].getProchaineCase().x==positionX&&m_monstre[m_decor[i][enCours.y][enCours.x].getMonstre()].getProchaineCase().y==positionY)
                            return 1;
        enCours.x=positionX-1;
        enCours.y=positionY;
        if(enCours.x>=0&&enCours.y>=0&&enCours.x<m_decor[0][0].size()&&enCours.y<m_decor[0].size())
            if(m_decor[i][ enCours.y][enCours.x].getMonstre()>-1&&m_decor[i][ enCours.y][enCours.x].getMonstre()<m_monstre.size())
                if(m_monstre[m_decor[i][ enCours.y][enCours.x].getMonstre()].enVie()&&m_monstre[m_decor[i][ enCours.y][enCours.x].getMonstre()].getCaracteristique().vitesse>0)
                    if(m_decor[i][enCours.y][enCours.x].getMonstre()<m_monstre.size())
                        if(m_monstre[m_decor[i][enCours.y][enCours.x].getMonstre()].getProchaineCase().x==positionX&&m_monstre[m_decor[i][enCours.y][enCours.x].getMonstre()].getProchaineCase().y==positionY)
                            return 1;
        enCours.x=positionX;
        enCours.y=positionY-1;
        if(enCours.x>=0&&enCours.y>=0&&enCours.x<m_decor[0][0].size()&&enCours.y<m_decor[0].size())
            if(m_decor[i][ enCours.y][enCours.x].getMonstre()>-1&&m_decor[i][ enCours.y][enCours.x].getMonstre()<m_monstre.size())
                if(m_monstre[m_decor[i][ enCours.y][enCours.x].getMonstre()].enVie()&&m_monstre[m_decor[i][ enCours.y][enCours.x].getMonstre()].getCaracteristique().vitesse>0)
                    if(m_decor[i][enCours.y][enCours.x].getMonstre()<m_monstre.size())
                        if(m_monstre[m_decor[i][enCours.y][enCours.x].getMonstre()].getProchaineCase().x==positionX&&m_monstre[m_decor[i][enCours.y][enCours.x].getMonstre()].getProchaineCase().y==positionY)
                            return 1;

	}
	return 0;
}

int Map::getTypeCase(int positionX,int positionY)
{
	for(int i=0;i<2;i++)
	{
	    if(positionY>=0&&positionY<m_decor[0].size()&&positionX>=0&&positionX<m_decor[0][0].size())
	    {
            if(m_decor[i][positionY][positionX].getTileset()>=0&&m_decor[i][positionY][positionX].getTileset()<m_tileset.size())
                if(m_tileset[m_decor[i][positionY][positionX].getTileset()].getCollisionTile(m_decor[i][positionY][positionX].getTile()))
                    return 1;

            if(m_decor[i][positionY][positionX].getMonstre()>-1&&m_decor[i][positionY][positionX].getMonstre()<m_monstre.size())
                if(m_monstre[m_decor[i][positionY][positionX].getMonstre()].enVie())
                    return 2;

            if(m_decor[i][positionY][positionX].getNombreObjets())
                return 4;

            for(int z=0;z<m_decor[i][positionY][positionX].getEvenement().size();z++)
                if(m_decor[i][positionY][positionX].getEvenement()[z]>=0&&m_decor[i][positionY][positionX].getEvenement()[z]<m_evenement.size())
                    if(m_evenement[m_decor[i][positionY][positionX].getEvenement()[z]].getType()==CHANGEMENT_DE_MAP)
                        return 3;
	    }
	    else
	    return 1;
	}

	return 0;
}

int Map::getMonstre(Hero *hero,View *camera,RenderWindow *ecran,coordonnee positionSouris,coordonnee casePointee)
{
    float distance=100000;
    int meilleur=-1;
    coordonnee vueMin,vueMax;

     const sf::FloatRect& ViewRect = camera->GetRect();

    vueMin.x=casePointee.x-2;
    vueMin.y=casePointee.y-2;
    vueMax.x=casePointee.x+2;
    vueMax.y=casePointee.y+2;

    if(vueMin.x<0) { vueMin.x=0; }
    if(vueMin.y<0) { vueMin.y=0; }
    if(vueMax.x>m_decor[0][0].size()) { vueMax.x=m_decor[0][0].size(); }
    if(vueMax.y>m_decor[0].size()) { vueMax.y=m_decor[0].size(); }

	for(int i=0;i<2;i++)
        for(int j=vueMin.y;j<vueMax.y;j++)
            for(int k=vueMin.x;k<vueMax.x;k++)
                    if(m_decor[i][j][k].getMonstre()>=0&&m_decor[i][j][k].getMonstre()<m_monstre.size())
                    if(m_monstre[m_decor[i][j][k].getMonstre()].enVie())
                    {
                        coordonneeDecimal temp;
                        temp.x=((float)m_monstre[m_decor[i][j][k].getMonstre()].getCoordonneePixel().x*(float)DIVISEUR_COTE_TILE-(float)m_monstre[m_decor[i][j][k].getMonstre()].getCoordonneePixel().y*DIVISEUR_COTE_TILE-1+(float)m_decor[1].size())*64;
                        temp.y=(float)(m_monstre[m_decor[i][j][k].getMonstre()].getCoordonneePixel().x*(float)DIVISEUR_COTE_TILE+(float)m_monstre[m_decor[i][j][k].getMonstre()].getCoordonneePixel().y*DIVISEUR_COTE_TILE)*32;
                        coordonnee positionSourisTotale;
                        positionSourisTotale.x=(int)ecran->ConvertCoords(ecran->GetInput().GetMouseX(),ecran->GetInput().GetMouseY()).x;
                        positionSourisTotale.y=(int)ecran->ConvertCoords(ecran->GetInput().GetMouseX(), ecran->GetInput().GetMouseY()).y;
                        //if(positionSourisTotale.x>

                        if(positionSourisTotale.x>temp.x-m_ModeleMonstre[m_monstre[m_decor[i][j][k].getMonstre()].getModele()].m_pose[m_monstre[m_decor[i][j][k].getMonstre()].getEtat()][(int)(m_monstre[m_decor[i][j][k].getMonstre()].getAngle()/45)][m_monstre[m_decor[i][j][k].getMonstre()].getPose()].getCoordonnee().w*0/128
                        &&positionSourisTotale.x<temp.x+m_ModeleMonstre[m_monstre[m_decor[i][j][k].getMonstre()].getModele()].m_pose[m_monstre[m_decor[i][j][k].getMonstre()].getEtat()][(int)(m_monstre[m_decor[i][j][k].getMonstre()].getAngle()/45)][m_monstre[m_decor[i][j][k].getMonstre()].getPose()].getCoordonnee().w*128/128
                        &&positionSourisTotale.y>temp.y-m_ModeleMonstre[m_monstre[m_decor[i][j][k].getMonstre()].getModele()].m_pose[m_monstre[m_decor[i][j][k].getMonstre()].getEtat()][(int)(m_monstre[m_decor[i][j][k].getMonstre()].getAngle()/45)][m_monstre[m_decor[i][j][k].getMonstre()].getPose()].getCoordonnee().h*48/128
                        &&positionSourisTotale.y<temp.y+64*m_ModeleMonstre[m_monstre[m_decor[i][j][k].getMonstre()].getModele()].m_pose[m_monstre[m_decor[i][j][k].getMonstre()].getEtat()][(int)(m_monstre[m_decor[i][j][k].getMonstre()].getAngle()/45)][m_monstre[m_decor[i][j][k].getMonstre()].getPose()].getCoordonnee().h/128)
                        {
                            float temp2=0;
                            temp2=gpl::sqrt((temp.x-(positionSourisTotale.x-64))
                            *(temp.x-(positionSourisTotale.x-64))
                            +(temp.y-(positionSourisTotale.y))
                            *(temp.y-(positionSourisTotale.y)));

                            if(distance>temp2)
                                meilleur=m_decor[i][j][k].getMonstre(),distance=temp2;
                        }
                    }

                    m_monstreIllumine=meilleur;
                    return meilleur;
}
