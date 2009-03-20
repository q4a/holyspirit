#ifndef GEN_LIGHT
#define GEN_LIGHT
#include "Light.h"

class  GenerateLight : private sf::Thread {
    private :
        std::vector<Light> light;
        std::vector<Wall> wall;

        virtual void run ();

    public :
        GenerateLight (std::vector<Light> &light, std::vector<Wall> &wall);

        void compute () {
            Launch ();
        }
        void stop () {
            Wait ();
        }

};
#endif
