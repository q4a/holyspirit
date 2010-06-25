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

class Configuration
{
    public :

    Configuration()
    {
        language = "any";
    }
    ~Configuration()
    {
    }

    void Charger();
    void ChargerConf();

    void Sauvegarder();

    coordonnee Resolution;
    bool Ombre,Minimap,amelioration_lampes,Herbes,syncronisation_verticale,mode_fenetre,postFX,video,particules,debug,desactivate_console,lissage,Reflection;
    int Lumiere,console;
    float effetMort,effetNoir,volume,music_volume,minute,zoom,frequence_sauvegarde,frequence_lumiere,luminosite,contrastes;
    int heure,cache,alpha;
    std::string version,chemin_maps,chemin_items,chemin_temps,chemin_saves,chemin_evenements,chemin_curseurs,chemin_menus,chemin_fonts,chemin_fx,nom_curseur_base,nom_effetNoir,nom_effetMort,nom_effetContrastes,nom_effetBlur,nom_effetFiltre,nom_minimap,nom_ame,nom_barre_ame,nom_barre_vie,nom_barre_vie_vide,nom_sang,chemin_son_mort;
    std::string font_titre,font,chemin_video_developpeur,nom_video_developpeur,nom_sac,nom_corner,chemin_aa;
    bool item_background,Distortion;

    std::string chemin_text_benedictions,chemin_text_menus;

    std::string language;

    bool RafraichirLumiere;
    int numero_screen,RafraichirOmbre;
};




class OptionsJeu : public QDialog // On hérite de QWidget (IMPORTANT)
{
    Q_OBJECT

    public:
    OptionsJeu();
    ~OptionsJeu()
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
        delete reflect;
        delete ombres;
        delete fx;
        delete distortion;
        delete item_background;
        delete herbes;
        delete particule;
        delete lissage;

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
    QCheckBox *modeFenetre,*syncro,*ombres,*fx,*distortion,*item_background,*herbes,*particule,*lissage,*reflect;
    QComboBox *listeResolution;

    std::vector <coordonnee> resolutions;

    Configuration config;
};


