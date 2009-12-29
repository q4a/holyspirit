

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

    m_positionSouris.x=450;
    m_positionSouris.y=300;

    console->Ajouter("",0);
    console->Ajouter("Chargement des curseurs :",0);

    idcurseur=moteurGraphique->AjouterImage(configuration->chemin_curseurs+configuration->nom_curseur_base,-1);

    m_img_select=moteurGraphique->AjouterImage("Data/Editeur/Menus/selection.png",-1);
    m_img_select2=moteurGraphique->AjouterImage("Data/Editeur/Menus/selection_2.png",-1);


    m_molette = 0;
    m_coordonnee.x = 1;
    m_coordonnee.y = 1;
}

void EventManager::GererLesEvenements(coordonnee tailleMap)
{
    m_molette = 0;
    Event Event;
    while (moteurGraphique->getEvent(Event))
    {
        switch (Event.Type)
        {
        case Event::KeyPressed:
            if (Event.Key.Code>=0&&Event.Key.Code<500)
                m_EventTableau[Event.Key.Code] = 1;
            break;
        case Event::KeyReleased:
            if (Event.Key.Code>=0&&Event.Key.Code<500)
                m_EventTableau[Event.Key.Code] = 0;
            break;
        case Event::MouseMoved:
            m_positionSouris.x=Event.MouseMove.X;
            m_positionSouris.y=Event.MouseMove.Y;
            break;
        case Event::MouseButtonPressed:
            m_Clic[Event.MouseButton.Button]=1;
            m_ClicAncien[Event.MouseButton.Button]=0;
            break;
        case Event::MouseButtonReleased:
            m_Clic[Event.MouseButton.Button]=0;
            m_ClicAncien[Event.MouseButton.Button]=1;
            break;
        case Event::MouseWheelMoved:
            m_molette = Event.MouseWheel.Delta;
            break;
       // case Event::Closed:
           // *continuer = false;
           // break;

        case Event::Resized:
            configuration->Redimensionnement.w=(float)configuration->Resolution.w*0.00125;
            configuration->Redimensionnement.h=(float)configuration->Resolution.h*0.00167;
            configuration->Redimensionnement.x=(float)configuration->Resolution.x*0.00125;
            configuration->Redimensionnement.y=(float)configuration->Resolution.y*0.00167;
            break;
        default:
            break;
        }

        if (Event.Type == sf::Event::Resized)
        {
            moteurGraphique->m_ecran->GetDefaultView().Reset(sf::FloatRect(0, 0, Event.Size.Width, Event.Size.Height));
        }
    }

    if (m_EventTableau[Key::PageDown])
        configuration->zoom*=(1+(m_clock.GetElapsedTime()));
    if (m_EventTableau[Key::PageUp])
        configuration->zoom*=((1-(m_clock.GetElapsedTime())));

    /*if (configuration->zoom<0.75+((float)configuration->Resolution.x/800-1)/2)
        configuration->zoom=0.75+((float)configuration->Resolution.x/800-1)/2;
    if (configuration->zoom>1.5+((float)configuration->Resolution.x/800-1))
        configuration->zoom=1.5+((float)configuration->Resolution.x/800-1);*/

    if (m_EventTableau[Key::Add])
    {
        if (m_EventTableau[Key::L])
            configuration->luminosite+=0.0001*50;
        if (m_EventTableau[Key::C])
            configuration->contrastes+=0.0001*2;
    }
    if (m_EventTableau[Key::Subtract])
    {
        if (m_EventTableau[Key::S])
            configuration->volume-=0.0001*50;
        if (m_EventTableau[Key::L])
            configuration->luminosite-=0.0001*50;
        if (m_EventTableau[Key::C])
            configuration->contrastes-=0.0001*2;
    }

    if (configuration->contrastes<1)
        configuration->contrastes=1;
    if (configuration->contrastes>5)
        configuration->contrastes=5;

    if (configuration->luminosite<0)
        configuration->luminosite=0;
    if (configuration->luminosite>64)
        configuration->luminosite=64;

    if (m_EventTableau[Key::Space])
        configuration->minute+=200*0.0001;

    if (m_EventTableau[Key::Tab])
    {
        if (configuration->console==0)
            configuration->console=1;
        else if (configuration->console==1)
            configuration->console=2;
        else
            configuration->console=0;

        m_EventTableau[Key::Tab]=false;
    }

    FloatRect ViewRect = moteurGraphique->m_camera.GetRect();

    coordonnee positionSourisTotale=moteurGraphique->getPositionSouris();

    //Conversion des coord cartésienne en coord iso
    if ((float)((positionSourisTotale.y*2-positionSourisTotale.x)/2)/64+tailleMap.y/2<(float)tailleMap.y/2)
        m_casePointee.y=((positionSourisTotale.y*2-positionSourisTotale.x)/2)/64-1;
    else
        m_casePointee.y=((positionSourisTotale.y*2-positionSourisTotale.x)/2)/64;

    m_casePointee.x=(positionSourisTotale.x+((positionSourisTotale.y*2-positionSourisTotale.x)/2))/64;

    if (m_casePointee.x<0)
        m_casePointee.x=0;
    if (m_casePointee.y<0)
        m_casePointee.y=0;
    if (m_casePointee.x>tailleMap.x - 1)
        m_casePointee.x=tailleMap.x - 1;
    if (m_casePointee.y>tailleMap.y - 1)
        m_casePointee.y=tailleMap.y - 1;
}

void EventManager::AfficherCurseur(int taillePinceau)
{
    coordonnee position;



    Sprite Sprite;
    Sprite.SetImage(*moteurGraphique->getImage(m_img_select));

    for(int i = getCasePointee().y - (int)(taillePinceau*0.5) ; i < getCasePointee().y + taillePinceau*0.5 ; ++i)
    for(int j = getCasePointee().x - (int)(taillePinceau*0.5) ; j < getCasePointee().x + taillePinceau*0.5 ; ++j)
    {
        position.x=(j-i-1)*64;
        position.y=(j+i)*32;
        Sprite.SetPosition(position.x, position.y);
        moteurGraphique->AjouterCommande(&Sprite,20,1);
    }
}

bool EventManager::getEvenement(int numeroEvenement,std::string evenement)
{
    if (evenement=="ET")
        if (numeroEvenement>=0&&numeroEvenement<500)
            return m_EventTableau[numeroEvenement];

    if (evenement=="C")
        if (numeroEvenement>=0&&numeroEvenement<=5)
            return m_Clic[numeroEvenement];

    if (evenement=="CA")
        if (numeroEvenement>=0&&numeroEvenement<=5)
            return m_ClicAncien[numeroEvenement];

    return 0;
}

void EventManager::StopEvenement(int numeroEvenement,std::string evenement)
{
    if (evenement=="ET")
        if (numeroEvenement>=0&&numeroEvenement<500)
            m_EventTableau[numeroEvenement]=0;

    if (evenement=="C")
        if (numeroEvenement>=0&&numeroEvenement<=5)
            m_Clic[numeroEvenement]=0;

    if (evenement=="CA")
        if (numeroEvenement>=0&&numeroEvenement<=5)
            m_ClicAncien[numeroEvenement]=0;
}



coordonnee EventManager::getCasePointee()
{
    return m_casePointee;
}
coordonnee EventManager::getPositionSouris()
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



