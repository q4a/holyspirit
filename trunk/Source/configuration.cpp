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
#include "globale.h"
#include <stdio.h>
#include <sstream>

#include <locale>

#include <libintl.h>

#ifdef WIN32
#include <windows.h>
#endif

void Configuration::Charger()
{
    error = "Error";

    zoom_or=1;
    zoom=1;

    InitKeys();

    ChargerConf();
    ChargerKeyMapping();
    ChargerInit();
    ChargerTxt();

    RafraichirOmbre=0;
    RafraichirLumiere=0;

    jour = 0;

    entering_text = 0;

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
                fichier>>zoom_or;
            if (chaine== "volume:")
                fichier>>volume;
            if (chaine== "music_volume:")
                fichier>>music_volume;
            if (chaine== "minimap:")
                fichier>>Minimap;
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
            if (chaine== "net_rate:")
                fichier>>net_rate;
            if (chaine== "ip:")
                fichier>>last_ip;
        }

        fichier.close();
    }
    else
        throw std::string("Unable to load configuration : configuration.conf");



    std::ifstream fichier2;

    fichier2.open("liste_resolution.conf", std::ios::in);
    if (fichier2)
    {
        char caractere;

        do
        {
            fichier2.get(caractere);

            if(caractere == '*')
            {
                m_liste_resolutions.push_back(coordonnee ());
                fichier2>>m_liste_resolutions.back().x;
                fichier2>>m_liste_resolutions.back().y;
            }

        }while(caractere != '$');

        fichier2.close();
    }
    else
        throw std::string("Unable to load configuration : liste_resolution.conf");
}

void Configuration::ChargerKeyMapping()
{
    std::ifstream fichier;

    fichier.open("key_mapping.conf", std::ios::in);
    if (fichier)
    {
        std::string chaine;
        while (fichier>>chaine)
        {
            if (chaine== "K_SHORTCUT_1:") {
                std::string buf; fichier>>buf; m_key_actions[K_SHORTCUT_1] = convert_string_to_key(buf); }
            if (chaine== "K_SHORTCUT_2:") {
                std::string buf; fichier>>buf; m_key_actions[K_SHORTCUT_2] = convert_string_to_key(buf); }
            if (chaine== "K_SHORTCUT_3:") {
                std::string buf; fichier>>buf; m_key_actions[K_SHORTCUT_3] = convert_string_to_key(buf); }
            if (chaine== "K_SHORTCUT_4:") {
                std::string buf; fichier>>buf; m_key_actions[K_SHORTCUT_4] = convert_string_to_key(buf); }
            if (chaine== "K_SHORTCUT_5:") {
                std::string buf; fichier>>buf; m_key_actions[K_SHORTCUT_5] = convert_string_to_key(buf); }
            if (chaine== "K_SHORTCUT_6:") {
                std::string buf; fichier>>buf; m_key_actions[K_SHORTCUT_6] = convert_string_to_key(buf); }
            if (chaine== "K_SHORTCUT_7:") {
                std::string buf; fichier>>buf; m_key_actions[K_SHORTCUT_7] = convert_string_to_key(buf); }
            if (chaine== "K_SHORTCUT_8:") {
                std::string buf; fichier>>buf; m_key_actions[K_SHORTCUT_8] = convert_string_to_key(buf); }
            if (chaine== "K_INVENTORY:") {
                std::string buf; fichier>>buf; m_key_actions[K_INVENTORY] = convert_string_to_key(buf); }
            if (chaine== "K_MIRACLES:") {
                std::string buf; fichier>>buf; m_key_actions[K_MIRACLES] = convert_string_to_key(buf); }
            if (chaine== "K_QUESTS:") {
                std::string buf; fichier>>buf; m_key_actions[K_QUESTS] = convert_string_to_key(buf); }
            if (chaine== "K_MENU:") {
                std::string buf; fichier>>buf; m_key_actions[K_MENU] = convert_string_to_key(buf); }
            if (chaine== "K_MAP:") {
                std::string buf; fichier>>buf; m_key_actions[K_MAP] = convert_string_to_key(buf); }
            if (chaine== "K_DOCS:") {
                std::string buf; fichier>>buf; m_key_actions[K_DOCS] = convert_string_to_key(buf); }
            if (chaine== "K_STAND:") {
                std::string buf; fichier>>buf; m_key_actions[K_STAND] = convert_string_to_key(buf); }
            if (chaine== "K_PICKITEMS:") {
                std::string buf; fichier>>buf; m_key_actions[K_PICKITEMS] = convert_string_to_key(buf); }
            if (chaine== "K_CHANGE_WEAPONS:") {
                std::string buf; fichier>>buf; m_key_actions[K_CHANGE_WEAPONS] = convert_string_to_key(buf); }
        }

        fichier.close();
    }
   /* else
        throw std::string("Unable to load configuration : key_mapping.conf");*/
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
            if (chaine== "sound_menu:")
            {
                std::string temp;
                fichier>>temp;
                sound_menu = moteurSons->AjouterBuffer(temp);
            }
            if (chaine== "sound_dialog:")
            {
                std::string temp;
                fichier>>temp;
                sound_dialog = moteurSons->AjouterBuffer(temp);
            }
            if (chaine== "sound_create_item:")
            {
                std::string temp;
                fichier>>temp;
                sound_create_item = moteurSons->AjouterBuffer(temp);
            }
            if (chaine== "sound_bless_item:")
            {
                std::string temp;
                fichier>>temp;
                sound_bless_item = moteurSons->AjouterBuffer(temp);
            }
            if (chaine== "sound_select_miracle:")
            {
                std::string temp;
                fichier>>temp;
                sound_select_miracle = moteurSons->AjouterBuffer(temp);
            }
            if (chaine== "sound_book:")
            {
                std::string temp;
                fichier>>temp;
                sound_book = moteurSons->AjouterBuffer(temp);
            }
            if (chaine== "sound_trade:")
            {
                std::string temp;
                fichier>>temp;
                sound_trade = moteurSons->AjouterBuffer(temp);
            }
            if (chaine== "sound_trade_i:")
            {
                std::string temp;
                fichier>>temp;
                sound_trade_i = moteurSons->AjouterBuffer(temp);
            }
            if (chaine== "sound_quest_start:")
            {
                std::string temp;
                fichier>>temp;
                sound_quest_start = moteurSons->AjouterBuffer(temp);
            }
            if (chaine== "sound_quest_step:")
            {
                std::string temp;
                fichier>>temp;
                sound_quest_step = moteurSons->AjouterBuffer(temp);
            }
            if (chaine== "sound_quest_end:")
            {
                std::string temp;
                fichier>>temp;
                sound_quest_end = moteurSons->AjouterBuffer(temp);
            }
            if (chaine== "sound_menu_open:")
            {
                std::string temp;
                fichier>>temp;
                sound_menu_open = moteurSons->AjouterBuffer(temp);
            }
            if (chaine== "sound_menu_close:")
            {
                std::string temp;
                fichier>>temp;
                sound_menu_close = moteurSons->AjouterBuffer(temp);
            }
            if (chaine== "sound_nofaith:")
            {
                std::string temp;
                fichier>>temp;
                sound_nofaith = moteurSons->AjouterBuffer(temp);
            }


            if (chaine== "basic_cursor:")
                fichier>>nom_curseur_base;
            if (chaine== "fxDeath:")
                fichier>>nom_effetMort;
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

            if (chaine== "menu_item_background:")
                fichier>>nom_item_background;
            if (chaine== "menu_item_unusable:")
                fichier>>nom_item_unusable;
            if (chaine== "menu_logo:")
                fichier>>menu_logo;
            if (chaine== "menu_slot:")
                fichier>>menu_slot;
            if (chaine== "menu_slot_text:")
                fichier>>menu_slot_text;
            if (chaine== "menu_del:")
                fichier>>menu_del;

            if (chaine== "ambient_shadow_tileset:")
                fichier>>ambientShadow_tileset;

            if (chaine== "miracle_item_physic:")
                fichier>>miracle_item_physic;
            if (chaine== "miracle_item_fire:")
                fichier>>miracle_item_fire;
            if (chaine== "miracle_item_holy:")
                fichier>>miracle_item_holy;
            if (chaine== "miracle_item_poison:")
                fichier>>miracle_item_poison;


            if (chaine== "miracle_item_physic_t:")
                fichier>>miracle_item_physic_t;
            if (chaine== "miracle_item_fire_t:")
                fichier>>miracle_item_fire_t;
            if (chaine== "miracle_item_holy_t:")
                fichier>>miracle_item_holy_t;
            if (chaine== "miracle_item_poison_t:")
                fichier>>miracle_item_poison_t;

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
            if (chaine== "text_docs:")
                fichier>>chemin_text_docs;
            if (chaine== "text_quests:")
                fichier>>chemin_text_quests;

            if (chaine== "font_title:")
                fichier>>font_titre;
            if (chaine== "font:")
                fichier>>font;
            if (chaine== "special_font:")
                fichier>>spec_font;
            if (chaine== "special_font_h:")
                fichier>>spec_font_h;
            if (chaine== "special_font_p:")
                fichier>>spec_font_p;

            if (chaine== "death_sound:")
                fichier>>chemin_son_mort;

            if (chaine== "map_start:")
                fichier>>map_start>>map_start_pos.x>>map_start_pos.y;

            if (chaine== "player_class:")
            {
                player_class.push_back("");
                fichier>>player_class.back();
            }

        }
        fichier.close();
    }
    else
        throw std::string("Unable to load configuration : holyspirit.ini");
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

#ifdef WIN32

	bool lang;

	if (language=="en") {
		lang = SetThreadLocale(0x0409);
	}
	else if (language=="fr") {
		lang = SetThreadLocale(0x040C);
	}
	else if (language=="es") {
		lang = SetThreadLocale(0x0C0A);
	}
	else if (language=="ru") {
		lang = SetThreadLocale(0x0419);
	}

#else
	//TODO use setlocale()
#endif

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
    text_docs         = ChargerFichierTxt(chemin_text_docs);
    text_quests       = ChargerFichierTxt(chemin_text_quests);

	std::string pathToTranslationFiles = chemin_localisation;

	bindtextdomain ("benedictions",pathToTranslationFiles.c_str());
	bind_textdomain_codeset ("benedictions","UTF-8");

	bindtextdomain ("menus",pathToTranslationFiles.c_str());
	bind_textdomain_codeset ("menus","UTF-8");

	bindtextdomain ("items",pathToTranslationFiles.c_str());
	bind_textdomain_codeset ("items","UTF-8");

	bindtextdomain ("entities",pathToTranslationFiles.c_str());
	bind_textdomain_codeset ("entities","UTF-8");

	bindtextdomain ("dialogs",pathToTranslationFiles.c_str());
	bind_textdomain_codeset ("dialogs","UTF-8");

	bindtextdomain ("maps",pathToTranslationFiles.c_str());
	bind_textdomain_codeset ("maps","UTF-8");

	bindtextdomain ("miracles",pathToTranslationFiles.c_str());
	bind_textdomain_codeset ("miracles","UTF-8");

	bindtextdomain ("docs",pathToTranslationFiles.c_str());
	bind_textdomain_codeset ("docs","UTF-8");

	bindtextdomain ("quests",pathToTranslationFiles.c_str());
	bind_textdomain_codeset ("quests","UTF-8");

	textdomain ("menus");
}

const std::string &Configuration::getText(int type, int no)
{
#define RETURN_TEXT(text) \
    if((int)text.size() > no && no >= 0) \
        return text[no]; \
    else \
        return error;

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
        case 7:
            RETURN_TEXT(text_docs);
        break;
        case 8:
            RETURN_TEXT(text_quests);
        break;
        default:
            return error;
        break;
    }
#undef RETURN_TEXT
}

void Configuration::Sauvegarder()
{
    SauvegarderKeyMapping();
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
        fichier<< "shadows: " <<Ombre<<std::endl;
        fichier<< "reflection: " <<Reflection<<std::endl;
        fichier<< "distortion: " <<Distortion<<std::endl;
        fichier<< "videos: " <<video<<std::endl;
        fichier<< "postFX: " <<postFX<<std::endl;
        fichier<< "alpha_opacity: "<<alpha<<std::endl;
        fichier<< "grass: " <<Herbes<<std::endl;
        fichier<< "particules: " <<particules<<std::endl;
        fichier<< "smooth: " <<lissage<<std::endl;
        fichier<< "zoom: " <<zoom_or<<std::endl;
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
        fichier<<"net_rate: " <<net_rate<<std::endl;
        fichier<<"debug_mod: "<<debug<<std::endl;
        fichier<<"desactivate_console: "<<desactivate_console<<std::endl;
        fichier<<"language: "<<language<<std::endl;
        fichier<<"ip: "<<last_ip<<std::endl;


        fichier.close();
    }
    else
        throw "Unable to save configuration";
}

void Configuration::SauvegarderKeyMapping()
{

    std::fstream fichier("key_mapping.conf", std::ios::in | std::ios::out | std::ios::trunc) ;

    if (fichier)
    {
        fichier<<"K_SHORTCUT_1: "<<convert_key_to_string(m_key_actions[K_SHORTCUT_1])<<std::endl;
        fichier<<"K_SHORTCUT_2: "<<convert_key_to_string(m_key_actions[K_SHORTCUT_2])<<std::endl;
        fichier<<"K_SHORTCUT_3: "<<convert_key_to_string(m_key_actions[K_SHORTCUT_3])<<std::endl;
        fichier<<"K_SHORTCUT_4: "<<convert_key_to_string(m_key_actions[K_SHORTCUT_4])<<std::endl;
        fichier<<"K_SHORTCUT_5: "<<convert_key_to_string(m_key_actions[K_SHORTCUT_5])<<std::endl;
        fichier<<"K_SHORTCUT_6: "<<convert_key_to_string(m_key_actions[K_SHORTCUT_6])<<std::endl;
        fichier<<"K_SHORTCUT_7: "<<convert_key_to_string(m_key_actions[K_SHORTCUT_7])<<std::endl;
        fichier<<"K_SHORTCUT_8: "<<convert_key_to_string(m_key_actions[K_SHORTCUT_8])<<std::endl;
        fichier<<"K_INVENTORY: "<<convert_key_to_string(m_key_actions[K_INVENTORY])<<std::endl;
        fichier<<"K_MIRACLES: "<<convert_key_to_string(m_key_actions[K_MIRACLES])<<std::endl;
        fichier<<"K_QUESTS: "<<convert_key_to_string(m_key_actions[K_QUESTS])<<std::endl;
        fichier<<"K_MENU: "<<convert_key_to_string(m_key_actions[K_MENU])<<std::endl;
        fichier<<"K_MAP: "<<convert_key_to_string(m_key_actions[K_MAP])<<std::endl;
        fichier<<"K_DOCS: "<<convert_key_to_string(m_key_actions[K_DOCS])<<std::endl;
        fichier<<"K_STAND: "<<convert_key_to_string(m_key_actions[K_STAND])<<std::endl;
        fichier<<"K_PICKITEMS: "<<convert_key_to_string(m_key_actions[K_PICKITEMS])<<std::endl;
        fichier<<"K_CHANGE_WEAPONS: "<<convert_key_to_string(m_key_actions[K_CHANGE_WEAPONS])<<std::endl;

        fichier.close();
    }
    else
        throw "Unable to save configuration";
}


bool Configuration::Options()
{
    sf::Text texte;
    texte.setCharacterSize(48);

    if(no_menu_option == O_PRINCIPAL)
    {

        if (eventManager->getPositionSouris().y > Resolution.h/2 - 128
          &&eventManager->getPositionSouris().y < Resolution.h/2 - 64)
          {
            if(eventManager->getEvenement(sf::Mouse::Left,EventClic))
            {
                no_menu_option = O_GRAPHICS,moteurSons->JouerSon(configuration->sound_menu,coordonnee (0,0),0);
                eventManager->StopEvenement(sf::Mouse::Left,EventClic);
                moteurGraphique->special_typo_p.Draw(gettext("Graphics options"), sf::Vector2f(configuration->Resolution.w/2,Resolution.h/2 - 128), 72, 19, true);
            }
            else
                moteurGraphique->special_typo_h.Draw(gettext("Graphics options"), sf::Vector2f(configuration->Resolution.w/2,Resolution.h/2 - 128), 72, 19, true);
          }
          else
            moteurGraphique->special_typo.Draw(gettext("Graphics options"), sf::Vector2f(configuration->Resolution.w/2,Resolution.h/2 - 128), 72, 19, true);


        if (eventManager->getPositionSouris().y > Resolution.h/2 - 64
          &&eventManager->getPositionSouris().y < Resolution.h/2)
          {
            if(eventManager->getEvenement(sf::Mouse::Left,EventClic))
            {
                no_menu_option = O_GRAPHISC_AVANCED,moteurSons->JouerSon(configuration->sound_menu,coordonnee (0,0),0);
                eventManager->StopEvenement(sf::Mouse::Left,EventClic);
                moteurGraphique->special_typo_p.Draw(gettext("Advanced options"), sf::Vector2f(configuration->Resolution.w/2,Resolution.h/2 - 64), 72, 19, true);
            }
            else
                moteurGraphique->special_typo_h.Draw(gettext("Advanced options"), sf::Vector2f(configuration->Resolution.w/2,Resolution.h/2 - 64), 72, 19, true);
          }
          else
            moteurGraphique->special_typo.Draw(gettext("Advanced options"), sf::Vector2f(configuration->Resolution.w/2,Resolution.h/2 - 64), 72, 19, true);



        if (eventManager->getPositionSouris().y > Resolution.h/2
          &&eventManager->getPositionSouris().y < Resolution.h/2+64)
          {
            if(eventManager->getEvenement(sf::Mouse::Left,EventClic))
            {
                no_menu_option = O_SOUNDS,moteurSons->JouerSon(configuration->sound_menu,coordonnee (0,0),0);
                   eventManager->StopEvenement(sf::Mouse::Left,EventClic);
                moteurGraphique->special_typo_p.Draw(gettext("Sound options"), sf::Vector2f(configuration->Resolution.w/2,Resolution.h/2), 72, 19, true);
            }
            else
                moteurGraphique->special_typo_h.Draw(gettext("Sound options"), sf::Vector2f(configuration->Resolution.w/2,Resolution.h/2), 72, 19, true);
          }
          else
            moteurGraphique->special_typo.Draw(gettext("Sound options"), sf::Vector2f(configuration->Resolution.w/2,Resolution.h/2), 72, 19, true);



        if (eventManager->getPositionSouris().y > Resolution.h/2+160
          &&eventManager->getPositionSouris().y < Resolution.h/2+224)
          {
            if(eventManager->getEvenement(sf::Mouse::Left,EventClic))
            {
                moteurSons->JouerSon(configuration->sound_menu,coordonnee (0,0),0);
                    eventManager->StopEvenement(sf::Mouse::Left,EventClic);
                eventManager->StopEvenement(sf::Mouse::Left,EventClic);
                return 1;
            }
            else
                moteurGraphique->special_typo_h.Draw(gettext("Return"), sf::Vector2f(configuration->Resolution.w/2,Resolution.h/2+160), 72, 19, true);
          }
          else
            moteurGraphique->special_typo.Draw(gettext("Return"), sf::Vector2f(configuration->Resolution.w/2,Resolution.h/2+160), 72, 19, true);

    }

    if(no_menu_option == O_GRAPHICS)
    {
        texte.setCharacterSize(32);



        {
            std::ostringstream buf;
            buf<<gettext("Resolution: ")<<Resolution.x<<" x "<<Resolution.y;

            if (eventManager->getPositionSouris().y > Resolution.h/2 - 240
              &&eventManager->getPositionSouris().y < Resolution.h/2 - 240 + 48)
              {
                if(eventManager->getEvenement(sf::Mouse::Left,EventClic))
                {
                    moteurSons->JouerSon(configuration->sound_menu,coordonnee (0,0),0);
                    eventManager->StopEvenement(sf::Mouse::Left,EventClic);

                    unsigned no = 0;
                    while(no++ < m_liste_resolutions.size())
                        if(m_liste_resolutions[no-1].x == Resolution.x
                        && m_liste_resolutions[no-1].y == Resolution.y)
                            break;

                    if(mode_fenetre)
                    {
                        sf::VideoMode test = test.getDesktopMode();

                        while(no < m_liste_resolutions.size()
                          && (m_liste_resolutions[no].x > (int)test.width
                           || m_liste_resolutions[no].y > (int)test.height))
                            no++;
                    }
                    else
                    {
                        bool ok = false;
                        while(no < m_liste_resolutions.size() && !ok)
                        {
                            sf::VideoMode test;

                            unsigned int VideoModesCount = test.getFullscreenModes().size();
                            for (unsigned int i = 0; i < VideoModesCount; ++i)
                            {
                                test = test.getFullscreenModes()[i];

                                if((int)test.width  == m_liste_resolutions[no].x
                                && (int)test.height == m_liste_resolutions[no].y)
                                    ok = true;
                            }

                            if(!ok)
                                no++;
                        }
                    }

                    if(no < m_liste_resolutions.size())
                    {
                        Resolution.x = m_liste_resolutions[no].x;
                        Resolution.y = m_liste_resolutions[no].y;
                    }
                    else
                    {
                        Resolution.x = m_liste_resolutions[0].x;
                        Resolution.y = m_liste_resolutions[0].y;
                    }

                    Resolution.w = Resolution.x;
                    Resolution.h = Resolution.y;

                    moteurGraphique->CreateNewWindow();
                }

                moteurGraphique->special_typo_h.Draw(buf.str(), sf::Vector2f(configuration->Resolution.w/2,Resolution.h/2-240), 48, 19, true);
              }
              else
                moteurGraphique->special_typo.Draw(buf.str(), sf::Vector2f(configuration->Resolution.w/2,Resolution.h/2-240), 48, 19, true);
        }


        ///
        /*
            if (eventManager->getPositionSouris().y > Resolution.h/2+128
              &&eventManager->getPositionSouris().y < Resolution.h/2+128+48)
              {
                if(eventManager->getEvenement(sf::Mouse::Left,EventClic))
                {
                    moteurSons->JouerSon(configuration->sound_menu,coordonnee (0,0),0);
                }

                moteurGraphique->special_typo_h.Draw(buf.str(), sf::Vector2f(configuration->Resolution.w/2,Resolution.h/2+128), 48, 19, true);
              }
              else
                moteurGraphique->special_typo.Draw(buf.str(), sf::Vector2f(configuration->Resolution.w/2,Resolution.h/2+128), 48, 19, true);
        */
        ///




        {
            std::ostringstream buf;
            buf<<gettext("Window mode: ")<<mode_fenetre;
            if (eventManager->getPositionSouris().y > Resolution.h/2 - 192
              &&eventManager->getPositionSouris().y < Resolution.h/2 - 192+48)
              {
                if(eventManager->getEvenement(sf::Mouse::Left,EventClic))
                {
                    moteurSons->JouerSon(configuration->sound_menu,coordonnee (0,0),0);
                    eventManager->StopEvenement(sf::Mouse::Left,EventClic);
                    mode_fenetre = !mode_fenetre;
                    moteurGraphique->CreateNewWindow();
                }

                moteurGraphique->special_typo_h.Draw(buf.str(), sf::Vector2f(configuration->Resolution.w/2,Resolution.h/2 - 192), 48, 19, true);
              }
              else
                moteurGraphique->special_typo.Draw(buf.str(), sf::Vector2f(configuration->Resolution.w/2,Resolution.h/2 - 192), 48, 19, true);
        }

        {
            std::ostringstream buf;
            buf<<gettext("Brightness: ")<<luminosite;
            if (eventManager->getPositionSouris().y > Resolution.h/2 - 144
              &&eventManager->getPositionSouris().y < Resolution.h/2 - 144+48)
              {
                if(eventManager->getEvenement(sf::Mouse::Left,EventClic)
                || eventManager->getEvenement(sf::Keyboard::Add,EventKey))
                {
                    eventManager->StopEvenement(sf::Mouse::Left,EventClic);
                    eventManager->StopEvenement(sf::Keyboard::Add,EventKey);
                    luminosite += 1;
                    if(luminosite > 50)
                        luminosite = 50;
                }
                if(eventManager->getEvenement(sf::Mouse::Right,EventClic)
                || eventManager->getEvenement(sf::Keyboard::Subtract,EventKey))
                {
                    eventManager->StopEvenement(sf::Mouse::Right,EventClic);
                    eventManager->StopEvenement(sf::Keyboard::Subtract,EventKey);
                    luminosite -= 1;
                    if(luminosite < 0)
                        luminosite = 0;
                }
                moteurGraphique->special_typo_h.Draw(buf.str(), sf::Vector2f(configuration->Resolution.w/2,Resolution.h/2 - 144), 48, 19, true);
              }
              else
                moteurGraphique->special_typo.Draw(buf.str(), sf::Vector2f(configuration->Resolution.w/2,Resolution.h/2 - 144), 48, 19, true);
        }

        {
            std::ostringstream buf;
            buf<<gettext("Contrast: ")<<contrastes*10;
            if (eventManager->getPositionSouris().y > Resolution.h/2 - 96
              &&eventManager->getPositionSouris().y < Resolution.h/2 - 96+48)
              {

                if(eventManager->getEvenement(sf::Mouse::Left,EventClic)
                || eventManager->getEvenement(sf::Keyboard::Add,EventKey))
                {
                    eventManager->StopEvenement(sf::Mouse::Left,EventClic);
                    eventManager->StopEvenement(sf::Keyboard::Add,EventKey);
                    contrastes += 0.1;
                    if(contrastes > 5)
                        contrastes = 5;
                }
                if(eventManager->getEvenement(sf::Mouse::Right,EventClic)
                || eventManager->getEvenement(sf::Keyboard::Subtract,EventKey))
                {
                    eventManager->StopEvenement(sf::Mouse::Right,EventClic);
                    eventManager->StopEvenement(sf::Keyboard::Subtract,EventKey);
                    contrastes -= 0.1;
                    if(contrastes < 0)
                        contrastes = 0;
                }

                moteurGraphique->special_typo_h.Draw(buf.str(), sf::Vector2f(configuration->Resolution.w/2,Resolution.h/2 - 96), 48, 19, true);
              }
              else
                moteurGraphique->special_typo.Draw(buf.str(), sf::Vector2f(configuration->Resolution.w/2,Resolution.h/2 - 96), 48, 19, true);
        }

       /* {
            std::ostringstream buf;
            buf<<gettext("Display background color of objects: ")<<item_background;
            if (eventManager->getPositionSouris().y > Resolution.h/2 - 48
              &&eventManager->getPositionSouris().y < Resolution.h/2 - 48+48)
              {
                if(eventManager->getEvenement(sf::Mouse::Left,EventClic))
                {
                    moteurSons->JouerSon(configuration->sound_menu,coordonnee (0,0),0);
                    eventManager->StopEvenement(sf::Mouse::Left,EventClic);
                    item_background = !item_background;
                }

                moteurGraphique->special_typo_h.Draw(buf.str(), sf::Vector2f(configuration->Resolution.w/2,Resolution.h/2 - 48), 48, 19, true);
              }
              else
                moteurGraphique->special_typo.Draw(buf.str(), sf::Vector2f(configuration->Resolution.w/2,Resolution.h/2 -48), 48, 19, true);
        }*/




        if (eventManager->getPositionSouris().y > Resolution.h/2+160
          &&eventManager->getPositionSouris().y < Resolution.h/2+224)
          {
            if(eventManager->getEvenement(sf::Mouse::Left,EventClic))
            {
                moteurSons->JouerSon(configuration->sound_menu,coordonnee (0,0),0);
                eventManager->StopEvenement(sf::Mouse::Left,EventClic);
                no_menu_option = O_PRINCIPAL;
            }
            else
                moteurGraphique->special_typo_h.Draw(gettext("Return"), sf::Vector2f(configuration->Resolution.w/2,Resolution.h/2+160), 72, 19, true);
          }
          else
            moteurGraphique->special_typo.Draw(gettext("Return"), sf::Vector2f(configuration->Resolution.w/2,Resolution.h/2+160), 72, 19, true);

    }

    if(no_menu_option == O_GRAPHISC_AVANCED)
    {
        texte.setCharacterSize(32);

        {
            std::ostringstream buf;
            buf<<gettext("Quality of lights: ")<<Lumiere;
            if (eventManager->getPositionSouris().y > Resolution.h/2 - 240
              &&eventManager->getPositionSouris().y < Resolution.h/2 - 240+48)
              {
                if(eventManager->getEvenement(sf::Mouse::Left,EventClic))
                {
                    moteurSons->JouerSon(configuration->sound_menu,coordonnee (0,0),0);
                    eventManager->StopEvenement(sf::Mouse::Left,EventClic);

                    if(Lumiere == 0)
                        Lumiere = 1;
                    else if(Lumiere == 1)
                        Lumiere = 2;
                    else
                        Lumiere = 0;
                }

                moteurGraphique->special_typo_h.Draw(buf.str(), sf::Vector2f(configuration->Resolution.w/2,Resolution.h/2 - 240), 48, 19, true);
              }
              else
                moteurGraphique->special_typo.Draw(buf.str(), sf::Vector2f(configuration->Resolution.w/2,Resolution.h/2 -240), 48, 19, true);
        }

        {
            std::ostringstream buf;
            buf<<gettext("Render the shadows: ")<<Ombre;
            if (eventManager->getPositionSouris().y > Resolution.h/2 - 192
              &&eventManager->getPositionSouris().y < Resolution.h/2 - 192+48)
              {
                if(eventManager->getEvenement(sf::Mouse::Left,EventClic))
                {
                    moteurSons->JouerSon(configuration->sound_menu,coordonnee (0,0),0);
                    eventManager->StopEvenement(sf::Mouse::Left,EventClic);

                    Ombre = !Ombre;
                }

                moteurGraphique->special_typo_h.Draw(buf.str(), sf::Vector2f(configuration->Resolution.w/2,Resolution.h/2 - 192), 48, 19, true);
              }
              else
                moteurGraphique->special_typo.Draw(buf.str(), sf::Vector2f(configuration->Resolution.w/2,Resolution.h/2 -192), 48, 19, true);
        }

        {
            std::ostringstream buf;
            buf<<gettext("Use shader effects: ")<<postFX;
            if (eventManager->getPositionSouris().y > Resolution.h/2 - 144
              &&eventManager->getPositionSouris().y < Resolution.h/2 - 144+48)
              {
                if(eventManager->getEvenement(sf::Mouse::Left,EventClic))
                {
                    moteurSons->JouerSon(configuration->sound_menu,coordonnee (0,0),0);
                    eventManager->StopEvenement(sf::Mouse::Left,EventClic);

                    postFX = !postFX;
                }

                moteurGraphique->special_typo_h.Draw(buf.str(), sf::Vector2f(configuration->Resolution.w/2,Resolution.h/2 - 144), 48, 19, true);
              }
              else
                moteurGraphique->special_typo.Draw(buf.str(), sf::Vector2f(configuration->Resolution.w/2,Resolution.h/2 -144), 48, 19, true);
        }

        {
            std::ostringstream buf;
            buf<<gettext("Enable reflection of objects: ")<<Reflection;
            if (eventManager->getPositionSouris().y > Resolution.h/2 - 96
              &&eventManager->getPositionSouris().y < Resolution.h/2 - 96+48)
              {
                if(eventManager->getEvenement(sf::Mouse::Left,EventClic))
                {
                    moteurSons->JouerSon(configuration->sound_menu,coordonnee (0,0),0);
                    eventManager->StopEvenement(sf::Mouse::Left,EventClic);

                    Reflection = !Reflection;
                }

                moteurGraphique->special_typo_h.Draw(buf.str(), sf::Vector2f(configuration->Resolution.w/2,Resolution.h/2 - 96), 48, 19, true);
              }
              else
                moteurGraphique->special_typo.Draw(buf.str(), sf::Vector2f(configuration->Resolution.w/2,Resolution.h/2 -96), 48, 19, true);
        }

        {
            std::ostringstream buf;
            buf<<gettext("Enable effects of distortion: ")<<Distortion;
            if (eventManager->getPositionSouris().y > Resolution.h/2 - 48
              &&eventManager->getPositionSouris().y < Resolution.h/2 - 48+48)
              {
                if(eventManager->getEvenement(sf::Mouse::Left,EventClic))
                {
                    moteurSons->JouerSon(configuration->sound_menu,coordonnee (0,0),0);
                    eventManager->StopEvenement(sf::Mouse::Left,EventClic);

                    Distortion = !Distortion;
                }

                moteurGraphique->special_typo_h.Draw(buf.str(), sf::Vector2f(configuration->Resolution.w/2,Resolution.h/2 - 48), 48, 19, true);
              }
              else
                moteurGraphique->special_typo.Draw(buf.str(), sf::Vector2f(configuration->Resolution.w/2,Resolution.h/2 -48), 48, 19, true);
        }

        {
            std::ostringstream buf;
            buf<<gettext("Render the grass: ")<<Herbes;
            if (eventManager->getPositionSouris().y > Resolution.h/2
              &&eventManager->getPositionSouris().y < Resolution.h/2+48)
              {
                if(eventManager->getEvenement(sf::Mouse::Left,EventClic))
                {
                    moteurSons->JouerSon(configuration->sound_menu,coordonnee (0,0),0);
                    eventManager->StopEvenement(sf::Mouse::Left,EventClic);

                    Herbes = !Herbes;
                }

                moteurGraphique->special_typo_h.Draw(buf.str(), sf::Vector2f(configuration->Resolution.w/2,Resolution.h/2), 48, 19, true);
              }
              else
                moteurGraphique->special_typo.Draw(buf.str(), sf::Vector2f(configuration->Resolution.w/2,Resolution.h/2), 48, 19, true);
        }

        {
            std::ostringstream buf;
            buf<<gettext("Enable dynamical debris: ")<<particules;
            if (eventManager->getPositionSouris().y > Resolution.h/2+48
              &&eventManager->getPositionSouris().y < Resolution.h/2+48+48)
              {
                if(eventManager->getEvenement(sf::Mouse::Left,EventClic))
                {
                    moteurSons->JouerSon(configuration->sound_menu,coordonnee (0,0),0);
                    eventManager->StopEvenement(sf::Mouse::Left,EventClic);

                    particules = !particules;
                }

                moteurGraphique->special_typo_h.Draw(buf.str(), sf::Vector2f(configuration->Resolution.w/2,Resolution.h/2+48), 48, 19, true);
              }
              else
                moteurGraphique->special_typo.Draw(buf.str(), sf::Vector2f(configuration->Resolution.w/2,Resolution.h/2+48), 48, 19, true);
        }



        if (eventManager->getPositionSouris().y > Resolution.h/2+160
          &&eventManager->getPositionSouris().y < Resolution.h/2+224)
          {
            if(eventManager->getEvenement(sf::Mouse::Left,EventClic))
            {
                moteurSons->JouerSon(configuration->sound_menu,coordonnee (0,0),0);
                eventManager->StopEvenement(sf::Mouse::Left,EventClic);
                no_menu_option = O_PRINCIPAL;
            }
            else
                moteurGraphique->special_typo_h.Draw(gettext("Return"), sf::Vector2f(configuration->Resolution.w/2,Resolution.h/2+160), 72, 19, true);
          }
          else
            moteurGraphique->special_typo.Draw(gettext("Return"), sf::Vector2f(configuration->Resolution.w/2,Resolution.h/2+160), 72, 19, true);
    }

    if(no_menu_option == O_SOUNDS)
    {
        texte.setCharacterSize(32);

        {
            std::ostringstream buf;
            buf<<gettext("Volume: ")<<volume;
            if (eventManager->getPositionSouris().y > Resolution.h/2-240
              &&eventManager->getPositionSouris().y < Resolution.h/2-240+48)
              {
                if(eventManager->getEvenement(sf::Mouse::Left,EventClic)
                || eventManager->getEvenement(sf::Keyboard::Add,EventKey))
                {
                    //eventManager->StopEvenement(sf::Mouse::Left,EventClic);
                    volume += 1;
                    if(volume > 100)
                        volume = 100;

                    sf::sleep(sf::milliseconds(100));
                }
                if(eventManager->getEvenement(sf::Mouse::Right,EventClic)
                || eventManager->getEvenement(sf::Keyboard::Subtract,EventKey))
                {
                    //eventManager->StopEvenement(sf::Mouse::Right,EventClic);
                    volume -= 1;
                    if(volume < 0)
                        volume = 0;

                    sf::sleep(sf::milliseconds(100));
                }

                sf::Listener::setGlobalVolume(volume);

                moteurGraphique->special_typo_h.Draw(buf.str(), sf::Vector2f(configuration->Resolution.w/2,Resolution.h/2-240), 48, 19, true);
              }
              else
                moteurGraphique->special_typo.Draw(buf.str(), sf::Vector2f(configuration->Resolution.w/2,Resolution.h/2-240), 48, 19, true);

        }

        {
            std::ostringstream buf;
            buf<<gettext("Music volume: ")<<music_volume;
            if (eventManager->getPositionSouris().y > Resolution.h/2-192
              &&eventManager->getPositionSouris().y < Resolution.h/2-192+48)
              {
                if(eventManager->getEvenement(sf::Mouse::Left,EventClic)
                || eventManager->getEvenement(sf::Keyboard::Add,EventKey))
                {
                    music_volume += 1;
                    if(music_volume > 100)
                        music_volume = 100;

                    sf::sleep(sf::milliseconds(100));
                }
                if(eventManager->getEvenement(sf::Mouse::Right,EventClic)
                || eventManager->getEvenement(sf::Keyboard::Subtract,EventKey))
                {
                    music_volume -= 1;
                    if(music_volume < 0)
                        music_volume = 0;

                    sf::sleep(sf::milliseconds(100));
                }

                moteurSons->setVolumeMusique((int)music_volume);

                moteurGraphique->special_typo_h.Draw(buf.str(), sf::Vector2f(configuration->Resolution.w/2,Resolution.h/2-192), 48, 19, true);
              }
              else
                moteurGraphique->special_typo.Draw(buf.str(), sf::Vector2f(configuration->Resolution.w/2,Resolution.h/2-192), 48, 19, true);
        }



        if (eventManager->getPositionSouris().y > Resolution.h/2+160
          &&eventManager->getPositionSouris().y < Resolution.h/2+224)
          {
            if(eventManager->getEvenement(sf::Mouse::Left,EventClic))
            {
                moteurSons->JouerSon(configuration->sound_menu,coordonnee (0,0),0);
                eventManager->StopEvenement(sf::Mouse::Left,EventClic);
                no_menu_option = O_PRINCIPAL;
            }
            else
                moteurGraphique->special_typo_h.Draw(gettext("Return"), sf::Vector2f(configuration->Resolution.w/2,Resolution.h/2+160), 72, 19, true);
          }
          else
            moteurGraphique->special_typo.Draw(gettext("Return"), sf::Vector2f(configuration->Resolution.w/2,Resolution.h/2+160), 72, 19, true);
    }

    return 0;
}

void Configuration::InitKeys()
{
    key_mapping_converter[sf::Keyboard::A] = "A";
    key_mapping_converter[sf::Keyboard::B] = "B";
    key_mapping_converter[sf::Keyboard::C] = "C";
    key_mapping_converter[sf::Keyboard::D] = "D";
    key_mapping_converter[sf::Keyboard::E] = "E";
    key_mapping_converter[sf::Keyboard::F] = "F";
    key_mapping_converter[sf::Keyboard::G] = "G";
    key_mapping_converter[sf::Keyboard::H] = "H";
    key_mapping_converter[sf::Keyboard::I] = "I";
    key_mapping_converter[sf::Keyboard::J] = "J";
    key_mapping_converter[sf::Keyboard::K] = "K";
    key_mapping_converter[sf::Keyboard::L] = "L";
    key_mapping_converter[sf::Keyboard::M] = "M";
    key_mapping_converter[sf::Keyboard::N] = "N";
    key_mapping_converter[sf::Keyboard::O] = "O";
    key_mapping_converter[sf::Keyboard::P] = "P";
    key_mapping_converter[sf::Keyboard::Q] = "Q";
    key_mapping_converter[sf::Keyboard::R] = "R";
    key_mapping_converter[sf::Keyboard::S] = "S";
    key_mapping_converter[sf::Keyboard::T] = "T";
    key_mapping_converter[sf::Keyboard::U] = "U";
    key_mapping_converter[sf::Keyboard::V] = "V";
    key_mapping_converter[sf::Keyboard::W] = "W";
    key_mapping_converter[sf::Keyboard::X] = "X";
    key_mapping_converter[sf::Keyboard::Y] = "Y";
    key_mapping_converter[sf::Keyboard::Z] = "Z";
    key_mapping_converter[sf::Keyboard::Num0] = "Num0";
    key_mapping_converter[sf::Keyboard::Num1] = "Num1";
    key_mapping_converter[sf::Keyboard::Num2] = "Num2";
    key_mapping_converter[sf::Keyboard::Num3] = "Num3";
    key_mapping_converter[sf::Keyboard::Num4] = "Num4";
    key_mapping_converter[sf::Keyboard::Num5] = "Num5";
    key_mapping_converter[sf::Keyboard::Num6] = "Num6";
    key_mapping_converter[sf::Keyboard::Num7] = "Num7";
    key_mapping_converter[sf::Keyboard::Num8] = "Num8";
    key_mapping_converter[sf::Keyboard::Num9] = "Num9";
    key_mapping_converter[sf::Keyboard::Escape] = "Escape";
    key_mapping_converter[sf::Keyboard::LControl] = "LControl";
    key_mapping_converter[sf::Keyboard::LShift] = "LShift";
    key_mapping_converter[sf::Keyboard::LAlt] = "LAlt";
    key_mapping_converter[sf::Keyboard::LSystem] = "LSystem";
    key_mapping_converter[sf::Keyboard::RControl] = "RControl";
    key_mapping_converter[sf::Keyboard::RShift] = "RShift";
    key_mapping_converter[sf::Keyboard::RAlt] = "RAlt";
    key_mapping_converter[sf::Keyboard::RSystem] = "RSystem";
    key_mapping_converter[sf::Keyboard::Menu] = "Menu";
    key_mapping_converter[sf::Keyboard::LBracket] = "LBracket";
    key_mapping_converter[sf::Keyboard::RBracket] = "RBracket";
    key_mapping_converter[sf::Keyboard::SemiColon] = "SemiColon";
    key_mapping_converter[sf::Keyboard::Comma] = "Comma";
    key_mapping_converter[sf::Keyboard::Period] = "Period";
    key_mapping_converter[sf::Keyboard::Quote] = "Quote";
    key_mapping_converter[sf::Keyboard::Slash] = "Slash";
    key_mapping_converter[sf::Keyboard::BackSlash] = "BackSlash";
    key_mapping_converter[sf::Keyboard::Tilde] = "Tilde";
    key_mapping_converter[sf::Keyboard::Equal] = "Equal";
    key_mapping_converter[sf::Keyboard::Dash] = "Dash";
    key_mapping_converter[sf::Keyboard::Space] = "Space";
    key_mapping_converter[sf::Keyboard::Return] = "Return";
    key_mapping_converter[sf::Keyboard::Back] = "Back";
    key_mapping_converter[sf::Keyboard::Tab] = "Tab";
    key_mapping_converter[sf::Keyboard::PageUp] = "PageUp";
    key_mapping_converter[sf::Keyboard::PageDown] = "PageDown";
    key_mapping_converter[sf::Keyboard::End] = "End";
    key_mapping_converter[sf::Keyboard::Home] = "Home";
    key_mapping_converter[sf::Keyboard::Insert] = "Insert";
    key_mapping_converter[sf::Keyboard::Delete] = "Delete";
    key_mapping_converter[sf::Keyboard::Add] = "Add";
    key_mapping_converter[sf::Keyboard::Subtract] = "Subtract";
    key_mapping_converter[sf::Keyboard::Multiply] = "Multiply";
    key_mapping_converter[sf::Keyboard::Divide] = "Divide";
    key_mapping_converter[sf::Keyboard::Left] = "Left";
    key_mapping_converter[sf::Keyboard::Right] = "Right";
    key_mapping_converter[sf::Keyboard::Up] = "Up";
    key_mapping_converter[sf::Keyboard::Down] = "Down";
    key_mapping_converter[sf::Keyboard::Numpad0] = "Numpad0";
    key_mapping_converter[sf::Keyboard::Numpad1] = "Numpad1";
    key_mapping_converter[sf::Keyboard::Numpad2] = "Numpad2";
    key_mapping_converter[sf::Keyboard::Numpad3] = "Numpad3";
    key_mapping_converter[sf::Keyboard::Numpad4] = "Numpad4";
    key_mapping_converter[sf::Keyboard::Numpad5] = "Numpad5";
    key_mapping_converter[sf::Keyboard::Numpad6] = "Numpad6";
    key_mapping_converter[sf::Keyboard::Numpad7] = "Numpad7";
    key_mapping_converter[sf::Keyboard::Numpad8] = "Numpad8";
    key_mapping_converter[sf::Keyboard::Numpad9] = "Numpad9";
    key_mapping_converter[sf::Keyboard::F1] = "F1";
    key_mapping_converter[sf::Keyboard::F2] = "F2";
    key_mapping_converter[sf::Keyboard::F3] = "F3";
    key_mapping_converter[sf::Keyboard::F4] = "F4";
    key_mapping_converter[sf::Keyboard::F5] = "F5";
    key_mapping_converter[sf::Keyboard::F6] = "F6";
    key_mapping_converter[sf::Keyboard::F7] = "F7";
    key_mapping_converter[sf::Keyboard::F8] = "F8";
    key_mapping_converter[sf::Keyboard::F9] = "F9";
    key_mapping_converter[sf::Keyboard::F10] = "F10";
    key_mapping_converter[sf::Keyboard::F11] = "F11";
    key_mapping_converter[sf::Keyboard::F12] = "F12";
    key_mapping_converter[sf::Keyboard::F13] = "F13";
    key_mapping_converter[sf::Keyboard::F14] = "F14";
    key_mapping_converter[sf::Keyboard::F15] = "F15";
    key_mapping_converter[sf::Keyboard::Pause] = "Pause";


    m_key_actions[K_SHORTCUT_1] = '1';
    m_key_actions[K_SHORTCUT_2] = '2';
    m_key_actions[K_SHORTCUT_3] = '3';
    m_key_actions[K_SHORTCUT_4] = '4';
    m_key_actions[K_SHORTCUT_5] = '5';
    m_key_actions[K_SHORTCUT_6] = '6';
    m_key_actions[K_SHORTCUT_7] = '7';
    m_key_actions[K_SHORTCUT_8] = '8';
    m_key_actions[K_INVENTORY]  = 'i';
    m_key_actions[K_MIRACLES]   = 't';
    m_key_actions[K_QUESTS]     = 'q';
    m_key_actions[K_MENU]       = sf::Keyboard::Escape;
    m_key_actions[K_MAP]        = 'm';
    m_key_actions[K_DOCS]       = 'd';
    m_key_actions[K_STAND]      = sf::Keyboard::LShift;
    m_key_actions[K_PICKITEMS]  = sf::Keyboard::LAlt;
    m_key_actions[K_CHANGE_WEAPONS]  = 'w';
}

int Configuration::convert_string_to_key(const std::string& string)
{
    for(int i = 0 ; i < 321 ; ++i)
        if(key_mapping_converter[i] == string)
            return i;

    return 0;
}

std::string Configuration::convert_key_to_string(int no)
{
    if(no >= 0 && no < 321)
        return key_mapping_converter[no];

    return key_mapping_converter[0];
}

