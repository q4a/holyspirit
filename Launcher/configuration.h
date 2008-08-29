#include <iostream>
#include <fstream>
#include <QApplication>
#include <QPushButton>
#include <QtGui>
#include <QProgressBar>
#include <QSplashScreen>
#include <QPixmap>


struct coordonnee
{
    int x,y,w,h;
};

struct Configuration
{
    void Charger()
    {
            coordonnee resolution;
            ///Chargement de la configuration

            std::ifstream fichier;
           /* fichier3.open("configuration.conf", std::ios::in);
            if(!fichier3)
            {
                fichier3.close();
                std::fstream fichier2("configuration.conf", std::ios::in | std::ios::out | std::ios::trunc) ;

                fichier2<<"AFFICHAGE\nresolution: 800 600\nluminosite: 0\ncontrastes: 1\nsyncronisation_verticale: 1\nmode_fenetre: 1\nombres: 1\nlumieres: 1\namelioration_des_lampes: 0\nfrequence_lumiere: 0.054\nherbes: 1\npostFX: 1\nsang: 1\n\nMUSIQUES\nvolume: 100\nson_mort: Data/Sons/Coeur.wav\n\nINTERFACE\nminimap: 1\nconsole: 0\n\nVERSION\nversion: 0.0.98\n\nCHEMINS\nmaps: Data/Maps/map\ntemps: Data/Temp/\nsaves: Data/Saves/\nevenements: Data/Maps/evenement\ncurseur: Data/Menus/Curseurs/\nmenus: Data/Menus/\nfonts: Data/Fonts/Cup and Talon.ttf\nfx: Data/Effets/\n\ncurseur_base: curseurMenu.png\n\nfxNoir: EffetNoir.sfx\nfxContrastes: EffetContrastes.sfx\nfxMort: EffetMort.sfx\n\nmenu_hud: Hudoo.png\nmenu_minimap: minimap.png\nmenu_bulle_vie: bulle_vie.png\nmenu_ame: Ame.png\nmenu_barre_ame: Barre_ame.png\nmenu_barre_vie: barre_vie.png\nmenu_barre_vie_vide: barre_vie_vide.png\nmenu_sang: sang.png\nmenu_inventaire: inventaire.png\n\nAUTRE\nvideos: 0\nchemin_video_developpeurs: Data/Videos/Naiponcin logo/\nnom_video_developpeurs: Naiponcin logo\n\ncache: 200\n\nfrequence_sauvegarde: 10\n";

                fichier2.close();

                //fichier.seekg(0, std::ios::beg);
            }
            fichier.close();*/

            fichier.open("configuration.conf", std::ios::in);
            if(fichier)
            {
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
            {
                std::cerr<<"Test";
                throw std::string("Impossible de charger la configuration");
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
                        fichier<<"CHEMINS"<<std::endl;

                        fichier<< "maps: " <<chemin_maps<<std::endl;
                        fichier<< "temps: " <<chemin_temps<<std::endl;
                        fichier<< "saves: " <<chemin_saves<<std::endl;
                        fichier<< "evenements: " <<chemin_evenements<<std::endl;
                        fichier<< "curseur: " <<chemin_curseurs<<std::endl;
                        fichier<< "menus: " <<chemin_menus<<std::endl;
                        fichier<< "fonts: " <<chemin_fonts<<std::endl;
                        fichier<< "fx: " <<chemin_fx<<std::endl;
                        fichier<< "chemin_video_developpeurs: " <<chemin_video_developpeur<<std::endl;
                        fichier<< "nom_video_developpeurs: " <<nom_video_developpeur<<std::endl;
                        fichier<< "son_mort: " <<chemin_son_mort<<std::endl;
                        fichier<<std::endl;
                        fichier<< "curseur_base: " <<nom_curseur_base<<std::endl;
                        fichier<< "fxNoir: " <<nom_effetNoir<<std::endl;
                        fichier<< "fxMort: " <<nom_effetMort<<std::endl;
                        fichier<< "fxContrastes: " <<nom_effetContrastes<<std::endl;
                        fichier<< "menu_hud: " <<nom_hud<<std::endl;
                        fichier<< "menu_minimap: " <<nom_minimap<<std::endl;
                        fichier<< "menu_bulle_vie: " <<nom_bulle_vie<<std::endl;
                        fichier<< "menu_ame: " <<nom_ame<<std::endl;
                        fichier<< "menu_barre_ame: " <<nom_barre_ame<<std::endl;
                        fichier<< "menu_barre_vie: " <<nom_barre_vie<<std::endl;
                        fichier<< "menu_barre_vie_vide: " <<nom_barre_vie_vide<<std::endl;
                        fichier<< "menu_inventaire: " <<nom_inventaire<<std::endl;
                        fichier<< "menu_sang: " <<nom_sang<<std::endl;
                        fichier<<std::endl;
                        fichier<<std::endl;
                        fichier<<"AUTRES"<<std::endl;
                        fichier<< "version: " <<version<<std::endl;

                        fichier<< "nom_video_developpeurs: " <<nom_video_developpeur<<std::endl;
                        fichier<< "cache: " <<cache<<std::endl;
                        fichier<< "frequence_sauvegarde: " <<frequence_sauvegarde<<std::endl;


                 fichier.close();
            }
            else
                throw "Impossible de sauvegarder la configuration";
    }


    coordonnee Resolution;
    bool Ombre,Lumiere,Minimap,amelioration_lampes,console,Herbes,syncronisation_verticale,mode_fenetre,postFX,sang,video;
    float effetMort,volume,minute,zoom,frequence_sauvegarde,frequence_lumiere,luminosite,contrastes;
    int heure,cache;
    std::string version,chemin_maps,chemin_temps,chemin_saves,chemin_evenements,chemin_curseurs,chemin_menus,chemin_fonts,chemin_fx,nom_curseur_base,nom_effetNoir,nom_effetMort,nom_effetContrastes,nom_hud,nom_minimap,nom_bulle_vie,nom_ame,nom_barre_ame,nom_barre_vie,nom_barre_vie_vide,nom_sang,nom_inventaire,chemin_son_mort;
    std::string chemin_video_developpeur,nom_video_developpeur;
};




class OptionsJeu : public QDialog // On hérite de QWidget (IMPORTANT)
{
    Q_OBJECT

    public:
    OptionsJeu();
    OptionsJeu::~OptionsJeu()
    {
        delete m_boutonRetour;
        delete text;
        delete text2;
        delete text3;
        delete text4;

        delete barreTauxContrastes;
        delete barreTauxLuminosite;
        delete barreVolume;

        delete modeFenetre;
        delete syncro;
        delete lumieres;
        delete ombres;
        delete fx;
        delete sang;
        delete herbes;

        delete listeResolution;
    }

    public slots:
    void ChangerContraste(int );
    void ChangerLuminosite(int );
    void ChangerVolume(int );

    void Quitter();

    private:
    QPushButton *m_boutonRetour;
    QLabel *text,*text2,*text3,*text4;
    QSlider *barreTauxContrastes,*barreTauxLuminosite,*barreVolume;
    QCheckBox *modeFenetre,*syncro,*lumieres,*ombres,*fx,*sang,*herbes;
    QComboBox *listeResolution;

    std::vector <coordonnee> resolutions;

     Configuration config;
};


