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

    m_button_2      = new Button(400,300,50,65);
    m_button_3      = new Button(200,300,50,65);

    AddWidget(m_button_2);
    AddWidget(m_button_3);
    AddWidget(m_button);
    m_button->AddWidget(m_button_in);
    m_button_in->AddWidget(m_button_in_in);

    m_button->SetPosition(200,100);
}

WindowGame::~WindowGame()
{
    delete m_button_in_in;
    delete m_button_in;
    delete m_button;
    delete m_button_2;
    delete m_button_3;
}

void WindowGame::AddWidget(Widget *widget)
{
    m_widgets.push_back(widget);
    //m_widgets.back()->SetGlobalPosition(m_widgets.back()->GetPosition().x, m_widgets.back()->GetPosition().y);
}

void WindowGame::Update()
{
    for( std::vector<Widget*>::iterator i = m_widgets.begin();
         i != m_widgets.end();
         ++i )
         {
            // (*i)->SetGlobalPosition((*i)->GetPosition().x, (*i)->GetPosition().y);
             (*i)->Update();
         }

}

void WindowGame::Run()
{
    while(m_window.IsOpened())
    {
        mainEventManager->Update(m_window);
        Update();

        if(mainEventManager->GetEvent(EventKey, sf::Key::Escape))
            m_window.Close();
        if(mainEventManager->GetEvent(EventKey, sf::Key::A))
            cout<<"Position X : "<<mainEventManager->GetMousePosition().x<<"Position Y : "<<mainEventManager->GetMousePosition().y<<endl;
        if(m_button_in_in->Clicked())
            m_window.Close();

        if(m_button_2->Released())
            m_button->SetPosition(m_button->GetPosition().x + 25 , m_button->GetPosition().y + 25);

        if(m_button_3->Hover())
            m_button->SetPosition(m_button->GetPosition().x - 1, m_button->GetPosition().y - 1);

        if(m_button->Released())
            m_button_in->SetPosition(m_button_in->GetPosition().x + 2 , m_button_in->GetPosition().y + 1);

        if(m_button_in->Released())
            m_button_in_in->SetPosition(m_button_in_in->GetPosition().x + 5 , m_button_in_in->GetPosition().y);



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

