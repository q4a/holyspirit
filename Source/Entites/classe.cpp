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


void                ChargerImageInterface(ifstream &fichier, Image_interface &image_interface)
{
    char            caractere;
    std::string     string;
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
                    image_interface.image=moteurGraphique->AjouterImage(string,-1);
                    break;
                case 'x' :
                    fichier>>image_interface.position.x;
                    break;
                case 'y' :
                    fichier>>image_interface.position.y;
                    break;
                case 'w' :
                    fichier>>image_interface.position.w;
                    break;
                case 'h' :
                    fichier>>image_interface.position.h;
                    break;
                }

                if (fichier.eof())
                {
                    console->Ajouter("Erreur : Classe Invalide",1);
                    caractere='$';
                }
            }
            while (caractere!='$');
            fichier.get(caractere);
        }
        if (fichier.eof())
        {
            console->Ajouter("Erreur : Classe Invalide",1);
            caractere='$';
        }

    }
    while (caractere!='$');
}

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
                console->Ajouter("Erreur : Classe \""+chemin+"\" Invalide",1);
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
                        console->Ajouter("Erreur : Classe \""+chemin+"\" Invalide",1);
                        caractere='$';
                    }

                }
                while (caractere!='$');
                fichier.get(caractere);
            }
            if (fichier.eof())
            {
                console->Ajouter("Erreur : Classe \""+chemin+"\" Invalide",1);
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
                        console->Ajouter("Erreur : Classe \""+chemin+"\" Invalide",1);
                        caractere='$';
                    }

                }
                while (caractere!='$');
                fichier.get(caractere);
            }
            if (fichier.eof())
            {
                console->Ajouter("Erreur : Classe \""+chemin+"\" Invalide",1);
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
                        console->Ajouter("Erreur : Classe \""+chemin+"\" Invalide",1);
                        caractere='$';
                    }

                }
                while (caractere!='$');
                fichier.get(caractere);
            }
            if (fichier.eof())
            {
                console->Ajouter("Erreur : Classe \""+chemin+"\" Invalide",1);
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
                        console->Ajouter("Erreur : Classe \""+chemin+"\" Invalide",1);
                        caractere='$';
                    }

                }
                while (caractere!='$');
                fichier.get(caractere);
            }
            if (fichier.eof())
            {
                console->Ajouter("Erreur : Classe \""+chemin+"\" Invalide",1);
                caractere='$';
            }

        }
        while (caractere!='$');

        ChargerImageInterface(fichier, inventaire);
        ChargerImageInterface(fichier, menu_marchand);
        ChargerImageInterface(fichier, hud);
        ChargerImageInterface(fichier, orbe_vie);
        ChargerImageInterface(fichier, orbe_foi);
        ChargerImageInterface(fichier, cache_vie);
        ChargerImageInterface(fichier, cache_foi);
        ChargerImageInterface(fichier, plus_button);
        ChargerImageInterface(fichier, scroll_button);

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
                        console->Ajouter("Erreur : Classe \""+chemin+"\" Invalide",1);
                        caractere='$';
                    }

                }
                while (caractere!='$');
                fichier.get(caractere);
            }
            if (fichier.eof())
            {
                console->Ajouter("Erreur : Classe \""+chemin+"\" Invalide",1);
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
                        console->Ajouter("Erreur : Classe \""+chemin+"\" Invalide",1);
                        caractere='$';
                    }

                }
                while (caractere!='$');
                fichier.get(caractere);
            }
            if (fichier.eof())
            {
                console->Ajouter("Erreur : Classe \""+chemin+"\" Invalide",1);
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
                        console->Ajouter("Erreur : Classe \""+chemin+"\" Invalide",1);
                        caractere='$';
                    }

                }
                while (caractere!='$');
                fichier.get(caractere);
            }
            if (fichier.eof())
            {
                console->Ajouter("Erreur : Classe \""+chemin+"\" Invalide",1);
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
                        console->Ajouter("Erreur : Classe \""+chemin+"\" Invalide",1);
                        caractere='$';
                    }

                }
                while (caractere!='$');
                fichier.get(caractere);
            }
            if (fichier.eof())
            {
                console->Ajouter("Erreur : Classe \""+chemin+"\" Invalide",1);
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
                    if (caractere=='m')
                    {
                        std::string temp;
                        fichier>>temp;
                        miracles.push_back(Miracle (temp));
                    }

                    if (fichier.eof())
                    {
                        console->Ajouter("Erreur : Classe \""+chemin+"\" Invalide",1);
                        caractere='$';
                    }
                }
                while (caractere!='$');
                fichier.get(caractere);
            }
            if (fichier.eof())
            {
                console->Ajouter("Erreur : Classe \""+chemin+"\" Invalide",1);
                caractere='$';
            }
        }
        while (caractere!='$');

        fichier.close();
    }
    else
        console->Ajouter("Impossible de charger : "+chemin,1);
}


