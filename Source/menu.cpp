

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


#include "menu.h"
#include "globale.h"

#include <iostream>
#include <fstream>
#include <sstream>

#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>


using namespace std;
using namespace sf;

void ChargerBorder(ifstream &fichier, Border &border);
void ChargerBouton(ifstream &fichier, Bouton_pressoire &bouton);
void ChargerImageInterface(ifstream &fichier, Image_interface &image_interface);

inline sf::Vector2f AutoScreenAdjust(float x, float y, float decalage = 0)
{
    sf::Vector2f temp;
    temp.x = x + (configuration->Resolution.x - 800) * 0.5;
    temp.y = y + (configuration->Resolution.y - 600) - decalage * configuration->Resolution.h/600;
    return temp;
}



sf::Color GetItemColor(int rarete);

Menu::~Menu()
{
    if (configuration->debug)
        console->Ajouter("Destruction du menu ...");
}

Menu::Menu()
{
    console->Ajouter("",0);
    console->Ajouter("Chargement des menus :",0);

    //m_dialogue.clear();
    m_speak_choice = -1;
    clear_diag = true;

    m_cur_talk_hauteur = 1000;

    std::string chemin = configuration->chemin_menus+"menus.hs";

    ifstream fichier;
    fichier.open(chemin.c_str(), ios::in);
    if (fichier)
    {
        ChargerBorder(fichier, border);
        ChargerImageInterface(fichier, mainscreen);
        ChargerImageInterface(fichier, logo);

        for(unsigned i = 0 ; i < 8 ; ++i)
            ChargerBouton(fichier, buttons[i]), buttons[i].no_opacity = true;
    }

    m_forced_dialogue = false;
}

void Menu::AfficherHUD(Classe *classe)
{
    Sprite sprite2;

    //On affiche l'HUD
    sprite2.SetImage(*moteurGraphique->getImage(classe->hud.image));
    sprite2.SetX(classe->hud.position.x + (configuration->Resolution.x - 800) * 0.5);
    sprite2.SetY(classe->hud.position.y + (configuration->Resolution.y - 600));
    sprite2.Resize(classe->hud.position.w, classe->hud.position.h);
    moteurGraphique->AjouterCommande(&sprite2,17,0);
}

void Menu::AfficherDialogue(float time,Classe *classe)
{
    Sprite sprite2;

    sprite2.SetImage(*moteurGraphique->getImage(classe->talk.image));
    sprite2.SetX(classe->talk.position.x + (configuration->Resolution.x - 800) * 0.5);
    sprite2.SetY(AutoScreenAdjust(0,classe->talk.position.y).y + m_cur_talk_hauteur);
    sprite2.Resize(classe->talk.position.w, classe->talk.position.h);
    moteurGraphique->AjouterCommande(&sprite2,16,0);

    sf::Text texte2;
    texte2.SetCharacterSize(14);
    texte2.SetFont(moteurGraphique->m_font);

   // if(m_dialogue.empty())
     //   clear_diag = false;

    if(!m_old_dialogue.empty() && !clear_diag)
    {
        if(m_cur_talk_hauteur < classe->talk.position.h)
            m_cur_talk_hauteur += time * 1024 * 1.5;

        if(m_cur_talk_hauteur >= classe->talk.position.h)
        {
            m_cur_talk_hauteur = classe->talk.position.h;
            m_old_dialogue.clear();
            m_old_choices.clear();

            clear_diag = true;
        }


        texte2.SetString(m_old_dialogue);

        texte2.SetPosition(AutoScreenAdjust(classe->position_contenu_dialogue.x,0).x + classe->position_contenu_dialogue.w * 0.5 - (texte2.GetRect().Width) * 0.5,
                          AutoScreenAdjust(0,classe->position_contenu_dialogue.y).y + m_cur_talk_hauteur);

        moteurGraphique->AjouterTexte(&texte2,16,0);

        float pos = texte2.GetRect().Top + texte2.GetRect().Height + 8;

        float minX = -1;
        float maxX = -1;
        for(unsigned i = 0 ; i < m_old_choices.size() ; ++i)
        {
            texte.SetString(m_old_choices[i].text);
            texte.SetPosition(AutoScreenAdjust(classe->position_contenu_dialogue.x,0).x + classe->position_contenu_dialogue.w * 0.5 - (texte.GetRect().Width) * 0.5,
                                pos);
            if(texte.GetPosition().x < minX || minX == -1)
                minX = texte.GetPosition().x;
            if(texte.GetPosition().x + texte.GetRect().Width > maxX || maxX == -1)
                maxX = texte.GetPosition().x + texte.GetRect().Width;
        }

        minX -= 48;
        maxX += 16;


        sf::Sprite bullet1, bullet2;

        bullet2.FlipX(true);

        for(unsigned i = 0 ; i < m_old_choices.size() ; ++i)
        {
            texte.SetString(m_old_choices[i].text);
            texte.SetPosition(minX + 48,
                                pos);

            bullet1.SetY(texte.GetPosition().y - 4);

            pos = texte.GetRect().Top + texte.GetRect().Height + 4;


            bullet1.SetImage(*moteurGraphique->getImage(classe->bullet_off.image));
            bullet1.SetX(minX);
            texte.SetColor(sf::Color(224,224,224));

             moteurGraphique->AjouterTexte(&texte,16,0);
             moteurGraphique->AjouterCommande(&bullet1,16,0);
        }
    }
    else if(!m_dialogue.empty())
    {
        m_old_dialogue = m_dialogue;

        texte.SetString(m_dialogue);

        m_hauteur = 140;
        m_hauteur += texte.GetRect().Height + 8;

        for(unsigned i = 0 ; i < m_choices.size() ; ++i)
        {
            texte.SetString(m_choices[i].text);
            m_hauteur += texte.GetRect().Height + 4;
        }

        if(m_cur_talk_hauteur < classe->talk.position.h - m_hauteur)
        {
            m_cur_talk_hauteur += time * 1024 * 1.25;
            if(m_cur_talk_hauteur > classe->talk.position.h - m_hauteur)
                m_cur_talk_hauteur = classe->talk.position.h - m_hauteur;
        }
        else if(m_cur_talk_hauteur > classe->talk.position.h - m_hauteur)
        {
            m_cur_talk_hauteur -= time * 1024 * 1.25;
            if(m_cur_talk_hauteur < classe->talk.position.h - m_hauteur)
                m_cur_talk_hauteur = classe->talk.position.h - m_hauteur;
        }

        texte.SetColor(sf::Color(224,224,224));

        texte.SetCharacterSize(14);
        texte.SetStyle(0);
        texte.SetFont(moteurGraphique->m_font);
        texte.SetString(m_dialogue);

        texte.SetPosition(AutoScreenAdjust(classe->position_contenu_dialogue.x,0).x + classe->position_contenu_dialogue.w * 0.5 - (texte.GetRect().Width) * 0.5,
                          AutoScreenAdjust(0,classe->position_contenu_dialogue.y).y + m_cur_talk_hauteur);//(classe->talk.position.h - m_hauteur) +(classe->talk.position.h) - (classe->talk.position.h) * alpha/255);

        moteurGraphique->AjouterTexte(&texte,16,0);

        float pos = texte.GetRect().Top + texte.GetRect().Height + 8;
        texte.SetStyle(0);

        sf::Sprite bullet1, bullet2;

        /*float minX = -1;
        float maxX = -1;
        for(unsigned i = 0 ; i < m_choices.size() ; ++i)
        {
            texte.SetString(m_choices[i].text);
            texte.SetPosition(AutoScreenAdjust(classe->position_contenu_dialogue.x,0).x + classe->position_contenu_dialogue.w * 0.5 - (texte.GetRect().Width) * 0.5,
                                pos);
            if(texte.GetPosition().x < minX || minX == -1)
                minX = texte.GetPosition().x;
            if(texte.GetPosition().x + texte.GetRect().Width > maxX || maxX == -1)
                maxX = texte.GetPosition().x + texte.GetRect().Width;
        }

        minX -= 48;
        maxX += 16;

        bullet2.FlipX(true);

        for(unsigned i = 0 ; i < m_choices.size() ; ++i)
        {
            texte.SetString(m_choices[i].text);
            texte.SetPosition(AutoScreenAdjust(classe->position_contenu_dialogue.x,0).x + classe->position_contenu_dialogue.w * 0.5 - (texte.GetRect().Width) * 0.5,
                                pos);

            bullet1.SetY(texte.GetPosition().y - 4);
            bullet2.SetY(texte.GetPosition().y - 4);

            pos = texte.GetRect().Top + texte.GetRect().Height + 4;

            if(eventManager->getPositionSouris().x > AutoScreenAdjust(classe->position_contenu_dialogue.x,0).x
             &&eventManager->getPositionSouris().x < AutoScreenAdjust(classe->position_contenu_dialogue.x,0).x + classe->position_contenu_dialogue.w
             &&eventManager->getPositionSouris().y > texte.GetRect().Top
             &&eventManager->getPositionSouris().y < texte.GetRect().Top + 20)
             {
                if(eventManager->getEvenement(sf::Mouse::Left, EventClic))
                {
                    m_speak_choice = m_choices[i].no;
                    eventManager->StopEvenement(sf::Mouse::Left, EventClic);
                    eventManager->StopEvenement(sf::Mouse::Left, EventClicA);
                    moteurSons->JouerSon(configuration->sound_menu,coordonnee (0,0),0);
                }
                bullet1.SetImage(*moteurGraphique->getImage(classe->bullet_on.image));
                bullet2.SetImage(*moteurGraphique->getImage(classe->bullet_on.image));
                bullet1.SetX(minX + 16);
                bullet2.SetX(maxX - 16);

                texte.SetColor(sf::Color(255,224,128));
             }
             else
             {
                bullet1.SetImage(*moteurGraphique->getImage(classe->bullet_off.image));
                bullet2.SetImage(*moteurGraphique->getImage(classe->bullet_off.image));
                bullet1.SetX(minX);
                bullet2.SetX(maxX);
                texte.SetColor(sf::Color(224,224,224));
             }

             moteurGraphique->AjouterTexte(&texte,16,0);
             moteurGraphique->AjouterCommande(&bullet1,16,0);
             moteurGraphique->AjouterCommande(&bullet2,16,0);
        }*/

        float minX = -1;
        float maxX = -1;
        for(unsigned i = 0 ; i < m_choices.size() ; ++i)
        {
            texte.SetString(m_choices[i].text);
            texte.SetPosition(AutoScreenAdjust(classe->position_contenu_dialogue.x,0).x + classe->position_contenu_dialogue.w * 0.5 - (texte.GetRect().Width) * 0.5,
                                pos);
            if(texte.GetPosition().x < minX || minX == -1)
                minX = texte.GetPosition().x;
            if(texte.GetPosition().x + texte.GetRect().Width > maxX || maxX == -1)
                maxX = texte.GetPosition().x + texte.GetRect().Width;
        }

        minX -= 48;
        maxX += 16;

        bullet2.FlipX(true);

        for(unsigned i = 0 ; i < m_choices.size() ; ++i)
        {
            texte.SetString(m_choices[i].text);
            texte.SetPosition(minX + 48,
                                pos);

            bullet1.SetY(texte.GetPosition().y - 4);
           // bullet2.SetY(texte.GetPosition().y - 4);

            pos = texte.GetRect().Top + texte.GetRect().Height + 4;

            if(eventManager->getPositionSouris().x > AutoScreenAdjust(classe->position_contenu_dialogue.x,0).x
             &&eventManager->getPositionSouris().x < AutoScreenAdjust(classe->position_contenu_dialogue.x,0).x + classe->position_contenu_dialogue.w
             &&eventManager->getPositionSouris().y > texte.GetRect().Top
             &&eventManager->getPositionSouris().y < texte.GetRect().Top + 20)
             {
                if(eventManager->getEvenement(sf::Mouse::Left, EventClic))
                {
                    m_speak_choice = m_choices[i].no;
                    eventManager->StopEvenement(sf::Mouse::Left, EventClic);
                    eventManager->StopEvenement(sf::Mouse::Left, EventClicA);
                    moteurSons->JouerSon(configuration->sound_menu,coordonnee (0,0),0);
                }
                bullet1.SetImage(*moteurGraphique->getImage(classe->bullet_on.image));
                //bullet2.SetImage(*moteurGraphique->getImage(classe->bullet_on.image));
                bullet1.SetX(minX + 16);
              //  bullet2.SetX(maxX - 16);

                texte.SetColor(sf::Color(255,224,128));
             }
             else
             {
                bullet1.SetImage(*moteurGraphique->getImage(classe->bullet_off.image));
                //bullet2.SetImage(*moteurGraphique->getImage(classe->bullet_off.image));
                bullet1.SetX(minX);
               // bullet2.SetX(maxX);
                texte.SetColor(sf::Color(224,224,224));
             }

             moteurGraphique->AjouterTexte(&texte,16,0);
             moteurGraphique->AjouterCommande(&bullet1,16,0);
            // moteurGraphique->AjouterCommande(&bullet2,16,0);
        }
    }

}


void Menu::AddSpeakChoice(const std::string &text, int no)
{
    m_choices.push_back(Speak_choice ());
    m_choices.back().text   = text;
    m_choices.back().no     = no;
}
void Menu::ClearSpeakChoice()
{
    m_old_choices = m_choices;
    m_speak_choice = -1;
    m_choices.clear();
    clear_diag = false;
}
int  Menu::getSpeakChoice()
{
    return m_speak_choice;
}

void Menu::AfficherDynamique(Caracteristique caracteristique,int type,Caracteristique caracteristiqueMonstre,Classe *classe)
{
    texte.SetFont(moteurGraphique->m_font);
    if (caracteristique.vie>0)
    {
        Sprite sprite;
        sprite.SetImage(*moteurGraphique->getImage(classe->orbe_vie.image));

        sprite.SetY(classe->orbe_vie.position.y + (configuration->Resolution.y - 600));

        if(caracteristique.vie<=(caracteristique.maxVie - caracteristique.reserveVie))
            sprite.SetX((classe->orbe_vie.position.x+(int)((caracteristique.maxVie-caracteristique.vie-caracteristique.reserveVie)/caracteristique.maxVie*classe->orbe_vie.position.w)) + (configuration->Resolution.x - 800) * 0.5);
        else
            sprite.SetX(classe->orbe_vie.position.x + (configuration->Resolution.x - 800) * 0.5);

        sprite.Resize(classe->orbe_vie.position.w, classe->orbe_vie.position.h);

        if(caracteristique.vie<=(caracteristique.maxVie - caracteristique.reserveVie))
        {
            float x = (caracteristique.maxVie-caracteristique.vie-caracteristique.reserveVie)
                        /caracteristique.maxVie*classe->orbe_vie.position.w;

            sprite.SetSubRect(sf::IntRect((int)x,
                                          0,
                                          classe->orbe_vie.position.w - (int) x,
                                          classe->orbe_vie.position.h));
        }
        else
            sprite.SetSubRect(sf::IntRect(0, 0, classe->orbe_vie.position.w, classe->orbe_vie.position.h));

        moteurGraphique->AjouterCommande(&sprite,17,0);

        if(caracteristique.reserveVie > 0)
        {
            sprite.SetX((classe->orbe_vie.position.x+(int)((caracteristique.maxVie-caracteristique.reserveVie)/caracteristique.maxVie*classe->orbe_vie.position.w)) + (configuration->Resolution.x - 800) * 0.5);

            float x = (caracteristique.maxVie-caracteristique.reserveVie)/caracteristique.maxVie*classe->orbe_vie.position.w;
            sprite.SetSubRect(sf::IntRect((int)x, 0, classe->orbe_vie.position.w - (int)x, classe->orbe_vie.position.h));

            sprite.SetColor(sf::Color(32,32,32,255));

            moteurGraphique->AjouterCommande(&sprite,18,0);
        }

        if(caracteristique.vie > caracteristique.maxVie - caracteristique.reserveVie)
        {
            sprite.SetX((classe->orbe_vie.position.x+(int)((caracteristique.maxVie*2-caracteristique.vie-caracteristique.reserveVie*2)/caracteristique.maxVie*classe->orbe_vie.position.w)) + (configuration->Resolution.x - 800) * 0.5);

            float x = (caracteristique.maxVie*2-caracteristique.vie-caracteristique.reserveVie*2)/caracteristique.maxVie*classe->orbe_vie.position.w;
            sprite.SetSubRect(sf::IntRect((int)x, 0, classe->orbe_vie.position.w - (int)x, classe->orbe_vie.position.h));

            sprite.SetBlendMode(Blend::Add);
            sprite.SetColor(sf::Color(255,255,255));
            moteurGraphique->AjouterCommande(&sprite,17,0);
            moteurGraphique->AjouterCommande(&sprite,17,0);
            moteurGraphique->AjouterCommande(&sprite,17,0);
        }
    }

    if(eventManager->getPositionSouris().x > classe->orbe_vie.position.x + (configuration->Resolution.x - 800) * 0.5
    && eventManager->getPositionSouris().x < (classe->orbe_vie.position.x + classe->orbe_vie.position.w) + (configuration->Resolution.x - 800) * 0.5
    && eventManager->getPositionSouris().y > classe->orbe_vie.position.y + (configuration->Resolution.y - 600)
    && eventManager->getPositionSouris().y < (classe->orbe_vie.position.y + classe->orbe_vie.position.h) + (configuration->Resolution.x - 600))
    {
        std::ostringstream buf;
        buf<<configuration->getText(0,3)<<" : "<<(int)caracteristique.vie<<" / "<<caracteristique.maxVie;

        moteurGraphique->AjouterTexte(buf.str(),coordonnee(eventManager->getPositionSouris().x,
                                                                         eventManager->getPositionSouris().y - 20),
                                                                         classe->border,19,0,12,sf::Color(224,224,224));
    }

    if (caracteristique.foi>0)
    {
        Sprite sprite;
        sprite.SetImage(*moteurGraphique->getImage(classe->orbe_foi.image));

        sprite.SetY(classe->orbe_foi.position.y + (configuration->Resolution.y - 600));
        sprite.SetX(classe->orbe_foi.position.x + (configuration->Resolution.x - 800) * 0.5);

        sprite.Resize(classe->orbe_foi.position.w, classe->orbe_foi.position.h);

        if(caracteristique.foi <= (caracteristique.maxFoi - caracteristique.reserveFoi))
            sprite.SetSubRect(sf::IntRect(0, 0, (int)((caracteristique.foi+caracteristique.reserveFoi)/caracteristique.maxFoi*classe->orbe_foi.position.w), classe->orbe_foi.position.h));
        else
            sprite.SetSubRect(sf::IntRect(0, 0, classe->orbe_foi.position.w, classe->orbe_foi.position.h));


        moteurGraphique->AjouterCommande(&sprite,17,0);

        if(caracteristique.reserveFoi > 0)
        {
            sprite.SetSubRect(sf::IntRect(0, 0, (int)(caracteristique.reserveFoi/caracteristique.maxFoi*classe->orbe_foi.position.w), classe->orbe_foi.position.h));

            sprite.SetColor(sf::Color(32,32,32,255));

            moteurGraphique->AjouterCommande(&sprite,18,0);
        }

        if(caracteristique.foi > caracteristique.maxFoi - caracteristique.reserveFoi)
        {
            sprite.SetSubRect(sf::IntRect(0, 0, (int)((caracteristique.foi-caracteristique.maxFoi+caracteristique.reserveFoi*2)/caracteristique.maxFoi*classe->orbe_foi.position.w), classe->orbe_foi.position.h));

            sprite.SetBlendMode(Blend::Add);
            sprite.SetColor(sf::Color(255,255,255));
            moteurGraphique->AjouterCommande(&sprite,17,0);
            moteurGraphique->AjouterCommande(&sprite,17,0);
            moteurGraphique->AjouterCommande(&sprite,17,0);
        }
    }

    if(eventManager->getPositionSouris().x > classe->orbe_foi.position.x + (configuration->Resolution.x - 800) * 0.5
    && eventManager->getPositionSouris().x < (classe->orbe_foi.position.x + classe->orbe_foi.position.w) + (configuration->Resolution.x - 800) * 0.5
    && eventManager->getPositionSouris().y > classe->orbe_foi.position.y + (configuration->Resolution.y - 600)
    && eventManager->getPositionSouris().y < (classe->orbe_foi.position.y + classe->orbe_foi.position.h) + (configuration->Resolution.y - 600))
    {
        std::ostringstream buf;
        buf<<configuration->getText(0,4)<<" : "<<(int)caracteristique.foi<<" / "<<caracteristique.maxFoi;

        moteurGraphique->AjouterTexte(buf.str(),coordonnee(eventManager->getPositionSouris().x,
                                                                         eventManager->getPositionSouris().y - 20),
                                                                         classe->border,19,0,12,sf::Color(224,224,224));
    }



    if (caracteristique.ancienPointAme>0)
    {
        Sprite sprite;

        sprite.SetImage(*moteurGraphique->getImage(classe->soul_bar.image));
       // sprite.Resize(344*configuration->Resolution.w/800, 16*configuration->Resolution.w/800);

        int temp= (int) ( (caracteristique.ancienPointAme-((caracteristique.niveau-1)*(caracteristique.niveau-1)*(caracteristique.niveau-1)*10)) * 344 / (((caracteristique.niveau)*(caracteristique.niveau)*(caracteristique.niveau)*10) - ((caracteristique.niveau-1)*(caracteristique.niveau-1)*(caracteristique.niveau-1)*10)));

        sprite.SetSubRect(sf::IntRect(0,0, temp, 16));

        sprite.SetX(classe->soul_bar.position.x + (configuration->Resolution.x - 800) * 0.5);
        sprite.SetY(classe->soul_bar.position.y + (configuration->Resolution.y - 600));

        moteurGraphique->AjouterCommande(&sprite,17,0);
    }



    if(eventManager->getPositionSouris().x > classe->soul_bar.position.x + (configuration->Resolution.x - 800) * 0.5
    && eventManager->getPositionSouris().x < classe->soul_bar.position.x + (configuration->Resolution.x - 800) * 0.5 + classe->soul_bar.position.w
    && eventManager->getPositionSouris().y > classe->soul_bar.position.y + (configuration->Resolution.y - 600)
    && eventManager->getPositionSouris().y < classe->soul_bar.position.y + (configuration->Resolution.y - 600) + classe->soul_bar.position.h)
    {
        std::ostringstream buf;
        buf<<configuration->getText(0,97)<<(int)caracteristique.ancienPointAme<<" / "<<((caracteristique.niveau)*(caracteristique.niveau)*(caracteristique.niveau)*10);

        moteurGraphique->AjouterTexte(buf.str(),coordonnee(eventManager->getPositionSouris().x,
                                                                         eventManager->getPositionSouris().y - 20),
                                                                         classe->border,19,0,12,sf::Color(224,224,224));
    }

    if (type==1 && caracteristiqueMonstre.maxVie > 1)
    {
        Sprite sprite,sprite2;

        sprite.SetImage(*moteurGraphique->getImage(classe->barre_vie_monstre_vide.image));
        sprite.SetX(configuration->Resolution.w/2+classe->barre_vie_monstre_vide.position.x);
        sprite.SetY(classe->barre_vie_monstre_vide.position.y);

        moteurGraphique->AjouterCommande(&sprite,16,0);

        sprite2.SetImage(*moteurGraphique->getImage(classe->barre_vie_monstre.image));

        sprite2.SetSubRect(sf::IntRect(0, 0, (int)(caracteristiqueMonstre.vie/caracteristiqueMonstre.maxVie*moteurGraphique->getImage(classe->barre_vie_monstre.image)->GetWidth()), 32));
        sprite2.SetX(configuration->Resolution.w/2 + classe->barre_vie_monstre.position.x);
        sprite2.SetY(classe->barre_vie_monstre.position.y);


        moteurGraphique->AjouterCommande(&sprite2,17,0);

        texte.SetFont(moteurGraphique->m_font);


        texte.SetCharacterSize(16);

        {
            std::ostringstream buf;
            buf<<caracteristiqueMonstre.nom;

            buf << " ("<<(int)caracteristiqueMonstre.vie<<" / "<<caracteristiqueMonstre.maxVie<<")";

            texte.SetString(buf.str());
        }

        texte.SetColor(GetItemColor(caracteristiqueMonstre.rang));

        texte.SetX(configuration->Resolution.w/2-(texte.GetRect().Width)/2);
        texte.SetY(32);
        moteurGraphique->AjouterTexte(&texte,18);

        texte.SetStyle(0);
    }
    else if(caracteristiqueMonstre.maxVie == 1 && m_dialogue.empty())
    {
        coordonnee pos =  eventManager->getPositionSouris();
        pos.y -= 28;
        moteurGraphique->AjouterTexte(caracteristiqueMonstre.nom, pos, classe->border,20, 0, 14, sf::Color(224,224,224));
    }
}

void Menu::AfficherChargement(string nom,int fond,int z=50)
{
    Sprite sprite;

    sprite.SetImage(*moteurGraphique->getImage(fond));
    sprite.SetColor(Color(255,255,255,z*255/50));
    sprite.SetX(0);
    sprite.SetY(0);
    sprite.Resize(configuration->Resolution.w,configuration->Resolution.h*5/6);
    moteurGraphique->AjouterCommande(&sprite,12);


    /*texte.SetFont(moteurGraphique->m_font_titre);
    texte.SetCharacterSize(50*configuration->Resolution.h/600);
    texte.SetString(nom);

    texte.SetX(configuration->Resolution.w/2-(texte.GetRect().Width)/2);
    texte.SetY(configuration->Resolution.h-(texte.GetRect().Height)/2-80*configuration->Resolution.h/600);
    texte.SetColor(Color(150,100,50,z*255/50));
    moteurGraphique->AjouterTexte(&texte,16,1);*/

    moteurGraphique->special_typo.Draw(nom, sf::Vector2f(configuration->Resolution.w/2, configuration->Resolution.h - 104),
                                       72, 18, true);
}


void Menu::AfficherQuetes(float decalage,Classe *classe)
{
    Sprite sprite;

    sprite.SetImage(*moteurGraphique->getImage(classe->quest.image));
    sprite.SetX(classe->quest.position.x + (configuration->Resolution.x - 800) * 0.5);
    sprite.SetY(classe->quest.position.y + (configuration->Resolution.y - 600) -decalage*configuration->Resolution.h/600);
    sprite.Resize(classe->quest.position.w, classe->quest.position.h);

    moteurGraphique->AjouterCommande(&sprite,15,0);

    sprite.SetColor(sf::Color(0,0,0,(int)((600+decalage)*254/600)));
    sprite.SetX(0);
    sprite.SetY(0);
    sprite.Resize(configuration->Resolution.w, configuration->Resolution.h);
    moteurGraphique->AjouterCommande(&sprite,14,0);
}

void Menu::AfficherDocs(float decalage,Classe *classe)
{
    Sprite sprite;

    sprite.SetImage(*moteurGraphique->getImage(classe->documents.image));
    sprite.SetX(classe->documents.position.x + (configuration->Resolution.x - 800) * 0.5);
    sprite.SetY(classe->documents.position.y + (configuration->Resolution.y - 600) -decalage*configuration->Resolution.h/600);
    sprite.Resize(classe->documents.position.w, classe->documents.position.h);

    moteurGraphique->AjouterCommande(&sprite,15,0);

    sprite.SetColor(sf::Color(0,0,0,(int)((600+decalage)*254/600)));
    sprite.SetX(0);
    sprite.SetY(0);
    sprite.Resize(configuration->Resolution.w, configuration->Resolution.h);
    moteurGraphique->AjouterCommande(&sprite,14,0);
}


void Menu::AfficherCraft(float decalage,Classe *classe)
{
    Sprite sprite;

    sprite.SetImage(*moteurGraphique->getImage(classe->craft.image));
    sprite.SetX(classe->craft.position.x + (configuration->Resolution.x - 800) * 0.5);
    sprite.SetY(classe->craft.position.y + (configuration->Resolution.y - 600) -decalage*configuration->Resolution.h/600);
    sprite.Resize(classe->craft.position.w, classe->craft.position.h);

    moteurGraphique->AjouterCommande(&sprite,15,0);

    sprite.SetColor(sf::Color(0,0,0,(int)((600+decalage)*254/600)));
    sprite.SetX(0);
    sprite.SetY(0);
    sprite.Resize(configuration->Resolution.w, configuration->Resolution.h);
    moteurGraphique->AjouterCommande(&sprite,14,0);

    classe->schema_craft.Afficher(decalage);
    classe->result_craft.Afficher(decalage);
    classe->button_craft.Afficher(decalage);
}

void Menu::AfficherMiracles(float decalage, Classe *classe)
{
    Sprite sprite;

    sprite.SetImage(*moteurGraphique->getImage(classe->interface_miracles.image));
    sprite.SetX(classe->interface_miracles.position.x + (configuration->Resolution.x - 800) * 0.5);
    sprite.SetY(classe->interface_miracles.position.y + (configuration->Resolution.y - 600) -decalage*configuration->Resolution.h/600);
    sprite.Resize(classe->interface_miracles.position.w, classe->interface_miracles.position.h);

    moteurGraphique->AjouterCommande(&sprite,15,0);

    sprite.SetColor(sf::Color(0,0,0,(int)((600+decalage)*254/600)));
    sprite.SetX(0);
    sprite.SetY(0);
    sprite.Resize(configuration->Resolution.w, configuration->Resolution.h);
    moteurGraphique->AjouterCommande(&sprite,14,0);
}

void Menu::AfficherInventaire(float decalage,Classe *classe,bool noTrader)
{
    Sprite sprite;

    sprite.SetImage(*moteurGraphique->getImage(classe->inventaire.image));
    sprite.SetX(classe->inventaire.position.x + (configuration->Resolution.x - 800) * 0.5);
    sprite.SetY(classe->inventaire.position.y + (configuration->Resolution.y - 600) -decalage*configuration->Resolution.h/600);
    sprite.Resize(classe->inventaire.position.w, classe->inventaire.position.h);

    moteurGraphique->AjouterCommande(&sprite,15,0);

    sprite.SetColor(sf::Color(0,0,0,(int)((600+decalage)*254/600)));
    sprite.SetX(0);
    sprite.SetY(0);
    sprite.Resize(configuration->Resolution.w, configuration->Resolution.h);
    moteurGraphique->AjouterCommande(&sprite,14,0);

    if (!noTrader)
    {
        Sprite sprite2;
        sprite2.SetImage(*moteurGraphique->getImage(classe->menu_marchand.image));
        sprite2.SetX(classe->menu_marchand.position.x + (configuration->Resolution.x - 800) * 0.5);
        sprite2.SetY(classe->menu_marchand.position.y + (configuration->Resolution.y - 600) -decalage*configuration->Resolution.h/600);
        sprite2.Resize(classe->menu_marchand.position.w, classe->menu_marchand.position.h);

        moteurGraphique->AjouterCommande(&sprite2,16,0);
    }
}


