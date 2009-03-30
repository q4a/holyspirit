#ifndef GEN_LIGHT
#define GEN_LIGHT
#include "light.h"

class  GenerateLight : private sf::Thread {
    private :
        std::vector<Light> *light;
        std::vector<Wall> *wall;
        Light_Entity *e;
        virtual void Run ();

    public :
        GenerateLight (std::vector<Light> *light, std::vector<Wall> *wall);

        void compute (Light_Entity *e) {
            (*this).e = e;
            Launch ();
        }
        void stop () {
            Wait ();
        }

};
#endif
