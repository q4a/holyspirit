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


#include "../jeu.h"

#include "../globale.h"

#include <iostream>
#include <fstream>
#include <sstream>

#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>


#include <dirent.h>

using namespace sf;
using namespace std;

std::string DecouperTexte(std::string texte, int tailleCadran, int tailleTexte);

c_MainMenu::c_MainMenu()
{
    no_ecran = E_PRINCIPAL;
    m_save   = false;
    m_reset  = false;
    m_supprimer_heros = false;
    classe_choisie = 0;
    nom_hero = "";
    moteurSons->PlayNewMusic(configuration->music_menu);

    m_mainscreen.SetImage(*moteurGraphique->getImage(moteurGraphique->AjouterImage(configuration->mainscreen_menu, -1)));
    m_mainscreen.Resize(configuration->Resolution.x, configuration->Resolution.y);

    m_background_hero.SetImage(*moteurGraphique->getImage(moteurGraphique->AjouterImage(configuration->chemin_menus+configuration->menu_slot, -1)));
    m_backtext_hero.SetImage(*moteurGraphique->getImage(moteurGraphique->AjouterImage(configuration->chemin_menus+configuration->menu_slot_text, -1)));
    //m_background_hero.Resize(150, 192);

    m_delete_heros.SetImage(*moteurGraphique->getImage(moteurGraphique->AjouterImage(configuration->chemin_menus+configuration->menu_del, -1)));
    m_delete_heros.SetPosition(configuration->Resolution.x/2 - 400, configuration->Resolution.y - 160);

    m_hs_logo.SetImage(*moteurGraphique->getImage(moteurGraphique->AjouterImage(configuration->chemin_menus+configuration->menu_logo, -1)));

    m_light = moteurGraphique->LightManager->Add_Dynamic_Light(sf::Vector2f(0,0),255,256,32,sf::Color(255,255,255));
    m_light_logo = moteurGraphique->LightManager->Add_Dynamic_Light(sf::Vector2f(0,0),255,256,32,sf::Color(255,192,0));

    ifstream fichier;

    fichier.open("credits.txt");
    if(fichier)
    {
        while(!fichier.eof())
        {
            m_credits.push_back("");
            getline(fichier,m_credits.back(),'\n');
        }

        fichier.close();
    }

    m_credit_defil = 0;
    time = 0;
    m_story = DecouperTexte(configuration->getText(0,66), 640,16);
}

c_MainMenu::~c_MainMenu()
{

}

void c_MainMenu::Utiliser(Jeu *jeu)
{
    jeu->m_display=true;

    temps_ecoule=jeu->Clock.GetElapsedTime();
    jeu->Clock.Reset();

    if(m_save)
    {
        if(jeu->m_jeu->m_thread_sauvegarde)
        {
            jeu->m_jeu->m_thread_sauvegarde->Wait();
            delete jeu->m_jeu->m_thread_sauvegarde;
            jeu->m_jeu->m_thread_sauvegarde = NULL;
        }

        moteurSons->PlayNewMusic(configuration->music_menu);

        for(unsigned j = 0 ; j < jeu->hero.m_personnage.m_miracleEnCours.size() ; ++j)
            for(unsigned k = 0 ; k < jeu->hero.m_personnage.m_miracleEnCours[j].m_infos.size() ; ++k)
            {
                if(jeu->hero.m_classe.miracles[jeu->hero.m_personnage.m_miracleEnCours[j].m_modele].m_effets[jeu->hero.m_personnage.m_miracleEnCours[j].m_infos[k]->m_effetEnCours].m_type == INVOCATION)
                {
                    jeu->hero.m_personnage.m_miracleEnCours[j].m_infos[k]->m_cible->Kill();
                    jeu->hero.m_personnage.m_miracleEnCours[j].m_infos[k]->m_cible->m_inexistant = true;
                }
            }

        eventManager->StopEvenement(Mouse::Left,EventClic);
        m_save = false;
        if (m_reset)
            Reset(jeu), jeu->hero.m_quetes.clear();
        else if(jeu->map)
            jeu->map->Sauvegarder(&jeu->hero);

        jeu->hero.SauvegarderApercu();
        jeu->hero.Sauvegarder();

        Reset(jeu);

        jeu->Clock.Reset();
        moteurGraphique->ViderParticules();
        moteurSons->StopAllSounds();

        if (jeu->map!=NULL)
        {
            delete jeu->map;
            jeu->map = NULL;
        }


        moteurGraphique->LightManager->Delete_All_Light();
        moteurGraphique->LightManager->Delete_All_Wall();

        moteurGraphique->m_soleil.rouge = 255;
        moteurGraphique->m_soleil.vert = 255;
        moteurGraphique->m_soleil.bleu = 255;

        m_light = moteurGraphique->LightManager->Add_Dynamic_Light(sf::Vector2f(0,0),255,256,32,sf::Color(255,255,255));
        m_light_logo = moteurGraphique->LightManager->Add_Dynamic_Light(sf::Vector2f(0,0),255,256,32,sf::Color(255,192,0));

        jeu->hero.Reset();

        m_reset = false;
        moteurGraphique->m_blur = 0;

        configuration->zoom = 1.0;
        configuration->effetNoir = 0;
    }

    m_mainscreen.Resize(configuration->Resolution.x, configuration->Resolution.y);
    moteurGraphique->AjouterCommande(&m_mainscreen, 10, 0);

    moteurGraphique->LightManager->SetPosition(m_light,sf::Vector2f(eventManager->getPositionSouris().x,
                                                                    eventManager->getPositionSouris().y * 2));

    moteurGraphique->LightManager->SetPosition(m_light_logo,sf::Vector2f(configuration->Resolution.x/2,
                                                                    256));
    configuration->RafraichirLumiere = true;
    moteurGraphique->m_soleil.intensite = 1;

    moteurGraphique->m_camera.SetCenter(configuration->Resolution.x * 0.5,
                                        configuration->Resolution.y * 0.5);

    moteurGraphique->LightManager->Generate(m_light);
    moteurGraphique->LightManager->Generate(m_light_logo);

    texte.SetCharacterSize(48);

    if(no_ecran == E_PRINCIPAL)
        E_Principal(jeu);
    else if(no_ecran == E_CONTINUER)
        E_Continuer(jeu);
    else if(no_ecran == E_NOUVEAU)
        E_Nouveau(jeu);
    else if(no_ecran == E_CREDITS)
        E_Credits(jeu);
    else if(no_ecran == E_STORY)
        E_Story(jeu);
    else if(no_ecran == E_OPTION)
        if(configuration->Options())
            no_ecran = E_PRINCIPAL;

    eventManager->AfficherCurseur();
}

void c_MainMenu::Reset(Jeu *jeu)
{
    struct dirent *lecture;

    DIR *repertoire;
    repertoire = opendir(configuration->chemin_temps.c_str());
    while ((lecture = readdir(repertoire)))
    {
        std::string temp=configuration->chemin_temps+lecture->d_name;
        remove(temp.c_str());
    }
    closedir(repertoire);

    jeu->hero.m_contenuSave.clear();
    jeu->hero.m_contenuSave.push_back(configuration->chemin_temps+"Save.sav.hs");

    jeu->hero.m_contenuSave.push_back(configuration->chemin_temps+"Image.png");
}

void c_MainMenu::ChargerListeSaves()
{
    m_chemin_saves.clear();
    m_images_saves.clear();
    m_incompatible_saves.clear();
    m_niveau_saves.clear();
    defilement_saves = 0;
    struct dirent *lecture;

    DIR *repertoire;
    repertoire = opendir(configuration->chemin_saves.c_str());
    if(!repertoire)
        console->Ajouter("Le repertoire n'existe pas",1);



    while ((lecture = readdir(repertoire)))
    {
        std::string name = lecture->d_name;
        if(name.find("sav.hs") != string::npos && name != "." && name != "..")
        {
            m_chemin_saves.push_back(name);

            std::string chemin_image = configuration->chemin_temps;
            chemin_image += "Image.png";


            sf::Sprite sprite;
            cDAT reader;
            reader.Read(configuration->chemin_saves+m_chemin_saves.back());
            int img  = moteurGraphique->AjouterImage(reader.GetFile(chemin_image), reader.GetFileSize(chemin_image), chemin_image, 1, 1);
            sprite.SetImage(*moteurGraphique->getImage(img));
            m_images_saves.push_back(sprite);

            Hero temp;
            m_incompatible_saves.push_back(temp.ChargerPresentation(m_chemin_saves.back()));
            m_niveau_saves.push_back(temp.m_caracteristiques.niveau);
        }
    }

    closedir(repertoire);
}

void  c_MainMenu::E_Principal(Jeu *jeu)
{
    m_hs_logo.SetPosition((configuration->Resolution.x - m_hs_logo.GetSize().x)/2,-16);
    moteurGraphique->AjouterCommande(&m_hs_logo, 19, 0);

    buttons_principal[0].no_opacity = true;
    buttons_principal[0].position.w = moteurGraphique->special_typo.getSize(configuration->getText(0,53), 72);
    buttons_principal[0].position.h = 64;
    buttons_principal[0].position.y = configuration->Resolution.h/2-96;
    buttons_principal[0].position.x = configuration->Resolution.w/2-buttons_principal[0].position.w/2;
    buttons_principal[0].Afficher(0);

    if(buttons_principal[0].m_press)
        moteurGraphique->special_typo_p.Draw(configuration->getText(0,53), sf::Vector2f(configuration->Resolution.w/2,
                                                                                        buttons_principal[0].position.y), 72, 19, true);
    else if(buttons_principal[0].m_hover)
        moteurGraphique->special_typo_h.Draw(configuration->getText(0,53), sf::Vector2f(configuration->Resolution.w/2,
                                                                                        buttons_principal[0].position.y), 72, 19, true);
    else
        moteurGraphique->special_typo.Draw(configuration->getText(0,53), sf::Vector2f(configuration->Resolution.w/2,
                                                                                      buttons_principal[0].position.y), 72, 19, true);

    if(buttons_principal[0].m_action)
    {
        buttons_principal[0].m_action = false;
        no_ecran = E_CONTINUER;
        ChargerListeSaves();
    }

    buttons_principal[1].no_opacity = true;
    buttons_principal[1].position.w = moteurGraphique->special_typo.getSize(configuration->getText(0,54), 72);
    buttons_principal[1].position.h = 64;
    buttons_principal[1].position.y = configuration->Resolution.h/2-32;
    buttons_principal[1].position.x = configuration->Resolution.w/2-buttons_principal[1].position.w/2;
    buttons_principal[1].Afficher(0);

    if(buttons_principal[1].m_press)
        moteurGraphique->special_typo_p.Draw(configuration->getText(0,54), sf::Vector2f(configuration->Resolution.w/2,
                                                                                        buttons_principal[1].position.y), 72, 19, true);
    else if(buttons_principal[1].m_hover)
        moteurGraphique->special_typo_h.Draw(configuration->getText(0,54), sf::Vector2f(configuration->Resolution.w/2,
                                                                                        buttons_principal[1].position.y), 72, 19, true);
    else
        moteurGraphique->special_typo.Draw(configuration->getText(0,54), sf::Vector2f(configuration->Resolution.w/2,
                                                                                      buttons_principal[1].position.y), 72, 19, true);

    if(buttons_principal[1].m_action)
    {
        buttons_principal[1].m_action = false;
        no_ecran = E_STORY;
        m_chemin_saves.clear();
        m_nom_classes.clear();
        m_description_classes.clear();

        for(unsigned i = 0 ; i < configuration->player_class.size(); ++i)
        {
            m_nom_classes.push_back(0);
            m_description_classes.push_back("");
            m_apercu_classe.push_back(sf::Sprite ());

            ifstream fichier2;
            fichier2.open(configuration->player_class[i].c_str(),ios::in);
            if(fichier2)
            {
                char caractere2;
                do
                {
                    fichier2.get(caractere2);
                    if (caractere2=='*')
                        fichier2>>m_nom_classes.back();
                    if (fichier2.eof())
                        caractere2='$';
                }
                while (caractere2!='$');


                do
                {
                    fichier2.get(caractere2);
                    if (caractere2=='*')
                    {
                        int temp;
                        fichier2>>temp;
                        m_description_classes.back() = configuration->getText(0,temp);

                        for(int i = 0 ; i < m_description_classes.back().size() ; ++i)
                            if(m_description_classes.back()[i] == '\\')
                                m_description_classes.back()[i] = '\n';
                    }
                    if (fichier2.eof())
                        caractere2='$';
                }
                while (caractere2!='$');

                do
                {
                    fichier2.get(caractere2);
                    if (caractere2=='*')
                    {
                        std::string temp;
                        fichier2>>temp;
                        m_apercu_classe.back().SetImage(*moteurGraphique->getImage(moteurGraphique->AjouterImage(temp,-1)));
                    }
                    if (fichier2.eof())
                        caractere2='$';
                }
                while (caractere2!='$');
            }
            fichier2.close();
        }
    }

    buttons_principal[2].no_opacity = true;
    buttons_principal[2].position.w = moteurGraphique->special_typo.getSize(configuration->getText(0,55), 72);
    buttons_principal[2].position.h = 64;
    buttons_principal[2].position.y = configuration->Resolution.h/2+32;
    buttons_principal[2].position.x = configuration->Resolution.w/2-buttons_principal[2].position.w/2;
    buttons_principal[2].Afficher(0);

    if(buttons_principal[2].m_press)
        moteurGraphique->special_typo_p.Draw(configuration->getText(0,55), sf::Vector2f(configuration->Resolution.w/2,
                                                                                        buttons_principal[2].position.y), 72, 19, true);
    else if(buttons_principal[2].m_hover)
        moteurGraphique->special_typo_h.Draw(configuration->getText(0,55), sf::Vector2f(configuration->Resolution.w/2,
                                                                                        buttons_principal[2].position.y), 72, 19, true);
    else
        moteurGraphique->special_typo.Draw(configuration->getText(0,55), sf::Vector2f(configuration->Resolution.w/2,
                                                                                      buttons_principal[2].position.y), 72, 19, true);

    if(buttons_principal[2].m_action)
    {
        buttons_principal[2].m_action = false;
        no_ecran = E_OPTION;
        configuration->no_menu_option = O_PRINCIPAL;
    }

    buttons_principal[3].no_opacity = true;
    buttons_principal[3].position.w = moteurGraphique->special_typo.getSize(configuration->getText(0,65), 72);
    buttons_principal[3].position.h = 64;
    buttons_principal[3].position.y = configuration->Resolution.h/2+96;
    buttons_principal[3].position.x = configuration->Resolution.w/2-buttons_principal[3].position.w/2;
    buttons_principal[3].Afficher(0);

    if(buttons_principal[3].m_press)
        moteurGraphique->special_typo_p.Draw(configuration->getText(0,65), sf::Vector2f(configuration->Resolution.w/2,
                                                                                        buttons_principal[3].position.y), 72, 19, true);
    else if(buttons_principal[3].m_hover)
        moteurGraphique->special_typo_h.Draw(configuration->getText(0,65), sf::Vector2f(configuration->Resolution.w/2,
                                                                                        buttons_principal[3].position.y), 72, 19, true);
    else
        moteurGraphique->special_typo.Draw(configuration->getText(0,65), sf::Vector2f(configuration->Resolution.w/2,
                                                                                      buttons_principal[3].position.y), 72, 19, true);

    if(buttons_principal[3].m_action)
    {
        buttons_principal[3].m_action = false;
        no_ecran = E_CREDITS;
        m_credit_defil = 0;
    }

    buttons_principal[4].no_opacity = true;
    buttons_principal[4].position.w = moteurGraphique->special_typo.getSize(configuration->getText(0,56), 72);
    buttons_principal[4].position.h = 64;
    buttons_principal[4].position.y = configuration->Resolution.h/2+160;
    buttons_principal[4].position.x = configuration->Resolution.w/2-buttons_principal[4].position.w/2;
    buttons_principal[4].Afficher(0);

    if(buttons_principal[4].m_press)
        moteurGraphique->special_typo_p.Draw(configuration->getText(0,56), sf::Vector2f(configuration->Resolution.w/2,
                                                                                        buttons_principal[4].position.y), 72, 19, true);
    else if(buttons_principal[4].m_hover)
        moteurGraphique->special_typo_h.Draw(configuration->getText(0,56), sf::Vector2f(configuration->Resolution.w/2,
                                                                                        buttons_principal[4].position.y), 72, 19, true);
    else
        moteurGraphique->special_typo.Draw(configuration->getText(0,56), sf::Vector2f(configuration->Resolution.w/2,
                                                                                      buttons_principal[4].position.y), 72, 19, true);

    if(buttons_principal[4].m_action)
    {
        buttons_principal[4].m_action = false;
        jeu->m_run=false;
    }
}


void  c_MainMenu::E_Continuer(Jeu *jeu)
{
    jeu->m_no_printscreen = true;
    defilement_saves -= eventManager->getMolette();

    if(defilement_saves > (int)m_chemin_saves.size() - 8)
        defilement_saves = (int)m_chemin_saves.size() - 8;
    if(defilement_saves < 0)
        defilement_saves = 0;

    if(eventManager->getEvenement(sf::Key::Escape, EventKey))
        no_ecran = E_PRINCIPAL, m_supprimer_heros = false,nom_hero.clear(),jeu->m_no_printscreen = false;


    buttons_continuer[0].no_opacity = true;
    buttons_continuer[0].position.w = moteurGraphique->special_typo.getSize(configuration->getText(0,0), 72);
    buttons_continuer[0].position.h = 64;
    buttons_continuer[0].position.y = configuration->Resolution.h/2+192;
    buttons_continuer[0].position.x = configuration->Resolution.w/2-buttons_continuer[0].position.w/2;
    buttons_continuer[0].Afficher(0);

    if(buttons_continuer[0].m_press)
        moteurGraphique->special_typo_p.Draw(configuration->getText(0,0), sf::Vector2f(configuration->Resolution.w/2,
                                                                                        buttons_continuer[0].position.y), 72, 19, true);
    else if(buttons_continuer[0].m_hover)
        moteurGraphique->special_typo_h.Draw(configuration->getText(0,0), sf::Vector2f(configuration->Resolution.w/2,
                                                                                        buttons_continuer[0].position.y), 72, 19, true);
    else
        moteurGraphique->special_typo.Draw(configuration->getText(0,0), sf::Vector2f(configuration->Resolution.w/2,
                                                                                      buttons_continuer[0].position.y), 72, 19, true);

    if(m_supprimer_heros)
        buttons_continuer[0].m_hover = false;

    if(buttons_continuer[0].m_action)
    {
        buttons_continuer[0].m_action = false;
        no_ecran = E_PRINCIPAL;
        jeu->m_no_printscreen = false;
        m_supprimer_heros = false;
        eventManager->StopEvenement(Mouse::Left,EventClic);
    }

    texte.SetCharacterSize(18);


    buttons_continuer[1].no_opacity = true;
    buttons_continuer[1].position.w = moteurGraphique->special_typo.getSize(configuration->getText(0,95), 32);
    buttons_continuer[1].position.h = 32;
    buttons_continuer[1].position.y = configuration->Resolution.h/2+ 192 + 64;
    buttons_continuer[1].position.x = configuration->Resolution.w/2-buttons_continuer[1].position.w/2 + 224;
    buttons_continuer[1].Afficher(0);

    if(buttons_continuer[1].m_press)
        moteurGraphique->special_typo_p.Draw(configuration->getText(0,95), sf::Vector2f(buttons_continuer[1].position.x,
                                                                                        buttons_continuer[1].position.y), 32, 19, false);
    else if(buttons_continuer[1].m_hover)
        moteurGraphique->special_typo_h.Draw(configuration->getText(0,95), sf::Vector2f(buttons_continuer[1].position.x,
                                                                                        buttons_continuer[1].position.y), 32, 19, false);
    else
        moteurGraphique->special_typo.Draw(configuration->getText(0,95), sf::Vector2f(buttons_continuer[1].position.x,
                                                                                      buttons_continuer[1].position.y), 32, 19, false);


    if(buttons_continuer[1].m_action)
    {
        buttons_continuer[1].m_action = false;
        m_supprimer_heros = true;
    }



    unsigned i;
    for(i = defilement_saves ; i < m_chemin_saves.size()
                            && i < 8 + (unsigned)defilement_saves; ++i)
    {

        m_background_hero.SetPosition(configuration->Resolution.w/2 - 331 + 160 * ((i - defilement_saves)%4 == 1) + 320 * ((i - defilement_saves)%4 == 2)  + 480 * ((i - defilement_saves)%4 == 3),
                                      configuration->Resolution.h/2 - 256 + ((int)((i - defilement_saves)/4)) * 224);

        moteurGraphique->AjouterCommande(&m_background_hero,17,0);



        std::string str;
        str += m_chemin_saves[i].substr(0, m_chemin_saves[i].size() -7);


        texte.SetCharacterSize(14);

        texte.SetString(str.c_str());

        if(texte.GetRect().Width > 128)
            texte.SetCharacterSize(12);

        texte.SetPosition((int)(configuration->Resolution.w/2 - 384 + 160 * ((i - defilement_saves)%4 == 1) + 320 * ((i - defilement_saves)%4 == 2)  + 480 * ((i - defilement_saves)%4 == 3) + 130 - texte.GetRect().Width/2),
                          (int)(configuration->Resolution.h/2 - 256 + ((int)((i - defilement_saves)/4)) * 224 + 140));

        m_images_saves[i].SetPosition(configuration->Resolution.w/2 - 336 + 160 * ((i - defilement_saves)%4 == 1) + 320 * ((i - defilement_saves)%4 == 2)  + 480 * ((i - defilement_saves)%4 == 3),
                                      configuration->Resolution.h/2 - 256 + ((int)((i - defilement_saves)/4)) * 224);
        m_images_saves[i].Resize(160,256);
        m_images_saves[i].SetSubRect(sf::IntRect(48,0,160,256));
        moteurGraphique->AjouterCommande(&m_images_saves[i],18,0);

        m_backtext_hero.SetPosition(configuration->Resolution.w/2 - 331 + 160 * ((i - defilement_saves)%4 == 1) + 320 * ((i - defilement_saves)%4 == 2)  + 480 * ((i - defilement_saves)%4 == 3),
                                      configuration->Resolution.h/2 - 256 + 136 + ((int)((i - defilement_saves)/4)) * 224);

        moteurGraphique->AjouterCommande(&m_backtext_hero,18,0);

        sf::Text texte_niv;

        std::ostringstream buf;

        if(m_incompatible_saves[i])
            buf<<configuration->getText(0,64);
        else
            buf<<configuration->getText(0,49)<<" : "<<m_niveau_saves[i];

        texte_niv.SetString(buf.str());
        texte_niv.SetCharacterSize(12);

        texte_niv.SetPosition(configuration->Resolution.w/2 - 384 + 160 * ((i - defilement_saves)%4 == 1) + 320 * ((i - defilement_saves)%4 == 2)  + 480 * ((i - defilement_saves)%4 == 3) + 130 - texte_niv.GetRect().Width/2,
                              configuration->Resolution.h/2 - 256 + ((int)((i - defilement_saves)/4)) * 224 + 158);

        if(!m_supprimer_heros
        &&((eventManager->getPositionSouris().y > texte.GetRect().Top
          &&eventManager->getPositionSouris().y < (texte.GetRect().Top + texte.GetRect().Height)
          &&eventManager->getPositionSouris().x > texte.GetRect().Left
          &&eventManager->getPositionSouris().x < (texte.GetRect().Left + texte.GetRect().Width)) ||

           (eventManager->getPositionSouris().y > m_images_saves[i].GetPosition().y
          &&eventManager->getPositionSouris().y < m_images_saves[i].GetPosition().y + 192
          &&eventManager->getPositionSouris().x > m_images_saves[i].GetPosition().x
          &&eventManager->getPositionSouris().x < m_images_saves[i].GetPosition().x + 160)))
        {
            m_images_saves[i].SetBlendMode(sf::Blend::Add);
            moteurGraphique->AjouterCommande(&m_images_saves[i],19,0);
            m_images_saves[i].SetBlendMode(sf::Blend::Alpha);

            texte.SetColor(Color(224,224,224));
            texte_niv.SetColor(Color(224,224,224));
            if(eventManager->getEvenement(Mouse::Left,EventClic))
            //if(!m_incompatible_saves[i])
            {
                moteurSons->JouerSon(configuration->sound_menu,coordonnee (0,0),0);
                coordonnee temp(0,0,-1,-1);

                jeu->hero.m_caracteristiques.nom = m_chemin_saves[i].substr(0, m_chemin_saves[i].size() - 7);
                jeu->hero.Charger(m_chemin_saves[i]);

                {
                    cDAT reader;
                    if (reader.Read(configuration->chemin_saves+m_chemin_saves[i]))
                        for (int k=0;k<(int)reader.GetNumberFile();k++)
                            reader.ExportFile(k),jeu->hero.m_contenuSave.push_back(reader.GetFileName(k)),std::cout<<reader.GetFileName(k)<<std::endl;
                }


                if(jeu->hero.m_last_potale >= 0 && jeu->hero.m_last_potale < (int)jeu->hero.m_potales.size() )
                {
                    std::string nomMap=jeu->hero.m_potales[jeu->hero.m_last_potale].chemin;

                    coordonnee coordonneePerso;
                    coordonneePerso.x=jeu->hero.m_potales[jeu->hero.m_last_potale].position.x;
                    coordonneePerso.y=jeu->hero.m_potales[jeu->hero.m_last_potale].position.y;

                    jeu->m_chargement->setC_Chargement(nomMap,coordonneePerso);
                }
                else
                    jeu->m_chargement->setC_Chargement(configuration->map_start,configuration->map_start_pos);


                //jeu->hero.Sauvegarder();

                jeu->m_contexte = jeu->m_chargement;

                eventManager->StopEvenement(Mouse::Left,EventClic);
                no_ecran = E_PRINCIPAL;
                jeu->m_no_printscreen = false;
                jeu->m_display = false;
            }
        }
        else
            texte.SetColor(Color(192,160,128)),texte_niv.SetColor(Color(192,160,128));

        moteurGraphique->AjouterTexte(&texte,18,0);
        moteurGraphique->AjouterTexte(&texte_niv,18,0);
    }

    texte.SetCharacterSize(16);
    {
        texte.SetString("- + -");
        texte.SetY((int)configuration->Resolution.h/2 + 160);
        texte.SetX((int)configuration->Resolution.w/2-texte.GetRect().Width/2 + 32);

        if(i < m_chemin_saves.size())
        {
            if(!m_supprimer_heros
              &&eventManager->getPositionSouris().y > texte.GetRect().Top
              &&eventManager->getPositionSouris().y < texte.GetRect().Top + 32
              &&eventManager->getPositionSouris().x > texte.GetRect().Left
              &&eventManager->getPositionSouris().x < (texte.GetRect().Left + texte.GetRect().Width))
            {
                texte.SetColor(Color(100,50,0));
                if(eventManager->getEvenement(Mouse::Left,EventClic))
                    defilement_saves ++,moteurSons->JouerSon(configuration->sound_menu,coordonnee (0,0),0);
                eventManager->StopEvenement(Mouse::Left,EventClic);
            }
            else
                texte.SetColor(Color(150,100,50));
        }
        else
            texte.SetColor(Color(128,128,128));
        moteurGraphique->AjouterTexte(&texte,18,1);
    }


    {
        texte.SetString("- - -");
        texte.SetY((int)configuration->Resolution.h/2 + 160 );
        texte.SetX((int)configuration->Resolution.w/2-texte.GetRect().Width/2 - 32);

        if(defilement_saves > 0)
        {
            if (!m_supprimer_heros
              &&eventManager->getPositionSouris().y > texte.GetRect().Top
              &&eventManager->getPositionSouris().y < (texte.GetRect().Top + 32)
              && eventManager->getPositionSouris().x > texte.GetRect().Left
              &&eventManager->getPositionSouris().x < (texte.GetRect().Left + texte.GetRect().Width))
            {
                texte.SetColor(Color(100,50,0));
                if(eventManager->getEvenement(Mouse::Left,EventClic))
                {
                    defilement_saves --;
                    moteurSons->JouerSon(configuration->sound_menu,coordonnee (0,0),0);
                    eventManager->StopEvenement(Mouse::Left,EventClic);
                }
            }
            else
                texte.SetColor(Color(150,100,50));
        }
        else
            texte.SetColor(Color(128,128,128));

        moteurGraphique->AjouterTexte(&texte,18,1);
    }


    if(m_supprimer_heros)
    {
        moteurGraphique->AjouterCommande(&m_delete_heros,19);
        texte.SetCharacterSize(18);

        texte.SetString(configuration->getText(0,96));
        texte.SetY((int)configuration->Resolution.h - 128 );
        texte.SetX((int)configuration->Resolution.w/2 - texte.GetRect().Width/2);

        texte.SetColor(Color(224,224,224));
        moteurGraphique->AjouterTexte(&texte,19,0);



        if(((eventManager->getChar() >= 'a' && eventManager->getChar() <= 'z')
         || (eventManager->getChar() >= 'A' && eventManager->getChar() <= 'Z')
         || (eventManager->getChar() >= '0' && eventManager->getChar() <= '9'))
        && nom_hero.size() < 16)
            nom_hero += eventManager->getChar(), eventManager->stopChar();

        if(eventManager->getEvenement(sf::Key::Back,EventKey))
            if(!nom_hero.empty())
                nom_hero.erase(nom_hero.begin() + nom_hero.size() - 1);
        eventManager->StopEvenement(sf::Key::Back,EventKey);


        texte.SetString(nom_hero);
        texte.SetX((int)configuration->Resolution.w/2 - texte.GetRect().Width/2);

        if(time > 0.5)
            texte.SetString(nom_hero + "|");

        time += temps_ecoule;
        if(time > 1)
            time = 0;

        texte.SetY((int)configuration->Resolution.h - 96 );

        texte.SetColor(Color(224,224,224));
        moteurGraphique->AjouterTexte(&texte,19,0);

        texte.SetString(configuration->getText(0,95));
        texte.SetY((int)configuration->Resolution.h - 64 );
        texte.SetX((int)configuration->Resolution.w/2 - texte.GetRect().Width/2);

        if (eventManager->getPositionSouris().y >  texte.GetRect().Top
          &&eventManager->getPositionSouris().y < (texte.GetRect().Top + 32)
          &&eventManager->getPositionSouris().x >  texte.GetRect().Left
          &&eventManager->getPositionSouris().x < (texte.GetRect().Left + texte.GetRect().Width))
        {
            texte.SetColor(Color(160,128,128));
            if(eventManager->getEvenement(Mouse::Left,EventClic))
            {
                m_supprimer_heros = false;
                moteurSons->JouerSon(configuration->sound_menu,coordonnee (0,0),0);
                eventManager->StopEvenement(Mouse::Left,EventClic);

                std::string buf = configuration->chemin_saves+nom_hero+".sav.hs";
                remove(buf.c_str());

                ChargerListeSaves();

                nom_hero.clear();
            }
        }
        else
            texte.SetColor(Color(224,224,224));

        moteurGraphique->AjouterTexte(&texte,19,0);
    }

    if(eventManager->getEvenement(Mouse::Left,EventClic)
    && m_supprimer_heros)
    {
        m_supprimer_heros = false;
        nom_hero.clear();
        eventManager->StopEvenement(Mouse::Left,EventClic);
    }
}
void  c_MainMenu::E_Nouveau(Jeu *jeu)
{
    jeu->m_no_printscreen = true;

    if(eventManager->getEvenement(sf::Key::Escape, EventKey))
        no_ecran = E_PRINCIPAL,nom_hero.clear(),jeu->m_no_printscreen = false;

    if(((eventManager->getChar() >= 'a' && eventManager->getChar() <= 'z')
     || (eventManager->getChar() >= 'A' && eventManager->getChar() <= 'Z')
     || (eventManager->getChar() >= '0' && eventManager->getChar() <= '9'))
    && nom_hero.size() < 16)
        nom_hero += eventManager->getChar(), eventManager->stopChar();

    if(eventManager->getEvenement(sf::Key::Back,EventKey))
        if(!nom_hero.empty())
            nom_hero.erase(nom_hero.begin() + nom_hero.size() - 1);
    eventManager->StopEvenement(sf::Key::Back,EventKey);

    texte.SetCharacterSize(20);

    moteurGraphique->special_typo.Draw(configuration->getText(0,68), sf::Vector2f(configuration->Resolution.w/2,
                                                                                  configuration->Resolution.h/2 - 256 - 16), 40, 19, true);

    for(unsigned i = 0 ; i < configuration->player_class.size(); ++i)
    {
        m_background_hero.SetPosition(configuration->Resolution.w/2 - configuration->player_class.size() * (m_background_hero.GetSize().x + 4) * 0.5
                                        + i * (m_background_hero.GetSize().x + 4),
                                      configuration->Resolution.h/2 - 208 - 16);

        if((int)i != classe_choisie)
            m_background_hero.SetColor(sf::Color(96,96,96));

        moteurGraphique->AjouterCommande(&m_background_hero,17,0);

        m_background_hero.SetColor(sf::Color(255,255,255));

        m_backtext_hero.SetPosition(configuration->Resolution.w/2 - configuration->player_class.size() * (m_background_hero.GetSize().x + 4) * 0.5
                                        + i * (m_background_hero.GetSize().x + 4),
                                    configuration->Resolution.h/2 - 208 + 136 - 16);

        moteurGraphique->AjouterCommande(&m_backtext_hero,19,0);

        texte.SetString(configuration->getText(3,m_nom_classes[i]));
        texte.SetFont(moteurGraphique->m_font_titre);

        texte.SetPosition((int)(configuration->Resolution.w/2 - configuration->player_class.size() * (m_background_hero.GetSize().x + 4) * 0.5
                                        + i * (m_background_hero.GetSize().x + 4) + 75 - texte.GetRect().Width/2),
                          (int)(configuration->Resolution.h/2 - 208 + 145 - 16));

        moteurGraphique->AjouterTexte(&texte,19,1);

        m_apercu_classe[i].SetPosition(configuration->Resolution.w/2 - configuration->player_class.size() * (m_background_hero.GetSize().x + 4) * 0.5
                                        + i * (m_background_hero.GetSize().x + 4),
                                      configuration->Resolution.h/2 - 208 - 16);
        m_apercu_classe[i].Resize(160,256);
        m_apercu_classe[i].SetSubRect(sf::IntRect(48,0,160,256));
        moteurGraphique->AjouterCommande(&m_apercu_classe[i],18,0);


        if (eventManager->getPositionSouris().y > m_background_hero.GetPosition().y
          &&eventManager->getPositionSouris().y < m_background_hero.GetPosition().y + m_background_hero.GetSize().y
          &&eventManager->getPositionSouris().x > m_background_hero.GetPosition().x
          &&eventManager->getPositionSouris().x < m_background_hero.GetPosition().x + m_background_hero.GetSize().x)
        {
            if(eventManager->getEvenement(Mouse::Left,EventClic))
            {
                moteurSons->JouerSon(configuration->sound_menu,coordonnee (0,0),0);
                eventManager->StopEvenement(Mouse::Left,EventClic);
                classe_choisie = i;
            }
        }
    }



    if(classe_choisie >= 0)
    {
        texte.SetCharacterSize(12);
        texte.SetString(m_description_classes[classe_choisie]);
        texte.SetFont(moteurGraphique->m_font);
        texte.SetPosition((int)(configuration->Resolution.w/2 - texte.GetRect().Width/2),
                          (int)(configuration->Resolution.h/2 - 24));
        moteurGraphique->AjouterTexte(&texte,19);

        jeu->menu.border.Afficher(coordonnee(configuration->Resolution.w/2 - texte.GetRect().Width/2 - 4,
                                         configuration->Resolution.h/2 - 24),
                              coordonnee(texte.GetRect().Width + 8,64), 18);
    }



    moteurGraphique->special_typo.Draw(configuration->getText(0,58), sf::Vector2f(configuration->Resolution.w/2,
                                                                                  configuration->Resolution.h/2 + 48), 40, 19, true);

    moteurGraphique->special_typo.Draw(nom_hero + (time > 0.5 ? "|" : ""), sf::Vector2f(configuration->Resolution.w/2,
                                                              configuration->Resolution.h/2 + 52 + 48), 48, 19, true);

    time += temps_ecoule;
    if(time > 1)
        time = 0;


    jeu->menu.border.Afficher(coordonnee(configuration->Resolution.w/2 - 192,
                                         configuration->Resolution.h/2 + 52 + 48),
                              coordonnee(384,40), 18);

    buttons_nouveau[1].no_opacity = true;
    buttons_nouveau[1].position.w = moteurGraphique->special_typo.getSize(configuration->getText(0,59), 72);
    buttons_nouveau[1].position.h = 64;
    buttons_nouveau[1].position.y = configuration->Resolution.h/2+128 + 32;
    buttons_nouveau[1].position.x = configuration->Resolution.w/2-buttons_nouveau[1].position.w/2;
    buttons_nouveau[1].Afficher(0);

    if(buttons_nouveau[1].m_press)
        moteurGraphique->special_typo_p.Draw(configuration->getText(0,59), sf::Vector2f(configuration->Resolution.w/2,
                                                                                        buttons_nouveau[1].position.y), 72, 19, true);
    else if(buttons_nouveau[1].m_hover)
        moteurGraphique->special_typo_h.Draw(configuration->getText(0,59), sf::Vector2f(configuration->Resolution.w/2,
                                                                                        buttons_nouveau[1].position.y), 72, 19, true);
    else
        moteurGraphique->special_typo.Draw(configuration->getText(0,59), sf::Vector2f(configuration->Resolution.w/2,
                                                                                      buttons_nouveau[1].position.y), 72, 19, true);


    if(buttons_nouveau[1].m_action)
    {
        buttons_nouveau[1].m_action = false;

        bool ok = true;

        if(classe_choisie < 0)
            ok = false;

        if(nom_hero.empty())
            ok = false;

        ChargerListeSaves();

        for(unsigned i = 0 ; i < m_chemin_saves.size() ; ++i)
            if(m_chemin_saves[i] == nom_hero+".sav.hs")
                ok = false;

        if(ok)
        {
            jeu->hero.m_cheminClasse = configuration->player_class[classe_choisie];
            jeu->hero.m_chemin_save = nom_hero + ".sav.hs";

            jeu->m_chargement->setC_Chargement(configuration->map_start,configuration->map_start_pos
                                               );

            jeu->hero.Charger(nom_hero + ".sav.hs");
            jeu->hero.m_contenuSave.push_back(configuration->chemin_temps + "Save.sav.hs");
            jeu->hero.m_contenuSave.push_back(configuration->chemin_temps + "Image.png");
            jeu->hero.m_caracteristiques.nom = nom_hero;
            Caracteristique caract = jeu->hero.m_personnage.getCaracteristique();
            caract.nom = nom_hero;
            jeu->hero.m_personnage.setCaracteristique(caract);
            jeu->hero.SauvegarderApercu();
            jeu->hero.Sauvegarder();
            jeu->hero.m_caracteristiques.nom = nom_hero;

            no_ecran = E_PRINCIPAL;
            jeu->m_no_printscreen = false;

            jeu->m_contexte = jeu->m_chargement;
        }

    }

    buttons_nouveau[0].no_opacity = true;
    buttons_nouveau[0].position.w = moteurGraphique->special_typo.getSize(configuration->getText(0,0), 72);
    buttons_nouveau[0].position.h = 64;
    buttons_nouveau[0].position.y = configuration->Resolution.h/2+192 + 32;
    buttons_nouveau[0].position.x = configuration->Resolution.w/2-buttons_nouveau[0].position.w/2;
    buttons_nouveau[0].Afficher(0);

    if(buttons_nouveau[0].m_press)
        moteurGraphique->special_typo_p.Draw(configuration->getText(0,0), sf::Vector2f(configuration->Resolution.w/2,
                                                                                        buttons_nouveau[0].position.y), 72, 19, true);
    else if(buttons_nouveau[0].m_hover)
        moteurGraphique->special_typo_h.Draw(configuration->getText(0,0), sf::Vector2f(configuration->Resolution.w/2,
                                                                                        buttons_nouveau[0].position.y), 72, 19, true);
    else
        moteurGraphique->special_typo.Draw(configuration->getText(0,0), sf::Vector2f(configuration->Resolution.w/2,
                                                                                      buttons_nouveau[0].position.y), 72, 19, true);

    if(buttons_nouveau[0].m_action)
    {
        buttons_nouveau[0].m_action = false;
        no_ecran = E_PRINCIPAL;
    }
}
void  c_MainMenu::E_Credits(Jeu *jeu)
{
    m_credit_defil += temps_ecoule * 20;

    for(int i = (int)(m_credit_defil/24) ; i < (int)(m_credit_defil/24) + 17; ++i)
    {
        int no = i;
        while(no >= (int)m_credits.size())
            no -= m_credits.size();

        texte.SetStyle(0);
        texte.SetCharacterSize(16);

        texte.SetColor(Color(150,100,50));

        if(i == (int)(m_credit_defil/24))
            texte.SetColor(Color(150,100,50, (int)(255 - (m_credit_defil - ((int)(m_credit_defil/24)*24))*255/24)));
        if(i == (int)(m_credit_defil/24) + 16)
            texte.SetColor(Color(150,100,50, (int)((m_credit_defil - ((int)(m_credit_defil/24 + 16)*24))*240/24)));

        std::string txt = m_credits[no];
        if(m_credits[no].size() > 0)
        {
            if(m_credits[no][0] == '*')
                texte.SetStyle(1), txt = txt.substr(1,txt.size());
            if(m_credits[no][0] == '+')
                texte.SetStyle(5), txt = txt.substr(1,txt.size());
            if(m_credits[no][0] == '-')
                texte.SetStyle(1), texte.SetCharacterSize(24), txt = txt.substr(1,txt.size());
        }


        texte.SetString(txt);
        texte.SetY((int)configuration->Resolution.h/2 - 256 + i * 24 - m_credit_defil);
        texte.SetX((int)configuration->Resolution.w/2-texte.GetRect().Width/2);
        moteurGraphique->AjouterTexte(&texte,19,1);
    }

   /* texte.SetStyle(0);
    texte.SetCharacterSize(48);
    texte.SetString(configuration->getText(0,57));
    texte.SetY((int)configuration->Resolution.h/2 + 192 );
    texte.SetX((int)configuration->Resolution.w/2-texte.GetRect().Width/2);
    if (eventManager->getPositionSouris().y > texte.GetRect().Top
      &&eventManager->getPositionSouris().y < (texte.GetRect().Top + texte.GetRect().Height))
    {
        texte.SetColor(Color(100,50,0));
        if(eventManager->getEvenement(Mouse::Left,EventClic))
        {
            no_ecran = E_PRINCIPAL;
            moteurSons->JouerSon(configuration->sound_menu,coordonnee (0,0),0);
            eventManager->StopEvenement(Mouse::Left,EventClic);
        }
    }
    else
        texte.SetColor(Color(150,100,50));
    moteurGraphique->AjouterTexte(&texte,19,1);*/

    buttons_credits[0].no_opacity = true;
    buttons_credits[0].position.w = moteurGraphique->special_typo.getSize(configuration->getText(0,0), 72);
    buttons_credits[0].position.h = 64;
    buttons_credits[0].position.y = configuration->Resolution.h/2+192;
    buttons_credits[0].position.x = configuration->Resolution.w/2-buttons_credits[0].position.w/2;
    buttons_credits[0].Afficher(0);

    if(buttons_credits[0].m_press)
        moteurGraphique->special_typo_p.Draw(configuration->getText(0,0), sf::Vector2f(configuration->Resolution.w/2,
                                                                                        buttons_credits[0].position.y), 72, 19, true);
    else if(buttons_credits[0].m_hover)
        moteurGraphique->special_typo_h.Draw(configuration->getText(0,0), sf::Vector2f(configuration->Resolution.w/2,
                                                                                        buttons_credits[0].position.y), 72, 19, true);
    else
        moteurGraphique->special_typo.Draw(configuration->getText(0,0), sf::Vector2f(configuration->Resolution.w/2,
                                                                                      buttons_credits[0].position.y), 72, 19, true);

    if(buttons_credits[0].m_action)
    {
        buttons_credits[0].m_action = false;
        no_ecran = E_PRINCIPAL;
    }
}
void  c_MainMenu::E_Story(Jeu *jeu)
{
    texte.SetFont(moteurGraphique->m_font);
    texte.SetStyle(0);
    texte.SetCharacterSize(16);
    texte.SetColor(Color(192,160,128));
    texte.SetString(m_story);
    texte.SetY((int)(configuration->Resolution.h/2 - 256));
    texte.SetX((int)(configuration->Resolution.w/2-texte.GetRect().Width/2));
    moteurGraphique->AjouterTexte(&texte,19);

    jeu->menu.border.Afficher(coordonnee(texte.GetPosition().x - 8,texte.GetPosition().y),
                              coordonnee(texte.GetRect().Width + 16,texte.GetRect().Height + 12), 18,
                              sf::Color(224,224,224,192));


    buttons_story[0].no_opacity = true;
    buttons_story[0].position.w = moteurGraphique->special_typo.getSize(configuration->getText(0,67), 72);
    buttons_story[0].position.h = 72;
    buttons_story[0].position.y = configuration->Resolution.h/2+192;
    buttons_story[0].position.x = configuration->Resolution.w/2-buttons_story[0].position.w/2;
    buttons_story[0].Afficher(0);

    if(buttons_story[0].m_press)
        moteurGraphique->special_typo_p.Draw(configuration->getText(0,67), sf::Vector2f(configuration->Resolution.w/2,
                                                                                      configuration->Resolution.h/2 + 192), 72, 19, true);
    else if(buttons_story[0].m_hover)
        moteurGraphique->special_typo_h.Draw(configuration->getText(0,67), sf::Vector2f(configuration->Resolution.w/2,
                                                                                      configuration->Resolution.h/2 + 192), 72, 19, true);
    else
        moteurGraphique->special_typo.Draw(configuration->getText(0,67), sf::Vector2f(configuration->Resolution.w/2,
                                                                                      configuration->Resolution.h/2 + 192), 72, 19, true);

    if(buttons_story[0].m_action)
    {
        buttons_story[0].m_action = false;
        no_ecran = E_NOUVEAU;
        classe_choisie = -1;
    }
}
