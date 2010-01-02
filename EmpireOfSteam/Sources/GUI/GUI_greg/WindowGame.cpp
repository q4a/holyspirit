#include "WindowGame.hpp"
#include <iostream>

using namespace std;
using namespace sf;

WindowGame::WindowGame() : GUIWindow(800,600,"LoS")
{
    m_button        = new Button(50,50,200,100);
    m_button_in     = new Button(25,50,100,25);
    m_button_in_in  = new Button(25,5,25,10);

    m_button_2      = new Button(400,300,200,65);
    m_button_3      = new Button(200,300,50,65);

    m_label         = new Label(100,25,"Test");
    m_label->SetColor(sf::Color(255, 0, 0));
    m_label->SetCharacterSize(18);

    m_button_2->SetImage("pictures/GUI/button1.png", Button_released);
    m_button_2->SetImage("pictures/GUI/button2.png", Button_hover);
    m_button_2->SetImage("pictures/GUI/button3.png", Button_clicked);

    m_button_2->AddWidget(m_label);

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

    delete m_label;
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


