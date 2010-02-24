#include "WindowGame.hpp"
#include <iostream>

using namespace std;
using namespace sf;

WindowGame::WindowGame() : GUIWindow(1024,768,"Empire of Steam")
{
    m_screen = MENU;

    AddWidget(&m_game);
    AddWidget(&m_menu);
    AddWidget(&m_option);
    AddWidget(&m_customisation);

    m_menu.SetPosition(0,0);
    m_game.SetPosition(1024,0);
    m_option.SetPosition(0,768);
    m_customisation.SetPosition(1024,768);

    m_view.x = 0;
    m_view.y = 0;
}

WindowGame::~WindowGame()
{

}

void WindowGame::Run()
{
    while(m_window.IsOpened())
    {
        mainEventManager->Update(m_window);
        Update();

        m_window.ShowMouseCursor(mainEventManager->GetShowCursor());

        if(mainEventManager->GetEvent(EventKey, sf::Key::Escape))
            m_window.Close();

        if(mainEventManager->GetFocus() == NULL)
            mainEventManager->SetFocus(this);

        if(mainEventManager->GetFocus() == this)
        {
            if(mainEventManager->GetEvent(EventKey, sf::Key::Right))
            {
                if(m_screen == GAME || m_screen == MENU)
                    m_screen = GAME;
                else
                    m_screen = CUSTOMISATION;
            }
            if(mainEventManager->GetEvent(EventKey, sf::Key::Left))
            {
                if(m_screen == GAME || m_screen == MENU)
                    m_screen = MENU;
                else
                    m_screen = OPTION;
            }

            if(mainEventManager->GetEvent(EventKey, sf::Key::Down) && m_screen == MENU)
                m_screen = OPTION;
            if(mainEventManager->GetEvent(EventKey, sf::Key::Up) && m_screen == OPTION)
                m_screen = MENU;
        }


        if((m_screen == GAME || m_screen == CUSTOMISATION) && m_view.x < 1024)
        {
            m_view.x += mainEventManager->GetTime() * 1000;

            if(m_view.x > 1024)
                m_view.x = 1024;
        }

        if((m_screen == MENU || m_screen == OPTION) && m_view.x > 0)
        {
            m_view.x -= mainEventManager->GetTime() * 1000;

            if(m_view.x < 0)
                m_view.x = 0;
        }

        if((m_screen == OPTION || m_screen == CUSTOMISATION) && m_view.y < 768)
        {
            m_view.y += mainEventManager->GetTime() * 1000;

            if(m_view.y > 768)
                m_view.y = 768;
        }

        if((m_screen == MENU || m_screen == GAME) && m_view.y > 0)
        {
            m_view.y -= mainEventManager->GetTime() * 1000;

            if(m_view.y < 0)
                m_view.y = 0;
        }

        m_menu          .SetPosition((int)(     - m_view.x), (int)(    - m_view.y));
        m_game          .SetPosition((int)(1024 - m_view.x), (int)(    - m_view.y));
        m_option        .SetPosition((int)(     - m_view.x), (int)(768 - m_view.y));
        m_customisation .SetPosition((int)(1024 - m_view.x), (int)(768 - m_view.y));

        Show();
    }
}


