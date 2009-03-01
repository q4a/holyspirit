

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
    console->Ajouter("",0);
	console->Ajouter("Chargement du systeme de particules : "+chemin,0);

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
                m_image=moteurGraphique->AjouterImage(m_cheminImage,-1);
            }
    		if(fichier.eof()){ char temp[255]; sprintf(temp,"Erreur : Système de particules \" %s \" Invalide",chemin.c_str());console->Ajouter(temp,1); caractere='$'; }

    	}while(caractere!='$');

    	do
    	{
    		fichier.get(caractere);
    		if(caractere=='*')
            {
                std::string m_cheminSon;
                getline(fichier, m_cheminSon);
                m_son=moteurSons->AjouterBuffer(m_cheminSon);
            }
    		if(fichier.eof()){ char temp[255]; sprintf(temp,"Erreur : Système de particules \" %s \" Invalide",chemin.c_str());console->Ajouter(temp,1); caractere='$'; }

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
                        case 'x' : fichier>>m_particules.back().positionImage.x; break;
                        case 'y' : fichier>>m_particules.back().positionImage.y; break;
                        case 'w' : fichier>>m_particules.back().positionImage.w; break;
                        case 'h' : fichier>>m_particules.back().positionImage.h; break;

                        case 'i' : fichier>>m_particules.back().min; break;
                        case 'a' : fichier>>m_particules.back().max; break;
                    }

                    if(fichier.eof()){ char temp[255]; sprintf(temp,"Erreur : Objet \" %s \" Invalide",chemin.c_str());console->Ajouter(temp,1); caractere='$'; }

                }while(caractere!='$');
                fichier.get(caractere);
            }
    		if(fichier.eof()){ char temp[255]; sprintf(temp,"Erreur : Objet \" %s \" Invalide",chemin.c_str());console->Ajouter(temp,1); caractere='$'; }

    	}while(caractere!='$');
    }
    else
    console->Ajouter("Impossible d'ouvrir : "+chemin,1);
}
