#include "MenuPanel.hpp"

MenuPanel::MenuPanel()
{
    m_background    = new GUIImage("pictures/GUI/menuBackground.png");
    m_deco          = new GUIImage(96,80,"pictures/GUI/coeur.png");
    m_login         = new GUIImage(448,512,"pictures/GUI/login.png");
    m_iris          = new GUIImage("pictures/GUI/irisClosed.png");
    m_iris_open     = new GUIImage("pictures/GUI/irisOpen.png");

    m_iris_button   = new Button(280,64,450,439);
    m_iris_button->SetImage(m_iris);

    m_login_input   = new GUIInput(656,528,256,32, "Pseudo");
    m_login_input->m_label.SetCharacterSize(24);
    m_login_input->SetMax(16);

    m_pass_input    = new GUIInput(656,576,256,32, "Motdepasse");
    m_pass_input->m_label.SetCharacterSize(24);
    m_pass_input->SetMax(16);

    AddWidget(m_background);
    AddWidget(m_deco);
    AddWidget(m_login);

    AddWidget(m_iris_button);

    AddWidget(m_login_input);
    AddWidget(m_pass_input);
}

MenuPanel::~MenuPanel()
{
    delete m_background;
    delete m_deco;
    delete m_login;
    delete m_iris;
    delete m_iris_open;

    delete m_iris_button;

    delete m_login_input;
    delete m_pass_input;
}

void MenuPanel::Update()
{
    if(m_iris_button->Released())
        m_iris_button->SetImage(m_iris_open, Button_released);

    Widget::Update();
}


