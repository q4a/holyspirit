#include "Button.hpp"
#include <iostream>
using namespace sf;

Button::Button() :  m_hover(false), m_clicked(false), m_released(false)
{
    m_position  = sf::Vector2i(0,0);
    m_size      = sf::Vector2i(0,0);
    m_IbuttonForm.LoadFromFile("pictures/GUI/buttonForm.png");
    m_sprite.SetImage(m_IbuttonForm);
    m_size      = sf::Vector2i((int)m_sprite.GetSize().x, (int)m_sprite.GetSize().y);
}
Button::Button(int x, int y) :  m_hover(false), m_clicked(false), m_released(false)
{
    m_IbuttonForm.LoadFromFile("pictures/GUI/buttonForm.png");
    m_sprite.SetImage(m_IbuttonForm);
    m_size      = sf::Vector2i((int)m_sprite.GetSize().x, (int)m_sprite.GetSize().y);
    SetPosition(x, y);
}
Button::Button(int x, int y, int w, int h) :    m_hover(false), m_clicked(false), m_released(false)
{
    m_IbuttonForm.LoadFromFile("pictures/GUI/buttonForm.png");
    SetGeometry(x ,y, w, h);

}

Button::~Button()
{

}

void Button::SetGeometry(int x, int y, int w, int h)
{
    Widget::SetGeometry(x, y, w, h);
    m_image.Create(m_size.x, m_size.y, true);
}

void Button::Update()
{
    Widget::Update();

    if(!mainEventManager->GetEvent(EventMouse, Mouse::Left))
    {
        if(  mainEventManager->GetMousePosition().x > m_position_global.x
         &&  mainEventManager->GetMousePosition().x < m_position_global.x + m_size.x
         &&  mainEventManager->GetMousePosition().y > m_position_global.y
         &&  mainEventManager->GetMousePosition().y < m_position_global.y + m_size.y
         && !mainEventManager->GetEvent(EventMouse, Mouse::Left))
        {
            m_hover = true;
        }
        else
        {
            m_hover     = false;
            m_clicked   = false;
            m_released  = false;
        }
    }


    if(m_hover)
    {
        if(mainEventManager->GetEvent(EventMouse, Mouse::Left))
            m_clicked = true;
        else
        {
            if(m_clicked)
                m_released = true;
            else
                m_released = false;
            m_clicked = false;
        }
    }
}

sf::Sprite Button::Show()
{
    m_image.Clear();

    sf::Sprite temp;
    temp.SetImage(m_IbuttonForm);
    temp.Resize(m_size.x, m_size.y);
    m_image.Draw(temp);

    for( std::vector<Widget*>::iterator i = m_widgets.begin();
         i != m_widgets.end();
         ++i )
            m_image.Draw((*i)->Show());

    m_image.Display();


    m_sprite.SetImage(m_image.GetImage());
    m_sprite.Resize(m_size.x, m_size.y);
    return  (m_sprite);
}

bool Button::Hover()
{
    return (m_hover);
}

bool Button::Clicked()
{
    return (m_clicked);
}

bool Button::Released()
{
    return (m_released);
}
