#include "WindowGame.hpp"
#include <iostream>

using namespace std;
using namespace sf;

WindowGame::WindowGame()
{
    m_window.Create(VideoMode(800, 600, 32), "LOS", Style::Titlebar);
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
        if(mainEventManager->GetEvent(EventKey, sf::Key::A))
            cout<<"Position X : "<<mainEventManager->GetMousePosition().x<<"Position Y : "<<mainEventManager->GetMousePosition().y<<endl;

        m_window.Clear();

        m_button.Show(m_window);

        m_window.Display();
    }
}
