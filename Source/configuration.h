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


#ifndef CONFIGH
#define CONFIGH

#include "singleton.h"
#include "constantes.h"


class Configuration : public CSingleton<Configuration>
{
    protected :

    Configuration()
    {
    }
    ~Configuration()
    {
    }

    public :


    friend Configuration* CSingleton<Configuration>::GetInstance();
    friend void CSingleton<Configuration>::Kill();

    void Charger();
    void ChargerConf();
    void ChargerInit();
    void ChargerTxt();

    void Sauvegarder();

    const std::string &getText(int, int);

    coordonnee Resolution;
    coordonneeDecimal Redimensionnement;
    bool Ombre,Minimap,amelioration_lampes,Herbes,syncronisation_verticale,mode_fenetre,postFX,sang,video,particules,debug,desactivate_console,lissage,Reflection;
    int Lumiere,console;
    float effetMort,effetNoir,volume,minute,zoom,frequence_sauvegarde,frequence_lumiere,luminosite,contrastes;
    int heure,cache,alpha;
    std::string version,chemin_maps,chemin_items,chemin_temps,chemin_saves,chemin_evenements,chemin_curseurs,chemin_menus,chemin_fonts,chemin_fx,nom_curseur_base,nom_ame,nom_barre_ame,nom_barre_vie,nom_barre_vie_vide,nom_sang,chemin_son_mort;
    std::string nom_effetNoir,nom_effetMort,nom_effetContrastes,nom_effetBlur,nom_effetFiltre, nom_effetShadow, nom_effetWater;
    std::string water_map;

    std::string font_titre,font,chemin_video_developpeur,nom_video_developpeur,nom_sac,nom_corner,chemin_aa;

    std::string chemin_localisation,chemin_text_benedictions,chemin_text_menus,chemin_text_items,chemin_text_entities,chemin_text_dialogs,chemin_text_maps,chemin_text_miracles;
    std::string player_class;

    std::string error;
    std::vector<std::string> text_menus;
    std::vector<std::string> text_benedictions;
    std::vector<std::string> text_items;
    std::vector<std::string> text_entities;
    std::vector<std::string> text_dialogs;
    std::vector<std::string> text_maps;
    std::vector<std::string> text_miracles;

    bool RafraichirLumiere;
    int numero_screen,RafraichirOmbre;
};
#endif
