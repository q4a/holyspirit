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

bool Bouton_pressoire::Survol()
{
    if(no_opacity
    && eventManager->getPositionSouris().x > position.x
    && eventManager->getPositionSouris().x < position.x + position.w
    && eventManager->getPositionSouris().y > position.y
    && eventManager->getPositionSouris().y < position.y + position.h)
        return (true);

    else if(!no_opacity
    && eventManager->getPositionSouris().x > AutoScreenAdjust(position.x,0).x
    && eventManager->getPositionSouris().x < AutoScreenAdjust(position.x,0).x + position.w
    && eventManager->getPositionSouris().y > AutoScreenAdjust(0,position.y).y
    && eventManager->getPositionSouris().y < AutoScreenAdjust(0,position.y).y + position.h)
        return (true);

    return (false);
}

sf::Sprite Bouton_pressoire::Afficher(float decalage)
{
    MySprite sprite;
    sprite.setTexture(*moteurGraphique->getImage(image.image));

    sprite.setTextureRect(sf::IntRect(  image.position.x, image.position.y,
                                    image.position.w, image.position.h));

    sprite.Resize(position.w,position.h);

    sprite.setPosition(AutoScreenAdjust(position.x,0).x,
                       AutoScreenAdjust(0,position.y - decalage).y);

    bool hover = false;

    if(Survol())
        if(no_opacity || sprite.getTexture()->copyToImage().getPixel(eventManager->getPositionSouris().x - (int)sprite.getPosition().x,
                                         eventManager->getPositionSouris().y - (int)sprite.getPosition().y).a > 0)
            hover = true;

    if(!eventManager->getEvenement(sf::Mouse::Left,EventClicA))
    {
        if(!m_hover && hover)
            moteurSons->JouerSon(configuration->sound_dialog,coordonnee (0,0),0);
        m_hover = hover;
    }

    if(hover && m_press && !eventManager->getEvenement(sf::Mouse::Left,EventClicA))
        m_press = false, m_action = true,
        moteurSons->JouerSon(configuration->sound_menu,coordonnee (0,0),0);

    if(hover && m_hover && eventManager->getEvenement(sf::Mouse::Left,EventClicA))
        m_press = true;

    if(!eventManager->getEvenement(sf::Mouse::Left,EventClicA))
        m_press = false;

    if(m_hover)
    {
        sprite.setTexture(*moteurGraphique->getImage(image_hover.image));
        sprite.setTextureRect(sf::IntRect(  image_hover.position.x, image_hover.position.y,
                                        image_hover.position.w, image_hover.position.h));
    }
    if(m_press)
    {
        sprite.setTexture(*moteurGraphique->getImage(image_press.image));
        sprite.setTextureRect(sf::IntRect(  image_press.position.x, image_press.position.y,
                                        image_press.position.w, image_press.position.h));
    }

    return sprite;
}

void Border::Afficher(coordonnee pos, coordonnee size, int couche, sf::Color color)
{
    MySprite sprite;
    sprite.setColor(color);
    sprite.setTexture(*moteurGraphique->getImage(image_lu.image));
    sprite.setTextureRect(sf::IntRect(  image_lu.position.x, image_lu.position.y,
                                    image_lu.position.w, image_lu.position.h));
    sprite.setPosition(pos.x - image_lu.position.w,
                       pos.y - image_lu.position.h);
    moteurGraphique->AjouterCommande(&sprite,couche,false);

    sprite.setTexture(*moteurGraphique->getImage(image_ru.image));
    sprite.setTextureRect(sf::IntRect(  image_ru.position.x, image_ru.position.y,
                                    image_ru.position.w, image_ru.position.h));
    sprite.setPosition(pos.x + size.x ,
                       pos.y - image_ru.position.h);
    moteurGraphique->AjouterCommande(&sprite,couche,false);

    sprite.setTexture(*moteurGraphique->getImage(image_ld.image));
    sprite.setTextureRect(sf::IntRect(  image_ld.position.x, image_ld.position.y,
                                    image_ld.position.w, image_ld.position.h));
    sprite.setPosition(pos.x - image_ld.position.w,
                       pos.y + size.y);
    moteurGraphique->AjouterCommande(&sprite,couche,false);

    sprite.setTexture(*moteurGraphique->getImage(image_rd.image));
    sprite.setTextureRect(sf::IntRect(  image_rd.position.x, image_rd.position.y,
                                    image_rd.position.w, image_rd.position.h));
    sprite.setPosition(pos.x + size.x,
                       pos.y + size.y);
    moteurGraphique->AjouterCommande(&sprite,couche,false);



    sprite.setTexture(*moteurGraphique->getImage(image_u.image));
    sprite.setTextureRect(sf::IntRect(  image_u.position.x, image_u.position.y,
                                    image_u.position.w, image_u.position.h));
    sprite.setPosition(pos.x,pos.y - image_u.position.h);
    sprite.Resize(size.x,image_u.position.h);
    moteurGraphique->AjouterCommande(&sprite,couche,false);

    sprite.setTexture(*moteurGraphique->getImage(image_d.image));
    sprite.setTextureRect(sf::IntRect(  image_d.position.x, image_d.position.y,
                                    image_d.position.w, image_d.position.h));
    sprite.setPosition(pos.x,pos.y + size.y);
    sprite.Resize(size.x,image_u.position.h);
    moteurGraphique->AjouterCommande(&sprite,couche,false);

    sprite.setTexture(*moteurGraphique->getImage(image_l.image));
    sprite.setTextureRect(sf::IntRect(  image_l.position.x, image_l.position.y,
                                    image_l.position.w, image_l.position.h));
    sprite.setPosition(pos.x - image_l.position.w,pos.y);
    sprite.Resize(image_l.position.w,size.y);
    moteurGraphique->AjouterCommande(&sprite,couche,false);

    sprite.setTexture(*moteurGraphique->getImage(image_r.image));
    sprite.setTextureRect(sf::IntRect(  image_r.position.x, image_r.position.y,
                                    image_r.position.w, image_r.position.h));
    sprite.setPosition(pos.x + size.x,pos.y);
    sprite.Resize(image_r.position.w,size.y);
    moteurGraphique->AjouterCommande(&sprite,couche,false);



    sprite.setTexture(*moteurGraphique->getImage(image_c.image));
    sprite.setTextureRect(sf::IntRect(  image_c.position.x, image_c.position.y,
                                    image_c.position.w, image_c.position.h));
    sprite.setPosition(pos.x,pos.y);
    sprite.Resize(size.x,size.y);
    moteurGraphique->AjouterCommande(&sprite,couche,false);
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


void                ChargerBouton(ifstream &fichier, Bouton_pressoire &bouton)
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

                    int no;
                    fichier>>no;
                    bouton.nom = configuration->getText(0, no);

                    break;
                case 'x' :
                    fichier>>bouton.position.x;
                    break;
                case 'y' :
                    fichier>>bouton.position.y;
                    break;
                case 'w' :
                    fichier>>bouton.position.w;
                    break;
                case 'h' :
                    fichier>>bouton.position.h;
                    break;
                case 'i' :

                        fichier.get(caractere);

                        if(caractere == 'n')
                            ChargerImageInterface(fichier, bouton.image);
                        else if(caractere == 'h')
                            ChargerImageInterface(fichier, bouton.image_hover);
                        else if(caractere == 'p')
                            ChargerImageInterface(fichier, bouton.image_press);
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
void                ChargerBorder(ifstream &fichier, Border &border)
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
                    case 't' :

                        fichier.get(caractere);

                        if(caractere == 'e')
                            ChargerImageInterface(fichier, border.image_c);
                        else if(caractere == 'b')
                        {
                            fichier.get(caractere);
                            if(caractere == 'u')
                                ChargerImageInterface(fichier, border.image_u);
                            else if(caractere == 'd')
                                ChargerImageInterface(fichier, border.image_d);
                            else if(caractere == 'r')
                                ChargerImageInterface(fichier, border.image_r);
                            else if(caractere == 'l')
                                ChargerImageInterface(fichier, border.image_l);
                        }
                        else if(caractere == 'c')
                        {
                            fichier.get(caractere);

                            if(caractere == 'r')
                            {
                                fichier.get(caractere);
                                if(caractere == 'u')
                                    ChargerImageInterface(fichier, border.image_ru);
                                else if(caractere == 'd')
                                    ChargerImageInterface(fichier, border.image_rd);
                            }
                            else if(caractere == 'l')
                            {
                                fichier.get(caractere);
                                if(caractere == 'u')
                                    ChargerImageInterface(fichier, border.image_lu);
                                else if(caractere == 'd')
                                    ChargerImageInterface(fichier, border.image_ld);
                            }

                        }
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
    weaponsSet = 0;

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
        case 'd' :
            fichier>>weaponsSet;
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
    sprite.setTexture(*moteurGraphique->getImage(image));
    sprite.setPosition(AutoScreenAdjust(position.x-2,
                                        position.y-2, decalage));
    moteurGraphique->AjouterCommande(&sprite, 15, 0);

    if(empty)
    {
        sprite.setTexture(*moteurGraphique->getImage(image_empty));
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
                fichier>>description;
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
                std::string buf;
                fichier>>buf;
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


        do
        {
            fichier.get(caractere);
            if (caractere=='*')
            {
                std::string temp;
                fichier>>temp;
                m_sound_death = moteurSons->AjouterBuffer(temp);
            }
            if (fichier.eof())
            {
                console->Ajouter("Erreur : Classe \""+chemin+"\" Invalide",1);
                caractere='$';
            }

        }
        while (caractere!='$');

        ChargerBorder(fichier, border);

        ChargerImageInterface(fichier, inventaire);
        ChargerImageInterface(fichier, interface_miracles);
        ChargerImageInterface(fichier, menu_marchand);
        ChargerImageInterface(fichier, hud);
        ChargerImageInterface(fichier, orbe_vie);
        ChargerImageInterface(fichier, orbe_foi);

        ChargerBouton(fichier, plus_button[0]);

        plus_button[1] = plus_button[0];
        plus_button[2] = plus_button[0];
        plus_button[3] = plus_button[0];
        plus_button[4] = plus_button[0];

        ChargerImageInterface(fichier, scroll_button);

        ChargerBouton(fichier, sort_inventory);

        ChargerImageInterface(fichier, talk);
        ChargerImageInterface(fichier, bullet_off);
        ChargerImageInterface(fichier, bullet_on);
        ChargerImageInterface(fichier, quest);
        ChargerImageInterface(fichier, documents);
        ChargerImageInterface(fichier, craft);
        ChargerImageInterface(fichier, icone_mm);
        ChargerImageInterface(fichier, soul_bar);

        miracles_plus_button.push_back(Bouton_pressoire ());
        ChargerBouton(fichier, miracles_plus_button.front());

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
        ChargerImageInterface(fichier, scroll_button_quest);
        ChargerCoordonneeInterface(fichier, position_list_docs);
        ChargerCoordonneeInterface(fichier, position_contenu_docs);
        ChargerImageInterface(fichier, scroll_button_ldocs);
        ChargerImageInterface(fichier, scroll_button_cdocs);

        for(int i = 0 ; i < 8 ; ++i)
            ChargerCoordonneeInterface(fichier, position_raccourcis[i]);

        ChargerImageInterface(fichier, p_miracleG);
        ChargerImageInterface(fichier, p_miracleD);

        ChargerBouton(fichier, boutons_menus_hud);
        ChargerBouton(fichier, boutons_menus_hud_2);

        ChargerBouton(fichier, boutons_menus_weapons);
        ChargerBouton(fichier, boutons_menus_weapons_2);
        ChargerBouton(fichier, boutons_menus_weapons_t);
        ChargerBouton(fichier, boutons_menus_weapons_t_2);

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
        ChargerBouton(fichier, onglets_miracles);

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

                miracles_plus_button.back().position.x = position_miracles.back().x + miracles_plus_button.front().position.x;
                miracles_plus_button.back().position.y = position_miracles.back().y + miracles_plus_button.front().position.y;


                miracles_plus_button.push_back(Bouton_pressoire ());
                miracles_plus_button.back() = miracles_plus_button.front();

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


