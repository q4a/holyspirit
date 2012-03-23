#include "globale.h"

#include <SFML/System/Utf.hpp>

void MySprite::resize(float x, float y)
{
    setScale(x/(float)getLocalBounds().width  * ((getScale().x > 0) ? 1.f : -1.f),
             y/(float)getLocalBounds().height * ((getScale().y > 0) ? 1.f : -1.f));
}

std::basic_string<sf::Uint32> toUtf32(std::string line)
{
std::basic_string<sf::Uint32> utf32line;
	sf::Utf8::toUtf32((line).begin(),(line).end(), back_inserter(utf32line));
    return utf32line;
}

