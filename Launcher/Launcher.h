#include <QApplication>
#include <QPushButton>
#include <QtGui>
#include <QHttp>
#include <QProgressBar>
#include <QSplashScreen>
#include <QPixmap>
#include <dirent.h>
#include <QDir>


#include <QtWebKit>


#define WINDOWS

#ifdef WINDOWS
#include <windows.h>
#endif

#include "configuration.h"

class Launcher : public QWidget // On hérite de QWidget (IMPORTANT)
{
    Q_OBJECT

    public:
    Launcher();
    ~Launcher()
    {
        delete m_boutonDemarrer;
        delete m_boutonQuitter;
        delete m_boutonMettreAJour;
        delete m_fond;
        delete m_miseAJour;
        delete m_miseAJourTotale;
        delete m_boutonOptions;

        delete m_pixmap;

        delete dir;

        delete pageWeb;

        delete listeLangages;

        liste_a_telecharger_nom.clear();
        liste_a_telecharger_rep.clear();
        listeRepertoire.clear();
        nom_fichier.clear();
        nom_fichier_nv.clear();
        rep_fichier_nv.clear();
        ver_fichier.clear();
        ver_fichier_nv.clear();
    }

    std::string no_language;

    public slots:
    void downloadFile();
    void downloadAll(bool);
    void LancerJeu();
    void Options();

    void languageChange(int index);

    void MettreAJourBDD();

    void miseAJourBarre2(int progressions, int temp);
    void miseAJourBarreTotale(float progressions, int temp);
    void miseAJourBarre(int fait, int total);

    void telechargerFichier(bool erreur=false);

    private:
    QPushButton *m_boutonDemarrer,*m_boutonQuitter,*m_boutonMettreAJour,*m_boutonOptions;
    QLabel *m_fond;
    QPixmap *m_pixmap;
    QProgressBar *m_miseAJour,*m_miseAJourTotale;

    QWebView *pageWeb;

    QFile *fichier;
    QHttp *http;
    QDir *dir;

    QComboBox *listeLangages;
    std::vector <std::string> languages;
    std::vector <std::string> languages_txt;

    std::vector <std::string> liste_a_telecharger_nom;
    std::vector <std::string> liste_a_telecharger_rep;
    std::vector <std::string> listeRepertoire;
    std::vector <std::string> nom_fichier,nom_fichier_nv,rep_fichier_nv;


    std::vector <std::string> final_nom;
    std::vector <std::string> final_rep;
    std::vector <int> final_ver;


    std::vector <int> ver_fichier,ver_fichier_nv,liste_a_telecharger_ver;
    int enCoursDeTelechargement;

    int httpGetId,fait,aFaire;
    float fait2;

    bool telechargementFini,m_installer;

    OptionsJeu optionsJeu;

};

