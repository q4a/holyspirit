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


#ifndef CLASSEH
#define CLASSEH

#include "miracle.h"
#include "../Moteurs/moteurGraphique.h"

struct Bouton
{
    coordonnee position;
    std::string nom;
    int lien;
    Image_interface image;

    bool Survol();
};

struct Bouton_pressoire
{
    Bouton_pressoire ()
    {
        m_hover     = false;
        m_press     = false;
        m_action    = false;

        no_opacity  = false;
    }

    coordonnee position;
    std::string nom;

    bool m_hover;
    bool m_press;
    bool m_action;

    bool no_opacity;

    Image_interface image;
    Image_interface image_hover;
    Image_interface image_press;

    bool Survol();
    sf::Sprite Afficher(float decalage);
};

struct Emplacement_inventaire
{
    coordonnee position;
    int emplacement;
    int image;
    int image_empty;
    bool empty;

    bool weaponsSet;

    void Charger(std::ifstream &fichier);
    void Afficher(float);
};

struct Classe
{
    void Charger(const std::string &chemin, const std::vector<int> &lvl_miracles, const Caracteristique &caract);

    int nom, description;
    Caracteristique caracteristique;

    std::string chemin_modele;

    std::vector<std::string> equipementParDefaut;
    std::string modeleNu[NOMBRE_MORCEAU_PERSONNAGE][3];

    Border border;

    Image_interface inventaire;
    Image_interface interface_miracles;
    Image_interface menu_marchand;
    Image_interface hud;
    Image_interface orbe_vie;
    Image_interface orbe_foi;

    Bouton_pressoire plus_button[5];

    Image_interface scroll_button;

    Bouton_pressoire sort_inventory;

    Image_interface talk;
    Image_interface bullet_on;
    Image_interface bullet_off;
    Image_interface quest;
    Image_interface documents;
    Image_interface craft;
    Image_interface icone_mm;
    Image_interface soul_bar;

    std::vector <Bouton_pressoire> miracles_plus_button;

    Image_interface miracles_cadre;
    Image_interface barre_vie_monstre;
    Image_interface barre_vie_monstre_vide;
    //std::vector<Image_interface> interface_miracles;

    std::vector<Bouton> boutons_miracles;

    coordonnee position_raccourcis[8];
    coordonnee position_miracleALancerDroite;
    coordonnee position_miracleALancerGauche;

    coordonnee position_sac_inventaire;
    coordonnee position_contenu_inventaire;
    coordonnee position_contenu_marchand;

    Emplacement_inventaire schema_craft;
    Emplacement_inventaire result_craft;
    Emplacement_inventaire button_craft;

    coordonnee position_contenu_dialogue;
    coordonnee position_contenu_quetes;
    Image_interface scroll_button_quest;
    coordonnee position_bouton_dialogue;
    coordonnee position_contenu_description_quete;
    coordonnee position_list_docs;
    coordonnee position_contenu_docs;
    Image_interface scroll_button_ldocs;
    Image_interface scroll_button_cdocs;
    coordonnee position_points_miracles;

    std::vector<Bouton>  boutons_menus_hud;
    std::vector<Bouton>  boutons_menus_hud_2;


    std::vector<Bouton>  boutons_menus_weapons;
    std::vector<Bouton>  boutons_menus_weapons_2;
    std::vector<Bouton>  boutons_menus_weapons_t;
    std::vector<Bouton>  boutons_menus_weapons_t_2;

    std::vector <Emplacement_inventaire> emplacements;

    std::vector<Miracle> miracles;
    std::vector<coordonnee> position_miracles;
    std::vector<int> page_miracles;
    std::vector<Image_interface> icone_miracles;
};
#endif
