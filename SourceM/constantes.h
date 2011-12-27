

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

#ifdef WIN32
# define WINDOWS
#else
# define LINUX
#endif

#define COTE_TILE 71.5542f
#define DIVISEUR_COTE_TILE 0.013975420031249039189872851628556f

#define NOMBRE_ETAT 4
#define CALCUL_PA_PROCHAIN_NIVEAU temp.niveau*temp.niveau*temp.niveau*10

#define NOMBRE_SANG 7

#define MAX_MUSIQUE 5

#define NOMBRE_TYPE_OBJET 2

#define NOMBRE_TXT_MENU 13

#define NOMBRE_SONS 128

#define NOMBRE_COUCHE_MAP 2

#define NOMBRE_MORCEAU_PERSONNAGE 8

#define NOMBRE_BENEDICTION 6

#define VERSION_SAVE 10

#define NET_PORT 4570

#include <cmath>
#include <utility>
#include <iostream>
#include <fstream>
#include <vector>
#include <SFML/Network.hpp>

#include "datFile.h"

#include "Moteurs/console.h"


enum {ARRET,COURSE,FRAPPE,MORT};
enum {NORMAL,BONNEFACTURE,BENI,SACRE,SANCTIFIE,DIVIN,INFERNAL,CRAFT,QUETE};

enum {AUCUN_EMPLACEMENT,ARME_PRINCIPAL,BOUCLIER,ARMURE_CORPS,ARMURE_CORPS2,CASQUE,BOTTES,GANT,ANNEAU,PENDENTIF};

enum {AUCUN,ARME,ARMURE,CONSOMMABLE,SCHEMA,LITANIE,JEWELERY,GOLEM,DOCUMENT,GOLD};

enum {EFFICACITE_ACCRUE,CARACT_SUPP,POINTS_SUPP,DEGATS_SUPP, ARMURE_SUPP, DEGATS_TEMPS_SUPP, MIRACLE_SUPP};

enum {FORCE, DEXTERITE, VITALITE, PIETE, CHARISME};
enum {PT_VIE, PT_FOI};
enum {PHYSIQUE, FEU, FOI, CORROSION};

enum {M_AUCUN, M_ORGANIQUE, M_ARMURE, M_VAPOREUX, M_ROCHE};

enum {B_CHAT,B_MIRACLES,B_INVENTAIRE,B_QUETES,B_MENU,B_MAP,B_DEPLIER, B_DOCS};

enum {P_NEWPLAYER, P_DELETEPLAYER, P_PLAYERCARACT, P_PLAYERSKIN, P_CHANGEMAP, P_INFOSMONSTRE, P_DEGATS, P_KILLMONSTRE,
      P_MIRACLE, P_INTERACT, P_ERASEFRIEND, P_QUEST, P_CLIMATE, P_MSGCHAT, P_READY};

inline sf::FloatRect GetViewRect(const sf::View& view)
{
    sf::FloatRect temp;

    temp.Left   = view.GetCenter().x - view.GetSize().x * 0.5;
    temp.Top    = view.GetCenter().y - view.GetSize().y * 0.5;
    temp.Width  = view.GetSize().x;
    temp.Height = view.GetSize().y;

    return temp;
};

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

struct coordonneeDecimal
{
    coordonneeDecimal()
    {
        x = 0;
        y = 0;
        w = 0;
        h = 0;
    }
    coordonneeDecimal(float X, float Y, float W, float H)
    {
        x = X;
        y = Y;
        w = W;
        h = H;
    }
    coordonneeDecimal(float X, float Y)
    {
        x = X;
        y = Y;
        w = 0;
        h = 0;
    }

    coordonneeDecimal(const coordonneeDecimal &temp)
    {
        x = temp.x;
        y = temp.y;
        w = temp.w;
        h = temp.h;
    }

    float x;
    float y;
    float w;
    float h;
};

struct coordonnee
{
    coordonnee()
    {
        x = 0;
        y = 0;
        w = 0;
        h = 0;
    }
    coordonnee(int X, int Y, int W, int H)
    {
        x = X;
        y = Y;
        w = W;
        h = H;
    }
    coordonnee(int X, int Y)
    {
        x = X;
        y = Y;
        w = 0;
        h = 0;
    }
    coordonnee(const coordonnee &temp)
    {
        x = temp.x;
        y = temp.y;
        w = temp.w;
        h = temp.h;
    }
    coordonnee(const coordonneeDecimal &temp)
    {
        x = (int)temp.x;
        y = (int)temp.y;
        w = (int)temp.w;
        h = (int)temp.h;
    }
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



class Lumiere
{
public:
    int intensite;
    int rouge;
    int vert;
    int bleu;
    float hauteur;
};


struct Caracteristique
{
    Caracteristique() : pointAme(0), ancienPointAme(0), positionAncienAme(0),
        vitesse(0), vie(0), maxVie(0), reserveVie(0), regenVie(0), foi(0),
        maxFoi(0), reserveFoi(0), regenFoi(0), volVie(0), volFoi(0), niveau(0),
        rang(0), force(0), dexterite(0), vitalite(0), piete(0), charisme(0),
        pts_restant(0), miracles_restant(0), modificateurTaille(0)
    {

    }

    std::string nom;

    int pointAme;
    float ancienPointAme,positionAncienAme;
    float vitesse;

    float vie;
    float maxVie;
    float reserveVie;
    float regenVie;

    float foi;
    float maxFoi;
    float reserveFoi;
    float regenFoi;

    float volVie;
    float volFoi;

    int degatsMin[4];
    int degatsMax[4];
    float armure[4];

    int niveau;
    int rang;

    int force;
    int dexterite;
    int vitalite;
    int piete;
    int charisme;
    int pts_restant;
    int miracles_restant;

    float modificateurTaille;
};


sf::Packet& operator >>(sf::Packet& Packet, Caracteristique& C);
sf::Packet& operator <<(sf::Packet& Packet, const Caracteristique& C);

sf::Packet& operator <<(sf::Packet& Packet, const coordonneeDecimal& C);
sf::Packet& operator >>(sf::Packet& Packet, coordonneeDecimal& C);

sf::Packet& operator <<(sf::Packet& Packet, const coordonnee& C);
sf::Packet& operator >>(sf::Packet& Packet, coordonnee& C);

sf::FloatRect GetViewRect(const sf::View& view);



#endif




