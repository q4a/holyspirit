#include "WindowGame.hpp"
#include <iostream>

using namespace std;
using namespace sf;

WindowGame::WindowGame()
{
    m_window.Create(VideoMode(800, 600, 32), "LOS", Style::Titlebar);

    m_button        = new Button(50,50,200,100);
    m_button_in     = new Button(25,50,100,25);
    m_button_in_in  = new Button(25,5,25,10);

    AddWidget(m_button);
    m_button->AddWidget(m_button_in);
    m_button_in->AddWidget(m_button_in_in);

    m_button->SetPosition(100,100);
}

WindowGame::~WindowGame()
{
    delete m_button_in_in;
    delete m_button_in;
    delete m_button;
}

void WindowGame::AddWidget(Widget *widget)
{
    m_widgets.push_back(widget);
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
        if(m_button_in_in->Clicked())
            m_window.Close();

        Show();
    }
}

void WindowGame::Show()
{
    m_window.Clear();

    for( std::vector<Widget*>::iterator i = m_widgets.begin();
         i != m_widgets.end();
         ++i )
            m_window.Draw((*i)->Show());

    m_window.Display();
}

