

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



#ifndef _JEUH
#define _JEUH

#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>

#include "contexte.h"
#include "../globale.h"

//! Contexte principal du jeu. Il gère à peu près tout ce qui concerne le jeu en in-game.
/**
Gère l'IA, les déplacements, les animations, l'affichage...
*/
class c_Jeu : public Contexte
{
public:
    //! Constructeur
    /**
    Initialise tout une série de variables :
    L'heure, qui prend une valeur aléatoire
    Tous les compteurs de temps à 0
    Initialise les écritures pour les FPS
    */
    c_Jeu();

    //! Utilise le contexte.
    /**
    C'est le coeur du contexte.
    Il fait appelle à toutes ses méthodes.
    \param jeu : class qui contient le jeu, donc le héro, la map en cours, ...
    */
    void Utiliser(Jeu *jeu);

    float alpha_map, alpha_sac, alpha_dialog;

    sf::Thread *m_thread_sauvegarde;

private:

    //! Incrémente les compteurs de temps avec le temps écoulé depuis la dernière boucle
    /**
    Va aussi s'occuper de faire appelle aux fonctions pour régénerer la vie, la foi, les points d'ames
    Et gerer le taux du post effect pour quand la vie est presque à 0
    \param jeu : class qui contient le jeu, donc le héro, la map en cours, ...
    */
    void GererTemps(Jeu *jeu);


    //! Gère l'IA des monstres
    /**
    Fait simplement appelle à la fonction de gestion de l'IA des monstres
    \param jeu : class qui contient le jeu, donc le héro, la map en cours, ...
    */
    void IA(Jeu *jeu);

    //! Gère les déplacements du héro
    /**
    Gère les déplacements du héro
    Regarde quel monstre est visé par la souris, plus frappe si SHIFT
    Placement de l'écouteur
    \param jeu : class qui contient le jeu, donc le héro, la map en cours, ...
    */
    void Deplacements(Jeu *jeu);

    //! Gère l'animation des décors, monstres et du héro
    /**
    Donc gère l'animation des entités et décors
    Gère les miracles du héro
    Gère les dégats infligés par le héro
    \param jeu : class qui contient le jeu, donc le héro, la map en cours, ...
    */
    void Animation(Jeu *jeu);

    //! Calcul la lumière du héro et le timing de rafraichissement des lumières
    /**
    \param jeu : class qui contient le jeu, donc le héro, la map en cours, ...
    */
    void Lumieres(Jeu *jeu);

    //! Gères les événements au clavier et à la souris
    /**
    Va gérer tous ce qui à trait aux événements, dire quel sera la case d'arrivée du héro, s'il faut afficher l'inventaire,
    l'objet à ramasser...
    \param jeu : class qui contient le jeu, donc le héro, la map en cours, ...
    */
    void Evenements(Jeu *jeu);

    //! Affiche le jeu
    /**
    Fait appel à toutes les fonctions d'affichage (  map, héro, menu...)
    Gère aussi le fondu de la minimap.
    \param jeu : class qui contient le jeu, donc le héro, la map en cours, ...
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


