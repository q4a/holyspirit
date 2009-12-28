#ifndef GLOBAL_HPP
#define GLOBAL_HPP

#include <SFML/Graphics.hpp>
#include "Singleton.hpp"
#include "EventManager.hpp"

extern EventManager *mainEventManager;

typedef struct Coordonees
{
    int x;
    int y;
};

#endif
