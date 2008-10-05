
#include "configuration.h"

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

    setFixedSize(512, 512);

    text = new QLabel(this);
    text->setScaledContents(false);
    if((int)((config.contrastes-1)*100/9)<10)
        sprintf(texte,"Taux de contrastes : 00%i",(int)((config.contrastes-1)*100/9));
    else if((int)((config.contrastes-1)*100/9)<100)
        sprintf(texte,"Taux de contrastes : 0%i",(int)((config.contrastes-1)*100/9));
    else
        sprintf(texte,"Taux de contrastes : %i",(int)((config.contrastes-1)*100/9));
    text->setText(texte);
    text->move(128,40);

    text2 = new QLabel(this);
    if((int)config.luminosite*100/64<10)
        sprintf(texte,"Taux de luminosité : 00%i",(int)config.luminosite*100/64);
    else if((int)config.luminosite*100/64<100)
        sprintf(texte,"Taux de luminosité : 0%i",(int)config.luminosite*100/64);
    else
        sprintf(texte,"Taux de luminosité : %i",(int)config.luminosite*100/64);
    text2->setText(texte);
    text2->move(128,72);

    text3 = new QLabel(this);
    if((int)config.volume<10)
        sprintf(texte,"Volume sonore : 00%i",(int)config.volume);
    else if((int)config.volume<100)
        sprintf(texte,"Volume sonore : 0%i",(int)config.volume);
    else
        sprintf(texte,"Volume sonore : %i",(int)config.volume);
    text3->setText(texte);
    text3->move(128,104);

    text4 = new QLabel("Résolution :",this);
    text4->move(128,160);

    text5 = new QLabel(this);
    sprintf(texte,"Qualité des lumières : %i",(int)config.Lumiere);
    text5->setText(texte);
    text5->move(128,136);

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



    modeFenetre= new QCheckBox("Mode fenêtré", this);
    modeFenetre->move(128,176);
    modeFenetre->setChecked(config.mode_fenetre);

    syncro= new QCheckBox("Syncronisation verticale", this);
    syncro->move(128,192);
    syncro->setChecked(config.syncronisation_verticale);

    /*lumieres= new QCheckBox("Afficher les lumières", this);
    lumieres->move(128,192);
    lumieres->setChecked(config.Lumiere);*/

    ombres= new QCheckBox("Afficher les ombres", this);
    ombres->move(128,208);
    ombres->setChecked(config.Ombre);

    fx= new QCheckBox("Utiliser les post-effects", this);
    fx->move(128,224);
    fx->setChecked(config.postFX);

    sang= new QCheckBox("Afficher le sang", this);
    sang->move(128,240);
    sang->setChecked(config.sang);

    herbes= new QCheckBox("Afficher l'herbe", this);
    herbes->move(128,256);
    herbes->setChecked(config.Herbes);

    /*coordonnee temp;


    resolutions.push_back(temp);
    resolutions[resolutions.size()-1].x=640;
    resolutions[resolutions.size()-1].y=480;

    resolutions.push_back(temp);
    resolutions[resolutions.size()-1].x=800;
    resolutions[resolutions.size()-1].y=600;

    resolutions.push_back(temp);
    resolutions[resolutions.size()-1].x=1024;
    resolutions[resolutions.size()-1].y=768;*/

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


    m_boutonRetour = new QPushButton("Sauvegarder et quitter", this);
    m_boutonRetour->setGeometry(128,448,256,32);


    QObject::connect(barreTauxLuminosite, SIGNAL(valueChanged(int)), this, SLOT(ChangerLuminosite(int)));
    QObject::connect(barreTauxContrastes, SIGNAL(valueChanged(int)), this, SLOT(ChangerContraste(int)));
    QObject::connect(barreVolume, SIGNAL(valueChanged(int)), this, SLOT(ChangerVolume(int)));
    QObject::connect(barreLumiere, SIGNAL(valueChanged(int)), this, SLOT(ChangerLumiere(int)));


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

    if(ombres->isChecked())
        config.Ombre=1;
    else
        config.Ombre=0;


    if(fx->isChecked())
        config.postFX=1;
    else
        config.postFX=0;


    if(sang->isChecked())
        config.sang=1;
    else
        config.sang=0;


    if(herbes->isChecked())
        config.Herbes=1;
    else
        config.Herbes=0;

    config.Resolution.w=resolutions[listeResolution->currentIndex()].x;
    config.Resolution.h=resolutions[listeResolution->currentIndex()].y;

    config.Sauvegarder();
    this->close();
}

void OptionsJeu::ChangerContraste(int contraste)
{
    config.contrastes=((float)contraste/10)+1;

    char texte[32];
    if((int)((config.contrastes-1)*100/9)<10)
        sprintf(texte,"Taux de contrastes : 00%i",(int)((config.contrastes-1)*100/9));
    else if((int)((config.contrastes-1)*100/9)<100)
        sprintf(texte,"Taux de contrastes : 0%i",(int)((config.contrastes-1)*100/9));
    else
        sprintf(texte,"Taux de contrastes : %i",(int)((config.contrastes-1)*100/9));
    text->setText(texte);
}
void OptionsJeu::ChangerLuminosite(int luminosite)
{
    config.luminosite=luminosite;
    char texte[32];

    if((int)config.luminosite*100/64<10)
        sprintf(texte,"Taux de luminosité : 00%i",(int)config.luminosite*100/64);
    else if((int)config.luminosite*100/64<100)
        sprintf(texte,"Taux de luminosité : 0%i",(int)config.luminosite*100/64);
    else
        sprintf(texte,"Taux de luminosité : %i",(int)config.luminosite*100/64);
    text2->setText(texte);
}
void OptionsJeu::ChangerLumiere(int lumiere)
{
    config.Lumiere=lumiere;
    char texte[32];

    sprintf(texte,"Qualité des lumières : %i",(int)config.Lumiere);
    text5->setText(texte);
}
void OptionsJeu::ChangerVolume(int volume)
{
    config.volume=volume;
    char texte[32];

    if((int)config.volume<10)
        sprintf(texte,"Volume sonore : 00%i",(int)config.volume);
    else if((int)config.volume<100)
        sprintf(texte,"Volume sonore : 0%i",(int)config.volume);
    else
        sprintf(texte,"Volume sonore : %i",(int)config.volume);
    text3->setText(texte);
}
