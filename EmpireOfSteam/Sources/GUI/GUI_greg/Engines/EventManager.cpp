#include "EventManager.hpp"

using namespace sf;

EventManager::EventManager()
{
    m_closed = false;
    m_wheel = 0;

    int i = 0;
    while(i < 500) m_key_events_tab[i++] = false;

    i = 0;
    while(i < 5) m_mouse_events_tab[i++] = false;

    m_time  = 0;
    m_clock.Reset();
}

EventManager::~EventManager()
{

}

void EventManager::Update(sf::RenderWindow &window)
{
    m_time = m_clock.GetElapsedTime();
    m_clock.Reset();

    while (window.GetEvent(m_event))
    {
        switch (m_event.Type)
        {
            case Event::KeyPressed:
                if (m_event.Key.Code>=0&&m_event.Key.Code<500)
                    m_key_events_tab[m_event.Key.Code] = 1;
                break;
            case Event::KeyReleased:
                if (m_event.Key.Code>=0&&m_event.Key.Code<500)
                    m_key_events_tab[m_event.Key.Code] = 0;
                break;
            case Event::MouseMoved:
                m_mouse_pos.x   = m_event.MouseMove.X;
                m_mouse_pos.y   = m_event.MouseMove.Y;
                break;
            case Event::MouseButtonPressed:
                m_mouse_events_tab[m_event.MouseButton.Button] = 1;
                break;
            case Event::MouseButtonReleased:
                m_mouse_events_tab[m_event.MouseButton.Button] = 0;
                break;
            case Event::MouseWheelMoved:
                m_wheel  = m_event.MouseWheel.Delta;
                break;
            case Event::Closed:
                m_closed = true;
                break;

            default:
                break;
        }
    }
}

bool EventManager::GetEvent(int type_event, int no_event)
{
    if(type_event == EventMouse)
    {
        if(no_event >= 0 && no_event < 5)
            return m_mouse_events_tab[no_event];
    }
    else if(type_event == EventKey)
    {
        if(no_event >= 0 && no_event < 500)
            return m_key_events_tab[no_event];
    }

    return false;
}

float EventManager::GetTime()
{
    return m_time;
}

const sf::Vector2i &EventManager::GetMousePosition()
{
    return m_mouse_pos;
}
