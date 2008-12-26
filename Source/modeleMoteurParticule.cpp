
#include "modeleMoteurParticule.h"
#include "globale.h"
#include <fstream>



ModeleParticuleSysteme::ModeleParticuleSysteme()
{

}
ModeleParticuleSysteme::ModeleParticuleSysteme(std::string chemin)
{
    Charger(chemin);
}
ModeleParticuleSysteme::~ModeleParticuleSysteme()
{
    m_particules.clear();
}
void ModeleParticuleSysteme::Charger(std::string chemin)
{
    console.Ajouter("",0);
	console.Ajouter("Chargement du systeme de particules : "+chemin,0);

	std::ifstream fichier;
    fichier.open(chemin.c_str(), std::ios::in);
    if(fichier)
    {
        char caractere;
        do
    	{
    		fichier.get(caractere);
    		if(caractere=='*')
            {
                std::string m_cheminImage;
                getline(fichier, m_cheminImage);
                m_image=moteurGraphique.AjouterImage(m_cheminImage);
            }
    		if(fichier.eof()){ char temp[255]; sprintf(temp,"Erreur : Système de particules \" %s \" Invalide",chemin.c_str());console.Ajouter(temp,1); caractere='$'; }

    	}while(caractere!='$');

    	do
    	{
    		fichier.get(caractere);
    		if(caractere=='*')
            {
                std::string m_cheminSon;
                getline(fichier, m_cheminSon);
                m_son=moteurSons.AjouterBuffer(m_cheminSon);
            }
    		if(fichier.eof()){ char temp[255]; sprintf(temp,"Erreur : Système de particules \" %s \" Invalide",chemin.c_str());console.Ajouter(temp,1); caractere='$'; }

    	}while(caractere!='$');


    	m_chemin=chemin;

    	do
    	{
    		fichier.get(caractere);
    		if(caractere=='*')
            {
                m_particules.push_back(ModeleParticule ());
                do
                {
                    fichier.get(caractere);
                    switch (caractere)
                    {
                        case 'x' : fichier>>m_particules[m_particules.size()-1].positionImage.x; break;
                        case 'y' : fichier>>m_particules[m_particules.size()-1].positionImage.y; break;
                        case 'w' : fichier>>m_particules[m_particules.size()-1].positionImage.w; break;
                        case 'h' : fichier>>m_particules[m_particules.size()-1].positionImage.h; break;

                        case 'i' : fichier>>m_particules[m_particules.size()-1].min; break;
                        case 'a' : fichier>>m_particules[m_particules.size()-1].max; break;
                    }

                    if(fichier.eof()){ char temp[255]; sprintf(temp,"Erreur : Objet \" %s \" Invalide",chemin.c_str());console.Ajouter(temp,1); caractere='$'; }

                }while(caractere!='$');
                fichier.get(caractere);
            }
    		if(fichier.eof()){ char temp[255]; sprintf(temp,"Erreur : Objet \" %s \" Invalide",chemin.c_str());console.Ajouter(temp,1); caractere='$'; }

    	}while(caractere!='$');
    }
    else
    console.Ajouter("Impossible d'ouvrir : "+chemin,1);
}
