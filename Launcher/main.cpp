#include <QApplication>
#include <QPushButton>
#include <QtGui>

#include <QProgressBar>


#include "Launcher.h"

int main(int argc, char *argv[])
{
    Configuration config;
    config.Charger();

    QApplication app(argc, argv);

    QString locale = QLocale::system().name().section('_', 0, 0);

    QTranslator translator;

    if(config.language == "any")
        config.language = locale.toStdString();

    config.Sauvegarder();

    std::string buf  = "hslauncher_"+config.language;
    QString temp(buf.c_str());
    translator.load(temp);
    app.installTranslator(&translator);

    Launcher launcher;

    launcher.no_language = config.language;

    // Affichage de la fenêtre
    launcher.show();

    return app.exec();
}

