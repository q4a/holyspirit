#include <SFML/System.hpp>
#ifndef CHARGTHREAD
#define CHARGTHREAD
#include "MoteurGraphique.h"
class ChargThread : private sf::Thread {
    public :
    ChargThread (MoteurGraphique *g);

    void charger () {
        Launch ();
    }
    private :
    MoteurGraphique *motGraph;
    virtual void run ();
};
#endif
