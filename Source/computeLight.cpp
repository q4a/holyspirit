#include "computeLight.h"
#include "Map.h"
ComputeLight::ComputeLight (Map &map) {
   (*this).map = map;
}
void ComputeLight::run () {
    m.animate ();
}
