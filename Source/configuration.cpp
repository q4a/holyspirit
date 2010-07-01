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



#include "configuration.h"
#include <stdio.h>

void Configuration::Charger()
{
    error = "Error";

    zoom=1;

    ChargerConf();
    ChargerInit();
    ChargerTxt();

    RafraichirOmbre=0;
    RafraichirLumiere=0;

    jour = 0;

    if (!Lumiere)
        Ombre=0;



}

void Configuration::ChargerConf()
{
    std::ifstream fichier;

    fichier.open("configuration.conf", std::ios::in);
    if (fichier)
    {
        std::string chaine;
        while (fichier>>chaine)
        {
            if (chaine== "resolution:")
            {
                fichier>>Resolution.x>>Resolution.y;
                Resolution.w=Resolution.x;
                Resolution.h=Resolution.y;
            }
            if (chaine== "brightness:")
                fichier>>luminosite;
            if (chaine== "contrast:")
                fichier>>contrastes;
            if (chaine== "alpha_opacity:")
                fichier>>alpha;
            if (chaine== "window:")
                fichier>>mode_fenetre;
            if (chaine== "videos:")
                fichier>>video;
            if (chaine== "postFX:")
                fichier>>postFX;
            if (chaine== "smooth:")
                fichier>>lissage;
            if (chaine== "shadows:")
                fichier>>Ombre;
            if (chaine== "reflection:")
                fichier>>Reflection;
            if (chaine== "distortion:")
                fichier>>Distortion;
            if (chaine== "light_quality:")
                fichier>>Lumiere;
            if (chaine== "grass:")
                fichier>>Herbes;
            if (chaine== "particules:")
                fichier>>particules;
            if (chaine== "vsync:")
                fichier>>syncronisation_verticale;
            if (chaine== "zoom:")
                fichier>>zoom;
            if (chaine== "volume:")
                fichier>>volume;
            if (chaine== "music_volume:")
                fichier>>music_volume;
            if (chaine== "minimap:")
                fichier>>Minimap;
            if (chaine== "light_refresh_rate:")
                fichier>>frequence_lumiere;
            if (chaine== "console:")
                fichier>>console;
            if (chaine== "item_background:")
                fichier>>item_background;

            if (chaine== "saving_frequency:")
                fichier>>frequence_sauvegarde;
            if (chaine== "debug_mod:")
                fichier>>debug;
            if (chaine== "desactivate_console:")
                fichier>>desactivate_console;
            if (chaine== "language:")
                fichier>>language;
        }
        fichier.close();
    }
    else
        throw std::string("Impossible de charger la configuration : configuration.conf");
}
void Configuration::ChargerInit()
{
    std::ifstream fichier;

    fichier.open("holyspirit.ini", std::ios::in);
    if (fichier)
    {
        std::string chaine;
        while (fichier>>chaine)
        {
            if (chaine== "version:")
                fichier>>version;
            if (chaine== "maps:")
                fichier>>chemin_maps;
            if (chaine== "items:")
                fichier>>chemin_items;
            if (chaine== "temps:")
                fichier>>chemin_temps;
            if (chaine== "saves:")
                fichier>>chemin_saves;
            if (chaine== "events:")
                fichier>>chemin_evenements;
            if (chaine== "cursor:")
                fichier>>chemin_curseurs;
            if (chaine== "menus:")
                fichier>>chemin_menus;
            if (chaine== "fonts:")
                fichier>>chemin_fonts;
            if (chaine== "fx:")
                fichier>>chemin_fx;
            if (chaine== "alpha_arts:")
                fichier>>chemin_aa;
            if (chaine== "music_menu:")
                fichier>>music_menu;
            if (chaine== "mainscreen_menu:")
                fichier>>mainscreen_menu;

            if (chaine== "basic_cursor:")
                fichier>>nom_curseur_base;
            if (chaine== "fxBlack:")
                fichier>>nom_effetNoir;
            if (chaine== "fxDeath:")
                fichier>>nom_effetMort;
            if (chaine== "fxContrast:")
                fichier>>nom_effetContrastes;
            if (chaine== "fxBlur:")
                fichier>>nom_effetBlur;
            if (chaine== "fxFilter:")
                fichier>>nom_effetFiltre;
            if (chaine== "fxWater:")
                fichier>>nom_effetWater;
            if (chaine== "fxDistortion:")
                fichier>>nom_effetDistortion;

            if (chaine== "fxWater_image:")
                fichier>>water_map;

            if (chaine== "menu_corner:")
                fichier>>nom_corner;
            if (chaine== "menu_item_background:")
                fichier>>nom_item_background;
            if (chaine== "menu_item_unusable:")
                fichier>>nom_item_unusable;

            if (chaine== "ambient_shadow_tileset:")
                fichier>>ambientShadow_tileset;

            if (chaine== "localisation:")
                fichier>>chemin_localisation;
            if (chaine== "text_benedictions:")
                fichier>>chemin_text_benedictions;
            if (chaine== "text_menus:")
                fichier>>chemin_text_menus;
            if (chaine== "text_items:")
                fichier>>chemin_text_items;
            if (chaine== "text_entities:")
                fichier>>chemin_text_entities;
            if (chaine== "text_dialogs:")
                fichier>>chemin_text_dialogs;
            if (chaine== "text_maps:")
                fichier>>chemin_text_maps;
            if (chaine== "text_miracles:")
                fichier>>chemin_text_miracles;

            if (chaine== "font_title:")
                fichier>>font_titre;
            if (chaine== "font:")
                fichier>>font;
            if (chaine== "cache: ")
                fichier>>cache;
            if (chaine== "death_sound:")
                fichier>>chemin_son_mort;

            if (chaine== "player_class:")
            {
                player_class.push_back("");
                fichier>>player_class.back();
            }

        }
        fichier.close();
    }
    else
        throw std::string("Impossible de charger la configuration : holyspirit.ini");
}

std::vector<std::string> Configuration::ChargerFichierTxt(std::string chemin)
{
    std::vector<std::string> text;

    std::ifstream fichier;
    fichier.open((chemin_localisation+language+"/"+chemin).c_str(), std::ios::in);
    if (fichier)
    {
        std::string chaine;
        while(!fichier.eof())
        {
            getline(fichier,chaine,'\n');
            #ifndef LINUX
                text.push_back( chaine.substr(0, chaine.size()) );
            #else
                text.push_back( chaine.substr(0, chaine.size() - 1) );
            #endif
        }
        fichier.close();
    }

    return text;
}

void Configuration::ChargerTxt()
{
    text_benedictions = ChargerFichierTxt(chemin_text_benedictions);
    text_menus        = ChargerFichierTxt(chemin_text_menus);
    text_items        = ChargerFichierTxt(chemin_text_items);
    text_entities     = ChargerFichierTxt(chemin_text_entities);
    text_dialogs      = ChargerFichierTxt(chemin_text_dialogs);
    text_maps         = ChargerFichierTxt(chemin_text_maps);
    text_miracles     = ChargerFichierTxt(chemin_text_miracles);
}

const std::string &Configuration::getText(int type, int no)
{
#define RETURN_TEXT(text) \
    if((int)text.size() > no) \
        return text[no];

    switch(type) {
        case 0:
            RETURN_TEXT(text_menus);
        break;
        case 1:
            RETURN_TEXT(text_benedictions);
        break;
        case 2:
            RETURN_TEXT(text_items);
        break;
        case 3:
            RETURN_TEXT(text_entities);
        break;
        case 4:
            RETURN_TEXT(text_dialogs);
        break;
        case 5:
            RETURN_TEXT(text_maps);
        break;
        case 6:
            RETURN_TEXT(text_miracles);
        break;
        default:
            return error;
        break;
    }
#undef RETURN_TEXT
}

void Configuration::Sauvegarder()
{
    ///Chargement de la configuration

    std::fstream fichier("configuration.conf", std::ios::in | std::ios::out | std::ios::trunc) ;

    if (fichier)
    {
        fichier<< "DISPLAY"<<std::endl;
        fichier<< "resolution: " <<Resolution.w<<" "<<Resolution.h<<std::endl;
        fichier<< "brightness: " <<luminosite<<std::endl;
        fichier<< "contrast: " <<contrastes<<std::endl;
        fichier<< "window: " <<mode_fenetre<<std::endl;
        fichier<< "vsync: " <<syncronisation_verticale<<std::endl;
        fichier<<std::endl;
        fichier<< "light_quality: " <<Lumiere<<std::endl;
        fichier<< "light_refresh_rate: " <<frequence_lumiere<<std::endl;
        fichier<< "shadows: " <<Ombre<<std::endl;
        fichier<< "reflection: " <<Reflection<<std::endl;
        fichier<< "distortion: " <<Distortion<<std::endl;
        fichier<< "videos: " <<video<<std::endl;
        fichier<< "postFX: " <<postFX<<std::endl;
        fichier<< "alpha_opacity: "<<alpha<<std::endl;
        fichier<< "grass: " <<Herbes<<std::endl;
        fichier<< "particules: " <<particules<<std::endl;
        fichier<< "smooth: " <<lissage<<std::endl;
        fichier<< "zoom: " <<zoom<<std::endl;
        fichier<<std::endl;
        fichier<<std::endl;
        fichier<<"SOUNDS"<<std::endl;
        fichier<< "volume: " <<volume<<std::endl;
        fichier<< "music_volume: " <<music_volume<<std::endl;
        fichier<<std::endl;
        fichier<<std::endl;
        fichier<<"INTERFACE"<<std::endl;
        fichier<< "minimap: " <<Minimap<<std::endl;
        fichier<< "console: " <<console<<std::endl;
        fichier<< "item_background: " <<item_background<<std::endl;
        fichier<<std::endl;
        fichier<<std::endl;
        fichier<<"OTHERS"<<std::endl;
        fichier<<"saving_frequency: " <<frequence_sauvegarde<<std::endl;
        fichier<<"debug_mod: "<<debug<<std::endl;
        fichier<<"desactivate_console: "<<desactivate_console<<std::endl;
        fichier<<"language: "<<language<<std::endl;


        fichier.close();
    }
    else
        throw "Impossible de sauvegarder la configuration";
}
