#ifndef EVENTMANAGER_HPP
#define EVENTMANAGER_HPP

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include "../Utils/Singleton.hpp"

class Widget;
enum {EventMouse, EventKey};

class EventManager : public CSingleton<EventManager>
{
    public:
        friend  EventManager*   CSingleton<EventManager>::GetInstance();
        friend  void            CSingleton<EventManager>::Kill();

                                EventManager();
                               ~EventManager();

                void            Update(sf::RenderWindow &window);

                bool            GetEvent(int, int);
        const   sf::Vector2i   &GetMousePosition();

                char            GetChar();

                float           GetTime();

                void            ShowCursor(bool);
                bool            GetShowCursor();

                Widget         *GetFocus();

                void            SetFocus(Widget *);

    protected:

    private:
        sf::Event       m_event;

        bool            m_key_events_tab[500];
        bool            m_mouse_events_tab[5];
        bool            m_closed;

        char            m_char;

        int             m_wheel;
        sf::Vector2i    m_mouse_pos;

        float           m_time[5];

        sf::Clock       m_clock;

        bool            m_show_cursor;

        Widget         *m_focus;

};

#endif
