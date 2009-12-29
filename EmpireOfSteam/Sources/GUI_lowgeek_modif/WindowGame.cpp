#include "WindowGame.hpp"

using namespace sf;

WindowGame::WindowGame()
{
    m_window.Create(VideoMode(800, 600, 32), "LOS", sf::Style::Titlebar);
}

WindowGame::~WindowGame()
{

}

void WindowGame::Run()
{
    while(m_window.IsOpened())
    {
        mainEventManager->Update(m_window);

        if(mainEventManager->GetEvent(EventKey, sf::Key::Escape))
            m_window.Close();



        m_window.Clear();

        m_window.Display();
    }
}
