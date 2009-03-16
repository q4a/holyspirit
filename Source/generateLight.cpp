#include "generateLight.h"
GenerateLight::GenerateLight (Light &light, std::vector<Wall> &wall) {
    (*this).wall = wall;
    (*this).light = light;
}
void GenerateLight::run () {
    light.Generate(wall);
}
