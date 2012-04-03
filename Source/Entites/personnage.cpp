

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

#ifdef _MSC_VER
#define _USE_MATH_DEFINES
#endif

#include "personnage.h"
#include "../globale.h"

#include <iostream>
#include <math.h>

#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>


using namespace std;
using namespace sf;

int calculerAngle(int x, int y)
{
    // Faudrait refaire plus proprement avec des fonctions trigonométrique, mais j'avais pas envie de réfléchir ^^'
    if (x>0)
    {
        if (y>0)
            return 270;
        else if (y<0)
            return 0;
        else
            return 315;
    }
    else if (x<0)
    {
        if (y>0)
            return 180;
        else if (y<0)
            return 90;
        else
            return 135;
    }
    else
    {
        if (y>0)
            return 225;
        else
            return 45;
    }
}

Personnage::Personnage()
{
    m_angle                             = 45;
    m_monstre                           = false;
    frappeEnCours                       = false;

    m_modele                            = -1;

    m_erreurPathfinding                 = false;

    m_caracteristique.pointAme          = 0;

    m_positionPixel.h                   = 0;
    m_cheminFinal.h                     = 0;
    m_positionCase.h                    = 0;

    m_nombreInvocation                  = 0;

    m_shooter                           = false;

    m_cible                             = NULL;

    m_angle                             = 0;
    m_next_angle                        = 0;
    m_time_next_angle                   = 0;

    m_pousse.x                          = 0;
    m_pousse.y                          = 0;
    m_pousse.w                          = 0;

    m_miracleALancer                    = -1;
    m_miracleFrappeEnCours              = false;

    m_positionPixel.h                   = 0;
    m_cheminFinal.h                     = 0;
    m_positionCase.h                    = 0;

    m_caracteristique.rang              = 0;

    m_impenetrable                      = 0;
    m_impoussable                       = 0;

    m_noDistanceRestriction             = 0;
    m_doitMourir                        = 0;
    m_collision                         = 1;

    m_vientDeFrapper                    = NULL;
    m_vientDetreTouche                  = NULL;
    m_vientDeToucher                    = false;
    m_vientDAttaquer.x                  = -1;

    m_stunned                           = false;
    m_ID                                = 0;
    m_etatForce                         = false;
    m_miracleBloquant                   = false;

    m_actif                             = true;

    m_entite_graphique.option_sonUnique     = false;
    m_entite_graphique.option_forcedLight   = true;

    m_pousseEnCours = false;

    m_inexistant = false;
    m_selectable = true;

    m_speak_time = 0;

    m_materiau = M_VAPOREUX;

    m_heroic = false;

    m_no = -1;

    m_clock.restart();

    m_positionPixelPrev.x = -1;
    m_positionPixelPrev.y = -1;
    m_deplacement_time = 0;
    m_cur_d_time = 0;

    dejaMort = false;
    notyetemulate = true;
}
Modele_Personnage::Modele_Personnage()
{
    m_ombre=1;

    m_caracteristique.vie                = 1;
    m_caracteristique.maxVie             = 1;
    m_caracteristique.reserveVie         = 0;
    m_caracteristique.regenVie           = 0;

    m_caracteristique.foi                = 1;
    m_caracteristique.maxFoi             = 1;
    m_caracteristique.reserveFoi         = 0;
    m_caracteristique.regenFoi           = 0;

    m_caracteristique.volVie             = 0;
    m_caracteristique.volFoi             = 0;

    m_caracteristique.vitesse            = 1;
    m_caracteristique.pointAme           = 0;
    m_caracteristique.ancienPointAme     = 0;
    m_caracteristique.positionAncienAme  = 0;
    m_caracteristique.niveau             = 1;
    m_caracteristique.nom                = "Entitie";

    m_caracteristique.force              = 0;
    m_caracteristique.dexterite          = 0;
    m_caracteristique.vitalite           = 0;
    m_caracteristique.piete              = 0;
    m_caracteristique.charisme           = 0;
    m_caracteristique.pts_restant        = 0;
    m_caracteristique.miracles_restant   = 0;

    m_caracteristique.modificateurTaille = 1;

    m_caracteristique.degatsMin      [0] = 0;
    m_caracteristique.degatsMax      [0] = 0;
    m_caracteristique.armure         [0] = 0;
    m_caracteristique.degatsMin      [1] = 0;
    m_caracteristique.degatsMax      [1] = 0;
    m_caracteristique.armure         [1] = 0;
    m_caracteristique.degatsMin      [2] = 0;
    m_caracteristique.degatsMax      [2] = 0;
    m_caracteristique.armure         [2] = 0;
    m_caracteristique.degatsMin      [3] = 0;
    m_caracteristique.degatsMax      [3] = 0;
    m_caracteristique.armure         [3] = 0;

    m_impenetrable                       = 0;
    m_impoussable                        = 0;
    m_noDistanceRestriction              = 0;
    m_selectable                         = 1;

    m_materiau = M_VAPOREUX;
}

Personnage::~Personnage()
{
}

void Modele_Personnage::Reinitialiser()
{
    m_tileset.clear();
}

Modele_Personnage::~Modele_Personnage()
{
    m_tileset.clear();
}


bool Modele_Personnage::Charger(string chemin)
{
    m_chemin = chemin;

    m_tileset.clear();

    console->Ajouter("",0);
    console->Ajouter("Loading character : "+chemin,0);

    cDAT reader;

    if(reader.Read(chemin))
    {
        ifstream *fichier   = NULL;
        fichier             = reader.GetInfos("infos.txt");
        if (fichier)
        {
            char caractere;

            m_tileset.push_back(std::vector <Tileset> (8, Tileset ()));
            //m_tileset.back().front().option_forcedShadow    = m_ombre;
            m_tileset.back().front().option_forcedReflect   = true;
            m_tileset.back().front().Charger(*fichier, -1, &reader);

            for (int j=1;j<8;j++)
            {
                m_tileset.back()[j] = m_tileset.back()[0];
                m_tileset.back()[j].DeleteTiles();
                m_tileset.back()[j].ChargerTiles(*fichier, -1/*, m_porteeLumineuse*/);
            }

            int etat=-1;
            caractere=' ';

            do
            {
                fichier->get(caractere);

                if (caractere=='*')
                {
                    fichier->putback(caractere);

                    m_tileset.push_back(m_tileset.front());
                    for (int j=0;j<8;j++)
                    {
                        m_tileset.back()[j].DeleteTiles();
                        m_tileset.back()[j].ChargerTiles(*fichier, -1/*, m_porteeLumineuse*/);
                    }

                    etat++;
                    fichier->get(caractere);
                }
            }
            while (caractere!='$');

            fichier->close();
        }
        else
        {
            console->Ajouter("Unable to open : "+chemin,1);
            return 0;
        }

        if(fichier != NULL)
            delete fichier;
    }
    else
    {
        console->Ajouter("Unable to open : "+chemin,1);
        return 0;
    }

    return true;
}

void Personnage::Sauvegarder(ofstream &fichier)
{
    for(unsigned i = 0; i < m_effets.size(); ++i)
    {
        if(m_effets[i].m_effet.m_actif)
        {
            if(m_effets[i].m_type == AURA_CARACTERISTIQUES)
            {
                if(m_effets[i].m_info1 == 0)
                {
                    m_caracteristique.maxVie -= m_effets[i].m_info2;
                    m_caracteristique.vie -= m_effets[i].m_info2;
                }
                if(m_effets[i].m_info1 == 1)
                {
                    if(m_effets[i].m_info2 == 0)
                        m_caracteristique.force -= m_effets[i].m_info3;
                    if(m_effets[i].m_info2 == 1)
                        m_caracteristique.dexterite -= m_effets[i].m_info3;
                    if(m_effets[i].m_info2 == 2)
                        m_caracteristique.vitalite -= m_effets[i].m_info3;
                    if(m_effets[i].m_info2 == 3)
                        m_caracteristique.piete -= m_effets[i].m_info3;
                    if(m_effets[i].m_info2 == 4)
                        m_caracteristique.charisme -= m_effets[i].m_info3;
                }
            }
        }
    }

    fichier <<"* m" <<m_modele
            <<" vi" <<m_caracteristique.vie
            <<" va" <<m_caracteristique.maxVie
            <<" di" <<m_caracteristique.degatsMin[PHYSIQUE]
            <<" da" <<m_caracteristique.degatsMax[PHYSIQUE]
            <<" r"  <<m_caracteristique.rang
            <<" a"  <<m_caracteristique.pointAme
            <<" t"  <<m_caracteristique.modificateurTaille
            <<" i"  <<m_entite_graphique.m_noAnimation
            <<" e"  <<m_etat
            <<" f"  <<m_etatForce
            <<" g"  <<m_angle;

    m_entite_graphique.SaveParameters(fichier);

    for(int z = 0 ; z < m_scriptAI.getNbrVariable() ; ++z)
        fichier<<" s "<<z<<" "<<m_scriptAI.getVariable(z)<<" ";

    for (unsigned o=0;o < m_objets.size();o++)
            m_objets[o].SauvegarderTexte(&fichier);

    fichier<<" $\n";

    for(unsigned i = 0; i < m_effets.size(); ++i)
    {
        if(m_effets[i].m_effet.m_actif)
        {
            if(m_effets[i].m_type == AURA_CARACTERISTIQUES)
            {
                if(m_effets[i].m_info1 == 0)
                {
                    m_caracteristique.maxVie += m_effets[i].m_info2;
                    m_caracteristique.vie    += m_effets[i].m_info2;
                }
                if(m_effets[i].m_info1 == 1)
                {
                    if(m_effets[i].m_info2 == 0)
                        m_caracteristique.force += m_effets[i].m_info3;
                    if(m_effets[i].m_info2 == 1)
                        m_caracteristique.dexterite += m_effets[i].m_info3;
                    if(m_effets[i].m_info2 == 2)
                        m_caracteristique.vitalite += m_effets[i].m_info3;
                    if(m_effets[i].m_info2 == 3)
                        m_caracteristique.piete += m_effets[i].m_info3;
                    if(m_effets[i].m_info2 == 4)
                        m_caracteristique.charisme += m_effets[i].m_info3;
                }
            }
        }
    }
}

int Personnage::getOrdre(Modele_Personnage *modele)
{
    if (modele->m_tileset.size()>0)
        if (m_etat>=0&&m_etat<NOMBRE_ETAT)
            if ((int)(m_angle/45)>=0&&(int)(m_angle/45)<(int)modele->m_tileset[m_etat].size())
                return modele->m_tileset[m_etat][(int)(m_angle/45)].getOrdreDuTile(m_entite_graphique.m_noAnimation);

    return -10;
}

void Personnage::Afficher(Modele_Personnage *modele, Border &border,bool surbrillance, bool sansEffetHaut, bool sansEffetBas)
{
    sf::Color effect_color(255,255,255);

    for(int i = 0; i < (int)m_effets.size(); ++i)
        if(m_effets[i].m_effet.m_actif)
        {
            if(!sansEffetBas)
            if(m_effets[i].m_effet.getOrdre() == -1)
            {
                m_effets[i].m_effet.m_position = m_positionPixel;
                m_effets[i].m_effet.Afficher();
            }

            if(m_effets[i].useColor)
                effect_color = m_effets[i].m_color;
        }

    if (modele!=NULL)
        if (m_etat>=0 && m_etat < (int)modele->m_tileset.size())
            if ((int)(m_angle/45) >= 0 && (int)(m_angle/45) < (int)modele->m_tileset[m_etat].size())
            {
                m_entite_graphique.m_tileset = &modele->m_tileset[m_etat][(int)(m_angle/45)];
                m_entite_graphique.setPosition(((m_positionPixel.x-m_positionPixel.y)*64/COTE_TILE) ,
                                               ((m_positionPixel.x+m_positionPixel.y)*32/COTE_TILE)+32 -m_positionPixel.h);

                m_entite_graphique.setColor(sf::Color (
                                            (m_entite_graphique.m_color.r * (float)effect_color.r / 255),
                                            (m_entite_graphique.m_color.r * (float)effect_color.g / 255),
                                            (m_entite_graphique.m_color.r * (float)effect_color.b / 255),
                                            (m_entite_graphique.m_color.r * (float)effect_color.a / 255)));

                moteurGraphique->AjouterEntiteGraphique(&m_entite_graphique);

                if(surbrillance)
                {
                    Entite_graphique temp = m_entite_graphique;
                    moteurGraphique->AjouterEntiteGraphique(&temp,sf::BlendAdd);
                }

                if(modele->m_ombre && m_entite_graphique.m_couche + m_entite_graphique.m_decalCouche >= 10)
                {
                    m_entite_graphique_shadow.setPosition(((m_positionPixel.x-m_positionPixel.y)*64/COTE_TILE) ,
                                                          ((m_positionPixel.x+m_positionPixel.y)*32/COTE_TILE)+32 -m_positionPixel.h);
                    moteurGraphique->AjouterEntiteGraphique(&m_entite_graphique_shadow);
                }
            }

    if(!sansEffetHaut)
        for(int i = 0; i < (int)m_effets.size(); ++i)
            if(m_effets[i].m_effet.m_actif)
                if(m_effets[i].m_effet.getOrdre() >= 0)
                {
                    m_effets[i].m_effet.m_position = m_positionPixel;
                    m_effets[i].m_effet.Afficher();
                }


    if(m_speak_time > 0)
    {
        int alpha = 255;
        if(m_speak_time < 2550)
            alpha = (int)m_speak_time/10;

        coordonnee pos;
        pos.x = (int)(((m_positionPixel.x-m_positionPixel.y)*64/COTE_TILE - GetViewRect(moteurGraphique->m_camera).left)/configuration->zoom);
        pos.y = (int)(((m_positionPixel.x+m_positionPixel.y)*32/COTE_TILE - m_positionPixel.h - 128 - GetViewRect(moteurGraphique->m_camera).top)/configuration->zoom);

        bool empty = m_speak.empty();

        if(empty)
            m_speak = gettext("Enter your message...");

        sf::Text text;
        text.setFont(moteurGraphique->m_font);
        text.setCharacterSize(12);
        text.setString(toUtf32(m_speak));
        pos.x -= (int)text.getGlobalBounds().width/2;

        moteurGraphique->AjouterTexte(m_speak, pos, border, 14, 0, 12, sf::Color(224,224,224,alpha));

        if(empty)
            m_speak.clear();
    }
}
void Personnage::regenererVie(float vie)
{
    m_caracteristique.vie+=vie;
    if (m_caracteristique.vie>m_caracteristique.maxVie)
        m_caracteristique.vie=m_caracteristique.maxVie;
}

int Personnage::Pathfinding(casePathfinding** map, bool noDelete)
{
    //if(!(m_arrivee.x==m_mauvaiseArrivee.x&&m_arrivee.y==m_mauvaiseArrivee.y))
    if (!(m_arrivee.x==m_positionCase.x&&m_arrivee.y==m_positionCase.y))
    {
        bool retest = false;
        m_erreurPathfinding=false;
        liste_case casesVisitee;
        coordonnee depart,arrivee,decalage;

        decalage.x=m_positionCase.x-5;
        decalage.y=m_positionCase.y-5;

        depart.x=m_positionCase.x-decalage.x;
        depart.y=m_positionCase.y-decalage.y;
        arrivee.x=m_arrivee.x-decalage.x;
        arrivee.y=m_arrivee.y-decalage.y;

        casesVisitee.setTailleListe(0);

        casesVisitee.AjouterCase(depart, -1);

        if (arrivee.y < 0)
            arrivee.y = depart.y - 1;
        if (arrivee.x < 0)
            arrivee.x = depart.x - 1;
        if (arrivee.y >= 10)
            arrivee.y = depart.y + 1;
        if (arrivee.x >= 10)
            arrivee.x = depart.x + 1;

        if (arrivee.y>=0&&arrivee.x>=0&&arrivee.y<10&&arrivee.x<10)
            if (map[arrivee.y][arrivee.x].collision)
            {
                coordonnee temp(-100,-100,-1,-1),enCours;

                enCours.y=arrivee.y-1;
                enCours.x=arrivee.x-1;
                if (enCours.y>=0&&enCours.y<10&&enCours.x>=0&&enCours.x<10)
                    if (!map[enCours.y][enCours.x].collision&&!(enCours.x==depart.x&&enCours.y==depart.y))
                        if (((enCours.y-depart.y)*(enCours.y-depart.y)+(enCours.x-depart.x)*(enCours.x-depart.x)) < ((temp.y-depart.y)*(temp.y-depart.y)+(temp.x-depart.x)*(temp.x-depart.x)))
                            temp.y=enCours.y,temp.x=enCours.x;
                enCours.y=arrivee.y;
                enCours.x=arrivee.x-1;
                if (enCours.y>=0&&enCours.y<10&&enCours.x>=0&&enCours.x<10)
                    if (!map[enCours.y][enCours.x].collision&&!(enCours.x==depart.x&&enCours.y==depart.y))
                        if (((enCours.y-depart.y)*(enCours.y-depart.y)+(enCours.x-depart.x)*(enCours.x-depart.x)) < ((temp.y-depart.y)*(temp.y-depart.y)+(temp.x-depart.x)*(temp.x-depart.x)))
                            temp.y=enCours.y,temp.x=enCours.x;
                enCours.y=arrivee.y-1;
                enCours.x=arrivee.x;
                if (enCours.y>=0&&enCours.y<10&&enCours.x>=0&&enCours.x<10)
                    if (!map[enCours.y][enCours.x].collision&&!(enCours.x==depart.x&&enCours.y==depart.y))
                        if (((enCours.y-depart.y)*(enCours.y-depart.y)+(enCours.x-depart.x)*(enCours.x-depart.x)) < ((temp.y-depart.y)*(temp.y-depart.y)+(temp.x-depart.x)*(temp.x-depart.x)))
                            temp.y=enCours.y,temp.x=enCours.x;

                enCours.y=arrivee.y+1;
                enCours.x=arrivee.x+1;
                if (enCours.y>=0&&enCours.y<10&&enCours.x>=0&&enCours.x<10)
                    if (!map[enCours.y][enCours.x].collision&&!(enCours.x==depart.x&&enCours.y==depart.y))
                        if (((enCours.y-depart.y)*(enCours.y-depart.y)+(enCours.x-depart.x)*(enCours.x-depart.x)) < ((temp.y-depart.y)*(temp.y-depart.y)+(temp.x-depart.x)*(temp.x-depart.x)))
                            temp.y=enCours.y,temp.x=enCours.x;
                enCours.y=arrivee.y+1;
                enCours.x=arrivee.x;
                if (enCours.y>=0&&enCours.y<10&&enCours.x>=0&&enCours.x<10)
                    if (!map[enCours.y][enCours.x].collision&&!(enCours.x==depart.x&&enCours.y==depart.y))
                        if (((enCours.y-depart.y)*(enCours.y-depart.y)+(enCours.x-depart.x)*(enCours.x-depart.x)) < ((temp.y-depart.y)*(temp.y-depart.y)+(temp.x-depart.x)*(temp.x-depart.x)))
                            temp.y=enCours.y,temp.x=enCours.x;
                enCours.y=arrivee.y;
                enCours.x=arrivee.x+1;
                if (enCours.y>=0&&enCours.y<10&&enCours.x>=0&&enCours.x<10)
                    if (!map[enCours.y][enCours.x].collision&&!(enCours.x==depart.x&&enCours.y==depart.y))
                        if (((enCours.y-depart.y)*(enCours.y-depart.y)+(enCours.x-depart.x)*(enCours.x-depart.x)) < ((temp.y-depart.y)*(temp.y-depart.y)+(temp.x-depart.x)*(temp.x-depart.x)))
                            temp.y=enCours.y,temp.x=enCours.x;


                enCours.y=arrivee.y+1;
                enCours.x=arrivee.x-1;
                if (enCours.y>=0&&enCours.y<10&&enCours.x>=0&&enCours.x<10)
                    if (!map[enCours.y][enCours.x].collision&&!(enCours.x==depart.x&&enCours.y==depart.y))
                        if (((enCours.y-depart.y)*(enCours.y-depart.y)+(enCours.x-depart.x)*(enCours.x-depart.x)) < ((temp.y-depart.y)*(temp.y-depart.y)+(temp.x-depart.x)*(temp.x-depart.x)))
                            temp.y=enCours.y,temp.x=enCours.x;
                enCours.y=arrivee.y-1;
                enCours.x=arrivee.x+1;
                if (enCours.y>=0&&enCours.y<10&&enCours.x>=0&&enCours.x<10)
                    if (!map[enCours.y][enCours.x].collision&&!(enCours.x==depart.x&&enCours.y==depart.y))
                        if (((enCours.y-depart.y)*(enCours.y-depart.y)+(enCours.x-depart.x)*(enCours.x-depart.x)) < ((temp.y-depart.y)*(temp.y-depart.y)+(temp.x-depart.x)*(temp.x-depart.x)))
                            temp.y=enCours.y,temp.x=enCours.x;

                if (temp.y!=-100&&temp.x!=-100)
                    arrivee=temp, m_arrivee.x = temp.x + decalage.x,  m_arrivee.y = temp.y + decalage.y;
                else
                    m_erreurPathfinding=true;
            }

        casesVisitee.IncrementerDistanceEnCours();
        while (!casesVisitee.AjouterCasesAdjacentes(map,&arrivee)&&!m_erreurPathfinding)
        {
            casesVisitee.IncrementerDistanceEnCours();
            if (casesVisitee.getDistance() >= 10)
                m_erreurPathfinding=true, retest = true;
        }

        if (!m_erreurPathfinding)
        {
            m_cheminFinal=arrivee;

            Case temp = casesVisitee.getCase(casesVisitee.getTailleListe() - 1);

            if(temp.getParent() == -1)
                temp = casesVisitee.getCase(temp.getParent());
            else
                while (casesVisitee.getCase(temp.getParent()).getParent() != -1)
                    temp = casesVisitee.getCase(temp.getParent());

            m_cheminFinal = temp.getPosition();

            m_arrivee.x=arrivee.x+decalage.x;
            m_arrivee.y=arrivee.y+decalage.y;

            m_cheminFinal.h=map[m_cheminFinal.y][m_cheminFinal.x].hauteur;

            m_cheminFinal.x+=decalage.x,m_cheminFinal.y+=decalage.y;

            m_ancienneArrivee=m_arrivee;
        }
        else
        {
            if(!noDelete && retest)
            {
                if(m_arrivee.x > m_positionCase.x)
                    m_arrivee.x = m_positionCase.x + 1;
                else if(m_arrivee.x < m_positionCase.x )
                    m_arrivee.x = m_positionCase.x - 1;
                else
                    m_arrivee.x = m_positionCase.x;

                if(m_arrivee.y > m_positionCase.y)
                    m_arrivee.y = m_positionCase.y + 1;
                else if(m_arrivee.y < m_positionCase.y)
                    m_arrivee.y = m_positionCase.y - 1;
                else
                    m_arrivee.y = m_positionCase.y;

                for(int i = 0 ; i < 10 ; ++i)
                for(int j = 0 ; j < 10 ; ++j)
                    map[i][j].valeur = -1, map[i][j].dist = 3, map[i][j].cases = -1;

                Pathfinding(map, true);
            }
            else
            {
                m_arrivee       = m_positionCase;
                m_cheminFinal   = m_positionCase;
                frappeEnCours=0;
            }
        }

        if(!noDelete)
        {
            for (int i=0;i<10;i++)
                delete[] map[i];
            delete[] map;
        }


        if (m_etat==0)
            return 0;
        else
            return 1;
    }

    if(!noDelete)
    {
        for (int i=0;i<10;i++)
            delete[] map[i];
        delete[] map;
    }


    return 2;
}


bool Personnage::SeDeplacer(float tempsEcoule)
{
    sf::Vector2f pos;
    pos.x=((m_positionPixel.x-m_positionPixel.y)*64/COTE_TILE) + 0.1;
    pos.y=((m_positionPixel.x+m_positionPixel.y)*64/COTE_TILE)+64+1 + 0.1;

    moteurGraphique->LightManager->SetPosition(m_entite_graphique.m_light,pos);

    m_time_next_angle += tempsEcoule;
    if(m_time_next_angle > 4)
    {
        int newAngle = m_next_angle;

        if(m_next_angle - m_angle > 180)
            newAngle -= 360;
        if(m_angle - m_next_angle > 180)
            newAngle += 360;

        m_time_next_angle -= 4;
        if(m_angle - newAngle >= 45 )
            m_angle -= 45;
        if(newAngle - m_angle >= 45 )
            m_angle += 45;

        if (m_angle>=360)
            m_angle-=360;
        if (m_angle<0)
            m_angle+=360;
    }

    if(m_pousseEnCours)
    {
        m_positionPixel.x += m_pousse.x * tempsEcoule * COTE_TILE * 0.05;
        m_positionPixel.y += m_pousse.y * tempsEcoule * COTE_TILE * 0.05;

        bool xgrand = (m_pousse.x > 0);
        bool ygrand = (m_pousse.y > 0);

        m_pousse.x -= tempsEcoule * (-1 + xgrand * 2) * 0.028;
        m_pousse.y -= tempsEcoule * (-1 + ygrand * 2) * 0.028;

        if(xgrand && m_pousse.x < 0)
            m_pousse.x = 0;
        else if(!xgrand && m_pousse.x > 0)
            m_pousse.x = 0;

        if(ygrand && m_pousse.y < 0)
            m_pousse.y = 0;
        else if(!ygrand && m_pousse.y > 0)
            m_pousse.y = 0;


        m_positionCase.x    = (int)((m_positionPixel.x+COTE_TILE*0.25)/COTE_TILE);
        m_positionCase.y    = (int)((m_positionPixel.y+COTE_TILE*0.25)/COTE_TILE);


        if(m_pousse.x == 0 && m_pousse.y == 0)
        {
            m_positionCase.x    = (int)((m_positionPixel.x + COTE_TILE * 0.5)/COTE_TILE);
            m_positionCase.y    = (int)((m_positionPixel.y + COTE_TILE * 0.5)/COTE_TILE);

            m_arrivee.x = m_positionCase.x;
            m_arrivee.y = m_positionCase.y;

            /*if(m_positionPixel.x < m_arrivee.x * COTE_TILE)
                m_positionCase.x --;
            else if(m_positionPixel.x > m_arrivee.x * COTE_TILE)
                m_positionCase.x ++;

            if(m_positionPixel.y < m_arrivee.y * COTE_TILE)
                m_positionCase.y --;
            else if(m_positionPixel.y > m_arrivee.y * COTE_TILE)
                m_positionCase.y ++;*/

            m_cheminFinal.x     = m_arrivee.x;
            m_cheminFinal.y     = m_arrivee.y;

            m_pousseEnCours = false;
        }


        //if(m_miracleALancer == -1)
        //    frappeEnCours = 0;

        return 0;
    }
    else if (m_caracteristique.vie > 0 && !m_stunned)
    {
        if (!frappeEnCours)
        {
            if (m_positionCase.y!=m_cheminFinal.y||m_positionCase.x!=m_cheminFinal.x)
            {
                if (m_etat!=1 && !m_etatForce)
                    m_etat=1,m_entite_graphique.m_noAnimation=0,m_entite_graphique.m_animation=0;

                m_positionPixelPrecedente.x=(int)m_positionPixel.x;
                m_positionPixelPrecedente.y=(int)m_positionPixel.y;

                if (m_positionCase.x<m_cheminFinal.x)
                {
                    if (m_positionCase.h!=m_cheminFinal.h && m_positionPixel.h!=m_cheminFinal.h)
                        m_positionPixel.h=m_positionCase.h*( COTE_TILE - fabs(m_positionPixel.x-m_positionCase.x*COTE_TILE))/COTE_TILE + m_cheminFinal.h*( fabs(m_positionPixel.x-m_positionCase.x*COTE_TILE))/COTE_TILE ;
                    if (m_positionCase.y>m_cheminFinal.y)
                        m_positionPixel.x+=(float)2*tempsEcoule*m_caracteristique.vitesse;
                    else
                        m_positionPixel.x+=(float)4*tempsEcoule*m_caracteristique.vitesse;
                }
                else if (m_positionCase.x>m_cheminFinal.x)
                {
                    if (m_positionCase.h!=m_cheminFinal.h && m_positionPixel.h!=m_cheminFinal.h)
                        m_positionPixel.h=m_positionCase.h*( COTE_TILE - fabs(m_positionPixel.x-m_positionCase.x*COTE_TILE))/COTE_TILE + m_cheminFinal.h*( fabs(m_positionPixel.x-m_positionCase.x*COTE_TILE))/COTE_TILE ;
                    if (m_positionCase.y<m_cheminFinal.y)
                        m_positionPixel.x-=(float)2*tempsEcoule*m_caracteristique.vitesse;
                    else
                        m_positionPixel.x-=(float)4*tempsEcoule*m_caracteristique.vitesse;
                }

                else
                {
                    if(m_positionPixel.x > m_positionCase.x * COTE_TILE)
                        m_positionPixel.x-=(float)4*tempsEcoule*m_caracteristique.vitesse;
                    if(m_positionPixel.x < m_positionCase.x * COTE_TILE)
                        m_positionPixel.x+=(float)4*tempsEcoule*m_caracteristique.vitesse;
                }

                if (m_positionCase.y<m_cheminFinal.y)
                {
                    if (m_positionCase.h!=m_cheminFinal.h && m_positionPixel.h!=m_cheminFinal.h)
                        m_positionPixel.h=m_positionCase.h*( COTE_TILE - fabs(m_positionPixel.y-m_positionCase.y*COTE_TILE))/COTE_TILE + m_cheminFinal.h*( fabs(m_positionPixel.y-m_positionCase.y*COTE_TILE))/COTE_TILE ;
                    if (m_positionCase.x>m_cheminFinal.x)
                        m_positionPixel.y+=(float)2*tempsEcoule*m_caracteristique.vitesse;
                    else
                        m_positionPixel.y+=(float)4*tempsEcoule*m_caracteristique.vitesse;
                }
                else if (m_positionCase.y>m_cheminFinal.y)
                {
                    if (m_positionCase.h!=m_cheminFinal.h && m_positionPixel.h!=m_cheminFinal.h)
                        m_positionPixel.h=m_positionCase.h*( COTE_TILE - fabs(m_positionPixel.y-m_positionCase.y*COTE_TILE))/COTE_TILE + m_cheminFinal.h*( fabs(m_positionPixel.y-m_positionCase.y*COTE_TILE))/COTE_TILE ;
                    if (m_positionCase.x<m_cheminFinal.x)
                        m_positionPixel.y-=(float)2*tempsEcoule*m_caracteristique.vitesse;
                    else
                        m_positionPixel.y-=(float)4*tempsEcoule*m_caracteristique.vitesse;
                }
                else
                {
                    if(m_positionPixel.y > m_positionCase.y * COTE_TILE)
                        m_positionPixel.y-=(float)4*tempsEcoule*m_caracteristique.vitesse;
                    if(m_positionPixel.y < m_positionCase.y * COTE_TILE)
                        m_positionPixel.y+=(float)4*tempsEcoule*m_caracteristique.vitesse;
                }

                m_next_angle = calculerAngle(m_cheminFinal.x-m_positionCase.x,m_cheminFinal.y-m_positionCase.y);

                if ((m_positionCase.x<m_cheminFinal.x&&m_positionPixel.x>=m_cheminFinal.x*COTE_TILE)
                  ||(m_positionCase.x>m_cheminFinal.x&&m_positionPixel.x<=m_cheminFinal.x*COTE_TILE)
                  || m_positionCase.x==m_cheminFinal.x)
                    if ((m_positionCase.y<m_cheminFinal.y&&m_positionPixel.y>=m_cheminFinal.y*COTE_TILE)
                      ||(m_positionCase.y>m_cheminFinal.y&&m_positionPixel.y<=m_cheminFinal.y*COTE_TILE)
                      || m_positionCase.y==m_cheminFinal.y)
                    {
                       // m_positionPixel.x=(m_cheminFinal.x*COTE_TILE);
                       // m_positionPixel.y=(m_cheminFinal.y*COTE_TILE);

                        m_positionPixel.h=m_cheminFinal.h;

                        m_positionCase.y=m_cheminFinal.y;
                        m_positionCase.x=m_cheminFinal.x;
                        m_positionCase.h=m_cheminFinal.h;

                        return 1;
                    }
            }
            else if (m_arrivee.x!=m_positionCase.x||m_arrivee.y!=m_positionCase.y)
                return 1;
            else if(!m_etatForce)
            {
                if (m_etat!=0)
                    m_entite_graphique.m_noAnimation=0;

                if(m_caracteristique.vie > 0)
                    m_etat=0,frappeEnCours=0;
            }
        }
        if (m_etat==2||(m_arrivee.x==m_positionCase.x&&m_arrivee.y==m_positionCase.y))
            return 1;

        if(m_caracteristique.vitesse == 0)
            m_cheminFinal = m_positionCase;
    }
    else
        return 1;

    return 0;
}

void Personnage::EmulerDeplacement(float time)
{
    m_speak_time -= time * 1000;

    int totalTime = 0;
    for(unsigned i = 0 ; i < m_emul_pos.size() ; ++i)
        totalTime += m_emul_pos[i].maxTime;
    if(totalTime > 1)
    {
        m_positionPixel.x = m_emul_pos.back().nextPos.x;
        m_positionPixel.y = m_emul_pos.back().nextPos.y;
        m_positionPixel.h = m_emul_pos.back().nextPos.h;
        m_emul_pos.clear();
    }

    if(!m_emul_pos.empty())
    {
        m_emul_pos.front().time += time;
        if(m_emul_pos.size() >= 5)
            m_emul_pos.front().time += time;
        if(m_emul_pos.front().maxTime > 1)
            m_emul_pos.front().time = m_emul_pos.front().maxTime;

        while(!m_emul_pos.empty() && m_emul_pos.front().time > m_emul_pos.front().maxTime)
        {
            float buf = m_emul_pos.front().time - m_emul_pos.front().maxTime;

            m_positionPixel.x = m_emul_pos.front().nextPos.x;
            m_positionPixel.y = m_emul_pos.front().nextPos.y;
            m_positionPixel.h = m_emul_pos.front().nextPos.h;

            m_emul_pos.erase(m_emul_pos.begin());

            if(!m_emul_pos.empty())
                m_emul_pos.front().time += buf;
        }
    }

    if(!m_emul_pos.empty())
    {
        m_positionPixel.x = m_emul_pos.front().getCurPos().x;
        m_positionPixel.y = m_emul_pos.front().getCurPos().y;
        m_positionPixel.h = m_emul_pos.front().getCurPos().h;
    }

    m_positionCase.x = (int)((float)m_positionPixel.x/(float)COTE_TILE + 0.5);
    m_positionCase.y = (int)((float)m_positionPixel.y/(float)COTE_TILE + 0.5);

    m_cheminFinal = m_positionCase;

    /*if(m_deplacement_time != 0)
    {
        m_cur_d_time += time;

        m_positionPixel.x = (m_positionPixelNext.x - m_positionPixelPrev.x)*m_cur_d_time/m_deplacement_time;
        m_positionPixel.y = (m_positionPixelNext.y - m_positionPixelPrev.y)*m_cur_d_time/m_deplacement_time;
        m_positionPixel.h = (m_positionPixelNext.h - m_positionPixelPrev.h)*m_cur_d_time/m_deplacement_time;

        if(m_cur_d_time > m_deplacement_time)
        {
            m_positionPixel = m_positionPixelNext;
            m_positionPixelPrev = m_positionPixel;
            m_deplacement_time = 0;
            m_cur_d_time = 0;
        }

        m_positionCase.x = (int)((float)m_positionPixel.x/(float)COTE_TILE + 0.5);
        m_positionCase.y = (int)((float)m_positionPixel.y/(float)COTE_TILE + 0.5);
    }*/
}

void Personnage::InfligerDegats(float degats, int type, float temps)
{
    float temp = degats;
    if(type < 4 && type >= 0)
    {
        if(m_caracteristique.armure[type] >= 0)
            degats -= (float)m_caracteristique.armure[type]/25;
        else
            degats *= 1 - m_caracteristique.armure[type] / 100;
    }

    if (degats < 0)
        degats = 0;
    if (degats > temp)
        degats = temp;

    if(temps == 0)
        m_caracteristique.vie-=degats;
    else if(degats > 0)
        m_degats.push_back(Degats (temps, degats, type));

    m_cible = NULL;

    if(degats > 0 && temps == 0 && EnVie())
    if(m_entite_graphique.m_tileset != NULL)
        if(m_entite_graphique.m_tileset->getNombreSonsSpecial(0) > 0)
        {
            int random = rand()%m_entite_graphique.m_tileset->getNombreSonsSpecial(0);
            int nbr = 0;

            coordonnee position;
            position.x=(m_positionCase.x-m_positionCase.y-1)/5;
            position.y=(m_positionCase.x+m_positionCase.y)/5;

            while(!m_entite_graphique.m_tileset->JouerSon(m_entite_graphique.m_tileset->getSonSpecial(0, random++),position, true) && nbr++ < m_entite_graphique.m_tileset->getNombreSonsSpecial(0))
            {
                if(random >= m_entite_graphique.m_tileset->getNombreSonsSpecial(0))
                    random = 0;
            }
        }

    if (m_caracteristique.vie<=0&&m_etat!=3)
        m_entite_graphique.m_noAnimation=0,m_etat=3,m_entite_graphique.m_animation = 0;

    m_touche = true;
}

void Personnage::Kill()
{
    m_caracteristique.vie = 0;
    m_entite_graphique.m_noAnimation=0;
    m_entite_graphique.m_animation = 0;
    m_etat=3;
}

int Personnage::Gerer(Modele_Personnage *modele,float temps, bool no_generate)
{
    GererEffets(temps);
    GererDegats(temps);
    return Animer(modele, temps, no_generate);
}

void Personnage::GererEffets(float temps)
{
    int nombreInactif = 0;
    m_stunned = false;
    for(int i = 0; i < (int)m_effets.size(); ++i)
    {
        if(m_caracteristique.vie <= 0)
            m_effets[i].m_effet.m_actif = false;

        m_effets[i].m_effet.Animer(temps);

        sf::Vector2f pos;
        pos.x=((m_positionPixel.x-m_positionPixel.y)*64/COTE_TILE) + 0.1;
        pos.y=((m_positionPixel.x+m_positionPixel.y)*64/COTE_TILE)+64+1 + 0.1;

        if(m_effets[i].m_effet.m_light.ID() != -1)
            moteurGraphique->LightManager->SetPosition(m_effets[i].m_effet.m_light,pos);

        if(m_effets[i].m_effet.m_old_actif && !m_effets[i].m_effet.m_actif)
        {
            if(m_effets[i].m_type == AURA_CARACTERISTIQUES)
            {
                if(m_effets[i].m_info1 == 0)
                {
                    m_caracteristique.maxVie -= m_effets[i].m_info2;
                   // m_caracteristique.vie -= m_effets[i].m_info2;
                    if(m_caracteristique.vie > m_caracteristique.maxVie)
                        m_caracteristique.vie = m_caracteristique.maxVie;
                }
                if(m_effets[i].m_info1 == 1)
                {
                    if(m_effets[i].m_info2 == 0)
                        m_caracteristique.force -= m_effets[i].m_info3;
                    if(m_effets[i].m_info2 == 1)
                        m_caracteristique.dexterite -= m_effets[i].m_info3;
                    if(m_effets[i].m_info2 == 2)
                        m_caracteristique.vitalite -= m_effets[i].m_info3;
                    if(m_effets[i].m_info2 == 3)
                        m_caracteristique.piete -= m_effets[i].m_info3;
                    if(m_effets[i].m_info2 == 4)
                        m_caracteristique.charisme -= m_effets[i].m_info3;
                }
            }
            if(m_effets[i].m_type == AURA_ARMURE)
                if(m_effets[i].m_info1 >= 0 && m_effets[i].m_info1 < 4)
                    m_caracteristique.armure[(int)m_effets[i].m_info1] -= m_effets[i].m_info2;
        }

        if(!m_effets[i].m_effet.m_actif)
            ++nombreInactif;
        else if(!m_doitMourir)
        {
            float viePrecedente = m_caracteristique.vie;

            if(m_effets[i].m_type == AURA_REGENERATION)
            {
                if(m_effets[i].m_info3)
                {
                    if(m_effets[i].m_info1 == 0)
                        m_caracteristique.vie += m_effets[i].m_info2 * temps;
                    if(m_effets[i].m_info1 == 1)
                        m_caracteristique.foi += m_effets[i].m_info2 * temps;
                }
            }

            if(m_effets[i].m_type == AURA_STUNNED)
                m_stunned = true;

            if(m_monstre)
                if(m_caracteristique.vie > m_caracteristique.maxVie)
                    m_caracteristique.vie = m_caracteristique.maxVie;

            if(m_caracteristique.vie <= 0 && viePrecedente >= 0)
                m_caracteristique.vie = viePrecedente, m_doitMourir = true;
        }

        m_effets[i].m_effet.m_old_actif = m_effets[i].m_effet.m_actif;
    }

    if(nombreInactif == (int)m_effets.size() || !EnVie())
        m_effets.clear();
}

void Personnage::GererDegats(float temps)
{
    for(int i = 0; i < (int)m_degats.size() ; ++i)
    {
        if(!m_doitMourir)
        {
            float viePrecedente = m_caracteristique.vie;

            m_caracteristique.vie -= m_degats[i].degats * temps / m_degats[i].temps;
            m_degats[i].temps_ecoule += temps;

            if(m_degats[i].temps_ecoule > m_degats[i].temps)
                m_degats.erase(m_degats.begin() + i--);

            if(m_caracteristique.vie <= 0 && viePrecedente >= 0)
                m_caracteristique.vie = viePrecedente, m_doitMourir = true;
        }
    }
}

int Personnage::Animer(Modele_Personnage *modele,float temps, bool no_generate)
{
    int retour=-2;

    if(m_monstre)
        retour = 0;

    m_speak_time -= temps * 1000;

    if(m_speak_time <= 0)
        m_speak_time = 0;

    if(!m_stunned)
    if(m_etat >= 0 && m_etat < (int)modele->m_tileset.size())
    if((int)(m_angle/45) >= 0 && (int)(m_angle/45) < (int)modele->m_tileset[m_etat].size())
    {
        if( m_entite_graphique.m_tileset != &modele->m_tileset[m_etat][(int)(m_angle/45)])
        {
            m_entite_graphique.m_tileset    = &modele->m_tileset[m_etat][(int)(m_angle/45)];
            if(!no_generate)
            {
                m_entite_graphique.NextTile(true);
                m_entite_graphique.Generer();
            }
        }
        m_entite_graphique.m_couche     = 10;
        m_entite_graphique.option_sonUnique = false;
        if(temps > 0)
            m_entite_graphique.Animer(temps);

        m_entite_graphique.setScale(m_caracteristique.modificateurTaille,m_caracteristique.modificateurTaille);

        m_entite_graphique.scale((float)m_entite_graphique.m_scale.x*0.01, (float)m_entite_graphique.m_scale.y*0.01);
       /* if (m_porteeLumineuse.intensite>0)
        {
            m_entite_graphique.SetColor(sf::Color(m_porteeLumineuse.rouge,m_porteeLumineuse.vert,m_porteeLumineuse.bleu, 255));
           *//* moteurGraphique->LightManager->SetColor(m_entite_graphique.m_light,sf::Color(m_porteeLumineuse.rouge,m_porteeLumineuse.vert,m_porteeLumineuse.bleu));

            if (m_caracteristique.rang==0)
                m_porteeLumineuse.intensite=m_entite_graphique.m_tileset->getLumiereDuTile(m_entite_graphique.m_noAnimation).intensite;
            moteurGraphique->LightManager->SetIntensity(m_entite_graphique.m_light,m_porteeLumineuse.intensite);*/
        /*}
        else
            m_entite_graphique.SetColor(sf::Color(255,255,255, 255));*/

        if(m_entite_graphique.attaque_touche)
        {
            if(m_monstre)
            {
                if(m_miracleALancer >= 0)
                    retour = 1;
                else
                    retour+=(rand()%(m_caracteristique.degatsMax[PHYSIQUE]-m_caracteristique.degatsMin[PHYSIQUE]+1)+m_caracteristique.degatsMin[PHYSIQUE]);
            }
            else
                retour = 0;
        }

        if(m_entite_graphique.attaque_stop)
        {
            if(m_monstre)
                frappeEnCours=false, m_etatForce = false;
            else
                retour = 1;
            m_cible = NULL;
        }

        if(m_entite_graphique.attaque_pause)
            retour = 2;

        /*if (m_monstre && m_entite_graphique.m_tileset != NULL)
        {
            if (m_entite_graphique.m_tileset->getLumiereDuTile(m_entite_graphique.m_noAnimation).intensite!=-1&&m_caracteristique.rang==0)
                m_porteeLumineuse.intensite=m_entite_graphique.m_tileset->getLumiereDuTile(m_entite_graphique.m_noAnimation).intensite;

            float inte=m_porteeLumineuse.intensite;
            if (inte>255)
                inte=255;

           // moteurGraphique->LightManager->SetIntensity(m_entite_graphique.m_light,(int)inte);
           // moteurGraphique->LightManager->SetRadius(m_entite_graphique.m_light,(int)m_porteeLumineuse.intensite*2);
        }*/
    }

    int angleOmbre=(int)((m_angle-moteurGraphique->m_angleOmbreSoleil)+22.5);

    while (angleOmbre<0)
        angleOmbre=360+angleOmbre;
    while (angleOmbre>=360)
        angleOmbre=angleOmbre-360;

    if(!m_stunned && configuration->Ombre)
    if(m_etat >= 0 && m_etat < (int)modele->m_tileset.size())
    if((int)(angleOmbre/45) >= 0 && (int)(angleOmbre/45) < (int)modele->m_tileset[m_etat].size())
    {
        m_entite_graphique_shadow              = m_entite_graphique;

        m_entite_graphique_shadow.m_tileset    = &modele->m_tileset[m_etat][(int)(angleOmbre/45)];

        m_entite_graphique_shadow.Generer();

        m_entite_graphique_shadow.m_couche     = 9;
        m_entite_graphique_shadow.m_decalCouche= 0;

        m_entite_graphique_shadow.option_forcedLight    = false;
        m_entite_graphique_shadow.m_shadow              = false;
        m_entite_graphique_shadow.m_reflect             = false;
        m_entite_graphique_shadow.m_light               = Light_Entity();

        m_entite_graphique_shadow.setScale(m_caracteristique.modificateurTaille,
                                                    m_caracteristique.modificateurTaille*(100-(float)moteurGraphique->m_soleil.hauteur)/50);
        m_entite_graphique_shadow.setRotation(-moteurGraphique->m_angleOmbreSoleil);
    }

    return retour;
}

void Personnage::Frappe(const coordonnee &position,const coordonnee &direction)
{
    if (m_etat<2)
    {
        m_entite_graphique.m_animation = 0;
        m_etat=2;
        m_entite_graphique.m_noAnimation=0;
    }

    if(!frappeEnCours)
        m_entite_graphique.m_noAnimation=0, m_miracleFrappeEnCours = false;

    frappeEnCours=1;

    float m=atan2(-(double)(direction.y-position.y),(double)(direction.x-position.x));

    m = (int)(m*180/M_PI);

    if(m < 0)
        m += 360;
    if(m > 360)
        m -= 360;

    if(m - (int)(m/45) * 45 > 23)
        m = (int)(m/45) * 45 + 45;
    else
        m = (int)(m/45) * 45;

    m-=45;

    setAngle((int)m);

    m_cheminFinal=m_positionCase;
    m_arrivee=m_cheminFinal;
}

void Personnage::DetruireEffets()
{
    for(unsigned i = 0; i < m_effets.size(); ++i)
    {
        /*if(m_effets[i].m_effet.m_actif)
        {
            if(m_effets[i].m_type == AURA_CARACTERISTIQUES)
            {
                if(m_effets[i].m_info1 == 0)
                {
                    m_caracteristique.maxVie -= m_effets[i].m_info2;
                    m_caracteristique.vie -= m_effets[i].m_info2;
                    if(m_caracteristique.vie <= 0 && -m_caracteristique.vie <  m_effets[i].m_info2)
                        m_caracteristique.vie = 1;
                }
            }
            if(m_effets[i].m_type == AURA_ARMURE)
                if(m_effets[i].m_info1 >= 0 && m_effets[i].m_info1 < 4)
                    m_caracteristique.armure[(int)m_effets[i].m_info1] -= m_effets[i].m_info2;
        }*/
        m_effets[i].m_effet.m_actif = false;
    }
}

int Personnage::AjouterEffet(Tileset *tileset, int type, int compteur, int info1, int info2, int info3, int colorr, int colorg, int colorb, int colora)
{
    m_effets.push_back(EffetPersonnage());

    m_effets.back().m_effet.m_tileset = tileset;
    m_effets.back().m_effet.m_compteur  = compteur;
    m_effets.back().m_effet.m_position  = m_positionPixel;
    m_effets.back().m_effet.m_couche    = 10;
    m_effets.back().m_effet.Initialiser(coordonnee ((int)((m_positionPixel.x - m_positionPixel.y) * 64 / COTE_TILE),
                                                    (int)((m_positionPixel.x + m_positionPixel.y) * 32 / COTE_TILE)));

    m_effets.back().m_type              = type;
    m_effets.back().m_info1             = info1;
    m_effets.back().m_info2             = info2;
    m_effets.back().m_info3             = info3;

    m_effets.back().m_color             = sf::Color(colorr, colorg, colorb, colora);

    if(colorr != 0 || colorg != 0 || colorb != 0 || colora != 0)
        m_effets.back().useColor = true;




    if(m_effets.back().m_type == AURA_CARACTERISTIQUES)
    {
        if(m_effets.back().m_info1 == 0)
        {
            m_caracteristique.maxVie += m_effets.back().m_info2;
            m_caracteristique.vie += m_effets.back().m_info2;
        }
        if(m_effets.back().m_info1 == 1)
        {
            if(m_effets.back().m_info2 == 0)
                m_caracteristique.force += m_effets.back().m_info3;
            if(m_effets.back().m_info2 == 1)
                m_caracteristique.dexterite += m_effets.back().m_info3;
            if(m_effets.back().m_info2 == 2)
                m_caracteristique.vitalite += m_effets.back().m_info3;
            if(m_effets.back().m_info2 == 3)
                m_caracteristique.piete += m_effets.back().m_info3;
            if(m_effets.back().m_info2 == 4)
                m_caracteristique.charisme += m_effets.back().m_info3;
        }
    }

    if(m_effets.back().m_type == AURA_ARMURE)
        if(m_effets.back().m_info1 >= 0 && m_effets.back().m_info1 < 4)
            m_caracteristique.armure[(int)m_effets.back().m_info1] += m_effets.back().m_info2;

    return m_effets.size() - 1;
}

void Personnage::RecalculerEffets()
{
    for(unsigned i = 0 ; i < m_effets.size() ; ++i)
    if(m_effets[i].m_effet.m_actif || m_effets[i].m_effet.m_old_actif)
    {
        if(m_effets[i].m_type == AURA_CARACTERISTIQUES)
        {
            if(m_effets[i].m_info1 == 0)
            {
                m_caracteristique.maxVie += m_effets[i].m_info2;
                m_caracteristique.vie += m_effets[i].m_info2;
            }
            /*if(m_effets[i].m_info1 == 1)
            {
                if(m_effets[i].m_info2 == 0)
                    m_caracteristique.force += m_effets[i].m_info3;
                if(m_effets[i].m_info2 == 1)
                    m_caracteristique.dexterite += m_effets[i].m_info3;
                if(m_effets[i].m_info2 == 2)
                    m_caracteristique.vitalite += m_effets[i].m_info3;
                if(m_effets[i].m_info2 == 3)
                    m_caracteristique.piete += m_effets[i].m_info3;
                if(m_effets[i].m_info2 == 4)
                    m_caracteristique.charisme += m_effets[i].m_info3;
            }*/
        }

        if(m_effets[i].m_type == AURA_ARMURE)
            if(m_effets[i].m_info1 >= 0 && m_effets[i].m_info1 < 4)
                m_caracteristique.armure[(int)m_effets[i].m_info1] += m_effets[i].m_info2;
    }
}

void Personnage::DesactiverEffets()
{
    for(unsigned i = 0 ; i < m_effets.size() ; ++i)
    if(m_effets[i].m_effet.m_actif || m_effets[i].m_effet.m_old_actif)
    {
        if(m_effets[i].m_type == AURA_CARACTERISTIQUES)
        {
            if(m_effets[i].m_info1 == 0)
            {
                m_caracteristique.maxVie -= m_effets[i].m_info2;
                m_caracteristique.vie -= m_effets[i].m_info2;
            }
            if(m_effets[i].m_info1 == 1)
            {
                if(m_effets[i].m_info2 == 0)
                    m_caracteristique.force -= m_effets[i].m_info3;
                if(m_effets[i].m_info2 == 1)
                    m_caracteristique.dexterite -= m_effets[i].m_info3;
                if(m_effets[i].m_info2 == 2)
                    m_caracteristique.vitalite -= m_effets[i].m_info3;
                if(m_effets[i].m_info2 == 3)
                    m_caracteristique.piete -= m_effets[i].m_info3;
                if(m_effets[i].m_info2 == 4)
                    m_caracteristique.charisme -= m_effets[i].m_info3;
            }
        }

        if(m_effets[i].m_type == AURA_ARMURE)
            if(m_effets[i].m_info1 >= 0 && m_effets[i].m_info1 < 4)
                m_caracteristique.armure[(int)m_effets[i].m_info1] -= m_effets[i].m_info2;
    }
}

void Personnage::ActiverEffets()
{
    for(unsigned i = 0 ; i < m_effets.size() ; ++i)
    if(m_effets[i].m_effet.m_actif || m_effets[i].m_effet.m_old_actif)
    {
        if(m_effets[i].m_type == AURA_CARACTERISTIQUES)
        {
            if(m_effets[i].m_info1 == 0)
            {
                m_caracteristique.maxVie += m_effets[i].m_info2;
                m_caracteristique.vie += m_effets[i].m_info2;
            }
            if(m_effets[i].m_info1 == 1)
            {
                if(m_effets[i].m_info2 == 0)
                    m_caracteristique.force += m_effets[i].m_info3;
                if(m_effets[i].m_info2 == 1)
                    m_caracteristique.dexterite += m_effets[i].m_info3;
                if(m_effets[i].m_info2 == 2)
                    m_caracteristique.vitalite += m_effets[i].m_info3;
                if(m_effets[i].m_info2 == 3)
                    m_caracteristique.piete += m_effets[i].m_info3;
                if(m_effets[i].m_info2 == 4)
                    m_caracteristique.charisme += m_effets[i].m_info3;
            }
        }

        if(m_effets[i].m_type == AURA_ARMURE)
            if(m_effets[i].m_info1 >= 0 && m_effets[i].m_info1 < 4)
                m_caracteristique.armure[(int)m_effets[i].m_info1] += m_effets[i].m_info2;
    }
}

void Personnage::setCaracteristique(const Caracteristique &caracteristique)
{
    m_caracteristique=caracteristique;
}
void Personnage::setProchaineCase(const coordonnee &position)
{
    m_cheminFinal=position;
}
void Personnage::setVitesse(float vitesse)
{
    m_caracteristique.vitesse=vitesse;
}

void Personnage::setEtat(int etat, int pose)
{
    if(m_etat != etat)
        m_entite_graphique.m_animation = 0;

    if(pose != -1)
        m_entite_graphique.m_noAnimation = pose;

    m_etat=etat,frappeEnCours=false/*,m_entite_graphique.m_noAnimation=0*/;
}
void Personnage::setJustEtat(int etat)
{
    if(m_etat != etat)
        m_entite_graphique.m_animation = 0;

    m_etat=etat/*,m_poseEnCours=0*/;

    if(etat == 2)
        frappeEnCours = 1;

}
void Personnage::addAngle(int angle)
{
    m_next_angle += angle;
    if(m_next_angle < 0)
        m_next_angle += 360;
    if(m_next_angle >= 360)
        m_next_angle -= 360;
 //   m_next_angle = m_angle;
}


void Personnage::setPose(int pose)
{
    if(m_entite_graphique.m_tileset
    && m_entite_graphique.m_tileset->getTaille() > pose)
        m_entite_graphique.m_noAnimation=pose;
    else if(!m_entite_graphique.m_tileset)
        m_entite_graphique.m_noAnimation=pose;
}
void Personnage::setAngle(int angle)
{
    m_next_angle=angle;
    if (m_next_angle>=360)
        m_next_angle-=360;
    if (m_next_angle<0)
        m_next_angle+=360;
}
void Personnage::setForcedAngle(int angle)
{
    m_angle=angle;
    if (m_angle>=360)
        m_angle-=360;
    if (m_angle<0)
        m_angle+=360;
    m_next_angle = m_angle;
}
void Personnage::setErreurPathfinding(bool erreur)
{
    m_erreurPathfinding=erreur;
}
void Personnage::setCoordonnee(const coordonnee &nouvellesCoordonnees)
{
    m_positionCase=nouvellesCoordonnees;
    if(m_caracteristique.vie > 0)
        m_etat=ARRET;

    m_positionPixel.x=(float)nouvellesCoordonnees.x*COTE_TILE;
    m_positionPixel.y=(float)nouvellesCoordonnees.y*COTE_TILE;
    m_positionPixel.h=nouvellesCoordonnees.h;

    m_arrivee.x=m_positionCase.x;
    m_arrivee.y=m_positionCase.y;

    m_cheminFinal.x=m_positionCase.x;
    m_cheminFinal.y=m_positionCase.y;

    //m_angle=0;
    m_entite_graphique.m_noAnimation=0;

    //moteurGraphique->LightManager->SetPosition(m_light,sf::Vector2f(m_positionPixel.x,m_positionPixel.y));

    m_cible = NULL;
}
void Personnage::setArrivee(const coordonnee &arrivee)
{
    if (!(m_mauvaiseArrivee.x==arrivee.x
        &&m_mauvaiseArrivee.y==arrivee.y))
        m_arrivee=arrivee;
}
void Personnage::setMauvaiseArrivee(const coordonnee &arrivee)
{
    m_mauvaiseArrivee=arrivee;
}

void Personnage::setCoordonneePixel(const coordonnee &position)
{
    m_positionPixel.x=position.x*COTE_TILE;
    m_positionPixel.y=position.y*COTE_TILE;
}
void Personnage::setCoordonneePixel2(const coordonneeDecimal &position)
{
    m_positionPixel.x=position.x;
    m_positionPixel.y=position.y;
}

void Personnage::setEmulatePos(const coordonneeDecimal &positionD, float time)
{
    if(notyetemulate)
    {
        notyetemulate = false;
        m_positionPixel = positionD;
    }
    else
    {
        coordonneeDecimal prevPos = m_positionPixel;

        if(!m_emul_pos.empty())
            prevPos = m_emul_pos.back().nextPos;

        if(!(prevPos.x == positionD.x && prevPos.y == positionD.y))
        {
            m_emul_pos.push_back(Emulate_pos ());
            m_emul_pos.back().nextPos = positionD;
            m_emul_pos.back().prevPos = prevPos;
            m_emul_pos.back().time = 0;
            m_emul_pos.back().maxTime = time;
        }
    }



    /*if(m_positionPixelPrev.x == -1 && m_positionPixelPrev.y == -1)
    {
        m_positionPixel = positionD;
        m_positionPixelPrev = m_positionPixel;
    }

    m_positionPixelNext = positionD;

    m_deplacement_time += m_clock.GetElapsedTime()*0.001;
    //m_cur_d_time = 0;
    m_clock.Reset();*/
}

void Personnage::ClearEmulate()
{
    notyetemulate = true;
    m_emul_pos.clear();
}

void Personnage::setDepart()
{
    m_depart=m_positionCase;
}

void Personnage::setModele(int modele)
{
    m_modele = modele;
}


void Personnage::setPousse(const coordonneeDecimal &pousse)
{
    if(!m_impoussable)
    {
        if(pousse.x == 0 && pousse.y == 0)
            if(fabs(m_pousse.x) > 0 || fabs(m_pousse.y) > 0 )
            {
                    m_positionCase.x    = (int)((m_positionPixel.x+COTE_TILE*0.5)/COTE_TILE);
                    m_positionCase.y    = (int)((m_positionPixel.y+COTE_TILE*0.5)/COTE_TILE);
                    m_positionPixel.x    = m_positionCase.x*COTE_TILE;
                    m_positionPixel.y    = m_positionCase.y*COTE_TILE;
            }

        m_pousseEnCours = true;
        m_pousse.x = pousse.x;
        m_pousse.y = pousse.y;
    }
}

void Personnage::Pousser(coordonneeDecimal vecteur)
{
    if(!m_impoussable)
    {
        if(fabs(vecteur.x) > fabs(m_pousse.x))
            m_pousse.x = vecteur.x;

        if(fabs(vecteur.y) > fabs(m_pousse.y))
            m_pousse.y = vecteur.y;


        m_pousseEnCours = true;
        frappeEnCours = false;
    }
}



void Personnage::AjouterPointAme(int pointAme)
{
    m_caracteristique.pointAme+=pointAme;
}

bool Personnage::EnVie()
{
    if (m_caracteristique.vie>0)
        return 1;
    return 0;
}

float Personnage::getTime()
{
    float t = m_clock.getElapsedTime().asMilliseconds();
    m_clock.restart();
    return t;
}

const coordonnee &Personnage::getDepart()
{
    return m_depart;
}

const coordonnee &Personnage::getCoordonnee()
{
    return m_positionCase;
}
const coordonnee &Personnage::getArrivee()
{
    return m_arrivee;
}

const Caracteristique &Personnage::getCaracteristique()
{
    return m_caracteristique;
}
const Caracteristique &Modele_Personnage::getCaracteristique()
{
    return m_caracteristique;
}
const std::string &Personnage::getNom()
{
    return m_caracteristique.nom;
}

int Personnage::getEtat()
{
    return m_etat;
}
int Personnage::getAngle()
{
    return m_angle;
}
int Personnage::getNextAngle()
{
    return m_next_angle;
}
int Personnage::getPose()
{
    return m_entite_graphique.m_noAnimation;
}
bool Personnage::getErreurPathfinding()
{
    return m_erreurPathfinding;
}
const coordonneeDecimal &Personnage::getCoordonneePixel()
{
    return m_positionPixel;
}
coordonneeDecimal Personnage::getEstimationMouvement()
{
    coordonneeDecimal buf;

    if(m_cheminFinal.y - m_positionCase.y == 0)
        buf.x = (m_cheminFinal.x - m_positionCase.x) * m_caracteristique.vitesse * 400;
    else
        buf.x = (m_cheminFinal.x - m_positionCase.x) * m_caracteristique.vitesse * 200;

    if(m_cheminFinal.x - m_positionCase.x == 0)
        buf.y = (m_cheminFinal.y - m_positionCase.y) * m_caracteristique.vitesse * 400;
    else
        buf.y = (m_cheminFinal.y - m_positionCase.y) * m_caracteristique.vitesse * 200;

    return buf;
}



const coordonneeDecimal &Personnage::getPousse()
{
    return m_pousse;
}
const coordonnee &Personnage::getProchaineCase()
{
    if (m_cheminFinal.x!=m_positionCase.x||m_cheminFinal.y!=m_positionCase.y)
        return m_cheminFinal;
    else
        return m_positionCase;
}

const std::vector<Objet> &Personnage::getObjets()
{
    return m_objets;
}
std::vector<Objet>* Personnage::getPointeurObjets()
{
    return &m_objets;
}

void Personnage::setObjets(std::vector<Objet> objets)
{
    m_objets=objets;
}

int Personnage::getModele()
{
    return m_modele;
}

