

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
#include "Climate.h"

class Jeu;

int TrierInventaire(std::vector<Objet> *, int , bool = false);

const int MINIMAP_SIZE = 10;

class Map
{
public:
    Map();
    ~Map();

    bool Charger(std::string,Hero *hero);
    void ChargerInfos(std::ifstream *, const std::string &chemin);
    void ChargerEntites(std::ifstream *, const std::string &chemin);
    void ChargerCases(std::ifstream *, const std::string &chemin, bool ,Hero *hero);
    void AddMonstre(std::vector <int> &, std::vector <int> &, std::vector <Entite_graphique> &, Entite_graphique &, coordonnee &position,
                    int &, int &, int );

    void Initialiser(Hero *hero);
    void InitialiserMinimap();
    void CreerSprite(sf::Vector3f position_case);
    void Sauvegarder(Hero *hero);

    void Afficher(Hero *hero,std::list<Hero> &players,bool alt,float alpha=255);
    void AfficherSac(coordonnee ,float ,coordonnee ,Caracteristique , std::string, Border &);

    void Animer(Hero *hero,float temps);

    void CalculerOmbresEtLumieres();
    void Detruire();

    void DelEffet(int no);

    void GererConditions(Jeu *jeu,Script *script,int noInstruction,int monstre,Hero *hero,float temps,Menu *menu, bool seDeplacer);
    void GererInstructions(Jeu *jeu,Script *script,int noInstruction,int monstre,Hero *hero,float temps,Menu *menu, bool seDeplacer);

    void GererProjectilesEtEffets(Jeu *jeu,Hero *hero,float temps);
    void GererScript(Jeu *jeu,Hero *hero,float temps,Menu *menu);
    void GererMonstres(Jeu *jeu,Hero *hero,float temps,Menu *menu);

    void GererMiracle(Personnage *personnage,std::vector<Miracle> &miracles,float temps,Jeu *jeu);

    bool Miracle_Aura           (Jeu *, Personnage *, Miracle &, Effet &, EntiteMiracle &, InfosEntiteMiracle &, float , int , bool);
    bool Miracle_Declencheur    (Jeu *, Personnage *, Miracle &, Effet &, EntiteMiracle &, InfosEntiteMiracle &, float , int , bool);
    bool Miracle_Effet          (Jeu *, Personnage *, Miracle &, Effet &, EntiteMiracle &, InfosEntiteMiracle &, float , int , bool);
    bool Miracle_Charme         (Jeu *, Personnage *, Miracle &, Effet &, EntiteMiracle &, InfosEntiteMiracle &, float , int , bool);
    bool Miracle_CorpsACorps    (Jeu *, Personnage *, Miracle &, Effet &, EntiteMiracle &, InfosEntiteMiracle &, float , int , bool);
    bool Miracle_Repetition     (Jeu *, Personnage *, Miracle &, Effet &, EntiteMiracle &, InfosEntiteMiracle &, float , int , bool);
    bool Miracle_Projectile     (Jeu *, Personnage *, Miracle &, Effet &, EntiteMiracle &, InfosEntiteMiracle &, float , int , bool);
    bool Miracle_EffetGraphique (Jeu *, Personnage *, Miracle &, Effet &, EntiteMiracle &, InfosEntiteMiracle &, float , int , bool);
    bool Miracle_Invocation     (Jeu *, Personnage *, Miracle &, Effet &, EntiteMiracle &, InfosEntiteMiracle &, float , int , bool);
    bool Miracle_Pose           (Jeu *, Personnage *, Miracle &, Effet &, EntiteMiracle &, InfosEntiteMiracle &, float , int , bool);
    bool Miracle_Charge         (Jeu *, Personnage *, Miracle &, Effet &, EntiteMiracle &, InfosEntiteMiracle &, float , int , bool);
    bool Miracle_EffetEcran     (Jeu *, Personnage *, Miracle &, Effet &, EntiteMiracle &, InfosEntiteMiracle &, float , int , bool);
    bool Miracle_Degats         (Jeu *, Personnage *, Miracle &, Effet &, EntiteMiracle &, InfosEntiteMiracle &, float , int , bool);
    bool Miracle_Souffle        (Jeu *, Personnage *, Miracle &, Effet &, EntiteMiracle &, InfosEntiteMiracle &, float , int , bool);
    bool Miracle_Zone           (Jeu *, Personnage *, Miracle &, Effet &, EntiteMiracle &, InfosEntiteMiracle &, float , int , bool);
    bool Miracle_Conditions     (Jeu *, Personnage *, Miracle &, Effet &, EntiteMiracle &, InfosEntiteMiracle &, float , int , bool);
    bool Miracle_Bloquer        (Jeu *, Personnage *, Miracle &, Effet &, EntiteMiracle &, InfosEntiteMiracle &, float , int , bool);
    bool Miracle_Variable       (Jeu *, Personnage *, Miracle &, Effet &, EntiteMiracle &, InfosEntiteMiracle &, float , int , bool);
    bool Miracle_Interchangement(Jeu *, Personnage *, Miracle &, Effet &, EntiteMiracle &, InfosEntiteMiracle &, float , int , bool);
    bool Miracle_Teleport       (Jeu *, Personnage *, Miracle &, Effet &, EntiteMiracle &, InfosEntiteMiracle &, float , int , bool);


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


    void TestVisionMonstre(int numero, Hero *hero,std::list<Hero> &players);
    bool TileVisible(int x,int y, coordonnee pos);

    bool DistanceWithHeros(Jeu *jeu, coordonnee , int);

    //void InfligerDegatsMasse(coordonnee position,int rayon,int degats,bool sourceConcernee, Hero *hero, bool pousser = true, bool heroCompris = true, bool monstre = true);

    bool InfligerDegats(int numeroMontre, Personnage *cible, float degats, int type, Jeu *jeu, float temps = 0);
    bool InfligerDegats(Personnage *monstre, Personnage *cible, float degats, int type, Jeu *jeu, float temps = 0);
    void KillMonstre(Personnage *monstre, int angle, float degats, Jeu *jeu);

    void MusiquePlay();
    void GererAmbiance(float temps);

    bool RamasserObjet(Hero *hero,bool enMain = false);

    void AjouterObjet(Objet objet);
    int  AjouterProjectile(coordonneeDecimal positionReel,coordonneeDecimal cible,coordonnee lanceur,int couche,float  vitesse,float decalageAngle,bool monstre,Tileset *tileset);
    void AjouterMonstre(Monstre monstre);
    void AjouterModeleMonstre(Modele_Monstre modele);

    int     getMonstreIllumine();
    bool    getCollision(int positionX,int positionY, int exception = -1); // Retourne 1 s'il y a une collision avec le décors se trouvant à la position X et Y
    int     getTypeCase(int positionX,int positionY);

    int     getMonstre(coordonnee casePointee);
    const   coordonnee &getSacPointe();
    int     getObjetPointe();
    int     getNombreObjets(coordonnee position);
    int     getNombreMonstres();
    int     getNombreModeleMonstres();
    const   coordonnee &getDimensions();
    const   std::string &getNom();

    void    DeplacerMonstreCase(int,int,int);

    bool    getCollisionPousse(int positionX,int positionY, int id = -1);
    void    TesterPoussable(Personnage &, float temps, int id = -1);

    Monstre *getEntiteMonstre (int numeroMonstre);
    Modele_Monstre &getModeleMonstre(int numeroMonstre);
    const coordonnee &getPositionMonstre(int numeroMonstre);
    casePathfinding ** getAlentourDuPersonnage(Jeu *jeu, coordonnee positionPersonnage, bool monstre); // Retourne un tableau de bool contenant toutes les collitions dans les alentour du héro, pour le pathfinding

    int m_defilerObjets,m_objetPointe,m_monstreIllumine;
    std::vector<std::string> m_musiques;
    int m_musiqueEnCours;
    std::vector<int> m_ambiances;
    std::vector<float> m_volumeAmbiances;
    std::vector<bool> m_playAmbiances, m_startPlayAmbiances;
    std::vector<std::string> m_ambiancesPath;

    std::vector<Climate> m_climates;

    int m_etat_chargement;
    bool m_loaded;

private:
    int m_calculLumiere;
    std::string m_nom_fichier;

    coordonnee m_dimensions;

    coordonnee m_sacPointe;
    Lumiere m_lumiere[24];
    std::vector <int> m_tileset;
    std::vector <int> m_herbe;
    std::vector <std::vector < Decor > >  m_decor[NOMBRE_COUCHE_MAP];


    std::string m_nom;
    int m_no_nom;

    int m_img_sky;
    std::string m_nom_img_sky;

    std::vector <std::string> m_fond;

    std::vector <Modele_Monstre> m_ModeleMonstre;
    std::vector <Monstre> m_monstre;
    std::vector <Projectile> m_projectile;
    std::vector <EffetGraphique> m_effets;

    sf::RenderTexture m_render_minimap[MINIMAP_SIZE][MINIMAP_SIZE];
    //sf::Texture       m_minimap;

    std::vector<Decor>::iterator IterY;
    std::vector<Decor>::iterator Iter;

    std::vector< std::vector<int> > m_listID;

    Script m_script;
};


#endif

