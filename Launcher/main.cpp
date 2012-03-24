#include <QApplication>
#include <QPushButton>
#include <QtGui>

#include <QProgressBar>

#include "Launcher.h"

int main(int argc, char *argv[]) {

    QApplication app(argc, argv);


    Configuration config;
    config.Charger();


    QString locale = QLocale::system().name().section('_', 0, 0);

    QTranslator translator;

    if(config.language == "any")
        config.language = locale.toStdString();

    config.Sauvegarder();

    translator.load( QString("Languages/hslauncher_%1").arg(config.language.c_str()));
    app.installTranslator(&translator);

    Launcher launcher;

    launcher.no_language = config.language;

    // Affichage de la fenêtre
    launcher.show();

    return app.exec();;
}

