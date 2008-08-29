#include <QApplication>
#include <QPushButton>
#include <QtGui>

#include <QProgressBar>
//#include <QDialog>
//#include "C:\Qt\4.4.1\include\QtNetwork\qhttp.h"


#include "Launcher.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    Launcher launcher;

    // Affichage de la fenêtre
    launcher.show();

    return app.exec();
}

