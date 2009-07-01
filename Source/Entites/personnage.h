

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

#include "../constantes.h"
#include "objet.h"
#include "liste_case.h"
#include "pose.h"
#include "../Moteurs/lightManager.h"
#include "miracle.h"


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
    const Caracteristique &getCaracteristique();

    void setPorteeLumineuse(Lumiere  lumiere);



    std::vector <std::vector<std::vector<Pose> > > m_pose;
    std::vector <int> m_image;

    std::vector<Miracle> m_miracles;

    bool m_ombre,m_explosif;

protected:
    Caracteristique m_caracteristique;
    Lumiere m_porteeLumineuse;
    std::vector <int> m_sons;
};

class Personnage
{
public:
    Personnage();



    const coordonnee &getCoordonnee();
    const coordonnee &getArrivee();
    const coordonneeDecimal &getCoordonneePixel();
    const coordonnee &getProchaineCase();
    bool getErreurPathfinding();
    bool enVie();
    int animer(Modele_Personnage *modele,float temps,bool *explosif,coordonnee positionHero);

    void Pousser(coordonnee vecteur);
    void PousserCase(coordonnee vecteur);

    bool seDeplacer(float,coordonnee dimensionsMap);
    void setCoordonnee(coordonnee nouvellesCoordonnees);
    void Afficher(coordonnee dimensionsMap,Modele_Personnage *modele,bool surbrillance=false);

    int pathfinding(casePathfinding** map,coordonnee exception);

    void frappe(coordonnee direction,coordonnee position);


    void infligerDegats(float degats);

    void setErreurPathfinding(bool);

    void setArrivee(coordonnee arrivee);
    void setMauvaiseArrivee(coordonnee arrivee);
    void setEtat(int  etat);
    void setJustEtat(int etat);
    void setPose(int  pose);
    void setAngle(int  angle);
    void setVitesse(float vitesse);
    void setCoordonneePixel(coordonnee position);
    void setProchaineCase(coordonnee position);
    void setCaracteristique(Caracteristique caracteristique);
    void setPorteeLumineuse(Lumiere  lumiere);
    void regenererVie(float vie);

    void AjouterPointAme(int pointAme);

    int getEtat();
    int getAngle();
    int getPose();
    const Lumiere &getPorteeLumineuse();
    const Caracteristique &getCaracteristique();
    const std::string &getNom();

    int getOrdre(Modele_Personnage *modele);

    LumiereOmbrage m_lumiere;
    bool frappeEnCours;
    bool m_shooter,m_monstre;
    bool m_touche;
    int m_nombreInvocation;

    Light_Entity m_light;

    int m_miracleALancer;
    std::vector <EntiteMiracle> m_miracleEnCours;

protected:
    int m_etat,m_poseEnCours,m_angle;
    float m_animation;
    bool m_erreurPathfinding;
    Caracteristique m_caracteristique;
    coordonnee m_cheminFinal,m_arrivee,m_ancienneArrivee,m_mauvaiseArrivee, m_positionCase,m_positionAffichage,m_positionPixelPrecedente;
    coordonneeDecimal m_positionPixel;

    Lumiere m_porteeLumineuse,m_porteeLumineuseBasique;
};


#endif

