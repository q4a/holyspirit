

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

int GestionBoutons(Jeu *jeu);

c_Inventaire::c_Inventaire()
{
    m_decalage=-600;
    m_trader=NULL;
}

void TrierInventaire(std::vector<Objet> *trade, int largeur)
{
    bool continuer=true;

    for(unsigned i=0;i<trade->size();++i)
    if(!(*trade)[i].m_dejaTrie)
    {
        continuer = true;
        for (int y=0;continuer;y++)
            for (int x=0;x<largeur&continuer;x++)
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
                }
            }
    }
}

void c_Inventaire::setTrader(std::vector<Objet> *trade,Classe *classe)
{
    m_trader = trade;
}



void c_Inventaire::Utiliser(Jeu *jeu)
{
    if (m_decalage<=-600)
        m_afficher=1;

    temps_ecoule=jeu->Clock.GetElapsedTime();
    jeu->m_display=true;
    jeu->Clock.Reset();

    jeu->map->Afficher(&jeu->hero,0,jeu->m_jeu->alpha_map);
    jeu->menu.Afficher(2,jeu->m_jeu->alpha_map,&jeu->hero.m_classe);

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
    }

    jeu->menu.AfficherInventaire(m_decalage,&jeu->hero.m_classe,m_trader==NULL);

    if(m_trader!=NULL)
    {
        jeu->hero.AfficherInventaire(m_decalage,*m_trader);
    }
    else
    {
        std::vector<Objet> temp;
        jeu->hero.AfficherInventaire(m_decalage,temp);
    }

    if(jeu->hero.m_objetEnMain == -1)
        eventManager->AfficherCurseur();


    jeu->map->AfficherSac(jeu->hero.m_personnage.getCoordonnee(),m_decalage,jeu->hero.m_classe.position_sac_inventaire,jeu->hero.m_caracteristiques);

    jeu->menu.Afficher(1,255,&jeu->hero.m_classe);
    jeu->menu.AfficherDynamique(jeu->hero.m_caracteristiques,-1,jeu->hero.m_caracteristiques,&jeu->hero.m_classe);

    if (eventManager->getEvenement(Mouse::Left,"C"))
    {
        if (jeu->hero.m_objetEnMain==-1&&jeu->map->getObjetPointe()==-1&&eventManager->getPositionSouris().x>jeu->hero.m_classe.position_sac_inventaire.x*configuration->Resolution.x/800&&eventManager->getPositionSouris().x<(jeu->hero.m_classe.position_sac_inventaire.x+jeu->hero.m_classe.position_sac_inventaire.w)*configuration->Resolution.x/800&&eventManager->getPositionSouris().y>jeu->hero.m_classe.position_sac_inventaire.y*configuration->Resolution.y/600&&eventManager->getPositionSouris().y<jeu->hero.m_classe.position_sac_inventaire.y*configuration->Resolution.y/600+20*configuration->Resolution.x/800)
            jeu->map->m_defilerObjets--,eventManager->StopEvenement(Mouse::Left,"C");

        if (jeu->hero.m_objetEnMain==-1&&jeu->map->getObjetPointe()==-1&&eventManager->getPositionSouris().x>jeu->hero.m_classe.position_sac_inventaire.x*configuration->Resolution.x/800&&eventManager->getPositionSouris().x<(jeu->hero.m_classe.position_sac_inventaire.x+jeu->hero.m_classe.position_sac_inventaire.w)*configuration->Resolution.x/800&&eventManager->getPositionSouris().y>jeu->hero.m_classe.position_sac_inventaire.y*configuration->Resolution.y/600+(jeu->hero.m_classe.position_sac_inventaire.h-1)*20*configuration->Resolution.x/800&&eventManager->getPositionSouris().y<jeu->hero.m_classe.position_sac_inventaire.y*configuration->Resolution.y/600+jeu->hero.m_classe.position_sac_inventaire.h*20*configuration->Resolution.x/800)
            jeu->map->m_defilerObjets++,eventManager->StopEvenement(Mouse::Left,"C");
    }

    if (eventManager->getEvenement(Mouse::Left,"C"))
    {
        if (jeu->map->RamasserObjet(&jeu->hero,1))
            eventManager->StopEvenement(Mouse::Left,"C");
    }

    if (eventManager->getEvenement(Mouse::Left,"C"))
    {
        if (jeu->hero.PrendreEnMain(m_trader))
            if (jeu->hero.m_objetADeposer>=0)
                jeu->map->AjouterObjet(jeu->hero.DeposerObjet());
        eventManager->StopEvenement(Mouse::Left,"C");
    }

    if (eventManager->getEvenement(Mouse::Right,"C"))
    {
        jeu->hero.UtiliserObjet(jeu->hero.m_objetVise);
        eventManager->StopEvenement(Mouse::Right,"C");
    }

    int temp = GestionBoutons(jeu);
    if(temp >= 0)
    {
        jeu->next_screen = temp;
        jeu->hero.m_defilement_trader=0;
        jeu->hero.m_max_defilement_trader=0;
        m_trader=NULL;
        jeu->hero.ChargerModele();
        m_afficher=0;
        jeu->Clock.Reset();
        eventManager->StopEvenement(Key::I,"ET");

        if (jeu->hero.m_objetEnMain>=0)
        {
            jeu->hero.m_objetADeposer=jeu->hero.m_objetEnMain;
            jeu->map->AjouterObjet(jeu->hero.DeposerObjet());
        }

        if(jeu->next_screen == 2 || jeu->next_screen == 4)
            jeu->next_screen = 3;
    }

    jeu->hero.m_defilement_trader -= eventManager->getMolette();

    if(eventManager->getEvenement(Mouse::Left,"CA"))
        if (eventManager->getPositionSouris().x>jeu->hero.m_classe.scroll_button.position.x*configuration->Resolution.x/800
        &&eventManager->getPositionSouris().x<(jeu->hero.m_classe.scroll_button.position.x+jeu->hero.m_classe.scroll_button.position.w)*configuration->Resolution.x/800
        &&eventManager->getPositionSouris().y>jeu->hero.m_classe.scroll_button.position.y*configuration->Resolution.h/600
        &&eventManager->getPositionSouris().y-jeu->hero.m_classe.scroll_button.position.h<(jeu->hero.m_classe.scroll_button.position.y+jeu->hero.m_classe.position_contenu_marchand.h*24)*configuration->Resolution.h/600)
            jeu->hero.m_defilement_trader = (int)((((float)eventManager->getPositionSouris().y-jeu->hero.m_classe.scroll_button.position.h*0.25-(float)jeu->hero.m_classe.scroll_button.position.y)/((float)jeu->hero.m_classe.position_contenu_marchand.h*24))*(float)(jeu->hero.m_max_defilement_trader-jeu->hero.m_classe.position_contenu_marchand.h));

    if (jeu->hero.m_defilement_trader<0)
        jeu->hero.m_defilement_trader=0;
    if (jeu->hero.m_defilement_trader>jeu->hero.m_max_defilement_trader-jeu->hero.m_classe.position_contenu_marchand.h)
        jeu->hero.m_defilement_trader=jeu->hero.m_max_defilement_trader-jeu->hero.m_classe.position_contenu_marchand.h;
}
