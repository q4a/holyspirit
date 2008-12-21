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

            std::ifstream fichier,fichier2;

            fichier.open("configuration.conf", std::ios::in);
            if(fichier)
            {
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
                        if(chaine== "vsync:") { fichier>>syncronisation_verticale; }
                        if(chaine== "volume:") { fichier>>volume; }
                        if(chaine== "minimap:") { fichier>>Minimap; }
                        if(chaine== "light_refresh_rate:") { fichier>>frequence_lumiere; }
                        if(chaine== "console:") { fichier>>console; }

                        if(chaine== "saving_frequency:") { fichier>>frequence_sauvegarde; }
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


                 fichier.close();
            }
            else
                throw "Impossible de sauvegarder la configuration";
    }

    coordonnee Resolution;
    int Lumiere;
    bool Ombre,Minimap,amelioration_lampes,console,Herbes,syncronisation_verticale,mode_fenetre,postFX,sang,video;
    float effetMort,volume,minute,zoom,frequence_sauvegarde,frequence_lumiere,luminosite,contrastes;
    int heure,cache,alpha;
    std::string version,chemin_maps,chemin_temps,chemin_saves,chemin_evenements,chemin_curseurs,chemin_menus,chemin_fonts,chemin_fx,nom_curseur_base,nom_effetNoir,nom_effetMort,nom_effetContrastes,nom_effetBlur,nom_hud,nom_minimap,nom_bulle_vie,nom_ame,nom_barre_ame,nom_barre_vie,nom_barre_vie_vide,nom_sang,nom_inventaire,chemin_son_mort;
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
        delete text5;
        delete text6;

        delete barreTauxContrastes;
        delete barreTauxLuminosite;
        delete barreVolume;
        delete barreLumiere;
        delete barreAlpha;

        delete modeFenetre;
        delete syncro;
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
    void ChangerLumiere(int );
    void ChangerAlpha(int );

    void Quitter();

    private:
    QPushButton *m_boutonRetour;
    QLabel *text,*text2,*text3,*text4,*text5,*text6;
    QSlider *barreTauxContrastes,*barreTauxLuminosite,*barreVolume,*barreLumiere,*barreAlpha;
    QCheckBox *modeFenetre,*syncro,*ombres,*fx,*sang,*herbes;
    QComboBox *listeResolution;

    std::vector <coordonnee> resolutions;

     Configuration config;
};


