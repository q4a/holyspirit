

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

#ifndef MAPH
#define MAPH


#include "../Entites/hero.h"
#include "tileset.h"
#include "decor.h"
#include "../menu.h"
#include "../Entites/monstre.h"
#include "evenement.h"

class Jeu;

class Map
{
public:
    Map();
    ~Map();

    bool Charger(std::string,Hero *hero);
    void Initialiser();
    void CreerSprite(sf::Vector3f position_case);
    void Sauvegarder(Hero *hero);

    void Afficher(Hero *hero,coordonnee positionSouris,bool alt,float alpha=255);
    void AfficherSac(coordonnee positionSac,float decalage,coordonnee positionSouris,coordonnee positionEcranSac,Caracteristique caract);
    void AfficherMinimap(coordonnee position,int typeCase,float alpha);
    void AfficherNomEvenement(coordonnee casePointee,coordonnee positionSouris);

    void Animer(Hero *hero,float temps,Menu *menu); // Animation des tiles
    bool TestEvenement(Jeu *jeu,float temps);
    void CalculerOmbresEtLumieres();
    void Detruire();

    int GererMiracle(EntiteMiracle *entiteMiracle,Miracle *modeleMiracle,Hero *hero,bool monstre,coordonnee lanceur, coordonnee cible,int couche);
    void GererConditions(Jeu *jeu,Script *script,int noInstruction,int i, int j, int k,Hero *hero,float temps,Menu *menu);
    void GererEvenements(int evenement,int z,int couche,int x,int y);
    void GererMonstres(Jeu *jeu,Hero *hero,float temps,Menu *menu);

    void InfligerDegatsMasse(coordonnee position,int rayon,int degats,bool sourceConcernee, Hero *hero,Menu *menu);

    bool InfligerDegats(int numeroMontre, float degats,Menu *menu, Hero *hero,bool pousser);
    void PousserMonstre(int numeroMonstre, coordonnee vecteur);
    void PousserMonstreCase(int numeroMonstre, coordonnee vecteur);

    void MusiquePlay(coordonnee position);

    void VerifierDeclencheursDegats(int i, int j);

    bool RamasserObjet(Hero *hero,bool enMain = false);

    void AjouterObjet(Objet objet);
    int AjouterProjectile(coordonneeDecimal positionReel,coordonnee cible,coordonnee lanceur,int couche,float  vitesse,float decalageAngle,int degats,bool monstre,Tile *tile, int image);




    int getMonstreIllumine();
    bool getCollision(int positionX,int positionY); // Retourne 1 s'il y a une collision avec le décors se trouvant à la position X et Y
    int getTypeCase(int positionX,int positionY);
    int getEvenement(coordonnee casePointee);
    int getMonstre(Hero *hero,coordonnee positionSouris,coordonnee casePointee);
    coordonnee getSacPointe();
    int getObjetPointe();
    int getNombreObjets(coordonnee position);
    int getNombreMonstres();
    bool getMonstreEnVie(int numeroMonstre);
    coordonnee getDimensions();

    Monstre *getEntiteMonstre (int numeroMonstre);
    coordonnee getPositionMonstre(int numeroMonstre);
    casePathfinding ** getAlentourDuPersonnage(coordonnee positionPersonnage); // Retourne un tableau de bool contenant toutes les collitions dans les alentour du héro, pour le pathfinding

    void setVolumeMusique(int volume);



    int m_defilerObjets;

private:
    int m_monstreIllumine,IDImageSac,m_objetPointe,m_calculLumiere,m_musiqueEnCours,m_nombreMusique;
    std::string m_nom_fichier;

    coordonnee m_dimensions;

    coordonnee m_sacPointe;
    Lumiere m_lumiere[24];
    std::vector <Tileset> m_tileset;
    std::vector <Herbe> m_herbe;
    Decor ***m_decor;
    std::vector < std::vector < Decor > >  decorBuffer[NOMBRE_COUCHE_MAP];
    std::vector <Evenement> m_evenement;

    sf::Music m_musique[MAX_MUSIQUE];
    std::string m_nom;

    std::vector <std::string> m_fond,m_cheminMusique;

    std::vector <Modele_Monstre> m_ModeleMonstre;
    std::vector <Monstre> m_monstre;
    std::vector <Projectile> m_projectile;
    std::vector <EffetGraphique> m_effets;

    std::vector<Decor>::iterator IterY;
    std::vector<Decor>::iterator Iter;
};


#endif

