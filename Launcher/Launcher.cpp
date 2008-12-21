#include "Launcher.h"

//using namespace boost::filesystem;

Launcher::Launcher() : QWidget()
{
    enCoursDeTelechargement=-2;

    QDesktopWidget bureau;

    setFixedSize(384, 512);
    setGeometry ( bureau.screenGeometry().width()/2-192,  bureau.screenGeometry().height()/2-256, 384, 512 );

    m_fond = new QLabel(this);
    m_pixmap = new QPixmap("Data/Menus/Launcher/Fond.png");
    m_fond->setPixmap(*m_pixmap);
    this->setMask(m_pixmap->mask());

    m_boutonDemarrer = new QPushButton("Lancer le jeu !", this);
    m_boutonDemarrer->setGeometry(64,128,256,32);

    // Création du bouton, ayant pour parent la "fenetre"


    m_boutonQuitter = new QPushButton("Quitter", this);
    m_boutonQuitter->setGeometry(128,384,128,32);

    m_boutonMettreAJour = new QPushButton("Mettre le jeu à jour", this);
    m_boutonMettreAJour->setGeometry(64,192,256,32);

    m_boutonOptions = new QPushButton("Options", this);
    m_boutonOptions->setGeometry(64,320,256,32);

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
    m_miseAJour->setGeometry(64,256,256,16);

    m_miseAJourTotale = new QProgressBar(this);
    m_miseAJourTotale->setValue(0);
    m_miseAJourTotale->hide();
    m_miseAJourTotale->setTextVisible(0);
    m_miseAJourTotale->setGeometry(64,280,256,16);





    QObject::connect(m_boutonQuitter, SIGNAL(clicked()), qApp, SLOT(quit()));

    QObject::connect(m_boutonMettreAJour, SIGNAL(clicked()), this, SLOT(telechargerFichier()));

    QObject::connect(m_boutonOptions, SIGNAL(clicked()), this, SLOT(Options()));

    QWidget::connect(m_boutonOptions, SIGNAL(clicked()), &optionsJeu, SLOT(exec()));


    QObject::connect(m_boutonDemarrer, SIGNAL(clicked()), qApp, SLOT(quit()));
    QObject::connect(m_boutonDemarrer, SIGNAL(clicked()), this, SLOT(LancerJeu()));

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
                }

                if(bdd_txt.eof()){ caractere='$'; }
            }while(caractere!='$');

        }
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
        m_boutonDemarrer->setEnabled(true);
        if(liste_a_telecharger_nom.size()>0)
        {
            m_boutonMettreAJour->setEnabled(true);
            m_boutonMettreAJour->setText("Une nouvelle mise à jour est disponible.");
        }
        else
            m_boutonMettreAJour->setText("Aucune mise à jour trouvée");

        if(m_installer)
        {
            m_boutonDemarrer->setEnabled(false);
            m_boutonMettreAJour->setText("Télécharger et installer le jeu");
        }


       // telechargerFichier(0);
     }
     else
     {
        //QMessageBox::information(0, "Erreur", "Mise à jour impossible.");
        m_boutonDemarrer->setEnabled(true);
        m_boutonMettreAJour->setText("Mise à jour impossible");
        //m_boutonMettreAJour->setText("Mise à jour impossible");
     }



   /* for(int i=0;i<(int)liste_a_telecharger_rep.size();i++)
    {
        #ifdef WINDOWS
            CreateDirectory(liste_a_telecharger_rep[i].c_str());
        #endif
        //std::string cmd="mkdir "+listeRepertoire[i];
        //system(cmd.c_str());
    }*/
 }


 void Launcher::telechargerFichier(bool erreur)
 {
     m_boutonMettreAJour->setEnabled(false);
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
                QMessageBox::information(0, "Erreur", "Mise à jour intérrompue.");
            else
                QMessageBox::information(0, "Erreur", "Mise à jour impossible.");
         }
         else
         {
             fait++;
             fait2=fait;
             miseAJourBarreTotale(fait2,aFaire);


            // http->abort();
            //http->clearPendingRequests();
            //fichier->close();

           // create_directory( liste_a_telecharger_nom[enCoursDeTelechargement] );



            /*std::string commande="mkdir "+liste_a_telecharger_rep[enCoursDeTelechargement];
            system(commande.c_str());

            system("mkdir Test");*/

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
         std::fstream bdd_txt_maj("maj.conf", std::ios::in | std::ios::out | std::ios::trunc);
         if(bdd_txt_maj)
         {
                for(int i=0;i<(int)nom_fichier_nv.size();i++)
                {
                    bdd_txt_maj<<"* "<<nom_fichier_nv[i]<<" "<<ver_fichier_nv[i]<<std::endl;
                }
                bdd_txt_maj<<"$";
         }

        m_boutonDemarrer->setText("Lancer le jeu !");
        if(liste_a_telecharger_nom.size()>0)
            m_boutonMettreAJour->setText("Mise à jour terminée.");
        else
            m_boutonMettreAJour->setText("Aucune mise à jour.");
        m_boutonDemarrer->setEnabled(true);


        //delete http;
        //delete fichier;
     }

 }

 void Launcher::downloadFile()
 {

      m_boutonDemarrer->setEnabled(false);
     m_boutonMettreAJour->setEnabled(false);

     fichier = new QFile;
      http = new QHttp;

      m_miseAJour->show();
      m_miseAJourTotale->show();


     fichier -> setFileName("maj.txt");

        if(fichier -> open(QIODevice::WriteOnly))
        {
            telechargementFini=false;
             m_boutonMettreAJour->setText("Téléchargement de la base de données...");

            QUrl url("http://holyspirit.alpha-arts.net/Fichiers_jeu/maj.txt");
            QHttp::ConnectionMode mode = url.scheme().toLower() == "https" ? QHttp::ConnectionModeHttps : QHttp::ConnectionModeHttp;
            http->setHost(url.host(), mode, url.port() == -1 ? 0 : url.port());
            QByteArray path = QUrl::toPercentEncoding(url.path(), "!$&'()*+,;=:@/");
            httpGetId = http->get(path, fichier);

            QObject::connect(http, SIGNAL( dataReadProgress(int,int)), this, SLOT(miseAJourBarre2(int,int)));
            QObject::connect(http, SIGNAL( done(bool)), this, SLOT(downloadAll(bool)));

            //while(m_boutonMettreAJour->text()!="Fini"){}
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

     if(progression*100/total<100)
     {

     }
     else
     {
         fichier->close();

     }
 }


 void Launcher::LancerJeu()
 {
     this->hide();
     system("Holyspirit.exe");
 }

 void Launcher::Options()
 {

 }

