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

Coordonnees EventManager::GetMousePosition()
{
    return Coordonnees(m_event.MouseMove.X, m_event.MouseMove.Y);
}
