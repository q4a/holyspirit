#include <iostream>
#include "WindowGame.hpp"

using namespace std;

EventManager *mainEventManager;

int main()
{
    mainEventManager = EventManager::GetInstance();

    WindowGame game;

    game.Run();

    EventManager::Kill();
    return 0;
}
