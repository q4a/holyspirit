#include "OptionPanel.hpp"

OptionPanel::OptionPanel()
{
    m_background    = new GUIImage("pictures/GUI/optionBackground.png");
    m_menu          = new GUIImage(32,64,"pictures/GUI/menuOption.png");
    m_screen        = new GUIImage(306,128,"pictures/GUI/optionScreen.png");
    AddWidget(m_background);
    AddWidget(m_menu);
    AddWidget(m_screen);
}

OptionPanel::~OptionPanel()
{
    delete m_background;
    delete m_menu;
    delete m_screen;
}

void OptionPanel::Update()
{
    Widget::Update();

}



