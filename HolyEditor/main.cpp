
////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include "MainWindow.h"
#include "MainWindow.moc"
#include <QApplication>
#include <QVBoxLayout>
#include <QFrame>
#include <QLabel>

#include "Moteurs/moteurGraphique.h"
#include "Moteurs/moteurSons.h"
#include "Moteurs/eventManager.h"
#include "configuration.h"

Configuration *configuration;
Console *console;
MoteurGraphique *moteurGraphique;
MoteurSons *moteurSons;
EventManager *eventManager;

#include "Map/map.h"

Map *map;

////////////////////////////////////////////////////////////
/// Entry point of application
///
/// \return Application exit code
///
////////////////////////////////////////////////////////////
int main(int argc, char **argv)
{

   // moteurGraphique->CreateNewWindow();

    map  = NULL;

    QApplication application(argc, argv);

    configuration=Configuration::GetInstance();
    console=Console::GetInstance();

    console->Ajouter("--------------------------------------------------------------------------------");
    console->Ajouter("Demarrage du jeu",0);
    console->Ajouter("--------------------------------------------------------------------------------");
    console->Ajouter("");

    console->Ajouter("Initialisation du moteur graphique");
    moteurGraphique = MoteurGraphique::GetInstance();

     eventManager=EventManager::GetInstance();

    MainWindow mainWindow;
    mainWindow.showMaximized();

    console->Ajouter("Initialisation du moteur sonore");
    moteurSons = MoteurSons::GetInstance();

    console->Ajouter("");

    srand(time(NULL));

    configuration->Charger();


   // if (!sf::PostFX::CanUsePostFX())
       // configuration->postFX = false;

    moteurGraphique->Charger();

    return application.exec();
    //return true;
}
