

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

#include "classe.h"
#include "personnage.h"
#include "monstre.h"

bool AjouterObjetInventaire(Objet newObj, std::vector<Objet>* inventaire, coordonnee taille, bool infini);

class Hero
{
public:
    Hero();
    ~Hero();

    void Sauvegarder();
    void Charger();
    void ChargerModele(bool tout=false);

    void Afficher(coordonnee dimensionsMap);
    void CalculerOrdreAffichage();

    void AfficherInventaire(float,std::vector<Objet>);
    void AfficherCaracteristiques(float decalage);
    void AfficherRaccourcis();

    //void GenererGrille();
    void PlacerCamera(coordonnee dimensionsMap);

    bool TestMonstreVise(Monstre *monstre);

    void AugmenterAme(float temps);
    void RecalculerCaracteristiques();

    void InfligerDegats(float degats);
    void RegenererVie(float vie);
    void RegenererFoi(float foi);

    int UtiliserClicDroit(int monstreVise);

    bool AjouterObjet(Objet objet,bool enMain=false);

    Objet DeposerObjet();
    bool PrendreEnMain(std::vector<Objet>*);

    bool PossibleEquiper(int numero, int emplacement);
    bool Equiper(int numero, int emplacement);
    bool UtiliserObjet(int numero);
   // void AttribuerPositionObjet(coordonnee position,int numero);
    void RangerObjet(int numero);

    bool AjouterMiracleArme();
    //void LibererCases(int numero);

    void setMonstreVise(int monstre);
    void setChercherSac(coordonnee);
    void setSacVise(coordonnee);

    int getMonstreVise();
    coordonnee getChercherSac();
    coordonnee getSacVise();

    Lumiere getPorteeLumineuse();

    std::string m_cheminClasse;
    Classe m_classe;

    Personnage m_personnage;
    Modele_Personnage m_modelePersonnage[NOMBRE_MORCEAU_PERSONNAGE];
    std::string m_cheminModele[NOMBRE_MORCEAU_PERSONNAGE],m_cheminModeleNouveau[NOMBRE_MORCEAU_PERSONNAGE];

    Caracteristique m_caracteristiques;

    int miracleEnCours,frappeEnCours,m_objetEnMain,m_objetADeposer,m_objetVise;
    int m_argent;

    //cDAT m_fichierSave;
    std::vector<std::string> m_contenuSave;

    int m_defilement_trader,m_max_defilement_trader,m_buttonPointe;
    int m_objets_raccourcis[4];

private:
    coordonneeDecimal m_positionAffichage;
    int m_monstreVise;
    coordonnee m_chercherSac,m_sacVise;

    int m_cas;

    int ordreAffichage[NOMBRE_MORCEAU_PERSONNAGE];

    std::vector<Objet> m_inventaire;
   // std::vector< std::vector<bool> > m_caseInventaire;

    int m_weaponMiracle;
    bool m_achat;
};

#endif


