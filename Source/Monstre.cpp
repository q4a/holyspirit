#include <iostream>
#include <fstream>

#include "Monstre.h"
#include "Constantes.h"
#include "Globale.h"

using namespace std;

Monstre::Monstre()
{
    m_vu=0;
    m_modele=-1;
    m_monstre=true;

    m_attente=0;
    m_compteur=0;

    m_miracleALancer=-1;
}

void Monstre::Charger(int numero,Modele_Monstre *modele)
{
   // Personnage::Charger(modele);

    m_modele=numero;
    m_caracteristique=modele->getCaracteristique();
    m_porteeLumineuse=modele->getPorteeLumineuse();
    m_porteeLumineuseBasique=m_porteeLumineuse;
    if(m_caracteristique.rang==0&&m_caracteristique.pointAme>0)
    {
        int temp=rand()%(1000);
        if(temp<50)
            m_caracteristique.rang=1;
        if(temp<15)
            m_caracteristique.rang=2;

        if(m_caracteristique.rang==1)
        {
            m_porteeLumineuse.intensite=512;
            m_caracteristique.maxVie*=3;
            m_caracteristique.vie*=3;
            m_caracteristique.degatsMin*=1;
            m_caracteristique.degatsMax*=1;
            m_caracteristique.pointAme*=2;

            m_caracteristique.modificateurTaille*=1.1;
        }
        if(m_caracteristique.rang==2)
        {
            m_porteeLumineuse.intensite=768;
            m_caracteristique.maxVie*=5;
            m_caracteristique.vie*=5;
            m_caracteristique.degatsMin*=2;
            m_caracteristique.degatsMax*=2;
            m_caracteristique.pointAme*=4;

            m_caracteristique.modificateurTaille*=1.2;
        }

        if(m_caracteristique.rang>0)
        {
            m_porteeLumineuse.rouge=rand()%255;
            m_porteeLumineuse.vert=rand()%255;
            m_porteeLumineuse.bleu=rand()%255;

            m_porteeLumineuseBasique=m_porteeLumineuse;
        }
    }
}


bool Modele_Monstre::Charger(string chemin)
{
    console.Ajouter("",0);
	console.Ajouter("Chargement du monstre : "+chemin,0);

    m_caracteristique.vitesse=0;
    m_caracteristique.vie=0;
    m_caracteristique.degatsMin=0;
    m_caracteristique.degatsMax=0;

    m_chemin=chemin;
    m_particules=-1;

    cDAT reader;

    reader.Read(chemin);

    ifstream *fichier;
    fichier = reader.GetInfos();

    if(fichier)
    {
    	char caractere;
    	do
    	{
    		fichier->get(caractere);
    		if(caractere=='*')
    		{

    			string cheminImage;
    			*fichier>>cheminImage;

                m_image.push_back(moteurGraphique.AjouterImage(reader.GetFile(cheminImage), reader.GetFileSize(cheminImage), cheminImage));
    		}
    		if(fichier->eof()){ char temp[1000]; sprintf(temp,"Erreur : Monstre \" %s \" Invalide",chemin.c_str());console.Ajouter(temp,1); caractere='$'; m_caracteristique.maxVie=0; }
    	}while(caractere!='$');

    	//m_buffer.clear();
    	do
    	{
    		fichier->get(caractere);
    		if(caractere=='*')
    		{
    			string cheminSon;
    			*fichier>>cheminSon;
                m_sons.push_back(moteurSons.AjouterBuffer(cheminSon));
    		}
    		if(fichier->eof()){ char temp[1000]; sprintf(temp,"Erreur : Monstre \" %s \" Invalide",chemin.c_str());console.Ajouter(temp,1); caractere='$'; m_caracteristique.maxVie=0;}
    	}while(caractere!='$');

        do
    	{
    	    //Chargement de la lumière ambiante
    		fichier->get(caractere);
    		if(caractere=='*')
            {
                do
                {
                    fichier->get(caractere);
                    switch (caractere)
                    {
                        case 'r': *fichier>>m_porteeLumineuse.rouge; break;
                        case 'v': *fichier>>m_porteeLumineuse.vert; break;
                        case 'b': *fichier>>m_porteeLumineuse.bleu; break;
                        case 'i': *fichier>>m_porteeLumineuse.intensite; break;
                    }
                     if(fichier->eof()){ char temp[1000]; sprintf(temp,"Erreur : Monstre \" %s \" Invalide",chemin.c_str());console.Ajouter(temp,1); caractere='$'; m_caracteristique.maxVie=0;}
                }while(caractere!='$');
                fichier->get(caractere);
                 if(fichier->eof()){ char temp[1000]; sprintf(temp,"Erreur : Monstre \" %s \" Invalide",chemin.c_str());console.Ajouter(temp,1); caractere='$'; m_caracteristique.maxVie=0;}
            }
    		if(fichier->eof()){ char temp[1000]; sprintf(temp,"Erreur : Monstre \" %s \" Invalide",chemin.c_str());console.Ajouter(temp,1); caractere='$'; }

    	}while(caractere!='$');

    	do
    	{
    		fichier->get(caractere);
    		if(caractere=='*')
            {
                getline(*fichier,m_caracteristique.nom);
                //*fichier>>m_caracteristique.nom;
            }
    		if(fichier->eof()){ char temp[1000]; sprintf(temp,"Erreur : Monstre \" %s \" Invalide",chemin.c_str());console.Ajouter(temp,1); caractere='$'; }

    	}while(caractere!='$');

    	do
    	{
    		fichier->get(caractere);
    		if(caractere=='*')
            {
                *fichier>>m_caracteristique.rang;
            }
    		if(fichier->eof()){ char temp[1000]; sprintf(temp,"Erreur : Monstre \" %s \" Invalide",chemin.c_str());console.Ajouter(temp,1); caractere='$'; }

    	}while(caractere!='$');

    	do
        {
            if(caractere=='*')
            {
                m_caracteristique.modificateurTaille=1;
                m_caracteristique.sang=0;
                m_explosif=false;
                do
                {
                    fichier->get(caractere);
                    switch (caractere)
                    {
                        case 'v': *fichier>>m_caracteristique.maxVie; break;
                        case 'd':  fichier->get(caractere); if(caractere=='i') *fichier>>m_caracteristique.degatsMin; else *fichier>>m_caracteristique.degatsMax ;  break;
                        case 'm': *fichier>>m_caracteristique.vitesse; break;
                        case 'o': *fichier>>m_ombre; break;
                        case 'a': *fichier>>m_caracteristique.pointAme; break;
                        case 'n': *fichier>>m_caracteristique.niveau; break;

                        case 's': *fichier>>m_caracteristique.sang; break;

                        case 't': *fichier>>m_caracteristique.modificateurTaille; break;

                        case 'e': *fichier>>m_explosif; break;
                    }
                     if(fichier->eof()){ char temp[1000]; sprintf(temp,"Erreur : Monstre \" %s \" Invalide",chemin.c_str());console.Ajouter(temp,1); caractere='$'; m_caracteristique.maxVie=0;}
                }while(caractere!='$');


                m_caracteristique.vie=m_caracteristique.maxVie;

                fichier->get(caractere);
                 if(fichier->eof()){ char temp[1000]; sprintf(temp,"Erreur : Monstre \" %s \" Invalide",chemin.c_str());console.Ajouter(temp,1); caractere='$'; m_caracteristique.maxVie=0;}
            }
            fichier->get(caractere);
            if(fichier->eof()){ char temp[1000]; sprintf(temp,"Erreur : Monstre \" %s \" Invalide",chemin.c_str());console.Ajouter(temp,1); caractere='$'; m_caracteristique.maxVie=0;  }
        }while(caractere!='$');

        do
    	{
    	    fichier->get(caractere);
    	    if(caractere=='*')
            {
                string temp;
                *fichier>>temp;
                //getline(fichier, temp);

                m_scriptAI.Charger(temp);
            }

    	    if(fichier->eof()){ char temp[1000]; sprintf(temp,"Erreur : Monstre \" %s \" Invalide",chemin.c_str());console.Ajouter(temp,1); caractere='$'; m_caracteristique.maxVie=0;  }
        }while(caractere!='$');


        do
    	{
    		fichier->get(caractere);
    		if(caractere=='*')
            {
                Objet tempModeleObjet;
                do
                {
                    fichier->get(caractere);
                    if(caractere=='r')
                    {
                        int temp2;
                        *fichier>>temp2;
                        tempModeleObjet.setChanceTrouver(temp2);
                    }

                    if(caractere=='*')
                    {
                        string temp2;
                        //getline(fichier, temp2);
                        *fichier>>temp2;
                        tempModeleObjet.Charger(temp2);
                    }

                    if(fichier->eof()){ char temp[1000]; sprintf(temp,"Erreur : Monstre \" %s \" Invalide",chemin.c_str());console.Ajouter(temp,1); caractere='$'; }
                }while(caractere!='$');
                m_objets.push_back(tempModeleObjet);
                fichier->get(caractere);
            }
    		if(fichier->eof()){ char temp[1000]; sprintf(temp,"Erreur : Monstre \" %s \" Invalide",chemin.c_str());console.Ajouter(temp,1); caractere='$'; }

    	}while(caractere!='$');

    	do
    	{
    	    fichier->get(caractere);
    	    if(caractere=='*')
            {
                string temp;
                //getline(fichier, temp);
                *fichier>>temp;
                m_particules=moteurGraphique.AjouterModeleSystemeParticules(temp);
            }

    	    if(fichier->eof()){ char temp[1000]; sprintf(temp,"Erreur : Monstre \" %s \" Invalide",chemin.c_str());console.Ajouter(temp,1); caractere='$'; m_caracteristique.maxVie=0;  }
        }while(caractere!='$');


        do
    	{
    	    fichier->get(caractere);
    	    if(caractere=='*')
            {
                string temp;
                //getline(fichier, temp);
                *fichier>>temp;
                m_miracles.push_back(Miracle (temp));
            }

    	    if(fichier->eof()){ char temp[1000]; sprintf(temp,"Erreur : Monstre \" %s \" Invalide",chemin.c_str());console.Ajouter(temp,1); caractere='$'; m_caracteristique.maxVie=0;  }
        }while(caractere!='$');


        Pose poseTemp;
        m_pose.resize(NOMBRE_ETAT,vector<vector<Pose> >(0,vector<Pose>(0,poseTemp)));

    	for(int i=0;i<NOMBRE_ETAT;i++)
    	{
    	    for(int j=0;j<8;j++)
    	    {
    	        m_pose[i].push_back(vector<Pose> (0,poseTemp));
                do
                {
                    if(caractere=='*')
                    {
                        coordonnee position={-1,-1,-1,-1},centre={-1,-1,-1,-1};
                        int animation=0,son=-1,image=0,attaque=-1,lumiere=-1;
                        float tempsAnimation=0.075;

                        do
                        {
                            fichier->get(caractere);
                            switch (caractere)
                            {
                                case 'x': *fichier>>position.x; break;
                                case 'y': *fichier>>position.y; break;
                                case 'w': *fichier>>position.w; break;
                                case 'h': *fichier>>position.h; break;
                                case 'a': *fichier>>animation; break;
                                case 't': *fichier>>tempsAnimation; break;
                                case 's': *fichier>>son; break;
                                case 'i': *fichier>>image; break;
                                case 'd': *fichier>>attaque; break;
                                case 'l': *fichier>>lumiere; break;

                                case 'c': fichier->get(caractere); if(caractere=='x') *fichier>>centre.x; else *fichier>>centre.y; break;
                            }
                            if(fichier->eof()){ char temp[1000]; sprintf(temp,"Erreur : Monstre \" %s \" Invalide",chemin.c_str());console.Ajouter(temp,1); caractere='$'; m_caracteristique.maxVie=0;}
                        }while(caractere!='$');

                        if(centre.x==-1)
                            centre.x=position.w/2;
                        if(centre.y==-1)
                            centre.y=position.h-32;

                        m_pose[i][j].push_back(poseTemp);
                        m_pose[i][j].back().setPose(position,centre,animation,son,image,attaque,lumiere,tempsAnimation,0);
                        fichier->get(caractere);
                        if(fichier->eof()){ char temp[1000]; sprintf(temp,"Erreur : Monstre \" %s \" Invalide",chemin.c_str());console.Ajouter(temp,1); caractere='$'; m_caracteristique.maxVie=0;  }
                    }
                    fichier->get(caractere);
                    if(fichier->eof()){ char temp[1000]; sprintf(temp,"Erreur : Monstre \" %s \" Invalide",chemin.c_str());console.Ajouter(temp,1); caractere='$'; m_caracteristique.maxVie=0; }
                }while(caractere!='$');
    	    }
    	}
    }
    else
    console.Ajouter("Impossible d'ouvrir : "+chemin,1);

    fichier->close();

    return 1;
}

std::vector<Objet> Modele_Monstre::getObjets(){return m_objets;}

void Monstre::testerVision(coordonnee positionHero)
{
    if(m_caracteristique.vie>0)
    {
        if(fabs(positionHero.x-m_positionCase.x)<5&&fabs(positionHero.y-m_positionCase.y)<5)
        {
            m_vu=1;
            if(m_etat==0) m_poseEnCours=0;
        }
        if(fabs(positionHero.x-m_positionCase.x)>10||fabs(positionHero.y-m_positionCase.y)>10)
        {
            if(m_vu)
                setDepart();
            m_vu=0,m_etat=0,m_poseEnCours=0;

        }
    }
}

coordonnee Monstre::getDepart(){return m_depart;}

int Monstre::getModele(){ return m_modele; }

bool Monstre::getVu(){return m_vu;}
void Monstre::setVu(bool vu){m_vu=vu;}

void Monstre::setDepart()
{
    m_depart=m_positionCase;
}
