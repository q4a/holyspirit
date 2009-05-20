
#ifndef CONFIGH
#define CONFIGH

#include "singleton.h"
#include "constantes.h"


class Configuration : public CSingleton<Configuration>
{
    protected :

    Configuration()
    {
    }
    ~Configuration()
    {
        delete[] text_benedictions;
        delete[] text_menus;
    }

    public :


    friend Configuration* CSingleton<Configuration>::GetInstance();
    friend void CSingleton<Configuration>::Kill();

    void Charger();
    void ChargerConf();
    void ChargerInit();
    void ChargerTxt();

    void Sauvegarder();

    coordonnee Resolution;
    coordonneeDecimal Redimensionnement;
    bool Ombre,Minimap,amelioration_lampes,Herbes,syncronisation_verticale,mode_fenetre,postFX,sang,video,particules,debug,desactivate_console,lissage;
    int Lumiere,console;
    float effetMort,effetNoir,volume,minute,zoom,frequence_sauvegarde,frequence_lumiere,luminosite,contrastes;
    int heure,cache,alpha;
    std::string version,chemin_maps,chemin_items,chemin_temps,chemin_saves,chemin_evenements,chemin_curseurs,chemin_menus,chemin_fonts,chemin_fx,nom_curseur_base,nom_effetNoir,nom_effetMort,nom_effetContrastes,nom_effetBlur,nom_minimap,nom_ame,nom_barre_ame,nom_barre_vie,nom_barre_vie_vide,nom_sang,chemin_son_mort;
    std::string font_titre,chemin_video_developpeur,nom_video_developpeur,nom_sac,chemin_aa;

    std::string chemin_text_benedictions,chemin_text_menus;

    std::string *text_benedictions;
    std::string *text_menus;

    bool RafraichirLumiere;
    int numero_screen,RafraichirOmbre;
};
#endif
