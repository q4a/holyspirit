#include "generateLight.h"
GenerateLight::GenerateLight (std::vector<Light> *light, std::vector<Wall> *wall) {
    (*this).light = light;
    (*this).wall = wall;
}

void GenerateLight::Run () {

    (*light)[(*e).ID()].Generate(*wall);
}

