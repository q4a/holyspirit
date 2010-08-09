
#include "configuration.h"


void Configuration::Charger()
{
    ChargerConf();

    zoom=1;

    RafraichirOmbre=0;
    RafraichirLumiere=0;

    if (!Lumiere)
        Ombre=0;



}

void Configuration::ChargerConf()
{
    std::ifstream fichier;

    fichier.open("configuration.conf", std::ios::in);
    if (fichier)
    {
        std::string chaine;
        while (fichier>>chaine)
        {
            if (chaine== "resolution:")
            {
                fichier>>Resolution.x>>Resolution.y;
                Resolution.w=Resolution.x;
                Resolution.h=Resolution.y;
            }
            if (chaine== "brightness:")
                fichier>>luminosite;
            if (chaine== "contrast:")
                fichier>>contrastes;
            if (chaine== "alpha_opacity:")
                fichier>>alpha;
            if (chaine== "window:")
                fichier>>mode_fenetre;
            if (chaine== "videos:")
                fichier>>video;
            if (chaine== "postFX:")
                fichier>>postFX;
            if (chaine== "smooth:")
                fichier>>lissage;
            if (chaine== "shadows:")
                fichier>>Ombre;
            if (chaine== "reflection:")
                fichier>>Reflection;
            if (chaine== "distortion:")
                fichier>>Distortion;
            if (chaine== "light_quality:")
                fichier>>Lumiere;
            if (chaine== "grass:")
                fichier>>Herbes;
            if (chaine== "particules:")
                fichier>>particules;
            if (chaine== "vsync:")
                fichier>>syncronisation_verticale;
            if (chaine== "volume:")
                fichier>>volume;
            if (chaine== "music_volume:")
                fichier>>music_volume;
            if (chaine== "minimap:")
                fichier>>Minimap;
            if (chaine== "light_refresh_rate:")
                fichier>>frequence_lumiere;
            if (chaine== "console:")
                fichier>>console;
            if (chaine== "item_background:")
                fichier>>item_background;
            if (chaine== "zoom:")
                fichier>>zoom;

            if (chaine== "saving_frequency:")
                fichier>>frequence_sauvegarde;
            if (chaine== "debug_mod:")
                fichier>>debug;
            if (chaine== "desactivate_console:")
                fichier>>desactivate_console;
            if (chaine== "language:")
                fichier>>language;

        }
        fichier.close();
    }
    else
        throw std::string("Impossible de charger la configuration : configuration.conf");
}

void Configuration::Sauvegarder()
{
    ///Chargement de la configuration

       std::fstream fichier("configuration.conf", std::ios::in | std::ios::out | std::ios::trunc) ;

    if (fichier)
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
        fichier<< "reflection: " <<Reflection<<std::endl;
        fichier<< "distortion: " <<Distortion<<std::endl;
        fichier<< "videos: " <<video<<std::endl;
        fichier<< "postFX: " <<postFX<<std::endl;
        fichier<< "alpha_opacity: "<<alpha<<std::endl;
        fichier<< "grass: " <<Herbes<<std::endl;
        fichier<< "particules: " <<particules<<std::endl;
        fichier<< "smooth: " <<lissage<<std::endl;
        fichier<< "zoom: " <<zoom<<std::endl;
        fichier<<std::endl;
        fichier<<std::endl;
        fichier<<"SOUNDS"<<std::endl;
        fichier<< "volume: " <<volume<<std::endl;
        fichier<< "music_volume: " <<music_volume<<std::endl;
        fichier<<std::endl;
        fichier<<std::endl;
        fichier<<"INTERFACE"<<std::endl;
        fichier<< "minimap: " <<Minimap<<std::endl;
        fichier<< "console: " <<console<<std::endl;
        fichier<< "item_background: " <<item_background<<std::endl;
        fichier<<std::endl;
        fichier<<std::endl;
        fichier<<"OTHERS"<<std::endl;
        fichier<<"saving_frequency: " <<frequence_sauvegarde<<std::endl;
        fichier<<"debug_mod: "<<debug<<std::endl;
        fichier<<"desactivate_console: "<<desactivate_console<<std::endl;
        fichier<<"language: "<<language<<std::endl;


        fichier.close();
    }
    else
        throw "Impossible de sauvegarder la configuration";
}


OptionsJeu::OptionsJeu() : QDialog()
{
    char texte[255];





    std::ifstream fichier;
    fichier.open("liste_resolution.conf", std::ios::in);
    if(fichier)
    {
        char caractere;
    	do
    	{
    	    //Chargement du nom
    		fichier.get(caractere);
    		if(caractere=='*')
    		{
    		    coordonnee temp;
    			fichier>>temp.x>>temp.y;
    			resolutions.push_back(temp);
    		}

    		if(fichier.eof()){ caractere='$'; }
    	}while(caractere!='$');
    }





    config.Charger();

    setFixedSize(600, 500);

    text = new QLabel(this);
    text->setScaledContents(false);

    QString temp = tr("Taux de contrastes");

    if((int)((config.contrastes-1)*100/9)<10)
        sprintf(texte,"%s : 00%i",temp.toStdString().c_str(),(int)((config.contrastes-1)*100/9));
    else if((int)((config.contrastes-1)*100/9)<100)
        sprintf(texte,"%s : 0%i",temp.toStdString().c_str(),(int)((config.contrastes-1)*100/9));
    else
        sprintf(texte,"%s : %i",temp.toStdString().c_str(),(int)((config.contrastes-1)*100/9));
    text->setText(texte);
    text->move(128,40);

    temp = tr("Taux de luminosité");

    text2 = new QLabel(this);
    if((int)config.luminosite*100/64<10)
        sprintf(texte,"%s : 00%i",temp.toStdString().c_str(),(int)config.luminosite*100/64);
    else if((int)config.luminosite*100/64<100)
        sprintf(texte,"%s : 0%i",temp.toStdString().c_str(),(int)config.luminosite*100/64);
    else
        sprintf(texte,"%s : %i",temp.toStdString().c_str(),(int)config.luminosite*100/64);
    text2->setText(texte);
    text2->move(128,72);

    temp = tr("Volume sonore");

    text3 = new QLabel(this);
    if((int)config.volume<10)
        sprintf(texte,"%s : 00%i",temp.toStdString().c_str(),(int)config.volume);
    else if((int)config.volume<100)
        sprintf(texte,"%s : 0%i",temp.toStdString().c_str(),(int)config.volume);
    else
        sprintf(texte,"%s : %i",temp.toStdString().c_str(),(int)config.volume);
    text3->setText(texte);
    text3->move(128,104);

    text4 = new QLabel(tr("Résolution :"),this);
    text4->move(128,160);

    text5 = new QLabel(this);
    temp = tr("Qualité des lumières");
    sprintf(texte,"%s : %i",temp.toStdString().c_str(),(int)config.Lumiere);
    text5->setText(texte);
    text5->move(128,136);


    temp = tr("Opacité alpha");
    text6 = new QLabel(this);
    if((int)config.alpha<10)
        sprintf(texte,"%s : 00%i",temp.toStdString().c_str(),(int)config.alpha);
    else if((int)config.alpha<100)
        sprintf(texte,"%s : 0%i",temp.toStdString().c_str(),(int)config.alpha);
    else
        sprintf(texte,"%s : %i",temp.toStdString().c_str(),(int)config.alpha);
    text6->setText(texte);
    text6->setText(texte);
    text6->move(128,348);

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

    barreTauxLuminosite=new QSlider(Qt::Horizontal,this);
    barreTauxLuminosite->setGeometry(256,64,128,32);
    barreTauxLuminosite->setValue((int)config.luminosite);
    barreTauxLuminosite->setRange(0,64);

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

    distortion= new QCheckBox(tr("Effets de distortion"), this);
    distortion->move(128,320);
    distortion->setChecked(config.Distortion);

    bool AjouterResolution=true;

    listeResolution = new QComboBox(this);
    for(int i=0;i<(int)resolutions.size();i++)
    {
        sprintf(texte,"%i %i",resolutions[i].x,resolutions[i].y);
        listeResolution->addItem(texte);

        if(resolutions[i].x==config.Resolution.x&&resolutions[i].y==config.Resolution.y)
            AjouterResolution=false,listeResolution->setCurrentIndex(i);
    }

    if(AjouterResolution)
    {
        resolutions.push_back(config.Resolution);

        sprintf(texte,"%i %i",resolutions[resolutions.size()-1].x,resolutions[resolutions.size()-1].y);
        listeResolution->addItem(texte);

        listeResolution->setCurrentIndex(resolutions.size()-1);
    }

    listeResolution->move(256, 160);


    m_boutonRetour = new QPushButton(tr("Sauvegarder et quitter"), this);
    m_boutonRetour->setGeometry(128,448,256,32);


    QObject::connect(barreTauxLuminosite, SIGNAL(valueChanged(int)), this, SLOT(ChangerLuminosite(int)));
    QObject::connect(barreTauxContrastes, SIGNAL(valueChanged(int)), this, SLOT(ChangerContraste(int)));
    QObject::connect(barreVolume, SIGNAL(valueChanged(int)), this, SLOT(ChangerVolume(int)));
    QObject::connect(barreLumiere, SIGNAL(valueChanged(int)), this, SLOT(ChangerLumiere(int)));
    QObject::connect(barreAlpha, SIGNAL(valueChanged(int)), this, SLOT(ChangerAlpha(int)));


    QObject::connect(m_boutonRetour, SIGNAL(clicked()), this, SLOT(Quitter()));

    //sprintf(texte,"Taux de contrastes : %i",(int)((config.contrastes-1)*100/9));
    //text->setText(texte);
}

void OptionsJeu::Quitter()
{
    if(modeFenetre->isChecked())
        config.mode_fenetre=1;
    else
        config.mode_fenetre=0;


    if(syncro->isChecked())
        config.syncronisation_verticale=1;
    else
        config.syncronisation_verticale=0;

    if(reflect->isChecked())
        config.Reflection=1;
    else
        config.Reflection=0;

    if(ombres->isChecked())
        config.Ombre=1;
    else
        config.Ombre=0;


    if(fx->isChecked())
        config.postFX=1;
    else
        config.postFX=0;


    if(distortion->isChecked())
        config.Distortion=1;
    else
        config.Distortion=0;

    if(item_background->isChecked())
        config.item_background=1;
    else
        config.item_background=0;


    if(herbes->isChecked())
        config.Herbes=1;
    else
        config.Herbes=0;

    if(particule->isChecked())
        config.particules=1;
    else
        config.particules=0;

    if(lissage->isChecked())
        config.lissage=1;
    else
        config.lissage=0;

    config.Resolution.w=resolutions[listeResolution->currentIndex()].x;
    config.Resolution.h=resolutions[listeResolution->currentIndex()].y;

    config.Sauvegarder();
    this->close();
}

void OptionsJeu::ChangerContraste(int contraste)
{
    config.contrastes=((float)contraste/10)+1;

    QString temp = tr("Taux de contrastes");
    char texte[32];
    if((int)((config.contrastes-1)*100/9)<10)
        sprintf(texte,"%s : 00%i",temp.toStdString().c_str(),(int)((config.contrastes-1)*100/9));
    else if((int)((config.contrastes-1)*100/9)<100)
        sprintf(texte,"%s : 0%i",temp.toStdString().c_str(),(int)((config.contrastes-1)*100/9));
    else
        sprintf(texte,"%s : %i",temp.toStdString().c_str(),(int)((config.contrastes-1)*100/9));
    text->setText(texte);
}
void OptionsJeu::ChangerLuminosite(int luminosite)
{
    config.luminosite=luminosite;
    char texte[32];

    QString temp = tr("Taux de luminosité");
    if((int)config.luminosite*100/64<10)
        sprintf(texte,"%s : 00%i",temp.toStdString().c_str(),(int)config.luminosite*100/64);
    else if((int)config.luminosite*100/64<100)
        sprintf(texte,"%s : 0%i",temp.toStdString().c_str(),(int)config.luminosite*100/64);
    else
        sprintf(texte,"%s : %i",temp.toStdString().c_str(),(int)config.luminosite*100/64);
    text2->setText(texte);
}
void OptionsJeu::ChangerLumiere(int lumiere)
{
    config.Lumiere=lumiere;
    char texte[32];

    QString temp = tr("Qualité des lumières");
    sprintf(texte,"%s : %i",temp.toStdString().c_str(),(int)config.Lumiere);
    text5->setText(texte);
}
void OptionsJeu::ChangerAlpha(int alpha)
{
    config.alpha=alpha;
    char texte[32];

    QString temp = tr("Opacité alpha");
    if((int)config.alpha<10)
        sprintf(texte,"%s : 00%i",temp.toStdString().c_str(),(int)config.alpha);
    else if((int)config.alpha<100)
        sprintf(texte,"%s : 0%i",temp.toStdString().c_str(),(int)config.alpha);
    else
        sprintf(texte,"%s : %i",temp.toStdString().c_str(),(int)config.alpha);
    text6->setText(texte);
}
void OptionsJeu::ChangerVolume(int volume)
{
    config.volume=volume;
    char texte[32];

    QString temp = tr("Volume sonore");
    if((int)config.volume<10)
        sprintf(texte,"%s : 00%i",temp.toStdString().c_str(),(int)config.volume);
    else if((int)config.volume<100)
        sprintf(texte,"%s : 0%i",temp.toStdString().c_str(),(int)config.volume);
    else
        sprintf(texte,"%s : %i",temp.toStdString().c_str(),(int)config.volume);
    text3->setText(texte);
}
