

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

void TrierInventaire(std::vector<Objet> *, int );

class Map
{
public:
    Map();
    ~Map();

    bool Charger(std::string,Hero *hero);
    void Initialiser(Hero *hero);
    void CreerSprite(sf::Vector3f position_case);
    void Sauvegarder(Hero *hero);

    void Afficher(Hero *hero,bool alt,float alpha=255);
    void AfficherSac(coordonnee ,float ,coordonnee ,Caracteristique , std::string);
    void AfficherMinimap(coordonnee position,int typeCase,float alpha);
    void AfficherNomEvenement(coordonnee casePointee,coordonnee positionSouris);

    void Animer(Hero *hero,float temps,Menu *menu); // Animation des tiles

    bool TestEvenement(Jeu *jeu,float temps);
    void CalculerOmbresEtLumieres();
    void Detruire();

    //int GererMiracle(EntiteMiracle *entiteMiracle,Miracle *modeleMiracle,Hero *hero,bool monstre, const coordonnee lanceur, const coordonnee cible,int couche);

    void GererConditions(Jeu *jeu,Script *script,int noInstruction,int monstre,Hero *hero,float temps,Menu *menu, bool seDeplacer);
    void GererInstructions(Jeu *jeu,Script *script,int noInstruction,int monstre,Hero *hero,float temps,Menu *menu, bool seDeplacer);

    void GererEvenements(int evenement,int z,int couche,int x,int y);
    void GererProjectilesEtEffets(Hero *hero,float temps);
    void GererScript(Jeu *jeu,Hero *hero,float temps,Menu *menu);
    void GererMonstres(Jeu *jeu,Hero *hero,float temps,Menu *menu);

    void GererMiracle(Personnage *personnage,std::vector<Miracle> &miracles,float temps,coordonnee positionHero,Hero *hero);

    bool Miracle_Aura           (Hero *, Personnage *, Miracle &, Effet &, EntiteMiracle &, InfosEntiteMiracle &, float , int );
    bool Miracle_Declencheur    (Hero *, Personnage *, Miracle &, Effet &, EntiteMiracle &, InfosEntiteMiracle &, float , int );
    bool Miracle_Effet          (Hero *, Personnage *, Miracle &, Effet &, EntiteMiracle &, InfosEntiteMiracle &, float , int );
    bool Miracle_Charme         (Hero *, Personnage *, Miracle &, Effet &, EntiteMiracle &, InfosEntiteMiracle &, float , int );
    bool Miracle_CorpsACorps    (Hero *, Personnage *, Miracle &, Effet &, EntiteMiracle &, InfosEntiteMiracle &, float , int );
    bool Miracle_Repetition     (Hero *, Personnage *, Miracle &, Effet &, EntiteMiracle &, InfosEntiteMiracle &, float , int );
    bool Miracle_Projectile     (Hero *, Personnage *, Miracle &, Effet &, EntiteMiracle &, InfosEntiteMiracle &, float , int );
    bool Miracle_EffetGraphique (Hero *, Personnage *, Miracle &, Effet &, EntiteMiracle &, InfosEntiteMiracle &, float , int );
    bool Miracle_Invocation     (Hero *, Personnage *, Miracle &, Effet &, EntiteMiracle &, InfosEntiteMiracle &, float , int );
    bool Miracle_Pose           (Hero *, Personnage *, Miracle &, Effet &, EntiteMiracle &, InfosEntiteMiracle &, float , int );
    bool Miracle_Charge         (Hero *, Personnage *, Miracle &, Effet &, EntiteMiracle &, InfosEntiteMiracle &, float , int );
    bool Miracle_EffetEcran     (Hero *, Personnage *, Miracle &, Effet &, EntiteMiracle &, InfosEntiteMiracle &, float , int );
    bool Miracle_Degats         (Hero *, Personnage *, Miracle &, Effet &, EntiteMiracle &, InfosEntiteMiracle &, float , int );
    bool Miracle_Souffle        (Hero *, Personnage *, Miracle &, Effet &, EntiteMiracle &, InfosEntiteMiracle &, float , int );
    bool Miracle_Zone           (Hero *, Personnage *, Miracle &, Effet &, EntiteMiracle &, InfosEntiteMiracle &, float , int );
    bool Miracle_Conditions     (Hero *, Personnage *, Miracle &, Effet &, EntiteMiracle &, InfosEntiteMiracle &, float , int );
    bool Miracle_Bloquer        (Hero *, Personnage *, Miracle &, Effet &, EntiteMiracle &, InfosEntiteMiracle &, float , int );


    void Script_Trade           (Jeu *jeu,Script *script,int noInstruction,int monstre,Hero *hero,float temps,Menu *menu, bool seDeplacer);
    void Script_Potale          (Jeu *jeu,Script *script,int noInstruction,int monstre,Hero *hero,float temps,Menu *menu, bool seDeplacer);
    void Script_Craft           (Jeu *jeu,Script *script,int noInstruction,int monstre,Hero *hero,float temps,Menu *menu, bool seDeplacer);
    void Script_Bless           (Jeu *jeu,Script *script,int noInstruction,int monstre,Hero *hero,float temps,Menu *menu, bool seDeplacer);
    void Script_Fight           (Jeu *jeu,Script *script,int noInstruction,int monstre,Hero *hero,float temps,Menu *menu, bool seDeplacer);
    void Script_Shoot           (Jeu *jeu,Script *script,int noInstruction,int monstre,Hero *hero,float temps,Menu *menu, bool seDeplacer);
    void Script_PlaySound       (Jeu *jeu,Script *script,int noInstruction,int monstre,Hero *hero,float temps,Menu *menu, bool seDeplacer);
    void Script_SetState        (Jeu *jeu,Script *script,int noInstruction,int monstre,Hero *hero,float temps,Menu *menu, bool seDeplacer);
    void Script_UseMiracle      (Jeu *jeu,Script *script,int noInstruction,int monstre,Hero *hero,float temps,Menu *menu, bool seDeplacer);
    void Script_Teleport        (Jeu *jeu,Script *script,int noInstruction,int monstre,Hero *hero,float temps,Menu *menu, bool seDeplacer);
    void Script_Follow          (Jeu *jeu,Script *script,int noInstruction,int monstre,Hero *hero,float temps,Menu *menu, bool seDeplacer);
    void Script_Evasion         (Jeu *jeu,Script *script,int noInstruction,int monstre,Hero *hero,float temps,Menu *menu, bool seDeplacer);
    void Script_RandomDisplace  (Jeu *jeu,Script *script,int noInstruction,int monstre,Hero *hero,float temps,Menu *menu, bool seDeplacer);


    void TestVisionMonstre(int numero, Hero *hero);
    bool TileVisible(int x,int y, coordonnee pos);

    //void InfligerDegatsMasse(coordonnee position,int rayon,int degats,bool sourceConcernee, Hero *hero, bool pousser = true, bool heroCompris = true, bool monstre = true);

    bool InfligerDegats(int numeroMontre, float degats, int type, Hero *hero,bool pousser);
    bool InfligerDegats(Personnage *monstre, float degats, int type, Hero *hero,bool pousser);

    void MusiquePlay(coordonnee position);

    void VerifierDeclencheursDegats(int i, int j);

    bool RamasserObjet(Hero *hero,bool enMain = false);

    void AjouterObjet(Objet objet);
    int  AjouterProjectile(coordonneeDecimal positionReel,coordonnee cible,coordonnee lanceur,int couche,float  vitesse,float decalageAngle,bool monstre,Tileset *tileset);
    void AjouterMonstre(Monstre monstre);
    void AjouterModeleMonstre(Modele_Monstre modele);

    int     getMonstreIllumine();
    bool    getCollision(int positionX,int positionY, int exception = -1); // Retourne 1 s'il y a une collision avec le décors se trouvant à la position X et Y
    int     getTypeCase(int positionX,int positionY);

    int     getEvenement(coordonnee casePointee);
    int     getMonstre(Hero *hero,coordonnee positionSouris,coordonnee casePointee);
    const   coordonnee &getSacPointe();
    int     getObjetPointe();
    int     getNombreObjets(coordonnee position);
    int     getNombreMonstres();
    int     getNombreModeleMonstres();
    bool    getMonstreEnVie(int numeroMonstre);
    const   coordonnee &getDimensions();
    const   std::string &getNom();

    bool    getCollisionPousse(int positionX,int positionY, int id = -1);
    void    TesterPoussable(Personnage &, float temps, int id = -1);

    Monstre *getEntiteMonstre (int numeroMonstre);
    const Modele_Monstre &getModeleMonstre(int numeroMonstre);
    const coordonnee &getPositionMonstre(int numeroMonstre);
    casePathfinding ** getAlentourDuPersonnage(coordonnee positionPersonnage); // Retourne un tableau de bool contenant toutes les collitions dans les alentour du héro, pour le pathfinding

    int m_defilerObjets,m_objetPointe,m_monstreIllumine;
    /*sf::Music m_musique[MAX_MUSIQUE];
    int m_musiqueEnCours,m_nombreMusique;*/
    std::vector<std::string> m_musiques;
    int m_musiqueEnCours;

private:
    int IDImageSac,m_calculLumiere;
    std::string m_nom_fichier;

    coordonnee m_dimensions;

    coordonnee m_sacPointe;
    Lumiere m_lumiere[24];
    std::vector <int> m_tileset;
    std::vector <int> m_herbe;
    std::vector <std::vector < Decor > >  m_decor[NOMBRE_COUCHE_MAP];
    std::vector <Evenement> m_evenement;


    std::string m_nom;
    int m_no_nom;

    int m_img_sky;
    std::string m_nom_img_sky;

    std::vector <std::string> m_fond;

    std::vector <Modele_Monstre> m_ModeleMonstre;
    std::vector <Monstre> m_monstre;
    std::vector <Projectile> m_projectile;
    std::vector <EffetGraphique> m_effets;

    std::vector<Decor>::iterator IterY;
    std::vector<Decor>::iterator Iter;

    std::vector< std::vector<int> > m_listID;

    Script m_script;
};


#endif

