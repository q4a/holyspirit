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

struct Image_interface
{
    coordonnee position;
    coordonnee centre;
    int image;
};

struct Bouton
{
    coordonnee position;
    std::string nom;
    int lien;
    Image_interface image;

    bool Survol();
};

struct Emplacement_inventaire
{
    coordonnee position;
    int emplacement;
    int image;
    int image_empty;
    bool empty;

    void Charger(std::ifstream &fichier);
    void Afficher(float);
};

struct Classe
{
    void Charger(const std::string &chemin, const std::vector<int> &lvl_miracles, const Caracteristique &caract);

    int nom;
    Caracteristique caracteristique;

    std::string chemin_modele;

    std::vector<std::string> equipementParDefaut;
    std::string modeleNu[NOMBRE_MORCEAU_PERSONNAGE][3];

    Image_interface inventaire;
    Image_interface interface_miracles;
    Image_interface menu_marchand;
    Image_interface hud;
    Image_interface orbe_vie;
    Image_interface orbe_foi;
    Image_interface plus_button;
    Image_interface scroll_button;
    Image_interface sort_inventory;
    Image_interface talk;
    Image_interface quest;
    Image_interface documents;
    Image_interface craft;
    Image_interface icone_mm;
    Image_interface soul_bar;
    Image_interface hud_pt_caract_rest;
    Image_interface hud_pt_miracle_rest;
    Image_interface miracles_plus_button;
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

    std::vector <Emplacement_inventaire> emplacements;

    std::vector<Miracle> miracles;
    std::vector<coordonnee> position_miracles;
    std::vector<int> page_miracles;
    std::vector<Image_interface> icone_miracles;
};
#endif
