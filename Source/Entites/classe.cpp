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


#include "classe.h"
#include "../globale.h"

using namespace std;

void Classe::Charger(string chemin)
{
    emplacements.clear();
    equipementParDefaut.clear();


    ifstream fichier;
    fichier.open(chemin.c_str(), ios::in);
    if (fichier)
    {
        char caractere;
        do
        {
            fichier.get(caractere);
            if (caractere=='*')
            {
                fichier>>nom;
            }
            if (fichier.eof())
            {
                char temp[255];
                sprintf(temp,"Erreur : Classe \" %s \" Invalide",chemin.c_str());
                console->Ajouter(temp,1);
                caractere='$';
            }
        }
        while (caractere!='$');


        do
        {
            fichier.get(caractere);
            if (caractere=='*')
            {
                do
                {
                    fichier.get(caractere);
                    switch (caractere)
                    {
                    case 'f' :
                        fichier>>caracteristique.force;
                        break;
                    case 'v' :
                        fichier>>caracteristique.vitalite;
                        break;
                    case 'p' :
                        fichier>>caracteristique.piete;
                        break;
                    case 'd' :
                        fichier>>caracteristique.dexterite;
                        break;
                    case 'c' :
                        fichier>>caracteristique.charisme;
                        break;
                    case 'i' :
                        fichier>>ID;
                        break;
                    }

                    if (fichier.eof())
                    {
                        char temp[255];
                        sprintf(temp,"Erreur : Classe \" %s \" Invalide",chemin.c_str());
                        console->Ajouter(temp,1);
                        caractere='$';
                    }

                }
                while (caractere!='$');
                fichier.get(caractere);
            }
            if (fichier.eof())
            {
                char temp[255];
                sprintf(temp,"Erreur : Classe \" %s \" Invalide",chemin.c_str());
                console->Ajouter(temp,1);
                caractere='$';
            }

        }
        while (caractere!='$');

        do
        {
            fichier.get(caractere);
            if (caractere=='*')
            {
                do
                {
                    fichier.get(caractere);
                    switch (caractere)
                    {
                    case 'm' :
                        string temp;
                        fichier>>temp;
                        equipementParDefaut.push_back(temp);
                        break;
                    }

                    if (fichier.eof())
                    {
                        char temp[255];
                        sprintf(temp,"Erreur : Classe \" %s \" Invalide",chemin.c_str());
                        console->Ajouter(temp,1);
                        caractere='$';
                    }

                }
                while (caractere!='$');
                fichier.get(caractere);
            }
            if (fichier.eof())
            {
                char temp[255];
                sprintf(temp,"Erreur : Classe \" %s \" Invalide",chemin.c_str());
                console->Ajouter(temp,1);
                caractere='$';
            }

        }
        while (caractere!='$');


        do
        {
            fichier.get(caractere);
            if (caractere=='*')
            {
                do
                {
                    fichier.get(caractere);
                    switch (caractere)
                    {
                    case 'm' :
                        fichier>>chemin_modele;
                        break;
                    }

                    if (fichier.eof())
                    {
                        char temp[255];
                        sprintf(temp,"Erreur : Classe \" %s \" Invalide",chemin.c_str());
                        console->Ajouter(temp,1);
                        caractere='$';
                    }

                }
                while (caractere!='$');
                fichier.get(caractere);
            }
            if (fichier.eof())
            {
                char temp[255];
                sprintf(temp,"Erreur : Classe \" %s \" Invalide",chemin.c_str());
                console->Ajouter(temp,1);
                caractere='$';
            }

        }
        while (caractere!='$');

        do
        {
            fichier.get(caractere);
            if (caractere=='*')
            {
                int temp=0;
                do
                {
                    fichier.get(caractere);
                    switch (caractere)
                    {
                    case 'm' :
                        fichier>>modeleNu[temp];
                        temp++;
                        break;
                    }

                    if (fichier.eof())
                    {
                        char temp[255];
                        sprintf(temp,"Erreur : Classe \" %s \" Invalide",chemin.c_str());
                        console->Ajouter(temp,1);
                        caractere='$';
                    }

                }
                while (caractere!='$');
                fichier.get(caractere);
            }
            if (fichier.eof())
            {
                char temp[255];
                sprintf(temp,"Erreur : Classe \" %s \" Invalide",chemin.c_str());
                console->Ajouter(temp,1);
                caractere='$';
            }

        }
        while (caractere!='$');

        std::string string;

        do
        {
            fichier.get(caractere);
            if (caractere=='*')
            {

                do
                {
                    fichier.get(caractere);
                    switch (caractere)
                    {
                    case 'm' :
                        fichier>>string;
                        inventaire.image=moteurGraphique->AjouterImage(string,-1);
                        break;
                    case 'x' :
                        fichier>>inventaire.position.x;
                        break;
                    case 'y' :
                        fichier>>inventaire.position.y;
                        break;
                    case 'w' :
                        fichier>>inventaire.position.w;
                        break;
                    case 'h' :
                        fichier>>inventaire.position.h;
                        break;
                    }

                    if (fichier.eof())
                    {
                        char temp[255];
                        sprintf(temp,"Erreur : Classe \" %s \" Invalide",chemin.c_str());
                        console->Ajouter(temp,1);
                        caractere='$';
                    }
                }
                while (caractere!='$');
                fichier.get(caractere);
            }
            if (fichier.eof())
            {
                char temp[255];
                sprintf(temp,"Erreur : Classe \" %s \" Invalide",chemin.c_str());
                console->Ajouter(temp,1);
                caractere='$';
            }

        }
        while (caractere!='$');

        do
        {
            fichier.get(caractere);
            if (caractere=='*')
            {

                do
                {
                    fichier.get(caractere);
                    switch (caractere)
                    {
                    case 'm' :
                        fichier>>string;
                        menu_marchand.image=moteurGraphique->AjouterImage(string,-1);
                        break;
                    case 'x' :
                        fichier>>menu_marchand.position.x;
                        break;
                    case 'y' :
                        fichier>>menu_marchand.position.y;
                        break;
                    case 'w' :
                        fichier>>menu_marchand.position.w;
                        break;
                    case 'h' :
                        fichier>>menu_marchand.position.h;
                        break;
                    }

                    if (fichier.eof())
                    {
                        char temp[255];
                        sprintf(temp,"Erreur : Classe \" %s \" Invalide",chemin.c_str());
                        console->Ajouter(temp,1);
                        caractere='$';
                    }
                }
                while (caractere!='$');
                fichier.get(caractere);
            }
            if (fichier.eof())
            {
                char temp[255];
                sprintf(temp,"Erreur : Classe \" %s \" Invalide",chemin.c_str());
                console->Ajouter(temp,1);
                caractere='$';
            }

        }
        while (caractere!='$');



        do
        {
            fichier.get(caractere);
            if (caractere=='*')
            {
                do
                {
                    fichier.get(caractere);
                    switch (caractere)
                    {
                    case 'm' :
                        fichier>>string;
                        hud.image=moteurGraphique->AjouterImage(string,-1);
                        break;
                    case 'x' :
                        fichier>>hud.position.x;
                        break;
                    case 'y' :
                        fichier>>hud.position.y;
                        break;
                    case 'w' :
                        fichier>>hud.position.w;
                        break;
                    case 'h' :
                        fichier>>hud.position.h;
                        break;
                    }

                    if (fichier.eof())
                    {
                        char temp[255];
                        sprintf(temp,"Erreur : Classe \" %s \" Invalide",chemin.c_str());
                        console->Ajouter(temp,1);
                        caractere='$';
                    }

                }
                while (caractere!='$');
                fichier.get(caractere);
            }
            if (fichier.eof())
            {
                char temp[255];
                sprintf(temp,"Erreur : Classe \" %s \" Invalide",chemin.c_str());
                console->Ajouter(temp,1);
                caractere='$';
            }

        }
        while (caractere!='$');

        do
        {
            fichier.get(caractere);
            if (caractere=='*')
            {
                do
                {
                    fichier.get(caractere);
                    switch (caractere)
                    {
                    case 'm' :
                        fichier>>string;
                        orbe_vie.image=moteurGraphique->AjouterImage(string,-1);
                        break;
                    case 'x' :
                        fichier>>orbe_vie.position.x;
                        break;
                    case 'y' :
                        fichier>>orbe_vie.position.y;
                        break;
                    case 'w' :
                        fichier>>orbe_vie.position.w;
                        break;
                    case 'h' :
                        fichier>>orbe_vie.position.h;
                        break;
                    }

                    if (fichier.eof())
                    {
                        char temp[255];
                        sprintf(temp,"Erreur : Classe \" %s \" Invalide",chemin.c_str());
                        console->Ajouter(temp,1);
                        caractere='$';
                    }

                }
                while (caractere!='$');
                fichier.get(caractere);
            }
            if (fichier.eof())
            {
                char temp[255];
                sprintf(temp,"Erreur : Classe \" %s \" Invalide",chemin.c_str());
                console->Ajouter(temp,1);
                caractere='$';
            }

        }
        while (caractere!='$');

        do
        {
            fichier.get(caractere);
            if (caractere=='*')
            {
                do
                {
                    fichier.get(caractere);
                    switch (caractere)
                    {
                    case 'm' :
                        fichier>>string;
                        orbe_foi.image=moteurGraphique->AjouterImage(string,-1);
                        break;
                    case 'x' :
                        fichier>>orbe_foi.position.x;
                        break;
                    case 'y' :
                        fichier>>orbe_foi.position.y;
                        break;
                    case 'w' :
                        fichier>>orbe_foi.position.w;
                        break;
                    case 'h' :
                        fichier>>orbe_foi.position.h;
                        break;
                    }

                    if (fichier.eof())
                    {
                        char temp[255];
                        sprintf(temp,"Erreur : Classe \" %s \" Invalide",chemin.c_str());
                        console->Ajouter(temp,1);
                        caractere='$';
                    }

                }
                while (caractere!='$');
                fichier.get(caractere);
            }
            if (fichier.eof())
            {
                char temp[255];
                sprintf(temp,"Erreur : Classe \" %s \" Invalide",chemin.c_str());
                console->Ajouter(temp,1);
                caractere='$';
            }

        }
        while (caractere!='$');

        do
        {
            fichier.get(caractere);
            if (caractere=='*')
            {
                do
                {
                    fichier.get(caractere);
                    switch (caractere)
                    {
                    case 'm' :
                        fichier>>string;
                        cache_vie.image=moteurGraphique->AjouterImage(string,-1);
                        break;
                    case 'x' :
                        fichier>>cache_vie.position.x;
                        break;
                    case 'y' :
                        fichier>>cache_vie.position.y;
                        break;
                    case 'w' :
                        fichier>>cache_vie.position.w;
                        break;
                    case 'h' :
                        fichier>>cache_vie.position.h;
                        break;
                    }

                    if (fichier.eof())
                    {
                        char temp[255];
                        sprintf(temp,"Erreur : Classe \" %s \" Invalide",chemin.c_str());
                        console->Ajouter(temp,1);
                        caractere='$';
                    }

                }
                while (caractere!='$');
                fichier.get(caractere);
            }
            if (fichier.eof())
            {
                char temp[255];
                sprintf(temp,"Erreur : Classe \" %s \" Invalide",chemin.c_str());
                console->Ajouter(temp,1);
                caractere='$';
            }

        }
        while (caractere!='$');

        do
        {
            fichier.get(caractere);
            if (caractere=='*')
            {
                do
                {
                    fichier.get(caractere);
                    switch (caractere)
                    {
                    case 'm' :
                        fichier>>string;
                        cache_foi.image=moteurGraphique->AjouterImage(string,-1);
                        break;
                    case 'x' :
                        fichier>>cache_foi.position.x;
                        break;
                    case 'y' :
                        fichier>>cache_foi.position.y;
                        break;
                    case 'w' :
                        fichier>>cache_foi.position.w;
                        break;
                    case 'h' :
                        fichier>>cache_foi.position.h;
                        break;
                    }

                    if (fichier.eof())
                    {
                        char temp[255];
                        sprintf(temp,"Erreur : Classe \" %s \" Invalide",chemin.c_str());
                        console->Ajouter(temp,1);
                        caractere='$';
                    }

                }
                while (caractere!='$');
                fichier.get(caractere);
            }
            if (fichier.eof())
            {
                char temp[255];
                sprintf(temp,"Erreur : Classe \" %s \" Invalide",chemin.c_str());
                console->Ajouter(temp,1);
                caractere='$';
            }

        }
        while (caractere!='$');

        do
        {
            fichier.get(caractere);
            if (caractere=='*')
            {
                do
                {
                    fichier.get(caractere);
                    switch (caractere)
                    {
                    case 'x' :
                        fichier>>position_sac_inventaire.x;
                        break;
                    case 'y' :
                        fichier>>position_sac_inventaire.y;
                        break;
                    case 'w' :
                        fichier>>position_sac_inventaire.w;
                        break;
                    case 'h' :
                        fichier>>position_sac_inventaire.h;
                        break;
                    }

                    if (fichier.eof())
                    {
                        char temp[255];
                        sprintf(temp,"Erreur : Classe \" %s \" Invalide",chemin.c_str());
                        console->Ajouter(temp,1);
                        caractere='$';
                    }

                }
                while (caractere!='$');
                fichier.get(caractere);
            }
            if (fichier.eof())
            {
                char temp[255];
                sprintf(temp,"Erreur : Classe \" %s \" Invalide",chemin.c_str());
                console->Ajouter(temp,1);
                caractere='$';
            }

        }
        while (caractere!='$');

        do
        {
            fichier.get(caractere);
            if (caractere=='*')
            {
                do
                {
                    fichier.get(caractere);
                    switch (caractere)
                    {
                    case 'x' :
                        fichier>>position_contenu_inventaire.x;
                        break;
                    case 'y' :
                        fichier>>position_contenu_inventaire.y;
                        break;
                    case 'w' :
                        fichier>>position_contenu_inventaire.w;
                        break;
                    case 'h' :
                        fichier>>position_contenu_inventaire.h;
                        break;
                    }

                    if (fichier.eof())
                    {
                        char temp[255];
                        sprintf(temp,"Erreur : Classe \" %s \" Invalide",chemin.c_str());
                        console->Ajouter(temp,1);
                        caractere='$';
                    }

                }
                while (caractere!='$');
                fichier.get(caractere);
            }
            if (fichier.eof())
            {
                char temp[255];
                sprintf(temp,"Erreur : Classe \" %s \" Invalide",chemin.c_str());
                console->Ajouter(temp,1);
                caractere='$';
            }

        }
        while (caractere!='$');

        do
        {
            fichier.get(caractere);
            if (caractere=='*')
            {
                do
                {
                    fichier.get(caractere);
                    switch (caractere)
                    {
                    case 'x' :
                        fichier>>position_contenu_marchand.x;
                        break;
                    case 'y' :
                        fichier>>position_contenu_marchand.y;
                        break;
                    case 'w' :
                        fichier>>position_contenu_marchand.w;
                        break;
                    case 'h' :
                        fichier>>position_contenu_marchand.h;
                        break;
                    }

                    if (fichier.eof())
                    {
                        char temp[255];
                        sprintf(temp,"Erreur : Classe \" %s \" Invalide",chemin.c_str());
                        console->Ajouter(temp,1);
                        caractere='$';
                    }

                }
                while (caractere!='$');
                fichier.get(caractere);
            }
            if (fichier.eof())
            {
                char temp[255];
                sprintf(temp,"Erreur : Classe \" %s \" Invalide",chemin.c_str());
                console->Ajouter(temp,1);
                caractere='$';
            }

        }
        while (caractere!='$');

        do
        {
            fichier.get(caractere);
            if (caractere=='*')
            {
                emplacements.push_back(Emplacement_inventaire ());
                do
                {
                    fichier.get(caractere);
                    switch (caractere)
                    {
                    case 'e' :
                        fichier>>emplacements.back().emplacement;
                        break;
                    case 'x' :
                        fichier>>emplacements.back().position.x;
                        break;
                    case 'y' :
                        fichier>>emplacements.back().position.y;
                        break;
                    case 'w' :
                        fichier>>emplacements.back().position.w;
                        break;
                    case 'h' :
                        fichier>>emplacements.back().position.h;
                        break;
                    }

                    if (fichier.eof())
                    {
                        char temp[255];
                        sprintf(temp,"Erreur : Classe \" %s \" Invalide",chemin.c_str());
                        console->Ajouter(temp,1);
                        caractere='$';
                    }

                }
                while (caractere!='$');
                fichier.get(caractere);
            }
            if (fichier.eof())
            {
                char temp[255];
                sprintf(temp,"Erreur : Classe \" %s \" Invalide",chemin.c_str());
                console->Ajouter(temp,1);
                caractere='$';
            }

        }
        while (caractere!='$');

        fichier.close();
    }
    else
        console->Ajouter("Impossible de charger : "+chemin,1);
}


