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

    m_time[0]  = 0;
    m_time[1]  = 0;
    m_time[2]  = 0;
    m_time[3]  = 0;
    m_time[4]  = 0;

    m_clock.Reset();

    m_show_cursor = true;

    m_char = -1;

    m_focus = NULL;
}

EventManager::~EventManager()
{

}

void EventManager::Update(sf::RenderWindow &window)
{
    m_time[4] = m_time[3];
    m_time[3] = m_time[2];
    m_time[2] = m_time[1];
    m_time[1] = m_time[0];
    m_time[0] = m_clock.GetElapsedTime();
    m_clock.Reset();

    while (window.GetEvent(m_event))
    {
        switch (m_event.Type)
        {
            case Event::KeyPressed:
                if (m_event.Key.Code>=0&&m_event.Key.Code<500)
                    m_key_events_tab[m_event.Key.Code] = 1;

                if(m_event.Key.Code >= 0 && m_event.Key.Code < 128)
                    m_char = m_event.Key.Code;

                break;
            case Event::KeyReleased:
                if (m_event.Key.Code>=0&&m_event.Key.Code<500)
                    m_key_events_tab[m_event.Key.Code] = 0;

                if(m_char == m_event.Key.Code)
                    m_char = -1;

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

char EventManager::GetChar()
{
    if(m_key_events_tab[sf::Key::LShift])
        return (m_char - 32);
    else
        return (m_char);
}

float EventManager::GetTime()
{
    return (m_time[0] + m_time[1] + m_time[2] + m_time[3] + m_time[4])/5;
}

const sf::Vector2i &EventManager::GetMousePosition()
{
    return m_mouse_pos;
}

void EventManager::ShowCursor(bool show)
{
    m_show_cursor = show;
}
bool EventManager::GetShowCursor()
{
    return (m_show_cursor);
}

Widget *EventManager::GetFocus()
{
    return (m_focus);
}

void EventManager::SetFocus(Widget *focus)
{
    m_focus = focus;
}
