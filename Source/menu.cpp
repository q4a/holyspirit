

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

Menu::~Menu()
{
    if (configuration->debug)
        console->Ajouter("Destruction du menu ...");
    m_sang.clear();
    m_ame.clear();
}

Menu::Menu()
{
    m_alphaSang=0;
    console->Ajouter("",0);
    console->Ajouter("Chargement des menus :",0);
    m_imageAme=moteurGraphique->AjouterImage(configuration->chemin_menus+configuration->nom_ame,-1);
    m_imageSang=moteurGraphique->AjouterImage(configuration->chemin_menus+configuration->nom_sang,-1);
    m_barrePointAme=moteurGraphique->AjouterImage(configuration->chemin_menus+configuration->nom_barre_ame,-1);

    m_barreVie=moteurGraphique->AjouterImage(configuration->chemin_menus+configuration->nom_barre_vie,-1);
    m_barreVieVide=moteurGraphique->AjouterImage(configuration->chemin_menus+configuration->nom_barre_vie_vide,-1);

    m_dialogue.clear();
    m_speak_choice = -1;
}

void Menu::AfficherHUD(Classe *classe)
{
    Sprite sprite2;

    //On affiche l'HUD
    sprite2.SetImage(*moteurGraphique->getImage(classe->hud.image));
    sprite2.SetX(classe->hud.position.x*configuration->Resolution.x/800);
    sprite2.SetY(classe->hud.position.y*configuration->Resolution.h/600);
    sprite2.Resize(classe->hud.position.w*configuration->Resolution.w/800, classe->hud.position.h*configuration->Resolution.h/600);
    moteurGraphique->AjouterCommande(&sprite2,17,0);

    Sprite sprite3;

    sprite3.SetImage(*moteurGraphique->getImage(classe->cache_vie.image));
    sprite3.SetX(classe->cache_vie.position.x*configuration->Resolution.x/800);
    sprite3.SetY(classe->cache_vie.position.y*configuration->Resolution.h/600);
    sprite3.Resize(classe->cache_vie.position.w*configuration->Resolution.w/800, classe->cache_vie.position.h*configuration->Resolution.h/600);
    moteurGraphique->AjouterCommande(&sprite3,18,0);

    sprite3.SetImage(*moteurGraphique->getImage(classe->cache_foi.image));
    sprite3.SetX(classe->cache_foi.position.x*configuration->Resolution.x/800);
    sprite3.SetY(classe->cache_foi.position.y*configuration->Resolution.h/600);
    sprite3.Resize(classe->cache_foi.position.w*configuration->Resolution.w/800, classe->cache_foi.position.h*configuration->Resolution.h/600);
    moteurGraphique->AjouterCommande(&sprite3,18,0);
}

void Menu::AfficherDialogue(int alpha,Classe *classe)
{
    Sprite sprite2;

    sprite2.SetImage(*moteurGraphique->getImage(classe->talk.image));
    sprite2.SetX(classe->talk.position.x*configuration->Resolution.x/800);
    sprite2.SetY(classe->talk.position.y*configuration->Resolution.h/600 + classe->talk.position.h*configuration->Resolution.h/600 - classe->talk.position.h*configuration->Resolution.h/600*alpha/255);
    sprite2.Resize(classe->talk.position.w*configuration->Resolution.w/800, classe->talk.position.h*configuration->Resolution.h/600);
    moteurGraphique->AjouterCommande(&sprite2,16,0);

    sf::Text texte;
    texte.SetCharacterSize(11 * configuration->Resolution.h/600);
    texte.SetFont(moteurGraphique->m_font);
    texte.SetString(m_dialogue);
    texte.SetPosition(classe->position_contenu_dialogue.x * configuration->Resolution.w/800 + classe->position_contenu_dialogue.w * configuration->Resolution.w/800/2 - (texte.GetRect().Right-texte.GetRect().Left)/2,
                      classe->position_contenu_dialogue.y  * configuration->Resolution.h/600 + classe->talk.position.h*configuration->Resolution.h/600 - classe->talk.position.h*configuration->Resolution.h/600*alpha/255);

    moteurGraphique->AjouterTexte(&texte,16,0);

    float pos = texte.GetRect().Bottom + 16;

    for(int i = 0 ; i < m_choices.size() ; ++i)
    {
        texte.SetString(m_choices[i].text);
        texte.SetPosition(texte.GetPosition().x, pos);

        pos = texte.GetRect().Bottom + 4;

        if(eventManager->getPositionSouris().x > texte.GetRect().Left
         &&eventManager->getPositionSouris().x < texte.GetRect().Right
         &&eventManager->getPositionSouris().y > texte.GetRect().Top
         &&eventManager->getPositionSouris().y < texte.GetRect().Bottom)
         {
             sf::Sprite background;
             background.SetImage(*moteurGraphique->getImage(0));
             background.Resize(texte.GetRect().Right - texte.GetRect().Left + 4, texte.GetRect().Bottom - texte.GetRect().Top + 4);
             background.SetPosition(texte.GetRect().Left - 2, texte.GetRect().Top - 2);
             background.SetColor(sf::Color(64,64,64));
             moteurGraphique->AjouterCommande(&background, 16, 0);

            if(eventManager->getEvenement(sf::Mouse::Left, EventClic))
            {
                m_speak_choice = m_choices[i].no;
                eventManager->StopEvenement(sf::Mouse::Left, EventClic);
            }

         }

         moteurGraphique->AjouterTexte(&texte,16,0);
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
    m_speak_choice = -1;
    m_choices.clear();
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

        sprite.SetX(classe->orbe_vie.position.x*configuration->Resolution.x/800);

        if(caracteristique.vie<=(caracteristique.maxVie - caracteristique.reserveVie))
            sprite.SetY(classe->orbe_vie.position.y*configuration->Resolution.h/600+(int)((caracteristique.maxVie-caracteristique.vie-caracteristique.reserveVie)/caracteristique.maxVie*classe->orbe_vie.position.h*configuration->Resolution.h/600));
        else
            sprite.SetY(classe->orbe_vie.position.y*configuration->Resolution.h/600);
        sprite.Resize(classe->orbe_vie.position.w*configuration->Resolution.w/800, classe->orbe_vie.position.h*configuration->Resolution.h/600);

        if(caracteristique.vie<=(caracteristique.maxVie - caracteristique.reserveVie))
            sprite.SetSubRect(sf::IntRect(0, (int)(classe->orbe_vie.position.h-(caracteristique.vie + caracteristique.reserveVie)*classe->orbe_vie.position.h/(caracteristique.maxVie)), classe->orbe_vie.position.w, (caracteristique.maxVie-caracteristique.reserveVie)/caracteristique.maxVie*classe->orbe_vie.position.h));
        else
            sprite.SetSubRect(sf::IntRect(0, 0, classe->orbe_vie.position.w, (caracteristique.maxVie-caracteristique.reserveVie)/caracteristique.maxVie*classe->orbe_vie.position.h));

        moteurGraphique->AjouterCommande(&sprite,17,0);

        if(caracteristique.reserveVie > 0)
        {
            sprite.SetY(classe->orbe_vie.position.y*configuration->Resolution.h/600+(int)((caracteristique.maxVie-caracteristique.reserveVie)/caracteristique.maxVie*classe->orbe_vie.position.h*configuration->Resolution.h/600));
            sprite.SetSubRect(sf::IntRect(0, (int)(classe->orbe_vie.position.h-caracteristique.reserveVie*classe->orbe_vie.position.h/(caracteristique.maxVie)), classe->orbe_vie.position.w, classe->orbe_vie.position.h));

            sprite.SetColor(sf::Color(64,64,64,255));

            moteurGraphique->AjouterCommande(&sprite,18,0);
        }

        if(caracteristique.vie > caracteristique.maxVie - caracteristique.reserveVie)
        {
            sprite.SetY(classe->orbe_vie.position.y*configuration->Resolution.h/600+(int)((caracteristique.maxVie*2-caracteristique.vie-caracteristique.reserveVie*2)/caracteristique.maxVie*classe->orbe_vie.position.h*configuration->Resolution.h/600));
            sprite.SetSubRect(sf::IntRect(0, (int)(classe->orbe_vie.position.h-(caracteristique.vie - caracteristique.maxVie + caracteristique.reserveVie*2)*classe->orbe_vie.position.h/(caracteristique.maxVie)), classe->orbe_vie.position.w, (caracteristique.maxVie-caracteristique.reserveVie)/caracteristique.maxVie*classe->orbe_vie.position.h));

            sprite.SetBlendMode(Blend::Add);
            sprite.SetColor(sf::Color(255,255,255));
            moteurGraphique->AjouterCommande(&sprite,17,0);
        }
    }

    if(eventManager->getPositionSouris().x > classe->orbe_vie.position.x*configuration->Resolution.w/800
    && eventManager->getPositionSouris().x < (classe->orbe_vie.position.x + classe->orbe_vie.position.w)*configuration->Resolution.w/800
    && eventManager->getPositionSouris().y > classe->orbe_vie.position.y*configuration->Resolution.h/600
    && eventManager->getPositionSouris().y < (classe->orbe_vie.position.y + classe->orbe_vie.position.h)*configuration->Resolution.h/600)
    {
        std::ostringstream buf;
        buf<<configuration->getText(0,3)<<" : "<<(int)caracteristique.vie<<" / "<<caracteristique.maxVie;

        moteurGraphique->AjouterTexte(buf.str(),coordonnee(eventManager->getPositionSouris().x,
                                                                         eventManager->getPositionSouris().y - 20),
                                                                         19,0,12,sf::Color(224,224,224),1);
    }

    if (caracteristique.foi>0)
    {
        Sprite sprite;
        sprite.SetImage(*moteurGraphique->getImage(classe->orbe_foi.image));

        sprite.SetX(classe->orbe_foi.position.x*configuration->Resolution.x/800);

        if(caracteristique.foi<=(caracteristique.maxFoi - caracteristique.reserveFoi))
            sprite.SetY(classe->orbe_foi.position.y*configuration->Resolution.h/600+(int)((caracteristique.maxFoi-caracteristique.foi-caracteristique.reserveFoi)/caracteristique.maxFoi*classe->orbe_foi.position.h*configuration->Resolution.h/600));
        else
            sprite.SetY(classe->orbe_foi.position.y*configuration->Resolution.h/600);
        sprite.Resize(classe->orbe_foi.position.w*configuration->Resolution.w/800, classe->orbe_foi.position.h*configuration->Resolution.h/600);

        if(caracteristique.foi<=(caracteristique.maxFoi - caracteristique.reserveFoi))
            sprite.SetSubRect(sf::IntRect(0, (int)(classe->orbe_foi.position.h-(caracteristique.foi + caracteristique.reserveFoi)*classe->orbe_foi.position.h/(caracteristique.maxFoi)), classe->orbe_foi.position.w, (caracteristique.maxFoi-caracteristique.reserveFoi)/caracteristique.maxFoi*classe->orbe_foi.position.h));
        else
            sprite.SetSubRect(sf::IntRect(0, 0, classe->orbe_foi.position.w, (caracteristique.maxFoi-caracteristique.reserveFoi)/caracteristique.maxFoi*classe->orbe_foi.position.h));

        moteurGraphique->AjouterCommande(&sprite,17,0);

        if(caracteristique.reserveFoi > 0)
        {
            sprite.SetY(classe->orbe_foi.position.y*configuration->Resolution.h/600+(int)((caracteristique.maxFoi-caracteristique.reserveFoi)/caracteristique.maxFoi*classe->orbe_foi.position.h*configuration->Resolution.h/600));
            sprite.SetSubRect(sf::IntRect(0, (int)(classe->orbe_foi.position.h-caracteristique.reserveFoi*classe->orbe_foi.position.h/(caracteristique.maxFoi)), classe->orbe_foi.position.w, classe->orbe_foi.position.h));

            sprite.SetColor(sf::Color(64,64,64,255));

            moteurGraphique->AjouterCommande(&sprite,18,0);
        }

        if(caracteristique.foi > caracteristique.maxFoi - caracteristique.reserveFoi)
        {
            sprite.SetY(classe->orbe_foi.position.y*configuration->Resolution.h/600+(int)((caracteristique.maxFoi*2-caracteristique.foi-caracteristique.reserveFoi*2)/caracteristique.maxFoi*classe->orbe_foi.position.h*configuration->Resolution.h/600));
            sprite.SetSubRect(sf::IntRect(0, (int)(classe->orbe_foi.position.h-(caracteristique.foi - caracteristique.maxFoi + caracteristique.reserveFoi*2)*classe->orbe_foi.position.h/(caracteristique.maxFoi)), classe->orbe_foi.position.w, (caracteristique.maxFoi-caracteristique.reserveFoi)/caracteristique.maxFoi*classe->orbe_foi.position.h));

            sprite.SetBlendMode(Blend::Add);
            sprite.SetColor(sf::Color(255,255,255));
            moteurGraphique->AjouterCommande(&sprite,17,0);
        }
    }

    if(eventManager->getPositionSouris().x > classe->orbe_foi.position.x*configuration->Resolution.w/800
    && eventManager->getPositionSouris().x < (classe->orbe_foi.position.x + classe->orbe_foi.position.w)*configuration->Resolution.w/800
    && eventManager->getPositionSouris().y > classe->orbe_foi.position.y*configuration->Resolution.h/600
    && eventManager->getPositionSouris().y < (classe->orbe_foi.position.y + classe->orbe_foi.position.h)*configuration->Resolution.h/600)
    {
        std::ostringstream buf;
        buf<<configuration->getText(0,4)<<" : "<<(int)caracteristique.foi<<" / "<<caracteristique.maxFoi;

        moteurGraphique->AjouterTexte(buf.str(),coordonnee(eventManager->getPositionSouris().x,
                                                                         eventManager->getPositionSouris().y - 20),
                                                                         19,0,12,sf::Color(224,224,224),1);
    }



    if (caracteristique.ancienPointAme>0)
    {
        Sprite sprite;

        sprite.SetImage(*moteurGraphique->getImage(m_barrePointAme));
        sprite.Resize(90*configuration->Resolution.w/800, 88*configuration->Resolution.w/800);

        int temp= (int) ( (caracteristique.ancienPointAme-((caracteristique.niveau-1)*(caracteristique.niveau-1)*(caracteristique.niveau-1)*10)) * 88 / (((caracteristique.niveau)*(caracteristique.niveau)*(caracteristique.niveau)*10) - ((caracteristique.niveau-1)*(caracteristique.niveau-1)*(caracteristique.niveau-1)*10)));

        sprite.SetSubRect(sf::IntRect(0,88-temp, 90, 88));

        sprite.SetX(configuration->Resolution.w/2-46*configuration->Resolution.w/800);
        sprite.SetY(configuration->Resolution.h-(temp)*configuration->Resolution.h/600);

        moteurGraphique->AjouterCommande(&sprite,17,0);
    }



    if(eventManager->getPositionSouris().x > configuration->Resolution.w/2-46*configuration->Resolution.w/800
    && eventManager->getPositionSouris().x < configuration->Resolution.w/2+46*configuration->Resolution.w/800
    && eventManager->getPositionSouris().y > configuration->Resolution.h-88*configuration->Resolution.h/600
    && eventManager->getPositionSouris().y < configuration->Resolution.h)
    {
        std::ostringstream buf;
        buf<<(int)caracteristique.ancienPointAme<<" / "<<((caracteristique.niveau)*(caracteristique.niveau)*(caracteristique.niveau)*10);

        moteurGraphique->AjouterTexte(buf.str(),coordonnee(eventManager->getPositionSouris().x,
                                                                         eventManager->getPositionSouris().y - 20),
                                                                         19,0,12,sf::Color(224,224,224),1);
    }


    texte.SetCharacterSize(16*configuration->Resolution.y/600);


    texte.SetColor(Color(255,255,255,255));
    {
        std::ostringstream buf;
        buf<<caracteristique.niveau;
        texte.SetString(buf.str());
    }

    texte.SetX(configuration->Resolution.w/2-(texte.GetRect().Right-texte.GetRect().Left)/2);
    texte.SetY(configuration->Resolution.h-52*configuration->Resolution.h/600);

    moteurGraphique->AjouterTexte(&texte,18);
    //ecran->Draw(texte);

    if(caracteristique.pts_restant > 0 || caracteristique.miracles_restant > 0 )
    {
        Sprite sprite;

        sprite.SetImage(*moteurGraphique->getImage(classe->hud_newlevel.image));
        sprite.Resize(classe->hud_newlevel.position.w*configuration->Resolution.w/800,
                      classe->hud_newlevel.position.h*configuration->Resolution.w/800);

        sprite.SetX(classe->hud_newlevel.position.x*configuration->Resolution.w/800);
        sprite.SetY(classe->hud_newlevel.position.y*configuration->Resolution.h/600);

        moteurGraphique->AjouterCommande(&sprite,17,0);
    }

    if (type!=-1)
    {
        for (Iter = m_ame.begin(); Iter != m_ame.end(); ++Iter)
        {
            if (Iter->m_mode<3)
            {
                Sprite sprite;

                sprite.SetImage(*moteurGraphique->getImage(m_imageAme));
                sprite.SetOrigin(16,16);
                sprite.Resize(32*configuration->Resolution.w/800*Iter->m_taille, 32*configuration->Resolution.w/800*Iter->m_taille);
                sprite.SetColor(sf::Color(255,255,255,(int)Iter->m_alpha));
                sprite.SetX((Iter->m_position.x+16)*configuration->Resolution.w/800);
                sprite.SetY((Iter->m_position.y+16)*configuration->Resolution.h/600);

                sprite.SetRotation(Iter->m_rotation);
                moteurGraphique->AjouterCommande(&sprite,17,0);
            }
        }

        for (IterSang = m_sang.begin(); IterSang != m_sang.end(); ++IterSang )
        {
            Sprite sprite;

            sprite.SetImage(*moteurGraphique->getImage(m_imageSang));
            sprite.SetOrigin(200*IterSang->m_taille,200*IterSang->m_taille);
            sprite.SetRotation(IterSang->m_rotation);
            sprite.SetOrigin(0,0);
            sprite.SetSubRect(sf::IntRect(200*IterSang->m_numero, 0,300+300*IterSang->m_numero, 200));
            sprite.Resize(300*configuration->Resolution.x/800*IterSang->m_taille, 200*configuration->Resolution.y/600*IterSang->m_taille);
            sprite.SetColor(sf::Color(255,255,255,(int)IterSang->m_alpha));
            sprite.SetX(IterSang->m_position.x*configuration->Resolution.x/800);
            sprite.SetY(IterSang->m_position.y*configuration->Resolution.y/600);

            moteurGraphique->AjouterCommande(&sprite,17,0);
        }
    }

    if (type==1 && caracteristiqueMonstre.vie > 0)
    {
        Sprite sprite,sprite2;

        sprite.SetImage(*moteurGraphique->getImage(m_barreVieVide));
        sprite.Resize(configuration->Resolution.w/2, 32*configuration->Resolution.w/800);
        sprite.SetX(configuration->Resolution.w/2-sprite.GetSize().x/2);
        sprite.SetY(8);

        moteurGraphique->AjouterCommande(&sprite,16,0);

        sprite2.SetImage(*moteurGraphique->getImage(m_barreVie));
        if (caracteristiqueMonstre.rang==1)
        {
            sprite2.SetColor(Color(128,64,255,255));
        }
        if (caracteristiqueMonstre.rang==2)
        {
            sprite2.SetColor(Color(32,32,255,255));
        }
        sprite2.Resize(configuration->Resolution.w/2, 32*configuration->Resolution.w/800);
        sprite2.SetSubRect(sf::IntRect(0, 0, (int)(caracteristiqueMonstre.vie/caracteristiqueMonstre.maxVie*400), 32));
        sprite2.SetX(configuration->Resolution.w/2-sprite.GetSize().x/2);
        sprite2.SetY(8);


        moteurGraphique->AjouterCommande(&sprite2,17,0);

        texte.SetFont(moteurGraphique->m_font);

        //char chaine[255];

        texte.SetCharacterSize(16*configuration->Resolution.h/600);
        //texte.SetStyle(1);

        {
            std::ostringstream buf;
            buf<<caracteristiqueMonstre.nom;
            /*if (caracteristiqueMonstre.rang==1)
                buf<<"Champion : "<<buf;
            if (caracteristiqueMonstre.rang==2)
                buf<<"Chef : "<<buf;*/

            buf << "( "<<(int)caracteristiqueMonstre.vie<<" / "<<caracteristiqueMonstre.maxVie<<" )";

            texte.SetString(buf.str());
        }

        texte.SetColor(Color(224,224,224,255));
        if (caracteristiqueMonstre.rang==1)
            texte.SetColor(Color(100,50,200,255));
        else if (caracteristiqueMonstre.rang==2)
            texte.SetColor(Color(32,32,128,255));
        else if (caracteristiqueMonstre.rang==3)
            texte.SetColor(Color(255,255,128,255));
        else if (caracteristiqueMonstre.rang==4)
            texte.SetColor(Color(128,255,255,255));
        else if (caracteristiqueMonstre.rang==5)
            texte.SetColor(Color(255,164,32,255));
        else if (caracteristiqueMonstre.rang==6)
            texte.SetColor(Color(224,0,0,255));

        texte.SetX(configuration->Resolution.w/2-(texte.GetRect().Right-texte.GetRect().Left)/2);
        texte.SetY(16*configuration->Resolution.h/600);
        moteurGraphique->AjouterTexte(&texte,18);

        texte.SetStyle(0);
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


    texte.SetFont(moteurGraphique->m_font_titre);
    texte.SetCharacterSize(50*configuration->Resolution.h/600);
    texte.SetString(nom);

    texte.SetX(configuration->Resolution.w/2-(texte.GetRect().Right-texte.GetRect().Left)/2);
    texte.SetY(configuration->Resolution.h-(texte.GetRect().Bottom-texte.GetRect().Top)/2-60*configuration->Resolution.h/600);
    texte.SetColor(Color(150,100,50,z*255/50));
    moteurGraphique->AjouterTexte(&texte,13,1);
}


void Menu::AfficherQuetes(float decalage,Classe *classe)
{
    Sprite sprite;

    sprite.SetImage(*moteurGraphique->getImage(classe->quest.image));
    sprite.SetX(classe->quest.position.x*configuration->Resolution.x/800);
    sprite.SetY(classe->quest.position.y*configuration->Resolution.h/600-decalage*configuration->Resolution.h/600);
    sprite.Resize(classe->quest.position.w*configuration->Resolution.w/800, classe->quest.position.h*configuration->Resolution.h/600);

    moteurGraphique->AjouterCommande(&sprite,15,0);
}


void Menu::AfficherCraft(float decalage,Classe *classe)
{
    Sprite sprite;

    sprite.SetImage(*moteurGraphique->getImage(classe->craft.image));
    sprite.SetX(classe->craft.position.x*configuration->Resolution.x/800);
    sprite.SetY(classe->craft.position.y*configuration->Resolution.h/600-decalage*configuration->Resolution.h/600);
    sprite.Resize(classe->craft.position.w*configuration->Resolution.w/800, classe->craft.position.h*configuration->Resolution.h/600);

    moteurGraphique->AjouterCommande(&sprite,15,0);
}

void Menu::AfficherMiracles(float decalage, Classe *classe, int fenetreEnCours)
{
    if(fenetreEnCours >= 0 && fenetreEnCours < (int)classe->interface_miracles.size())
    {
        Sprite sprite;

        sprite.SetImage(*moteurGraphique->getImage(classe->interface_miracles[fenetreEnCours].image));
        sprite.SetX(classe->interface_miracles[fenetreEnCours].position.x*configuration->Resolution.x/800);
        sprite.SetY(classe->interface_miracles[fenetreEnCours].position.y*configuration->Resolution.h/600-decalage*configuration->Resolution.h/600);
        sprite.Resize(classe->interface_miracles[fenetreEnCours].position.w*configuration->Resolution.w/800, classe->interface_miracles[fenetreEnCours].position.h*configuration->Resolution.h/600);

        moteurGraphique->AjouterCommande(&sprite,15,0);
    }
}

void Menu::AfficherInventaire(float decalage,Classe *classe,bool noTrader)
{
    Sprite sprite;

    sprite.SetImage(*moteurGraphique->getImage(classe->inventaire.image));
    sprite.SetX(classe->inventaire.position.x*configuration->Resolution.x/800);
    sprite.SetY(classe->inventaire.position.y*configuration->Resolution.h/600-decalage*configuration->Resolution.h/600);
    sprite.Resize(classe->inventaire.position.w*configuration->Resolution.w/800, classe->inventaire.position.h*configuration->Resolution.h/600);

    moteurGraphique->AjouterCommande(&sprite,15,0);

    if (!noTrader)
    {
        sprite.SetImage(*moteurGraphique->getImage(classe->menu_marchand.image));
        sprite.SetX(classe->menu_marchand.position.x*configuration->Resolution.x/800);
        sprite.SetY(classe->menu_marchand.position.y*configuration->Resolution.h/600-decalage*configuration->Resolution.h/600);
        sprite.Resize(classe->menu_marchand.position.w*configuration->Resolution.w/800, classe->menu_marchand.position.h*configuration->Resolution.h/600);

        moteurGraphique->AjouterCommande(&sprite,16,0);
    }
}

void Menu::AjouterSang(coordonneeDecimal position)
{
    m_sang.push_back(position);
    /*m_alphaSang+=64;
    if(m_alphaSang>255)
        m_alphaSang=255;*/
}
void Menu::AjouterAme(coordonneeDecimal position,int pointAme)
{
    Ame temp(position,pointAme);
    m_ame.push_back(temp);
}



int Menu::GererDynamique(float temps)
{
    int retour=0,k=0;
    for (Iter = m_ame.begin(); Iter != m_ame.end(); ++Iter,++k)
    {
        if (Iter->m_mode==-1)
        {
            Iter->m_taille+=5*temps;
            Iter->m_alpha+=500*temps;

            if (Iter->m_alpha>=128)
                Iter->m_alpha=128;

            if (Iter->m_taille>=1.5)
                Iter->m_taille=1.5;

            if (Iter->m_alpha>=128&&Iter->m_taille>=1)
            {
                Iter->m_alpha=128;
                Iter->m_taille=1;
                Iter->m_mode=0;
            }
        }
        if (Iter->m_mode==0)
        {
            if (Iter->m_depart.x<=400&&Iter->m_position.x>=383&&Iter->m_position.y>=540||Iter->m_depart.x>400&&Iter->m_position.x<=384&&Iter->m_position.y>=540)
            {
                Iter->m_position.x=384;
                Iter->m_position.y=540;
                Iter->m_mode=1,retour+=Iter->m_pointAme;
            }
            else
            {
                Iter->m_rotation-=200*temps;
                Iter->m_position.x+=(384-Iter->m_depart.x)*temps*0.5;
                Iter->m_position.y+=(540-Iter->m_depart.y)*temps*0.5;

                if (Iter->m_taille>1.5)
                    Iter->augmenter=false;
                if (Iter->m_taille<0.75)
                    Iter->augmenter=true;

                if (Iter->augmenter)
                    Iter->m_taille+=3*temps;
                else
                    Iter->m_taille-=3*temps;

            }
        }
        if (Iter->m_mode==1)
        {
            if (Iter->m_taille<2)
            {
                Iter->m_taille+=2*temps;
                Iter->m_alpha+=200*temps;
                Iter->m_rotation-=300*temps;
            }
            else
                Iter->m_mode=2;
        }
        if (Iter->m_mode==2)
        {
            if (Iter->m_taille>0.1)
            {
                Iter->m_taille-=1*temps*2;
                Iter->m_alpha-=200*temps;
                Iter->m_rotation-=500*temps;
            }
            else
                Iter->m_mode=3;
        }
        if (Iter->m_mode==3)
        {
            m_ame.erase (Iter);

            if ((int)m_ame.size()>0)
                Iter=m_ame.begin()+k;
            else
                break;
        }
    }
    k=0;
    for (IterSang = m_sang.begin(); IterSang != m_sang.end(); ++IterSang,++k )
    {
        IterSang->m_alpha-=temps*300;

        if (IterSang->m_alpha<0)
            IterSang->m_alpha=0;
        if (IterSang->m_alpha==0)
        {
            m_sang.erase(IterSang);

            if ((int)m_sang.size()>0)
                IterSang=m_sang.begin()+k;
            else
                break;
        }
    }

    return retour;
}

