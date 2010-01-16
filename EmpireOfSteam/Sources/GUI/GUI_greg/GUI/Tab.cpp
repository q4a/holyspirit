#include "Tab.hpp"
#include <iostream>
using namespace sf;

Tab::Tab(std::string label) :  Button(label)
{
    m_img_hover.LoadFromFile("pictures/GUI/onglet_h.png");
    m_img_clicked.LoadFromFile("pictures/GUI/onglet_c.png");
    m_img_released.LoadFromFile("pictures/GUI/onglet.png");

    (*m_drawable) = sf::Sprite();
    m_drawable->SetImage(m_img_released);
    m_size      = sf::Vector2i((int)m_drawable->GetSize().x, (int)m_drawable->GetSize().y);
}
Tab::Tab(int x, int y, std::string label) :  Button(x, y, label)
{
    m_img_hover.LoadFromFile("pictures/GUI/onglet_h.png");
    m_img_clicked.LoadFromFile("pictures/GUI/onglet_c.png");
    m_img_released.LoadFromFile("pictures/GUI/onglet.png");

    (*m_drawable) = sf::Sprite();
    m_drawable->SetImage(m_img_released);
    m_size      = sf::Vector2i((int)m_drawable->GetSize().x, (int)m_drawable->GetSize().y);
}
Tab::Tab(int x, int y, int w, int h, std::string label) :  Button(x, y, w, h, label)
{
    m_img_hover.LoadFromFile("pictures/GUI/onglet_h.png");
    m_img_clicked.LoadFromFile("pictures/GUI/onglet_c.png");
    m_img_released.LoadFromFile("pictures/GUI/onglet.png");

    (*m_drawable) = sf::Sprite();
    m_drawable->SetImage(m_img_released);

    SetGeometry(x ,y, w, h);
}

Tab::~Tab()
{
}

void Tab::SetGeometry(int x, int y, int w, int h)
{
    Widget::SetGeometry(x, y, w, h);
}

void Tab::SetImage(const sf::Image &img, int type)
{
    if(type == Button_released)
        m_img_released = img;
    if(type == Button_hover)
        m_img_hover = img;
    if(type == Button_clicked)
        m_img_clicked = img;
}

void Tab::SetImage(const std::string &img, int type)
{
    if(type == Button_released)
        m_img_released.LoadFromFile(img);
    if(type == Button_hover)
        m_img_hover.LoadFromFile(img);
    if(type == Button_clicked)
        m_img_clicked.LoadFromFile(img);
}

void Tab::SetState(int type)
{
    m_hover     = false;
    m_clicked   = false;
    m_released  = false;
    if(type == Button_hover)
        m_hover     = true;
    if(type == Button_clicked)
        m_clicked   = true;
}

void Tab::Update()
{
    Widget::Update();
    if(m_clicked)
        m_label.SetPosition(m_size.x / 16,
                            m_size.y / 2);
    else
        m_label.SetPosition(m_size.x / 16,
                            m_size.y / 16);
    m_label.Update();

    sf::Vector2i pos = m_position;
    if(m_parent != NULL)
    {
        pos.x   = m_position.x + m_parent->GetGlobalPosition().x;
        pos.y   = m_position.y + m_parent->GetGlobalPosition().y;
    }

    if(m_clicked)
        m_drawable->SetImage(m_img_clicked);
    else if(m_hover)
        m_drawable->SetImage(m_img_hover);
    else
        m_drawable->SetImage(m_img_released);
    m_drawable->Resize(m_size.x, m_size.y);
    m_drawable->SetPosition(pos.x, pos.y);


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


    if(m_hover)
    {
        if(mainEventManager->GetEvent(EventMouse, Mouse::Left))
            m_clicked = true;
        else
            m_clicked = false;
    }
}

