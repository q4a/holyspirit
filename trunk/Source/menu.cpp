

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
        console->Ajouter("Destruction of the menu ...");
}

Menu::Menu()
{
    console->Ajouter("",0);
    console->Ajouter("Loading menus :",0);

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
    MySprite sprite2;

    //On affiche l'HUD
    sprite2.setTexture(*moteurGraphique->getImage(classe->hud.image));
    sprite2.setPosition(classe->hud.position.x + (configuration->Resolution.x - 800) * 0.5 ,
                        classe->hud.position.y + (configuration->Resolution.y - 600));
    sprite2.resize(classe->hud.position.w, classe->hud.position.h);
    moteurGraphique->AjouterCommande(&sprite2,17,0);
}

void Menu::AfficherDialogue(float time,Classe *classe)
{
    MySprite sprite2;

    sprite2.setTexture(*moteurGraphique->getImage(classe->talk.image));
    sprite2.setPosition(classe->talk.position.x + (configuration->Resolution.x - 800) * 0.5 ,
                        AutoScreenAdjust(0,classe->talk.position.y).y + m_cur_talk_hauteur);
    sprite2.resize(classe->talk.position.w, classe->talk.position.h);
    moteurGraphique->AjouterCommande(&sprite2,16,0);

    sf::Text texte2;
    texte2.setCharacterSize(14);
    texte2.setFont(moteurGraphique->m_font);

   // if(m_dialogue.empty())
     //   clear_diag = false;

   /* if(!m_old_dialogue.empty() && !clear_diag)
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

        texte2.setPosition(AutoScreenAdjust(classe->position_contenu_dialogue.x,0).x + classe->position_contenu_dialogue.w * 0.5 - (texte2.getGlobalBounds().Width) * 0.5,
                          AutoScreenAdjust(0,classe->position_contenu_dialogue.y).y + m_cur_talk_hauteur);

        moteurGraphique->AjouterTexte(&texte2,16,0);

        float pos = texte2.getGlobalBounds().Top + texte2.getGlobalBounds().Height + 8;

        float minX = -1;
        float maxX = -1;
        for(unsigned i = 0 ; i < m_old_choices.size() ; ++i)
        {
            texte.SetString(m_old_choices[i].text);
            texte.setPosition(AutoScreenAdjust(classe->position_contenu_dialogue.x,0).x + classe->position_contenu_dialogue.w * 0.5 - (texte.getGlobalBounds().Width) * 0.5,
                                pos);
            if(texte.getPosition().x < minX || minX == -1)
                minX = texte.getPosition().x;
            if(texte.getPosition().x + texte.getGlobalBounds().Width > maxX || maxX == -1)
                maxX = texte.getPosition().x + texte.getGlobalBounds().Width;
        }

        minX -= 48;
        maxX += 16;


        sf::Sprite bullet1, bullet2;

        bullet2.FlipX(true);

        for(unsigned i = 0 ; i < m_old_choices.size() ; ++i)
        {
            texte.SetString(m_old_choices[i].text);
            texte.setPosition(minX + 48,
                                pos);

            bullet1.SetY(texte.getPosition().y - 4);

            pos = texte.getGlobalBounds().Top + texte.getGlobalBounds().Height + 4;


            bullet1.SetTexture(*moteurGraphique->getImage(classe->bullet_off.image));
            bullet1.SetX(minX);
            texte.SetColor(sf::Color(224,224,224));

             moteurGraphique->AjouterTexte(&texte,16,0);
             moteurGraphique->AjouterCommande(&bullet1,16,0);
        }
    }
    else if(!m_dialogue.empty())*/
    {
        m_old_dialogue = m_dialogue;

        texte.setString(m_dialogue);

        m_hauteur = 140;
        m_hauteur += texte.getGlobalBounds().height + 8;

        for(unsigned i = 0 ; i < m_choices.size() ; ++i)
        {
            texte.setString(m_choices[i].text);
            m_hauteur += texte.getGlobalBounds().height + 4;
        }

        m_hauteur = (int)((m_hauteur+16)/32) * 32;
        if(m_dialogue.empty())
            m_hauteur = 0;

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

        texte.setColor(sf::Color(224,224,224));

        texte.setCharacterSize(14);
        texte.setStyle(0);
        texte.setFont(moteurGraphique->m_font);
        texte.setString(m_dialogue);

        texte.setPosition(AutoScreenAdjust(classe->position_contenu_dialogue.x,0).x + classe->position_contenu_dialogue.w * 0.5 - (texte.getGlobalBounds().width) * 0.5,
                          AutoScreenAdjust(0,classe->position_contenu_dialogue.y).y + m_cur_talk_hauteur);//(classe->talk.position.h - m_hauteur) +(classe->talk.position.h) - (classe->talk.position.h) * alpha/255);

        moteurGraphique->AjouterTexte(&texte,16,0);

        float pos = texte.getGlobalBounds().top + texte.getGlobalBounds().height + 8;
        texte.setStyle(0);

        sf::Sprite bullet1, bullet2;

        /*float minX = -1;
        float maxX = -1;
        for(unsigned i = 0 ; i < m_choices.size() ; ++i)
        {
            texte.SetString(m_choices[i].text);
            texte.setPosition(AutoScreenAdjust(classe->position_contenu_dialogue.x,0).x + classe->position_contenu_dialogue.w * 0.5 - (texte.getGlobalBounds().Width) * 0.5,
                                pos);
            if(texte.getPosition().x < minX || minX == -1)
                minX = texte.getPosition().x;
            if(texte.getPosition().x + texte.getGlobalBounds().Width > maxX || maxX == -1)
                maxX = texte.getPosition().x + texte.getGlobalBounds().Width;
        }

        minX -= 48;
        maxX += 16;

        bullet2.FlipX(true);

        for(unsigned i = 0 ; i < m_choices.size() ; ++i)
        {
            texte.SetString(m_choices[i].text);
            texte.setPosition(AutoScreenAdjust(classe->position_contenu_dialogue.x,0).x + classe->position_contenu_dialogue.w * 0.5 - (texte.getGlobalBounds().Width) * 0.5,
                                pos);

            bullet1.SetY(texte.getPosition().y - 4);
            bullet2.SetY(texte.getPosition().y - 4);

            pos = texte.getGlobalBounds().Top + texte.getGlobalBounds().Height + 4;

            if(eventManager->getPositionSouris().x > AutoScreenAdjust(classe->position_contenu_dialogue.x,0).x
             &&eventManager->getPositionSouris().x < AutoScreenAdjust(classe->position_contenu_dialogue.x,0).x + classe->position_contenu_dialogue.w
             &&eventManager->getPositionSouris().y > texte.getGlobalBounds().Top
             &&eventManager->getPositionSouris().y < texte.getGlobalBounds().Top + 20)
             {
                if(eventManager->getEvenement(sf::Mouse::Left, EventClic))
                {
                    m_speak_choice = m_choices[i].no;
                    eventManager->StopEvenement(sf::Mouse::Left, EventClic);
                    eventManager->StopEvenement(sf::Mouse::Left, EventClicA);
                    moteurSons->JouerSon(configuration->sound_menu,coordonnee (0,0),0);
                }
                bullet1.SetTexture(*moteurGraphique->getImage(classe->bullet_on.image));
                bullet2.SetTexture(*moteurGraphique->getImage(classe->bullet_on.image));
                bullet1.SetX(minX + 16);
                bullet2.SetX(maxX - 16);

                texte.SetColor(sf::Color(255,224,128));
             }
             else
             {
                bullet1.SetTexture(*moteurGraphique->getImage(classe->bullet_off.image));
                bullet2.SetTexture(*moteurGraphique->getImage(classe->bullet_off.image));
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
            texte.setString(m_choices[i].text);
            texte.setPosition(AutoScreenAdjust(classe->position_contenu_dialogue.x,0).x + classe->position_contenu_dialogue.w * 0.5 - (texte.getGlobalBounds().width) * 0.5,
                                pos);
            if(texte.getPosition().x < minX || minX == -1)
                minX = texte.getPosition().x;
            if(texte.getPosition().x + texte.getGlobalBounds().width > maxX || maxX == -1)
                maxX = texte.getPosition().x + texte.getGlobalBounds().width;
        }

        minX -= 48;
        maxX += 16;

        bullet2.scale(-1,1);

        int current_choice = -1;
        for(unsigned i = 0 ; i < m_choices.size() ; ++i)
        {
            texte.setString(m_choices[i].text);
            texte.setPosition(minX + 48,
                                pos);

            bullet1.setPosition(bullet1.getPosition().x, texte.getPosition().y - 4);
           // bullet2.SetY(texte.getPosition().y - 4);

            pos = texte.getGlobalBounds().top + texte.getGlobalBounds().height + 4;

            if(eventManager->getPositionSouris().x > AutoScreenAdjust(classe->position_contenu_dialogue.x,0).x
             &&eventManager->getPositionSouris().x < AutoScreenAdjust(classe->position_contenu_dialogue.x,0).x + classe->position_contenu_dialogue.w
             &&eventManager->getPositionSouris().y > texte.getGlobalBounds().top
             &&eventManager->getPositionSouris().y < texte.getGlobalBounds().top + 20)
             {
                 current_choice = i;
                if(eventManager->getEvenement(sf::Mouse::Left, EventClic))
                {
                    m_speak_choice = m_choices[i].no;
                    eventManager->StopEvenement(sf::Mouse::Left, EventClic);
                    eventManager->StopEvenement(sf::Mouse::Left, EventClicA);
                    moteurSons->JouerSon(configuration->sound_menu,coordonnee (0,0),0);
                }
                bullet1.setTexture(*moteurGraphique->getImage(classe->bullet_on.image));
                //bullet2.SetTexture(*moteurGraphique->getImage(classe->bullet_on.image));
                bullet1.setPosition(minX + 16, bullet1.getPosition().y);
              //  bullet2.SetX(maxX - 16);

                if(m_current_choice != (int)i)
                    moteurSons->JouerSon(configuration->sound_dialog,coordonnee (0,0),0);

                texte.setColor(sf::Color(255,224,128));
             }
             else
             {
                bullet1.setTexture(*moteurGraphique->getImage(classe->bullet_off.image));
                //bullet2.SetTexture(*moteurGraphique->getImage(classe->bullet_off.image));
                bullet1.setPosition(minX, bullet1.getPosition().y);
               // bullet2.SetX(maxX);
                texte.setColor(sf::Color(224,224,224));
             }

             moteurGraphique->AjouterTexte(&texte,16,0);
             moteurGraphique->AjouterCommande(&bullet1,16,0);
            // moteurGraphique->AjouterCommande(&bullet2,16,0);
        }
        m_current_choice = current_choice;
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
    texte.setFont(moteurGraphique->m_font);
    if (caracteristique.vie>0)
    {
        MySprite sprite;
        sprite.setTexture(*moteurGraphique->getImage(classe->orbe_vie.image));

        sprite.setPosition(sprite.getPosition().x, classe->orbe_vie.position.y + (configuration->Resolution.y - 600));

        if(caracteristique.vie<=(caracteristique.maxVie - caracteristique.reserveVie))
            sprite.setPosition((classe->orbe_vie.position.x+(int)((caracteristique.maxVie-caracteristique.vie-caracteristique.reserveVie)/caracteristique.maxVie*classe->orbe_vie.position.w)) + (configuration->Resolution.x - 800) * 0.5, sprite.getPosition().y);
        else
            sprite.setPosition(classe->orbe_vie.position.x + (configuration->Resolution.x - 800) * 0.5, sprite.getPosition().y);

        sprite.resize(classe->orbe_vie.position.w, classe->orbe_vie.position.h);

        if(caracteristique.vie<=(caracteristique.maxVie - caracteristique.reserveVie))
        {
            float x = (caracteristique.maxVie-caracteristique.vie-caracteristique.reserveVie)
                        /caracteristique.maxVie*classe->orbe_vie.position.w;

            sprite.setTextureRect(sf::IntRect((int)x,
                                          0,
                                          classe->orbe_vie.position.w - (int) x,
                                          classe->orbe_vie.position.h));
        }
        else
            sprite.setTextureRect(sf::IntRect(0, 0, classe->orbe_vie.position.w, classe->orbe_vie.position.h));

        moteurGraphique->AjouterCommande(&sprite,17,0);

        if(caracteristique.reserveVie > 0)
        {
            sprite.setPosition((classe->orbe_vie.position.x+(int)((caracteristique.maxVie-caracteristique.reserveVie)/caracteristique.maxVie*classe->orbe_vie.position.w)) + (configuration->Resolution.x - 800) * 0.5, sprite.getPosition().y);

            float x = (caracteristique.maxVie-caracteristique.reserveVie)/caracteristique.maxVie*classe->orbe_vie.position.w;
            sprite.setTextureRect(sf::IntRect((int)x, 0, classe->orbe_vie.position.w - (int)x, classe->orbe_vie.position.h));

            sprite.setColor(sf::Color(32,32,32,255));

            moteurGraphique->AjouterCommande(&sprite,18,0);
        }

        if(caracteristique.vie > caracteristique.maxVie - caracteristique.reserveVie)
        {
            sprite.setPosition((classe->orbe_vie.position.x+(int)((caracteristique.maxVie*2-caracteristique.vie-caracteristique.reserveVie*2)/caracteristique.maxVie*classe->orbe_vie.position.w)) + (configuration->Resolution.x - 800) * 0.5, sprite.getPosition().y);

            float x = (caracteristique.maxVie*2-caracteristique.vie-caracteristique.reserveVie*2)/caracteristique.maxVie*classe->orbe_vie.position.w;
            sprite.setTextureRect(sf::IntRect((int)x, 0, classe->orbe_vie.position.w - (int)x, classe->orbe_vie.position.h));

            sprite.setColor(sf::Color(255,255,255));
            moteurGraphique->AjouterCommande(&sprite,17,0,sf::BlendAdd);
            moteurGraphique->AjouterCommande(&sprite,17,0,sf::BlendAdd);
            moteurGraphique->AjouterCommande(&sprite,17,0,sf::BlendAdd);
        }
    }

    if(eventManager->getPositionSouris().x > classe->orbe_vie.position.x + (configuration->Resolution.x - 800) * 0.5
    && eventManager->getPositionSouris().x < (classe->orbe_vie.position.x + classe->orbe_vie.position.w) + (configuration->Resolution.x - 800) * 0.5
    && eventManager->getPositionSouris().y > classe->orbe_vie.position.y + (configuration->Resolution.y - 600)
    && eventManager->getPositionSouris().y < (classe->orbe_vie.position.y + classe->orbe_vie.position.h) + (configuration->Resolution.x - 600))
    {
        std::ostringstream buf;
        buf<<gettext("Life")<<" : "<<(int)caracteristique.vie<<" / "<<caracteristique.maxVie;

        moteurGraphique->AjouterTexte(buf.str(),coordonnee(eventManager->getPositionSouris().x,
                                                                         eventManager->getPositionSouris().y - 20),
                                                                         classe->border,19,0,12,sf::Color(224,224,224));
    }

    if (caracteristique.foi>0)
    {
        MySprite sprite;
        sprite.setTexture(*moteurGraphique->getImage(classe->orbe_foi.image));

        sprite.setPosition(classe->orbe_foi.position.x + (configuration->Resolution.x - 800) * 0.5 ,
                           classe->orbe_foi.position.y + (configuration->Resolution.y - 600));

        sprite.resize(classe->orbe_foi.position.w, classe->orbe_foi.position.h);

        if(caracteristique.foi <= (caracteristique.maxFoi - caracteristique.reserveFoi))
            sprite.setTextureRect(sf::IntRect(0, 0, (int)((caracteristique.foi+caracteristique.reserveFoi)/caracteristique.maxFoi*classe->orbe_foi.position.w), classe->orbe_foi.position.h));
        else
            sprite.setTextureRect(sf::IntRect(0, 0, classe->orbe_foi.position.w, classe->orbe_foi.position.h));


        moteurGraphique->AjouterCommande(&sprite,17,0);

        if(caracteristique.reserveFoi > 0)
        {
            sprite.setTextureRect(sf::IntRect(0, 0, (int)(caracteristique.reserveFoi/caracteristique.maxFoi*classe->orbe_foi.position.w), classe->orbe_foi.position.h));

            sprite.setColor(sf::Color(32,32,32,255));

            moteurGraphique->AjouterCommande(&sprite,18,0);
        }

        if(caracteristique.foi > caracteristique.maxFoi - caracteristique.reserveFoi)
        {
            sprite.setTextureRect(sf::IntRect(0, 0, (int)((caracteristique.foi-caracteristique.maxFoi+caracteristique.reserveFoi*2)/caracteristique.maxFoi*classe->orbe_foi.position.w), classe->orbe_foi.position.h));

            sprite.setColor(sf::Color(255,255,255));
            moteurGraphique->AjouterCommande(&sprite,17,0,sf::BlendAdd);
            moteurGraphique->AjouterCommande(&sprite,17,0,sf::BlendAdd);
            moteurGraphique->AjouterCommande(&sprite,17,0,sf::BlendAdd);
        }
    }

    if(eventManager->getPositionSouris().x > classe->orbe_foi.position.x + (configuration->Resolution.x - 800) * 0.5
    && eventManager->getPositionSouris().x < (classe->orbe_foi.position.x + classe->orbe_foi.position.w) + (configuration->Resolution.x - 800) * 0.5
    && eventManager->getPositionSouris().y > classe->orbe_foi.position.y + (configuration->Resolution.y - 600)
    && eventManager->getPositionSouris().y < (classe->orbe_foi.position.y + classe->orbe_foi.position.h) + (configuration->Resolution.y - 600))
    {
        std::ostringstream buf;
        buf<<gettext("Faith")<<" : "<<(int)caracteristique.foi<<" / "<<caracteristique.maxFoi;

        moteurGraphique->AjouterTexte(buf.str(),coordonnee(eventManager->getPositionSouris().x,
                                                                         eventManager->getPositionSouris().y - 20),
                                                                         classe->border,19,0,12,sf::Color(224,224,224));
    }



    if (caracteristique.ancienPointAme>0)
    {
        Sprite sprite;

        sprite.setTexture(*moteurGraphique->getImage(classe->soul_bar.image));
       // sprite.resize(344*configuration->Resolution.w/800, 16*configuration->Resolution.w/800);

        int temp= (int) ( (caracteristique.ancienPointAme-((caracteristique.niveau-1)*(caracteristique.niveau-1)*(caracteristique.niveau-1)*10)) * 344 / (((caracteristique.niveau)*(caracteristique.niveau)*(caracteristique.niveau)*10) - ((caracteristique.niveau-1)*(caracteristique.niveau-1)*(caracteristique.niveau-1)*10)));

        sprite.setTextureRect(sf::IntRect(0,0, temp, 16));

        sprite.setPosition(classe->soul_bar.position.x + (configuration->Resolution.x - 800) * 0.5 ,
                           classe->soul_bar.position.y + (configuration->Resolution.y - 600));

        moteurGraphique->AjouterCommande(&sprite,17,0);
    }



    if(eventManager->getPositionSouris().x > classe->soul_bar.position.x + (configuration->Resolution.x - 800) * 0.5
    && eventManager->getPositionSouris().x < classe->soul_bar.position.x + (configuration->Resolution.x - 800) * 0.5 + classe->soul_bar.position.w
    && eventManager->getPositionSouris().y > classe->soul_bar.position.y + (configuration->Resolution.y - 600)
    && eventManager->getPositionSouris().y < classe->soul_bar.position.y + (configuration->Resolution.y - 600) + classe->soul_bar.position.h)
    {
        std::ostringstream buf;
        buf<<gettext("Points of souls: ")<<(int)caracteristique.ancienPointAme<<" / "<<((caracteristique.niveau)*(caracteristique.niveau)*(caracteristique.niveau)*10);

        moteurGraphique->AjouterTexte(buf.str(),coordonnee(eventManager->getPositionSouris().x,
                                                                         eventManager->getPositionSouris().y - 20),
                                                                         classe->border,19,0,12,sf::Color(224,224,224));
    }

    if (type==1 && caracteristiqueMonstre.maxVie > 1)
    {
        Sprite sprite,sprite2;

        sprite.setTexture(*moteurGraphique->getImage(classe->barre_vie_monstre_vide.image));
        sprite.setPosition(configuration->Resolution.w/2+classe->barre_vie_monstre_vide.position.x ,
                           classe->barre_vie_monstre_vide.position.y);

        moteurGraphique->AjouterCommande(&sprite,16,0);

        sprite2.setTexture(*moteurGraphique->getImage(classe->barre_vie_monstre.image));

        float vie = (int)caracteristiqueMonstre.vie;
        if(vie == 0)
            vie = 1;
        sprite2.setTextureRect(sf::IntRect(0, 0, (int)(vie/caracteristiqueMonstre.maxVie*moteurGraphique->getImage(classe->barre_vie_monstre.image)->getWidth()), 32));
        sprite2.setPosition(configuration->Resolution.w/2 + classe->barre_vie_monstre.position.x ,
                            classe->barre_vie_monstre.position.y);


        moteurGraphique->AjouterCommande(&sprite2,17,0);

        texte.setFont(moteurGraphique->m_font);


        texte.setCharacterSize(16);

        {
            std::ostringstream buf;
            buf<<caracteristiqueMonstre.nom;

            buf << " ("<<(int)caracteristiqueMonstre.vie<<" / "<<caracteristiqueMonstre.maxVie<<")";

            texte.setString(buf.str());
        }

        texte.setColor(GetItemColor(caracteristiqueMonstre.rang));

        texte.setPosition(configuration->Resolution.w/2-(texte.getGlobalBounds().width)/2 ,
                          32);
        moteurGraphique->AjouterTexte(&texte,18);

        texte.setStyle(0);
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
    MySprite sprite;

    sprite.setTexture(*moteurGraphique->getImage(fond));
    sprite.setColor(Color(255,255,255,z*255/50));
    sprite.setPosition(0,0);
    sprite.resize(configuration->Resolution.w,configuration->Resolution.h*5/6);
    moteurGraphique->AjouterCommande(&sprite,12);


    /*texte.SetFont(moteurGraphique->m_font_titre);
    texte.SetCharacterSize(50*configuration->Resolution.h/600);
    texte.SetString(nom);

    texte.SetX(configuration->Resolution.w/2-(texte.getGlobalBounds().Width)/2);
    texte.SetY(configuration->Resolution.h-(texte.getGlobalBounds().Height)/2-80*configuration->Resolution.h/600);
    texte.SetColor(Color(150,100,50,z*255/50));
    moteurGraphique->AjouterTexte(&texte,16,1);*/

    moteurGraphique->special_typo.Draw(nom, sf::Vector2f(configuration->Resolution.w/2, configuration->Resolution.h - 104),
                                       72, 18, true);
}


void Menu::AfficherQuetes(float decalage,Classe *classe)
{
    MySprite sprite;

    sprite.setTexture(*moteurGraphique->getImage(classe->quest.image));
    sprite.setPosition(classe->quest.position.x + (configuration->Resolution.x - 800) * 0.5 ,
                       classe->quest.position.y + (configuration->Resolution.y - 600) -decalage*configuration->Resolution.h/600);
    sprite.resize(classe->quest.position.w, classe->quest.position.h);

    moteurGraphique->AjouterCommande(&sprite,15,0);

    sprite.setColor(sf::Color(0,0,0,(int)((600+decalage)*254/600)));
    sprite.setPosition(0,0);
    sprite.resize(configuration->Resolution.w, configuration->Resolution.h);
    moteurGraphique->AjouterCommande(&sprite,14,0);
}

void Menu::AfficherDocs(float decalage,Classe *classe)
{
    MySprite sprite;

    sprite.setTexture(*moteurGraphique->getImage(classe->documents.image));
    sprite.setPosition(classe->documents.position.x + (configuration->Resolution.x - 800) * 0.5 ,
                       classe->documents.position.y + (configuration->Resolution.y - 600) -decalage*configuration->Resolution.h/600);
    sprite.resize(classe->documents.position.w, classe->documents.position.h);

    moteurGraphique->AjouterCommande(&sprite,15,0);

    sprite.setColor(sf::Color(0,0,0,(int)((600+decalage)*254/600)));
    sprite.setPosition(0,0);
    sprite.resize(configuration->Resolution.w, configuration->Resolution.h);
    moteurGraphique->AjouterCommande(&sprite,14,0);
}


void Menu::AfficherCraft(float decalage,Classe *classe)
{
    MySprite sprite;

    sprite.setTexture(*moteurGraphique->getImage(classe->craft.image));
    sprite.setPosition(classe->craft.position.x + (configuration->Resolution.x - 800) * 0.5 ,
                       classe->craft.position.y + (configuration->Resolution.y - 600) -decalage*configuration->Resolution.h/600);
    sprite.resize(classe->craft.position.w, classe->craft.position.h);

    moteurGraphique->AjouterCommande(&sprite,15,0);

    sprite.setColor(sf::Color(0,0,0,(int)((600+decalage)*254/600)));
    sprite.setPosition(0,0);
    sprite.resize(configuration->Resolution.w, configuration->Resolution.h);
    moteurGraphique->AjouterCommande(&sprite,14,0);

    classe->schema_craft.Afficher(decalage);
    classe->result_craft.Afficher(decalage);
    classe->button_craft.Afficher(decalage);
}

void Menu::AfficherMiracles(float decalage, Classe *classe)
{
    MySprite sprite;

    sprite.setTexture(*moteurGraphique->getImage(classe->interface_miracles.image));
    sprite.setPosition(classe->interface_miracles.position.x + (configuration->Resolution.x - 800) * 0.5,
                       classe->interface_miracles.position.y + (configuration->Resolution.y - 600) -decalage*configuration->Resolution.h/600);
    sprite.resize(classe->interface_miracles.position.w, classe->interface_miracles.position.h);

    moteurGraphique->AjouterCommande(&sprite,15,0);

    sprite.setColor(sf::Color(0,0,0,(int)((600+decalage)*254/600)));
    sprite.setPosition(0,0);
    sprite.resize(configuration->Resolution.w, configuration->Resolution.h);
    moteurGraphique->AjouterCommande(&sprite,14,0);
}

void Menu::AfficherInventaire(float decalage,Classe *classe,bool noTrader)
{
    MySprite sprite;

    sprite.setTexture(*moteurGraphique->getImage(classe->inventaire.image));
    sprite.setPosition(classe->inventaire.position.x + (configuration->Resolution.x - 800) * 0.5 ,
                       classe->inventaire.position.y + (configuration->Resolution.y - 600) -decalage*configuration->Resolution.h/600);
    sprite.resize(classe->inventaire.position.w, classe->inventaire.position.h);

    moteurGraphique->AjouterCommande(&sprite,15,0);

    sprite.setTexture(*moteurGraphique->getImage(0));
    sprite.setColor(sf::Color(0,0,0,(int)((600+decalage)*254/600)));
    sprite.setPosition(0,0);
    sprite.resize(configuration->Resolution.x, configuration->Resolution.y);
    moteurGraphique->AjouterCommande(&sprite,14,0);

    //configuration->effetNoir = decalage/600;

    if (!noTrader)
    {
        MySprite sprite2;
        sprite2.setTexture(*moteurGraphique->getImage(classe->menu_marchand.image));
        sprite2.setPosition(classe->menu_marchand.position.x + (configuration->Resolution.x - 800) * 0.5 ,
                            classe->menu_marchand.position.y + (configuration->Resolution.y - 600) -decalage*configuration->Resolution.h/600);
        sprite2.resize(classe->menu_marchand.position.w, classe->menu_marchand.position.h);

        moteurGraphique->AjouterCommande(&sprite2,16,0);
    }
}


