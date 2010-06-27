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

#include <sstream>

using namespace std;

inline sf::Vector2f AutoScreenAdjust(float x, float y, float decalage = 0)
{
    sf::Vector2f temp;
    temp.x = x + (configuration->Resolution.x - 800) * 0.5;
    temp.y = y + (configuration->Resolution.y - 600) - decalage * configuration->Resolution.h/600;
    return temp;
}

bool Bouton::Survol()
{
    if(eventManager->getPositionSouris().x > AutoScreenAdjust(position.x,0).x
    && eventManager->getPositionSouris().x < AutoScreenAdjust(position.x,0).x + position.w
    && eventManager->getPositionSouris().y > AutoScreenAdjust(0,position.y).y
    && eventManager->getPositionSouris().y < AutoScreenAdjust(0,position.y).y + position.h)
        return (true);

    return (false);
}

void                ChargerImageInterface(ifstream &fichier, Image_interface &image_interface)
{
    char            caractere;
    std::string     string;

    //image_interface.centre = coordonnee();
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
                case 'c' :
                    fichier.get(caractere);
                    if(caractere == 'x')
                        fichier>>image_interface.centre.x;
                    if(caractere == 'y')
                        fichier>>image_interface.centre.y;
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


void                ChargerImageInterface(ifstream &fichier, std::vector <Image_interface> &image_interface)
{
    char            caractere;
    std::string     string;
    do
    {
        fichier.get(caractere);
        if (caractere=='*')
        {
            image_interface.push_back(Image_interface ());
            do
            {
                fichier.get(caractere);
                switch (caractere)
                {
                case 'm' :
                    fichier>>string;
                    image_interface.back().image=moteurGraphique->AjouterImage(string,-1);
                    break;
                case 'x' :
                    fichier>>image_interface.back().position.x;
                    break;
                case 'y' :
                    fichier>>image_interface.back().position.y;
                    break;
                case 'w' :
                    fichier>>image_interface.back().position.w;
                    break;
                case 'h' :
                    fichier>>image_interface.back().position.h;
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

void                ChargerBouton(ifstream &fichier, std::vector <Bouton> &bouton)
{
    char            caractere;
    std::string     string;
    do
    {
        fichier.get(caractere);
        if (caractere=='*')
        {
            bouton.push_back(Bouton ());
            do
            {
                fichier.get(caractere);
                switch (caractere)
                {
                case 'm' :

                    int no;
                    fichier>>no;
                    bouton.back().nom = configuration->getText(0, no);

                    break;
                case 'e' :
                    fichier>>bouton.back().lien;
                    break;
                case 'x' :
                    fichier>>bouton.back().position.x;
                    break;
                case 'y' :
                    fichier>>bouton.back().position.y;
                    break;
                case 'w' :
                    fichier>>bouton.back().position.w;
                    break;
                case 'h' :
                    fichier>>bouton.back().position.h;
                    break;
                case 'i' :
                        ChargerImageInterface(fichier, bouton.back().image);
                        fichier.get(caractere);
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


void                ChargerCoordonneeInterface(ifstream &fichier, coordonnee &coord_interface)
{
    char            caractere;

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
                    fichier>>coord_interface.x;
                    break;
                case 'y' :
                    fichier>>coord_interface.y;
                    break;
                case 'w' :
                    fichier>>coord_interface.w;
                    break;
                case 'h' :
                    fichier>>coord_interface.h;
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

void Emplacement_inventaire::Charger(ifstream &fichier)
{
    char caractere;
    std::string buf;
    do
    {
        fichier.get(caractere);
        switch (caractere)
        {
        case 'e' :
            fichier>>emplacement;
            break;
        case 'x' :
            fichier>>position.x;
            break;
        case 'y' :
            fichier>>position.y;
            break;
        case 'w' :
            fichier>>position.w;
            break;
        case 'h' :
            fichier>>position.h;
            break;
        case 'n' :
            fichier>>buf;
            image_empty = moteurGraphique->AjouterImage(buf,-1);
            break;
        case 'm' :
            fichier>>buf;
            image = moteurGraphique->AjouterImage(buf,-1);
            break;
        }

        if (fichier.eof())
        {
            console->Ajouter("Erreur : Classe Invalide",1);
            caractere='$';
        }

    }
    while (caractere!='$');
}

void Emplacement_inventaire::Afficher(float decalage)
{
    sf::Sprite sprite;
    sprite.SetImage(*moteurGraphique->getImage(image));
    sprite.SetPosition(AutoScreenAdjust(position.x-2,
                                        position.y-2, decalage));
    moteurGraphique->AjouterCommande(&sprite, 15, 0);

    if(empty)
    {
        sprite.SetImage(*moteurGraphique->getImage(image_empty));
        moteurGraphique->AjouterCommande(&sprite, 15, 0);
    }
}


void Classe::Charger(const std::string &chemin, const std::vector<int> &lvl_miracles, const Caracteristique &caract)
{
    emplacements.clear();
    equipementParDefaut.clear();

  //  interface_miracles.clear();

    boutons_miracles.clear();

    miracles.clear();
    position_miracles.clear();


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
                        equipementParDefaut.push_back(string ());
                        fichier>>equipementParDefaut.back();
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


        {
            int o = -1;
            do
            {
                fichier.get(caractere);
                if (caractere=='*')
                {
                    ++o;
                    if(o > NOMBRE_MORCEAU_PERSONNAGE)
                        o = NOMBRE_MORCEAU_PERSONNAGE;

                    int temp=0;
                    do
                    {
                        fichier.get(caractere);
                        switch (caractere)
                        {
                        case 'm' :
                            fichier>>modeleNu[o][temp];
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
                    caractere = ' ';
                }
                if (fichier.eof())
                {
                    console->Ajouter("Erreur : Classe \""+chemin+"\" Invalide",1);
                    caractere='$';
                }

            }
            while (caractere!='$');
        }

        ChargerImageInterface(fichier, inventaire);
        ChargerImageInterface(fichier, interface_miracles);
        ChargerImageInterface(fichier, menu_marchand);
        ChargerImageInterface(fichier, hud);
        ChargerImageInterface(fichier, orbe_vie);
        ChargerImageInterface(fichier, orbe_foi);
        ChargerImageInterface(fichier, plus_button);
        ChargerImageInterface(fichier, scroll_button);
        ChargerImageInterface(fichier, sort_inventory);
        ChargerImageInterface(fichier, talk);
        ChargerImageInterface(fichier, quest);
        ChargerImageInterface(fichier, craft);
        ChargerImageInterface(fichier, arrow);
        ChargerImageInterface(fichier, icone_mm);
        ChargerImageInterface(fichier, soul_bar);
        ChargerImageInterface(fichier, hud_pt_caract_rest);
        ChargerImageInterface(fichier, hud_pt_miracle_rest);
        ChargerImageInterface(fichier, miracles_plus_button);
        ChargerImageInterface(fichier, miracles_cadre);
        ChargerImageInterface(fichier, barre_vie_monstre);
        ChargerImageInterface(fichier, barre_vie_monstre_vide);

        ChargerCoordonneeInterface(fichier, position_sac_inventaire);
        ChargerCoordonneeInterface(fichier, position_contenu_inventaire);
        ChargerCoordonneeInterface(fichier, position_contenu_marchand);
        ChargerCoordonneeInterface(fichier, position_contenu_dialogue);
        ChargerCoordonneeInterface(fichier, position_bouton_dialogue);
        ChargerCoordonneeInterface(fichier, position_contenu_quetes);
        ChargerCoordonneeInterface(fichier, position_contenu_description_quete);

        for(int i = 0 ; i < 8 ; ++i)
            ChargerCoordonneeInterface(fichier, position_raccourcis[i]);

        ChargerCoordonneeInterface(fichier, position_miracleALancerGauche);
        ChargerCoordonneeInterface(fichier, position_miracleALancerDroite);

        ChargerBouton(fichier, boutons_menus_hud);

        do
        {
            fichier.get(caractere);
            if (caractere=='*')
            {
                schema_craft.Charger(fichier);
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
                result_craft.Charger(fichier);
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
                button_craft.Charger(fichier);
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
                emplacements.back().Charger(fichier);
                fichier.get(caractere);
            }
            if (fichier.eof())
            {
                console->Ajouter("Erreur : Classe \""+chemin+"\" Invalide",1);
                caractere='$';
            }

        }
        while (caractere!='$');

        ChargerCoordonneeInterface(fichier, position_points_miracles);

        ChargerBouton(fichier, boutons_miracles);

        do
        {
            fichier.get(caractere);
            if (caractere=='*')
            {
                int buf = -1;
                std::string temp;
                position_miracles.push_back(coordonnee ());
                page_miracles.push_back(-1);
                icone_miracles.push_back(Image_interface());

                do
                {
                    fichier.get(caractere);
                    if (caractere=='m')
                    {
                        int level = 0;

                        fichier>>temp;

                        if(lvl_miracles.size() > miracles.size())
                            level = lvl_miracles[miracles.size()];

                        miracles.push_back(Miracle (temp, caract, level));

                        //if(!miracles.back().m_max == )
                        {
                            Miracle mir(temp, caract, level + 1);
                            for(int i = 0 ; i < (int)mir.m_description_effets.size() ; ++i)
                                miracles.back().m_description_effets_suivant.push_back(mir.m_description_effets[i]);

                            miracles.back().m_coutFoi_suivant = mir.m_coutFoi;
                            miracles.back().m_reserveFoi_suivant = mir.m_reserveFoi;
                            miracles.back().m_coutVie_suivant = mir.m_coutVie;
                            miracles.back().m_reserveVie_suivant = mir.m_reserveVie;
                        }

                    }

                    if(caractere == 'x')
                        fichier>>position_miracles.back().x;
                    if(caractere == 'y')
                        fichier>>position_miracles.back().y;
                    if(caractere == 'h')
                        fichier>>position_miracles.back().h;
                    if(caractere == 'w')
                        fichier>>position_miracles.back().w;

                    if(caractere == 'e')
                        fichier>>page_miracles.back();

                    if(caractere == 'i')
                    {
                        ChargerImageInterface(fichier, icone_miracles.back());
                        fichier.get(caractere);
                    }


                    if(caractere == 'b')
                        fichier>>buf;

                    if (fichier.eof())
                    {
                        console->Ajouter("Erreur : Classe \""+chemin+"\" Invalide",1);
                        caractere='$';
                    }
                }
                while (caractere!='$');

                miracles.back().m_buf = buf;

                if(lvl_miracles.size() >= miracles.size())
                    if(miracles.back().m_buf != -1 && lvl_miracles[miracles.size()-1] > 0)
                        miracles[miracles.back().m_buf].Concatenencer(temp, caract, lvl_miracles[miracles.size()-1]);

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


