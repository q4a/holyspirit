#include "Button.hpp"
#include <iostream>
using namespace sf;

Button::Button() :  m_hover(false), m_clicked(false), m_released(false)
{
    m_position  = sf::Vector2i(0,0);
    m_size      = sf::Vector2i(0,0);

    m_img_hover.LoadFromFile("pictures/GUI/buttonForm_h.png");
    m_img_clicked.LoadFromFile("pictures/GUI/buttonForm_c.png");
    m_img_released.LoadFromFile("pictures/GUI/buttonForm.png");

    m_drawable = new sf::Sprite();

    m_drawable->SetImage(m_img_released);
    m_size      = sf::Vector2i((int)m_drawable->GetSize().x, (int)m_drawable->GetSize().y);
}
Button::Button(int x, int y) :  m_hover(false), m_clicked(false), m_released(false)
{
    m_img_hover.LoadFromFile("pictures/GUI/buttonForm_h.png");
    m_img_clicked.LoadFromFile("pictures/GUI/buttonForm_c.png");
    m_img_released.LoadFromFile("pictures/GUI/buttonForm.png");

    m_drawable = new sf::Sprite();

    m_drawable->SetImage(m_img_released);
    m_size      = sf::Vector2i((int)m_drawable->GetSize().x, (int)m_drawable->GetSize().y);
    SetPosition(x, y);
}
Button::Button(int x, int y, int w, int h) :    m_hover(false), m_clicked(false), m_released(false)
{
    m_img_hover.LoadFromFile("pictures/GUI/buttonForm_h.png");
    m_img_clicked.LoadFromFile("pictures/GUI/buttonForm_c.png");
    m_img_released.LoadFromFile("pictures/GUI/buttonForm.png");

    m_drawable = new sf::Sprite();
    m_drawable->SetImage(m_img_released);

    SetGeometry(x ,y, w, h);
}

Button::~Button()
{
    delete m_drawable;
}

void Button::Show(std::list<sf::Drawable *> &drawables)
{
    drawables.push_back(m_drawable);
    Widget::Show(drawables);
}

void Button::SetGeometry(int x, int y, int w, int h)
{
    Widget::SetGeometry(x, y, w, h);
}

void Button::SetImage(const sf::Image &img, int type)
{
    if(type == Button_released)
        m_img_released = img;
    if(type == Button_hover)
        m_img_hover = img;
    if(type == Button_clicked)
        m_img_clicked = img;
}

void Button::SetImage(const std::string &img, int type)
{
    if(type == Button_released)
        m_img_released.LoadFromFile(img);
    if(type == Button_hover)
        m_img_hover.LoadFromFile(img);
    if(type == Button_clicked)
        m_img_clicked.LoadFromFile(img);
}
void Button::Update()
{
    Widget::Update();

    sf::Vector2i pos = m_position;
    if(m_parent != NULL)
    {
        pos.x   = m_position.x + m_parent->GetGlobalPosition().x;
        pos.y   = m_position.y + m_parent->GetGlobalPosition().y;
    }

    if(!mainEventManager->GetEvent(EventMouse, Mouse::Left))
    {
        if(  mainEventManager->GetMousePosition().x > pos.x
         &&  mainEventManager->GetMousePosition().x < pos.x + m_size.x
         &&  mainEventManager->GetMousePosition().y > pos.y
         &&  mainEventManager->GetMousePosition().y < pos.y + m_size.y
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

    if(m_clicked)
        m_drawable->SetImage(m_img_clicked);
    else if(m_hover)
        m_drawable->SetImage(m_img_hover);
    else
        m_drawable->SetImage(m_img_released);
    m_drawable->Resize(m_size.x, m_size.y);
    m_drawable->SetPosition(pos.x, pos.y);
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
