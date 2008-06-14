
#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include <math.h>
#include "eventManager.h"
#include "prototypes.h"
#include "Globale.h"
#include <iostream>

using namespace sf;

EventManager::EventManager()
{
	for(int i=0;i<500;i++)
	    m_EventTableau[i]=0;
	for(int i=0;i<5;i++)
	    m_Clic[i]=0;
	for(int i=0;i<5;i++)
	    m_ClicAncien[i]=0;

	m_positionSouris.x=450;
	m_positionSouris.y=300;

	console.Ajouter("",0);
    console.Ajouter("Chargement des curseurs :",0);

	if(!m_curseur.LoadFromFile(configuration.chemin_curseurs+configuration.nom_curseur_base))
         console.Ajouter("Impossible de charger :"+configuration.chemin_curseurs+configuration.nom_curseur_base,1);
    else
        console.Ajouter("Chargement de : "+configuration.chemin_curseurs+configuration.nom_curseur_base,0);

}

void EventManager::GererLesEvenements(RenderWindow *ecran,View *camera,bool *continuer,float temps,Hero *hero,Map *map)
{
	Event Event;
	while(ecran->GetEvent(Event))
	{

		switch(Event.Type)
   		{
        	case Event::KeyPressed:
        	    if(Event.Key.Code>=0&&Event.Key.Code<500)
					m_EventTableau[Event.Key.Code] = 1;
            	break;
        	case Event::KeyReleased:
				if(Event.Key.Code>=0&&Event.Key.Code<500)
					m_EventTableau[Event.Key.Code] = 0;
            	break;
            case Event::MouseMoved:
                m_positionSouris.x=Event.MouseMove.X;
                m_positionSouris.y=Event.MouseMove.Y;
                break;
            case Event::MouseButtonPressed:
                m_Clic[Event.MouseButton.Button]=1;
                break;
            case Event::MouseButtonReleased:
                m_Clic[Event.MouseButton.Button]=0;
                break;
    	}
    	if (Event.Type == Event::Closed)
        *continuer = false;
	}

    if(m_EventTableau[Key::Escape])
		*continuer=false;
	if(m_EventTableau[Key::PageUp])
		camera->Zoom *=1+(0.5*temps);
	if(m_EventTableau[Key::PageDown])
		camera->Zoom *=1-(0.5*temps);

    if(m_EventTableau[Key::Add])
		configuration.volume+=temps*50;
    if(m_EventTableau[Key::Subtract])
		configuration.volume-=temps*50;

    if(m_EventTableau[Key::P])
    {
		configuration.effetMort+=temps*50;
		if(configuration.effetMort>200)
		configuration.effetMort=0;
    }

    if(m_EventTableau[Key::M])
    {
        if(!configuration.Minimap)
            configuration.Minimap=true;
        else
            configuration.Minimap=false;
        m_EventTableau[Key::M]=false;
    }

    if(m_EventTableau[Key::Up])
    {
        console.defiler(0);
        m_EventTableau[Key::Up]=false;
    }
    if(m_EventTableau[Key::Down])
    {
         console.defiler(1);
         m_EventTableau[Key::Down]=false;
    }

    if(m_EventTableau[Key::Space])
    {
        configuration.minute+=5;
        m_EventTableau[Key::Space]=false;
    }

    if(m_EventTableau[Key::Tab])
    {
        if(configuration.console)
            configuration.console=false;
        else
            configuration.console=true;
        m_EventTableau[Key::Tab]=false;
    }


    // Je règle mon niveau de zoom en fonction de la résolution
	if(camera->Zoom<0.75+((float)configuration.Resolution.x/800-1)/2)
		camera->Zoom=0.75+((float)configuration.Resolution.x/800-1)/2;
	if(camera->Zoom>1.5+((float)configuration.Resolution.x/800-1))
		camera->Zoom=1.5+((float)configuration.Resolution.x/800-1);

	ecran->SetView(camera);
	FloatRect ViewRect = ecran->GetViewRect();

	coordonnee positionSourisTotale;
	positionSourisTotale.x=(int)(ViewRect.Left+m_positionSouris.x/camera->Zoom);
	positionSourisTotale.y=(int)(ViewRect.Top+m_positionSouris.y/camera->Zoom);


    //Conversion des coord cartésienne en coord iso
    if((float)((positionSourisTotale.y*2-positionSourisTotale.x)/2)/64+map->getDimensions().y/2<(float)map->getDimensions().y/2)
        m_casePointee.y=((positionSourisTotale.y*2-positionSourisTotale.x)/2)/64+map->getDimensions().y/2-1;
    else
        m_casePointee.y=((positionSourisTotale.y*2-positionSourisTotale.x)/2)/64+map->getDimensions().y/2;

    m_casePointee.x=(positionSourisTotale.x+((positionSourisTotale.y*2-positionSourisTotale.x)/2))/64-map->getDimensions().y/2;

    if(m_casePointee.x<0)
        m_casePointee.x=0;
    if(m_casePointee.y<0)
        m_casePointee.y=0;
    if(m_casePointee.x>map->getDimensions().x)
        m_casePointee.x=map->getDimensions().x;
    if(m_casePointee.y>map->getDimensions().y)
        m_casePointee.y=map->getDimensions().y;

	if(m_Clic[Mouse::Left]&&!m_EventTableau[Key::Shift])
	{

        hero->setMonstreVise(-1);
        hero->setMonstreVise(map->getMonstre(hero,camera,ecran,m_positionSouris));
        if(hero->getEnemiVise()==-1)
         hero->m_personnage.setArrivee(m_casePointee);
	}
	if(m_Clic[Mouse::Right]||m_Clic[Mouse::Left]&&m_EventTableau[Key::Shift])
	{
	    coordonnee temp;
	    temp.x=configuration.Resolution.x/2;
	    temp.y=configuration.Resolution.y/2;
        hero->m_personnage.frappe(m_positionSouris,temp);
	}
}

void EventManager::AfficherCurseur(sf::RenderWindow *ecran)
{
	Sprite Sprite;
	Sprite.SetImage(m_curseur);
	Sprite.SetLeft(m_positionSouris.x);
	Sprite.SetTop(m_positionSouris.y);
	ecran->Draw(Sprite);
}

bool EventManager::getEvenement(int numeroEvenement,std::string evenement)
{
	if(evenement=="EventTableau")
	    if(numeroEvenement>=0&&numeroEvenement<500)
	        return m_EventTableau[numeroEvenement];

	if(evenement=="Clic")
	    if(numeroEvenement>=0&&numeroEvenement<=5)
	        return m_Clic[numeroEvenement];

	if(evenement=="ClicAncien")
	    if(numeroEvenement>=0&&numeroEvenement<=5)
	        return m_ClicAncien[numeroEvenement];
}

coordonnee EventManager::getCasePointee(){return m_casePointee;}
coordonnee EventManager::getPositionSouris(){return m_positionSouris;}

void EventManager::arreterClique(){m_Clic[Mouse::Left]=false;}



