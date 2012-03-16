

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



#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include <math.h>
#include "eventManager.h"
#include "../globale.h"
#include <iostream>

using namespace sf;

EventManager::EventManager()
{
    for (int i=0;i<500;i++)
        m_EventTableau[i]=0;
    for (int i=0;i<5;i++)
        m_Clic[i]=0;
    for (int i=0;i<5;i++)
        m_ClicAncien[i]=0;

    PressAnyKey = false;

    m_positionSouris.x=450;
    m_positionSouris.y=300;

    console->Ajouter("",0);
    console->Ajouter("Chargement des curseurs :",0);

    idcurseur=moteurGraphique->AjouterImage(configuration->chemin_curseurs+configuration->nom_curseur_base,-1);

    m_molette = 0;
    m_char = -1;
}

void EventManager::GererLesEvenements(bool *continuer,float temps,coordonnee tailleMap)
{
    PressAnyKey = false;
    isEnteredText = false;

    m_molette = 0;
    Event Event;
    while (moteurGraphique->getEvent(Event))
    {
        switch (Event.Type)
        {
        case Event::KeyPressed:
            if (Event.Key.Code>=0&&Event.Key.Code<500)
                m_EventTableau[Event.Key.Code] = 1, PressAnyKey = true;
            break;
        case Event::KeyReleased:
            if (Event.Key.Code>=0&&Event.Key.Code<500)
                m_EventTableau[Event.Key.Code] = 0;
            break;

        case sf::Event::TextEntered:
            m_char = Event.Text.Unicode, isEnteredText = true;
            break;

        case Event::MouseMoved:
            m_positionSouris.x=Event.MouseMove.X;
            m_positionSouris.y=Event.MouseMove.Y;
            break;
        case Event::MouseButtonPressed:
            m_Clic[Event.MouseButton.Button]=1;
            m_ClicAncien[Event.MouseButton.Button]=1, PressAnyKey = true;
            break;
        case Event::MouseButtonReleased:
            m_Clic[Event.MouseButton.Button]=0;
            m_ClicAncien[Event.MouseButton.Button]=0;
            break;
        case Event::MouseWheelMoved:
            m_molette = Event.MouseWheel.Delta;
            break;
        case Event::Closed:
            *continuer = false;
            break;

        case Event::LostFocus:
            for (int i=0;i<500;i++)
                m_EventTableau[i]=0;
            for (int i=0;i<5;i++)
                m_Clic[i]=0;
            for (int i=0;i<5;i++)
                m_ClicAncien[i]=0;
            break;

        case Event::Resized:
            configuration->Redimensionnement.w=(float)configuration->Resolution.w*0.00125;
            configuration->Redimensionnement.h=(float)configuration->Resolution.h*0.00167;
            configuration->Redimensionnement.x=(float)configuration->Resolution.x*0.00125;
            configuration->Redimensionnement.y=(float)configuration->Resolution.y*0.00167;
            break;
        default:
            break;
        }
    }

    if(m_EventTableau[sf::Keyboard::Back])
        isEnteredText = false;
    if(m_EventTableau[sf::Keyboard::Return])
        isEnteredText = false;

    if (m_EventTableau[Keyboard::PageDown])
        configuration->zoom_or*=(1+(0.5*temps));
    if (m_EventTableau[Keyboard::PageUp])
        configuration->zoom_or*=((1-(0.5*temps)));

    if (configuration->zoom_or<0.75-((float)configuration->Resolution.y/600-1)/2)
        configuration->zoom_or=0.75-((float)configuration->Resolution.y/600-1)/2;
    if (configuration->zoom_or>1.5-((float)configuration->Resolution.y/600-1))
        configuration->zoom_or=1.5-((float)configuration->Resolution.y/600-1);

    if (m_EventTableau[Keyboard::Add])
    {
        if (m_EventTableau[Keyboard::S])
            configuration->volume+=temps*50;
        if (m_EventTableau[Keyboard::L])
            configuration->luminosite+=temps*50;
        if (m_EventTableau[Keyboard::C])
            configuration->contrastes+=temps*2;
    }
    if (m_EventTableau[Keyboard::Subtract])
    {
        if (m_EventTableau[Keyboard::S])
            configuration->volume-=temps*50;
        if (m_EventTableau[Keyboard::L])
            configuration->luminosite-=temps*50;
        if (m_EventTableau[Keyboard::C])
            configuration->contrastes-=temps*2;
    }

    if (configuration->contrastes<1)
        configuration->contrastes=1;
    if (configuration->contrastes>5)
        configuration->contrastes=5;

    if (configuration->luminosite<0)
        configuration->luminosite=0;
    if (configuration->luminosite>64)
        configuration->luminosite=64;
    if (m_EventTableau[Keyboard::Up])
    {
        console->Defiler(0);
        m_EventTableau[Keyboard::Up]=false;
    }
    if (m_EventTableau[Keyboard::Down])
    {
        console->Defiler(1);
        m_EventTableau[Keyboard::Down]=false;
    }

    if(!configuration->entering_text)
    if (m_EventTableau[Keyboard::Space])
        configuration->minute+=200*temps;

    sf::Vector2f positionSourisTotale = moteurGraphique->getPositionSouris();

    //Conversion des coord cartésienne en coord iso
    m_casePointee.x = (int)(positionSourisTotale.y * 2 + positionSourisTotale.x) / 128;
    m_casePointee.y = (int)(positionSourisTotale.y * 2 - positionSourisTotale.x) / 128;

    if (m_casePointee.x<0)
        m_casePointee.x=0;
    if (m_casePointee.y<0)
        m_casePointee.y=0;
    if (m_casePointee.x>tailleMap.x)
        m_casePointee.x=tailleMap.x;
    if (m_casePointee.y>tailleMap.y)
        m_casePointee.y=tailleMap.y;
}

void EventManager::AfficherCurseur(bool transparent)
{
    m_positionSouris.x = moteurGraphique->getPositionSourisEcran().x;
    m_positionSouris.y = moteurGraphique->getPositionSourisEcran().y;

    Sprite Sprite;
    Sprite.SetTexture(*moteurGraphique->getImage(idcurseur));
    if (configuration->Resolution.x > 0)
        Sprite.SetX(m_positionSouris.x*configuration->Resolution.w/configuration->Resolution.x-2);
    if (configuration->Resolution.y > 0)
        Sprite.SetY(m_positionSouris.y*configuration->Resolution.h/configuration->Resolution.y-2);

    if(transparent)
        Sprite.SetColor(sf::Color(255,255,255,64));

    if(m_Clic[Mouse::Left])
    {
        Sprite.Scale(0.95,0.95);
        Sprite.Move(-2,-1);
    }

    moteurGraphique->AjouterCommande(&Sprite,20,0);
}

bool EventManager::getEvenement(int numeroEvenement, int type)
{
    if (type==EventKey)
        if (numeroEvenement>=0&&numeroEvenement<500)
            return m_EventTableau[numeroEvenement];

    if (type==EventClic)
        if (numeroEvenement>=0&&numeroEvenement<=5)
            return m_Clic[numeroEvenement];

    if (type==EventClicA)
        if (numeroEvenement>=0&&numeroEvenement<=5)
            return m_ClicAncien[numeroEvenement];

    return 0;
}

void EventManager::StopEvenement(int numeroEvenement,int type)
{
    if (type==EventKey)
        if (numeroEvenement>=0&&numeroEvenement<500)
            m_EventTableau[numeroEvenement]=0;

    if (type==EventClic)
        if (numeroEvenement>=0&&numeroEvenement<=5)
            m_Clic[numeroEvenement]=0;

    if (type==EventClicA)
        if (numeroEvenement>=0&&numeroEvenement<=5)
            m_ClicAncien[numeroEvenement]=0;
}



const coordonnee &EventManager::getCasePointee()
{
    return m_casePointee;
}
const coordonnee &EventManager::getPositionSouris()
{
    return m_positionSouris;
}
int EventManager::getMolette()
{
    return m_molette;
}


void EventManager::arreterClique()
{
    m_Clic[Mouse::Left]=false;
}

sf::Uint32 EventManager::getChar()
{
    return (m_char);
}

bool EventManager::IsEnteredText()
{
    return (isEnteredText);
}

