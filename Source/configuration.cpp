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

    ChargerConf();
    ChargerInit();
    ChargerTxt();

    zoom=1;

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
            if (chaine== "blood:")
                fichier>>sang;
            if (chaine== "shadows:")
                fichier>>Ombre;
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
            if (chaine== "minimap:")
                fichier>>Minimap;
            if (chaine== "light_refresh_rate:")
                fichier>>frequence_lumiere;
            if (chaine== "console:")
                fichier>>console;

            if (chaine== "saving_frequency:")
                fichier>>frequence_sauvegarde;
            if (chaine== "debug_mod:")
                fichier>>debug;
            if (chaine== "desactivate_console:")
                fichier>>desactivate_console;
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

            if (chaine== "menu_minimap:")
                fichier>>nom_minimap;

            if (chaine== "menu_soul:")
                fichier>>nom_ame;
            if (chaine== "menu_soul_orb:")
                fichier>>nom_barre_ame;
            if (chaine== "menu_life_monster:")
                fichier>>nom_barre_vie;
            if (chaine== "menu_life_monster_blank:")
                fichier>>nom_barre_vie_vide;
            if (chaine== "menu_bag:")
                fichier>>nom_sac;

            if (chaine== "text_benedictions:")
                fichier>>chemin_text_benedictions;
            if (chaine== "text_menus:")
                fichier>>chemin_text_menus;

            if (chaine== "font_title:")
                fichier>>font_titre;
            if (chaine== "menu_blood:")
                fichier>>nom_sang;
            if (chaine== "cache: ")
                fichier>>cache;
            if (chaine== "death_sound:")
                fichier>>chemin_son_mort;

        }
        fichier.close();
    }
    else
        throw std::string("Impossible de charger la configuration : holyspirit.ini");
}

void Configuration::ChargerTxt()
{
    std::ifstream fichier;
    fichier.open(chemin_text_benedictions.c_str(), std::ios::in);
    if (fichier)
    {
        std::string chaine;
        for (int i=0;i<NOMBRE_BENEDICTION;i++)
            getline(fichier,text_benedictions[i]);
        fichier.close();
    }

    std::ifstream fichier2;
    fichier2.open(chemin_text_menus.c_str(), std::ios::in);
    if (fichier2)
    {
        std::string chaine;
        for (int i=0;i<NOMBRE_TXT_MENU;i++)
            getline(fichier2,text_menus[i]);
        fichier2.close();
    }
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
        fichier<< "videos: " <<video<<std::endl;
        fichier<< "postFX: " <<postFX<<std::endl;
        fichier<< "alpha_opacity: "<<alpha<<std::endl;
        fichier<< "blood: " <<sang<<std::endl;
        fichier<< "grass: " <<Herbes<<std::endl;
        fichier<< "particules: " <<particules<<std::endl;
        fichier<< "smooth: " <<lissage<<std::endl;
        fichier<<std::endl;
        fichier<<std::endl;
        fichier<<"SOUNDS"<<std::endl;
        fichier<< "volume: " <<volume<<std::endl;
        fichier<<std::endl;
        fichier<<std::endl;
        fichier<<"INTERFACE"<<std::endl;
        fichier<< "minimap: " <<Minimap<<std::endl;
        fichier<< "console: " <<console<<std::endl;
        fichier<<std::endl;
        fichier<<std::endl;
        fichier<<"OTHERS"<<std::endl;
        fichier<< "saving_frequency: " <<frequence_sauvegarde<<std::endl;
        fichier<<"debug_mod: "<<debug<<std::endl;
        fichier<<"desactivate_console: "<<desactivate_console<<std::endl;

        fichier.close();
    }
    else
        throw "Impossible de sauvegarder la configuration";
}
