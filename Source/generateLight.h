#ifndef GEN_LIGHT
#define GEN_LIGHT
#include "Light.h"
class GenerateLight : private sf::Thread {
    private :
        std::vector<Wall> wall;
        Light light;
        virtual void run ();
    public :
        GenerateLight (Light &light, std::vector<Wall> &wall);
        void compute () {
            Launch ();
        }
};
#endif
