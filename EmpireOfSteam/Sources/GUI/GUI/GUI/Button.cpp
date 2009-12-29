#include "Button.hpp"

using namespace sf;

Button::Button()
{
    m_IbuttonForm.LoadFromFile("pictures/GUI/buttonForm.png");
    m_buttonForm.SetImage(m_IbuttonForm);
    m_width = m_buttonForm.GetSize().x;
    m_height = m_buttonForm.GetSize().y;
}

Button::~Button()
{

}

void Button::Show(RenderWindow &window)
{
    window.Draw(m_buttonForm);
}

void Button::SetGeometry(int x, int y, int w, int h)
{
    Widget::SetGeometry(x, y, w, h);

    m_buttonForm.SetScale(w, h);
    m_buttonForm.SetPosition(x, y);
}

void Button::SetPosition(int x, int y)
{
    Widget::SetPosition(x, y);

    m_buttonForm.SetPosition(x, y);
}

bool Button::Clicked()
{
    if(mainEventManager->GetMousePosition().x > m_x && mainEventManager->GetMousePosition().x < m_x + m_width &&
       mainEventManager->GetMousePosition().y > m_y && mainEventManager->GetMousePosition().y < m_y + m_height &&
       mainEventManager->GetEvent(EventMouse, Mouse::Left))
       return true;
    else
        return false;
}
