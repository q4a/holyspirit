#include "EventManager.hpp"

using namespace sf;

EventManager::EventManager()
{

}

EventManager::~EventManager()
{

}

Event& EventManager::GetEvent()
{
    return m_event;
}
