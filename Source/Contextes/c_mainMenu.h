

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



#ifndef C_MAINMENU
#define C_MAINMENU

#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>

#include "contexte.h"

enum {E_PRINCIPAL, E_CONTINUER, E_NOUVEAU, E_CREDITS, E_STORY, E_OPTION, E_MULTI};

class c_MainMenu : public Contexte
{
public:
    c_MainMenu();
    virtual ~c_MainMenu();
    void Utiliser(Jeu *jeu);
    void Reset(Jeu *jeu);

    void ChargerListeSaves();

    void  E_Principal(Jeu *jeu);
    void  E_Continuer(Jeu *jeu);
    void  E_Nouveau(Jeu *jeu);
    void  E_Credits(Jeu *jeu);
    void  E_Story(Jeu *jeu);
    void  E_Multi(Jeu *jeu);

    bool m_save;
    bool m_reset;

private:
    float temps_ecoule;
    int no_ecran;
    int defilement_saves;
    int classe_choisie;

    std::vector <int> m_nom_classes;
    std::vector <std::string> m_description_classes;
    std::vector <int> m_niveau_saves;
    std::vector <bool> m_incompatible_saves;
    std::vector <std::string> m_chemin_saves;
    std::vector <sf::Sprite> m_apercu_classe;
    std::vector <sf::Sprite> m_images_saves;
    std::string nom_hero, entered_ip;

    sf::Text texte;

    sf::Sprite m_mainscreen;
    sf::Sprite m_hs_logo;
    sf::Sprite m_background_hero;
    sf::Sprite m_backtext_hero;
    sf::Sprite m_delete_heros;
    Light_Entity m_light;
    Light_Entity m_light_logo;

    float time;

    std::vector<std::string> m_credits;
    float m_credit_defil;

    std::string m_story;

    bool m_supprimer_heros;

    ///E_Principal(Jeu *jeu);
    Bouton_pressoire buttons_principal[6];
    ///E_Continuer(Jeu *jeu);
    Bouton_pressoire buttons_continuer[2];
    ///E_Nouveau(Jeu *jeu);
    Bouton_pressoire buttons_nouveau[2];
    ///E_Credits(Jeu *jeu);
    Bouton_pressoire buttons_credits[1];
    ///E_Story(Jeu *jeu);
    Bouton_pressoire buttons_story[1];
    ///E_Story(Jeu *jeu);
    Bouton_pressoire buttons_multi[3];
};

#endif

