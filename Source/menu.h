

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
#include <SFML/Audio.hpp>

#ifndef MENUH
#define MENUH

#include "constantes.h"
#include "Entites/classe.h"

class Ame
{
    public:
    Ame (coordonneeDecimal position,int pointAme)
    {
        m_position=position;
        m_depart=position;
        m_taille=0;
        m_mode=-1;
        m_rotation=0;
        m_pointAme=pointAme;
        m_alpha=0;
        augmenter=true;
    }
    coordonneeDecimal m_position,m_depart;
    float m_taille,m_rotation,m_alpha;
    int m_mode,m_pointAme;
    bool augmenter;
};

class Sang
{
    public:
    Sang (coordonneeDecimal position)
    {
        m_position=position;
        m_taille=(float)(rand()%(100-25) + 25)/100;
        m_rotation=(float)(rand()%360);
        m_alpha=255;
        m_numero=(rand()%(NOMBRE_SANG));
    }
    coordonneeDecimal m_position;
    float m_taille,m_rotation,m_alpha;
    int m_numero;
};

struct Speak_choice
{
    int no;
    std::string text;
};

class Menu
{
	public:
	Menu();
	~Menu();
	void AfficherHUD(Classe *classe);
	void AfficherDialogue(float, Classe *classe);
	void AfficherDynamique(Caracteristique caracteristique,int type,Caracteristique caracteristiqueMonstre,Classe *classe);
	void AfficherChargement(std::string nom,int fond,int z);
	void AfficherInventaire(float,Classe *classe,bool);
	void AfficherQuetes(float,Classe *classe);
	void AfficherDocs(float,Classe *classe);
	void AfficherCraft(float,Classe *classe);
	void AfficherMiracles(float,Classe *classe );

	void AddSpeakChoice(const std::string &text, int no);
	void ClearSpeakChoice();
	int getSpeakChoice();

	std::string m_dialogue;
	coordonnee  m_dialogue_position;

	float m_hauteur;
	float m_cur_talk_hauteur;

	Border border;
	Image_interface mainscreen;
	Image_interface logo;
	Bouton_pressoire buttons[8];


	private:
	sf::Text texte;

	int m_speak_choice;
	std::vector <Speak_choice> m_choices;
	std::vector <Speak_choice> m_old_choices;


	std::string m_old_dialogue;
};

#endif

