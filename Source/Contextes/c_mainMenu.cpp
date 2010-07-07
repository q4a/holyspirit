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
    classe_choisie = 0;
    nom_hero = "";
    moteurSons->PlayNewMusic(configuration->music_menu);

    m_mainscreen.SetImage(*moteurGraphique->getImage(moteurGraphique->AjouterImage(configuration->mainscreen_menu, -1)));
    m_mainscreen.Resize(configuration->Resolution.x, configuration->Resolution.y);

    m_light = moteurGraphique->LightManager->Add_Dynamic_Light(sf::Vector2f(0,0),255,256,32,sf::Color(255,255,255));

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
    m_story = DecouperTexte(configuration->getText(0,66), 640,16);
}

void c_MainMenu::Utiliser(Jeu *jeu)
{

    jeu->m_display=true;

    temps_ecoule=0;
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

        m_light = moteurGraphique->LightManager->Add_Dynamic_Light(sf::Vector2f(0,0),255,256,32,sf::Color(255,255,255));

        jeu->hero = Hero ();

        m_reset = false;
        moteurGraphique->m_blur = 0;
    }

    if(no_ecran != E_CONTINUER)
        moteurGraphique->AjouterCommande(&m_mainscreen, 10, 0);

    moteurGraphique->LightManager->SetPosition(m_light,sf::Vector2f(eventManager->getPositionSouris().x,
                                                                    eventManager->getPositionSouris().y * 2));
    configuration->RafraichirLumiere = true;
    moteurGraphique->m_soleil.intensite = 1;

    moteurGraphique->m_camera.SetCenter(configuration->Resolution.x * 0.5,
                                        configuration->Resolution.y * 0.5);

    moteurGraphique->LightManager->Generate(m_light);

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
    jeu->hero.m_contenuSave.push_back(configuration->chemin_temps+jeu->hero.m_chemin_save);

    std::string chemin_image;
    for(int i = 0  ; i < jeu->hero.m_chemin_save.size() - 7 ; ++i)
        chemin_image.push_back(jeu->hero.m_chemin_save[i]);
    chemin_image += ".png";
    jeu->hero.m_contenuSave.push_back(configuration->chemin_temps+chemin_image);
}

void  c_MainMenu::E_Principal(Jeu *jeu)
{
    texte.SetString(configuration->getText(0,53));
    texte.SetY(configuration->Resolution.h/2-128);
    texte.SetX(configuration->Resolution.w/2-texte.GetRect().Width/2);
    if (eventManager->getPositionSouris().y > texte.GetRect().Top
      &&eventManager->getPositionSouris().y < (texte.GetRect().Top + texte.GetRect().Height))
    {

        texte.SetColor(Color(100,50,0));
        if (eventManager->getEvenement(Mouse::Left,EventClic))
        {
            no_ecran = E_CONTINUER;
            m_chemin_saves.clear();
            m_images_saves.clear();
            m_incompatible_saves.clear();
            m_niveau_saves.clear();
            defilement_saves = 0;
            struct dirent *lecture;

            DIR *repertoire;
            repertoire = opendir(configuration->chemin_saves.c_str());
            if(!repertoire)
                console->Ajouter("Le repertoire n'existe pas");



            while ((lecture = readdir(repertoire)))
            {
                bool ok = false;

                std::string name = lecture->d_name;
                if(name.find("sav.hs") != string::npos && name != "." && name != "..")
                {
                    m_chemin_saves.push_back(name);

                    std::string chemin_image = configuration->chemin_temps;
                    chemin_image += m_chemin_saves.back().substr(0, m_chemin_saves.back().size() - 7);
                    chemin_image += ".png";


                    sf::Sprite sprite;
                    cDAT reader;
                    reader.Read(configuration->chemin_saves+m_chemin_saves.back());
                    int img  = moteurGraphique->AjouterImage(reader.GetFile(chemin_image), reader.GetFileSize(chemin_image), chemin_image, 1);
                    sprite.SetImage(*moteurGraphique->getImage(img));
                    m_images_saves.push_back(sprite);

                    Hero temp;
                    m_incompatible_saves.push_back(temp.ChargerPresentation(m_chemin_saves.back()));
                    m_niveau_saves.push_back(temp.m_caracteristiques.niveau);
                }
            }

            closedir(repertoire);
        }

        eventManager->StopEvenement(Mouse::Left,EventClic);
    }
    else
        texte.SetColor(Color(150,100,50));
    moteurGraphique->AjouterTexte(&texte,19,1);

    texte.SetString(configuration->getText(0,54));
    texte.SetY(configuration->Resolution.h/2-64 );
    texte.SetX(configuration->Resolution.w/2-texte.GetRect().Width/2);
    if (eventManager->getPositionSouris().y > texte.GetRect().Top
      &&eventManager->getPositionSouris().y < (texte.GetRect().Top + texte.GetRect().Height))
    {
        texte.SetColor(Color(100,50,0));
        if (eventManager->getEvenement(Mouse::Left,EventClic))
        {
            no_ecran = E_STORY;
            m_chemin_saves.clear();
            m_nom_classes.clear();

            for(unsigned i = 0 ; i < configuration->player_class.size(); ++i)
            {
                m_nom_classes.push_back(0);

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
                }
                fichier2.close();
            }

        }
        eventManager->StopEvenement(Mouse::Left,EventClic);
    }
    else
        texte.SetColor(Color(150,100,50));
    moteurGraphique->AjouterTexte(&texte,19,1);

    texte.SetString(configuration->getText(0,55));
    texte.SetY(configuration->Resolution.h/2);
    texte.SetX(configuration->Resolution.w/2-texte.GetRect().Width/2);
    if (eventManager->getPositionSouris().y > texte.GetRect().Top
      &&eventManager->getPositionSouris().y < (texte.GetRect().Top + texte.GetRect().Height))
    {
        texte.SetColor(Color(100,50,0));
        eventManager->StopEvenement(Mouse::Left,EventClic);
    }
    else
        texte.SetColor(Color(150,100,50));
    moteurGraphique->AjouterTexte(&texte,19,1);

    texte.SetString(configuration->getText(0,65));
    texte.SetY(configuration->Resolution.h/2+64);
    texte.SetX(configuration->Resolution.w/2-texte.GetRect().Width/2);
    if (eventManager->getPositionSouris().y > texte.GetRect().Top
      &&eventManager->getPositionSouris().y < (texte.GetRect().Top + texte.GetRect().Height))
    {
        texte.SetColor(Color(100,50,0));
        if (eventManager->getEvenement(Mouse::Left,EventClic))
            no_ecran = E_CREDITS, m_credit_defil = 0;
         eventManager->StopEvenement(Mouse::Left,EventClic);
    }
    else
        texte.SetColor(Color(150,100,50));
    moteurGraphique->AjouterTexte(&texte,19,1);


    texte.SetString(configuration->getText(0,56));
    texte.SetY(configuration->Resolution.h/2+128);
    texte.SetX(configuration->Resolution.w/2-texte.GetRect().Width/2);
    if (eventManager->getPositionSouris().y > texte.GetRect().Top
      &&eventManager->getPositionSouris().y < (texte.GetRect().Top + texte.GetRect().Height))
    {
        texte.SetColor(Color(100,50,0));
        if (eventManager->getEvenement(Mouse::Left,EventClic))
            jeu->m_run=false;
         eventManager->StopEvenement(Mouse::Left,EventClic);
    }
    else
        texte.SetColor(Color(150,100,50));
    moteurGraphique->AjouterTexte(&texte,19,1);
}
void  c_MainMenu::E_Continuer(Jeu *jeu)
{
    defilement_saves -= eventManager->getMolette();

    if(defilement_saves > (int)m_chemin_saves.size() - 8)
        defilement_saves = (int)m_chemin_saves.size() - 8;
    if(defilement_saves < 0)
        defilement_saves = 0;

    if(eventManager->getEvenement(sf::Key::Escape, EventKey))
        no_ecran = E_PRINCIPAL;

    texte.SetString(configuration->getText(0,57));
    texte.SetY(configuration->Resolution.h/2 + 192 );
    texte.SetX(configuration->Resolution.w/2-texte.GetRect().Width/2);
    if (eventManager->getPositionSouris().y > texte.GetRect().Top
      &&eventManager->getPositionSouris().y < (texte.GetRect().Top + texte.GetRect().Height))
    {
        texte.SetColor(Color(100,50,0));
        if(eventManager->getEvenement(Mouse::Left,EventClic))
        {
            no_ecran = E_PRINCIPAL;
            eventManager->StopEvenement(Mouse::Left,EventClic);
        }
    }
    else
        texte.SetColor(Color(150,100,50));
    moteurGraphique->AjouterTexte(&texte,19,1);

    texte.SetCharacterSize(18);

    unsigned i;
    for(i = defilement_saves ; i < m_chemin_saves.size()
                            && i < 8 + defilement_saves; ++i)
    {
        std::string str;
        str += m_chemin_saves[i].substr(0, m_chemin_saves[i].size() -7);

        texte.SetString(str.c_str());

        texte.SetPosition(configuration->Resolution.w/2 - 384 + 160 * ((i - defilement_saves)%4 == 1) + 320 * ((i - defilement_saves)%4 == 2)  + 480 * ((i - defilement_saves)%4 == 3) + 130 - texte.GetRect().Width/2,
                          configuration->Resolution.h/2 - 256 + ((int)((i - defilement_saves)/4)) * 192 + 160);

        m_images_saves[i].SetPosition(configuration->Resolution.w/2 - 336 + 160 * ((i - defilement_saves)%4 == 1) + 320 * ((i - defilement_saves)%4 == 2)  + 480 * ((i - defilement_saves)%4 == 3),
                                      configuration->Resolution.h/2 - 256 + ((int)((i - defilement_saves)/4)) * 192);
        m_images_saves[i].Resize(160,256);
        m_images_saves[i].SetSubRect(sf::IntRect(48,0,160,256));
        moteurGraphique->AjouterCommande(&m_images_saves[i],19,0);


        sf::Text texte_niv;

        std::ostringstream buf;

        if(m_incompatible_saves[i])
            buf<<configuration->getText(0,64);
        else
            buf<<configuration->getText(0,49)<<" : "<<m_niveau_saves[i];

        texte_niv.SetPosition(texte.GetPosition().x, texte.GetPosition().y + 20);
        texte_niv.SetString(buf.str());
        texte_niv.SetCharacterSize(16);


        if ((eventManager->getPositionSouris().y > texte.GetRect().Top
          &&eventManager->getPositionSouris().y < (texte.GetRect().Top + texte.GetRect().Height)
          &&eventManager->getPositionSouris().x > texte.GetRect().Left
          &&eventManager->getPositionSouris().x < (texte.GetRect().Left + texte.GetRect().Width)) ||

           (eventManager->getPositionSouris().y > m_images_saves[i].GetPosition().y
          &&eventManager->getPositionSouris().y < m_images_saves[i].GetPosition().y + 192
          &&eventManager->getPositionSouris().x > m_images_saves[i].GetPosition().x
          &&eventManager->getPositionSouris().x < m_images_saves[i].GetPosition().x + 160))
        {
            texte.SetColor(Color(100,50,0));
            texte_niv.SetColor(Color(100,50,0));
            if(eventManager->getEvenement(Mouse::Left,EventClic))
            //if(!m_incompatible_saves[i])
            {
                coordonnee temp(0,0,-1,-1);

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
                    jeu->m_chargement->setC_Chargement("Begin.map.hs",temp,1);


                jeu->hero.Sauvegarder();

                jeu->m_contexte = jeu->m_chargement;

                eventManager->StopEvenement(Mouse::Left,EventClic);
                no_ecran = E_PRINCIPAL;
                jeu->m_display = false;
            }
        }
        else
            texte.SetColor(Color(150,100,50)),texte_niv.SetColor(Color(150,100,50));
        moteurGraphique->AjouterTexte(&texte,19,1);
        moteurGraphique->AjouterTexte(&texte_niv,19,1);
    }

    if(i < m_chemin_saves.size())
    {
        texte.SetString("- + -");
        texte.SetY(configuration->Resolution.h/2 + 160);
        texte.SetX(configuration->Resolution.w/2-texte.GetRect().Width/2 + 32);
        if (eventManager->getPositionSouris().y > texte.GetRect().Top
          &&eventManager->getPositionSouris().y < texte.GetRect().Top + 32)
        if (eventManager->getPositionSouris().x > texte.GetRect().Left
          &&eventManager->getPositionSouris().x < (texte.GetRect().Left + texte.GetRect().Width))
        {
            texte.SetColor(Color(100,50,0));
            if(eventManager->getEvenement(Mouse::Left,EventClic))
                defilement_saves ++;
            eventManager->StopEvenement(Mouse::Left,EventClic);
        }
        else
            texte.SetColor(Color(150,100,50));
        moteurGraphique->AjouterTexte(&texte,19,1);
    }

    if(defilement_saves > 0)
    {
        texte.SetString("- - -");
        texte.SetY(configuration->Resolution.h/2 + 160 );
        texte.SetX(configuration->Resolution.w/2-texte.GetRect().Width/2 - 32);
        if (eventManager->getPositionSouris().y > texte.GetRect().Top
          &&eventManager->getPositionSouris().y < (texte.GetRect().Top + 32))
        if (eventManager->getPositionSouris().x > texte.GetRect().Left
          &&eventManager->getPositionSouris().x < (texte.GetRect().Left + texte.GetRect().Width))
        {
            texte.SetColor(Color(100,50,0));
            if(eventManager->getEvenement(Mouse::Left,EventClic))
            {
                 defilement_saves --;
                eventManager->StopEvenement(Mouse::Left,EventClic);
            }
        }
        else
            texte.SetColor(Color(150,100,50));
        moteurGraphique->AjouterTexte(&texte,19,1);
    }
}
void  c_MainMenu::E_Nouveau(Jeu *jeu)
{
    if(eventManager->getEvenement(sf::Key::Escape, EventKey))
        no_ecran = E_PRINCIPAL;

    if(eventManager->getChar() >= 0)
        nom_hero += eventManager->getChar(), eventManager->stopChar();

    if(eventManager->getEvenement(sf::Key::Back,EventKey))
        if(!nom_hero.empty())
            nom_hero.erase(nom_hero.begin() + nom_hero.size() - 1);
    eventManager->StopEvenement(sf::Key::Back,EventKey);

    texte.SetCharacterSize(32);


    texte.SetString(configuration->getText(0,68));
    texte.SetY(configuration->Resolution.h/2 - 256 );
    texte.SetX(configuration->Resolution.w/2-texte.GetRect().Width/2);
    texte.SetColor(Color(150,100,50));
    moteurGraphique->AjouterTexte(&texte,19,1);

    for(unsigned i = 0 ; i < configuration->player_class.size(); ++i)
    {
        texte.SetString(configuration->getText(3,m_nom_classes[i]));
        texte.SetY(configuration->Resolution.h/2 - 128 - m_nom_classes.size() * 32 + i* 48 );
        texte.SetX(configuration->Resolution.w/2-texte.GetRect().Width/2);

        if (eventManager->getPositionSouris().y > texte.GetRect().Top
          &&eventManager->getPositionSouris().y < (texte.GetRect().Top + texte.GetRect().Height))
        {
            texte.SetColor(Color(100,50,0));
            if(eventManager->getEvenement(Mouse::Left,EventClic))
            {
                eventManager->StopEvenement(Mouse::Left,EventClic);
                classe_choisie = i;
            }
        }
        else
            texte.SetColor(Color(150,100,50));

        if(i == classe_choisie)
            texte.SetColor(sf::Color(200,150,100));

        moteurGraphique->AjouterTexte(&texte,19,1);
    }

    texte.SetCharacterSize(32);
    texte.SetString(configuration->getText(0,58));
    texte.SetY(configuration->Resolution.h/2 );
    texte.SetX(configuration->Resolution.w/2-texte.GetRect().Width/2);
    texte.SetColor(Color(150,100,50));
    moteurGraphique->AjouterTexte(&texte,19,1);

    texte.SetString(nom_hero);
    texte.SetY(configuration->Resolution.h/2 + 48 );
    texte.SetX(configuration->Resolution.w/2-texte.GetRect().Width/2);
    texte.SetColor(Color(150,100,50));

    if(time > 0.5)
        texte.SetString(nom_hero + "|");
    else
        texte.SetString(nom_hero);
    time += temps_ecoule;
    if(time > 1)
        time = 0;

    moteurGraphique->AjouterTexte(&texte,19,1);

    texte.SetCharacterSize(48);

    texte.SetString(configuration->getText(0,59));
    texte.SetY(configuration->Resolution.h/2 + 128 );
    texte.SetX(configuration->Resolution.w/2-texte.GetRect().Width/2);
    if (eventManager->getPositionSouris().y > texte.GetRect().Top
      &&eventManager->getPositionSouris().y < (texte.GetRect().Top + texte.GetRect().Height)
        || eventManager->getEvenement(sf::Key::Return, EventKey))
    {
        texte.SetColor(Color(100,50,0));
        if(eventManager->getEvenement(Mouse::Left,EventClic)
        || eventManager->getEvenement(sf::Key::Return, EventKey))
        if(!nom_hero.empty())
        {
            jeu->hero.m_cheminClasse = configuration->player_class[classe_choisie];
            jeu->hero.m_chemin_save = nom_hero + ".sav.hs";

            coordonnee temp(0,0,-1,-1);
            jeu->m_chargement->setC_Chargement("Begin.map.hs",temp,1);

            jeu->hero.Charger(nom_hero + ".sav.hs");
            jeu->hero.m_contenuSave.push_back(configuration->chemin_temps + nom_hero + ".sav.hs");
            jeu->hero.m_contenuSave.push_back(configuration->chemin_temps + nom_hero + ".png");
            jeu->hero.m_caracteristiques.nom = nom_hero;
            Caracteristique caract = jeu->hero.m_personnage.getCaracteristique();
            caract.nom = nom_hero;
            jeu->hero.m_personnage.setCaracteristique(caract);
            jeu->hero.Sauvegarder();
            jeu->hero.m_caracteristiques.nom = nom_hero;

            no_ecran = E_PRINCIPAL;

            jeu->m_contexte = jeu->m_chargement;

        }
        eventManager->StopEvenement(Mouse::Left,EventClic);
    }
    else
        texte.SetColor(Color(150,100,50));
    moteurGraphique->AjouterTexte(&texte,19,1);

    texte.SetString(configuration->getText(0,57));
    texte.SetY(configuration->Resolution.h/2 + 192 );
    texte.SetX(configuration->Resolution.w/2-texte.GetRect().Width/2);
    if (eventManager->getPositionSouris().y > texte.GetRect().Top
      &&eventManager->getPositionSouris().y < (texte.GetRect().Top + texte.GetRect().Height))
    {
        texte.SetColor(Color(100,50,0));
        if(eventManager->getEvenement(Mouse::Left,EventClic))
            no_ecran = E_PRINCIPAL,nom_hero = "";
        eventManager->StopEvenement(Mouse::Left,EventClic);
    }
    else
        texte.SetColor(Color(150,100,50));
    moteurGraphique->AjouterTexte(&texte,19,1);
}
void  c_MainMenu::E_Credits(Jeu *jeu)
{
    m_credit_defil += temps_ecoule * 20;



    for(int i = (int)(m_credit_defil/24) ; i < (int)(m_credit_defil/24) + 17; ++i)
    {
        int no = i;
        while(no >= m_credits.size())
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
        texte.SetY(configuration->Resolution.h/2 - 256 + i * 24 - m_credit_defil);
        texte.SetX(configuration->Resolution.w/2-texte.GetRect().Width/2);
        moteurGraphique->AjouterTexte(&texte,19,1);
    }

    texte.SetStyle(0);
    texte.SetCharacterSize(48);
    texte.SetString(configuration->getText(0,57));
    texte.SetY(configuration->Resolution.h/2 + 192 );
    texte.SetX(configuration->Resolution.w/2-texte.GetRect().Width/2);
    if (eventManager->getPositionSouris().y > texte.GetRect().Top
      &&eventManager->getPositionSouris().y < (texte.GetRect().Top + texte.GetRect().Height))
    {
        texte.SetColor(Color(100,50,0));
        if(eventManager->getEvenement(Mouse::Left,EventClic))
        {
            no_ecran = E_PRINCIPAL;
            eventManager->StopEvenement(Mouse::Left,EventClic);
        }
    }
    else
        texte.SetColor(Color(150,100,50));
    moteurGraphique->AjouterTexte(&texte,19,1);
}
void  c_MainMenu::E_Story(Jeu *jeu)
{
    texte.SetStyle(0);
    texte.SetCharacterSize(16);
    texte.SetColor(Color(150,100,50));
    texte.SetString(m_story);
    texte.SetY(configuration->Resolution.h/2 - 256 );
    texte.SetX(configuration->Resolution.w/2-texte.GetRect().Width/2);
    moteurGraphique->AjouterTexte(&texte,19,1);

    texte.SetStyle(0);    texte.SetCharacterSize(48);
    texte.SetString(configuration->getText(0,67));
    texte.SetY(configuration->Resolution.h/2 + 192 );
    texte.SetX(configuration->Resolution.w/2-texte.GetRect().Width/2);
    if (eventManager->getPositionSouris().y > texte.GetRect().Top
      &&eventManager->getPositionSouris().y < (texte.GetRect().Top + texte.GetRect().Height))
    {
        texte.SetColor(Color(100,50,0));
        if(eventManager->getEvenement(Mouse::Left,EventClic))
        {
            no_ecran = E_NOUVEAU;
            eventManager->StopEvenement(Mouse::Left,EventClic);
        }
    }
    else
        texte.SetColor(Color(150,100,50));
    moteurGraphique->AjouterTexte(&texte,19,1);
}
