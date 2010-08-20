#include <QDesktopWidget>
#include <QProgressBar>
#include <QApplication>
#include <QBitmap>
#include <QPushButton>
#include <QMessageBox>
#include <QtWebKit>

#include "defines.h"
#include "optionsjeu.h"




#ifdef WINDOWS
    #include <windows.h>
#endif

#ifdef LINUX
    #include <stdlib.h>
#endif





typedef struct {
    std::string alias;
    std::string langue;
} Langage;
typedef struct {
    std::string repertoire;
    std::string nom;
    unsigned int version;
} Fichier;

class Launcher : public QWidget // On hérite de QWidget (IMPORTANT)
{
    Q_OBJECT

    public:
        Launcher();
        ~Launcher();
        inline void ajouterFichierDansTelecharges(const Fichier &fichier);

        std::string no_language;

    public slots:
        void telechargerFichierMAJDistant();
        void verificationPresenceMiseAJour(bool);
        void LancerJeu();

        void languageChange(int index);


        void miseAJourBarre2(int progressions, int temp);
        void miseAJourBarreTotale(float progressions, int temp);
        void miseAJourBarre(int fait, int total);

        void telechargerFichier(bool erreur=false);

    private:
            OptionsJeu      *_optionsJeu;

            QPushButton     *m_boutonDemarrer,
                            *m_boutonQuitter,
                            *m_boutonMettreAJour,
                            *m_boutonOptions;
            QProgressBar    *m_miseAJour,
                            *m_miseAJourTotale;


            QFile           *_fichier;
            QHttp           *_http;
            QList<Langage>  *_langages;
            QList<Fichier>  *_fichiersLocaux;
            QQueue<Fichier> *_fichiersATelecharger;

            Fichier         *_fichierEnTelechargement;


            int             _nombreDeFichiersTelecharges;
            bool            _installer;


};

