

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
#include "../Moteurs/lightManager.h"
#include "miracle.h"
#include "script.h"
#include "../Map/tileset.h"

struct Degats
{
    Degats (float tps, float dgt, int t) : temps(tps), temps_ecoule(0), degats(dgt), type(t) {}
    float   temps;
    float   temps_ecoule;
    float   degats;
    int     type;
};

struct Emulate_pos
{
    coordonneeDecimal getCurPos()
    {
        coordonneeDecimal t;
        t.x = prevPos.x + (nextPos.x - prevPos.x) * time / maxTime;
        t.y = prevPos.y + (nextPos.y - prevPos.y) * time / maxTime;
        t.h = prevPos.h + (nextPos.h - prevPos.h) * time / maxTime;
        return t;
    }

    float time;
    float maxTime;
    coordonneeDecimal nextPos;
    coordonneeDecimal prevPos;

    int etat;
    int angle,pose;

    Caracteristique caract;
};

class Modele_Personnage
{
public:
    Modele_Personnage();
    virtual ~Modele_Personnage();
    bool Charger(std::string chemin);

    void Reinitialiser();

    const Caracteristique &getCaracteristique();

    std::vector<std::vector<Tileset> > m_tileset;

    std::vector<Miracle> m_miracles;

    bool m_ombre;
    bool m_impenetrable;
    bool m_impoussable;
    bool m_collision;
    bool m_selectable;
    bool m_noDistanceRestriction;

    std::string m_chemin;

    int m_materiau;

protected:
    Caracteristique m_caracteristique;
};

class Personnage
{
public:
    Personnage();
    virtual ~Personnage();

    void Sauvegarder(std::ofstream &fichier);

    bool EnVie();

    int     Animer(Modele_Personnage *modele,float temps, bool = false);
    int     Gerer(Modele_Personnage *modele,float temps, bool = false);
    void    GererEffets(float temps);
    void    GererDegats(float temps);

    void Pousser(coordonneeDecimal vecteur);

    bool SeDeplacer(float);
    void EmulerDeplacement(float);

    void Afficher(Modele_Personnage *modele, Border &border,bool surbrillance=false, bool plusHaut=false, bool plusBas=false);

    int Pathfinding(casePathfinding** map, bool noDelete = false);

    void Frappe(const coordonnee &direction,const coordonnee &position);

    void InfligerDegats(float degats, int type, float temps = 0);

    int AjouterEffet(Tileset *tileset, int type, int compteur, int info1, int info2, int info3, int colorr, int colorg, int colorb, int colora);
    void DetruireEffets();
    void RecalculerEffets();
    void DesactiverEffets();
    void ActiverEffets();

    void Kill();


    void setDepart();
    void setErreurPathfinding(bool);
    void setCoordonnee(const coordonnee &nouvellesCoordonnees);
    void setArrivee(const coordonnee &arrivee);
    void setMauvaiseArrivee(const coordonnee &arrivee);
    void setEtat(int  etat, int pose = -1);
    void setJustEtat(int etat);
    void setPose(int  pose);
    void setAngle(int  angle);
    void setForcedAngle(int  angle);
    void addAngle(int angle);
    void setVitesse(float vitesse);
    void setCoordonneePixel(const coordonnee &position);
    void setCoordonneePixel2(const coordonneeDecimal &position);
    void setEmulatePos(const coordonneeDecimal &positionD, int etat, int pose, int angle, Caracteristique &caract, float time);
    void setProchaineCase(const coordonnee &position);
    void setCaracteristique(const Caracteristique &caracteristique);
    void regenererVie(float vie);
    void setModele(int);

    void ClearEmulate();

    void setPousse(const coordonneeDecimal &pousse, bool forced = false);

    void AjouterPointAme(int pointAme);

    int getEtat();
    int getAngle();
    int getNextAngle();
    int getPose();
    const Caracteristique &getCaracteristique();
    const std::string &getNom();

    const coordonnee &getCoordonnee();
    const coordonnee &getArrivee();
    const coordonneeDecimal &getCoordonneePixel();
    coordonneeDecimal getEstimationMouvement();
    const coordonneeDecimal &getPousse();
    const coordonnee &getProchaineCase();
    const coordonnee &getDepart();
    bool getErreurPathfinding();
    float getTime();

    int getOrdre(Modele_Personnage *modele);

    int getModele();

    void setObjets(std::vector<Objet>);
    const std::vector<Objet> &getObjets();
    std::vector<Objet> *getPointeurObjets();


    Lumiere m_lumiere;
    bool frappeEnCours;
    bool m_lancementMiracleEnCours;
    bool m_miracleFrappeEnCours;
    bool m_shooter,m_monstre;
    bool m_touche;
    int m_nombreInvocation;

    int m_miracleALancer;
    std::vector <EntiteMiracle> m_miracleEnCours;

    std::vector <EffetPersonnage> m_effets;

    Script m_scriptAI;
    bool m_friendly;
    bool m_actif;

    bool m_impenetrable;
    bool m_collision;
    bool m_impoussable;
    bool m_doitMourir;

    bool m_inexistant;
    bool m_selectable;
    bool m_noDistanceRestriction;

    Personnage* m_vientDeFrapper;
    Personnage* m_vientDeToucher;
    Personnage* m_vientDetreTouche;
    coordonnee  m_vientDAttaquer;
    int         m_degatsInflige;

    Personnage *m_cible;

    int m_ID;
    bool m_etatForce;
    bool m_miracleBloquant;

    Entite_graphique m_entite_graphique;
    Entite_graphique m_entite_graphique_shadow;

    std::vector<Degats> m_degats;
    std::vector<int>    m_variables_miracles;


    std::string     m_speak;
    float           m_speak_time;

    int             m_materiau;

    bool            m_heroic;

    int             m_no;
    bool dejaMort;

protected:
    int m_etat,m_angle,m_next_angle;
    float m_time_next_angle;
    bool m_erreurPathfinding;
    Caracteristique m_caracteristique;
    coordonnee m_cheminFinal,m_arrivee,m_ancienneArrivee,m_mauvaiseArrivee, m_positionCase,m_positionAffichage,m_positionPixelPrecedente;
    coordonneeDecimal m_positionPixel, m_positionPixelNext,m_positionPixelPrev;
    sf::Clock m_clock;
    float m_deplacement_time, m_cur_d_time;

    int m_modele;
    std::vector<Objet> m_objets;

    coordonneeDecimal  m_pousse;
    coordonnee m_depart;
    bool m_pousseEnCours;

    bool m_stunned;

    std::vector<Emulate_pos> m_emul_pos;
    bool notyetemulate;
};


#endif

