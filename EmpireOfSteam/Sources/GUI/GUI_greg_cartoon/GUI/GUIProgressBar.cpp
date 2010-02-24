#include "GUIProgressBar.hpp"
#include <iostream>
using namespace sf;

GUIProgressBar::GUIProgressBar(std::string label) :  Widget()
{
    m_img_background    = NULL;
    m_img_bar           = NULL;

    m_my_img_background = false;
    m_my_img_bar        = false;

    m_value = 0;
    m_min   = 0;
    m_max   = 100;

    m_label.SetText(label);
    m_label.SetParent(this);
}
GUIProgressBar::GUIProgressBar(int x, int y, std::string label ) : Widget(x, y)
{
    m_img_background    = NULL;
    m_img_bar           = NULL;

    m_my_img_background = false;
    m_my_img_bar        = false;

    m_value = 0;
    m_min   = 0;
    m_max   = 100;

    m_label.SetText(label);
    m_label.SetParent(this);
}
GUIProgressBar::GUIProgressBar(int x, int y, int w, int h, std::string label) :  Widget(x, y, w, h)
{
    m_img_background    = NULL;
    m_img_bar           = NULL;

    m_my_img_background = false;
    m_my_img_bar        = false;

    m_value = 0;
    m_min   = 0;
    m_max   = 100;

    m_label.SetText(label);
    m_label.SetParent(this);
}

GUIProgressBar::~GUIProgressBar()
{
    //delete m_drawable;
    if(m_my_img_background && m_img_background != NULL)
        delete m_img_background;
    if(m_my_img_bar && m_img_bar != NULL)
        delete m_img_bar;
}

void GUIProgressBar::Show(std::list<sf::Drawable *> &drawables)
{
    if(m_img_background != NULL)
        m_img_background->Show(drawables);
    if(m_img_bar != NULL)
        m_img_bar->Show(drawables);

    m_label.Show(drawables);
    Widget::Show(drawables);
}

void GUIProgressBar::SetGeometry(int x, int y, int w, int h)
{
    Widget::SetGeometry(x, y, w, h);
}

void GUIProgressBar::SetImage(const sf::Image &img, int type)
{
    if(type == Img_Bar)
    {
        if(m_my_img_bar && m_img_bar != NULL)
            delete m_img_bar;
        m_img_bar = new GUIImage(0,0,m_size.x,m_size.y);
        m_img_bar->SetImage(img);
        m_img_bar->SetParent(this);
        m_my_img_bar = true;
    }
    if(type == Img_Background)
    {
        if(m_my_img_background && m_img_background != NULL)
            delete m_img_background;
        m_img_background = new GUIImage(0,0,m_size.x,m_size.y);
        m_img_background->SetImage(img);
        m_img_background->SetParent(this);
        m_my_img_background = true;
    }
}

void GUIProgressBar::SetImage(const std::string &img, int type)
{
    if(type == Img_Bar)
    {
        if(m_my_img_bar && m_img_bar != NULL)
            delete m_img_bar;
        m_img_bar = new GUIImage(0,0,m_size.x,m_size.y,img);
        m_img_bar->SetParent(this);
        m_my_img_bar = true;
    }
    if(type == Img_Background)
    {
        if(m_my_img_background && m_img_background != NULL)
            delete m_img_background;
        m_img_background = new GUIImage(0,0,m_size.x,m_size.y,img);
        m_img_background->SetParent(this);
        m_my_img_background = true;
    }
}


void GUIProgressBar::SetImage(GUIImage *img, int type)
{
    if(type == Img_Bar)
    {
        if(m_my_img_bar && m_img_bar != NULL)
            delete m_img_bar;
        m_img_bar = img;
        m_img_bar->SetParent(this);
        m_my_img_bar = false;
    }
    if(type == Img_Background)
    {
        if(m_my_img_background && m_img_background != NULL)
            delete m_img_background;
        m_img_background = img;
        m_img_background->SetParent(this);
        m_my_img_background = false;
    }
}

void GUIProgressBar::SetValue(int value)
{
    m_value = value;
    if(m_value > m_max)
        m_value = m_max;
    if(m_value < m_min)
        m_value = m_min;
}
void GUIProgressBar::SetMax(int value)
{
    m_max = value;
    if(m_max <= m_min)
        m_max = m_min + 1;
}
void GUIProgressBar::SetMin(int value)
{
    m_min = value;
    if(m_max <= m_min)
        m_min = m_max - 1;
}

int GUIProgressBar::GetValue()
{
    return (m_value);
}
int GUIProgressBar::GetMax()
{
    return (m_max);
}
int GUIProgressBar::GetMin()
{
    return (m_min);
}

void GUIProgressBar::Update()
{
    Widget::Update();
    m_label.SetPosition(m_size.x / 2 - m_label.GetTextSize().x / 2,
                        m_size.y / 2 - m_label.GetTextSize().y / 2);
    m_label.Update();

    if(m_img_bar != NULL)
        m_img_bar->Update();
    if(m_img_background != NULL)
        m_img_background->Update();

    if(m_img_bar != NULL)
        m_img_bar->SetSubGeometry(0,0,(m_value-m_min) * 100 / (m_max-m_min) ,100);
}

