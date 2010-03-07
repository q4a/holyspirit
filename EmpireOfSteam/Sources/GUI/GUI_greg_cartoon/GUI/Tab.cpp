#include "Tab.hpp"
#include <iostream>
using namespace sf;

Tab::Tab(std::string label) :  Button(label)
{
    //(*m_drawable) = sf::Sprite();
    //m_drawable->SetImage(m_img_released);
    //m_size      = sf::Vector2i((int)m_drawable->GetSize().x, (int)m_drawable->GetSize().y);
}
Tab::Tab(int x, int y, std::string label) :  Button(x, y, label)
{
}
Tab::Tab(int x, int y, int w, int h, std::string label) :  Button(x, y, w, h, label)
{
    //SetGeometry(x ,y, w, h);
}

Tab::~Tab()
{
}

void Tab::SetGeometry(int x, int y, int w, int h)
{
    Widget::SetGeometry(x, y, w, h);
}

void Tab::Update()
{
    sf::Vector2i pos = m_position;
    if(m_parent != NULL)
    {
        pos.x   = m_position.x + m_parent->GetGlobalPosition().x;
        pos.y   = m_position.y + m_parent->GetGlobalPosition().y;
    }

    if(mainEventManager->GetEvent(EventMouse, Mouse::Left))
        if(  mainEventManager->GetMousePosition().x > pos.x
         &&  mainEventManager->GetMousePosition().x < pos.x + m_size.x
         &&  mainEventManager->GetMousePosition().y > pos.y
         &&  mainEventManager->GetMousePosition().y < pos.y + m_size.y)
            mainEventManager->SetFocus(this);

    Widget::Update();
    if(m_clicked)
        m_label.SetPosition(m_size.x / 16,
                            m_size.y / 2);
    else
        m_label.SetPosition(m_size.x / 16,
                            m_size.y / 16);
    m_label.Update();

    if(m_img_released != NULL)
        m_img_released->Update();
    if(m_img_hover != NULL)
        m_img_hover->Update();
    if(m_img_clicked != NULL)
        m_img_clicked->Update();

    if(!mainEventManager->GetEvent(EventMouse, Mouse::Left))
    {
        if(  mainEventManager->GetMousePosition().x > pos.x
         &&  mainEventManager->GetMousePosition().x < pos.x + m_size.x
         &&  mainEventManager->GetMousePosition().y > pos.y
         &&  mainEventManager->GetMousePosition().y < pos.y + m_size.y)
            m_hover = true;
        else
        {
            m_hover     = false;
            m_clicked   = false;
        }
    }

    if(mainEventManager->GetFocus() == this)
    {
        if(m_hover)
        {
            if(mainEventManager->GetEvent(EventMouse, Mouse::Left))
                m_clicked = true;
            else
            {
                mainEventManager->SetFocus(NULL);
                m_clicked = false;
            }
        }
    }
}

