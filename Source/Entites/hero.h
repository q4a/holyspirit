

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





#ifndef HEROH
#define HEROH

#include <iostream>
#include <string>
#include <list>

#include "classe.h"
#include "personnage.h"
#include "monstre.h"
#include "quete.h"

bool AjouterObjetInventaire(Objet newObj, std::vector<Objet>* inventaire, coordonnee taille, bool infini);

struct Potale
{
    coordonnee position;
    std::string chemin;
    int nom;
};

struct Raccourci
{
    bool miracle;
    int no;
    int nbr;
};

struct Item_Used
{
    Item_Used()
    {
        time = 0;
        max_time = 0;
    }

    std::string name;
    float       time;
    float       max_time;
};

class Hero
{
public:
    Hero();
    ~Hero();

    void Reset();

    void Sauvegarder();
    void SauvegarderApercu();
    void Charger(const std::string &chemin);
    void ChargerModele(bool tout=false);
    bool ChargerPresentation(const std::string &chemin);

    void Afficher();
    void CalculerOrdreAffichage();

    bool AfficherInventaire(float, std::vector<Objet> *, bool hideLeft, bool bless = false, bool craft = false);
    void AfficherQuetes(float);
    void AfficherDocs(float);
    void AfficherPotales(float);
    bool AfficherMiracles(float , int );
    void AfficherCaracteristiques(float decalage, bool trader);
    void AfficherRaccourcis();
    void AfficherAmisEtCraft();

    void PlacerCamera();

    bool TestMonstreVise(Personnage *monstre);

    void GererTemps(float temps);
    void RecalculerCaracteristiques(bool bis = true);
    void RecalculerGolems();

    void RegenererVie(float vie);
    void RegenererFoi(float foi);
    void RegenererMiracles(float time);

    void addPotale(int x, int y, int nom, const std::string &chemin);

    bool AjouterObjet(Objet &objet,bool enMain=false);

    Objet DeposerObjet();
    bool PrendreEnMain(std::vector<Objet>*, bool craft = false, bool bless = false);
    void delObjet(int numero);

    void GererBless();
    void GererCraft(std::vector<Objet>*);

    bool PossibleEquiper(int numero, int emplacement);
    bool PossibleEquiper(Objet  &, int emplacement);
    bool Equiper(int numero, int emplacement);
    bool UtiliserObjet(int numero);
    bool RangerObjet(int numero);
    const std::string &getNomObjet(int numero);
    int getNombreObjet();

    void UpdateRaccourcis();

    void AutoTrierInventaire();
    void AutoTrierCoffre();

    bool UtiliserMiracle(int, Personnage*, coordonnee );
    void StopMiraclesFrappe();
    void StopMiraclesCharme();

    void setChercherSac(coordonnee);
    void setSacVise(coordonnee);

    coordonnee getChercherSac();
    coordonnee getSacVise();

    Lumiere getPorteeLumineuse();

    std::string m_cheminClasse;
    std::string m_chemin_save;
    Classe m_classe;

    Personnage m_personnage;
    Modele_Personnage m_modelePersonnage[NOMBRE_MORCEAU_PERSONNAGE];
    std::string m_cheminModele[NOMBRE_MORCEAU_PERSONNAGE],m_cheminModeleNouveau[NOMBRE_MORCEAU_PERSONNAGE];

    Caracteristique m_caracteristiques;

    int miracleEnCours,frappeEnCours,m_objetEnMain,m_objetADeposer,m_objetVise,m_miracleEnMain;
    int m_argent;
    int m_holywater;

    int m_miracle_gauche;

    //cDAT m_fichierSave;
    std::vector<std::string> m_contenuSave;

    int m_defilement_trader,m_max_defilement_trader;
    Raccourci m_raccourcis[8];

    int m_quetePointee, m_queteSelectionnee;
    int m_defil_quetes;
    int m_docPointe, m_docSelectionne;
    int m_defil_cdoc, m_defil_ldoc;
    int m_max_defil_cdoc, m_max_defil_ldoc;
    int m_potale_selectionnee;
    int m_last_potale;

    std::vector<Quete>  m_quetes;
    std::vector<Document>  m_docs;
    std::vector<Potale> m_potales;
    std::vector<int> m_lvl_miracles;

    std::vector<Personnage *> m_amis;

    std::vector<Objet> m_coffre;

    float m_angleFleche;

    int m_no_schema_craft;
    int m_no_schema_bless;
    int m_no_result_craft;
    int m_no_result_bless;

    coordonnee m_case_visee;

    //bool m_golem_action;

    int m_select_friend;

    std::vector <Item_Used> m_items_cooldown;

    bool newQuest, newDoc;

    int m_cas;

private:
    coordonnee m_chercherSac,m_sacVise;


    int m_ordreAffichage[NOMBRE_MORCEAU_PERSONNAGE];

    std::vector<Objet> m_inventaire;

    std::vector<int> m_weaponMiracle;

    bool m_achat;

    float m_craft_time;
    float m_bless_time;
    float m_craft_time_max;
    float m_bless_time_max;

    bool m_trier_en_hauteur;
};

#endif


