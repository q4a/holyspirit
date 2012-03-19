#include "globale.h"

void MySprite::resize(float x, float y)
{
    setScale(x/(float)getLocalBounds().width  * ((getScale().x > 0) ? 1.f : -1.f),
             y/(float)getLocalBounds().height * ((getScale().y > 0) ? 1.f : -1.f));
}
