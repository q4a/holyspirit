

/*Copyright (C) 2009 Naisse Gr�goire

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


#include "c_jeu.h"
#include "../jeu.h"

#include "../globale.h"

#include <iostream>
#include <fstream>

#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

using namespace sf;
using namespace std;

int GestionBoutons(Jeu *jeu, bool = false, bool = true, bool = false);
void GestionRaccourcis(Jeu *jeu);

inline sf::Vector2f AutoScreenAdjust(float x, float y, float decalage = 0)
{
    sf::Vector2f temp;
    temp.x = x + (configuration->Resolution.x - 800) * 0.5;
    temp.y = y + (configuration->Resolution.y - 600) - decalage * configuration->Resolution.h/600;
    return temp;
}

c_Inventaire::c_Inventaire()
{
    m_decalage=-600;
    m_trader=NULL;
}
c_Inventaire::~c_Inventaire()
{
}

int TrierInventaire(std::vector<Objet> *trade, int largeur, bool enHauteur)
{
    int taille = 0;
    bool continuer=true;

    for(unsigned i=0;i<trade->size();++i)
    if(!(*trade)[i].m_dejaTrie)
    {
        continuer = true;
        if(enHauteur)
        {
            for (int x=0;continuer;x++)
                for (int y=0;y<largeur&&continuer;y++)
                {
                    bool ajouter=true;
                    for (int h=0;h<(*trade)[i].getTaille().y;h++)
                        for (int w=0;w<(*trade)[i].getTaille().x;w++)
                            if (y+h<largeur)
                            {
                                for (unsigned j=0;j<i;j++)
                                    for (int Y=0;Y<(*trade)[j].getTaille().y;Y++)
                                        for (int X=0;X<(*trade)[j].getTaille().x;X++)
                                            if ((*trade)[j].getPosition().x+X==x+w && (*trade)[j].getPosition().y+Y==y+h)
                                                ajouter=false;
                            }
                            else
                                ajouter=false;

                    if (ajouter)
                    {
                        continuer=false;
                        (*trade)[i].setPosition(x,y);
                        (*trade)[i].m_dejaTrie = true;

                        if(x + (*trade)[i].getTaille().x > taille)
                            taille = x + (*trade)[i].getTaille().x;
                    }
                }
        }
        else
        {
            for (int y=0;continuer;y++)
                for (int x=0;x<largeur&&continuer;x++)
                {
                    bool ajouter=true;
                    for (int h=0;h<(*trade)[i].getTaille().y;h++)
                        for (int w=0;w<(*trade)[i].getTaille().x;w++)
                            if (x+w<largeur)
                            {
                                for (unsigned j=0;j<i;j++)
                                    for (int Y=0;Y<(*trade)[j].getTaille().y;Y++)
                                        for (int X=0;X<(*trade)[j].getTaille().x;X++)
                                            if ((*trade)[j].getPosition().x+X==x+w && (*trade)[j].getPosition().y+Y==y+h)
                                                ajouter=false;
                            }
                            else
                                ajouter=false;

                    if (ajouter)
                    {
                        continuer=false;
                        (*trade)[i].setPosition(x,y);
                        (*trade)[i].m_dejaTrie = true;

                        if(y + (*trade)[i].getTaille().y > taille)
                            taille = y + (*trade)[i].getTaille().y;


                    }
                }
        }
    }



    return taille;
}

void c_Inventaire::setTrader(std::vector<Objet> *trade)
{
    m_trader = trade;
}



void c_Inventaire::Utiliser(Jeu *jeu)
{
    temps_ecoule=jeu->Clock.getElapsedTime().asSeconds();
    jeu->m_display=true;
    jeu->Clock.restart();

    moteurGraphique->Gerer(0);
    jeu->map->GererAmbiance(temps_ecoule);

    //jeu->map->Animer(&jeu->hero,0);
    jeu->map->Afficher(jeu->m_listHeroes,0,jeu->m_jeu->alpha_map);
    jeu->hero.AfficherAmisEtCraft();
   // jeu->menu.Afficher(2,jeu->m_jeu->alpha_map,&jeu->hero.m_classe);->
    if(configuration->multi)
    {
        jeu->m_jeu->IA(jeu);
        jeu->m_jeu->Animation(jeu);
    }

    if (m_afficher)
        m_decalage+=temps_ecoule*2000;
    else
        m_decalage-=temps_ecoule*2000;

    if (m_decalage>0)
        m_decalage=0;
    if (m_decalage<-600)
    {
        m_decalage=-600;
        jeu->Next();
        m_afficher = 1;
    }

    jeu->menu.AfficherInventaire(m_decalage,&jeu->hero.m_classe,m_trader==NULL);

    if(m_trader!=NULL)
    {
        jeu->hero.AfficherInventaire(m_decalage,m_trader, false);
    }
    else
    {
        std::vector<Objet> temp;
        jeu->hero.AfficherInventaire(m_decalage,&temp,false);
    }

    if(jeu->hero.m_objetEnMain == -1)
        eventManager->AfficherCurseur();

    if (eventManager->getEvenement(Mouse::Left,EventClic))
    {
        if (jeu->hero.m_objetEnMain==-1
          &&jeu->map->getObjetPointe()==-1
          &&eventManager->getPositionSouris().x > AutoScreenAdjust(jeu->hero.m_classe.position_sac_inventaire.x,0).x
          &&eventManager->getPositionSouris().x < AutoScreenAdjust(jeu->hero.m_classe.position_sac_inventaire.x + jeu->hero.m_classe.position_sac_inventaire.w,0).x
          &&eventManager->getPositionSouris().y > AutoScreenAdjust(0,jeu->hero.m_classe.position_sac_inventaire.y).y
          &&eventManager->getPositionSouris().y < AutoScreenAdjust(0,jeu->hero.m_classe.position_sac_inventaire.y + 20).y)
            jeu->map->m_defilerObjets--,eventManager->StopEvenement(Mouse::Left,EventClic);

        if (jeu->hero.m_objetEnMain==-1
          &&jeu->map->getObjetPointe()==-1
          &&eventManager->getPositionSouris().x > AutoScreenAdjust(jeu->hero.m_classe.position_sac_inventaire.x,0).x
          &&eventManager->getPositionSouris().x < AutoScreenAdjust(jeu->hero.m_classe.position_sac_inventaire.x + jeu->hero.m_classe.position_sac_inventaire.w,0).x
          &&eventManager->getPositionSouris().y > AutoScreenAdjust(0,jeu->hero.m_classe.position_sac_inventaire.y + jeu->hero.m_classe.position_sac_inventaire.h * 20 - 20).y
          &&eventManager->getPositionSouris().y < AutoScreenAdjust(0,jeu->hero.m_classe.position_sac_inventaire.y + jeu->hero.m_classe.position_sac_inventaire.h * 20).y)
            jeu->map->m_defilerObjets++,eventManager->StopEvenement(Mouse::Left,EventClic);
    }

    int temp = GestionBoutons(jeu);

    if(temp >= 0)
    {
        jeu->next_screen = temp;
        jeu->hero.m_defilement_trader=0;
        jeu->hero.m_max_defilement_trader=0;
        m_trader=NULL;
        jeu->hero.ChargerModele();
        net->SendSkin();
        jeu->hero.RecalculerCaracteristiques(true);
        m_afficher=0;
        jeu->Clock.restart();
        eventManager->StopEvenement(Keyboard::I,EventKey);

        /*if (jeu->hero.m_objetEnMain>=0)
        {
            jeu->hero.m_objetADeposer=jeu->hero.m_objetEnMain;
            jeu->map->AjouterObjet(jeu->hero.DeposerObjet());
        }*/

        if(jeu->next_screen == 2 || jeu->next_screen == 4)
            jeu->next_screen = 3;
    }

    if (eventManager->getEvenement(Mouse::Left,EventClic))
    {
        if (jeu->map->RamasserObjet(&jeu->hero,1))
            eventManager->StopEvenement(Mouse::Left,EventClic);
    }




    for(unsigned i = 0 ; i < jeu->hero.m_classe.boutons_menus_weapons.size() ; ++i)
    {
        Bouton *bouton;

        if(jeu->hero.m_weaponsSet == jeu->hero.m_classe.boutons_menus_weapons[i].lien)
            bouton = &jeu->hero.m_classe.boutons_menus_weapons_2[i];
        else
            bouton = &jeu->hero.m_classe.boutons_menus_weapons[i];

        MySprite sprite;

        sprite.setTexture(*moteurGraphique->getImage(bouton->image.image));
        sprite.setTextureRect(sf::IntRect(bouton->image.position.x,
                                      bouton->image.position.y,
                                      bouton->image.position.w,
                                      bouton->image.position.h));

        sprite.setPosition(AutoScreenAdjust(bouton->position.x,
                                            bouton->position.y-m_decalage));

        sprite.resize(bouton->position.w, bouton->position.h);

        moteurGraphique->AjouterCommande(&sprite, 17,0);

        if(bouton->Survol())
        {
            moteurGraphique->AjouterTexte(bouton->nom,coordonnee(eventManager->getPositionSouris().x,
                                                       eventManager->getPositionSouris().y - 20),
                                            jeu->hero.m_classe.border,
                                            19,0,12,sf::Color(224,224,224));
            if(eventManager->getEvenement(Mouse::Left,EventClic))
            {
                eventManager->StopEvenement(Mouse::Left,EventClic);

                jeu->hero.m_weaponsSet = jeu->hero.m_classe.boutons_menus_weapons[i].lien;

                jeu->hero.RecalculerCaracteristiques(true);
            }
        }
    }

    if (eventManager->getEvenement(Mouse::Left,EventClic)
     && !jeu->hero.m_classe.sort_inventory.m_hover)
    {
        if (jeu->hero.PrendreEnMain(m_trader))
            if (jeu->hero.m_objetADeposer>=0)
                jeu->map->AjouterObjet(jeu->hero.DeposerObjet());
        eventManager->StopEvenement(Mouse::Left,EventClic);
    }

    if (eventManager->getEvenement(Mouse::Right,EventClic))
    {
        jeu->hero.UtiliserObjet(jeu->hero.m_objetVise);
        eventManager->StopEvenement(Mouse::Right,EventClic);
    }


    jeu->hero.m_defilement_trader -= eventManager->getMolette();

    if(eventManager->getEvenement(Mouse::Left,EventClicA))
    if(jeu->hero.m_max_defilement_trader-jeu->hero.m_classe.position_contenu_marchand.h != 0)
      if (eventManager->getPositionSouris().x > AutoScreenAdjust(jeu->hero.m_classe.scroll_button.position.x,0).x
        &&eventManager->getPositionSouris().x < AutoScreenAdjust(jeu->hero.m_classe.scroll_button.position.x+jeu->hero.m_classe.scroll_button.position.w,0).x
        &&eventManager->getPositionSouris().y > AutoScreenAdjust(0,jeu->hero.m_classe.scroll_button.position.y).y
        &&eventManager->getPositionSouris().y < AutoScreenAdjust(0,jeu->hero.m_classe.scroll_button.position.y+jeu->hero.m_classe.position_contenu_marchand.h*32).y)
            jeu->hero.m_defilement_trader = (int)((((float)eventManager->getPositionSouris().y-jeu->hero.m_classe.scroll_button.position.h*0.5-(float)AutoScreenAdjust(0,jeu->hero.m_classe.scroll_button.position.y).y)/((float)jeu->hero.m_classe.position_contenu_marchand.h*28))*(float)(jeu->hero.m_max_defilement_trader-jeu->hero.m_classe.position_contenu_marchand.h));

    if (jeu->hero.m_defilement_trader<0)
        jeu->hero.m_defilement_trader=0;
    if (jeu->hero.m_defilement_trader>jeu->hero.m_max_defilement_trader-jeu->hero.m_classe.position_contenu_marchand.h)
        jeu->hero.m_defilement_trader=jeu->hero.m_max_defilement_trader-jeu->hero.m_classe.position_contenu_marchand.h;

    jeu->map->AfficherSac(jeu->hero.m_personnage.getCoordonnee(),m_decalage,jeu->hero.m_classe.position_sac_inventaire,jeu->hero.m_caracteristiques,jeu->hero.m_cheminClasse, jeu->hero.m_classe.border);

    jeu->menu.AfficherHUD(&jeu->hero.m_classe);
    jeu->menu.AfficherDynamique(jeu->hero.m_caracteristiques,-1,jeu->hero.m_caracteristiques,&jeu->hero.m_classe);

    coordonnee position;
    position.x=(jeu->hero.m_personnage.getCoordonnee().x-jeu->hero.m_personnage.getCoordonnee().y-1)/5;
    position.y=(jeu->hero.m_personnage.getCoordonnee().x+jeu->hero.m_personnage.getCoordonnee().y)/5;

    Listener::setGlobalVolume((float)configuration->volume);
    Listener::setPosition(-position.x, 0, position.y);
    Listener::setDirection(0, 0, 1);
    jeu->map->MusiquePlay();
    jeu->sonMort.setPosition(position.x,0,position.y);
}
