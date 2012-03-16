#include "Launcher.h"
#include <wchar.h>
//using namespace boost::filesystem;

Launcher::Launcher() : QWidget() {
    _langages                       = new QList<Langage>;
    _fichiersLocaux                 = new QList<Fichier>;
    _fichiersATelecharger           = NULL;
    _fichierEnTelechargement        = NULL;
    _nombreDeFichiersTelecharges    = 0;
    _optionsJeu                     = new OptionsJeu;
    _fichier                        = NULL;
    _http                           = NULL;
    _installer                      = QFile::exists(PATH_FICHIER_MAJ_TXT_LOCAL) ? true : false;

    setGeometry ( QApplication::desktop()->width()/2-250,  QApplication::desktop()->height()/2-300, 500, 600 );

    //Chargement du CSS
    {
        QFile feuilleCSS("Data/Menus/Launcher/launcher.css");
        if(feuilleCSS.open(QIODevice::ReadOnly)) {
            setStyleSheet(QString(feuilleCSS.readAll()));
        }
        else {
            qWarning("Le fichier launcher.css n'existe pas : ");
            qWarning("      L'interface du Launcher ne sera donc pas affichée.");
        }

        feuilleCSS.close();
    }

    QLabel *fond = new QLabel(this);
    //On ajoute un fond.
    {
        QPixmap pixmap(tr("Data/Menus/Launcher/Fond.png"));
        fond->setPixmap(pixmap);
        setAttribute(Qt::WA_TranslucentBackground);
        setWindowFlags(Qt::FramelessWindowHint);
        setMask(pixmap.mask());
    }
        
    m_boutonDemarrer = new QPushButton(tr("Lancer le jeu"), this);
        m_boutonDemarrer->setGeometry(114,324,271,32);
    m_boutonMettreAJour = new QPushButton(tr("Mettre le jeu à jour"), this);
        m_boutonMettreAJour->setGeometry(QRect(114,360,271,32));
        m_boutonMettreAJour->setEnabled(false);
    m_miseAJour = new QProgressBar(this);
        m_miseAJour->setValue(0);
        m_miseAJour->setTextVisible(0);
        m_miseAJour->setGeometry(121,400,260,20);
    m_miseAJourTotale = new QProgressBar(this);
        m_miseAJourTotale->setValue(0);
        m_miseAJourTotale->setTextVisible(0);
        m_miseAJourTotale->setGeometry(121,422,260,20);
    m_boutonOptions = new QPushButton(tr("Options"), this);
     m_boutonOptions->setGeometry(114,448,271,32);
    m_boutonQuitter = new QPushButton(tr("Quitter"), this);
        m_boutonQuitter->setGeometry(186,484,128,32);
        m_boutonQuitter->setObjectName("boutonQuitter");


    QFrame *frame = new QFrame(this);
    frame->setFrameShape(QFrame::StyledPanel);
    frame->setGeometry(64, 160, 372, 160);

    QWebView *pageWeb = new QWebView(frame);
        pageWeb->setGeometry(2,2,368,156);
        pageWeb->load(QUrl(tr(PATH_FICHIER_NEWS_DISTANT)));
        pageWeb->setContentsMargins(0,0,0,0);


    {
        std::ifstream f_langues(PATH_FICHIER_LISTE_LANGAGES, std::ios::in);
        if(f_langues) {
            char caractere;

            while(!f_langues.eof()) {
                    f_langues.get(caractere);

                    if(caractere == '*') {
                            Langage langue;
                            f_langues >> langue.alias >> langue.langue;
                            _langages->push_back( langue );
                    }
                    else if(caractere == '$')
                        break;

            }
        }
        else
            qWarning("Le fichier liste_langages.conf n'a pu être ouvert.");
    }
    {
        std::ifstream fichierLocal(PATH_FICHIER_MAJ_CONF_LOCAL, std::ios::in);
        if(fichierLocal.good()) {
            char caractere;

            while(!fichierLocal.eof()) {
                if(fichierLocal.tellg() == -1)
                    break;
                fichierLocal.get(caractere);
                if(caractere == '*') {
                    Fichier fichier;
                    fichierLocal >> fichier.nom >> fichier.version;
                    _fichiersLocaux->push_back(fichier);
                }
                else if(caractere == '$')
                    break;
            }

            fichierLocal.close();
        }
    }

    QComboBox *listeLangages = new QComboBox(this);
    listeLangages->move(64, 556);

    Configuration config;
    config.Charger();

    const int nombreLangages = _langages->size();
    for(int i = 0; i < nombreLangages; i++) {
        listeLangages->addItem(QString::fromStdString(_langages->at(i).langue));

        if(config.language == _langages->at(i).alias)
           listeLangages->setCurrentIndex(i);

    }


    QObject::connect(m_boutonQuitter,       SIGNAL(clicked()),                  qApp,        SLOT(quit()));
    QObject::connect(m_boutonMettreAJour,   SIGNAL(clicked()),                  this,        SLOT(telechargerFichier()));
    QObject::connect(m_boutonOptions,       SIGNAL(clicked()),                  _optionsJeu, SLOT(exec()));
    QObject::connect(m_boutonDemarrer,      SIGNAL(clicked()),                  qApp,        SLOT(quit()));
    QObject::connect(m_boutonDemarrer,      SIGNAL(clicked()),                  this,        SLOT(LancerJeu()));
    QObject::connect(listeLangages,         SIGNAL(currentIndexChanged(int)),   this,        SLOT(languageChange(int)));

    telechargerFichierMAJDistant();
}
Launcher::~Launcher() {
        delete m_boutonDemarrer;
        delete m_boutonQuitter;
        delete m_boutonMettreAJour;
        delete m_miseAJour;
        delete m_miseAJourTotale;
        delete m_boutonOptions;
        delete _langages;
        delete _fichiersATelecharger;
        delete _fichiersLocaux;
        delete _fichier;
        delete _http;
        delete _optionsJeu;
}
void Launcher::telechargerFichierMAJDistant()  {
    QUrl url(PATH_FICHIER_MAJ_DISTANT);

   _fichier = new QFile(PATH_FICHIER_MAJ_TXT_LOCAL,this);
   _http    = new QHttp( url.host(),
                        (url.scheme().toLower() == "https") ? QHttp::ConnectionModeHttps : QHttp::ConnectionModeHttp,
                        (url.port() == -1) ? 0 : url.port(),
                        this);


   if(_fichier->open(QIODevice::WriteOnly)) {
       m_boutonMettreAJour->setText(tr("Téléchargement de la base de données..."));

       _http->get(QUrl::toPercentEncoding(url.path(), "!$&'()*+,;=:@/"), _fichier);

       QObject::connect(_http, SIGNAL(dataReadProgress(int,int)),  this, SLOT(miseAJourBarre2(int,int)));
       QObject::connect(_http, SIGNAL(done(bool)),                 this, SLOT(verificationPresenceMiseAJour(bool)));
   }
   else
       qWarning("Le fichier maj.txt local n'a pu etre ouvert.");
}
void Launcher::verificationPresenceMiseAJour(bool demarrer) {
    //On a ouvert le fichier maj.txt dans Launcher::telechargerFichierMAJDistant()
    //Donc on le ferme.
    _fichier->close();

    //demarrer vaut true si done renvoit une erreur.
    if(demarrer) {
       m_boutonDemarrer->setEnabled(true);
       m_boutonMettreAJour->setText(tr("Mise à jour impossible"));
       m_boutonMettreAJour->setEnabled(false);
       return;
    }


    //Récupération de la liste des fichiers distants.
    QList<Fichier> *fichiersDistants = new QList<Fichier>;
    {
        std::ifstream f_listeDistante(PATH_FICHIER_MAJ_TXT_LOCAL, std::ios::in);

        if(f_listeDistante.good()) {
            char caractere;

            while(!f_listeDistante.eof()) {
                f_listeDistante.get(caractere);
                if(f_listeDistante.tellg() == -1)
                    break;

                if(caractere == '*') {
                    Fichier fichier;
                    f_listeDistante >> fichier.nom >> fichier.repertoire >> fichier.version;
                    fichiersDistants->push_back(fichier);
                }
                else if( caractere == '$') break;
            }
            f_listeDistante.close();
        }
        else
            qWarning("Ouverture de maj.txt impossible.");

    }

    //La liste de fichiers est vide ==> Mise à jour en cours (sur le site).
    if(fichiersDistants->size() == 0) {
        m_boutonDemarrer->setEnabled(true);
        m_boutonMettreAJour->setText(tr("Maintenance du système.\n Réessayez dans quelques minutes."));
        m_boutonMettreAJour->setEnabled(false);
        return;
    }

    _fichiersLocaux = new QList<Fichier>;
    //Récupération de la liste des fichiers locaux.
    {
       std::ifstream f_listeLocale(PATH_FICHIER_MAJ_CONF_LOCAL, std::ios::in);
       if(f_listeLocale.good()) {
           char caractere;
           while(!f_listeLocale.eof()) {
               f_listeLocale.get(caractere);

               if(f_listeLocale.tellg() == -1)
                   break;

               if(caractere == '*') {
                    Fichier fichier;
                    f_listeLocale >> fichier.nom >> fichier.version;
                    _fichiersLocaux->push_back(fichier);
               }
               else if(caractere == '$') break;
           }
           f_listeLocale.close();
       }
       else {
           qWarning("Impossible d'ouvrir le fichier maj.conf : ");
           std::ofstream f_conf(PATH_FICHIER_MAJ_CONF_LOCAL,std::ios::out);
           if(f_conf.good()) {
               f_conf << "$";
               f_conf.close();
               qWarning("    Fichier maj.conf recréé");
               qWarning("    Attention cela retéléchargera le jeu dans son entier.");
               verificationPresenceMiseAJour(false);
           }
           else
               qWarning("    Impossible de créér ce fichier.");
       }
    }
    

//    //Aucun fichier n'a été installé !!
    if(_fichiersLocaux->size() < 500) {
        m_boutonDemarrer->setEnabled(false);
        m_boutonMettreAJour->setEnabled(true);
        m_boutonMettreAJour->setText(tr("Télécharger et installer"));
    }


    //Sélection des fichiers à télécharger.
    _fichiersATelecharger = new QQueue<Fichier>;
    {
        QStringList listePath;
        QDir dir;
        const int nombreFichiersDistants = fichiersDistants->size(),
                  nombreFichiersLocaux   = _fichiersLocaux->size();



        //Si il n'y aucun fichier d'installer !!
        if(nombreFichiersLocaux == 0) {
            for(int i = 0; i < nombreFichiersDistants; i++)
                _fichiersATelecharger->enqueue(fichiersDistants->at(i));

        }
        else {
            for(int i = 0; i < nombreFichiersDistants; i++) {
                for(int j = 0; j < nombreFichiersLocaux; j++) {

                    if( _fichiersLocaux->at(j).nom == fichiersDistants->at(i).nom && _fichiersLocaux->at(j).version >= fichiersDistants->at(i).version)
                        break;
                        //Fichier présent mais de version trop ancienne.
                    else if( _fichiersLocaux->at(j).nom == fichiersDistants->at(i).nom && _fichiersLocaux->at(j).version < fichiersDistants->at(i).version) {
                        _fichiersATelecharger->enqueue(fichiersDistants->at(i));
                        break;
                    }
                    //Fichier non présent.
                    else if(j == (nombreFichiersLocaux-1) && fichiersDistants->at(i).nom != _fichiersLocaux->at(j).nom ) {
                        _fichiersATelecharger->enqueue(fichiersDistants->at(i));
                    }

                }
            }
        }


        //Certains dossiers ne peuvent exister donc on va pallier à ce petit problème.
        const int nombreFichiersATelecharger = _fichiersATelecharger->size();
        for(int i = 0; i < nombreFichiersATelecharger; i++) {
            if(!dir.exists( fichiersDistants->at(i).repertoire.c_str() ))
                listePath.append(fichiersDistants->at(i).repertoire.c_str());
        }
        //Création des paths.
        listePath.removeDuplicates();
        const int nombrePaths = listePath.size();
        for(int i = 0; i < nombrePaths; i++)
            dir.mkpath(listePath[i]);
    }

    delete fichiersDistants;

    //On s'occupe de supprimer les fichiers inutiles et de les ajouter dans le fichier maj.conf
    //Ca permet d'éviter de télécharger des fichiers inutiles.
#ifdef WINDOWS
    std::ifstream f_specific("Data/Menus/Launcher/specific.linux",std::ios::in);
#endif
#ifdef LINUX
    std::ifstream f_specific("Data/Menus/Launcher/specific.windows",std::ios::in);
#endif
    if(f_specific.good()) {
        QList<std::string> fichiersAExclure;
        while(!f_specific.eof()) {
            char caractere;
            f_specific.get(caractere);
            if(caractere == '*') {
                std::string fichier;
                f_specific >> fichier;
                fichiersAExclure.append(fichier);
            }
            else if(caractere == '$')
                break;

        }
        const int n_fichiersAExclure = fichiersAExclure.size();
        int n_fichiersATelecharger = _fichiersATelecharger->size();
        for(int i = 0; i < n_fichiersAExclure; i++) {
            for(int j = 0; j < n_fichiersATelecharger; j++) {
                if(_fichiersATelecharger->at(j).nom == fichiersAExclure[i] ) {
                    qWarning("Le fichier %s ne sera pas téléchargé comme spécifié dans le fichier specific.system.", fichiersAExclure[i].c_str());
                    ajouterFichierDansTelecharges(_fichiersATelecharger->at(j));

                    _fichiersATelecharger->removeAt(j);

                    //On vient de supprimer un élément donc nécessairement :
                    n_fichiersATelecharger--;
                }
            }
        }
    }
    else
        qWarning("Impossible d'ouvrir les fichiers de spécifications.");

    if(_fichiersATelecharger->size() > 0) {
        m_boutonMettreAJour->setEnabled(true);
        m_boutonMettreAJour->setText(tr("Mise à jour disponible."));
    }
    else {
        m_boutonDemarrer->setEnabled(true);
        m_boutonMettreAJour->setEnabled(false);
        m_boutonMettreAJour->setText(tr("Aucune mise à jour trouvée"));
    }

 }


void Launcher::telechargerFichier(bool erreur) {

    //Sert à ajouter une ligne dans le fichier maj.conf (permet d'éviter le retéléchargement de tous les fichiers déjà téléchargés).
    if(_fichierEnTelechargement) {
        qWarning("    Le fichier %s a fini d'être téléchargé.",_fichierEnTelechargement->nom.c_str());
        _fichier->close();

        _nombreDeFichiersTelecharges++;

        ajouterFichierDansTelecharges(*_fichierEnTelechargement);
    }

    delete _http;

    m_boutonMettreAJour->setEnabled(false);
    m_boutonDemarrer->setEnabled(false);

    miseAJourBarreTotale( _nombreDeFichiersTelecharges, _fichiersATelecharger->size());

    //Il y a encore des fichiers
    if(_fichiersATelecharger->size() > 0) {
        if(erreur) {
            QMessageBox::information(0, tr("Erreur"), _http->error() == 7 ? tr("Mise à jour interrompue.") : tr("Mise à jour impossible."));
            return;
        }

        _fichierEnTelechargement = new Fichier(_fichiersATelecharger->dequeue());

        qWarning("Téléchargement en cours du fichier %s.", _fichierEnTelechargement->nom.c_str());
        m_boutonMettreAJour->setText(_fichierEnTelechargement->nom.c_str());

        QUrl url(QString("http://holyspirit.alpha-arts.net/Fichiers_jeu/Holyspirit/%1").arg(_fichierEnTelechargement->nom.c_str()));

        _fichier->setFileName(_fichierEnTelechargement->nom.c_str());
        _http = new QHttp( url.host(),
                   url.scheme().toLower() == "https" ? QHttp::ConnectionModeHttps : QHttp::ConnectionModeHttp,
                   url.port() == -1 ? 0 : url.port(),
                   this);



        if(!_fichier->open(QIODevice::WriteOnly))
            qWarning("Impossible d'ouvrir le fichier %s : mise à jour stoppée.", _fichierEnTelechargement->nom.c_str());

        _http->get(QUrl::toPercentEncoding(url.path(), "!$&'()*+,;=:@/"), _fichier);

        QObject::connect(_http, SIGNAL( done(bool)),                 this, SLOT(telechargerFichier(bool)));
        QObject::connect(_http, SIGNAL( dataReadProgress(int,int)),  this, SLOT(miseAJourBarre(int,int)));
    }
    else {
        m_boutonDemarrer->setText(tr("Lancer le jeu"));

        //Le pointeur _fichiersATelecharger n'est initialisé que lorsqu'il y a une mise à jour en cours sinon il est vaut NULL.
        if(_fichiersATelecharger) {
            m_boutonDemarrer->setEnabled(true);
            m_boutonMettreAJour->setText(tr("Mise à jour terminée."));
        }
        else
            m_boutonMettreAJour->setText(tr("Aucune mise à jour."));
    }

}

void Launcher::ajouterFichierDansTelecharges(const Fichier &fichier) {
    bool fichierEnregistre = false;
    {
        const int nombreFichiersLocaux = _fichiersLocaux->size();
        for(int i = 0; i < nombreFichiersLocaux; i++) {
            if(_fichiersLocaux->at(i).nom == fichier.nom) {
                _fichiersLocaux->replace(i,(Fichier){fichier.nom,"",fichier.version});
                fichierEnregistre = true;
                break;
            }
        }
    }
    if(!fichierEnregistre)
        _fichiersLocaux->push_back(fichier);

    const int nombreFichiersLocaux = _fichiersLocaux->size();
    std::ofstream fichierLocal(PATH_FICHIER_MAJ_CONF_LOCAL,std::ios::out);
    if(fichierLocal.good()) {
        for(int i = 0; i < nombreFichiersLocaux; i++)
            fichierLocal << "* " << _fichiersLocaux->at(i).nom << " " << _fichiersLocaux->at(i).version << std::endl;
        fichierLocal << "$";
        fichierLocal.close();
    }

}

void Launcher::miseAJourBarre(int progression, int total) {
    m_miseAJour->setValue( progression * 100 / total);
    miseAJourBarreTotale( (float)_nombreDeFichiersTelecharges + (float)progression / (float)total , _fichiersATelecharger->size());
}
void Launcher::miseAJourBarreTotale(float progression, int total) {
    m_miseAJourTotale->setValue( (int)( (float)progression * 100 / (float)total ) );
}
void Launcher::miseAJourBarre2(int progression, int total) {
    m_miseAJour->setValue(progression*100/total);
 }
void Launcher::LancerJeu() {
#ifdef WINDOWS
    ShellExecute(NULL,NULL,"Holyspirit.exe",NULL,NULL,SW_SHOWNORMAL);
#else
    #ifdef LINUX
       if(system("Holyspirit") == -1)
           qWarning("Holyspirit n'a pu etre lancé automatiquement.");
    #else
        #error "Holyspirit ne pourra etre lancé automatiquement : commmande d'OS non spécifié ou LINUX ou WINDOWS non défini."
    #endif
#endif

       this->hide();
 }
void Launcher::languageChange(int index) {
    Configuration config;

    config.Charger();
    config.language = _langages->at(index).alias;
    config.Sauvegarder();
}
