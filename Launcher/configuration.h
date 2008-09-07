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
    bool Ombre,Lumiere,Minimap,amelioration_lampes,console,Herbes,syncronisation_verticale,mode_fenetre,postFX,sang,video;
    float effetMort,volume,minute,zoom,frequence_sauvegarde,frequence_lumiere,luminosite,contrastes;
    int heure,cache;
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


