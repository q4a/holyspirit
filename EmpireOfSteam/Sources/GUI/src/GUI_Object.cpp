#include "GUI_Object.h"

GUI_Object::GUI_Object()
{
    //ctor
}

GUI_Object::~GUI_Object()
{
    //dtor
}

void GUI_Object::Update(float time)
{

}

const sf::Image &GUI_Object::GetImage()
{
    m_render.Display();
    return m_render.GetImage();
}

