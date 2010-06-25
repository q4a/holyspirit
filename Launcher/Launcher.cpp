#include "Launcher.h"
#include <wchar.h>
//using namespace boost::filesystem;

Launcher::Launcher() : QWidget()
{
    enCoursDeTelechargement=-2;

    QDesktopWidget bureau;

    setGeometry ( bureau.screenGeometry().width()/2-250,  bureau.screenGeometry().height()/2-300, 500, 600 );

    m_fond = new QLabel(this);
    m_pixmap = new QPixmap("Data/Menus/Launcher/Fond.png");
    m_fond->setPixmap(*m_pixmap);
    this->setMask(m_pixmap->mask());

    m_boutonDemarrer = new QPushButton(tr("Lancer le jeu !"), this);
    m_boutonDemarrer->setGeometry(114,324,271,32);

    m_boutonMettreAJour = new QPushButton(tr("Mettre le jeu à jour"), this);
    m_boutonMettreAJour->setGeometry(114,360,271,32);

    m_boutonMettreAJour->setEnabled(false);

    m_boutonOptions = new QPushButton(tr("Options"), this);
    m_boutonOptions->setGeometry(114,448,271,32);

    // Création du bouton, ayant pour parent la "fenetre"
    m_boutonQuitter = new QPushButton(tr("Quitter"), this);
    m_boutonQuitter->setGeometry(186,484,128,32);

    QFrame *frame = new QFrame(this);
    frame->setFrameShape(QFrame::StyledPanel);
    frame->setGeometry(64, 160, 372, 160);
    frame->setStyleSheet(
    " QFrame, QLabel, QToolTip {"
      "   border: 2px inset black;"
       "  border-radius: 4px;"
      "   padding: 2px;"
    "}"
    );


    m_boutonDemarrer-> setStyleSheet(
      "QPushButton {"
        " border-radius: 6px;"
        "background-image: url(\"Data/Menus/Launcher/Button1.png\"); "
        "font-size: 18px;"
        "font-weight: bold;"
        "color: #700e0e;"
        "font-family: Garamond;"
    " }"

    " QPushButton:hover {"
        "background-image: url(\"Data/Menus/Launcher/Button2.png\"); "
        "color: #235824;"
    " }"

     "QPushButton:disabled {"
     "background-image: url(\"Data/Menus/Launcher/Button5.png\"); "
     "color: #700e0e;"
    " } "


     );

     m_boutonMettreAJour-> setStyleSheet(
      "QPushButton {"
        " border-radius: 6px;"
        "background-image: url(\"Data/Menus/Launcher/Button1.png\"); "
        "font-size: 14px;"
        "font-weight: bold;"
        "color: #700e0e;"
        "font-family: Garamond;"
    " }"

    " QPushButton:hover {"
        "background-image: url(\"Data/Menus/Launcher/Button2.png\"); "
        "color: #235824;"
    " }"

     "QPushButton:disabled {"
     "background-image: url(\"Data/Menus/Launcher/Button5.png\"); "
     "color: #700e0e;"
    " } "
     );

     m_boutonOptions-> setStyleSheet(
      "QPushButton {"
        " border-radius: 6px;"
        "background-image: url(\"Data/Menus/Launcher/Button1.png\"); "
        "font-size: 18px;"
        "font-weight: bold;"
        "color: #700e0e;"
        "font-family: Garamond;"
    " }"

    " QPushButton:hover {"
        "background-image: url(\"Data/Menus/Launcher/Button2.png\"); "
        "color: #235824;"
    " }"

     "QPushButton:disabled {"
     "background-image: url(\"Data/Menus/Launcher/Button5.png\"); "
    " } "
     );

     m_boutonQuitter->setStyleSheet(
      "QPushButton {"
        " border-radius: 6px;"
        "background-image: url(\"Data/Menus/Launcher/Button3.png\"); "
        "font-size: 18px;"
        "font-weight: bold;"
        "color: #700e0e;"
        "font-family: Garamond;"
    " }"


    " QPushButton:hover {"
        "background-image: url(\"Data/Menus/Launcher/Button4.png\"); "
        "color: #235824;"
    " }"
     );


    pageWeb = new QWebView(frame);
    pageWeb->setGeometry(2,2,368,156);
    pageWeb->load(QUrl(tr("http://holyspirit.alpha-arts.net/pages/news.php")));
    pageWeb->setContentsMargins(0,0,0,0);



    std::ifstream test;
        test.open("maj.txt", std::ios::in);

        if(!test)
        {
            m_installer=true;

        }
        else
            m_installer=false;

        test.close();

    dir=new QDir();

    m_miseAJour = new QProgressBar(this);
    m_miseAJour->setValue(0);
    m_miseAJour->hide();
    m_miseAJour->setTextVisible(0);
    m_miseAJour->setGeometry(121,400,260,20);

    m_miseAJourTotale = new QProgressBar(this);
    m_miseAJourTotale->setValue(0);
    m_miseAJourTotale->hide();
    m_miseAJourTotale->setTextVisible(0);
    m_miseAJourTotale->setGeometry(121,422,260,20);

    m_miseAJour->setStyleSheet(
     " QProgressBar {"
     " border-radius: 6px;"
     "border: 2px solid #d5a764;"
     "background-image: url(\"Data/Menus/Launcher/barre.png\");"
     "background-color: #eebb65;"
     "}"

     "QProgressBar::chunk {"
     "    background-image: url(\"Data/Menus/Launcher/barre2.png\");"
     "    width: 16px;"
     "}"
     );

    m_miseAJourTotale->setStyleSheet(
     " QProgressBar {"
     " border-radius: 6px;"
     "border: 2px solid #d5a764;"
     "background-image: url(\"Data/Menus/Launcher/barre.png\");"
     "background-color: #eebb65;"
     "}"

     "QProgressBar::chunk {"
     "    background-image: url(\"Data/Menus/Launcher/barre2.png\");"
     "    width: 16px;"
     "}"
     );

    std::ifstream fichier;
    fichier.open("liste_langages.conf", std::ios::in);
    if(fichier)
    {
        char caractere;
    	do
    	{
    	    //Chargement du nom
    		fichier.get(caractere);
    		if(caractere=='*')
    		{
    		    languages.push_back(std::string ());
    		    languages_txt.push_back(std::string ());
    			fichier>>languages.back()>>languages_txt.back();
    		}

    		if(fichier.eof()){ caractere='$'; }
    	}while(caractere!='$');
    }

    listeLangages = new QComboBox(this);
    listeLangages->move(64, 556);

    Configuration config;
    config.Charger();

    for(int i=0;i<(int)languages_txt.size();i++)
    {
        listeLangages->addItem(languages_txt[i].c_str());
        if(config.language == languages[i])
            listeLangages->setCurrentIndex(i);
    }


    QObject::connect(m_boutonQuitter, SIGNAL(clicked()), qApp, SLOT(quit()));

    QObject::connect(m_boutonMettreAJour, SIGNAL(clicked()), this, SLOT(telechargerFichier()));

    QObject::connect(m_boutonOptions, SIGNAL(clicked()), this, SLOT(Options()));

    QWidget::connect(m_boutonOptions, SIGNAL(clicked()), &optionsJeu, SLOT(exec()));


    QObject::connect(m_boutonDemarrer, SIGNAL(clicked()), qApp, SLOT(quit()));
    QObject::connect(m_boutonDemarrer, SIGNAL(clicked()), this, SLOT(LancerJeu()));

    QObject::connect(listeLangages, SIGNAL(currentIndexChanged (int)), this, SLOT(languageChange(int)));

    downloadFile();

}

 void Launcher::downloadAll(bool demarrer)
 {
     if(!demarrer)
     {
         fait=-1;
         aFaire=0;

        std::ifstream bdd_txt;
        bdd_txt.open("maj.conf", std::ios::in);

        if(bdd_txt)
        {
            char caractere;
            do
            {
                //Chargement du nom
                bdd_txt.get(caractere);
                if(caractere=='*')
                {
                    std::string nom,rep;
                    int ver;
                     bdd_txt>>nom>>ver;

                     nom_fichier.push_back(nom);
                     ver_fichier.push_back(ver);

                     final_nom.push_back(nom);
                     final_ver.push_back(ver);
                }

                if(bdd_txt.eof()){ caractere='$'; }
            }while(caractere!='$');
        }
        else
            m_installer = true;
        bdd_txt.close();



        std::ifstream bdd_sql;
        bdd_sql.open("maj.txt", std::ios::in);

        if(bdd_sql)
        {
            char caractere;
            do
            {
                //Chargement du nom
                bdd_sql.get(caractere);
                if(caractere=='*')
                {
                    std::string nom,rep;
                    int ver;
                     bdd_sql>>nom>>rep>>ver;

                     nom_fichier_nv.push_back(nom);
                     ver_fichier_nv.push_back(ver);
                     rep_fichier_nv.push_back(rep);

                }

                if(bdd_sql.eof()){ caractere='$'; }
            }while(caractere!='$');

        }
        bdd_sql.close();

        if(nom_fichier_nv.size()>0)
        {
             for(int i=0;i<(int)nom_fichier_nv.size();i++)
             {
                 bool telechargement=true;

                    for(int j=0;j<(int)nom_fichier.size();j++)
                    {
                        if(nom_fichier[j]==nom_fichier_nv[i])
                        {
                            if(ver_fichier[j]==ver_fichier_nv[i])
                                telechargement=false;
                        }

                    }

                if(telechargement)
                {
                    liste_a_telecharger_nom.push_back(nom_fichier_nv[i]);
                    liste_a_telecharger_rep.push_back(rep_fichier_nv[i]);

                    liste_a_telecharger_ver.push_back(ver_fichier_nv[i]);
                }
             }

             for(int i=0;i<(int)liste_a_telecharger_rep.size();i++)
             {
                 std::string temp;
                      std::string::size_type stTemp = liste_a_telecharger_rep[i].find('/');

                     while(stTemp !=  std::string::npos)
                     {
                         temp+=liste_a_telecharger_rep[i].substr(0, stTemp)+"/";
                         dir->mkdir(temp.c_str());
                         liste_a_telecharger_rep[i] = liste_a_telecharger_rep[i].substr(stTemp + 1);
                         stTemp = liste_a_telecharger_rep[i].find('/');
                     }
             }


            aFaire=(int)liste_a_telecharger_nom.size();

            enCoursDeTelechargement=-1;
            if(liste_a_telecharger_nom.size()>0)
            {
                m_boutonMettreAJour->setEnabled(true);
                m_boutonMettreAJour->setText(tr("Mise à jour disponible."));
            }
            else
            {
                m_boutonMettreAJour->setEnabled(false);
                m_boutonMettreAJour->setText(tr("Aucune mise à jour trouvée"));
            }

            if(m_installer)
            {
                m_boutonDemarrer->setEnabled(false);
                m_boutonMettreAJour->setEnabled(true);
                m_boutonMettreAJour->setText(tr("Télécharger et installer"));
            }
        }
        else
        {
            m_boutonMettreAJour-> setStyleSheet(
             "QPushButton {"
                " border-radius: 6px;"
                "background-image: url(\"Data/Menus/Launcher/Button1.png\"); "
                "font-weight: bold;"
                "color: #700e0e;"
                "font-family: Arial;"
                "font-size: 12px;"
            " }"

             "QPushButton:disabled {"
             "background-image: url(\"Data/Menus/Launcher/Button5.png\"); "
            " } "
             );

            m_boutonDemarrer->setEnabled(true);
            m_boutonMettreAJour->setText(tr("Maintenance du système.\n Veuillez rééssayer dans quelques minutes."));
            m_boutonMettreAJour->setEnabled(false);
        }
     }
     else
     {
        m_boutonDemarrer->setEnabled(true);
        m_boutonMettreAJour->setText(tr("Mise à jour impossible"));
        m_boutonMettreAJour->setEnabled(false);
     }
 }


 void Launcher::telechargerFichier(bool erreur)
 {
     m_boutonMettreAJour-> setStyleSheet(
             "QPushButton {"
                " border-radius: 6px;"
                "background-image: url(\"Data/Menus/Launcher/Button1.png\"); "
                "font-weight: bold;"
                "color: #700e0e;"
                "font-family: Arial;"
                "font-size: 12px;"
            " }"

             "QPushButton:disabled {"
             "background-image: url(\"Data/Menus/Launcher/Button5.png\"); "
            " } "
             );

     if(enCoursDeTelechargement>=0&&enCoursDeTelechargement<(int)liste_a_telecharger_nom.size())
     {
         bool nouveau=true;
        for(int i=0;i<(int)final_nom.size();i++)
            if(final_nom[i]==liste_a_telecharger_nom[enCoursDeTelechargement])
            {
                nouveau=false;
                final_ver[i]=liste_a_telecharger_ver[enCoursDeTelechargement];
            }

        if(nouveau)
        {
            final_nom.push_back(liste_a_telecharger_nom[enCoursDeTelechargement]);
            final_ver.push_back(liste_a_telecharger_ver[enCoursDeTelechargement]);
        }

         MettreAJourBDD();
     }


     m_boutonMettreAJour->setEnabled(false);
     m_boutonDemarrer->setEnabled(false);
     enCoursDeTelechargement++;

     delete http;
    delete fichier;
    http=new QHttp;
    fichier = new QFile;

     if(enCoursDeTelechargement<(int)liste_a_telecharger_nom.size())
     {
         if(erreur)
         {
            if(http->error()==7)
                QMessageBox::information(0, tr("Erreur"), tr("Mise à jour intérrompue."));
            else
                QMessageBox::information(0, tr("Erreur"), tr("Mise à jour impossible."));
         }
         else
         {
             fait++;
             fait2=fait;
             miseAJourBarreTotale(fait2,aFaire);

             m_boutonMettreAJour->setText(liste_a_telecharger_nom[enCoursDeTelechargement].c_str());

            std::string chemin = "http://holyspirit.alpha-arts.net/Fichiers_jeu/Holyspirit/"+liste_a_telecharger_nom[enCoursDeTelechargement];

            QUrl url(chemin.c_str());

            fichier -> setFileName(liste_a_telecharger_nom[enCoursDeTelechargement].c_str());
            fichier -> open(QIODevice::WriteOnly);

            QHttp::ConnectionMode mode = url.scheme().toLower() == "https" ? QHttp::ConnectionModeHttps : QHttp::ConnectionModeHttp;
            http->setHost(url.host(), mode, url.port() == -1 ? 0 : url.port());
            QByteArray path = QUrl::toPercentEncoding(url.path(), "!$&'()*+,;=:@/");
            httpGetId = http->get(path, fichier);

            QObject::connect(http, SIGNAL( done (bool)), this, SLOT(telechargerFichier(bool)));
            QObject::connect(http, SIGNAL( dataReadProgress(int,int)), this, SLOT(miseAJourBarre(int,int)));



         }
     }
     else
     {
         fait ++;
         fait2=fait;
         miseAJourBarreTotale(fait2,aFaire);
         MettreAJourBDD();

        m_boutonDemarrer->setText(tr("Lancer le jeu !"));
        if(liste_a_telecharger_nom.size()>0)
        {
            m_boutonDemarrer->setEnabled(true);
            m_boutonMettreAJour->setText(tr("Mise à jour terminée."));

            m_boutonMettreAJour-> setStyleSheet(
              "QPushButton {"
                " border-radius: 6px;"
                "background-image: url(\"Data/Menus/Launcher/Button1.png\"); "
                "font-size: 14px;"
                "font-weight: bold;"
                "color: #700e0e;"
                "font-family: Garamond;"
            " }"

            " QPushButton:hover {"
                "background-image: url(\"Data/Menus/Launcher/Button2.png\"); "
                "color: #235824;"
            " }"

             "QPushButton:disabled {"
             "background-image: url(\"Data/Menus/Launcher/Button5.png\"); "
             "color: #700e0e;"
            " } "
             );
        }
        else
            m_boutonMettreAJour->setText(tr("Aucune mise à jour."));
     }

 }

 void Launcher::MettreAJourBDD()
 {
    std::fstream bdd_txt_maj("maj.conf", std::ios::in | std::ios::out | std::ios::trunc);
    if(bdd_txt_maj)
    {
        for(int i=0;i<(int)final_nom.size();i++)
        {
            bdd_txt_maj<<"* "<<final_nom[i]<<" "<<final_ver[i]<<std::endl;
        }
        bdd_txt_maj<<"$";
        bdd_txt_maj.close();
    }
 }

 void Launcher::downloadFile()
 {
     fichier = new QFile;
      http = new QHttp;

      m_miseAJour->show();
      m_miseAJourTotale->show();


     fichier -> setFileName("maj.txt");

        if(fichier -> open(QIODevice::WriteOnly))
        {
            telechargementFini=false;
             m_boutonMettreAJour->setText(tr("Téléchargement de la base de données..."));

            QUrl url("http://holyspirit.alpha-arts.net/Fichiers_jeu/maj.txt");
            QHttp::ConnectionMode mode = url.scheme().toLower() == "https" ? QHttp::ConnectionModeHttps : QHttp::ConnectionModeHttp;
            http->setHost(url.host(), mode, url.port() == -1 ? 0 : url.port());
            QByteArray path = QUrl::toPercentEncoding(url.path(), "!$&'()*+,;=:@/");
            httpGetId = http->get(path, fichier);

            QObject::connect(http, SIGNAL( dataReadProgress(int,int)), this, SLOT(miseAJourBarre2(int,int)));
            QObject::connect(http, SIGNAL( done(bool)), this, SLOT(downloadAll(bool)));
        }



 }

void Launcher::miseAJourBarre(int progression, int total)
{
    m_miseAJour->setValue(progression*100/total);
    fait2=(float)fait+(float)progression/(float)total;
    miseAJourBarreTotale(fait2,aFaire);
}

void Launcher::miseAJourBarreTotale(float progression, int total)
{
    float temp=(float)progression*100/(float)total;
    m_miseAJourTotale->setValue((int)(temp));
}

void Launcher::miseAJourBarre2(int progression, int total)
{

    m_miseAJour->setValue(progression*100/total);

     if(progression*100/total>=100)
         fichier->close();
 }


 void Launcher::LancerJeu()
 {
     wchar_t *temp;
     temp= new wchar_t[15];

     temp[0]='H';
     temp[1]='o';
     temp[2]='l';
     temp[3]='y';
     temp[4]='s';
     temp[5]='p';
     temp[6]='i';
     temp[7]='r';
     temp[8]='i';
     temp[9]='t';
     temp[10]='.';
     temp[11]='e';
     temp[12]='x';
     temp[13]='e';

     temp[14]='\0';

     this->hide();
    ShellExecute(NULL,NULL,temp,NULL,NULL,SW_SHOWNORMAL);

 }

 void Launcher::Options()
 {

 }

  void Launcher::languageChange(int index)
  {
        Configuration config;
        config.Charger();

        config.language = languages[index];

        config.Sauvegarder();
  }

