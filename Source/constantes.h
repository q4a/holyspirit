///**************************************************\\\
||| Dernière modification : Le 06/02/08 par Gregouar |||
///**************************************************///




#ifndef CONSTANTEH
#define CONSTANTEH

#define COTE_TILE 71.5542
#define NOMBRE_ETAT 4

#include <cmath>
#include <utility>
#include <iostream>
#include <vector>

#include "console.h"


enum{ARRET,COURSE,FRAPPE,MORT};
enum{CHANGEMENT_DE_MAP};

template <typename T>
inline T valeurAbsolue(T valeur)
{
    if(valeur < 0)
        valeur = - valeur;
    return valeur;
}

struct coordonnee
{
	int x;
	int y;
	int h;
	int w;
};

struct Configuration
{
    coordonnee Resolution;
    bool Ombre,Lumiere,Minimap,FonduLumiere,amelioration_lampes,console,Herbes;
    float effetMort,volume;
    std::string version,chemin_maps,chemin_evenements,chemin_curseurs,chemin_menus,chemin_fonts,chemin_fx,nom_curseur_base,nom_effetNoir,nom_effetBlur,nom_effetColorize,nom_hud,nom_minimap;
};

struct coordonneeDecimal
{
    double x;
	double y;
};

class Lumiere
{
    public:
    int intensite;
    int rouge;
    int vert;
    int bleu;
};

struct Ombre
{
    int angle;
    int intensite;
    int intensiteBasique;
};

class LumiereOmbrage : public Lumiere
{
    public:
    LumiereOmbrage()
    {
    }
    void detruire()
    {
        /*if(nombreOmbre>0)
            delete[] m_ombre;*/
        m_ombre.clear();
    }
    void AjouterOmbre(int intensite, int angle)
    {
       /* Ombre *ombreTemp;
        if(nombreOmbre>0)
        {
            ombreTemp=new Ombre[nombreOmbre];
            for(int i=0;i<nombreOmbre;i++)
                ombreTemp[i]=m_ombre[i];
            delete[] m_ombre;
        }
        m_ombre= new Ombre[nombreOmbre+1];
        if(nombreOmbre>0)
        {
            for(int i=0;i<nombreOmbre;i++)
                m_ombre[i]=ombreTemp[i];
            delete[] ombreTemp;
        }*/

       /*if(nombreOmbre<5)
        {
        m_ombre[nombreOmbre].intensite=intensite;
        m_ombre[nombreOmbre].intensiteBasique=intensite;
        m_ombre[nombreOmbre].angle=angle;


        for(int i=0;i<nombreOmbre;i++)
        {
            int angle;
            angle=valeurAbsolue(m_ombre[nombreOmbre].angle-m_ombre[i].angle);
            if(angle>180)
                angle=360-angle;

            m_ombre[nombreOmbre].intensite-=m_ombre[i].intensiteBasique*angle/180;
            m_ombre[i].intensite-=m_ombre[nombreOmbre].intensiteBasique*angle/180;

            if(m_ombre[nombreOmbre].intensite<0)
                m_ombre[nombreOmbre].intensite=0;
            if(m_ombre[i].intensite<0)
                m_ombre[i].intensite=0;
        }

         nombreOmbre++;
        }*/


        Ombre ombreTemp;
        m_ombre.push_back(ombreTemp);

        m_ombre[m_ombre.size()-1].intensite=intensite;
        m_ombre[m_ombre.size()-1].intensiteBasique=intensite;
        m_ombre[m_ombre.size()-1].angle=angle;


        for(int i=0;i<m_ombre.size()-1;i++)
        {
            int angle;
            angle=valeurAbsolue(m_ombre[m_ombre.size()-1].angle-m_ombre[i].angle);
            if(angle>180)
                angle=360-angle;

            m_ombre[m_ombre.size()-1].intensite-=m_ombre[i].intensiteBasique*angle/180;
            m_ombre[i].intensite-=m_ombre[m_ombre.size()-1].intensiteBasique*angle/180;

            if(m_ombre[m_ombre.size()-1].intensite<0)
                m_ombre[m_ombre.size()-1].intensite=0;
            if(m_ombre[i].intensite<0)
                m_ombre[i].intensite=0;
        }

    }
    LumiereOmbrage LumiereOmbrage::operator=(const Lumiere &lumiere)
    {
        intensite = lumiere.intensite;
        rouge = lumiere.rouge;
        vert = lumiere.vert;
        bleu = lumiere.bleu;

        return *this;
    }

    LumiereOmbrage LumiereOmbrage::operator=(const LumiereOmbrage &lumiere)
    {
        intensite = lumiere.intensite;
        rouge = lumiere.rouge;
        vert = lumiere.vert;
        bleu = lumiere.bleu;

        Ombre temp;
        m_ombre.resize(lumiere.m_ombre.size(),temp);

        for(int i=0;i<lumiere.m_ombre.size();i++)
        {
            m_ombre[i].intensite=lumiere.m_ombre[i].intensite;
            m_ombre[i].intensiteBasique=lumiere.m_ombre[i].intensiteBasique;
            m_ombre[i].angle=lumiere.m_ombre[i].angle;
        }

        return *this;
    }

    /*LumiereOmbrage LumiereOmbrage::operator=(const LumiereOmbrage &lumiere)
    {
        intensite = lumiere.intensite;
        rouge = lumiere.rouge;
        vert = lumiere.vert;
        bleu = lumiere.bleu;

        //lumiere.m_ombre.resize(m_ombre.size());

        //for(int i=0;i<m_ombre.size();i++)
        //m_ombre[i] = lumiere.m_ombre[i];

        return *this;
    }*/

    std::vector <Ombre> m_ombre;

};

#endif










