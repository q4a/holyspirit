#include "GUI_Button.h"

GUI_Button::GUI_Button()
{
    m_is_clicked = false;
    //ctor
}

GUI_Button::~GUI_Button()
{
    //dtor
}

void GUI_Button::Update()
{
    m_is_clicked = false;
    if(Event_manager -> getEvent(Event::Mouse, sf::Mouse::Left))
        if(Collision(Event_manager -> getMousePosition(), getDimensions()))
            m_is_clicked = true;
}

bool GUI_Button::getState()
{
    return m_is_clicked;
}
