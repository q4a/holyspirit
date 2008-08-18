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
enum{CHANGEMENT_DE_MAP,LUMIERE};

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
            bool video;
            std::string chemin_video_developpeur,nom_video_developpeur;

            ///Chargement de la configuration

            std::ifstream fichier,fichier3;
            fichier3.open("configuration.conf", std::ios::in);
            if(!fichier3)
            {
                fichier3.close();
                std::fstream fichier2("configuration.conf", std::ios::in | std::ios::out | std::ios::trunc) ;

                fichier2<<"AFFICHAGE\nresolution: 800 600\nluminosite: 0\ncontrastes: 1\nsyncronisation_verticale: 1\nmode_fenetre: 1\nombres: 1\nlumieres: 1\namelioration_des_lampes: 0\nfrequence_lumiere: 0.054\nherbes: 1\npostFX: 1\nsang: 1\n\nMUSIQUES\nvolume: 100\nson_mort: Data/Sons/Coeur.wav\n\nINTERFACE\nminimap: 1\nconsole: 0\n\nVERSION\nversion: 0.0.98\n\nCHEMINS\nmaps: Data/Maps/map\ntemps: Data/Temp/\nsaves: Data/Saves/\nevenements: Data/Maps/evenement\ncurseur: Data/Menus/Curseurs/\nmenus: Data/Menus/\nfonts: Data/Fonts/Cup and Talon.ttf\nfx: Data/Effets/\n\ncurseur_base: curseurMenu.png\n\nfxNoir: EffetNoir.sfx\nfxContrastes: EffetContrastes.sfx\nfxMort: EffetMort.sfx\n\nmenu_hud: Hudoo.png\nmenu_minimap: minimap.png\nmenu_bulle_vie: bulle_vie.png\nmenu_ame: Ame.png\nmenu_barre_ame: Barre_ame.png\nmenu_barre_vie: barre_vie.png\nmenu_barre_vie_vide: barre_vie_vide.png\nmenu_sang: sang.png\nmenu_inventaire: inventaire.png\n\nAUTRE\nvideos: 0\nchemin_video_developpeurs: Data/Videos/Naiponcin logo/\nnom_video_developpeurs: Naiponcin logo\n\ncache: 200\n\nfrequence_sauvegarde: 10\n";

                fichier2.close();

                //fichier.seekg(0, std::ios::beg);
            }
            fichier.close();

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
                        if(chaine== "version:") { fichier>>version; }

                        if(chaine== "maps:") { fichier>>chemin_maps; }
                        if(chaine== "temps:") { fichier>>chemin_temps; }
                        if(chaine== "saves:") { fichier>>chemin_saves; }
                        if(chaine== "evenements:") { fichier>>chemin_evenements; }
                        if(chaine== "curseur:") { fichier>>chemin_curseurs; }
                        if(chaine== "menus:") { fichier>>chemin_menus; }
                        if(chaine== "fonts:") { fichier>>chemin_fonts; }
                        if(chaine== "fx:") { fichier>>chemin_fx; }
                        if(chaine== "chemin_video_developpeurs:") { fichier>>chemin_video_developpeur; }
                        if(chaine== "nom_video_developpeurs:") { fichier>>nom_video_developpeur; }

                        if(chaine== "curseur_base:") { fichier>>nom_curseur_base; }
                        if(chaine== "fxNoir:") { fichier>>nom_effetNoir; }
                        if(chaine== "fxMort:") { fichier>>nom_effetMort; }
                        if(chaine== "fxContrastes:") { fichier>>nom_effetContrastes; }

                        if(chaine== "menu_hud:") { fichier>>nom_hud; }
                        if(chaine== "menu_minimap:") { fichier>>nom_minimap; }
                        if(chaine== "menu_bulle_vie:") { fichier>>nom_bulle_vie; }
                        if(chaine== "menu_ame:") { fichier>>nom_ame; }
                        if(chaine== "menu_barre_ame:") { fichier>>nom_barre_ame; }
                        if(chaine== "menu_barre_vie:") { fichier>>nom_barre_vie; }
                        if(chaine== "menu_barre_vie_vide:") { fichier>>nom_barre_vie_vide; }
                        if(chaine== "menu_inventaire:") { fichier>>nom_inventaire; }


                        if(chaine== "menu_sang:") { fichier>>nom_sang; }
                        if(chaine== "nom_video_developpeurs: ") { fichier>>nom_video_developpeur; }

                        if(chaine== "cache: ") { fichier>>cache; }

                        if(chaine== "frequence_sauvegarde:") { fichier>>frequence_sauvegarde; }

                        if(chaine== "son_mort:") { fichier>>chemin_son_mort; }

                }
                 fichier.close();
            }
            else
                throw "Impossible de charger la configuration";

            ///Application de la configuration

            Resolution.x=resolution.x;
            Resolution.y=resolution.y;
            Resolution.w=resolution.x;
            Resolution.h=resolution.y;

            zoom=1;

            if(!Lumiere)
                Ombre=0;

    }

    coordonnee Resolution;
    bool Ombre,Lumiere,Minimap,amelioration_lampes,console,Herbes,syncronisation_verticale,mode_fenetre,postFX,sang;
    float effetMort,volume,minute,zoom,frequence_sauvegarde,frequence_lumiere,luminosite,contrastes;
    int heure,cache;
    std::string version,chemin_maps,chemin_temps,chemin_saves,chemin_evenements,chemin_curseurs,chemin_menus,chemin_fonts,chemin_fx,nom_curseur_base,nom_effetNoir,nom_effetMort,nom_effetContrastes,nom_hud,nom_minimap,nom_bulle_vie,nom_ame,nom_barre_ame,nom_barre_vie,nom_barre_vie_vide,nom_sang,nom_inventaire,chemin_son_mort;
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




