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


void Configuration::Charger()
{
    error = "Error";

    ChargerConf();
    ChargerInit();
    ChargerTxt();

    zoom=1;

    RafraichirOmbre=0;
    RafraichirLumiere=0;

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

            if (chaine== "menu_bag:")
                fichier>>nom_sac;
            if (chaine== "menu_corner:")
                fichier>>nom_corner;
            if (chaine== "menu_item_background:")
                fichier>>nom_item_background;
            if (chaine== "menu_item_unusable:")
                fichier>>nom_item_unusable;

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

void Configuration::ChargerTxt()
{
    std::ifstream fichier;
    fichier.open((chemin_localisation+language+"/"+chemin_text_benedictions).c_str(), std::ios::in);
    if (fichier)
    {
        std::string chaine;
        while(!fichier.eof())
        {
            text_benedictions.push_back("");
            getline(fichier,text_benedictions.back());
        }
        fichier.close();
    }

    std::ifstream fichier2;
    fichier2.open((chemin_localisation+language+"/"+chemin_text_menus).c_str(), std::ios::in);
    if (fichier2)
    {
        std::string chaine;
        while(!fichier2.eof())
        {
            text_menus.push_back("");
            getline(fichier2,text_menus.back());
        }
        fichier2.close();
    }


    std::ifstream fichier3;
    fichier3.open((chemin_localisation+language+"/"+chemin_text_items).c_str(), std::ios::in);
    if (fichier3)
    {
        std::string chaine;
        while(!fichier3.eof())
        {
            text_items.push_back("");
            getline(fichier3,text_items.back());
        }
        fichier3.close();
    }

    std::ifstream fichier4;
    fichier4.open((chemin_localisation+language+"/"+chemin_text_entities).c_str(), std::ios::in);
    if (fichier4)
    {
        std::string chaine;
        while(!fichier4.eof())
        {
            text_entities.push_back("");
            getline(fichier4,text_entities.back());
        }
        fichier4.close();
    }

    std::ifstream fichier5;
    fichier5.open((chemin_localisation+language+"/"+chemin_text_dialogs).c_str(), std::ios::in);
    if (fichier5)
    {
        std::string chaine;
        while(!fichier5.eof())
        {
            text_dialogs.push_back("");
            getline(fichier5,text_dialogs.back());
        }
        fichier5.close();
    }

    std::ifstream fichier6;
    fichier6.open((chemin_localisation+language+"/"+chemin_text_maps).c_str(), std::ios::in);
    if (fichier6)
    {
        std::string chaine;
        while(!fichier6.eof())
        {
            text_maps.push_back("");
            getline(fichier6,text_maps.back());
        }
        fichier6.close();
    }

    std::ifstream fichier7;
    fichier7.open((chemin_localisation+language+"/"+chemin_text_miracles).c_str(), std::ios::in);
    if (fichier7)
    {
        std::string chaine;
        while(!fichier7.eof())
        {
            text_miracles.push_back("");
            getline(fichier7,text_miracles.back());
        }
        fichier7.close();
    }

}

const std::string &Configuration::getText(int type, int no)
{
         if(type == 0)
    {
        if((int)text_menus.size() > no)
            return text_menus[no];
    }
    else if(type == 1)
    {
        if((int)text_benedictions.size() > no)
            return text_benedictions[no];
    }
    else if(type == 2)
    {
        if((int)text_items.size() > no)
            return text_items[no];
    }
    else if(type == 3)
    {
        if((int)text_entities.size() > no)
            return text_entities[no];
    }
    else if(type == 4)
    {
        if((int)text_dialogs.size() > no)
            return text_dialogs[no];
    }
    else if(type == 5)
    {
        if((int)text_maps.size() > no)
            return text_maps[no];
    }
    else if(type == 6)
    {
        if((int)text_miracles.size() > no)
            return text_miracles[no];
    }

    return error;
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
