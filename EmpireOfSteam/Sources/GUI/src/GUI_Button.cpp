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
    if(Event_manager -> GetEvent(Event::Mouse, sf::Mouse::Left))
        if(Collision(Event_manager -> GetMousePosition(), sf::Rect(m_position.x, m_position.x + m_dimensions.x
                                                                  ,m_position.y, m_position.y + m_dimensions.y)))
            m_is_clicked = true;
}

bool GUI_Button::GetState()
{
    return m_is_clicked;
}
