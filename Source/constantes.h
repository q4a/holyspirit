///**************************************************\\\
||| Dernière modification : Le 06/02/08 par Gregouar |||
///**************************************************///




#ifndef CONSTANTEH
#define CONSTANTEH

#define COTE_TILE 71.5542
#define DIVISEUR_COTE_TILE 0.013975420031249039189872851628556

#define NOMBRE_ETAT 4
#define CALCUL_PA_PROCHAIN_NIVEAU temp.niveau*temp.niveau*temp.niveau*10

#define NOMBRE_SANG 3

#include <cmath>
#include <utility>
#include <iostream>
#include <fstream>
#include <vector>

#include "console.h"


enum{ARRET,COURSE,FRAPPE,MORT};
enum{CHANGEMENT_DE_MAP,LUMIERE,INFLIGER_DEGATS,DECLENCHEUR_DEGAT_TO_EVENEMENT,CHANGER_DECOR,TIMER,EXPLOSION};
enum{NORMAL,BONNEFACTURE,BENI,SACRE,SANCTIFIE,DIVIN,INFERNAL,CRAFT};

namespace gpl
{
/**
\brief Racine carre

\param number : X

\return racine carre de X
*/
inline float sqrt(float number) {
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
    void Charger()
    {
            coordonnee resolution;
            ///Chargement de la configuration

            std::ifstream fichier,fichier2;

            fichier.open("configuration.conf", std::ios::in);
            if(fichier)
            {
                char caractere;
                std::string chaine;
                while(fichier>>chaine)
                {
                        if(chaine=="resolution:") { fichier>>resolution.x>>resolution.y; }
                        if(chaine=="luminosite:") { fichier>>luminosite; }
                        if(chaine=="contrastes:") { fichier>>contrastes; }
                        if(chaine== "mode_fenetre:") { fichier>>mode_fenetre; }
                        if(chaine== "videos:") { fichier>>video; }
                        if(chaine== "postFX:") { fichier>>postFX; }
                        if(chaine== "sang:") { fichier>>sang; }
                        if(chaine== "ombres:") { fichier>>Ombre; }
                        if(chaine== "lumieres:") { fichier>>Lumiere; }
                        if(chaine== "herbes:") { fichier>>Herbes; }
                        if(chaine== "syncronisation_verticale:") { fichier>>syncronisation_verticale; }
                        if(chaine== "volume:") { fichier>>volume; }
                        if(chaine== "minimap:") { fichier>>Minimap; }
                        if(chaine== "amelioration_des_lampes:") { fichier>>amelioration_lampes; }
                        if(chaine== "frequence_lumiere:") { fichier>>frequence_lumiere; }
                        if(chaine== "console:") { fichier>>console; }

                        if(chaine== "frequence_sauvegarde:") { fichier>>frequence_sauvegarde; }

                }
                 fichier.close();
            }
            else
            {
                throw std::string("Impossible de charger la configuration : configuration.conf");
            }


            fichier2.open("holyspirit.ini", std::ios::in);
            if(fichier2)
            {
                char caractere;
                std::string chaine;
                while(fichier2>>chaine)
                {
                        if(chaine== "version:") { fichier2>>version; }

                        if(chaine== "maps:") { fichier2>>chemin_maps; }
                        if(chaine== "temps:") { fichier2>>chemin_temps; }
                        if(chaine== "saves:") { fichier2>>chemin_saves; }
                        if(chaine== "evenements:") { fichier2>>chemin_evenements; }
                        if(chaine== "curseur:") { fichier2>>chemin_curseurs; }
                        if(chaine== "menus:") { fichier2>>chemin_menus; }
                        if(chaine== "fonts:") { fichier2>>chemin_fonts; }
                        if(chaine== "fx:") { fichier2>>chemin_fx; }
                        if(chaine== "chemin_video_developpeurs:") { fichier2>>chemin_video_developpeur; }
                        if(chaine== "nom_video_developpeurs:") { fichier2>>nom_video_developpeur; }
                        if(chaine== "alpha_arts:") { fichier2>>chemin_aa; }

                        if(chaine== "curseur_base:") { fichier2>>nom_curseur_base; }
                        if(chaine== "fxNoir:") { fichier2>>nom_effetNoir; }
                        if(chaine== "fxMort:") { fichier2>>nom_effetMort; }
                        if(chaine== "fxContrastes:") { fichier2>>nom_effetContrastes; }
                        if(chaine== "fxBlur:") { fichier2>>nom_effetBlur; }

                        if(chaine== "menu_hud:") { fichier2>>nom_hud; }
                        if(chaine== "menu_minimap:") { fichier2>>nom_minimap; }
                        if(chaine== "menu_bulle_vie:") { fichier2>>nom_bulle_vie; }
                        if(chaine== "menu_ame:") { fichier2>>nom_ame; }
                        if(chaine== "menu_barre_ame:") { fichier2>>nom_barre_ame; }
                        if(chaine== "menu_barre_vie:") { fichier2>>nom_barre_vie; }
                        if(chaine== "menu_barre_vie_vide:") { fichier2>>nom_barre_vie_vide; }
                        if(chaine== "menu_inventaire:") { fichier2>>nom_inventaire; }
                        if(chaine== "menu_sac:") { fichier2>>nom_sac; }


                        if(chaine== "font_titre:") { fichier2>>font_titre; }

                        if(chaine== "menu_sang:") { fichier2>>nom_sang; }
                        if(chaine== "nom_video_developpeurs: ") { fichier2>>nom_video_developpeur; }

                        if(chaine== "cache: ") { fichier2>>cache; }

                        if(chaine== "son_mort:") { fichier2>>chemin_son_mort; }

                }
                 fichier2.close();
            }
            else
            {
                throw std::string("Impossible de charger la configuration : holyspirit.ini");
            }

            ///Application de la configuration

            Resolution.x=resolution.x;
            Resolution.y=resolution.y;
            Resolution.w=resolution.x;
            Resolution.h=resolution.y;

            zoom=1;

            if(!Lumiere)
                Ombre=0;
    }

    void Sauvegarder()
    {
            ///Chargement de la configuration

            std::fstream fichier("configuration.conf", std::ios::in | std::ios::out | std::ios::trunc) ;

            if(fichier)
            {
                        fichier<<"AFFICHAGE"<<std::endl;
                        fichier<<"resolution: " <<Resolution.w<<" "<<Resolution.h<<std::endl;
                        fichier<<"luminosite: " <<luminosite<<std::endl;
                        fichier<<"contrastes: " <<contrastes<<std::endl;
                        fichier<< "mode_fenetre: " <<mode_fenetre<<std::endl;
                        fichier<< "syncronisation_verticale: " <<syncronisation_verticale<<std::endl;
                        fichier<<std::endl;
                        fichier<< "lumieres: " <<Lumiere<<std::endl;
                        fichier<< "amelioration_des_lampes: " <<amelioration_lampes<<std::endl;
                        fichier<< "frequence_lumiere: " <<frequence_lumiere<<std::endl;
                        fichier<< "ombres: " <<Ombre<<std::endl;
                        fichier<< "videos: " <<video<<std::endl;
                        fichier<< "postFX: " <<postFX<<std::endl;
                        fichier<< "sang: " <<sang<<std::endl;
                        fichier<< "herbes: " <<Herbes<<std::endl;
                        fichier<<std::endl;
                        fichier<<std::endl;
                        fichier<<"MUSIQUES"<<std::endl;
                        fichier<< "volume: " <<volume<<std::endl;
                        fichier<<std::endl;
                        fichier<<std::endl;
                        fichier<<"INTERFACE"<<std::endl;
                        fichier<< "minimap: " <<Minimap<<std::endl;
                        fichier<< "console: " <<console<<std::endl;
                        fichier<<std::endl;
                        fichier<<std::endl;

                        fichier<<"AUTRES"<<std::endl;
                        fichier<< "frequence_sauvegarde: " <<frequence_sauvegarde<<std::endl;


                 fichier.close();
            }
            else
                throw "Impossible de sauvegarder la configuration";
    }


    coordonnee Resolution;
    bool Ombre,Minimap,amelioration_lampes,console,Herbes,syncronisation_verticale,mode_fenetre,postFX,sang,video;
    int Lumiere;
    float effetMort,volume,minute,zoom,frequence_sauvegarde,frequence_lumiere,luminosite,contrastes;
    int heure,cache;
    std::string version,chemin_maps,chemin_temps,chemin_saves,chemin_evenements,chemin_curseurs,chemin_menus,chemin_fonts,chemin_fx,nom_curseur_base,nom_effetNoir,nom_effetMort,nom_effetContrastes,nom_effetBlur,nom_hud,nom_minimap,nom_bulle_vie,nom_ame,nom_barre_ame,nom_barre_vie,nom_barre_vie_vide,nom_sang,nom_inventaire,chemin_son_mort;
    std::string font_titre,chemin_video_developpeur,nom_video_developpeur,nom_sac,chemin_aa;
};

struct Caracteristique
{
    std::string nom;

    int pointAme;
    float ancienPointAme,positionAncienAme;
    float vitesse;
    float vie;
    int maxVie;
    int degatsMin,degatsMax;
    int niveau;
    int rang;

    float modificateurTaille;

    bool sang;
};

struct coordonneeDecimal
{
    float x;
	float y;
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
        Ombre ombreTemp;
        m_ombre.push_back(ombreTemp);

        m_ombre[m_ombre.size()-1].intensite=intensite;
        m_ombre[m_ombre.size()-1].intensiteBasique=intensite;
        m_ombre[m_ombre.size()-1].angle=angle;
        m_ombre[m_ombre.size()-1].taille=(100-(float)hauteur_lumiere)/50;


        for(int i=0;i<m_ombre.size()-1;i++)
        {
            float angle;
            angle=valeurAbsolue(m_ombre[m_ombre.size()-1].angle-m_ombre[i].angle);
            if(angle>180)
                angle=360-angle;

            m_ombre[m_ombre.size()-1].intensite-=(int)((float)m_ombre[i].intensiteBasique*angle/270);
            m_ombre[i].intensite-=(int)((float)m_ombre[m_ombre.size()-1].intensiteBasique*angle/270);

            if(m_ombre[m_ombre.size()-1].intensite<0)
                m_ombre[m_ombre.size()-1].intensite=0;
            if(m_ombre[i].intensite<0)
                m_ombre[i].intensite=0;
        }

    }
    LumiereOmbrage LumiereOmbrage::operator=(const LumiereOmbrage &lumiere)
    {
        intensite = lumiere.intensite;
        rouge = lumiere.rouge;
        vert = lumiere.vert;
        bleu = lumiere.bleu;
        hauteur = lumiere.hauteur;

        Ombre temp;
        m_ombre.resize(lumiere.m_ombre.size(),temp);

        for(int i=0;i<lumiere.m_ombre.size();i++)
        {
            m_ombre[i].intensite=lumiere.m_ombre[i].intensite;
            m_ombre[i].intensiteBasique=lumiere.m_ombre[i].intensiteBasique;
            m_ombre[i].angle=lumiere.m_ombre[i].angle;
            m_ombre[i].taille=lumiere.m_ombre[i].taille;
        }

        return *this;
    }
    LumiereOmbrage LumiereOmbrage::operator=(const Lumiere &lumiere)
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

#endif




