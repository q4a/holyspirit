#include "configuration.h"

Configuration::Configuration() {
    //On met des valeurs par défaut qui seront alors sauvegardées lors de l'enregistrement !!
    Resolution.x = 800;
    Resolution.w = 800;
    Resolution.y = 600;
    Resolution.h = 600;

    luminosite =  20;
    contrastes =  1.5;
    mode_fenetre = 1;
    syncronisation_verticale = 0;
    Lumiere = 2;
    frequence_lumiere = 0.01;
    Ombre = 1;
    Reflection = 1;
    Distortion = 0;
    video = 1;
    postFX = 1;
    alpha = 128;
    Herbes = 1;
    particules = 1;
    lissage = 1;
    zoom = 1.4;
    volume = 50;
    music_volume = 100;
    Minimap = 0;
    console = 2;
    item_background = 1;
    frequence_sauvegarde = 10;
    debug = 1;
    desactivate_console = 0;
    language = "any";

    RafraichirOmbre   = 0;
    RafraichirLumiere = 0;

    if(!Lumiere)
        Ombre = 0;
}

void Configuration::Charger() {

    std::ifstream fichier("configuration.conf", std::ios::in);

    if(fichier.good()) {

        std::string chaine;
        while(fichier >> chaine) {

            if(chaine == "resolution:") {
                fichier >> Resolution.x >> Resolution.y;
                Resolution.w = Resolution.x;
                Resolution.h = Resolution.y;
            }
            if(chaine == "brightness:")         fichier >> luminosite;
            if(chaine == "contrast:")           fichier >> contrastes;
            if(chaine == "alpha_opacity:")      fichier >> alpha;
            if(chaine == "window:")             fichier >> mode_fenetre;
            if(chaine == "videos:")             fichier >> video;
            if(chaine == "postFX:")             fichier >> postFX;
            if(chaine == "smooth:")             fichier >> lissage;
            if(chaine == "shadows:")            fichier >> Ombre;
            if(chaine == "reflection:")         fichier >> Reflection;
            if(chaine == "distortion:")         fichier >> Distortion;
            if(chaine == "light_quality:")      fichier >> Lumiere;
            if(chaine == "grass:")              fichier >> Herbes;
            if(chaine == "particules:")         fichier >> particules;
            if(chaine == "vsync:")              fichier >> syncronisation_verticale;
            if(chaine == "volume:")             fichier >> volume;
            if(chaine == "music_volume:")       fichier >> music_volume;
            if(chaine == "minimap:")            fichier >> Minimap;
            if(chaine == "light_refresh_rate:") fichier >> frequence_lumiere;
            if(chaine == "console:")            fichier >> console;
            if(chaine == "item_background:")    fichier >> item_background;
            if(chaine == "saving_frequency:")   fichier >> frequence_sauvegarde;
            if(chaine == "debug_mod:")          fichier >> debug;
            if(chaine == "desactivate_console:")fichier >> desactivate_console;
            if(chaine == "language:")           fichier >> language;
            if(chaine == "zoom:")               fichier >> zoom;


        }
        fichier.close();
    }
    else {
        qWarning("Le fichier configuration.conf n'a pu être ouvert.");
        qWarning("   Création de ce fichier en cours...");
        Sauvegarder();
        Charger();
        return;
    }

    if(!Lumiere)
        Ombre = 0;
}


void Configuration::Sauvegarder() {
    //Sauvegarde de la configuration
    std::fstream fichier("configuration.conf", std::ios::out | std::ios::trunc) ;

    if (fichier) {
        fichier << "DISPLAY"<< std::endl;
        fichier <<  "resolution: "  << Resolution.w     <<  " " <<  Resolution.h << std::endl;
        fichier <<  "brightness: "  << luminosite               << std::endl;
        fichier <<  "contrast: "    << contrastes               << std::endl;
        fichier <<  "window: "      << mode_fenetre             << std::endl;
        fichier <<  "vsync: "       << syncronisation_verticale << std::endl;
        fichier <<  std::endl;
        fichier <<  "light_quality: "       << Lumiere              << std::endl;
        fichier <<  "light_refresh_rate: "  << frequence_lumiere    << std::endl;
        fichier <<  "shadows: "             << Ombre                << std::endl;
        fichier <<  "reflection: "          << Reflection           << std::endl;
        fichier <<  "distortion: "          << Distortion           << std::endl;
        fichier <<  "videos: "              << video                << std::endl;
        fichier <<  "postFX: "              << postFX               << std::endl;
        fichier <<  "alpha_opacity: "       << alpha                << std::endl;
        fichier <<  "grass: "               << Herbes               << std::endl;
        fichier <<  "particules: "          << particules           << std::endl;
        fichier <<  "smooth: "              << lissage              << std::endl;
        fichier <<  "zoom: "                << zoom                 << std::endl;
        fichier <<  std::endl;
        fichier <<  std::endl;

        fichier << "SOUNDS" << std::endl;
        fichier <<  "volume: "          << volume       << std::endl;
        fichier <<  "music_volume: "    << music_volume << std::endl;

        fichier <<  std::endl;
        fichier <<  std::endl;

        fichier << "INTERFACE"<< std::endl;
        fichier <<  "minimap: "         << Minimap          << std::endl;
        fichier <<  "console: "         << console          << std::endl;
        fichier <<  "item_background: " << item_background  << std::endl;

        fichier <<  std::endl;
        fichier <<  std::endl;

        fichier << "OTHERS" << std::endl;
        fichier << "saving_frequency: "     << frequence_sauvegarde << std::endl;
        fichier << "debug_mod: "            << debug                << std::endl;
        fichier << "desactivate_console: "  << desactivate_console  << std::endl;
        fichier << "language: "             << language             << std::endl;



        fichier.close();
    }
    else
        qWarning("Impossible de sauvegarder la configuration.");
}
