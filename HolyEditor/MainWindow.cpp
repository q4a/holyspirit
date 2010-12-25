
#include "MainWindow.h"
#include "highlighter.moc"

#include <iostream>
#include <sstream>
#include "Map/map.h"
#include "globale.h"

#include <QApplication>
#include <QPushButton>
#include <QtGui>

extern Map *map;

MainWindow::MainWindow() : QWidget()
{
    setWindowTitle(tr("HolyEditor"));

    m_dejaWheel = false;
    m_redoMax   = 0;
    m_undoMax   = 0;
    m_tourUndo  = 0;

    old_focus   = true;

    map = NULL;


    QFont font;
     font.setFamily("Courier");
     font.setFixedPitch(true);
     font.setPointSize(8);

    win_script      = new QWidget();
    win_script->setGeometry(200,200,600,600);
    text_script = new QTextEdit();
    text_script->setGeometry(200,200,600,600);
    text_script->setLineWrapMode(QTextEdit::NoWrap);
    text_script->setFont(font);
    //text_script->setColor(QColor(0,255,0));
    text_script->setStyleSheet("color:red;");

    highlighter = new Highlighter(text_script->document());

    fenetreNouveau      = new QWidget();
    fenetreNouveau->setGeometry(200,200,200,138);
    nomMap              = new QLineEdit     ("Nom",fenetreNouveau);
    nomMap->setGeometry(4,4,180,20);
    tailleX             = new QSpinBox      (fenetreNouveau);
    tailleX->setGeometry(4,34,140,20);
    tailleX->setPrefix("X : ");
    tailleX->setMaximum ( 500 );
    tailleX->setMinimum ( 3 );
    tailleY             = new QSpinBox      (fenetreNouveau);
    tailleY->setGeometry(4,64,140,20);
    tailleY->setPrefix("Y : ");
    tailleY->setMaximum ( 500 );
    tailleY->setMinimum ( 3 );


    okNouveau           = new QPushButton   ("Créer", fenetreNouveau);
    okNouveau->move(4,94);
    annulerNouveau      = new QPushButton   ("Annuler", fenetreNouveau );
    annulerNouveau->move(104,94);

    menu                = new QMenuBar(this);

    menu->setGeometry(0, 0, this->geometry().width(), 24);

    menuFichier     = menu->addMenu(tr("&Fichier"));
    actionNouveau   = menuFichier->addAction("&Nouvelle Map");
    actionNouveau->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_N));

    actionOuvrir    = menuFichier->addAction("&Ouvrir Map");
    actionOuvrir->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_O));

    actionSauver    = menuFichier->addAction("&Sauvegarder");
    actionSauver->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_S));

    actionSauverAs  = menuFichier->addAction("&Sauvegarder sous");

    actionQuitter   = menuFichier->addAction("&Quitter");
    actionQuitter->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_Q));



    menuEdition      = menu->addMenu(tr("&Edition"));

    actionUndo     = menuEdition->addAction("&Undo");
    actionUndo->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_Z));

    actionRedo     = menuEdition->addAction("&Redo");
    actionRedo->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_Y));



    menuImporterRessources  = menu->addMenu(tr("&Ressources"));
    actionImporterTileset   = menuImporterRessources->addAction("&Importer Tileset");
    actionImporterTileset->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_T));

    actionImporterHerbe     = menuImporterRessources->addAction("&Importer Herbe");
    actionImporterHerbe->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_H));

    actionImporterEntite    = menuImporterRessources->addAction("&Importer Entité");
    actionImporterEntite->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_E));


    menuMap        = menu->addMenu(tr("&Map"));

    actionOptionsMap        = menuMap->addAction("&Options de la map");
    actionScriptMap         = menuMap->addAction("&Script de la map");


    menuAide        = menu->addMenu(tr("&Aide"));

    // Create a SFML view inside the main frame
    SFMLView        = new MyCanvas;
    moteurGraphique->m_ecran = SFMLView;

    menuInfos       = new QFrame;

    menuInfos       ->setFrameShape(QFrame::StyledPanel);

    listTileset      = new QTreeWidget();
    listHerbe        = new QTreeWidget();
    listEntites      = new QTreeWidget();

    listTileset     ->setSortingEnabled(true);
    listHerbe       ->setSortingEnabled(true);
    listEntites     ->setSortingEnabled(true);

    ongletRessources    = new QTabWidget();
    ongletRessources->addTab(listTileset, "Tilesets");
    ongletRessources->addTab(listHerbe, "Herbes");
    ongletRessources->addTab(listEntites, "Entités");

    couche0 = new QRadioButton("Couche no 1 (Tout ce qui est sol)", menuInfos);
    couche0->setChecked(true);
    couche0->move(4,0);
    couche1 = new QRadioButton("Couche no 2 (Tout ce qui est décor)", menuInfos);
    couche1->move(4,20);

    afficherCollisions = new QCheckBox("Afficher les collisions", menuInfos);
    afficherCollisions->move(4,60);
    afficherMurLumiere = new QCheckBox("Afficher les murs pour la lumière", menuInfos);
    afficherMurLumiere->move(4,80);

    afficherCouche0 = new QCheckBox("Afficher la couche 1", menuInfos);
    afficherCouche0->move(4,100);
    afficherCouche0->setChecked(true);
    afficherCouche1 = new QCheckBox("Afficher la couche 2", menuInfos);
    afficherCouche1->move(4,120);
    afficherCouche1->setChecked(true);

    afficherRelief = new QCheckBox("Afficher le relief", menuInfos);
    afficherRelief->move(204,80);

    modifLayer = new QSpinBox(menuInfos);
    modifLayer->setGeometry(4,40,200,18);
    modifLayer->setMaximum ( 5 );
    modifLayer->setMinimum ( -5 );
    modifLayer->setPrefix ( tr("Modificateur de couche : ") );

    moduleAleatoireMin = new QSpinBox(menuInfos);
    moduleAleatoireMin->setGeometry(204,0,200,18);
    moduleAleatoireMin->setMaximum ( 9 );
    moduleAleatoireMin->setMinimum ( 0 );
    moduleAleatoireMin->setValue ( 0 );
    moduleAleatoireMin->setPrefix ( tr("Module aléatoire minimum : ") );

    moduleAleatoireMax = new QSpinBox(menuInfos);
    moduleAleatoireMax->setGeometry(204,20,200,18);
    moduleAleatoireMax->setMaximum ( 9 );
    moduleAleatoireMax->setMinimum ( 0 );
    moduleAleatoireMax->setValue ( 9 );
    moduleAleatoireMax->setPrefix ( tr("Module aléatoire maximum : ") );

    moduleAleatoireCur = new QSpinBox(menuInfos);
    moduleAleatoireCur->setGeometry(204,40,200,18);
    moduleAleatoireCur->setMaximum ( 9 );
    moduleAleatoireCur->setMinimum ( 0 );
    moduleAleatoireCur->setValue ( 0 );
    moduleAleatoireCur->setPrefix ( tr("Module aléatoire courant : ") );

    taillePinceau = new QSpinBox(menuInfos);
    taillePinceau->setGeometry(204,60,200,18);
    taillePinceau->setMaximum ( 30 );
    taillePinceau->setMinimum ( 1 );
    taillePinceau->setValue ( 1 );
    taillePinceau->setPrefix ( tr("Taille pinceau : ") );

    {
        decalX             = new QSpinBox      (menuInfos);
        decalX->setGeometry(414,0,64,20);
        decalX->setPrefix("T.X : ");
        decalX->setMinimum(-64);
        decalX->setMaximum(64);

        rand_decalX             = new QSpinBox      (menuInfos);
        rand_decalX->setGeometry(494,0,64,20);
        rand_decalX->setPrefix("+/- : ");

        decalY             = new QSpinBox      (menuInfos);
        decalY->setGeometry(414,20,64,20);
        decalY->setPrefix("T.Y : ");
        decalY->setMinimum(-64);
        decalY->setMaximum(64);

        rand_decalY             = new QSpinBox      (menuInfos);
        rand_decalY->setGeometry(494,20,64,20);
        rand_decalY->setPrefix("+/- : ");

        scaleX             = new QSpinBox      (menuInfos);
        scaleX->setGeometry(414,40,64,20);
        scaleX->setPrefix("S.X : ");
        scaleX->setMaximum(500);
        scaleX->setMinimum(-500);
        scaleX->setValue(100);

        rand_scaleX             = new QSpinBox      (menuInfos);
        rand_scaleX->setGeometry(494,40,64,20);
        rand_scaleX->setPrefix("+/- : ");
        rand_scaleX->setMaximum(500);

        scaleY             = new QSpinBox      (menuInfos);
        scaleY->setGeometry(414,60,64,20);
        scaleY->setPrefix("S.Y : ");
        scaleY->setMinimum(-500);
        scaleY->setMaximum(500);
        scaleY->setValue(100);

        rand_scaleY             = new QSpinBox      (menuInfos);
        rand_scaleY->setGeometry(494,60,64,20);
        rand_scaleY->setPrefix("+/- : ");
        rand_scaleY->setMaximum(500);

        rotate             = new QSpinBox      (menuInfos);
        rotate->setGeometry(414,80,64,20);
        rotate->setPrefix("R : ");
        rotate->setMinimum(-360);
        rotate->setMaximum(360);

        rand_rotate             = new QSpinBox      (menuInfos);
        rand_rotate->setGeometry(494,80,64,20);
        rand_rotate->setPrefix("+/- : ");
        rand_rotate->setMaximum(360);

        colorR             = new QSpinBox      (menuInfos);
        colorR->setGeometry(578,0,64,20);
        colorR->setPrefix("C.R : ");
        colorR->setMaximum ( 255 );
        colorR->setMinimum ( 0 );
        colorR->setValue ( 255 );
        rand_colorR             = new QSpinBox      (menuInfos);
        rand_colorR->setGeometry(642,0,64,20);
        rand_colorR->setPrefix("+/- : ");
        rand_colorR->setMaximum ( 255 );
        rand_colorR->setMinimum ( 0 );
        colorG             = new QSpinBox      (menuInfos);
        colorG->setGeometry(578,20,64,20);
        colorG->setPrefix("C.G : ");
        colorG->setMaximum ( 255 );
        colorG->setMinimum ( 0);
        colorG->setValue ( 255 );
        rand_colorG             = new QSpinBox      (menuInfos);
        rand_colorG->setGeometry(642,20,64,20);
        rand_colorG->setPrefix("+/- : ");
        rand_colorG->setMaximum ( 255 );
        rand_colorG->setMinimum ( 0 );
        colorB             = new QSpinBox      (menuInfos);
        colorB->setGeometry(578,40,64,20);
        colorB->setPrefix("C.B : ");
        colorB->setMaximum ( 255 );
        colorB->setMinimum ( 0 );
        colorB->setValue ( 255 );
        rand_colorB             = new QSpinBox      (menuInfos);
        rand_colorB->setGeometry(642,40,64,20);
        rand_colorB->setPrefix("+/- : ");
        rand_colorB->setMaximum ( 255 );
        rand_colorB->setMinimum ( 0 );
        colorA             = new QSpinBox      (menuInfos);
        colorA->setGeometry(578,60,64,20);
        colorA->setPrefix("C.A : ");
        colorA->setMaximum ( 255 );
        colorA->setMinimum ( 0 );
        colorA->setValue ( 255 );
        rand_colorA             = new QSpinBox      (menuInfos);
        rand_colorA->setGeometry(642,60,64,20);
        rand_colorA->setPrefix("+/- : ");
        rand_colorA->setMaximum ( 255 );
        rand_colorA->setMinimum ( 0 );

        lie_scale = new QCheckBox(menuInfos);
        lie_scale->setGeometry(478,50,20,20);
        lie_colorR = new QCheckBox(menuInfos);
        lie_colorR->setGeometry(706,0,20,20);
        lie_colorG = new QCheckBox(menuInfos);
        lie_colorG->setGeometry(706,20,20,20);
        lie_colorB = new QCheckBox(menuInfos);
        lie_colorB->setGeometry(706,40,20,20);
    }


    position = new QLabel(menuInfos);
    position->setGeometry(204,100,190,18);


    layout3          = new QVBoxLayout;
    layout3->addWidget(SFMLView, 4);
    layout3->addWidget(menuInfos, 1);

    layout2         = new QHBoxLayout;
    layout2->addWidget(ongletRessources, 3);
    layout2->addLayout(layout3, 8);


    // Create the main layout
    layout          = new QVBoxLayout;
    layout->addWidget(menu, 0);
    layout->addLayout(layout2, 1);
    this->setLayout(layout);

    connect(&myTimer, SIGNAL(timeout()), this, SLOT(repaint()));
    myTimer.start();

    connect(actionNouveau,  SIGNAL(triggered()), this, SLOT(nouveau()));
    connect(okNouveau,      SIGNAL(clicked()), this, SLOT(s_creerNouveau()));
    connect(annulerNouveau, SIGNAL(clicked()), this, SLOT(s_annulerNouveau()));

    connect(actionQuitter, SIGNAL(triggered()), qApp, SLOT(quit()));
    connect(actionOuvrir, SIGNAL(triggered()), this, SLOT(ouvrir()));
    connect(actionSauver, SIGNAL(triggered()), this, SLOT(sauver()));
    connect(actionSauverAs, SIGNAL(triggered()), this, SLOT(sauverAs()));
    connect(actionUndo, SIGNAL(triggered()), this, SLOT(undo()));
    connect(actionRedo, SIGNAL(triggered()), this, SLOT(redo()));
    connect(actionImporterTileset, SIGNAL(triggered()), this, SLOT(importerTileset()));
    connect(actionImporterHerbe  , SIGNAL(triggered()), this, SLOT(importerHerbe()));
    connect(actionImporterEntite , SIGNAL(triggered()), this, SLOT(importerEntite()));

    connect(actionScriptMap,  SIGNAL(triggered()), this, SLOT(script()));

    connect(listTileset     , SIGNAL(itemPressed(QTreeWidgetItem *, int)), this, SLOT(selectTileset  (QTreeWidgetItem *, int)));
    connect(listHerbe       , SIGNAL(itemPressed(QTreeWidgetItem *, int)), this, SLOT(selectHerbe    (QTreeWidgetItem *, int)));
    connect(listEntites     , SIGNAL(itemPressed(QTreeWidgetItem *, int)), this, SLOT(selectEntite   (QTreeWidgetItem *, int)));

    connect(listTileset     , SIGNAL(itemDoubleClicked (QTreeWidgetItem *, int)), this, SLOT(addTileset  (QTreeWidgetItem *, int)));
    connect(listHerbe       , SIGNAL(itemDoubleClicked (QTreeWidgetItem *, int)), this, SLOT(addHerbe    (QTreeWidgetItem *, int)));
    connect(listEntites     , SIGNAL(itemDoubleClicked (QTreeWidgetItem *, int)), this, SLOT(addEntite   (QTreeWidgetItem *, int)));
}

MainWindow::~MainWindow()
{

    delete decalX;
    delete rand_decalX;
    delete decalY;
    delete rand_decalY;
    delete scaleX;
    delete rand_scaleX;
    delete scaleY;
    delete rand_scaleY;
    delete rotate;
    delete rand_rotate;
    delete colorR;
    delete rand_colorR;
    delete colorG;
    delete rand_colorG;
    delete colorB;
    delete rand_colorB;
    delete colorA;
    delete rand_colorA;
    delete lie_scale;
    delete lie_colorR;
    delete lie_colorG;
    delete lie_colorB;

    delete modifLayer;
    delete moduleAleatoireMin;
    delete moduleAleatoireMax;
    delete moduleAleatoireCur;
    delete taillePinceau;

    delete position;

    delete couche0;
    delete couche1;

    delete afficherCollisions;
    delete afficherRelief;
    delete afficherMurLumiere;
    delete afficherCouche0;
    delete afficherCouche1;

    delete highlighter;


    delete listTileset;
    delete listHerbe;
    delete listEntites;

    delete ongletRessources;

    delete SFMLView;
    delete menuInfos;

    delete layout3;
    delete layout2;
    delete layout;

    delete actionNouveau;
    delete actionOuvrir;
    delete actionSauver;
    delete actionSauverAs;
    delete actionUndo;
    delete actionRedo;
    delete actionQuitter;

    delete actionImporterTileset;
    delete actionImporterHerbe;
    delete actionImporterEntite;
    delete actionOptionsMap;

    delete menuEdition;
    delete menuAide;
    delete menuFichier;
    delete menuMap;
    delete menuImporterRessources;

    delete menu;

    delete nomMap;
    delete tailleX;
    delete tailleY;
    delete okNouveau;
    delete annulerNouveau;

    delete fenetreNouveau;


    delete text_script;
    delete win_script;

    if (map != NULL)
        delete map;

    console->Ajouter("");
    console->Ajouter("Destruction des moteurs");

    moteurGraphique->Kill();
    moteurSons->Kill();
    configuration->Kill();
    eventManager->Kill();

    console->Ajouter("");
    console->Ajouter("EXIT_SUCCESS");

    console->Kill();
}

void MainWindow::paintEvent(QPaintEvent*)
{
    moteurGraphique->m_camera.SetCenter((eventManager->m_coordonnee.x - eventManager->m_coordonnee.y) * 64.0f ,
                                        (eventManager->m_coordonnee.x + eventManager->m_coordonnee.y) * 32.0f + 32.0f);
    moteurGraphique->m_camera.SetSize(SFMLView->geometry().width(), SFMLView->geometry().height());
    moteurGraphique->m_camera.Zoom(configuration->zoom);

    moteurGraphique->m_camera.Move(moteurGraphique->decalageCamera.x, moteurGraphique->decalageCamera.y);

    if (map!=NULL)
        if (map->m_pret)
        {
            map->Afficher(0,255);
            map->Animer(eventManager->m_clock.GetElapsedTime());


            map->m_selectSprite.Move(decalX->value(), decalY->value());
            map->m_selectSprite.Scale(fabs(scaleX->value()*0.01),fabs(scaleY->value()*0.01));
            if(scaleX->value()<0)
            {
                map->m_selectSprite.FlipX(true);
                map->m_selectSprite.SetOrigin(map->m_selectSprite.GetSubRect().Width - map->m_selectSprite.GetOrigin().x,
                                              map->m_selectSprite.GetOrigin().y);
            }
            else
                map->m_selectSprite.FlipX(false);
            if(scaleY->value()<0)
                map->m_selectSprite.FlipY(true);
            else
                map->m_selectSprite.FlipY(false);

            map->m_selectSprite.Rotate(rotate->value());
            map->m_selectSprite.SetColor(sf::Color( map->m_selectSprite.GetColor().r * colorR->value() / 255,
                                                    map->m_selectSprite.GetColor().g * colorG->value() / 255,
                                                    map->m_selectSprite.GetColor().b * colorB->value() / 255,
                                                    map->m_selectSprite.GetColor().a * colorA->value() / 255));

            if(map->m_mode_brush)
            {
                map->m_select_brush.m_entite_graphique.m_sprite.Move(decalX->value(), decalY->value());
                map->m_select_brush.m_entite_graphique.m_sprite.SetScale(fabs(scaleX->value()*0.01),fabs(scaleY->value()*0.01));

                if(scaleX->value()<0)
                    map->m_select_brush.m_entite_graphique.m_sprite.FlipX(true);
                else
                    map->m_select_brush.m_entite_graphique.m_sprite.FlipX(false);
                if(scaleY->value()<0)
                    map->m_select_brush.m_entite_graphique.m_sprite.FlipY(true);
                else
                    map->m_select_brush.m_entite_graphique.m_sprite.FlipY(false);


                map->m_select_brush.m_entite_graphique.m_sprite.SetRotation(rotate->value());
                map->m_select_brush.m_entite_graphique.m_sprite.SetColor(sf::Color( colorR->value(),
                                                                                    colorG->value(),
                                                                                    colorB->value(),
                                                                                    colorA->value()));
                for (unsigned o = 0 ; o < map->m_select_brush.getMonstre().size() ; ++o)
                    if (map->m_select_brush.getMonstre()[o]>=0
                      &&map->m_select_brush.getMonstre()[o]<(int)map->m_monstre.size())
                          setEntiteGraphiqueParametres(map->m_monstre[map->m_select_brush.getMonstre()[o]].m_entite_graphique);

            }
        }

    eventManager->AfficherCurseur(taillePinceau->value());

    moteurGraphique->Afficher();

    //SFMLView->setFocus(Qt::MouseFocusReason);

    if (map != NULL)
    {
        if (map->m_pret)
            eventManager->GererLesEvenements(map->getDimensions());
        else
            eventManager->GererLesEvenements(coordonnee (1,1));
    }
    else
        eventManager->GererLesEvenements(coordonnee (1,1));

    std::ostringstream buf;
    buf<<"Position curseur : "<< eventManager->getCasePointee().x <<" ; "<< eventManager->getCasePointee().y;
    position->setText(buf.str().c_str());


    if (/*eventManager->getPositionSouris().x > moteurGraphique->m_ecran->GetWidth()-32 && eventManager->getPositionSouris().x < moteurGraphique->m_ecran->GetWidth() - 8 ||*/ eventManager->getEvenement(sf::Key::Right,"ET"))
    {
        eventManager->m_coordonnee.x+=eventManager->m_clock.GetElapsedTime()*5*((configuration->zoom - 1)/2+1);
        eventManager->m_coordonnee.y-=eventManager->m_clock.GetElapsedTime()*5*((configuration->zoom - 1)/2+1);
    }
    if (/*ventManager->getPositionSouris().x < 32 && eventManager->getPositionSouris().x > 8||*/ eventManager->getEvenement(sf::Key::Left,"ET"))
    {
        eventManager->m_coordonnee.x-=eventManager->m_clock.GetElapsedTime()*5*((configuration->zoom - 1)/2+1);
        eventManager->m_coordonnee.y+=eventManager->m_clock.GetElapsedTime()*5*((configuration->zoom - 1)/2+1);
    }

    if (/*eventManager->getPositionSouris().y > moteurGraphique->m_ecran->GetHeight()-32 && eventManager->getPositionSouris().y < moteurGraphique->m_ecran->GetHeight() - 8 ||*/ eventManager->getEvenement(sf::Key::Down,"ET"))
    {
        if(eventManager->getEvenement(sf::Key::Right,"ET") || eventManager->getEvenement(sf::Key::Left,"ET"))
        {
            eventManager->m_coordonnee.x+=eventManager->m_clock.GetElapsedTime()*5*((configuration->zoom - 1)/2+1);
            eventManager->m_coordonnee.y+=eventManager->m_clock.GetElapsedTime()*5*((configuration->zoom - 1)/2+1);
        }
        else
        {
            eventManager->m_coordonnee.x+=eventManager->m_clock.GetElapsedTime()*10*((configuration->zoom - 1)/2+1);
            eventManager->m_coordonnee.y+=eventManager->m_clock.GetElapsedTime()*10*((configuration->zoom - 1)/2+1);
        }
    }
    if (/*eventManager->getPositionSouris().y < 32 && eventManager->getPositionSouris().y > 8 ||*/ eventManager->getEvenement(sf::Key::Up,"ET"))
    {
        if(eventManager->getEvenement(sf::Key::Right,"ET") || eventManager->getEvenement(sf::Key::Left,"ET"))
        {
            eventManager->m_coordonnee.x-=eventManager->m_clock.GetElapsedTime()*5*((configuration->zoom - 1)/2+1);
            eventManager->m_coordonnee.y-=eventManager->m_clock.GetElapsedTime()*5*((configuration->zoom - 1)/2+1);
        }
        else
        {
            eventManager->m_coordonnee.x-=eventManager->m_clock.GetElapsedTime()*10*((configuration->zoom - 1)/2+1);
            eventManager->m_coordonnee.y-=eventManager->m_clock.GetElapsedTime()*10*((configuration->zoom - 1)/2+1);
        }
    }

    if(eventManager->getEvenement(sf::Key::Back,"ET"))
    {
        decalX->setValue(0);
        rand_decalX->setValue(0);
        decalY->setValue(0);
        rand_decalY->setValue(0);
        scaleX->setValue(100);
        rand_scaleX->setValue(0);
        scaleY->setValue(100);
        rand_scaleY->setValue(0);
        rotate->setValue(0);
        rand_rotate->setValue(0);

        colorR->setValue(255);
        rand_colorR->setValue(0);
        colorG->setValue(255);
        rand_colorG->setValue(0);
        colorB->setValue(255);
        rand_colorB->setValue(0);
        colorA->setValue(255);
        rand_colorA->setValue(0);
    }

    /*if (eventManager->getEvenement(sf::Mouse::Middle, "C"))
    {
        if (m_dejaWheel)
        {
            eventManager->m_coordonnee.x+=(m_oldmouse.x - eventManager->getPositionSouris().x)*0.0005;
            eventManager->m_coordonnee.y-=(m_oldmouse.x - eventManager->getPositionSouris().x)*0.0005;

            eventManager->m_coordonnee.x+=(m_oldmouse.y - eventManager->getPositionSouris().y)*0.0005;
            eventManager->m_coordonnee.y+=(m_oldmouse.y - eventManager->getPositionSouris().y)*0.0005;
        }
        else
            m_oldmouse = eventManager->getPositionSouris();

        m_dejaWheel = true;
    }
    else
        m_dejaWheel = false;*/

    if (eventManager->m_coordonnee.x < 0)
        eventManager->m_coordonnee.x = 0;
    if (eventManager->m_coordonnee.y < 0)
        eventManager->m_coordonnee.y = 0;

    if (map != NULL)
    {
        if (eventManager->m_coordonnee.x > map->getDimensions().x)
            eventManager->m_coordonnee.x = map->getDimensions().x;
        if (eventManager->m_coordonnee.y > map->getDimensions().y)
            eventManager->m_coordonnee.y = map->getDimensions().y;
    }

    if (map != NULL)
    {
        if (moduleAleatoireMin->value() > moduleAleatoireMax->value())
            moduleAleatoireMin->setValue(moduleAleatoireMax->value());

        map->m_taillePinceau = taillePinceau->value();

        map->m_moduleAleatoireMin = moduleAleatoireMin->value();
        map->m_moduleAleatoireMax = moduleAleatoireMax->value();
        map->m_moduleAleatoireCur = moduleAleatoireCur->value();

        if(couche0->isChecked() && map->m_selectCouche == 1 ||
           couche1->isChecked() && map->m_selectCouche == 0)
            modifLayer->setValue(0);

        if (couche0->isChecked())
            map->m_selectCouche = 0;
        else if (couche1->isChecked())
            map->m_selectCouche = 1;

        if(eventManager->getMolette() != 0)
        {
            if (couche1->isChecked())
            {
                map->m_selectCouche = 0;
                couche0->setChecked(true);
                couche1->setChecked(false);
            }
            else
            {
                map->m_selectCouche = 1;
                couche1->setChecked(true);
                couche0->setChecked(false);
            }
            //eventManager->StopEvenement(sf::Key::C, "ET");
        }


        if (afficherCollisions->isChecked())
            map->m_afficher_collision = true;
        else
            map->m_afficher_collision = false;

        if (afficherRelief->isChecked())
            map->m_afficher_relief = true;
        else
            map->m_afficher_relief = false;

        if (afficherMurLumiere->isChecked())
            map->m_afficher_murLumiere = true;
        else
            map->m_afficher_murLumiere = false;


        if (afficherCouche0->isChecked())
            map->m_afficher_couche0 = true;
        else
            map->m_afficher_couche0 = false;

        if (afficherCouche1->isChecked())
            map->m_afficher_couche1 = true;
        else
            map->m_afficher_couche1 = false;

        if(eventManager->getEvenement(sf::Key::LShift,"ET"))
        {
            if(!m_translate)
                m_old_param_mouse = eventManager->getPositionSouris();

            decalX->setValue(eventManager->getPositionSouris().x - m_old_param_mouse.x);
            decalY->setValue(eventManager->getPositionSouris().y - m_old_param_mouse.y);
            m_translate = true;
        }
        else
            m_translate = false;

        if(eventManager->getEvenement(sf::Key::S,"ET") && !eventManager->getEvenement(sf::Key::LControl,"ET"))
        {
            if(!m_rescale)
                m_old_param_mouse = eventManager->getPositionSouris();

            scaleX->setValue((eventManager->getPositionSouris().x - m_old_param_mouse.x));
            scaleY->setValue(-(eventManager->getPositionSouris().y - m_old_param_mouse.y));
            m_rescale = true;
        }
        else
            m_rescale = false;


        if(eventManager->getEvenement(sf::Key::R,"ET"))
        {
            if(!m_rotate)
                m_old_param_mouse = eventManager->getPositionSouris();

            rotate->setValue(eventManager->getPositionSouris().x - m_old_param_mouse.x);
            m_rotate = true;
        }
        else
            m_rotate = false;

        if(lie_scale->isChecked())
            scaleY->setValue(scaleX->value());
        if(lie_colorR->isChecked())
        {
            if(lie_colorG->isChecked())
                colorG->setValue(colorR->value());
            if(lie_colorB->isChecked())
                colorB->setValue(colorR->value());
        }
        else if(lie_colorG->isChecked())
        {
            if(lie_colorB->isChecked())
                colorB->setValue(colorG->value());
        }

        if(SFMLView->hasFocus() && !old_focus)
            eventManager->StopEvenement(sf::Mouse::Left, "C"), old_focus = true;
        else if(!SFMLView->hasFocus())
            old_focus = false;

        if(eventManager->getEvenement(sf::Key::O,"ET"))
        {
            map->m_entite_angle++;
            if(map->m_entite_angle >= 8)
                map->m_entite_angle = 0;

            eventManager->StopEvenement(sf::Key::O,"ET");
        }

        if (eventManager->getPositionSouris().y > 0 && eventManager->getPositionSouris().y < (int)moteurGraphique->m_ecran->GetHeight()
                && eventManager->getPositionSouris().x > 0 && eventManager->getPositionSouris().x < (int)moteurGraphique->m_ecran->GetWidth ()
                && eventManager->getEvenement(sf::Mouse::Left, "C"))
        {
            if (map->m_selectEntite >= 0)
            for(int i = eventManager->getCasePointee().y - (int)(taillePinceau->value()*0.5) ; i < eventManager->getCasePointee().y + taillePinceau->value()*0.5 ; ++i)
            for(int j = eventManager->getCasePointee().x - (int)(taillePinceau->value()*0.5) ; j < eventManager->getCasePointee().x + taillePinceau->value()*0.5 ; ++j)
            if(i >= 0 && j >= 0 && i < map->getDimensions().y && j < map->getDimensions().x)
            {
                if (map->m_selectEntite >= 1)
                {
                    map->m_monstre.push_back(Monstre ());
                    map->m_monstre.back().Charger(map->m_selectEntite - 1,&map->m_ModeleMonstre[map->m_selectEntite - 1]);
                    map->m_monstre.back().setCoordonnee(coordonnee(j,i)),map->m_monstre.back().setDepart();
                    map->m_monstre.back().setAngle(map->m_entite_angle * 45);
                    setEntiteGraphiqueParametres(map->m_monstre.back().m_entite_graphique);
                    if(eventManager->getEvenement(sf::Key::Add, "ET"))
                        map->m_add_monstre.push_back(true), eventManager->StopEvenement(sf::Mouse::Left, "C");
                    else
                        map->m_add_monstre.push_back(false), eventManager->StopEvenement(sf::Mouse::Left, "C");
                }

                if (map->m_moduleAleatoireMin == 0 && map->m_moduleAleatoireMax == 9)
                {
                    if(!eventManager->getEvenement(sf::Key::Add, "ET"))
                        map->m_decor[1][i][j].back().m_monstre.clear();
                    if (map->m_selectEntite >= 1)
                        map->m_decor[1][i][j].back().m_monstre.push_back(map->m_monstre.size()-1);
                }
                else
                {
                    bool ajouter = true;
                    for (unsigned k = 0 ; k < map->m_decor[1][i][j].size() ; ++k)
                        if (map->m_decor[1][i][j][k].m_moduleAleatoireMin == map->m_moduleAleatoireMin
                                &&map->m_decor[1][i][j][k].m_moduleAleatoireMax == map->m_moduleAleatoireMax)
                        {
                            ajouter = false;
                            if(!eventManager->getEvenement(sf::Key::Add, "ET"))
                                map->m_decor[1][i][j].back().m_monstre.clear();
                            if (map->m_selectEntite >= 1)
                                map->m_decor[1][i][j][k].m_monstre.push_back(map->m_monstre.size()-1);
                        }
                    if (ajouter)
                    {
                        map->m_decor[1][i][j].push_back(map->m_decor[1][i][j].back());
                        map->m_decor[1][i][j][map->m_decor[1][i][j].size()-2] = Decor ();
                        map->m_decor[1][i][j][map->m_decor[1][i][j].size()-2].m_monstre.clear();
                        if (map->m_selectEntite >= 1)
                            map->m_decor[1][i][j][map->m_decor[1][i][j].size()-2].m_monstre.push_back(map->m_monstre.size()-1);
                        map->m_decor[1][i][j][map->m_decor[1][i][j].size()-2].m_moduleAleatoireMin = map->m_moduleAleatoireMin;
                        map->m_decor[1][i][j][map->m_decor[1][i][j].size()-2].m_moduleAleatoireMax = map->m_moduleAleatoireMax;
                    }
                }
            }

            if (map->m_selectTileset >= 0)
            for(int i = eventManager->getCasePointee().y - (int)(taillePinceau->value()*0.5) ; i < eventManager->getCasePointee().y + taillePinceau->value()*0.5 ; ++i)
            for(int j = eventManager->getCasePointee().x - (int)(taillePinceau->value()*0.5) ; j < eventManager->getCasePointee().x + taillePinceau->value()*0.5 ; ++j)
            if(i >= 0 && j >= 0 && i < map->getDimensions().y && j < map->getDimensions().x)
            {
                int layer = modifLayer->value();
                if (map->m_selectCouche == 0)
                    layer += 1;
                else if (map->m_selectCouche == 1)
                    layer += 10;
                if (layer < 1)
                    layer = 1;
                if (layer == 9)
                    layer = 8;

                if (map->m_moduleAleatoireMin == 0 && map->m_moduleAleatoireMax == 9)
                {
                    map->m_decor[map->m_selectCouche][i][j].back().m_tile.clear();
                    map->m_decor[map->m_selectCouche][i][j].back().m_tileset = map->m_selectTileset-1;
                    map->m_decor[map->m_selectCouche][i][j].back().m_couche = layer;
                    if (map->m_selectTileset >= 1)
                    {
                        map->m_decor[map->m_selectCouche][i][j].back().m_tile.push_back(map->m_selectTile);
                        setEntiteGraphiqueParametres(map->m_decor[map->m_selectCouche][i][j].back().m_entite_graphique);
                    }
                }
                else
                {
                    bool ajouter = true;
                    for (unsigned k = 0 ; k < map->m_decor[map->m_selectCouche][i][j].size() ; ++k)
                        if (map->m_decor[map->m_selectCouche][i][j][k].m_moduleAleatoireMin == map->m_moduleAleatoireMin
                                &&map->m_decor[map->m_selectCouche][i][j][k].m_moduleAleatoireMax == map->m_moduleAleatoireMax)
                        {
                            ajouter = false;
                            map->m_decor[map->m_selectCouche][i][j][k].m_tile.clear();
                            map->m_decor[map->m_selectCouche][i][j][k].m_tileset = map->m_selectTileset-1;
                            map->m_decor[map->m_selectCouche][i][j][k].m_couche = layer;
                            if (map->m_selectTileset >= 1)
                            {
                                map->m_decor[map->m_selectCouche][i][j][k].m_tile.push_back(map->m_selectTile);
                                setEntiteGraphiqueParametres(map->m_decor[map->m_selectCouche][i][j][k].m_entite_graphique);
                            }
                        }
                    if (ajouter)
                    {
                        map->m_decor[map->m_selectCouche][i][j].push_back(map->m_decor[map->m_selectCouche][i][j].back());
                        map->m_decor[map->m_selectCouche][i][j][map->m_decor[map->m_selectCouche][i][j].size()-2] = Decor ();

                        map->m_decor[map->m_selectCouche][i][j][map->m_decor[map->m_selectCouche][i][j].size()-2].m_tile.clear();
                        map->m_decor[map->m_selectCouche][i][j][map->m_decor[map->m_selectCouche][i][j].size()-2].m_tileset = map->m_selectTileset-1;
                        map->m_decor[map->m_selectCouche][i][j][map->m_decor[map->m_selectCouche][i][j].size()-2].m_couche = layer;
                        if (map->m_selectTileset >= 1)
                        {
                            map->m_decor[map->m_selectCouche][i][j][map->m_decor[map->m_selectCouche][i][j].size()-2].m_tile.push_back(map->m_selectTile);
                            setEntiteGraphiqueParametres(map->m_decor[map->m_selectCouche][i][j][map->m_decor[map->m_selectCouche][i][j].size()-2].m_entite_graphique);
                        }


                        map->m_decor[map->m_selectCouche][i][j][map->m_decor[map->m_selectCouche][i][j].size()-2].m_moduleAleatoireMin = map->m_moduleAleatoireMin;
                        map->m_decor[map->m_selectCouche][i][j][map->m_decor[map->m_selectCouche][i][j].size()-2].m_moduleAleatoireMax = map->m_moduleAleatoireMax;
                    }
                }
            }

            if (map->m_selectHerbe >= 0)
            for(int i = eventManager->getCasePointee().y - (int)(taillePinceau->value()*0.5) ; i < eventManager->getCasePointee().y + taillePinceau->value()*0.5 ; ++i)
            for(int j = eventManager->getCasePointee().x - (int)(taillePinceau->value()*0.5) ; j < eventManager->getCasePointee().x + taillePinceau->value()*0.5 ; ++j)
            if(i >= 0 && j >= 0 && i < map->getDimensions().y && j < map->getDimensions().x)
            {
                if (map->m_moduleAleatoireMin == 0 && map->m_moduleAleatoireMax == 9)
                    map->m_decor[map->m_selectCouche][i][j].back().m_herbe = map->m_selectHerbe - 1;
                else
                {
                    bool ajouter = true;
                    for (unsigned k = 0 ; k < map->m_decor[map->m_selectCouche][i][j].size() ; ++k)
                        if (map->m_decor[map->m_selectCouche][i][j][k].m_moduleAleatoireMin == map->m_moduleAleatoireMin
                                &&map->m_decor[map->m_selectCouche][i][j][k].m_moduleAleatoireMax == map->m_moduleAleatoireMax)
                        {
                            ajouter = false;
                            map->m_decor[map->m_selectCouche][i][j][k].m_herbe = map->m_selectHerbe - 1;
                        }
                    if (ajouter)
                    {
                        map->m_decor[map->m_selectCouche][i][j].push_back(map->m_decor[map->m_selectCouche][i][j].back());
                        map->m_decor[map->m_selectCouche][i][j][map->m_decor[map->m_selectCouche][i][j].size()-2] = Decor ();

                        map->m_decor[map->m_selectCouche][i][j][map->m_decor[map->m_selectCouche][i][j].size()-2].m_herbe = map->m_selectHerbe - 1;

                        map->m_decor[map->m_selectCouche][i][j][map->m_decor[map->m_selectCouche][i][j].size()-2].m_moduleAleatoireMin = map->m_moduleAleatoireMin;
                        map->m_decor[map->m_selectCouche][i][j][map->m_decor[map->m_selectCouche][i][j].size()-2].m_moduleAleatoireMax = map->m_moduleAleatoireMax;
                    }
                }

                for(int z = 0 ; z < (int)map->m_decor[map->m_selectCouche][i][j].size() ; ++z)
                    if (map->m_decor[map->m_selectCouche][i][j][z].getHerbe()>=0
                     && map->m_decor[map->m_selectCouche][i][j][z].getHerbe()<(int)map->m_herbe.size())
                    {
                        if (moteurGraphique->getTileset(map->m_herbe[map->m_decor[map->m_selectCouche][i][j][z].getHerbe()])->getTaille()>0)
                        {

                            coordonnee position,positionPartieDecor;

                            int numeroHerbe=0;
                            if (moteurGraphique->getTileset(map->m_herbe[map->m_decor[map->m_selectCouche][i][j][z].getHerbe()])->getTaille()>0)
                                numeroHerbe = (rand() % (moteurGraphique->getTileset(map->m_herbe[map->m_decor[map->m_selectCouche][i][j][z].getHerbe()])->getTaille()));
                            map->m_decor[map->m_selectCouche][i][j][z].setNumeroHerbe(numeroHerbe);

                            position.x=(j-i)*64;
                            position.y=(j+i+1)*32;

                            if (map->m_selectCouche==0)
                                position.y-=32;
                            position.x+=map->m_decor[0][i][j][z].getDecalageHerbe().x;

                            map->m_decor[map->m_selectCouche][i][j][z].m_entite_herbe = moteurGraphique->getEntiteGraphique(map->m_herbe[map->m_decor[map->m_selectCouche][i][j][z].getHerbe()], numeroHerbe, 10);
                            map->m_decor[map->m_selectCouche][i][j][z].m_entite_herbe.m_sprite.SetPosition(position.x, position.y - map->m_decor[0][i][j][z].getHauteur());
                            map->m_decor[map->m_selectCouche][i][j][z].m_entite_herbe.m_sprite.SetScale((float)map->m_decor[map->m_selectCouche][i][j][z].getTailleHerbe()/100,(float)map->m_decor[map->m_selectCouche][i][j][z].getTailleHerbe()/100);
                            map->m_decor[map->m_selectCouche][i][j][z].m_entite_herbe.m_sprite.SetColor(map->m_decor[map->m_selectCouche][i][j][z].getCouleurHerbe());

                            map->m_decor[map->m_selectCouche][i][j][z].m_entite_herbe.Initialiser(coordonnee ());


                           /* coordonnee position,positionPartieDecor;

                            int numeroHerbe=0;
                            if (moteurGraphique->getTileset(map->m_herbe[map->m_decor[map->m_selectCouche][i][j][z].getHerbe()])->getTaille()>0)
                                numeroHerbe = (rand() % (moteurGraphique->getTileset(map->m_herbe[map->m_decor[map->m_selectCouche][i][j][z].getHerbe()])->getTaille()));
                            map->m_decor[map->m_selectCouche][i][j][z].setNumeroHerbe(numeroHerbe);

                            position.x=(j-i-1)*64;
                            position.y=(j+i)*32;

                            positionPartieDecor=moteurGraphique->getTileset(map->m_herbe[map->m_decor[map->m_selectCouche][i][j][z].getHerbe()])->getPositionDuTile(map->m_decor[map->m_selectCouche][i][j][z].getNumeroHerbe());

                            if (map->m_selectCouche==0)
                                position.y-=32;
                            position.x+=map->m_decor[map->m_selectCouche][i][j][z].getDecalageHerbe().x;

                            map->m_decor[map->m_selectCouche][i][j][z].m_entite_herbe.m_sprite.SetImage(*moteurGraphique->getImage(moteurGraphique->getTileset(map->m_herbe[map->m_decor[map->m_selectCouche][i][j][z].getHerbe()])->getImage(map->m_decor[map->m_selectCouche][i][j][z].getNumeroHerbe())));
                            map->m_decor[map->m_selectCouche][i][j][z].m_entite_herbe.m_sprite.SetSubRect(sf::IntRect(positionPartieDecor.x, positionPartieDecor.y, positionPartieDecor.x+positionPartieDecor.w, positionPartieDecor.y+positionPartieDecor.h));
                            map->m_decor[map->m_selectCouche][i][j][z].m_entite_herbe.m_sprite.SetScale((float)map->m_decor[map->m_selectCouche][i][j][z].getTailleHerbe()/100,(float)map->m_decor[map->m_selectCouche][i][j][z].getTailleHerbe()/100);
                            map->m_decor[map->m_selectCouche][i][j][z].m_entite_herbe.m_sprite.SetX(position.x+64-positionPartieDecor.w/2);
                            map->m_decor[map->m_selectCouche][i][j][z].m_entite_herbe.m_sprite.SetY(position.y-positionPartieDecor.h+64);
                            map->m_decor[map->m_selectCouche][i][j][z].m_entite_herbe.m_sprite.SetColor(map->m_decor[map->m_selectCouche][i][j][z].getCouleurHerbe());*/
                        }
                    }
                    else
                        map->m_decor[map->m_selectCouche][i][j][z].m_entite_herbe.m_sprite.SetSubRect(sf::IntRect(0,0,0,0));
            }

            if (map->m_mode_brush)
            for(int i = eventManager->getCasePointee().y - (int)(taillePinceau->value()*0.5) ; i < eventManager->getCasePointee().y + taillePinceau->value()*0.5 ; ++i)
            for(int j = eventManager->getCasePointee().x - (int)(taillePinceau->value()*0.5) ; j < eventManager->getCasePointee().x + taillePinceau->value()*0.5 ; ++j)
            if(i >= 0 && j >= 0 && i < map->getDimensions().y && j < map->getDimensions().x)
            {
                int layer = modifLayer->value();
                if (map->m_selectCouche == 0)
                    layer += 1;
                else if (map->m_selectCouche == 1)
                    layer += 10;
                if (layer < 1)
                    layer = 1;
                if (layer == 9)
                    layer = 8;

                map->m_select_brush.m_couche = layer;

                if (map->m_moduleAleatoireMin == 0 && map->m_moduleAleatoireMax == 9)
                {
                    // Essayer de copier à la main en vérifiant !tile.empty(), etc
                    if(map->m_select_brush.m_tileset != -1)
                        map->m_decor[map->m_selectCouche][i][j].back().m_tileset = map->m_select_brush.m_tileset;

                    if(!map->m_select_brush.m_tile.empty())
                    {
                        map->m_decor[map->m_selectCouche][i][j].back().m_tile = map->m_select_brush.m_tile;
                        setEntiteGraphiqueParametres(map->m_decor[map->m_selectCouche][i][j].back().m_entite_graphique);
                    }

                    if(!map->m_select_brush.m_objets.empty())
                        map->m_decor[map->m_selectCouche][i][j].back().m_objets = map->m_select_brush.m_objets;

                    if(map->m_select_brush.m_couche != -1)
                        map->m_decor[map->m_selectCouche][i][j].back().m_couche = map->m_select_brush.m_couche;

                    if(!map->m_select_brush.m_monstre.empty())
                    {
                        map->m_decor[1][i][j].back().m_monstre = map->m_select_brush.m_monstre;
                        for(int z = 0 ; z < map->m_decor[1][i][j].back().m_monstre.size() ; ++z)
                        {
                            map->m_monstre[map->m_decor[1][i][j].back().m_monstre[z]].setCoordonnee(coordonnee (j, i));
                            setEntiteGraphiqueParametres(map->m_monstre[map->m_decor[1][i][j].back().m_monstre[z]].m_entite_graphique);
                            map->m_monstre[map->m_decor[1][i][j].back().m_monstre[z]].Animer(&map->m_ModeleMonstre[map->m_monstre[map->m_decor[1][i][j].back().m_monstre[z]].getModele()],1);

                            map->m_monstre.push_back(Monstre ());
                            if(eventManager->getEvenement(sf::Key::Add, "ET") && z == 0)
                                map->m_add_monstre.push_back(true), eventManager->StopEvenement(sf::Mouse::Left, "C");
                            else
                                map->m_add_monstre.push_back(false);

                            map->m_monstre.back().Charger(map->m_monstre[map->m_decor[1][i][j].back().m_monstre[z]].m_modele,
                                                         &map->m_ModeleMonstre[map->m_monstre[map->m_decor[1][i][j].back().m_monstre[z]].m_modele]);
                            map->m_monstre.back().setCoordonnee(coordonnee(j,i)),map->m_monstre.back().setDepart();
                            map->m_select_brush.m_monstre[z] = map->m_monstre.size()-1;
                        }

                    }


                    if(map->m_select_brush.m_herbe != -1)
                        map->m_decor[map->m_selectCouche][i][j].back().m_herbe = map->m_select_brush.m_herbe;
                }
                else
                {
                    bool ajouter = true;
                    for (unsigned k = 0 ; k < map->m_decor[map->m_selectCouche][i][j].size() ; ++k)
                        if (map->m_decor[map->m_selectCouche][i][j][k].m_moduleAleatoireMin == map->m_moduleAleatoireMin
                                &&map->m_decor[map->m_selectCouche][i][j][k].m_moduleAleatoireMax == map->m_moduleAleatoireMax)
                        {
                            ajouter = false;
                            map->m_decor[map->m_selectCouche][i][j][k] = map->m_select_brush;
                        }
                    if (ajouter)
                    {
                        map->m_decor[map->m_selectCouche][i][j].push_back(map->m_decor[map->m_selectCouche][i][j].back());
                        map->m_decor[map->m_selectCouche][i][j][map->m_decor[map->m_selectCouche][i][j].size()-2] = Decor ();

                        //map->m_decor[map->m_selectCouche][i][j][map->m_decor[map->m_selectCouche][i][j].size()-2] = map->m_select_brush;

                        if(map->m_select_brush.m_tileset != -1)
                            map->m_decor[map->m_selectCouche][i][j][map->m_decor[map->m_selectCouche][i][j].size()-2].m_tileset = map->m_select_brush.m_tileset;

                        if(!map->m_select_brush.m_tile.empty())
                            map->m_decor[map->m_selectCouche][i][j][map->m_decor[map->m_selectCouche][i][j].size()-2].m_tile = map->m_select_brush.m_tile;

                        if(!map->m_select_brush.m_objets.empty())
                            map->m_decor[map->m_selectCouche][i][j][map->m_decor[map->m_selectCouche][i][j].size()-2].m_objets = map->m_select_brush.m_objets;

                        if(map->m_select_brush.m_couche != -1)
                            map->m_decor[map->m_selectCouche][i][j][map->m_decor[map->m_selectCouche][i][j].size()-2].m_couche = map->m_select_brush.m_couche;

                        if(!map->m_select_brush.m_monstre.empty())
                        {
                            map->m_decor[map->m_selectCouche][i][j][map->m_decor[map->m_selectCouche][i][j].size()-2].m_monstre = map->m_select_brush.m_monstre;
                            for(int z = 0 ; z < map->m_decor[map->m_selectCouche][i][j][map->m_decor[map->m_selectCouche][i][j].size()-2].m_monstre.size() ; ++z)
                                map->m_monstre[map->m_decor[map->m_selectCouche][i][j][map->m_decor[map->m_selectCouche][i][j].size()-2].m_monstre[z]].setCoordonnee(coordonnee (j, i));
                        }

                        if(map->m_select_brush.m_herbe != -1)
                            map->m_decor[map->m_selectCouche][i][j][map->m_decor[map->m_selectCouche][i][j].size()-2].m_herbe = map->m_select_brush.m_herbe;

                        map->m_decor[map->m_selectCouche][i][j][map->m_decor[map->m_selectCouche][i][j].size()-2].m_moduleAleatoireMin = map->m_moduleAleatoireMin;
                        map->m_decor[map->m_selectCouche][i][j][map->m_decor[map->m_selectCouche][i][j].size()-2].m_moduleAleatoireMax = map->m_moduleAleatoireMax;
                    }
                }
            }
        }

        if (eventManager->getEvenement(sf::Mouse::Right, "C"))
        {
            eventManager->StopEvenement(sf::Mouse::Right, "C");
            if (!map->m_select_brush.m_tile.empty()
             || !map->m_select_brush.m_monstre.empty()
             || map->m_select_brush.m_herbe != -1
             || map->m_selectEntite != -1
             || map->m_selectTileset != -1
             || map->m_selectTile != -1
             || map->m_selectHerbe != -1
              )
            {
                map->m_select_brush.m_tile.clear();
                map->m_select_brush.m_monstre.clear();
                map->m_select_brush.m_herbe = -1;
                map->m_mode_brush = false;

                map->m_selectEntite     = -1;
                map->m_selectTileset    = -1;
                map->m_selectTile       = -1;
                map->m_selectHerbe      = -1;
            }
            else
            {
                int no = QInputDialog::getInteger(this, "Nombre", "ID du/des entités(s)", map->m_max_ID);

                if(no > map->m_max_ID)
                    map->m_max_ID = no;

                for(int i = eventManager->getCasePointee().y - (int)(taillePinceau->value()*0.5) ; i < eventManager->getCasePointee().y + taillePinceau->value()*0.5 ; ++i)
                for(int j = eventManager->getCasePointee().x - (int)(taillePinceau->value()*0.5) ; j < eventManager->getCasePointee().x + taillePinceau->value()*0.5 ; ++j)
                if(i >= 0 && j >= 0 && i < map->getDimensions().y && j < map->getDimensions().x)
                {
                    for (unsigned k = 0 ; k < map->m_decor[1][i][j].size() ; ++k)
                        if (map->m_decor[1][i][j][k].m_moduleAleatoireMin == map->m_moduleAleatoireMin
                          &&map->m_decor[1][i][j][k].m_moduleAleatoireMax == map->m_moduleAleatoireMax)
                        {
                            for(int p = 0 ; p < map->m_decor[1][i][j][k].m_monstre.size() ; ++p)
                                map->m_monstre[map->m_decor[1][i][j][k].m_monstre[p]].m_ID = no;
                        }
                }
            }
        }

        if(eventManager->getEvenement(sf::Mouse::Middle, "C")
        || eventManager->getEvenement(sf::Key::A, "ET"))
        {
            eventManager->StopEvenement(sf::Mouse::Middle, "C");
            eventManager->StopEvenement(sf::Key::A, "ET");
            {
                int no = QInputDialog::getInteger(this, "Nombre", "Hauteur de la case");

                for(int i = eventManager->getCasePointee().y - (int)(taillePinceau->value()*0.5) ; i < eventManager->getCasePointee().y + taillePinceau->value()*0.5 ; ++i)
                for(int j = eventManager->getCasePointee().x - (int)(taillePinceau->value()*0.5) ; j < eventManager->getCasePointee().x + taillePinceau->value()*0.5 ; ++j)
                if(i >= 0 && j >= 0 && i < map->getDimensions().y && j < map->getDimensions().x)
                {
                    for (unsigned k = 0 ; k < map->m_decor[0][i][j].size() ; ++k)
                        if (map->m_decor[0][i][j][k].m_moduleAleatoireMin == map->m_moduleAleatoireMin
                          &&map->m_decor[0][i][j][k].m_moduleAleatoireMax == map->m_moduleAleatoireMax)
                        {
                            map->m_decor[0][i][j][k].m_hauteur = no;
                        }
                }
            }
        }


        if (map->m_selectEntite == -100)
            map->m_selectSprite.SetSubRect(sf::IntRect(0,0,0,0));

        if (eventManager->getEvenement(sf::Mouse::Right, "CA") && !eventManager->getEvenement(sf::Mouse::Right, "C")
                ||eventManager->getEvenement(sf::Mouse::Left , "CA") && !eventManager->getEvenement(sf::Mouse::Left , "C"))
        {
            backup();

            if (eventManager->getEvenement(sf::Mouse::Left , "CA"))
                eventManager->StopEvenement(sf::Mouse::Left , "CA");
            if (eventManager->getEvenement(sf::Mouse::Right , "CA"))
                eventManager->StopEvenement(sf::Mouse::Right , "CA");
        }
    }


    eventManager->m_clock.Reset();
}

void MainWindow::setEntiteGraphiqueParametres(Entite_graphique &entite)
{
    entite.m_decalage.x = decalX->value();
    if(rand_decalX->value() != 0)
        entite.m_decalage.x += rand_decalX->value() - rand() % rand_decalX->value() * 2;

    entite.m_decalage.y = decalY->value();
    if(rand_decalY->value() != 0)
        entite.m_decalage.y += rand_decalY->value() - rand() % rand_decalY->value() * 2;

    entite.m_scale.x = scaleX->value();
    if(rand_scaleX->value() != 0)
        entite.m_scale.x += rand_scaleX->value() - rand() % rand_scaleX->value() * 2;

    entite.m_scale.y = scaleY->value();
    if(rand_scaleY->value() != 0)
        entite.m_scale.y += rand_scaleY->value() - rand() % rand_scaleY->value() * 2;

    entite.m_rotation = rotate->value();
    if(rand_rotate->value() != 0)
        entite.m_rotation += rand_rotate->value() - rand() % rand_rotate->value() * 2;

    entite.m_color.r = colorR->value();
    if(rand_colorR->value() != 0)
        entite.m_color.r += rand_colorR->value() - rand() % rand_colorR->value() * 2;
    entite.m_color.g = colorG->value();
    if(rand_colorG->value() != 0)
        entite.m_color.g += rand_colorG->value() - rand() % rand_colorG->value() * 2;
    entite.m_color.b = colorB->value();
    if(rand_colorB->value() != 0)
        entite.m_color.b += rand_colorB->value() - rand() % rand_colorB->value() * 2;
    entite.m_color.a = colorA->value();
    if(rand_colorA->value() != 0)
        entite.m_color.a += rand_colorA->value() - rand() % rand_colorA->value() * 2;

    if(lie_scale->isChecked())
        entite.m_scale.y = entite.m_scale.x;
    if(lie_colorR->isChecked())
    {
        if(lie_colorG->isChecked())
            entite.m_color.g = entite.m_color.r;
        if(lie_colorB->isChecked())
            entite.m_color.b = entite.m_color.r;
    }
    else if(lie_colorG->isChecked())
    {
        if(lie_colorB->isChecked())
            entite.m_color.b = entite.m_color.g;
    }
}

void MainWindow::backup()
{
    if(map != NULL)
    {
        std::string temp = map->m_nom_fichier;
            std::string temp2 = "Data/Editeur/Temp";

            for (unsigned k = 0 ; k < temp.size() ; ++k)
            {
                if (temp[k] == 'M')
                    if (k ++ < temp.size())
                        if (temp[k] == 'a')
                            if (k ++ < temp.size())
                                if (temp[k] == 'p')
                                    if (k ++ < temp.size())
                                        if (temp[k] == 's')
                                            if (k ++ < temp.size())
                                                if (temp[k] == '/')
                                                {
                                                    for (; k < temp.size() ; ++k)
                                                    {
                                                        if (temp[k] == '.')
                                                            k = temp.size();
                                                        else
                                                            temp2.push_back(temp[k]);
                                                    }
                                                }
            }

            map->m_no_backup++;

            if (map->m_no_backup > 50)
                map->m_no_backup = 0, m_tourUndo = true;

            m_redoMax = map->m_no_backup;
            m_undoMax = map->m_no_backup;


            std::ostringstream temp3;
            temp3 << temp2 << "_back_" << map->m_no_backup << ".map.hs";
            std::cout<<temp3<<std::endl;
            map->Sauvegarder(temp3.str());
    }
}


void MainWindow::MettreListesAJour()
{
    if(map != NULL)
    {
        listTileset->clear();
        for (unsigned k = 0 ; k <= map->m_tileset.size() ; ++k)
        {
            QStringList temp;

            if (k == 0)
                temp<<"Suppression tile";
            else
                temp<<moteurGraphique->getTileset(map->m_tileset[k-1])->getChemin().c_str();

            QTreeWidgetItem *widget = new QTreeWidgetItem(temp, 0);
            widget ->setData (1, 0, QVariant(k));

            if (k > 0)
                for (unsigned l = 0 ; l < moteurGraphique->getTileset(map->m_tileset[k-1])->m_tile.size() ; ++l)
                {
                    std::ostringstream buf;
                    if(!moteurGraphique->getTileset(map->m_tileset[k-1])->m_tile[l].m_nom.empty())
                        buf<<moteurGraphique->getTileset(map->m_tileset[k-1])->m_tile[l].m_nom;
                    else
                        buf<<l;
                    QStringList temp;
                    temp<<buf.str().c_str();
                    QTreeWidgetItem *widget2 = new QTreeWidgetItem(temp, 0);
                    widget2->setText(0,temp.at(0));
                    widget2 ->setData (1, 0, QVariant(l));

                    widget->addChild(widget2);
                }

            listTileset->addTopLevelItem(widget);
        }

        listHerbe->clear();
        for (unsigned k = 0 ; k <= map->m_herbe.size() ; ++k)
        {
            QStringList temp;

            if (k == 0)
                temp<<"Suppression herbe";
            else
                temp<<moteurGraphique->getTileset(map->m_herbe[k-1])->getChemin().c_str();

            QTreeWidgetItem *widget = new QTreeWidgetItem(temp, 0);
            widget ->setData (1, 0, QVariant(k));

            listHerbe->addTopLevelItem(widget);
        }

        listEntites->clear();
        for (unsigned k = 0 ; k <= map->m_ModeleMonstre.size() ; ++k)
        {
            QStringList temp;
            if (k == 0)
                temp<<"Suppression entité";
            else
                temp<<map->m_ModeleMonstre[k-1].m_chemin.c_str();
            QTreeWidgetItem *widget = new QTreeWidgetItem(temp, 0);
            widget ->setData (1, 0, QVariant(k));

            listEntites->addTopLevelItem(widget);
        }
    }
}

void MainWindow::nouveau()
{
    fenetreNouveau->show();
}

void MainWindow::s_creerNouveau()
{

    if(map != NULL)
        delete map;
    map = new Map();
    map->Creer(tailleX->value(), tailleY->value());

    map->m_nom = nomMap->text().toStdString();
    map->m_no_nom = configuration->text_maps.size();
    configuration->text_maps.push_back(nomMap->text().toStdString());

    eventManager->m_coordonnee.x = 1;
    eventManager->m_coordonnee.y = 1;

    MettreListesAJour();

    fenetreNouveau->hide();
}
void MainWindow::s_annulerNouveau()
{
    fenetreNouveau->hide();
}

void MainWindow::ouvrir()
{
    //chemin = QFileDialog::getExistingDirectory(this);
    chemin = QFileDialog::getOpenFileName(this, "Ouvrir une map", "Data/Editeur/Maps", "Map (*.map.hs )");

    if (!chemin.toStdString().empty())
    {
        std::string temp = chemin.toStdString();
        std::string temp2 = "Data";

        for (unsigned k = 0 ; k < temp.size() ; ++k)
        {
            if (temp[k] == 'D')
                if (k ++ < temp.size())
                    if (temp[k] == 'a')
                        if (k ++ < temp.size())
                            if (temp[k] == 't')
                                if (k ++ < temp.size())
                                    if (temp[k] == 'a')
                                        if (k ++ < temp.size())
                                            if (temp[k] == '/')
                                            {
                                                for (; k < temp.size() ; ++k)
                                                    temp2.push_back(temp[k]);
                                            }
        }

        temp = temp2;

        if (map != NULL)
            delete map;

        eventManager->m_coordonnee.x = 1;
        eventManager->m_coordonnee.y = 1;

        map = new Map();
        map->Charger(temp);
        MettreListesAJour();

        map->m_select_brush.m_herbe = -1;

        std::ostringstream str;
        map->m_script.Sauvegarder(str);
        QString tempstr(str.str().c_str());
       // tempstr.replace(QString("then"), QString("<span style=\"color : blue\">then<\\span>"));
        text_script->setText(tempstr);
        map->m_script.m_text = str.str().c_str();

        std::string buf = "HolyEditor - " + map->m_nom_fichier;
        setWindowTitle(QString(buf.c_str()));
    }
}


void MainWindow::sauver()
{
    if (map != NULL)
    {
        map->m_script.m_text = text_script->toPlainText().toStdString();
        if(!map->m_nom_fichier.empty())
            map->Sauvegarder(map->m_nom_fichier);
        else
            sauverAs();

        std::string buf = "HolyEditor - " + map->m_nom_fichier;
        setWindowTitle(QString(buf.c_str()));
    }
}

void MainWindow::sauverAs()
{
    if (map != NULL)
    {
        map->m_nom_fichier = QFileDialog::getSaveFileName(this, "Enregistrer une map", "Data/Editeur/Maps", "Map (*.map.hs )").toStdString();
        map->Sauvegarder(map->m_nom_fichier);

        std::string temp2 = "Data";

        for (unsigned k = 0 ; k < map->m_nom_fichier.size() ; ++k)
        {
            if (map->m_nom_fichier[k] == 'D')
                if (k ++ < map->m_nom_fichier.size())
                    if (map->m_nom_fichier[k] == 'a')
                        if (k ++ < map->m_nom_fichier.size())
                            if (map->m_nom_fichier[k] == 't')
                                if (k ++ < map->m_nom_fichier.size())
                                    if (map->m_nom_fichier[k] == 'a')
                                        if (k ++ < map->m_nom_fichier.size())
                                            if (map->m_nom_fichier[k] == '/')
                                            {
                                                for (; k < map->m_nom_fichier.size() ; ++k)
                                                    temp2.push_back(map->m_nom_fichier[k]);
                                            }
        }
        map->m_nom_fichier = temp2;
    }

    configuration->Sauvegarder();
}

void MainWindow::undo()
{
    if (map != NULL)
    {
        bool ok = false;

        if(m_tourUndo)
        {
            if(map->m_no_backup - 1 < 0)
            {
                if(map->m_no_backup - 1 + 50 != m_undoMax)
                    ok = true;
            }
            else
            {
                if(map->m_no_backup - 1 != m_undoMax)
                    ok = true;
            }
        }
        else if(map->m_no_backup - 1 >= 0)
            ok = true;

        if(ok)
        {
            int m_selectTileset   = map->m_selectTileset;
            int m_selectTile      = map->m_selectTile;
            int m_selectHerbe     = map->m_selectHerbe;
            int m_selectEntite    = map->m_selectEntite;
            int m_selectEvenement = map->m_selectTileset;
            Decor m_select_brush  = map->m_select_brush;
            bool m_mode_brush     = map->m_mode_brush;

            std::string temp = map->m_nom_fichier;
            std::string temp2 = "Data/Editeur/Temp";

            for (unsigned k = 0 ; k < temp.size() ; ++k)
            {
                if (temp[k] == 'M')
                    if (k ++ < temp.size())
                        if (temp[k] == 'a')
                            if (k ++ < temp.size())
                                if (temp[k] == 'p')
                                    if (k ++ < temp.size())
                                        if (temp[k] == 's')
                                            if (k ++ < temp.size())
                                                if (temp[k] == '/')
                                                {
                                                    for (; k < temp.size() ; ++k)
                                                    {
                                                        if (temp[k] == '.')
                                                            k = temp.size();
                                                        else
                                                            temp2.push_back(temp[k]);
                                                    }
                                                }
            }
            map->m_no_backup--;

            if (map->m_no_backup < 0)
                map->m_no_backup = 50;

            std::ostringstream temp3;
            temp3 << temp2 << "_back_" << map->m_no_backup << ".map.hs";

            int no = map->m_no_backup;
            std::string nom = map->m_nom_fichier;


            delete map;

            map = new Map();
            if (!map->Charger(temp3.str()))
            {
                map->m_no_backup = no+= 2;
                if (map->m_no_backup > 50)
                    map->m_no_backup -= 50;
                undo();
            }
            map->m_no_backup = no;
            map->m_nom_fichier = nom;
            MettreListesAJour();

            map->m_selectTileset    = m_selectTileset;
            map->m_selectTile       = m_selectTile;
            map->m_selectHerbe      = m_selectHerbe;
            map->m_selectEntite     = m_selectEntite;
            map->m_selectTileset    = m_selectEvenement;
            map->m_select_brush     = m_select_brush;
            map->m_mode_brush   = m_mode_brush;

        }
    }
}

void MainWindow::redo()
{
    if (map != NULL)
    {
        bool ok = false;

        if(map->m_no_backup + 1 > 50)
        {
            if(map->m_no_backup + 1 - 50 <= m_redoMax)
                ok = true;
        }
        else
            if(map->m_no_backup +1 <= m_redoMax)
                ok = true;

        if(ok)
        {
            int m_selectTileset   = map->m_selectTileset;
            int m_selectTile      = map->m_selectTile;
            int m_selectHerbe     = map->m_selectHerbe;
            int m_selectEntite    = map->m_selectEntite;
            int m_selectEvenement = map->m_selectTileset;
            Decor m_select_brush  = map->m_select_brush;
            bool m_mode_brush     = map->m_mode_brush;

            std::string temp = map->m_nom_fichier;
            std::string temp2 = "Data/Editeur/Temp";

            for (unsigned k = 0 ; k < temp.size() ; ++k)
            {
                if (temp[k] == 'M')
                    if (k ++ < temp.size())
                        if (temp[k] == 'a')
                            if (k ++ < temp.size())
                                if (temp[k] == 'p')
                                    if (k ++ < temp.size())
                                        if (temp[k] == 's')
                                            if (k ++ < temp.size())
                                                if (temp[k] == '/')
                                                {
                                                    for (; k < temp.size() ; ++k)
                                                    {
                                                        if (temp[k] == '.')
                                                            k = temp.size();
                                                        else
                                                            temp2.push_back(temp[k]);
                                                    }
                                                }
            }
            map->m_no_backup++;

            if (map->m_no_backup < 0)
                map->m_no_backup = 50;

            std::ostringstream temp3;
            temp3 << temp2 << "_back_" << map->m_no_backup << ".map.hs";

            int no = map->m_no_backup;
            std::string nom = map->m_nom_fichier;


            delete map;

            map = new Map();
            if (!map->Charger(temp3.str()))
            {
                map->m_no_backup = no-= 2;
                if (map->m_no_backup < 0)
                    map->m_no_backup += 50;
                redo();
            }
            map->m_no_backup = no;
            map->m_nom_fichier = nom;
            MettreListesAJour();

            map->m_selectTileset    = m_selectTileset;
            map->m_selectTile       = m_selectTile;
            map->m_selectHerbe      = m_selectHerbe;
            map->m_selectEntite     = m_selectEntite;
            map->m_selectTileset    = m_selectEvenement;
            map->m_select_brush     = m_select_brush;
            map->m_mode_brush   = m_mode_brush;

        }
    }
}


void MainWindow::importerTileset()
{
    QStringList  cheminList;
    cheminList = QFileDialog::getOpenFileNames(this, "Ouvrir un tileset", "Data/Graphics/Landscapes", "Tileset (*.ts.hs )");

    for (int j = 0; j < cheminList.size(); ++j)
        if (!cheminList.at(j).toStdString().empty())
        {
            std::string temp = cheminList.at(j).toStdString();
            std::string temp2 = "Data";

            for (unsigned k = 0 ; k < temp.size() ; ++k)
            {
                if (temp[k] == 'D')
                    if (k ++ < temp.size())
                        if (temp[k] == 'a')
                            if (k ++ < temp.size())
                                if (temp[k] == 't')
                                    if (k ++ < temp.size())
                                        if (temp[k] == 'a')
                                            if (k ++ < temp.size())
                                                if (temp[k] == '/')
                                                {
                                                    for (; k < temp.size() ; ++k)
                                                        temp2.push_back(temp[k]);
                                                }
            }

            temp = temp2;

            if (map != NULL)
            {
                bool add = true;
                for (unsigned i = 0 ; i < map->m_tileset.size() ; ++i)
                    if (moteurGraphique->getTileset(map->m_tileset[i])->getChemin() == temp)
                    {
                        add = false;
                        QMessageBox::information(this, "Tileset existant", "Ce tileset a déjà été ajouté à la map.");
                    }
                if (add)
                {
                    map->m_tileset.push_back(moteurGraphique->AjouterTileset(temp));
                }
            }
            else
                QMessageBox::critical(this, "Erreur", "Il faut avoir chargé une map pour importer un tileset.");
        }

    MettreListesAJour();
    backup();
}

void MainWindow::importerHerbe()
{
    QStringList  cheminList;
    cheminList = QFileDialog::getOpenFileNames(this, "Ouvrir un tileset", "Data/Graphics/Grass", "Tileset (*.ts.hs )");

    for (int j = 0; j < cheminList.size(); ++j)
        if (!cheminList.at(j).toStdString().empty())
        {
            std::string temp = cheminList.at(j).toStdString();
            std::string temp2 = "Data";

            for (unsigned k = 0 ; k < temp.size() ; ++k)
            {
                if (temp[k] == 'D')
                    if (k ++ < temp.size())
                        if (temp[k] == 'a')
                            if (k ++ < temp.size())
                                if (temp[k] == 't')
                                    if (k ++ < temp.size())
                                        if (temp[k] == 'a')
                                            if (k ++ < temp.size())
                                                if (temp[k] == '/')
                                                {
                                                    for (; k < temp.size() ; ++k)
                                                        temp2.push_back(temp[k]);
                                                }
            }

            temp = temp2;

            if (map != NULL)
            {
                bool add = true;
                for (unsigned i = 0 ; i < map->m_herbe.size() ; ++i)
                    if (moteurGraphique->getTileset(map->m_herbe[i])->getChemin() == temp)
                    {
                        add = false;
                        QMessageBox::information(this, "Tileset existant", "Ce tileset a déjà été ajouté à la map.");
                    }
                if (add)
                {
                    map->m_herbe.push_back(moteurGraphique->AjouterTileset (temp));
                }
            }
            else
                QMessageBox::critical(this, "Erreur", "Il faut avoir chargé une map pour importer un tileset.");
        }

    MettreListesAJour();
    backup();
}


void MainWindow::importerEntite()
{
    QStringList  cheminList;
    cheminList = QFileDialog::getOpenFileNames(this, "Ouvrir une entite", "Data/Entities/Resources", "Entite (*.char.hs )");

    for (int j = 0; j < cheminList.size(); ++j)
        if (!cheminList.at(j).toStdString().empty())
        {
            std::string temp = cheminList.at(j).toStdString();
            std::string temp2 = "Data";

            for (unsigned k = 0 ; k < temp.size() ; ++k)
            {
                if (temp[k] == 'D')
                    if (k ++ < temp.size())
                        if (temp[k] == 'a')
                            if (k ++ < temp.size())
                                if (temp[k] == 't')
                                    if (k ++ < temp.size())
                                        if (temp[k] == 'a')
                                            if (k ++ < temp.size())
                                                if (temp[k] == '/')
                                                {
                                                    for (; k < temp.size() ; ++k)
                                                        temp2.push_back(temp[k]);
                                                }
            }

            temp = temp2;

            if (map != NULL)
            {
                bool add = true;
                for (unsigned i = 0 ; i < map->m_ModeleMonstre.size() ; ++i)
                    if (map->m_ModeleMonstre[i].m_chemin== temp)
                    {
                        add = false;
                        QMessageBox::information(this, "Entité existant", "Cette entité a déjà été ajoutée à la map.");
                    }
                if (add)
                {
                    map->m_ModeleMonstre.push_back(Modele_Monstre ());
                    map->m_ModeleMonstre.back().Charger(temp);
                }
            }
            else
                QMessageBox::critical(this, "Erreur", "Il faut avoir chargé une map pour importer une entité.");
        }

    MettreListesAJour();
    backup();
}

void MainWindow::script()
{
    if (map != NULL)
        text_script->show();
}

void MainWindow::selectTileset(QTreeWidgetItem *item, int column)
{
    // std::cout<<item->text(column).toStdString()<<std::endl;

    map->m_selectEntite      = -1;
    map->m_selectHerbe       = -1;

    //std::cout<<item->parent()->text(column).toStdString()<<std::endl;
    if (item->parent() != NULL)
    {
        map->m_selectTileset = item->parent()->data(1,0).toInt();
        map->m_selectTile = item->data(1,0).toInt();
    }
    else
    {
        map->m_selectTileset = item->data(1,0).toInt();
        map->m_selectTile = 0;
    }

    if(map->m_selectTile<moteurGraphique->getTileset(map->m_tileset[map->m_selectTileset-1])->m_tile.size())
     if(moteurGraphique->getTileset(map->m_tileset[map->m_selectTileset-1])->m_tile[map->m_selectTile].m_couche_editeur >= 0
      &&moteurGraphique->getTileset(map->m_tileset[map->m_selectTileset-1])->m_tile[map->m_selectTile].m_couche_editeur < 2)
     {
        map->m_selectCouche = moteurGraphique->getTileset(map->m_tileset[map->m_selectTileset-1])->m_tile[map->m_selectTile].m_couche_editeur;
        couche0->setChecked(false);
        couche1->setChecked(false);
        if(map->m_selectCouche == 0)
            couche0->setChecked(true);
        if(map->m_selectCouche == 1)
            couche1->setChecked(true);
     }

    map->m_mode_brush       = false;

    SFMLView->setFocus(Qt::MouseFocusReason);
}


void MainWindow::selectHerbe(QTreeWidgetItem *item, int column)
{
    map->m_selectEntite      = -1;
    map->m_selectTileset     = -1;
    map->m_selectTile        = -1;

    map->m_selectHerbe = item->data(1,0).toInt();

    map->m_mode_brush       = false;

    SFMLView->setFocus(Qt::MouseFocusReason);
}
void MainWindow::selectEntite(QTreeWidgetItem *item, int column)
{
    map->m_selectHerbe       = -1;
    map->m_selectTileset     = -1;
    map->m_selectTile        = -1;

    map->m_selectEntite = item->data(1,0).toInt();

    map->m_mode_brush       = false;

    SFMLView->setFocus(Qt::MouseFocusReason);
}

void MainWindow::addTileset(QTreeWidgetItem *item, int column)
{
    if (map != NULL)
    {
        map->m_selectEntite      = -1;
        map->m_selectTileset     = -1;
        map->m_selectTile        = -1;
        map->m_selectHerbe       = -1;

        if (item->parent() != NULL)
        {
            map->m_select_brush.m_tileset = item->parent()->data(1,0).toInt() - 1;
            map->m_select_brush.m_tile.push_back(item->data(1,0).toInt());
        }

        map->m_mode_brush       = true;

        SFMLView->setFocus(Qt::MouseFocusReason);
    }
}
void MainWindow::addHerbe(QTreeWidgetItem *item, int column)
{
    if (map != NULL)
    {
        map->m_selectEntite      = -1;
        map->m_selectTileset     = -1;
        map->m_selectTile        = -1;
        map->m_selectHerbe       = -1;

        map->m_select_brush.m_herbe = item->data(1,0).toInt() - 1;

        map->m_mode_brush       = true;

        SFMLView->setFocus(Qt::MouseFocusReason);
    }
}
void MainWindow::addEntite(QTreeWidgetItem *item, int column)
{
    if (map != NULL)
    {
        map->m_selectEntite      = -1;
        map->m_selectTileset     = -1;
        map->m_selectTile        = -1;
        map->m_selectHerbe       = -1;

        map->m_monstre.push_back(Monstre ());
        map->m_monstre.back().Charger(item->data(1,0).toInt() - 1,&map->m_ModeleMonstre[item->data(1,0).toInt() - 1]);
        map->m_monstre.back().setCoordonnee(eventManager->getCasePointee()),map->m_monstre.back().setDepart();
        map->m_add_monstre.push_back(false);

        map->m_select_brush.m_monstre.push_back(map->m_monstre.size()-1);

        map->m_mode_brush       = true;

        SFMLView->setFocus(Qt::MouseFocusReason);
    }
}




