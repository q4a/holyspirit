#include "Button.hpp"
#include <iostream>
using namespace sf;

Button::Button(std::string label) :  Widget(), m_hover(false), m_clicked(false), m_released(false)
{
    m_position  = sf::Vector2i(0,0);
    m_size      = sf::Vector2i(0,0);

    m_img_hover     = NULL;
    m_img_clicked   = NULL;
    m_img_released  = NULL;

    m_my_img_hover      = false;
    m_my_img_clicked    = false;
    m_my_img_released   = false;

    m_label.SetText(label);
    m_label.SetParent(this);
}
Button::Button(int x, int y, std::string label ) : Widget(x, y), m_hover(false), m_clicked(false), m_released(false)
{
    m_img_hover     = NULL;
    m_img_clicked   = NULL;
    m_img_released  = NULL;

    m_my_img_hover      = false;
    m_my_img_clicked    = false;
    m_my_img_released   = false;

    m_label.SetText(label);
    m_label.SetParent(this);
}
Button::Button(int x, int y, int w, int h, std::string label) :  Widget(x, y, w, h), m_hover(false), m_clicked(false), m_released(false)
{
    m_img_hover     = NULL;
    m_img_clicked   = NULL;
    m_img_released  = NULL;

    m_my_img_hover      = false;
    m_my_img_clicked    = false;
    m_my_img_released   = false;

    m_label.SetText(label);
    m_label.SetParent(this);
}

Button::~Button()
{
    //delete m_drawable;
    if(m_my_img_hover && m_img_hover != NULL)
        delete m_img_hover;
    if(m_my_img_clicked && m_img_clicked != NULL)
        delete m_img_clicked;
    if(m_my_img_released && m_img_released != NULL)
        delete m_img_released;
}

void Button::Show(std::list<sf::Drawable *> &drawables)
{
    if(m_clicked && m_img_clicked != NULL)
        m_img_clicked->Show(drawables);
    else if(m_hover && m_img_hover != NULL)
        m_img_hover->Show(drawables);
    else if(m_img_released != NULL)
        m_img_released->Show(drawables);

    m_label.Show(drawables);
    Widget::Show(drawables);
}

void Button::SetGeometry(int x, int y, int w, int h)
{
    Widget::SetGeometry(x, y, w, h);
}

void Button::SetImage(const sf::Image &img, int type)
{
    if(type == Button_released)
    {
        if(m_my_img_released && m_img_released != NULL)
            delete m_img_released;
        m_img_released = new GUIImage(0,0,m_size.x,m_size.y);
        m_img_released->SetImage(img);
        m_img_released->SetParent(this);
        m_my_img_released = true;
    }
    if(type == Button_hover)
    {
        if(m_my_img_hover && m_img_hover != NULL)
            delete m_img_hover;
        m_img_hover = new GUIImage(0,0,m_size.x,m_size.y);
        m_img_hover->SetImage(img);
        m_img_hover->SetParent(this);
        m_my_img_hover = true;
    }
    if(type == Button_clicked)
    {
        if(m_my_img_clicked && m_img_clicked != NULL)
            delete m_img_clicked;
        m_img_clicked = new GUIImage(0,0,m_size.x,m_size.y);
        m_img_clicked->SetImage(img);
        m_img_clicked->SetParent(this);
        m_my_img_clicked = true;
    }
}

void Button::SetImage(const std::string &img, int type)
{
    if(type == Button_released)
    {
        if(m_my_img_released && m_img_released != NULL)
            delete m_img_released;
        m_img_released = new GUIImage(0,0,m_size.x,m_size.y,img);
        m_img_released->SetParent(this);
        m_my_img_released = true;
    }
    if(type == Button_hover)
    {
        if(m_my_img_hover && m_img_hover != NULL)
            delete m_img_hover;
        m_img_hover = new GUIImage(0,0,m_size.x,m_size.y,img);
        m_img_hover->SetParent(this);
        m_my_img_hover = true;
    }
    if(type == Button_clicked)
    {
        if(m_my_img_released && m_img_clicked != NULL)
            delete m_img_clicked;
        m_img_clicked = new GUIImage(0,0,m_size.x,m_size.y,img);
        m_img_clicked->SetParent(this);
        m_my_img_clicked = true;
    }
}


void Button::SetImage(GUIImage *img, int type)
{
    if(type == Button_released)
    {
        if(m_my_img_released && m_img_released != NULL)
            delete m_img_released;
        m_img_released = img;
        m_img_released->SetParent(this);
        m_my_img_released = false;
    }
    if(type == Button_hover)
    {
        if(m_my_img_hover && m_img_hover != NULL)
            delete m_img_hover;
        m_img_hover = img;
        m_img_hover->SetParent(this);
        m_my_img_hover = false;
    }
    if(type == Button_clicked)
    {
        if(m_my_img_clicked && m_img_clicked != NULL)
            delete m_img_clicked;
        m_img_clicked = img;
        m_img_clicked->SetParent(this);
        m_my_img_clicked = false;
    }
}

void Button::SetState(int type)
{
    m_hover     = false;
    m_clicked   = false;
    m_released  = false;
    if(type == Button_hover)
        m_hover     = true;
    if(type == Button_clicked)
        m_clicked   = true;
}

void Button::Update()
{
    Widget::Update();
    m_label.SetPosition(m_size.x / 2 - m_label.GetTextSize().x / 2,
                        m_size.y / 2 - m_label.GetTextSize().y / 2);
    m_label.Update();

    if(m_img_released != NULL)
        m_img_released->Update();
    if(m_img_hover != NULL)
        m_img_hover->Update();
    if(m_img_clicked != NULL)
        m_img_clicked->Update();

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

    /*if(m_clicked)
        m_drawable->SetImage(m_img_clicked);
    else if(m_hover)
        m_drawable->SetImage(m_img_hover);
    else
        m_drawable->SetImage(m_img_released);
    m_drawable->Resize(m_size.x, m_size.y);
    m_drawable->SetPosition(pos.x, pos.y);*/
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
