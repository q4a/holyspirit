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

#define NBR_KEYS_ACTIONS 17

enum {O_PRINCIPAL, O_GRAPHICS, O_GRAPHISC_AVANCED, O_SOUNDS};

enum {
        K_SHORTCUT_1,
        K_SHORTCUT_2,
        K_SHORTCUT_3,
        K_SHORTCUT_4,
        K_SHORTCUT_5,
        K_SHORTCUT_6,
        K_SHORTCUT_7,
        K_SHORTCUT_8,
        K_INVENTORY,
        K_MIRACLES,
        K_QUESTS,
        K_MENU,
        K_MAP,
        K_DOCS,
        K_STAND,
        K_PICKITEMS,
        K_CHANGE_WEAPONS
        };

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
    void SetLanguage();

    void ChargerKeyMapping();

    std::vector<std::string> ChargerFichierTxt(std::string chemin);

    void Sauvegarder();
    void SauvegarderKeyMapping();

    const std::string &getText(int, int);

    bool Options();


    void InitKeys();
    int         convert_string_to_key(const std::string&);
    std::string convert_key_to_string(int);


    coordonnee Resolution;
    coordonneeDecimal Redimensionnement;
    bool Ombre,Minimap,amelioration_lampes,Herbes,syncronisation_verticale,mode_fenetre,postFX,video,particules,debug,desactivate_console,lissage,Reflection,Distortion;
    int Lumiere,console;
    float effetMort,effetNoir,volume,music_volume,minute,zoom,zoom_or,frequence_sauvegarde,luminosite,contrastes,elapsed_time;
    int heure,jour,alpha;
    bool item_background;
    std::string version,chemin_maps,chemin_items,chemin_temps,chemin_saves,chemin_curseurs,chemin_menus,chemin_fonts,chemin_fx,nom_curseur_base,chemin_son_mort;
    std::string nom_item_background, nom_item_unusable, menu_logo, menu_slot, menu_slot_text, menu_del;
    std::string nom_effetMort,nom_effetBlur,nom_effetFiltre, nom_effetWater, nom_effetDistortion;
    std::string water_map;
    std::string music_menu;
    std::string mainscreen_menu;
    int sound_menu,sound_dialog,sound_create_item,sound_bless_item, sound_select_miracle,sound_book,
        sound_quest_start, sound_quest_step, sound_quest_end, sound_trade, sound_trade_i,
        sound_menu_open, sound_menu_close, sound_nofaith;
    std::string ambientShadow_tileset;

    std::string miracle_item_physic,miracle_item_fire, miracle_item_holy, miracle_item_poison,
                miracle_item_physic_t,miracle_item_fire_t, miracle_item_holy_t, miracle_item_poison_t;

    std::string font_titre,font,spec_font,spec_font_h,spec_font_p,chemin_video_developpeur,nom_video_developpeur,chemin_aa;

    std::string chemin_localisation,chemin_text_benedictions,chemin_text_menus,chemin_text_items,chemin_text_entities,chemin_text_dialogs,chemin_text_maps,chemin_text_miracles
    ,chemin_text_docs,chemin_text_quests;
    std::vector<std::string> player_class;

    std::string map_start;
    coordonnee  map_start_pos;

    std::string language;

	const char* mylocale;

    std::string last_ip;

    std::string error;
    std::vector<std::string> text_menus;
    std::vector<std::string> text_benedictions;
    std::vector<std::string> text_items;
    std::vector<std::string> text_entities;
    std::vector<std::string> text_dialogs;
    std::vector<std::string> text_maps;
    std::vector<std::string> text_miracles;
    std::vector<std::string> text_docs;
    std::vector<std::string> text_quests;

    std::vector<coordonnee> m_liste_resolutions;

    bool RafraichirLumiere;
    int numero_screen,RafraichirOmbre;

    int no_menu_option;

    std::string key_mapping_converter[321];
    int m_key_actions[NBR_KEYS_ACTIONS];

    bool hote;
    bool multi;

    int net_rate;

    bool entering_text;
};
#endif
