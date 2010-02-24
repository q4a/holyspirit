

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



#ifndef _JEUH
#define _JEUH

#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>

#include "contexte.h"
#include "../globale.h"

//! Contexte principal du jeu. Il g�re � peu pr�s tout ce qui concerne le jeu en in-game.
/**
G�re l'IA, les d�placements, les animations, l'affichage...
*/
class c_Jeu : public Contexte
{
public:
    //! Constructeur
    /**
    Initialise tout une s�rie de variables :
    L'heure, qui prend une valeur al�atoire
    Tous les compteurs de temps � 0
    Initialise les �critures pour les FPS
    */
    c_Jeu();

    //! Utilise le contexte.
    /**
    C'est le coeur du contexte.
    Il fait appelle � toutes ses m�thodes.
    \param jeu : class qui contient le jeu, donc le h�ro, la map en cours, ...
    */
    void Utiliser(Jeu *jeu);

    float alpha_map, alpha_sac, alpha_dialog;

    sf::Thread *m_thread_sauvegarde;

private:

    //! Incr�mente les compteurs de temps avec le temps �coul� depuis la derni�re boucle
    /**
    Va aussi s'occuper de faire appelle aux fonctions pour r�g�nerer la vie, la foi, les points d'ames
    Et gerer le taux du post effect pour quand la vie est presque � 0
    \param jeu : class qui contient le jeu, donc le h�ro, la map en cours, ...
    */
    void GererTemps(Jeu *jeu);


    //! G�re l'IA des monstres
    /**
    Fait simplement appelle � la fonction de gestion de l'IA des monstres
    \param jeu : class qui contient le jeu, donc le h�ro, la map en cours, ...
    */
    void IA(Jeu *jeu);

    //! G�re les d�placements du h�ro
    /**
    G�re les d�placements du h�ro
    Regarde quel monstre est vis� par la souris, plus frappe si SHIFT
    Placement de l'�couteur
    \param jeu : class qui contient le jeu, donc le h�ro, la map en cours, ...
    */
    void Deplacements(Jeu *jeu);

    //! G�re l'animation des d�cors, monstres et du h�ro
    /**
    Donc g�re l'animation des entit�s et d�cors
    G�re les miracles du h�ro
    G�re les d�gats inflig�s par le h�ro
    \param jeu : class qui contient le jeu, donc le h�ro, la map en cours, ...
    */
    void Animation(Jeu *jeu);

    //! Calcul la lumi�re du h�ro et le timing de rafraichissement des lumi�res
    /**
    \param jeu : class qui contient le jeu, donc le h�ro, la map en cours, ...
    */
    void Lumieres(Jeu *jeu);

    //! G�res les �v�nements au clavier et � la souris
    /**
    Va g�rer tous ce qui � trait aux �v�nements, dire quel sera la case d'arriv�e du h�ro, s'il faut afficher l'inventaire,
    l'objet � ramasser...
    \param jeu : class qui contient le jeu, donc le h�ro, la map en cours, ...
    */
    void Evenements(Jeu *jeu);

    //! Affiche le jeu
    /**
    Fait appel � toutes les fonctions d'affichage (  map, h�ro, menu...)
    G�re aussi le fondu de la minimap.
    \param jeu : class qui contient le jeu, donc le h�ro, la map en cours, ...
    */
    void Affichage(Jeu *jeu);
    void FPS(Jeu *jeu);

    bool continuer,lumiere,augmenter;
    int nbrTourBoucle;
    sf::Text Version,Temps,fps,TourBoucle;
    coordonneeDecimal oldHeroPos;

    float temps[5];
    float tempsActuel,tempsPrecedent,tempsDepuisDerniereAnimation,tempsEcoule,tempsNbrTourBoucle,tempsEcouleDepuisDernierCalculLumiere,tempsEcouleDepuisDernierCalculOmbre,tempsEcouleDepuisDernierDeplacement,tempsEcouleDepuisDernierIA,tempsEcouleDepuisDernierAffichage,tempsEcouleDepuisFPS,tempsEffetMort,tempsSauvergarde;

    float lowFPS;

};

#endif


