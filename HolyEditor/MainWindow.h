
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
#include <QTextEdit>

#include "highlighter.h"

#include <iostream>

class MainWindow : public QWidget // On h�rite de QWidget (IMPORTANT)
{
    Q_OBJECT

    public:
    MainWindow();
    ~MainWindow();

    void setEntiteGraphiqueParametres(Entite_graphique &);

    void MettreListesAJour();

    public slots:
    void nouveau();
    void s_creerNouveau();
    void s_annulerNouveau();

    void ouvrir();
    void importerTileset();
    void importerHerbe();
    void importerEntite();
    void sauver();
    void sauverAs();
    void undo();
    void redo();

    void script();

    void selectTileset  (QTreeWidgetItem *, int);
    void selectHerbe    (QTreeWidgetItem *, int);
    void selectEntite   (QTreeWidgetItem *, int);

    void addTileset     (QTreeWidgetItem *, int);
    void addHerbe       (QTreeWidgetItem *, int);
    void addEntite      (QTreeWidgetItem *, int);

    protected:

    void paintEvent(QPaintEvent*);
    void backup();

    private:
    MyCanvas        *SFMLView;
    QVBoxLayout     *layout;
    QHBoxLayout     *layout2;
    QVBoxLayout     *layout3;

    QWidget         *fenetreNouveau;
    QWidget         *win_script;

    QTextEdit       *text_script;
    Highlighter     *highlighter;


    QLineEdit       *nomMap;
    QSpinBox        *tailleX;
    QSpinBox        *tailleY;
    QPushButton     *okNouveau;
    QPushButton     *annulerNouveau;


    QFrame          *menuRessources;
    QFrame          *menuInfos;

    QMenuBar        *menu;
    QMenu           *menuFichier;
    QMenu           *menuEdition;
    QMenu           *menuImporterRessources;
    QMenu           *menuMap;
    QMenu           *menuAide;

    QTabWidget      *ongletRessources;

    QTreeWidget     *listTileset;
    QTreeWidget     *listHerbe;
    QTreeWidget     *listEntites;


    QRadioButton    *couche0;
    QRadioButton    *couche1;

    QCheckBox       *afficherCollisions;
    QCheckBox       *afficherRelief;
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
    QAction         *actionOptionsMap;
    QAction         *actionScriptMap;



    QSpinBox        *decalX, *rand_decalX,
                    *decalY, *rand_decalY,
                    *scaleX, *rand_scaleX,
                    *scaleY, *rand_scaleY,
                    *rotate, *rand_rotate,
                    *colorR, *rand_colorR,
                    *colorG, *rand_colorG,
                    *colorB, *rand_colorB,
                    *colorA, *rand_colorA;

    QCheckBox       *lie_scale,
                    *lie_colorR, *lie_colorG, *lie_colorB;

    QString          chemin;

    QTimer           myTimer;

    bool             m_dejaWheel;
    coordonnee       m_oldmouse;

    int              m_redoMax;
    int              m_undoMax;
    bool             m_tourUndo;
    bool             old_focus;

    coordonnee       m_old_param_mouse;
    bool             m_rescale;
    bool             m_translate;
    bool             m_rotate;
};

#endif

