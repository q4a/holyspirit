

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

class Hero
{
public:
    Hero();
    ~Hero();

    void Sauvegarder();
    void SauvegarderApercu();
    void Charger(std::string chemin);
    void ChargerModele(bool tout=false);
    bool ChargerPresentation(std::string chemin);

    void Afficher();
    void CalculerOrdreAffichage();

    bool AfficherInventaire(float, std::vector<Objet> *, bool hideLeft);
    void AfficherQuetes(float);
    void AfficherPotales(float);
    void AfficherFlecheQuetes(const std::string &nomMap, float temps);
    bool AfficherMiracles(float , int );
    void AfficherCaracteristiques(float decalage, bool trader);
    void AfficherRaccourcis();
    void AfficherAmis();

    void PlacerCamera();

    bool TestMonstreVise(Personnage *monstre);

    void AugmenterAme(float temps);
    void RecalculerCaracteristiques(bool bis = true);
    void RecalculerGolems();

    //void InfligerDegats(float degats);
    void RegenererVie(float vie);
    void RegenererFoi(float foi);
    void RegenererMiracles(float time);

    void addPotale(int x, int y, int nom, const std::string &chemin);

    bool AjouterObjet(Objet objet,bool enMain=false);

    Objet DeposerObjet();
    bool PrendreEnMain(std::vector<Objet>*, bool craft = false, bool bless = false);
    void delObjet(int numero);

    void GererBless(std::vector<Objet>*);
    void GererCraft(std::vector<Objet>*);

    void GererBoutonsInventaire();

    bool PossibleEquiper(int numero, int emplacement);
    bool Equiper(int numero, int emplacement);
    bool UtiliserObjet(int numero);
    void RangerObjet(int numero);
    const std::string &getNomObjet(int numero);
    int getNombreObjet();

    void UpdateRaccourcis();

    void AutoTrierInventaire();

    bool UtiliserMiracle(int, Personnage*, coordonnee );
    bool AjouterMiracleArme();
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

    //cDAT m_fichierSave;
    std::vector<std::string> m_contenuSave;

    int m_defilement_trader,m_max_defilement_trader,m_buttonPointe;
    Raccourci m_raccourcis[8];

    int m_quetePointee, m_queteSelectionnee;
    int m_potale_selectionnee;
    int m_last_potale;

    std::vector<Quete>  m_quetes;
    std::vector<Potale> m_potales;
    std::vector<int> m_lvl_miracles;

    std::vector<Personnage *> m_amis;

    std::vector<Objet> m_coffre;

    float m_angleFleche;

    std::list<sf::Sprite> m_minimap;

    int m_no_schema;
    int m_no_result;

private:
    coordonnee m_chercherSac,m_sacVise;

    int m_cas;

    int m_ordreAffichage[NOMBRE_MORCEAU_PERSONNAGE];

    std::vector<Objet> m_inventaire;

    std::vector<int> m_weaponMiracle;

    bool m_achat;
};

#endif


