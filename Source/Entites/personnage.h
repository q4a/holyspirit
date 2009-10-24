

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



#ifndef PERSONNAGEH
#define PERSONNAGEH

#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

#include <fstream>

#include "../constantes.h"
#include "objet.h"
#include "liste_case.h"
#include "pose.h"
#include "../Moteurs/lightManager.h"
#include "miracle.h"
#include "script.h"

class Modele_Personnage
{
public:
    Modele_Personnage();
    ~Modele_Personnage();
    bool Charger(std::string chemin);
    void ChargerPose(std::ifstream *fichier);

    void Reinitialiser();

    void JouerSon(int numeroSon,coordonnee position,coordonnee positionHero, bool uniqueSound=false);

    const Lumiere &getPorteeLumineuse();
    int getNombreSons();
    int getNombreSonsTouche();
    int getSonTouche(int);
    const Caracteristique &getCaracteristique();

    void setPorteeLumineuse(Lumiere  lumiere);



    std::vector <std::vector<std::vector<Pose> > > m_pose;
    std::vector <int> m_image;

    std::vector<Miracle> m_miracles;

    bool m_ombre;
    bool m_impenetrable;
    bool m_impoussable;

protected:
    Caracteristique m_caracteristique;
    Lumiere m_porteeLumineuse;
    std::vector <int> m_sons;
    std::vector <int> m_sons_touche;
};

class Personnage
{
public:
    Personnage();

    void Sauvegarder(std::ofstream &fichier);

    bool EnVie();
    int Animer(Modele_Personnage *modele,float temps,coordonnee positionHero);

    void Pousser(coordonneeDecimal vecteur);

    bool SeDeplacer(float,coordonnee dimensionsMap, bool pousserPossible = true);

    void Afficher(coordonnee dimensionsMap,Modele_Personnage *modele,bool surbrillance=false, bool sansEffet=false);

    int Pathfinding(casePathfinding** map,coordonnee exception, bool noDelete = false);

    void Frappe(coordonnee direction,coordonnee position);

    void InfligerDegats(float degats, Modele_Personnage *modele);

    int AjouterEffet(std::vector<Tile> &tiles, int type, int compteur, int info1, int info2, int info3);
    void DetruireEffets();


    void setDepart();
    void setErreurPathfinding(bool);
    void setCoordonnee(coordonnee nouvellesCoordonnees);
    void setArrivee(coordonnee arrivee);
    void setMauvaiseArrivee(coordonnee arrivee);
    void setEtat(int  etat);
    void setJustEtat(int etat);
    void setPose(int  pose);
    void setAngle(int  angle);
    void addAngle(int angle);
    void setVitesse(float vitesse);
    void setCoordonneePixel(coordonnee position);
    void setProchaineCase(coordonnee position);
    void setCaracteristique(Caracteristique caracteristique);
    void setPorteeLumineuse(Lumiere  lumiere);
    void regenererVie(float vie);
    void setModele(int);

    void setPousse(coordonneeDecimal pousse);

    void AjouterPointAme(int pointAme);

    int getEtat();
    int getAngle();
    int getPose();
    const Lumiere &getPorteeLumineuse();
    const Caracteristique &getCaracteristique();
    const std::string &getNom();

    const coordonnee &getCoordonnee();
    const coordonnee &getArrivee();
    const coordonneeDecimal &getCoordonneePixel();
    const coordonneeDecimal &getPousse();
    const coordonnee &getProchaineCase();
    const coordonnee &getDepart();
    bool getErreurPathfinding();

    int getOrdre(Modele_Personnage *modele);

    int getModele();

    void setObjets(std::vector<Objet>);
    const std::vector<Objet> &getObjets();
    std::vector<Objet> *getPointeurObjets();


    Lumiere m_lumiere;
    bool frappeEnCours;
    bool m_lancementMiracleEnCours;
    bool m_shooter,m_monstre;
    bool m_touche;
    int m_nombreInvocation;

    Light_Entity m_light;

    int m_miracleALancer;
    std::vector <EntiteMiracle> m_miracleEnCours;

    std::vector <EffetPersonnage> m_effets;

    Script m_scriptAI;
    bool m_friendly;

    bool m_impenetrable;
    bool m_impoussable;
    bool m_doitMourir;

    Personnage* m_vientDeFrapper;
    Personnage* m_vientDetreTouche;
    int         m_degatsInflige;

    Personnage *m_cible;

    int m_ID;

protected:
    int m_etat,m_poseEnCours,m_angle;
    float m_animation;
    bool m_erreurPathfinding;
    Caracteristique m_caracteristique;
    coordonnee m_cheminFinal,m_arrivee,m_ancienneArrivee,m_mauvaiseArrivee, m_positionCase,m_positionAffichage,m_positionPixelPrecedente;
    coordonneeDecimal m_positionPixel;

    int m_modele;
    std::vector<Objet> m_objets;

    Lumiere m_porteeLumineuse,m_porteeLumineuseBasique;

    coordonneeDecimal  m_pousse;
    coordonnee m_depart;

    bool m_stunned;
};


#endif

