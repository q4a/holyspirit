#include "optionsjeu.h"

OptionsJeu::OptionsJeu() : QDialog() {

    config.Charger();

    setFixedSize(600, 500);

    ms_contrastes      = new QLabel(tr("Taux de contrastes : %1")   .arg( (int)( (config.contrastes-1) * 100 / 9 ) ) ,this);
    ms_luminosite      = new QLabel(tr("Taux de luminosité : %1")   .arg( (int)config.luminosite * 100 / 64 )        ,this);
    ms_volume          = new QLabel(tr("Volume sonore : %1")        .arg( (int)config.volume)                        ,this);
    ms_resolution      = new QLabel(tr("Résolution :")                                                               ,this);
    ms_qualiteLumieres = new QLabel(tr("Qualité des lumières : %1") .arg( (int)config.Lumiere)                       ,this);
    ms_alpha           = new QLabel(tr("Opacité alpha : %1")        .arg( (int)config.alpha)                         ,this);

    ms_contrastes      ->move(60,40);
    ms_luminosite      ->move(60,72);
    ms_volume          ->move(60,104);
    ms_qualiteLumieres ->move(60,136);
    ms_resolution      ->move(60,160);
    ms_alpha           ->move(60,348);



    barreTauxContrastes=new QSlider(Qt::Horizontal,this);
    barreTauxContrastes->setGeometry(256,32,128,32);
    barreTauxContrastes->setValue((int)((config.contrastes-1)*10));
    barreTauxContrastes->setRange(0,90);

    barreTauxLuminosite=new QSlider(Qt::Horizontal,this);
    barreTauxLuminosite->setGeometry(256,64,128,32);
    barreTauxLuminosite->setValue((int)config.luminosite);
    barreTauxLuminosite->setRange(0,64);


    barreVolume=new QSlider(Qt::Horizontal,this);
    barreVolume->setGeometry(256,96,128,32);
    barreVolume->setValue((int)config.volume);
    barreVolume->setRange(0,100);

    barreLumiere=new QSlider(Qt::Horizontal,this);
    barreLumiere->setGeometry(256,128,128,32);
    barreLumiere->setValue((int)config.Lumiere);
    barreLumiere->setRange(0,2);

    barreAlpha=new QSlider(Qt::Horizontal,this);
    barreAlpha->setGeometry(256,342,128,32);
    barreAlpha->setValue((int)config.alpha);
    barreAlpha->setRange(64,255);



    modeFenetre= new QCheckBox(tr("Mode fenêtré"), this);
    modeFenetre->move(128,176);
    modeFenetre->setChecked(config.mode_fenetre);

    syncro= new QCheckBox(tr("Syncronisation verticale"), this);
    syncro->move(128,192);
    syncro->setChecked(config.syncronisation_verticale);


    ombres= new QCheckBox(tr("Afficher les ombres"), this);
    ombres->move(128,208);
    ombres->setChecked(config.Ombre);

    fx= new QCheckBox(tr("Utiliser les post-effects"), this);
    fx->move(128,224);
    fx->setChecked(config.postFX);

    item_background= new QCheckBox(tr("Fonds des objets colorés"), this);
    item_background->move(128,240);
    item_background->setChecked(config.item_background);

    herbes= new QCheckBox(tr("Afficher l'herbe"), this);
    herbes->move(128,256);
    herbes->setChecked(config.Herbes);

    particule= new QCheckBox(tr("Afficher les particules"), this);
    particule->move(128,272);
    particule->setChecked(config.particules);

    lissage= new QCheckBox(tr("Activer le lissage"), this);
    lissage->move(128,288);
    lissage->setChecked(config.lissage);

    reflect= new QCheckBox(tr("Réflections dans l'eau"), this);
    reflect->move(128,304);
    reflect->setChecked(config.Reflection);

    distortion= new QCheckBox(tr("Effets de distorsion"), this);
    distortion->move(128,320);
    distortion->setChecked(config.Distortion);

    listeResolution = new QComboBox(this);
    listeResolution->move(256, 160);

    m_boutonRetour = new QPushButton(tr("Sauvegarder et quitter"), this);
    m_boutonRetour->setGeometry(128,448,256,32);


    bool maResolutionEstAffichee = false;

    //On ouvre le fichier qui contient les différentes résolutions.
    std::ifstream f_resolutions("liste_resolution.conf",std::ios::in);
        if(f_resolutions) {
            char caractere;
            while(!f_resolutions.eof()){
                f_resolutions.get(caractere);
                if(caractere == '*') {
                    int width, height;
                    f_resolutions >> width >> height;
                    listeResolution->addItem(QString("%1").arg(width)+QString("x%2").arg(height));

                    if(config.Resolution.x == width && config.Resolution.y == height) {
                        listeResolution->setCurrentIndex(listeResolution->count() - 1);
                        maResolutionEstAffichee = true;
                    }
                }
                else if(caractere == '$') break;

            }
            f_resolutions.close();
        }
        else
            qWarning("Impossible d'ouvrir le fichier resolutions.list :\n\tLe choix des résolutions n'est' donc pas disponible.");


    if(!maResolutionEstAffichee) {
        listeResolution->addItem(QString("%1x").arg(config.Resolution.x) + QString("%1").arg(config.Resolution.y));
        listeResolution->setCurrentIndex(listeResolution->count() - 1);
    }




    QObject::connect(barreTauxLuminosite, SIGNAL(valueChanged(int)), this, SLOT(ChangerLuminosite(int)));
    QObject::connect(barreTauxContrastes, SIGNAL(valueChanged(int)), this, SLOT(ChangerContraste(int)));
    QObject::connect(barreVolume, SIGNAL(valueChanged(int)), this, SLOT(ChangerVolume(int)));
    QObject::connect(barreLumiere, SIGNAL(valueChanged(int)), this, SLOT(ChangerLumiere(int)));
    QObject::connect(barreAlpha, SIGNAL(valueChanged(int)), this, SLOT(ChangerAlpha(int)));
    QObject::connect(m_boutonRetour, SIGNAL(clicked()), this, SLOT(Quitter()));

}
OptionsJeu::~OptionsJeu() {
    delete  m_boutonRetour;
    delete  ms_contrastes;
    delete  ms_luminosite;
    delete  ms_volume;
    delete  ms_resolution;
    delete  ms_qualiteLumieres;
    delete  ms_alpha;
    delete  barreTauxContrastes;
    delete  barreTauxLuminosite;
    delete  barreVolume;
    delete  barreLumiere;
    delete  barreAlpha;
    delete  modeFenetre;
    delete  syncro;
    delete  reflect;
    delete  ombres;
    delete  fx;
    delete  distortion;
    delete  item_background;
    delete  herbes;
    delete  particule;
    delete  lissage;
    delete  listeResolution;
}
void OptionsJeu::Quitter() {

    config.mode_fenetre             = (modeFenetre->isChecked())     ? 1 : 0;
    config.syncronisation_verticale = (syncro->isChecked())          ? 1 : 0;
    config.Reflection               = (reflect->isChecked())         ? 1 : 0;
    config.Ombre                    = (ombres->isChecked())          ? 1 : 0;
    config.postFX                   = (fx->isChecked())              ? 1 : 0;
    config.Distortion               = (distortion->isChecked())      ? 1 : 0;
    config.item_background          = (item_background->isChecked()) ? 1 : 0;
    config.Herbes                   = (herbes->isChecked())          ? 1 : 0;
    config.particules               = (particule->isChecked())       ? 1 : 0;
    config.lissage                  = (lissage->isChecked())         ? 1 : 0;

    QString resolution = listeResolution->currentText();

    config.Resolution.w = resolution.left(resolution.indexOf("x")).toULongLong();
    config.Resolution.h = resolution.mid(resolution.indexOf("x") + 1).toULongLong();

    config.Sauvegarder();
    close();
}

void OptionsJeu::ChangerContraste(int contraste) {
    config.contrastes = ((float)contraste/10) + 1;
    ms_contrastes->setText( QString(tr("Taux de contrastes : %1"))
                 .arg(  (int)((config.contrastes - 1) * 100 / 9) ) );

}
void OptionsJeu::ChangerLuminosite(int luminosite) {
    config.luminosite = luminosite;
    ms_luminosite->setText(QString(tr("Taux de luminosité : %1"))
                    .arg((int)config.luminosite * 100 / 64) );

}
void OptionsJeu::ChangerLumiere(int lumiere) {
    config.Lumiere = lumiere;
    ms_qualiteLumieres->setText(QString(tr("Qualité des lumières : %1"))
                   .arg( (int)config.Lumiere ) );

}
void OptionsJeu::ChangerAlpha(int alpha) {
    config.alpha = alpha;
    ms_alpha->setText(QString(tr("Opacité alpha : %1"))
                    .arg( (int)config.alpha ) );

}
void OptionsJeu::ChangerVolume(int volume) {
    config.volume = volume;
    ms_volume->setText(QString(tr("Volume sonore : %1"))
                   .arg(  (int)config.volume ) );
}
