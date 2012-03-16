#include <iostream>
#include <fstream>
#include <QApplication>


struct coordonnee {
    int x,y,w,h;
};

class Configuration {

    public:
        Configuration();
        ~Configuration() {}

        void Charger();

        void Sauvegarder();

        coordonnee  Resolution;

        bool        Ombre,
                    Minimap,
                    amelioration_lampes,
                    Herbes,
                    syncronisation_verticale,
                    mode_fenetre,
                    postFX,
                    video,
                    particules,
                    debug,
                    desactivate_console,
                    lissage,
                    Reflection,
                    item_background,
                    Distortion,
                    RafraichirLumiere;

        int         Lumiere,
                    console,
                    heure,
                    cache,
                    alpha,
                    numero_screen,
                    RafraichirOmbre;

        float       effetMort,
                    effetNoir,
                    volume,
                    music_volume,
                    minute,
                    zoom,
                    frequence_sauvegarde,
                    frequence_lumiere,
                    luminosite,
                    contrastes;

        std::string version,
                    chemin_maps,
                    chemin_items,
                    chemin_temps,
                    chemin_saves,
                    chemin_evenements,
                    chemin_curseurs,
                    chemin_menus,
                    chemin_fonts,
                    chemin_fx,
                    nom_curseur_base,
                    nom_effetNoir,
                    nom_effetMort,
                    nom_effetContrastes,
                    nom_effetBlur,
                    nom_effetFiltre,
                    nom_minimap,
                    nom_ame,
                    nom_barre_ame,
                    nom_barre_vie,
                    nom_barre_vie_vide,
                    nom_sang,
                    chemin_son_mort,
                    font_titre,
                    font,
                    chemin_video_developpeur,
                    nom_video_developpeur,
                    nom_sac,
                    nom_corner,
                    chemin_aa,
                    chemin_text_benedictions,
                    chemin_text_menus,
                    language;
};
