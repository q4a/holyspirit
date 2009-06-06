

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




#ifndef CONSTANTEH
#define CONSTANTEH

#define COTE_TILE 71.5542
#define DIVISEUR_COTE_TILE 0.013975420031249039189872851628556

#define NOMBRE_ETAT 4
#define CALCUL_PA_PROCHAIN_NIVEAU temp.niveau*temp.niveau*temp.niveau*10

#define NOMBRE_SANG 7

#define MAX_MUSIQUE 5

#define NOMBRE_TYPE_OBJET 2

#define NOMBRE_TXT_MENU 13

#define NOMBRE_SONS 128

#define NOMBRE_COUCHE_MAP 2

#define NOMBRE_MORCEAU_PERSONNAGE 5

#include <cmath>
#include <utility>
#include <iostream>
#include <fstream>
#include <vector>

#include "datFile.h"

#include "Moteurs/console.h"


enum{ARRET,COURSE,FRAPPE,MORT};
enum{CHANGEMENT_DE_MAP,LUMIERE,INFLIGER_DEGATS,DECLENCHEUR_DEGAT_TO_EVENEMENT,CHANGER_DECOR,TIMER,EXPLOSION};
enum{NORMAL,BONNEFACTURE,BENI,SACRE,SANCTIFIE,DIVIN,INFERNAL,CRAFT};

enum{AUCUN_EMPLACEMENT,ARME_PRINCIPAL,BOUCLIER,ARMURE_CORPS,ARMURE_CORPS2,CASQUE,BOTTES,GANT,ANNEAU,PENDENTIF};


enum{AUCUN,ARME,ARMURE};

enum{EFFICACITE_ACCRUE,FO_SUPP,DEX_SUPP,VIT_SUPP,PI_SUPP,CH_SUPP,VIE_SUPP,FOI_SUPP,DEGATS_FEU,DEGATS_FOI};

/// http://www.sfml-dev.org/wiki/fr/sources/fonctions_maths
/// Par hiura

namespace gpl
{
    /**
    \brief Racine carre

    \param number : X

    \return racine carre de X
    */
    inline float sqrt(float number)
    {
        long i;
        float x, y;
        const float f = 1.5F;

        x = number * 0.5F;
        y  = number;
        i  = * ( long * ) &y;
        i  = 0x5f3759df - ( i >> 1 );
        y  = * ( float * ) &i;
        y  = y * ( f - ( x * y * y ) );
        y  = y * ( f - ( x * y * y ) ); // si on veut plus de précision
        return number * y;
    }
} // namespace gpl

template <typename T>
inline T valeurAbsolue(T valeur)
{
    if (valeur < 0)
        valeur = - valeur;
    return valeur;
}

struct coordonnee
{
    int x;
    int y;
    int w;
    int h;
};

struct coordonnee3D
{
    float x;
    float y;
    float z;
};

struct coordonneeDecimal
{
    float x;
    float y;
    float w;
    float h;
};

class Lumiere
{
public:
    int intensite;
    int rouge;
    int vert;
    int bleu;
    float hauteur;
};

struct Ombre
{
    float angle;
    int intensite;
    int intensiteBasique;
    float taille;
};

class LumiereOmbrage : public Lumiere
{
public:
    LumiereOmbrage()
    {
    }
    ~LumiereOmbrage()
    {
        m_ombre.clear();
    }
    void detruire()
    {
        m_ombre.clear();
    }
    void AjouterOmbre(int intensite, float angle, float hauteur_lumiere)
    {
        m_ombre.push_back(Ombre ());

        m_ombre.back().intensite=intensite;
        m_ombre.back().intensiteBasique=intensite;
        m_ombre.back().angle=angle;
        m_ombre.back().taille=(100-(float)hauteur_lumiere)/50;


        for (std::vector <Ombre>::iterator ombre = m_ombre.begin(); ombre!=m_ombre.end(); ++ombre)
        {
            float angle;
            angle=valeurAbsolue(m_ombre.back().angle-ombre->angle);
            if (angle>180)
                angle=360-angle;

            m_ombre.back().intensite-=(int)((float)ombre->intensiteBasique*angle/270);
            ombre->intensite-=(int)((float)m_ombre.back().intensiteBasique*angle/270);

            if (m_ombre.back().intensite<0)
                m_ombre.back().intensite=0;
            if (ombre->intensite<0)
                ombre->intensite=0;
        }

    }
    LumiereOmbrage operator=(const LumiereOmbrage &lumiere)
    {
        intensite = lumiere.intensite;
        rouge = lumiere.rouge;
        vert = lumiere.vert;
        bleu = lumiere.bleu;
        hauteur = lumiere.hauteur;

        Ombre temp;
        m_ombre.resize(lumiere.m_ombre.size(),temp);

        for (int i=0;i<(int)lumiere.m_ombre.size();i++)
        {
            m_ombre[i].intensite=lumiere.m_ombre[i].intensite;
            m_ombre[i].intensiteBasique=lumiere.m_ombre[i].intensiteBasique;
            m_ombre[i].angle=lumiere.m_ombre[i].angle;
            m_ombre[i].taille=lumiere.m_ombre[i].taille;
        }

        return *this;
    }
    LumiereOmbrage operator=(const Lumiere &lumiere)
    {
        intensite = lumiere.intensite;
        rouge = lumiere.rouge;
        vert = lumiere.vert;
        bleu = lumiere.bleu;
        hauteur = lumiere.hauteur;

        return *this;
    }

    std::vector <Ombre> m_ombre;

};

struct Caracteristique
{
    std::string nom;

    int pointAme;
    float ancienPointAme,positionAncienAme;
    float vitesse;
    float vie;
    float maxVie;
    float foi;
    int maxFoi;
    int degatsMin,degatsMax,armure;
    int niveau;
    int rang;

    int force;
    int dexterite;
    int vitalite;
    int piete;
    int charisme;
    int pts_restant;

    float modificateurTaille;

    bool sang;
};





#endif




