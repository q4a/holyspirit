#include <QApplication>
#include <QPushButton>
#include <QtGui>
#include <QHttp>
#include <QProgressBar>
#include <QSplashScreen>
#include <QPixmap>
#include <dirent.h>
#include <QDir>

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
    Launcher::~Launcher()
    {
        delete m_boutonDemarrer;
        delete m_boutonQuitter;
        delete m_boutonMettreAJour;
        delete m_fond;
        delete m_miseAJour;
        delete m_miseAJourTotale;
        delete m_boutonOptions;

        delete m_pixmap;

        liste_a_telecharger_nom.clear();
        liste_a_telecharger_rep.clear();
        listeRepertoire.clear();
        nom_fichier.clear();
        nom_fichier_nv.clear();
        rep_fichier_nv.clear();
        ver_fichier.clear();
        ver_fichier_nv.clear();

       // delete http;
    }



    public slots:
    void downloadFile();
    void downloadAll(bool);
    void LancerJeu();
    void Options();

    void miseAJourBarre2(int progressions, int temp);
    void miseAJourBarreTotale(float progressions, int temp);
    void miseAJourBarre(int fait, int total);

    void telechargerFichier(bool);

    private:
    QPushButton *m_boutonDemarrer,*m_boutonQuitter,*m_boutonMettreAJour,*m_boutonOptions;
    QLabel *m_fond;
    QPixmap *m_pixmap;
    QProgressBar *m_miseAJour,*m_miseAJourTotale;

    QFile *fichier;
    QHttp *http;

    std::vector <std::string> liste_a_telecharger_nom,;
    std::vector <std::string>  liste_a_telecharger_rep;
    std::vector <std::string> listeRepertoire;
    std::vector <std::string> nom_fichier,nom_fichier_nv,rep_fichier_nv;
    std::vector <int> ver_fichier,ver_fichier_nv;
    int enCoursDeTelechargement;

    //QNetworkAccessManager manager;

    int httpGetId,fait,aFaire;
    float fait2;

    bool telechargementFini;

    OptionsJeu optionsJeu;

};

