#include <SFML/System.hpp>
#include "ChargThread.h"


using namespace sf;
ChargThread::ChargThread (MoteurGraphique *g){
    motGraph = g;
}
void ChargThread::run () {
     motGraph->Charger();
}
