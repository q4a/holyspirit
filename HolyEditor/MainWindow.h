#ifndef DEF_MAINWINDOW
#define DEF_MAINWINDOW

#include "SFMLCanvas.h"
#include "globale.h"

#include <QApplication>
#include <QVBoxLayout>
#include <QFrame>
#include <QLabel>
#include <QMenuBar>
#include <QFileDialog>
#include <QTabWidget>
#include <QTreeWidget>
#include <QListWidget>
#include <QRadioButton>
#include <QSpinBox>
#include <QCheckBox>


#include <iostream>

class MainWindow : public QWidget // On hérite de QWidget (IMPORTANT)
{
    Q_OBJECT

    public:
    MainWindow();
    ~MainWindow();

    void MettreListesAJour();

    public slots:
    void nouveau();
    void s_creerNouveau();
    void s_annulerNouveau();

    void ouvrir();
    void importerTileset();
    void importerHerbe();
    void importerEntite();
    void importerEvenement();
    void sauver();
    void sauverAs();
    void undo();
    void redo();

    void selectTileset  (QTreeWidgetItem *, int);
    void selectHerbe    (QTreeWidgetItem *, int);
    void selectEntite   (QTreeWidgetItem *, int);
    void selectEvenement(QTreeWidgetItem *, int);

    void addTileset     (QTreeWidgetItem *, int);
    void addHerbe       (QTreeWidgetItem *, int);
    void addEntite      (QTreeWidgetItem *, int);
    void addEvenement   (QTreeWidgetItem *, int);

    protected:

    void paintEvent(QPaintEvent*);
    void backup();

    private:
    MyCanvas        *SFMLView;
    QVBoxLayout     *layout;
    QHBoxLayout     *layout2;
    QVBoxLayout     *layout3;

    QWidget         *fenetreNouveau;

    QLineEdit       *nomMap;
    QSpinBox        *tailleX;
    QSpinBox        *tailleY;
    QPushButton     *okNouveau;
    QPushButton     *annulerNouveau;


    QFrame          *menuRessources;
    QFrame          *menuBrush;
    QFrame          *menuInfos;

    QMenuBar        *menu;
    QMenu           *menuFichier;
    QMenu           *menuImporterRessources;
    QMenu           *menuAide;

    QTabWidget      *ongletRessources;

    QTreeWidget     *listTileset;
    QTreeWidget     *listHerbe;
    QTreeWidget     *listEntites;
    QTreeWidget     *listEvenements;


    QRadioButton    *couche0;
    QRadioButton    *couche1;

    QCheckBox       *afficherCollisions;
    QCheckBox       *afficherMurLumiere;

    QCheckBox       *afficherCouche0;
    QCheckBox       *afficherCouche1;

    QSpinBox        *modifLayer;
    QSpinBox        *moduleAleatoireMin;
    QSpinBox        *moduleAleatoireMax;
    QSpinBox        *moduleAleatoireCur;
    QSpinBox        *taillePinceau;

    QLabel          *position;

    QAction         *actionNouveau;
    QAction         *actionOuvrir;
    QAction         *actionSauver;
    QAction         *actionSauverAs;
    QAction         *actionUndo;
    QAction         *actionRedo;
    QAction         *actionQuitter;

    QAction         *actionImporterTileset;
    QAction         *actionImporterHerbe;
    QAction         *actionImporterEntite;
    QAction         *actionImporterEvenement;
    QAction         *actionOptionsMap;

    QString          chemin;

    QTimer           myTimer;

    bool             m_dejaWheel;
    coordonnee       m_oldmouse;

    int              m_redoMax;
    int              m_undoMax;
    bool             m_tourUndo;
};

#endif

