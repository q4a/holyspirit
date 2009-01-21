///**************************************************\\\
||| Dernière modification : Le 06/02/08 par Gregouar |||
///**************************************************///




#ifndef CONSTANTEH
#define CONSTANTEH

#define COTE_TILE 71.5542
#define DIVISEUR_COTE_TILE 0.013975420031249039189872851628556

#define NOMBRE_ETAT 4
#define CALCUL_PA_PROCHAIN_NIVEAU temp.niveau*temp.niveau*temp.niveau*10

#define NOMBRE_SANG 7

#define MAX_MUSIQUE 5

#define NOMBRE_TYPE_OBJET 2

#define NOMBRE_BENEDICTION 8

#define NOMBRE_SONS 200

#define NOMBRE_COUCHE_MAP 2

#include <cmath>
#include <utility>
#include <iostream>
#include <fstream>
#include <vector>

#include "console.h"


enum{ARRET,COURSE,FRAPPE,MORT};
enum{CHANGEMENT_DE_MAP,LUMIERE,INFLIGER_DEGATS,DECLENCHEUR_DEGAT_TO_EVENEMENT,CHANGER_DECOR,TIMER,EXPLOSION};
enum{NORMAL,BONNEFACTURE,BENI,SACRE,SANCTIFIE,DIVIN,INFERNAL,CRAFT};

enum{AUCUN_EMPLACEMENT,ARME_PRINCIPAL,BOUCLIER,ARMURE_CORPS};
enum{AUCUN,ARME,ARMURE};

enum{EFFICACITE_ACCRUE,FO_SUPP,DEX_SUPP,VIT_SUPP,PI_SUPP,CH_SUPP,VIE_SUPP,FOI_SUPP};

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

struct coordonnee3D
{
	float x;
	float y;
	float z;
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
                        if(chaine== "resolution:") { fichier>>resolution.x>>resolution.y; }
                        if(chaine== "brightness:") { fichier>>luminosite; }
                        if(chaine== "contrast:") { fichier>>contrastes; }
                        if(chaine== "alpha_opacity:") { fichier>>alpha; }
                        if(chaine== "window:") { fichier>>mode_fenetre; }
                        if(chaine== "videos:") { fichier>>video; }
                        if(chaine== "postFX:") { fichier>>postFX; }
                        if(chaine== "blood:") { fichier>>sang; }
                        if(chaine== "shadows:") { fichier>>Ombre; }
                        if(chaine== "light_quality:") { fichier>>Lumiere; }
                        if(chaine== "grass:") { fichier>>Herbes; }
                        if(chaine== "particules:") { fichier>>particules; }
                        if(chaine== "vsync:") { fichier>>syncronisation_verticale; }
                        if(chaine== "volume:") { fichier>>volume; }
                        if(chaine== "minimap:") { fichier>>Minimap; }
                        if(chaine== "light_refresh_rate:") { fichier>>frequence_lumiere; }
                        if(chaine== "console:") { fichier>>console; }


                        if(chaine== "saving_frequency:") { fichier>>frequence_sauvegarde; }
                        if(chaine== "debug_mod:") { fichier>>debug; }
                        if(chaine== "desactivate_console:") { fichier>>desactivate_console; }

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
                        if(chaine== "events:") { fichier2>>chemin_evenements; }
                        if(chaine== "cursor:") { fichier2>>chemin_curseurs; }
                        if(chaine== "menus:") { fichier2>>chemin_menus; }
                        if(chaine== "fonts:") { fichier2>>chemin_fonts; }
                        if(chaine== "fx:") { fichier2>>chemin_fx; }
                        if(chaine== "alpha_arts:") { fichier2>>chemin_aa; }

                        if(chaine== "basic_cursor:") { fichier2>>nom_curseur_base; }
                        if(chaine== "fxBlack:") { fichier2>>nom_effetNoir; }
                        if(chaine== "fxDeath:") { fichier2>>nom_effetMort; }
                        if(chaine== "fxContrast:") { fichier2>>nom_effetContrastes; }
                        if(chaine== "fxBlur:") { fichier2>>nom_effetBlur; }

                        if(chaine== "menu_hud:") { fichier2>>nom_hud; }
                        if(chaine== "menu_minimap:") { fichier2>>nom_minimap; }
                        if(chaine== "menu_life_orb:") { fichier2>>nom_bulle_vie; }
                        if(chaine== "menu_faith_orb:") { fichier2>>nom_bulle_foi; }

                        if(chaine== "menu_soul:") { fichier2>>nom_ame; }
                        if(chaine== "menu_soul_orb:") { fichier2>>nom_barre_ame; }
                        if(chaine== "menu_life_monster:") { fichier2>>nom_barre_vie; }
                        if(chaine== "menu_life_monster_blank:") { fichier2>>nom_barre_vie_vide; }
                        if(chaine== "menu_inventory:") { fichier2>>nom_inventaire; }
                        if(chaine== "menu_bag:") { fichier2>>nom_sac; }

                        if(chaine== "text_benedictions:") { fichier2>>chemin_text_benedictions; }



                        if(chaine== "font_title:") { fichier2>>font_titre; }

                        if(chaine== "menu_blood:") { fichier2>>nom_sang; }

                        if(chaine== "cache: ") { fichier2>>cache; }

                        if(chaine== "death_sound:") { fichier2>>chemin_son_mort; }

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


            text_benedictions=new std::string [NOMBRE_BENEDICTION];

            char temp[255];

            std::ifstream fichier3;
            fichier3.open(chemin_text_benedictions.c_str(), std::ios::in);
            if(fichier3)
            {
                std::string chaine;
                for(int i=0;i<NOMBRE_BENEDICTION;i++)
                    getline(fichier3,text_benedictions[i]);
                fichier3.close();
            }

    }

    void Sauvegarder()
    {
            ///Chargement de la configuration

            std::fstream fichier("configuration.conf", std::ios::in | std::ios::out | std::ios::trunc) ;

            if(fichier)
            {
                        fichier<< "DISPLAY"<<std::endl;
                        fichier<< "resolution: " <<Resolution.w<<" "<<Resolution.h<<std::endl;
                        fichier<< "brightness: " <<luminosite<<std::endl;
                        fichier<< "contrast: " <<contrastes<<std::endl;
                        fichier<< "window: " <<mode_fenetre<<std::endl;
                        fichier<< "vsync: " <<syncronisation_verticale<<std::endl;
                        fichier<<std::endl;
                        fichier<< "light_quality: " <<Lumiere<<std::endl;
                        fichier<< "light_refresh_rate: " <<frequence_lumiere<<std::endl;
                        fichier<< "shadows: " <<Ombre<<std::endl;
                        fichier<< "videos: " <<video<<std::endl;
                        fichier<< "postFX: " <<postFX<<std::endl;
                        fichier<< "alpha_opacity: "<<alpha<<std::endl;
                        fichier<< "blood: " <<sang<<std::endl;
                        fichier<< "grass: " <<Herbes<<std::endl;
                        fichier<< "particules: " <<particules<<std::endl;
                        fichier<<std::endl;
                        fichier<<std::endl;
                        fichier<<"SOUNDS"<<std::endl;
                        fichier<< "volume: " <<volume<<std::endl;
                        fichier<<std::endl;
                        fichier<<std::endl;
                        fichier<<"INTERFACE"<<std::endl;
                        fichier<< "minimap: " <<Minimap<<std::endl;
                        fichier<< "console: " <<console<<std::endl;
                        fichier<<std::endl;
                        fichier<<std::endl;

                        fichier<<"OTHERS"<<std::endl;
                        fichier<< "saving_frequency: " <<frequence_sauvegarde<<std::endl;
                        fichier<<"debug_mod: "<<debug<<std::endl;
                        fichier<<"desactivate_console: "<<desactivate_console<<std::endl;



                 fichier.close();
            }
            else
                throw "Impossible de sauvegarder la configuration";
    }


    coordonnee Resolution;
    bool Ombre,Minimap,amelioration_lampes,console,Herbes,syncronisation_verticale,mode_fenetre,postFX,sang,video,particules,debug,desactivate_console;
    int Lumiere;
    float effetMort,effetNoir,volume,minute,zoom,frequence_sauvegarde,frequence_lumiere,luminosite,contrastes;
    int heure,cache,alpha;
    std::string version,chemin_maps,chemin_temps,chemin_saves,chemin_evenements,chemin_curseurs,chemin_menus,chemin_fonts,chemin_fx,nom_curseur_base,nom_effetNoir,nom_effetMort,nom_effetContrastes,nom_effetBlur,nom_hud,nom_minimap,nom_bulle_vie,nom_bulle_foi,nom_ame,nom_barre_ame,nom_barre_vie,nom_barre_vie_vide,nom_sang,nom_inventaire,chemin_son_mort;
    std::string font_titre,chemin_video_developpeur,nom_video_developpeur,nom_sac,chemin_aa;

    std::string chemin_text_benedictions;

    std::string *text_benedictions;
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

    float modificateurTaille;

    bool sang;
};

struct coordonneeDecimal
{
    float x;
	float y;
	float h;
	float w;
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




