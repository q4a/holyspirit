#include "Map.h"

#include <iostream>
#include <fstream>
#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>


using namespace std;
using namespace sf;

#include "Globale.h"


Map::Map()
{
    carreBrun.SetSmooth(true);
    carreBleu.SetSmooth(true);
    carreRouge.SetSmooth(true);
    carreVert.SetSmooth(true);
    carreBrun.Create(8*configuration.Resolution.x/800, 8*configuration.Resolution.x/800, Color(128, 64, 0)),carreBleu.Create(8*configuration.Resolution.x/800, 8*configuration.Resolution.x/800, Color(32, 0, 128)),carreRouge.Create(8*configuration.Resolution.x/800, 8*configuration.Resolution.x/800, Color(128, 0, 0)),carreVert.Create(8*configuration.Resolution.x/800, 8*configuration.Resolution.x/800, Color(0, 128, 0));


    if (sf::PostFX::CanUsePostFX() == true&&configuration.postFX)
    {
        lumiereMask.LoadFromFile("Data/Menus/lumiereMask.png");
    }
}

Map::~Map()
{
    m_tileset.clear();
    m_herbe.clear();
    for(int i=0;i<m_decor.size();i++)
    {
        for(int j=0;j<m_decor[0].size();j++)
        {
            m_decor[i][j].clear();
        }
        m_decor[i].clear();
    }
    m_decor.clear();
    m_ModeleMonstre.clear();
    m_monstre.clear();
    for(int i=0;i<m_evenement.size();i++)
        m_evenement[i].deleteInformations();
    m_evenement.clear();
    m_musique.Stop();
}

void Map::Detruire()
{
    m_tileset.clear();
    m_herbe.clear();
    for(int i=0;i<m_decor.size();i++)
    {
        for(int j=0;j<m_decor[0].size();j++)
        {
            m_decor[i][j].clear();
        }
        m_decor[i].clear();
    }
    m_decor.clear();
    m_ModeleMonstre.clear();
    m_monstre.clear();
    for(int i=0;i<m_evenement.size();i++)
        m_evenement[i].deleteInformations();
    m_evenement.clear();
    m_musique.Stop();
}

bool Map::Charger(int numeroMap)
{
	char numero[7];
	string chemin = configuration.chemin_maps;

	sprintf(numero,"%ld.txt",numeroMap);
	chemin += numero;

	console.Ajouter("",0);
	console.Ajouter("Chargement de la map : "+chemin,0);

	m_lumiere[0].intensite=0;
	m_lumiere[0].rouge=0;
	m_lumiere[0].vert=0;
	m_lumiere[0].bleu=0;

	ifstream fichier;
    fichier.open(chemin.c_str(), ios::in);
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

    		if(fichier.eof()){ char temp[1000]; sprintf(temp,"Erreur : Map \" %s \" Invalide",chemin.c_str());console.Ajouter(temp,1); return 0; }
    	}while(caractere!='$');
    	do
    	{
    	    //Chargement des musiques
    		fichier.get(caractere);
    		if(caractere=='*')
    		{
    			string cheminDeLaMusique;
                getline(fichier, cheminDeLaMusique);
                if(!m_musique.OpenFromFile(cheminDeLaMusique.c_str()))
                    console.Ajouter("Impossible de charger : "+cheminDeLaMusique,1);
                else
                console.Ajouter("Chargement de : "+cheminDeLaMusique,0);

                m_musique.SetLoop(true);
                m_musique.SetVolume(0);
    		}
    		if(fichier.eof()){ char temp[1000]; sprintf(temp,"Erreur : Map \" %s \" Invalide",chemin.c_str()); throw (&temp); }
    	}while(caractere!='$');
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
    		if(fichier.eof()){ char temp[1000]; sprintf(temp,"Erreur : Map \" %s \" Invalide",chemin.c_str());console.Ajouter(temp,1); return 0; }
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
    		if(fichier.eof()){ char temp[1000]; sprintf(temp,"Erreur : Map \" %s \" Invalide",chemin.c_str());console.Ajouter(temp,1); return 0; }

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
    		if(fichier.eof()){ char temp[1000]; sprintf(temp,"Erreur : Map \" %s \" Invalide",chemin.c_str());console.Ajouter(temp,1); return 0; }

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
                //AjouterTileSet(cheminDuTileset);
                //tilesetTemp.Charger(cheminDuTileset);
                m_ModeleMonstre.push_back(monstreModeleTemp);
                if(!m_ModeleMonstre[m_ModeleMonstre.size()-1].Charger(cheminDuMonstre))
                    return 0;
                else
                console.Ajouter("Chargement de : "+cheminDuMonstre,0);

    		}
    		if(fichier.eof()){ char temp[1000]; sprintf(temp,"Erreur : Map \" %s \" Invalide",chemin.c_str());console.Ajouter(temp,1); return 0; }
    	}while(caractere!='$');


    	Monstre monstreTemp;
    	do
    	{
    	    //Chargement des tileset
    		fichier.get(caractere);
    		if(caractere=='*')
    		{
    		    int numeroMonstre=0;
    		    do
    			{
    				fichier.get(caractere);
    				switch (caractere)
    				{
    					case 'm': fichier>>numeroMonstre; break;
    				}
    			}while(caractere!='$');
                m_monstre.push_back(monstreTemp);
                m_monstre[m_monstre.size()-1].Charger(numeroMonstre,&m_ModeleMonstre[numeroMonstre]);
                fichier.get(caractere);
    		}
    		if(fichier.eof()){ char temp[1000]; sprintf(temp,"Erreur : Map \" %s \" Invalide",chemin.c_str());console.Ajouter(temp,1); return 0; }

    	}while(caractere!='$');


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
    				if(fichier.eof()){ char temp[1000]; sprintf(temp,"Erreur : Map \" %s \" Invalide",chemin.c_str());console.Ajouter(temp,1); return 0; }
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
    				if(fichier.eof()){ char temp[1000]; sprintf(temp,"Erreur : Map \" %s \" Invalide",chemin.c_str());console.Ajouter(temp,1); return 0; }
    			}while(caractere!='$');

    			fichier.get(caractere);
    		}
    		if(fichier.eof()){ char temp[1000]; sprintf(temp,"Erreur : Map \" %s \" Invalide",chemin.c_str());console.Ajouter(temp,1); return 0; }
    	}while(caractere!='$');


    	Decor decorTemp(-1,-1,-1,-1,-1);

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
                        int tileset=-1,tileFinal=-1,evenement=-1,monstre=-1,herbe=-1;
                        vector <int>tile;
                        do
                        {
                            fichier.get(caractere);
                            switch (caractere)
                            {
                                case 's': fichier>>tileset; break;
                                case 't': int temp; fichier>>temp; tile.push_back(temp); break;
                                case 'e': fichier>>evenement; break;
                                case 'm': fichier>>monstre; break;
                                case 'h': fichier>>herbe; break;
                            }
                            if(fichier.eof()){ char temp[1000]; sprintf(temp,"Erreur : Map \" %s \" Invalide",chemin.c_str());console.Ajouter(temp,1); return 0; }
                        }while(caractere!='|');
                        //AjouterDecor(tileset,tile,evenement,position,couche,monstre);
                        //decorTemp2.setDecor(tileset,tile,evenement,monstre);

                        if(couche==1)
                            if(m_decor[0][position.y][position.x].getHerbe()>=0&&herbe<0)
                                herbe=m_decor[0][position.y][position.x].getHerbe();

                        if(monstre>=0&&monstre<m_monstre.size())
                        m_monstre[monstre].setCoordonnee(position);

                        if(tile.size()>0)
                        {
                            int random = (rand() % (tile.size() -1 - 0 + 1)) + 0;
                            if(random>=0&&random<tile.size())
                                tileFinal = tile[random];

                            tile.clear();
                        }

                        if(fichier.eof()){ char temp[1000]; sprintf(temp,"Erreur : Map \" %s \" Invalide",chemin.c_str());console.Ajouter(temp,1); return 0; }


                        m_decor[couche][position.y].push_back(decorTemp);
                        m_decor[couche][position.y][position.x].setDecor(tileset,tileFinal,evenement,monstre,herbe);
                        tileset=-1,tile.clear(),tileFinal=-1,evenement=-1,monstre=-1,herbe=-1;
                        position.x++;
                        fichier.get(caractere);
                    }while(caractere!='\n');
                    position.x=0;
                    position.y++;
                }
                if(fichier.eof()){ char temp[1000]; sprintf(temp,"Erreur : Map \" %s \" Invalide",chemin.c_str());console.Ajouter(temp,1); return 0; }

            }while(caractere!='$');

    	}
    }
    else
    {
        console.Ajouter("Impossible d'ouvrir le fichier : "+chemin,1);
        return 0;
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
                        int numeroHerbe = (rand() % (m_herbe[m_decor[couche][i][j].getHerbe()].getTaille() -1 - 0 + 1)) + 0;
                        m_decor[couche][i][j].setNumeroHerbe(numeroHerbe);
                    }
            }

    return 1;
}


void Map::calculerOmbresEtLumieres(sf::RenderWindow* ecran,Hero *hero,sf::View *camera)
{
    m_lumiereHero.detruire();

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
    if(lampesMax.x>m_decor[0][0].size()) { lampesMax.x=m_decor[0][0].size(); }
    if(lampesMax.y>m_decor[0].size()) { lampesMax.y=m_decor[0].size(); }


    //Calcul des tile qui peuvent se trouver les plus loin du perso, tout en restant dans le champs de vision
    vueMin.x=hero->m_personnage.getCoordonnee().x-15;
    vueMin.y=hero->m_personnage.getCoordonnee().y-15;
    vueMax.x=hero->m_personnage.getCoordonnee().x+15;
    vueMax.y=hero->m_personnage.getCoordonnee().y+15;

    //Vérification que ces tiles soit bien dans la map
    if(vueMin.x<0) { vueMin.x=0; }
    if(vueMin.y<0) { vueMin.y=0; }
    if(vueMax.x>m_decor[0][0].size()) { vueMax.x=m_decor[0][0].size(); }
    if(vueMax.y>m_decor[0].size()) { vueMax.y=m_decor[0].size(); }
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

    m_lumiereHero=hero->m_modelePersonnage.getPorteeLumineuse();

    m_lumiereHero.rouge=(lumiereMap.rouge*lumiereMap.intensite+m_lumiereHero.rouge*m_lumiereHero.intensite)/(m_lumiereHero.intensite+lumiereMap.intensite);
    m_lumiereHero.vert=(lumiereMap.vert*lumiereMap.intensite+m_lumiereHero.vert*m_lumiereHero.intensite)/(m_lumiereHero.intensite+lumiereMap.intensite);
    m_lumiereHero.bleu=(lumiereMap.bleu*lumiereMap.intensite+m_lumiereHero.bleu*m_lumiereHero.intensite)/(m_lumiereHero.intensite+lumiereMap.intensite);

    m_lumiereHero.intensite+=lumiereMap.intensite;
    if(m_lumiereHero.intensite>255) m_lumiereHero.intensite=255;
    if(m_lumiereHero.intensite<0) m_lumiereHero.intensite=0;

    if(configuration.Ombre)
        m_lumiereHero.AjouterOmbre(lumiereMap.intensite/4,angleOmbreMap,lumiereMap.hauteur);


        // Je fonctionne comme pour en bas, juste que je prend le héro comme source de lumière, pour la portée lumineuse
    for(int i=0;i<30;i++)
        for(int j=0;j<30;j++)
        {
            m_tableauDesLampes[i][j].detruire();
             m_tableauDesLampes[i][j]=lumiereMap;
            if(configuration.Ombre)
                m_tableauDesLampes[i][j].AjouterOmbre((int)((float)lumiereMap.intensite*0.25),angleOmbreMap,lumiereMap.hauteur);

            if(i>=0&&i<vueMax.y-vueMin.y&&j>=0&&j<vueMax.x-vueMin.x)
            {

                lumiereTemp=0;
                lumiereTemp=(float)hero->m_modelePersonnage.getPorteeLumineuse().intensite-((((float)gpl::sqrt((hero->m_personnage.getCoordonneePixel().x-(j+vueMin.x)*COTE_TILE)*(hero->m_personnage.getCoordonneePixel().x-(j+vueMin.x)*COTE_TILE)+(hero->m_personnage.getCoordonneePixel().y-(i+vueMin.y)*COTE_TILE)*(hero->m_personnage.getCoordonneePixel().y-(i+vueMin.y)*COTE_TILE))))*DIVISEUR_COTE_TILE)*30;

                lumiere=hero->m_modelePersonnage.getPorteeLumineuse();

                lumiere.intensite=(int)lumiereTemp;

                float lumiereTemp2=0;

                 bool aMoitieSombre=true;

                lumiereTemp=0;
                lumiereTemp2=0;
                if(lumiere.intensite>0)
                    for(int o=1;o<10;o++)
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

                   // if(!m_decor[1][i+vueMin.y][j+vueMin.x].getTileset()>0)
                    //if(m_tileset[m_decor[1][i+vueMin.y][j+vueMin.x].getTileset()].getLumiereDuTile(m_decor[1][i+vueMin.y][j+vueMin.x].getTile()).intensite==0)
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
                        m_lumiereHero.intensite+=lumiere.intensite;

                    if(lumiere.intensite>0)
                    {
                        m_lumiereHero.rouge=(lumiere.rouge*lumiere.intensite/(m_lumiereHero.intensite+lumiere.intensite)+m_lumiereHero.rouge*m_lumiereHero.intensite/(m_lumiereHero.intensite+lumiere.intensite));
                        m_lumiereHero.vert=(lumiere.vert*lumiere.intensite/(m_lumiereHero.intensite+lumiere.intensite)+m_lumiereHero.vert*m_lumiereHero.intensite/(m_lumiereHero.intensite+lumiere.intensite));
                        m_lumiereHero.bleu=(lumiere.bleu*lumiere.intensite/(m_lumiereHero.intensite+lumiere.intensite)+m_lumiereHero.bleu*m_lumiereHero.intensite/(m_lumiereHero.intensite+lumiere.intensite));

                    }

                    if(m_lumiereHero.intensite>255)
                        m_lumiereHero.intensite=255;
                    if(m_lumiereHero.intensite<0)
                        m_lumiereHero.intensite=0;
                    if(m_lumiereHero.rouge>255)
                        m_lumiereHero.rouge=255;
                    if(m_lumiereHero.vert>255)
                        m_lumiereHero.vert=255;
                    if(m_lumiereHero.bleu>255)
                        m_lumiereHero.bleu=255;
                }

                if(configuration.Ombre)
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
                }
            }
        }






        for(int couche=1;couche==1;couche++)
        {
            for(int j=lampesMin.y;j<lampesMax.y;j++)
            {
                for(int k=lampesMin.x;k<lampesMax.x;k++)
                {
                    bool ok=false;
                    if(m_decor[0][j][k].getMonstre()>=0&&m_decor[couche][j][k].getMonstre()<m_monstre.size())// Je vérifie que le tile est initialisé ^^
                        ok=true;
                    if(m_decor[0][j][k].getEvenement()>=0&&m_decor[couche][j][k].getEvenement()<m_evenement.size())// Je vérifie que le tile est initialisé ^^
                        if(m_evenement[m_decor[0][j][k].getEvenement()].getType()==LUMIERE)
                            ok=true;
                    if(m_decor[1][j][k].getMonstre()>=0&&m_decor[couche][j][k].getMonstre()<m_monstre.size())// Je vérifie que le tile est initialisé ^^
                        ok=true;
                    if(m_decor[1][j][k].getEvenement()>=0&&m_decor[couche][j][k].getEvenement()<m_evenement.size())// Je vérifie que le tile est initialisé ^^
                        if(m_evenement[m_decor[1][j][k].getEvenement()].getType()==LUMIERE)
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

                        if(m_decor[0][j][k].getEvenement()>=0&&m_decor[0][j][k].getEvenement()<m_evenement.size())// Je vérifie que le tile est initialisé ^^
                            if(m_evenement[m_decor[0][j][k].getEvenement()].getType()==LUMIERE)
                            {
                                lumiereTile.rouge=(lumiereTile.rouge*lumiereTile.intensite+m_evenement[m_decor[0][j][k].getEvenement()].getInformation(0)*m_evenement[m_decor[0][j][k].getEvenement()].getInformation(3))/(m_evenement[m_decor[0][j][k].getEvenement()].getInformation(3)+lumiereTile.intensite);
                                lumiereTile.vert=(lumiereTile.vert*lumiereTile.intensite+m_evenement[m_decor[0][j][k].getEvenement()].getInformation(1)*m_evenement[m_decor[0][j][k].getEvenement()].getInformation(3))/(m_evenement[m_decor[0][j][k].getEvenement()].getInformation(3)+lumiereTile.intensite);
                                lumiereTile.bleu=(lumiereTile.bleu*lumiereTile.intensite+m_evenement[m_decor[0][j][k].getEvenement()].getInformation(2)*m_evenement[m_decor[0][j][k].getEvenement()].getInformation(3))/(m_evenement[m_decor[0][j][k].getEvenement()].getInformation(3)+lumiereTile.intensite);

                                lumiereTile.hauteur=(lumiereTile.hauteur*lumiereTile.intensite+m_evenement[m_decor[0][j][k].getEvenement()].getInformation(4)*m_evenement[m_decor[0][j][k].getEvenement()].getInformation(3))/(m_evenement[m_decor[0][j][k].getEvenement()].getInformation(3)+lumiereTile.intensite);

                                lumiereTile.intensite+=m_evenement[m_decor[0][j][k].getEvenement()].getInformation(3);
                                if(lumiereTile.intensite>255)
                                    lumiereTile.intensite=255;
                                if(lumiereTile.intensite<0)
                                    lumiereTile.intensite=0;
                            }
                        if(m_decor[1][j][k].getEvenement()>=0&&m_decor[1][j][k].getEvenement()<m_evenement.size())// Je vérifie que le tile est initialisé ^^
                            if(m_evenement[m_decor[1][j][k].getEvenement()].getType()==LUMIERE)
                            {
                                lumiereTile.rouge=(lumiereTile.rouge*lumiereTile.intensite+m_evenement[m_decor[1][j][k].getEvenement()].getInformation(0)*m_evenement[m_decor[1][j][k].getEvenement()].getInformation(3))/(m_evenement[m_decor[1][j][k].getEvenement()].getInformation(3)+lumiereTile.intensite);
                                lumiereTile.vert=(lumiereTile.vert*lumiereTile.intensite+m_evenement[m_decor[1][j][k].getEvenement()].getInformation(1)*m_evenement[m_decor[1][j][k].getEvenement()].getInformation(3))/(m_evenement[m_decor[1][j][k].getEvenement()].getInformation(3)+lumiereTile.intensite);
                                lumiereTile.bleu=(lumiereTile.bleu*lumiereTile.intensite+m_evenement[m_decor[1][j][k].getEvenement()].getInformation(2)*m_evenement[m_decor[1][j][k].getEvenement()].getInformation(3))/(m_evenement[m_decor[1][j][k].getEvenement()].getInformation(3)+lumiereTile.intensite);

                                lumiereTile.hauteur=(lumiereTile.hauteur*lumiereTile.intensite+m_evenement[m_decor[1][j][k].getEvenement()].getInformation(4)*m_evenement[m_decor[1][j][k].getEvenement()].getInformation(3))/(m_evenement[m_decor[1][j][k].getEvenement()].getInformation(3)+lumiereTile.intensite);

                                lumiereTile.intensite+=m_evenement[m_decor[1][j][k].getEvenement()].getInformation(3);
                                if(lumiereTile.intensite>255)
                                    lumiereTile.intensite=255;
                                if(lumiereTile.intensite<0)
                                    lumiereTile.intensite=0;
                            }


                        //Maintenant, je vais calculer la lumière à ajouter à tout les tiles affiché à l'écran
                        for(int l=vueMin.y;l<vueMax.y;l++)
                            for(int m=vueMin.x;m<vueMax.x;m++)
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
                                    ||position.x+positionPartieDecor.w>=ViewRect.Left&&position.x<ViewRect.Right&&position.y+positionPartieDecor.h>=ViewRect.Top&&position.y-positionPartieDecor.h+64<ViewRect.Bottom)
                                  //  if(position.x>=ViewRect.Left-384&&position.x<ViewRect.Right+384&&position.y>=ViewRect.Top-384&&position.y<ViewRect.Bottom+384)// Je test si le tile va êtrz affiché à l'écran ou pas
                                    {

                                    //lumiere=m_lumiere;
                                    lumiere.intensite=0;
                                    lumiereTemp=0;
                                    // Je réduit la quantité de lumière reçue, en fonction de la distance, qui est calculée avec un simple pythagore ^^
                                    lumiereTemp=(float)lumiereTile.intensite-(float)(gpl::sqrt((k*COTE_TILE-m*COTE_TILE)*(k*COTE_TILE-m*COTE_TILE)+(j*COTE_TILE-l*COTE_TILE)*(j*COTE_TILE-l*COTE_TILE)))/5;
                                    lumiere.hauteur=lumiereTile.hauteur;

                                    if(lumiereTemp>0)
                                    lumiere.intensite=(int)lumiereTemp;
                                    else
                                    lumiere.intensite=0;

                                    //Je vais tester s'il y a un obstacle entre la source de lumière et le tile
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
                                        }

                                        // J'applique la même chose au héro
                                        if(l==hero->m_personnage.getCoordonnee().y&&m==hero->m_personnage.getCoordonnee().x)
                                        {
                                           m_lumiereHero.rouge=(lumiere.rouge*lumiere.intensite+m_lumiereHero.rouge*m_lumiereHero.intensite)/(m_lumiereHero.intensite+lumiere.intensite);
                                           m_lumiereHero.vert=(lumiere.vert*lumiere.intensite+m_lumiereHero.vert*m_lumiereHero.intensite)/(m_lumiereHero.intensite+lumiere.intensite);
                                           m_lumiereHero.bleu=(lumiere.bleu*lumiere.intensite+m_lumiereHero.bleu*m_lumiereHero.intensite)/(m_lumiereHero.intensite+lumiere.intensite);

                                            if(m_lumiereHero.rouge>255)
                                               m_lumiereHero.rouge=255;
                                            if(m_lumiereHero.vert>255)
                                               m_lumiereHero.vert=255;
                                            if(m_lumiereHero.bleu>255)
                                               m_lumiereHero.bleu=255;

                                            if(configuration.Ombre)
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
                                               m_lumiereHero.AjouterOmbre((int)lumiereTemp,(int)angle,lumiere.hauteur);
                                            }

                                            if(lumiere.intensite>0)
                                            {
                                                m_lumiereHero.intensite+=lumiere.intensite;

                                                if(m_lumiereHero.intensite>255)
                                                   m_lumiereHero.intensite=255;
                                                if(m_lumiereHero.intensite<0)
                                                   m_lumiereHero.intensite=0;
                                            }
                                        }
                                    }

                                        if(m_decor[0][j][k].getMonstre()>=0&&m_decor[0][j][k].getMonstre()<m_monstre.size())
                                        if(m_monstre[m_decor[0][j][k].getMonstre()].getCaracteristique().vie>0)
                                        {
                                            //lumiere=m_lumiere;
                                            lumiere.intensite=0;
                                            lumiereTemp=0;
                                            // Je réduit la quantité de lumière reçue, en fonction de la distance, qui est calculée avec un simple pythagore ^^
                                           // lumiereTemp=(float)lumiereTile.intensite-(float)(gpl::sqrt((k*COTE_TILE-m*COTE_TILE)*(k*COTE_TILE-m*COTE_TILE)+(j*COTE_TILE-l*COTE_TILE)*(j*COTE_TILE-l*COTE_TILE)))/5;
                                            lumiereTemp=(float)m_ModeleMonstre[m_monstre[m_decor[0][j][k].getMonstre()].getModele()].getPorteeLumineuse().intensite-((((float)gpl::sqrt((m_monstre[m_decor[0][j][k].getMonstre()].getCoordonneePixel().x-(m)*COTE_TILE)*(m_monstre[m_decor[0][j][k].getMonstre()].getCoordonneePixel().x-(m)*COTE_TILE)+(m_monstre[m_decor[0][j][k].getMonstre()].getCoordonneePixel().y-(l)*COTE_TILE)*(m_monstre[m_decor[0][j][k].getMonstre()].getCoordonneePixel().y-(l)*COTE_TILE))))*DIVISEUR_COTE_TILE)*15;
                                            lumiere.hauteur=m_ModeleMonstre[m_monstre[m_decor[0][j][k].getMonstre()].getModele()].getPorteeLumineuse().hauteur;

                                            if(lumiereTemp>0)
                                            lumiere.intensite=(int)lumiereTemp;
                                            else
                                            lumiere.intensite=0;

                                            lumiere.rouge=m_ModeleMonstre[m_monstre[m_decor[0][j][k].getMonstre()].getModele()].getPorteeLumineuse().rouge;
                                            lumiere.vert=m_ModeleMonstre[m_monstre[m_decor[0][j][k].getMonstre()].getModele()].getPorteeLumineuse().vert;
                                            lumiere.bleu=m_ModeleMonstre[m_monstre[m_decor[0][j][k].getMonstre()].getModele()].getPorteeLumineuse().bleu;
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
                                        }

                                        if(m_decor[1][j][k].getMonstre()>=0&&m_decor[1][j][k].getMonstre()<m_monstre.size())
                                        if(m_monstre[m_decor[1][j][k].getMonstre()].getCaracteristique().vie>0)
                                        {
                                            //lumiere=m_lumiere;
                                            lumiere.intensite=0;
                                            lumiereTemp=0;
                                            // Je réduit la quantité de lumière reçue, en fonction de la distance, qui est calculée avec un simple pythagore ^^
                                            lumiereTemp=(float)m_ModeleMonstre[m_monstre[m_decor[1][j][k].getMonstre()].getModele()].getPorteeLumineuse().intensite-((((float)gpl::sqrt((m_monstre[m_decor[1][j][k].getMonstre()].getCoordonneePixel().x-(m)*COTE_TILE)*(m_monstre[m_decor[1][j][k].getMonstre()].getCoordonneePixel().x-(m)*COTE_TILE)+(m_monstre[m_decor[1][j][k].getMonstre()].getCoordonneePixel().y-(l)*COTE_TILE)*(m_monstre[m_decor[1][j][k].getMonstre()].getCoordonneePixel().y-(l)*COTE_TILE))))*DIVISEUR_COTE_TILE)*15;
                                            lumiere.hauteur=m_ModeleMonstre[m_monstre[m_decor[1][j][k].getMonstre()].getModele()].getPorteeLumineuse().hauteur;

                                            if(lumiereTemp>0)
                                            lumiere.intensite=(int)lumiereTemp;
                                            else
                                            lumiere.intensite=0;

                                            if(lumiere.intensite>0)
                                            {
                                                lumiere.rouge=m_ModeleMonstre[m_monstre[m_decor[1][j][k].getMonstre()].getModele()].getPorteeLumineuse().rouge;
                                                lumiere.vert=m_ModeleMonstre[m_monstre[m_decor[1][j][k].getMonstre()].getModele()].getPorteeLumineuse().vert;
                                                lumiere.bleu=m_ModeleMonstre[m_monstre[m_decor[1][j][k].getMonstre()].getModele()].getPorteeLumineuse().bleu;
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
                                            }
                                        }
                                    }
                                }
                    }
                }
            }
        }

        if(configuration.amelioration_lampes)
        for(int z=0;z<1;z++)
            for(int i=0;i<vueMax.y-vueMin.y;i++)
            for(int j=0;j<vueMax.x-vueMin.x;j++)
            {
                for(int y=-1;y<=1;y++)
                    for(int w=-1;w<=1;w++)
                        if(!((y==0&&w==0)))
                        {
                            if(i+y>0&&i+y+vueMin.y<m_decor[0].size()&&j+w>0&&j+w+vueMin.x<m_decor[0][0].size())
                            if(i+y>0&&i+y<30&&j+w>0&&j+w<30)
                            {
                                if((double)m_tableauDesLampes[i+y][j+w].intensite<(double)m_tableauDesLampes[i][j].intensite/1.5)
                                {
                                    if(m_decor[1][i+vueMin.y][j+vueMin.x].getTileset()!= -1)
                                    {
                                    }
                                    else
                                    {
                                        bool erreur=false;
                                        /*if(!((w==1&&m_decor[1][i+vueMin.y+y][j+vueMin.x+w].getTileset()== 1)||(y==1&&m_decor[1][i+vueMin.y+y][j+vueMin.x+w].getTileset()== 1)))
                                        {*/
                                        if(m_decor[1][i+vueMin.y+y][j+vueMin.x+w].getTileset()>-1&&m_decor[1][i+vueMin.y+y][j+vueMin.x+w].getTileset()<m_tileset.size())
                                        {
                                            if(m_tileset[m_decor[1][i+vueMin.y+y][j+vueMin.x+w].getTileset()].getOrientationDuTile(m_decor[1][i+vueMin.y+y][j+vueMin.x+w].getTile())=='y'||m_tileset[m_decor[1][i+vueMin.y+y][j+vueMin.x+w].getTileset()].getOrientationDuTile(m_decor[1][i+vueMin.y+y][j+vueMin.x+w].getTile())=='d')
                                                if(w>0)
                                                    erreur=true;
                                            if(m_tileset[m_decor[1][i+vueMin.y+y][j+vueMin.x+w].getTileset()].getOrientationDuTile(m_decor[1][i+vueMin.y+y][j+vueMin.x+w].getTile())=='x'||m_tileset[m_decor[1][i+vueMin.y+y][j+vueMin.x+w].getTileset()].getOrientationDuTile(m_decor[1][i+vueMin.y+y][j+vueMin.x+w].getTile())=='g')
                                                if(y>0)
                                                    erreur=true;
                                            if(m_tileset[m_decor[1][i+vueMin.y+y][j+vueMin.x+w].getTileset()].getOrientationDuTile(m_decor[1][i+vueMin.y+y][j+vueMin.x+w].getTile())=='h')
                                                if(y>0||w>0)
                                                    erreur=true;
                                            if(m_tileset[m_decor[1][i+vueMin.y+y][j+vueMin.x+w].getTileset()].getOrientationDuTile(m_decor[1][i+vueMin.y+y][j+vueMin.x+w].getTile())=='b')
                                                if(y>0||w>0)
                                                    erreur=true;
                                        }

                                        if(!erreur)
                                        {
                                        double m=(double)m_tableauDesLampes[i][j].intensite/4;
                                        m_tableauDesLampes[i+y][j+w].intensite=(int)(m_tableauDesLampes[i][j].intensite/1.5);
                                        m_tableauDesLampes[i+y][j+w].rouge=m_tableauDesLampes[i][j].rouge;
                                        m_tableauDesLampes[i+y][j+w].vert=m_tableauDesLampes[i][j].vert;
                                        m_tableauDesLampes[i+y][j+w].bleu=m_tableauDesLampes[i][j].bleu;
                                        if(m_tableauDesLampes[i+y][j+w].intensite>255)
                                            m_tableauDesLampes[i+y][j+w].intensite=255;
                                        }
                                    }
                                }
                            }
                        }

        }


        // Bah voilà, j'espère que j'ai été clair ^^'
        //for(int z=0;z<configuration.anticrenelage_lumieres;z++)

}

void Map::Afficher(RenderWindow* ecran,View *camera,int type,Hero *hero,coordonnee positionSouris)
{
	coordonnee positionPartieDecor,vueMin,vueMax,positionHero;

	Sprite Sprite, spriteLumiereMask;

	 spriteLumiereMask.SetImage(lumiereMask);

	positionPartieDecor.x=0;
	positionPartieDecor.y=0;
	positionPartieDecor.w=128;
	positionPartieDecor.h=64;

	for(int i=0;i<m_tileset.size();i++)
		m_tileset[i].remiseAZeroDesSons();

    if(type==1)
    {
        coordonnee position;

        vueMin.x=hero->m_personnage.getCoordonnee().x-15;
        vueMin.y=hero->m_personnage.getCoordonnee().y-15;
        vueMax.x=hero->m_personnage.getCoordonnee().x+15;
        vueMax.y=hero->m_personnage.getCoordonnee().y+15;


        if(vueMin.x<0) { vueMin.x=0; }
        if(vueMin.y<0) { vueMin.y=0; }
        if(vueMax.x>m_decor[0][0].size()) { vueMax.x=m_decor[0][0].size(); }
        if(vueMax.y>m_decor[0].size()) { vueMax.y=m_decor[0].size(); }

        for(int couche=0;couche<2;couche++)
        {
            for(int j=hero->m_personnage.getCoordonnee().y-15;j<hero->m_personnage.getCoordonnee().y+15;j++)
            {
                for(int k=hero->m_personnage.getCoordonnee().x-15;k<hero->m_personnage.getCoordonnee().x+15;k++)
                {
                    position.x=(k-j-1+m_decor[0].size())*64;
                    position.y=(k+j)*32;
                    const sf::FloatRect& ViewRect = camera->GetRect();

                    if(j>=0&&j<m_decor[0].size()&&k>=0&&k<m_decor[0][0].size())
                    {
                        if(couche==1&&position.x+288>ViewRect.Left&&position.x<ViewRect.Right&&position.y+160>ViewRect.Top&&position.y<ViewRect.Bottom&&configuration.FonduLumiere)
                        {
                            spriteLumiereMask.SetX(position.x-128);
                            spriteLumiereMask.SetY(position.y-64);
                            int intensite=255-m_tableauDesLampes[j-vueMin.y][k-vueMin.x].intensite;
                            if(intensite>255)
                                intensite=255;
                            if(intensite<0)
                                intensite=0;
                            if(intensite>0)
                            {
                                //EffectBlur.SetParameter("offset", 0.01);
                                spriteLumiereMask.SetColor(sf::Color(255,255,255,intensite));
                                ecran->Draw(spriteLumiereMask);
                            }

                        }

                        if(couche==1)
                        {
                            if(j>=0&&j<m_decor[0].size()&&k>=0&&k<m_decor[0][0].size())
                            if(configuration.Herbes)
                                if(m_decor[0][j][k].getHerbe()>=0&&m_decor[0][j][k].getHerbe()<m_herbe.size())
                                {
                                    positionPartieDecor=m_herbe[m_decor[0][j][k].getHerbe()].getPositionDuTile(m_decor[0][j][k].getNumeroHerbe());

                                    position.y-=32;


                                    Sprite.SetImage(*m_herbe[m_decor[0][j][k].getHerbe()].getImage());
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

                                             ecran->Draw(Sprite);
                                        }
                                }

                        if(j>=0&&j<m_decor[0].size()&&k>=0&&k<m_decor[0][0].size())
                        {
                                if(m_monstreIllumine==m_decor[0][j][k].getMonstre()&&m_monstreIllumine!=-1||m_monstreIllumine==m_decor[1][j][k].getMonstre()&&m_monstreIllumine!=-1)
                                {

                                    LumiereOmbrage temp;
                                    temp=m_tableauDesLampes[j-vueMin.y][k-vueMin.x];

                                    temp.rouge=255;
                                    temp.vert=96;
                                    temp.bleu=96;
                                    temp.intensite=255;

                                    if(m_decor[0][j][k].getMonstre()>=0&&m_decor[0][j][k].getMonstre()<m_monstre.size())
                                        if(m_monstre[m_decor[0][j][k].getMonstre()].getCaracteristique().vie>0)
                                            m_monstre[m_decor[0][j][k].getMonstre()].Afficher(ecran,camera,position,getDimensions(),&temp,&m_ModeleMonstre[m_monstre[m_decor[0][j][k].getMonstre()].getModele()]);
                                        else
                                            m_monstre[m_decor[0][j][k].getMonstre()].Afficher(ecran,camera,position,getDimensions(),&m_tableauDesLampes[j-vueMin.y][k-vueMin.x],&m_ModeleMonstre[m_monstre[m_decor[0][j][k].getMonstre()].getModele()]);
                                    if(m_decor[1][j][k].getMonstre()>=0&&m_decor[1][j][k].getMonstre()<m_monstre.size())
                                        if(m_monstre[m_decor[1][j][k].getMonstre()].getCaracteristique().vie>0)
                                            m_monstre[m_decor[1][j][k].getMonstre()].Afficher(ecran,camera,position,getDimensions(),&temp,&m_ModeleMonstre[m_monstre[m_decor[1][j][k].getMonstre()].getModele()]);
                                        else
                                            m_monstre[m_decor[1][j][k].getMonstre()].Afficher(ecran,camera,position,getDimensions(),&m_tableauDesLampes[j-vueMin.y][k-vueMin.x],&m_ModeleMonstre[m_monstre[m_decor[1][j][k].getMonstre()].getModele()]);
                                    temp.detruire();
                                }
                                else
                                {
                                    if(m_decor[0][j][k].getMonstre()>=0&&m_decor[0][j][k].getMonstre()<m_monstre.size())
                                        m_monstre[m_decor[0][j][k].getMonstre()].Afficher(ecran,camera,position,getDimensions(),&m_tableauDesLampes[j-vueMin.y][k-vueMin.x],&m_ModeleMonstre[m_monstre[m_decor[0][j][k].getMonstre()].getModele()]);
                                    if(m_decor[1][j][k].getMonstre()>=0&&m_decor[1][j][k].getMonstre()<m_monstre.size())
                                        m_monstre[m_decor[1][j][k].getMonstre()].Afficher(ecran,camera,position,getDimensions(),&m_tableauDesLampes[j-vueMin.y][k-vueMin.x],&m_ModeleMonstre[m_monstre[m_decor[1][j][k].getMonstre()].getModele()]);
                                }
                        }

                        if(hero->m_personnage.getCoordonnee().x==hero->m_personnage.getProchaineCase().x&&hero->m_personnage.getCoordonnee().y==hero->m_personnage.getProchaineCase().y)
                        {
                            if(hero->m_personnage.getCoordonnee().x==k&&hero->m_personnage.getCoordonnee().y==j)
                                hero->m_personnage.Afficher(ecran,camera,position,getDimensions(),&m_lumiereHero,&hero->m_modelePersonnage);
                        }
                        else
                        {
                            if(hero->m_personnage.getCoordonnee().x>hero->m_personnage.getProchaineCase().x&&hero->m_personnage.getCoordonnee().y>hero->m_personnage.getProchaineCase().y)
                            {
                                if(hero->m_personnage.getProchaineCase().x+1==k&&hero->m_personnage.getProchaineCase().y+1==j)
                                    hero->m_personnage.Afficher(ecran,camera,position,getDimensions(),&m_lumiereHero,&hero->m_modelePersonnage);
                            }
                            else
                                if(hero->m_personnage.getProchaineCase().x==k&&hero->m_personnage.getProchaineCase().y==j)
                                    hero->m_personnage.Afficher(ecran,camera,position,getDimensions(),&m_lumiereHero,&hero->m_modelePersonnage);
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

                                    Sprite.SetImage(*m_herbe[m_decor[1][j][k].getHerbe()].getImage());
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

                                             ecran->Draw(Sprite);
                                        }
                                }
                    }


                        if(m_decor[couche][j][k].getTileset()>=0&&m_decor[couche][j][k].getTileset()<m_tileset.size())
                        {
                            positionPartieDecor=m_tileset[m_decor[couche][j][k].getTileset()].getPositionDuTile(m_decor[couche][j][k].getTile());
                            if(position.x+positionPartieDecor.h*2+positionPartieDecor.w>=ViewRect.Left&&position.x-positionPartieDecor.h*2-positionPartieDecor.w-64<ViewRect.Right&&position.y+positionPartieDecor.h+positionPartieDecor.w>=ViewRect.Top&&position.y-positionPartieDecor.h-32<ViewRect.Bottom+100&&m_tileset[m_decor[couche][j][k].getTileset()].getOmbreDuTile(m_decor[couche][j][k].getTile()&&configuration.Ombre&&m_tileset[m_decor[couche][j][k].getTileset()].getOmbreDuTile(m_decor[couche][j][k].getTile()))
                            ||position.x+positionPartieDecor.w>=ViewRect.Left&&position.x<ViewRect.Right&&position.y+positionPartieDecor.h>=ViewRect.Top&&position.y-positionPartieDecor.h+64<ViewRect.Bottom)
                            {
                                Sprite.SetImage(*m_tileset[m_decor[couche][j][k].getTileset()].getImage());
                                Sprite.SetSubRect(IntRect(positionPartieDecor.x, positionPartieDecor.y, positionPartieDecor.x+positionPartieDecor.w, positionPartieDecor.y+positionPartieDecor.h));

                                if(configuration.Ombre)
                                {
                                    if(m_tileset[m_decor[couche][j][k].getTileset()].getOmbreDuTile(m_decor[couche][j][k].getTile()))
                                    {
                                        for(int o=0;o<m_tableauDesLampes[j-vueMin.y][k-vueMin.x].m_ombre.size();o++)
                                        {
                                            Sprite.SetColor(sf::Color(0,0,0,m_tableauDesLampes[j-vueMin.y][k-vueMin.x].m_ombre[o].intensite));
                                           /* if(m_tableauDesLampes[j-vueMin.y][k-vueMin.x].m_ombre[o].angle<90&&m_tableauDesLampes[j-vueMin.y][k-vueMin.x].m_ombre[o].angle)
                                                Sprite.SetLeft(position.x+64-positionPartieDecor.w/2-(Sprite.GetWidth()*m_tableauDesLampes[j-vueMin.y][k-vueMin.x].m_ombre[o].angle/2/90)*1.5*(m_tableauDesLampes[j-vueMin.y][k-vueMin.x].m_ombre[o].taille/2));
                                            if(m_tableauDesLampes[j-vueMin.y][k-vueMin.x].m_ombre[o].angle>=90)
                                                Sprite.SetLeft(position.x+64-positionPartieDecor.w/2-(Sprite.GetWidth()*(180-m_tableauDesLampes[j-vueMin.y][k-vueMin.x].m_ombre[o].angle)/2/90)*1.5*(m_tableauDesLampes[j-vueMin.y][k-vueMin.x].m_ombre[o].taille/2));
                                            if(m_tableauDesLampes[j-vueMin.y][k-vueMin.x].m_ombre[o].angle<180&&m_tableauDesLampes[j-vueMin.y][k-vueMin.x].m_ombre[o].angle>=0)
                                                Sprite.SetTop(position.y-positionPartieDecor.h*1.5+64+(Sprite.GetHeight()*m_tableauDesLampes[j-vueMin.y][k-vueMin.x].m_ombre[o].angle/2/90)*(m_tableauDesLampes[j-vueMin.y][k-vueMin.x].m_ombre[o].taille/2));
                                            if(m_tableauDesLampes[j-vueMin.y][k-vueMin.x].m_ombre[o].angle<0)
                                                Sprite.SetTop(position.y-positionPartieDecor.h*1.5+64+(Sprite.GetHeight()*(-m_tableauDesLampes[j-vueMin.y][k-vueMin.x].m_ombre[o].angle)/2/90)*(m_tableauDesLampes[j-vueMin.y][k-vueMin.x].m_ombre[o].taille/2));*/

                                            /*Sprite.SetX(position.x+64-positionPartieDecor.w/2);
                                            Sprite.SetY(position.y-positionPartieDecor.h*m_tableauDesLampes[j-vueMin.y][k-vueMin.x].m_ombre[o].taille+32+32*m_tableauDesLampes[j-vueMin.y][k-vueMin.x].m_ombre[o].taille);
                                            Sprite.SetScale(1, m_tableauDesLampes[j-vueMin.y][k-vueMin.x].m_ombre[o].taille);
                                            Sprite.SetCenter(positionPartieDecor.w/2,positionPartieDecor.h*m_tableauDesLampes[j-vueMin.y][k-vueMin.x].m_ombre[o].taille-32*m_tableauDesLampes[j-vueMin.y][k-vueMin.x].m_ombre[o].taille);
                                            Sprite.SetRotation(m_tableauDesLampes[j-vueMin.y][k-vueMin.x].m_ombre[o].angle);
                                            ecran->Draw(Sprite);
                                             Sprite.SetCenter(0,0);
                                            Sprite.SetScale(1, 1);
                                            Sprite.SetRotation(0);*/

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
                                            ecran->Draw(Sprite);
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
                                    Sprite.SetX(position.x+64-positionPartieDecor.w/2);
                                    Sprite.SetY(position.y-positionPartieDecor.h+64);

                                    int alpha=255;

                                    if(couche==1&&positionPartieDecor.h>256)
                                    {
                                        alpha=(int)((positionHero.y+32)-position.y)+64;
                                        //if(position.y>positionHero.y+96)
                                          //  alpha=32;

                                        if(alpha<64)
                                            alpha=64;
                                        if(alpha>255)
                                            alpha=255;


                                        /*float nombre=gpl::sqrt(fabs(positionSouris.x/camera->Zoom-position.x+ViewRect.Left)*fabs(positionSouris.x/camera->Zoom-position.x+ViewRect.Left)+fabs(positionSouris.y/camera->Zoom-position.y+ViewRect.Top+positionPartieDecor.h/2)*fabs(positionSouris.y/camera->Zoom-position.y+ViewRect.Top+positionPartieDecor.h/2))*0.5;
                                        if(nombre<alpha)
                                            alpha=(int)nombre;*/
                                    }



                                    if(configuration.Lumiere&&configuration.FonduLumiere)
                                    {
                                        if(couche==1)
                                            Sprite.SetColor(sf::Color(
                                            (m_tableauDesLampes[j-vueMin.y][k-vueMin.x].intensite*m_tableauDesLampes[j-vueMin.y][k-vueMin.x].rouge)/255,
                                            (m_tableauDesLampes[j-vueMin.y][k-vueMin.x].intensite*m_tableauDesLampes[j-vueMin.y][k-vueMin.x].vert)/255,
                                            (m_tableauDesLampes[j-vueMin.y][k-vueMin.x].intensite*m_tableauDesLampes[j-vueMin.y][k-vueMin.x].bleu)/255,
                                            255));
                                        else
                                            Sprite.SetColor(sf::Color(
                                            (m_tableauDesLampes[j-vueMin.y][k-vueMin.x].rouge),
                                            (m_tableauDesLampes[j-vueMin.y][k-vueMin.x].vert),
                                            (m_tableauDesLampes[j-vueMin.y][k-vueMin.x].bleu),
                                            alpha));
                                    }
                                    else if(configuration.Lumiere)
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

                                     ecran->Draw(Sprite);
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
                                    Sprite.SetImage(*m_tileset[m_decor[couche][w][z].getTileset()].getImage());
                                    Sprite.SetSubRect(IntRect(positionPartieDecor.x, positionPartieDecor.y, positionPartieDecor.x+positionPartieDecor.w, positionPartieDecor.y+positionPartieDecor.h));
                                    Sprite.SetX(position.x+64-positionPartieDecor.w/2);
                                    Sprite.SetY(position.y-positionPartieDecor.h+64);
                                    ecran->Draw(Sprite);
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
        vueMax.x=hero->m_personnage.getCoordonnee().x+15;
        vueMax.y=hero->m_personnage.getCoordonnee().y+15;

        Sprite.SetCenter(4*configuration.Resolution.x/800,4*configuration.Resolution.x/800);
        Sprite.SetRotation(45);

        /*if(vueMin.x<0) { vueMin.x=0; }
        if(vueMin.y<0) { vueMin.y=0; }
        if(vueMax.x>m_decor[0][0].size()) { vueMax.x=m_decor[0][0].size(); }
        if(vueMax.y>m_decor[0].size()) { vueMax.y=m_decor[0].size(); }*/
            for(int j=vueMin.y;j<vueMax.y;j++)
            {
                for(int k=vueMin.x;k<vueMax.x;k++)
                {
                    position.x=(float)(((k-vueMin.x)-(j-vueMin.y)-1+40)*6*(float)configuration.Resolution.x/800);
                    position.y=(float)(((k-vueMin.x)+(j-vueMin.y))*6*(float)configuration.Resolution.x/800);
                    if(position.x+465*configuration.Resolution.x/800>600*configuration.Resolution.x/800&&position.x+465*configuration.Resolution.x/800<800*configuration.Resolution.x/800&&position.y*configuration.Resolution.x/800>0&&position.y-80*configuration.Resolution.x/800<195*configuration.Resolution.x/800)
                    {
                        if(getTypeCase(k,j)==1)
                        {
                            Sprite.SetImage(carreBrun);
                            Sprite.SetX((float)(position.x+465*configuration.Resolution.x/800));
                            Sprite.SetY((float)(position.y-80*configuration.Resolution.x/800));
                            ecran->Draw(Sprite);
                        }

                        if(getTypeCase(k,j)==2)
                        {
                            Sprite.SetImage(carreRouge);

                            Sprite.SetX(position.x+465*configuration.Resolution.x/800);
                            Sprite.SetY(position.y-80*configuration.Resolution.x/800);
                            ecran->Draw(Sprite);
                        }

                        if(getTypeCase(k,j)==3)
                        {
                                Sprite.SetImage(carreVert);
                                Sprite.SetX(position.x+465*configuration.Resolution.x/800);
                                Sprite.SetY(position.y-80*configuration.Resolution.x/800);
                                ecran->Draw(Sprite);
                        }

                        if(hero->m_personnage.getCoordonnee().x==k&&hero->m_personnage.getCoordonnee().y==j)
                        {
                                Sprite.SetImage(carreBleu);
                                Sprite.SetX((float)(position.x+465*configuration.Resolution.x/800));
                                Sprite.SetY((float)(position.y-80*configuration.Resolution.x/800));
                                ecran->Draw(Sprite);
                        }
                    }
                }
            }
	}
}

void Map::AfficherNomEvenement(sf::RenderWindow* ecran,coordonnee casePointee,coordonnee positionSouris)
{

    int evenement=-1;
    for(int i=0;i<2;i++)
        if(m_decor[i][casePointee.y][casePointee.x].getEvenement()>-1)
            evenement=m_decor[i][casePointee.y][casePointee.x].getEvenement();

    if(evenement>=0)
    {
        if(m_evenement[evenement].getType()==CHANGEMENT_DE_MAP)
        {
            string nom;
            char chemin[128];
            sprintf(chemin,"Data/Maps/map%ld.txt",m_evenement[evenement].getInformation(0));


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
            texte.SetY(positionSouris.y-16);
            texte.SetX(positionSouris.x);
            ecran->Draw(texte);
        }
    }
}

bool Map::testEvenement(sf::RenderWindow* ecran,Hero *hero,sf::View *camera,Menu* menu,Contexte **ctx,c_Chargement **chargement)
{
    for(int i=0;i<2;i++)
    if(m_decor[i][hero->m_personnage.getCoordonnee().y][hero->m_personnage.getCoordonnee().x].getEvenement()>=0)
    {
        if(m_evenement[m_decor[i][hero->m_personnage.getCoordonnee().y][hero->m_personnage.getCoordonnee().x].getEvenement()].getType()==CHANGEMENT_DE_MAP)
        {
            int numeroMap=m_evenement[m_decor[i][hero->m_personnage.getCoordonnee().y][hero->m_personnage.getCoordonnee().x].getEvenement()].getInformation(0);

            console.Ajouter("",0);
            console.Ajouter("---------------------------------------------------------------------------------",0);
            console.Ajouter("EVENEMENT : Changement de map",0);
            console.Ajouter("---------------------------------------------------------------------------------",0);

            coordonnee coordonneePerso;
            coordonneePerso.x=m_evenement[m_decor[i][hero->m_personnage.getCoordonnee().y][hero->m_personnage.getCoordonnee().x].getEvenement()].getInformation(1);
            coordonneePerso.y=m_evenement[m_decor[i][hero->m_personnage.getCoordonnee().y][hero->m_personnage.getCoordonnee().x].getEvenement()].getInformation(2);

            sf::Clock Clock;
             Clock.Reset();
            float Time = 0,temps_ecoule=0,tempsEcouleDepuisDernierAffichage=0;

            (*chargement)->setC_Chargement(numeroMap,coordonneePerso);
            *ctx = *chargement;
        }
    }
    return 1;
}


void Map::animer(Hero *hero,float temps,Menu *menu)
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
                m_decor[i][j][k].augmenterAnimation(temps);
                while(m_decor[i][j][k].getAnimation()>=0.075)
                {
                    if(m_decor[i][j][k].getTileset()>=0&&m_decor[i][j][k].getTileset()<m_tileset.size())
                        if(m_tileset[m_decor[i][j][k].getTileset()].getAnimationTile(m_decor[i][j][k].getTile())>=0)
                            m_decor[i][j][k].setDecor(m_decor[i][j][k].getTileset(),m_tileset[m_decor[i][j][k].getTileset()].getAnimationTile(m_decor[i][j][k].getTile()),m_decor[i][j][k].getEvenement(),m_decor[i][j][k].getMonstre(),m_decor[i][j][k].getHerbe());
                    m_decor[i][j][k].decrementerAnimation();
                }
                    if(m_decor[i][j][k].getMonstre()>=0&&m_decor[i][j][k].getMonstre()<m_monstre.size())
                        hero->m_personnage.infligerDegats(m_monstre[m_decor[i][j][k].getMonstre()].animer(&m_ModeleMonstre[m_monstre[m_decor[i][j][k].getMonstre()].getModele()],m_decor[0].size(),temps));

           }

}

void Map::musiquePlay(coordonnee position)
{
    Sound::Status Status = m_musique.GetStatus();

	if(Status==0)
	m_musique.Play();

	if(m_musique.GetVolume()<50)
	m_musique.SetVolume(m_musique.GetVolume()+1);

	m_musique.SetPosition(-position.x, 0, position.y);
}


void Map::gererMonstres(Hero *hero,float temps)
{
    coordonnee vueMin,vueMax;

    vueMin.x=hero->m_personnage.getCoordonnee().x-15;
    vueMin.y=hero->m_personnage.getCoordonnee().y-15;
    vueMax.x=hero->m_personnage.getCoordonnee().x+15;
    vueMax.y=hero->m_personnage.getCoordonnee().y+15;

    if(vueMin.x<0) { vueMin.x=0; }
    if(vueMin.y<0) { vueMin.y=0; }
    if(vueMax.x>m_decor[0][0].size()) { vueMax.x=m_decor[0][0].size(); }
    if(vueMax.y>m_decor[0].size()) { vueMax.y=m_decor[0].size(); }

	for(int i=0;i<2;i++)
        for(int j=vueMin.y;j<vueMax.y;j++)
            for(int k=vueMin.x;k<vueMax.x;k++)
                    if(m_decor[i][j][k].getMonstre()>=0&&m_decor[i][j][k].getMonstre()<m_monstre.size())
                    {

                        m_monstre[m_decor[i][j][k].getMonstre()].testerVision(hero->m_personnage.getCoordonnee());
                        if(m_monstre[m_decor[i][j][k].getMonstre()].getVu())
                        {
                            if(m_monstre[m_decor[i][j][k].getMonstre()].getCaracteristique().vie>0)
                            {
                                if(fabs(m_monstre[m_decor[i][j][k].getMonstre()].getCoordonnee().x-hero->m_personnage.getCoordonnee().x)>1||fabs(m_monstre[m_decor[i][j][k].getMonstre()].getCoordonnee().y-hero->m_personnage.getCoordonnee().y)>1)
                                {
                                    coordonnee arrivee,enCours;

                                    arrivee=hero->m_personnage.getCoordonnee();
                                    m_monstre[m_decor[i][j][k].getMonstre()].setArrivee(arrivee);

                                        if(m_monstre[m_decor[i][j][k].getMonstre()].seDeplacer(temps*100))
                                        {
                                            coordonnee tempCoord={-1,-1,-1,-1};
                                            m_monstre[m_decor[i][j][k].getMonstre()].pathfinding(getAlentourDuPersonnage(m_monstre[m_decor[i][j][k].getMonstre()].getCoordonnee()),tempCoord);
                                            if(!(m_monstre[m_decor[i][j][k].getMonstre()].getCoordonnee().y==j&&m_monstre[m_decor[i][j][k].getMonstre()].getCoordonnee().x==k))
                                            if(m_monstre[m_decor[i][j][k].getMonstre()].getCoordonnee().x>0&&m_monstre[m_decor[i][j][k].getMonstre()].getCoordonnee().x<m_decor[0][0].size()&&m_monstre[m_decor[i][j][k].getMonstre()].getCoordonnee().y>0&&m_monstre[m_decor[i][j][k].getMonstre()].getCoordonnee().y<m_decor[0].size())
                                            {
                                                m_decor[i][m_monstre[m_decor[i][j][k].getMonstre()].getCoordonnee().y][m_monstre[m_decor[i][j][k].getMonstre()].getCoordonnee().x].setMonstre(m_decor[i][j][k].getMonstre());
                                                //m_decor[i][m_monstre[m_decor[i][j][k].getMonstre()].getProchaineCase().y][m_monstre[m_decor[i][j][k].getMonstre()].getProchaineCase().x].setMonstre(m_decor[i][j][k].getMonstre());
                                                m_decor[i][j][k].setMonstre(-1);
                                            }
                                        }
                                }
                                else
                                {
                                    if(m_monstre[m_decor[i][j][k].getMonstre()].getEtat()!=2)
                                        m_monstre[m_decor[i][j][k].getMonstre()].setEtat(2);
                                    coordonnee temp,temp2;
                                    temp.x=(hero->m_personnage.getCoordonnee().x-hero->m_personnage.getCoordonnee().y-1+m_decor[1].size())*64;
                                    temp.y=(hero->m_personnage.getCoordonnee().x+hero->m_personnage.getCoordonnee().y)*32;
                                    temp2.x=(k-j-1+m_decor[1].size())*64;
                                    temp2.y=(k+j)*32;
                                    m_monstre[m_decor[i][j][k].getMonstre()].frappe(temp,temp2);
                                }
                            }
                            else if(m_monstre[m_decor[i][j][k].getMonstre()].getEtat()!=3)
                            m_monstre[m_decor[i][j][k].getMonstre()].setEtat(3);
                        }
                    }
}

bool Map::infligerDegats(int numeroMonstre, int degats,Menu *menu,sf::View *camera)
{
    if(numeroMonstre>=0&&numeroMonstre<m_monstre.size())
    {
        if(rand()%(100)>25)
        {
            coordonnee position;
            position.x=rand()%(800);
            position.y=rand()%(300);

            menu->AjouterSang(position);
        }

        m_monstre[numeroMonstre].infligerDegats(degats);
        if(m_monstre[numeroMonstre].getCaracteristique().vie<=0)
        {
            coordonneeDecimal position;
            position.x=((m_monstre[numeroMonstre].getCoordonnee().x-m_monstre[numeroMonstre].getCoordonnee().y-1+m_decor[0].size())*64)-camera->GetRect().Left+48-(configuration.Resolution.x/2-400);
            position.y=((m_monstre[numeroMonstre].getCoordonnee().x+m_monstre[numeroMonstre].getCoordonnee().y)*32)-camera->GetRect().Top-96;
            menu->AjouterAme(position,m_monstre[numeroMonstre].getCaracteristique().pointAme);
            return 1;
        }
    }
    return 0;
}

void Map::setVolumeMusique(int volume){m_musique.SetVolume(volume);}


coordonnee Map::getPositionMonstre(int numeroMonstre)
{
    if(numeroMonstre>=0&&numeroMonstre<m_monstre.size())
        return m_monstre[numeroMonstre].getCoordonnee();
}

Monstre *Map::getEntiteMonstre(int numeroMonstre)
{
    if(numeroMonstre>=0&&numeroMonstre<m_monstre.size())
        return &m_monstre[numeroMonstre];
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
            if(m_decor[i][casePointee.y][casePointee.x].getEvenement()>-1)
                temp=m_decor[i][casePointee.y][casePointee.x].getEvenement();

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
                if(m_monstre[m_decor[i][positionY][positionX].getMonstre()].getCaracteristique().vie>0)
                    return 1;

        coordonnee enCours;

        enCours.x=positionX+1;
        enCours.y=positionY+1;
        if(enCours.x>=0&&enCours.y>=0&&enCours.x<m_decor[0][0].size()&&enCours.y<m_decor[0].size())
         if(m_decor[i][ enCours.y][enCours.x].getMonstre()>-1&&m_decor[i][ enCours.y][enCours.x].getMonstre()<m_monstre.size())
                if(m_monstre[m_decor[i][ enCours.y][enCours.x].getMonstre()].getCaracteristique().vie>0)
            if(m_decor[i][enCours.y][enCours.x].getMonstre()<m_monstre.size())
                if(m_monstre[m_decor[i][enCours.y][enCours.x].getMonstre()].getProchaineCase().x==positionX&&m_monstre[m_decor[i][enCours.y][enCours.x].getMonstre()].getProchaineCase().y==positionY)
                    return 1;
        enCours.x=positionX-1;
        enCours.y=positionY-1;
        if(enCours.x>=0&&enCours.y>=0&&enCours.x<m_decor[0][0].size()&&enCours.y<m_decor[0].size())
         if(m_decor[i][ enCours.y][enCours.x].getMonstre()>-1&&m_decor[i][ enCours.y][enCours.x].getMonstre()<m_monstre.size())
                if(m_monstre[m_decor[i][ enCours.y][enCours.x].getMonstre()].getCaracteristique().vie>0)
            if(m_decor[i][enCours.y][enCours.x].getMonstre()<m_monstre.size())
                if(m_monstre[m_decor[i][enCours.y][enCours.x].getMonstre()].getProchaineCase().x==positionX&&m_monstre[m_decor[i][enCours.y][enCours.x].getMonstre()].getProchaineCase().y==positionY)
                    return 1;
        enCours.x=positionX+1;
        enCours.y=positionY-1;
        if(enCours.x>=0&&enCours.y>=0&&enCours.x<m_decor[0][0].size()&&enCours.y<m_decor[0].size())
         if(m_decor[i][ enCours.y][enCours.x].getMonstre()>-1&&m_decor[i][ enCours.y][enCours.x].getMonstre()<m_monstre.size())
                if(m_monstre[m_decor[i][ enCours.y][enCours.x].getMonstre()].getCaracteristique().vie>0)
            if(m_decor[i][enCours.y][enCours.x].getMonstre()<m_monstre.size())
                if(m_monstre[m_decor[i][enCours.y][enCours.x].getMonstre()].getProchaineCase().x==positionX&&m_monstre[m_decor[i][enCours.y][enCours.x].getMonstre()].getProchaineCase().y==positionY)
                    return 1;
        enCours.x=positionX-1;
        enCours.y=positionY+1;
        if(enCours.x>=0&&enCours.y>=0&&enCours.x<m_decor[0][0].size()&&enCours.y<m_decor[0].size())
         if(m_decor[i][ enCours.y][enCours.x].getMonstre()>-1&&m_decor[i][ enCours.y][enCours.x].getMonstre()<m_monstre.size())
                if(m_monstre[m_decor[i][ enCours.y][enCours.x].getMonstre()].getCaracteristique().vie>0)
            if(m_decor[i][enCours.y][enCours.x].getMonstre()<m_monstre.size())
                if(m_monstre[m_decor[i][enCours.y][enCours.x].getMonstre()].getProchaineCase().x==positionX&&m_monstre[m_decor[i][enCours.y][enCours.x].getMonstre()].getProchaineCase().y==positionY)
                    return 1;
            enCours.x=positionX;
        enCours.y=positionY+1;
        if(enCours.x>=0&&enCours.y>=0&&enCours.x<m_decor[0][0].size()&&enCours.y<m_decor[0].size())
         if(m_decor[i][ enCours.y][enCours.x].getMonstre()>-1&&m_decor[i][ enCours.y][enCours.x].getMonstre()<m_monstre.size())
                if(m_monstre[m_decor[i][ enCours.y][enCours.x].getMonstre()].getCaracteristique().vie>0)
            if(m_decor[i][enCours.y][enCours.x].getMonstre()<m_monstre.size())
                if(m_monstre[m_decor[i][enCours.y][enCours.x].getMonstre()].getProchaineCase().x==positionX&&m_monstre[m_decor[i][enCours.y][enCours.x].getMonstre()].getProchaineCase().y==positionY)
                    return 1;
        enCours.x=positionX+1;
        enCours.y=positionY;
        if(enCours.x>=0&&enCours.y>=0&&enCours.x<m_decor[0][0].size()&&enCours.y<m_decor[0].size())
         if(m_decor[i][ enCours.y][enCours.x].getMonstre()>-1&&m_decor[i][ enCours.y][enCours.x].getMonstre()<m_monstre.size())
                if(m_monstre[m_decor[i][ enCours.y][enCours.x].getMonstre()].getCaracteristique().vie>0)
            if(m_decor[i][enCours.y][enCours.x].getMonstre()<m_monstre.size())
                if(m_monstre[m_decor[i][enCours.y][enCours.x].getMonstre()].getProchaineCase().x==positionX&&m_monstre[m_decor[i][enCours.y][enCours.x].getMonstre()].getProchaineCase().y==positionY)
                    return 1;
        enCours.x=positionX-1;
        enCours.y=positionY;
        if(enCours.x>=0&&enCours.y>=0&&enCours.x<m_decor[0][0].size()&&enCours.y<m_decor[0].size())
         if(m_decor[i][ enCours.y][enCours.x].getMonstre()>-1&&m_decor[i][ enCours.y][enCours.x].getMonstre()<m_monstre.size())
                if(m_monstre[m_decor[i][ enCours.y][enCours.x].getMonstre()].getCaracteristique().vie>0)
            if(m_decor[i][enCours.y][enCours.x].getMonstre()<m_monstre.size())
                if(m_monstre[m_decor[i][enCours.y][enCours.x].getMonstre()].getProchaineCase().x==positionX&&m_monstre[m_decor[i][enCours.y][enCours.x].getMonstre()].getProchaineCase().y==positionY)
                    return 1;
        enCours.x=positionX;
        enCours.y=positionY-1;
        if(enCours.x>=0&&enCours.y>=0&&enCours.x<m_decor[0][0].size()&&enCours.y<m_decor[0].size())
         if(m_decor[i][ enCours.y][enCours.x].getMonstre()>-1&&m_decor[i][ enCours.y][enCours.x].getMonstre()<m_monstre.size())
                if(m_monstre[m_decor[i][ enCours.y][enCours.x].getMonstre()].getCaracteristique().vie>0)
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
                if(m_monstre[m_decor[i][positionY][positionX].getMonstre()].getCaracteristique().vie>0)
                    return 2;

            if(m_decor[i][positionY][positionX].getEvenement()>=0&&m_decor[i][positionY][positionX].getEvenement()<m_evenement.size())
                if(m_evenement[m_decor[i][positionY][positionX].getEvenement()].getType()==CHANGEMENT_DE_MAP)
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

    vueMin.x=casePointee.x-5;
    vueMin.y=casePointee.y-5;
    vueMax.x=casePointee.x+5;
    vueMax.y=casePointee.y+5;

    if(vueMin.x<0) { vueMin.x=0; }
    if(vueMin.y<0) { vueMin.y=0; }
    if(vueMax.x>m_decor[0][0].size()) { vueMax.x=m_decor[0][0].size(); }
    if(vueMax.y>m_decor[0].size()) { vueMax.y=m_decor[0].size(); }

	for(int i=0;i<2;i++)
        for(int j=vueMin.y;j<vueMax.y;j++)
            for(int k=vueMin.x;k<vueMax.x;k++)
                    if(m_decor[i][j][k].getMonstre()>=0&&m_decor[i][j][k].getMonstre()<m_monstre.size())
                    if(m_monstre[m_decor[i][j][k].getMonstre()].getCaracteristique().vie>0)
                    {
                        coordonnee temp;
                        temp.x=(int)((float)m_monstre[m_decor[i][j][k].getMonstre()].getCoordonneePixel().x*(float)DIVISEUR_COTE_TILE-(float)m_monstre[m_decor[i][j][k].getMonstre()].getCoordonneePixel().y*DIVISEUR_COTE_TILE-1+(float)m_decor[1].size())*64;
                        temp.y=(int)(float)(m_monstre[m_decor[i][j][k].getMonstre()].getCoordonneePixel().x*(float)DIVISEUR_COTE_TILE+(float)m_monstre[m_decor[i][j][k].getMonstre()].getCoordonneePixel().y*DIVISEUR_COTE_TILE)*32;

                        if(positionSouris.x+ViewRect.Left>temp.x-m_ModeleMonstre[m_monstre[m_decor[i][j][k].getMonstre()].getModele()].m_pose[m_monstre[m_decor[i][j][k].getMonstre()].getEtat()][(int)(m_monstre[m_decor[i][j][k].getMonstre()].getAngle()/45)][m_monstre[m_decor[i][j][k].getMonstre()].getPose()].getCoordonnee().w*0/128
                        &&positionSouris.x+ViewRect.Left<temp.x+m_ModeleMonstre[m_monstre[m_decor[i][j][k].getMonstre()].getModele()].m_pose[m_monstre[m_decor[i][j][k].getMonstre()].getEtat()][(int)(m_monstre[m_decor[i][j][k].getMonstre()].getAngle()/45)][m_monstre[m_decor[i][j][k].getMonstre()].getPose()].getCoordonnee().w*128/128
                        &&positionSouris.y+ViewRect.Top>temp.y-m_ModeleMonstre[m_monstre[m_decor[i][j][k].getMonstre()].getModele()].m_pose[m_monstre[m_decor[i][j][k].getMonstre()].getEtat()][(int)(m_monstre[m_decor[i][j][k].getMonstre()].getAngle()/45)][m_monstre[m_decor[i][j][k].getMonstre()].getPose()].getCoordonnee().h*48/128
                        &&positionSouris.y+ViewRect.Top<temp.y+64*m_ModeleMonstre[m_monstre[m_decor[i][j][k].getMonstre()].getModele()].m_pose[m_monstre[m_decor[i][j][k].getMonstre()].getEtat()][(int)(m_monstre[m_decor[i][j][k].getMonstre()].getAngle()/45)][m_monstre[m_decor[i][j][k].getMonstre()].getPose()].getCoordonnee().h/128)
                        {
                            float temp2=0;
                            temp2=gpl::sqrt((temp.x+m_ModeleMonstre[m_monstre[m_decor[i][j][k].getMonstre()].getModele()].m_pose[m_monstre[m_decor[i][j][k].getMonstre()].getEtat()][(int)(m_monstre[m_decor[i][j][k].getMonstre()].getAngle()/45)][m_monstre[m_decor[i][j][k].getMonstre()].getPose()].getCoordonnee().w/2-positionSouris.x-ViewRect.Left)
                            *(temp.x+m_ModeleMonstre[m_monstre[m_decor[i][j][k].getMonstre()].getModele()].m_pose[m_monstre[m_decor[i][j][k].getMonstre()].getEtat()][(int)(m_monstre[m_decor[i][j][k].getMonstre()].getAngle()/45)][m_monstre[m_decor[i][j][k].getMonstre()].getPose()].getCoordonnee().w/2-positionSouris.x-ViewRect.Left)
                            +(temp.y-(positionSouris.y-32)-ViewRect.Top)
                            *(temp.y-(positionSouris.y-32)-ViewRect.Top));

                            if(distance>temp2)
                                meilleur=m_decor[i][j][k].getMonstre(),distance=temp2;
                        }
                    }

                    m_monstreIllumine=meilleur;
                    return meilleur;
}
