

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
51 Franklin Street, Fifth Floor, Boston, MA 02110-1101 USA.*/


#include "liste_case.h"
#include "../globale.h"

#include <iostream>

liste_case::~liste_case()
{
    m_liste.clear();
}

void liste_case::setTailleListe(int taille)
{
    Case caseTemp;
    m_liste.resize(taille,caseTemp);
    m_distanceEnCours=0;
}
int liste_case::getTailleListe()
{
    return m_liste.size();
}

void liste_case::AjouterCase(coordonnee coordonneeAjoutable, int parent)
{
    Case caseTemp;
    caseTemp.actif = true;
    m_liste.push_back(caseTemp);
    m_liste.back().setCoordonnee(coordonneeAjoutable,m_distanceEnCours, parent);
}

void liste_case::Supprimer()
{
    m_liste.clear();
}

/*bool liste_case::TesterCasesEnCours(coordonnee caseCherchee)
{
    // Je regarde si je suis sur ma case d'arrivee
    for (iter=m_liste.begin();iter!=m_liste.end();++iter)
        if (iter->getPosition().x==caseCherchee.x&&iter->getPosition().y==caseCherchee.y)
            return 1;

    return 0;
}*/

bool liste_case::AjouterCasesAdjacentes(casePathfinding **grille,coordonnee *arrivee)
{
    coordonnee enCours;
    bool retour = false;

    //J'ajoute toutes les cases adjacentes à celle que j'ai déjà dans ma liste
    for (int i=m_liste.size()-1;i>=0;i--)
        if(m_liste[i].actif)
        {
            if (m_liste[i].getDistance()==m_distanceEnCours-1)
            {
                enCours.x=m_liste[i].getPosition().x+1;
                enCours.y=m_liste[i].getPosition().y;
                if (enCours.x>=0&&enCours.y>=0&&enCours.x<10&&enCours.y<10)
                    if((enCours.x == arrivee->x && enCours.y == arrivee->y) || !retour)
                    if(!grille[enCours.y][enCours.x].collision
                    &&  ((grille[enCours.y][enCours.x].valeur == m_distanceEnCours && grille[enCours.y][enCours.x].dist > 1)
                      ||grille[enCours.y][enCours.x].valeur == -1)
                    &&  fabs(grille[enCours.y][enCours.x].hauteur- grille[m_liste[i].getPosition().y][m_liste[i].getPosition().x].hauteur)<=32)
                    {
                        if(grille[enCours.y][enCours.x].cases >= 0)
                            m_liste[grille[enCours.y][enCours.x].cases].actif = false;
                            //m_liste.erase(m_liste.begin() + grille[enCours.y][enCours.x].cases);
                        AjouterCase(enCours, i);
                        grille[enCours.y][enCours.x].valeur = m_distanceEnCours;
                        //grille[enCours.y][enCours.x].dist = 1;
                        grille[enCours.y][enCours.x].cases = m_liste.size()-1;
                    }

                if(enCours.x == arrivee->x && enCours.y == arrivee->y)
                    retour = true;

                enCours.x=m_liste[i].getPosition().x-1;
                enCours.y=m_liste[i].getPosition().y;
                if (enCours.x>=0&&enCours.y>=0&&enCours.x<10&&enCours.y<10)
                    if((enCours.x == arrivee->x && enCours.y == arrivee->y) || !retour)
                    if(!grille[enCours.y][enCours.x].collision
                    &&  ((grille[enCours.y][enCours.x].valeur == m_distanceEnCours && grille[enCours.y][enCours.x].dist > 1)
                      ||grille[enCours.y][enCours.x].valeur == -1)
                    &&  fabs(grille[enCours.y][enCours.x].hauteur- grille[m_liste[i].getPosition().y][m_liste[i].getPosition().x].hauteur)<=32)
                    {
                        if(grille[enCours.y][enCours.x].cases >= 0)
                            m_liste[grille[enCours.y][enCours.x].cases].actif = false;
                        AjouterCase(enCours, i);
                        grille[enCours.y][enCours.x].valeur = m_distanceEnCours;
                        //grille[enCours.y][enCours.x].dist = 1;
                        grille[enCours.y][enCours.x].cases = m_liste.size()-1;
                    }

                if(enCours.x == arrivee->x && enCours.y == arrivee->y)
                    retour = true;


                enCours.x=m_liste[i].getPosition().x;
                enCours.y=m_liste[i].getPosition().y+1;
                if (enCours.x>=0&&enCours.y>=0&&enCours.x<10&&enCours.y<10)
                    if((enCours.x == arrivee->x && enCours.y == arrivee->y) || !retour)
                    if(!grille[enCours.y][enCours.x].collision
                    &&  ((grille[enCours.y][enCours.x].valeur == m_distanceEnCours && grille[enCours.y][enCours.x].dist > 1)
                      ||grille[enCours.y][enCours.x].valeur == -1)
                    &&  fabs(grille[enCours.y][enCours.x].hauteur- grille[m_liste[i].getPosition().y][m_liste[i].getPosition().x].hauteur)<=32)
                    {
                        if(grille[enCours.y][enCours.x].cases >= 0)
                            m_liste[grille[enCours.y][enCours.x].cases].actif = false;
                        AjouterCase(enCours, i);
                        grille[enCours.y][enCours.x].valeur = m_distanceEnCours;
                        //grille[enCours.y][enCours.x].dist = 1;
                        grille[enCours.y][enCours.x].cases = m_liste.size()-1;
                    }

                if(enCours.x == arrivee->x && enCours.y == arrivee->y)
                    retour = true;


                enCours.x=m_liste[i].getPosition().x;
                enCours.y=m_liste[i].getPosition().y-1;
                if (enCours.x>=0&&enCours.y>=0&&enCours.x<10&&enCours.y<10)
                    if((enCours.x == arrivee->x && enCours.y == arrivee->y) || !retour)
                    if(!grille[enCours.y][enCours.x].collision
                    &&  ((grille[enCours.y][enCours.x].valeur == m_distanceEnCours && grille[enCours.y][enCours.x].dist > 1)
                      ||grille[enCours.y][enCours.x].valeur == -1)
                    &&  fabs(grille[enCours.y][enCours.x].hauteur- grille[m_liste[i].getPosition().y][m_liste[i].getPosition().x].hauteur)<=32)
                    {
                        if(grille[enCours.y][enCours.x].cases >= 0)
                            m_liste[grille[enCours.y][enCours.x].cases].actif = false;
                        AjouterCase(enCours, i);
                        grille[enCours.y][enCours.x].valeur = m_distanceEnCours;
                        //grille[enCours.y][enCours.x].dist = 1;
                        grille[enCours.y][enCours.x].cases = m_liste.size()-1;
                    }

                if(enCours.x == arrivee->x && enCours.y == arrivee->y)
                    retour = true;


                enCours.x=m_liste[i].getPosition().x+1;
                enCours.y=m_liste[i].getPosition().y+1;
                if (enCours.x>=0&&enCours.y>=0&&enCours.x<10&&enCours.y<10)
                    if((enCours.x == arrivee->x && enCours.y == arrivee->y) || !retour)
                    if(!grille[enCours.y][enCours.x].collision
                    &&  ((grille[enCours.y][enCours.x].valeur == m_distanceEnCours && grille[enCours.y][enCours.x].dist > 2)
                      ||grille[enCours.y][enCours.x].valeur == -1)
                    &&  fabs(grille[enCours.y][enCours.x].hauteur- grille[m_liste[i].getPosition().y][m_liste[i].getPosition().x].hauteur)<=32)
                    {
                        if(grille[enCours.y][enCours.x].cases >= 0)
                            m_liste[grille[enCours.y][enCours.x].cases].actif = false;
                        AjouterCase(enCours, i);
                        grille[enCours.y][enCours.x].valeur = m_distanceEnCours;
                        //grille[enCours.y][enCours.x].dist = 2;
                        grille[enCours.y][enCours.x].cases = m_liste.size()-1;
                    }

                if(enCours.x == arrivee->x && enCours.y == arrivee->y)
                    retour = true;


                enCours.x=m_liste[i].getPosition().x-1;
                enCours.y=m_liste[i].getPosition().y-1;
                if (enCours.x>=0&&enCours.y>=0&&enCours.x<10&&enCours.y<10)
                    if((enCours.x == arrivee->x && enCours.y == arrivee->y) || !retour)
                    if(!grille[enCours.y][enCours.x].collision
                    &&  ((grille[enCours.y][enCours.x].valeur == m_distanceEnCours && grille[enCours.y][enCours.x].dist > 2)
                      ||grille[enCours.y][enCours.x].valeur == -1)
                    &&  fabs(grille[enCours.y][enCours.x].hauteur- grille[m_liste[i].getPosition().y][m_liste[i].getPosition().x].hauteur)<=32)
                    {
                        if(grille[enCours.y][enCours.x].cases >= 0)
                            m_liste[grille[enCours.y][enCours.x].cases].actif = false;
                        AjouterCase(enCours, i);
                        grille[enCours.y][enCours.x].valeur = m_distanceEnCours;
                        //grille[enCours.y][enCours.x].dist = 2;
                        grille[enCours.y][enCours.x].cases = m_liste.size()-1;
                    }

                if(enCours.x == arrivee->x && enCours.y == arrivee->y)
                    retour = true;

                enCours.x=m_liste[i].getPosition().x-1;
                enCours.y=m_liste[i].getPosition().y+1;
                if (enCours.x>=0&&enCours.y>=0&&enCours.x<10&&enCours.y<10)
                    if((enCours.x == arrivee->x && enCours.y == arrivee->y) || !retour)
                    if(!grille[enCours.y][enCours.x].collision
                    &&  ((grille[enCours.y][enCours.x].valeur == m_distanceEnCours && grille[enCours.y][enCours.x].dist > 2)
                      ||grille[enCours.y][enCours.x].valeur == -1)
                    &&  fabs(grille[enCours.y][enCours.x].hauteur- grille[m_liste[i].getPosition().y][m_liste[i].getPosition().x].hauteur)<=32)
                    {
                        if(grille[enCours.y][enCours.x].cases >= 0)
                            m_liste[grille[enCours.y][enCours.x].cases].actif = false;
                        AjouterCase(enCours, i);
                        grille[enCours.y][enCours.x].valeur = m_distanceEnCours;
                        //grille[enCours.y][enCours.x].dist = 2;
                        grille[enCours.y][enCours.x].cases = m_liste.size()-1;
                    }

                if(enCours.x == arrivee->x && enCours.y == arrivee->y)
                    retour = true;


                enCours.x=m_liste[i].getPosition().x+1;
                enCours.y=m_liste[i].getPosition().y-1;
                if (enCours.x>=0&&enCours.y>=0&&enCours.x<10&&enCours.y<10)
                    if((enCours.x == arrivee->x && enCours.y == arrivee->y) || !retour)
                    if(!grille[enCours.y][enCours.x].collision
                    &&  ((grille[enCours.y][enCours.x].valeur == m_distanceEnCours && grille[enCours.y][enCours.x].dist > 2)
                      ||grille[enCours.y][enCours.x].valeur == -1)
                    &&  fabs(grille[enCours.y][enCours.x].hauteur- grille[m_liste[i].getPosition().y][m_liste[i].getPosition().x].hauteur)<=32)
                    {
                        if(grille[enCours.y][enCours.x].cases >= 0)
                            m_liste[grille[enCours.y][enCours.x].cases].actif = false;
                        AjouterCase(enCours, i);
                        grille[enCours.y][enCours.x].valeur = m_distanceEnCours;
                        //grille[enCours.y][enCours.x].dist = 2;
                        grille[enCours.y][enCours.x].cases = m_liste.size()-1;
                    }

                if(enCours.x == arrivee->x && enCours.y == arrivee->y)
                    retour = true;
            }
            else
                return retour;
        }

    return retour;
}

void liste_case::IncrementerDistanceEnCours()
{
    m_distanceEnCours++;
}

void liste_case::DecrementerDistanceEnCours()
{
    m_distanceEnCours--;
}

int liste_case::getDistance()
{
    return m_distanceEnCours;
}

Case liste_case::getCase(int ID)
{
    if(ID >=0 && ID < (int)m_liste.size())
        return m_liste[ID];

    return m_liste[0];
}

/*coordonnee liste_case::TrouverLeChemin(coordonnee caseEnCours)
{
    // Je repart de mon arrivee et suis les cases dans l'ordre décroisant jusqu'à me retrouver à ma case départ
    coordonnee enCours;
    for (iter = m_liste.begin() ; iter != m_liste.end() ; ++iter)
        if (iter->getPosition().x==caseEnCours.x&&iter->getPosition().y==caseEnCours.y)
        {
            enCours.y=iter->getPosition().y;
            enCours.x=iter->getPosition().x+1;
            for (std::vector <Case>::iterator iter2 = m_liste.begin() ; iter2 != m_liste.end() ; ++iter2)
                if (iter2->getDistance()==m_distanceEnCours-1&&iter2->getPosition().x==enCours.x&&iter2->getPosition().y==enCours.y)
                    return iter2->getPosition();

            enCours.y=iter->getPosition().y;
            enCours.x=iter->getPosition().x-1;
            for (std::vector <Case>::iterator iter2 = m_liste.begin() ; iter2 != m_liste.end() ; ++iter2)
                if (iter2->getDistance()==m_distanceEnCours-1&&iter2->getPosition().x==enCours.x&&iter2->getPosition().y==enCours.y)
                    return iter2->getPosition();

            enCours.y=iter->getPosition().y+1;
            enCours.x=iter->getPosition().x;
            for (std::vector <Case>::iterator iter2 = m_liste.begin() ; iter2 != m_liste.end() ; ++iter2)
                if (iter2->getDistance()==m_distanceEnCours-1&&iter2->getPosition().x==enCours.x&&iter2->getPosition().y==enCours.y)
                    return iter2->getPosition();

            enCours.y=iter->getPosition().y-1;
            enCours.x=iter->getPosition().x;
            for (std::vector <Case>::iterator iter2 = m_liste.begin() ; iter2 != m_liste.end() ; ++iter2)
                if (iter2->getDistance()==m_distanceEnCours-1&&iter2->getPosition().x==enCours.x&&iter2->getPosition().y==enCours.y)
                    return iter2->getPosition();

            enCours.y=iter->getPosition().y-1;
            enCours.x=iter->getPosition().x+1;
            for (std::vector <Case>::iterator iter2 = m_liste.begin() ; iter2 != m_liste.end() ; ++iter2)
                if (iter2->getDistance()==m_distanceEnCours-1&&iter2->getPosition().x==enCours.x&&iter2->getPosition().y==enCours.y)
                    return iter2->getPosition();

            enCours.y=iter->getPosition().y+1;
            enCours.x=iter->getPosition().x-1;
            for (std::vector <Case>::iterator iter2 = m_liste.begin() ; iter2 != m_liste.end() ; ++iter2)
                if (iter2->getDistance()==m_distanceEnCours-1&&iter2->getPosition().x==enCours.x&&iter2->getPosition().y==enCours.y)
                    return iter2->getPosition();

            enCours.y=iter->getPosition().y+1;
            enCours.x=iter->getPosition().x+1;
            for (std::vector <Case>::iterator iter2 = m_liste.begin() ; iter2 != m_liste.end() ; ++iter2)
                if (iter2->getDistance()==m_distanceEnCours-1&&iter2->getPosition().x==enCours.x&&iter2->getPosition().y==enCours.y)
                    return iter2->getPosition();

            enCours.y=iter->getPosition().y-1;
            enCours.x=iter->getPosition().x-1;
            for (std::vector <Case>::iterator iter2 = m_liste.begin() ; iter2 != m_liste.end() ; ++iter2)
                if (iter2->getDistance()==m_distanceEnCours-1&&iter2->getPosition().x==enCours.x&&iter2->getPosition().y==enCours.y)
                    return iter2->getPosition();

            enCours.y=iter->getPosition().y;
            enCours.x=iter->getPosition().x;
            for (std::vector <Case>::iterator iter2 = m_liste.begin() ; iter2 != m_liste.end() ; ++iter2)
                if (iter2->getDistance()==m_distanceEnCours-1&&iter2->getPosition().x==enCours.x&&iter2->getPosition().y==enCours.y)
                    return iter2->getPosition();
        }
    return enCours;
}*/


