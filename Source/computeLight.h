#ifndef COMPUTE_LIGHT
#define COMPUTE_LIGHT
#include "Map.h"
class ComputeLight : private sf::Thread {
    private :
        Map map;
        virtual void run ();
    public :
        ComputeLight (Map &map);
        void compute () {
            Launch ();
        }
 };
 #endif

