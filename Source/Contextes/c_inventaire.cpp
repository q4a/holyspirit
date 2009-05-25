

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

c_Inventaire::c_Inventaire()
{
    m_decalage=-600;
}

void c_Inventaire::setTrader(std::vector<Objet> trade,Classe *classe)
{
    for (int i=0;i<(int)trade.size();i++)
        AjouterTrader(trade[i],m_trader,classe);
}

void AjouterTrader(Objet newObj,std::vector<Objet>& trader,Classe *classe)
{
    bool continuer=true;
    trader.push_back(newObj);

    for (int y=0;continuer;y++)
        for (int x=0;x<classe->position_contenu_marchand.w&&continuer;x++)
        {
            bool ajouter=true;
            for (int h=0;h<trader.back().getTaille().y;h++)
                for (int w=0;w<trader.back().getTaille().x;w++)
                    if (x+w<classe->position_contenu_marchand.w)
                    {
                        for (int j=0;j<(int)trader.size()-1;j++)
                            for (int Y=0;Y<trader[j].getTaille().y;Y++)
                                for (int X=0;X<trader[j].getTaille().x;X++)
                                    if (trader[j].getPosition().x+X==x+w && trader[j].getPosition().y+Y==y+h)
                                        ajouter=false;
                    }
                    else
                        ajouter=false;

            if (ajouter)
            {
                continuer=false;
                trader.back().setPosition(x,y);
            }
        }
}

void c_Inventaire::Utiliser(Jeu *jeu)
{

    if (m_decalage<=-600)
        m_afficher=1;

    temps_ecoule=jeu->Clock.GetElapsedTime();
    jeu->m_display=true;
    jeu->Clock.Reset();

    if (jeu->hero.m_objetEnMain==-1)
        eventManager->AfficherCurseur();

    if (eventManager->getEvenement(Key::I,"ET")||eventManager->getEvenement(Key::Escape,"ET"))
    {
        jeu->hero.m_defilement_trader=0;
        jeu->hero.m_max_defilement_trader=0;
        m_trader.clear();
        jeu->hero.ChargerModele();
        m_afficher=0;
        jeu->Clock.Reset();
        eventManager->StopEvenement(Key::I,"ET");

        if (jeu->hero.m_objetEnMain>=0)
        {
            jeu->hero.m_objetADeposer=jeu->hero.m_objetEnMain;
            jeu->map->AjouterObjet(jeu->hero.DeposerObjet());
        }
    }

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

    jeu->hero.PlacerCamera(jeu->map->getDimensions()); // On place la camera suivant ou se trouve le perso

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
        jeu->m_contexte=jeu->m_jeu,eventManager->StopEvenement(Key::I,"ET");
    }

    jeu->menu.AfficherInventaire(m_decalage,&jeu->hero.m_classe,m_trader.empty());

    jeu->hero.AfficherInventaire(m_decalage,m_trader);

    jeu->map->AfficherSac(jeu->hero.m_personnage.getCoordonnee(),m_decalage,jeu->hero.m_classe.position_sac_inventaire,jeu->hero.m_caracteristiques);

    jeu->menu.Afficher(1,255,&jeu->hero.m_classe);
    jeu->menu.AfficherDynamique(jeu->hero.m_caracteristiques,-1,jeu->hero.m_caracteristiques,&jeu->hero.m_classe);
}
