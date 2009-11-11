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
};

struct Emplacement_inventaire
{
    coordonnee position;
    int emplacement;
};

struct Classe
{
    void Charger(std::string chemin, const std::vector<int> &lvl_miracles, const Caracteristique &caract);

    int ID;

    std::string nom;
    Caracteristique caracteristique;

    std::string chemin_modele;

    std::vector<std::string> equipementParDefaut;
    std::string modeleNu[NOMBRE_MORCEAU_PERSONNAGE][3];

    Image_interface inventaire;
    Image_interface menu_marchand;
    Image_interface hud;
    Image_interface orbe_vie;
    Image_interface orbe_foi;
    Image_interface cache_vie;
    Image_interface cache_foi;
    Image_interface plus_button;
    Image_interface scroll_button;
    Image_interface talk;
    Image_interface quest;
    Image_interface arrow;
    std::vector<Image_interface> interface_miracles;

    std::vector<Bouton> boutons_miracles;

    coordonnee position_sac_inventaire;
    coordonnee position_contenu_inventaire;
    coordonnee position_contenu_marchand;

    coordonnee position_contenu_dialogue;
    coordonnee position_contenu_quetes;
    coordonnee position_contenu_description_quete;

    coordonnee position_points_miracles;

    std::vector <Emplacement_inventaire> emplacements;

    std::vector<Miracle> miracles;
    std::vector<coordonnee> position_miracles;
    std::vector<int> page_miracles;
};
#endif
