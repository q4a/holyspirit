#include "Button.hpp"

using namespace sf;

Button::Button()
{
    m_IbuttonForm.LoadFromFile("pictures/GUI/buttonForm.png");
    m_buttonForm.SetImage(m_IbuttonForm);
}

Button::~Button()
{

}

void Button::Show(RenderWindow &window)
{
    window.Draw(m_buttonForm);
}
