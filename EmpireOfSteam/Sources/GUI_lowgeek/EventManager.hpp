#ifndef EVENTMANAGER_HPP
#define EVENTMANAGER_HPP

#include <SFML/Graphics.hpp>
#include "Singleton.hpp"

class EventManager : public CSingleton<EventManager>
{
    public:
        friend EventManager* CSingleton<EventManager>::GetInstance();
        friend void CSingleton<EventManager>::Kill();
        EventManager();
        ~EventManager();
        sf::Event& GetEvent();

    protected:

    private:
        sf::Event m_event;
};

#endif
