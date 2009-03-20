#include "generateLight.h"
GenerateLight::GenerateLight (std::vector<Light> &light, std::vector<Wall> &wall) {
    (*this).light = light;
    (*this).wall = wall;
}

void GenerateLight::run () {
    for(int i=0;i<(int)light.size();i++) {
       light[i].Generate(wall);
    }
}
