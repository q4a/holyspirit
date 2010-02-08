#include "Widget.hpp"

using namespace sf;

Widget::Widget() : m_position(0,0), m_size(0,0), m_subPosition(0,0), m_subSize(100,100)
{
    m_parent    = NULL;
    m_drawable  = NULL;

}

Widget::Widget(int x, int y) : m_position(x,y), m_size(0,0), m_subPosition(0,0), m_subSize(100,100)
{
    m_parent    = NULL;
    m_drawable  = NULL;
}

Widget::Widget(int x, int y, int w, int h) : m_position(x,y), m_size(w,h), m_subPosition(0,0), m_subSize(100,100)
{
    m_parent    = NULL;
    m_drawable  = NULL;
}

Widget::~Widget()
{
    for( std::vector<Widget*>::iterator i = m_widgets.begin();
         i != m_widgets.end();
         ++i )
            (*i)->m_parent = NULL;
}

void Widget::AddWidget(Widget *widget)
{
    m_widgets.push_back(widget);
    m_widgets.back()->m_parent = this;
    m_widgets.back()->Update();
}


void Widget::Show(std::list<sf::Drawable *> &drawables)
{
    for( std::vector<Widget*>::iterator i = m_widgets.begin();
         i != m_widgets.end();
         ++i )
            (*i)->Show(drawables);
}

void Widget::Update()
{
    for( std::vector<Widget*>::iterator i = m_widgets.begin();
         i != m_widgets.end();
         ++i )
            (*i)->Update();
}

void Widget::SetGeometry(int x, int y, int w, int h)
{
    m_position.x        = x;
    m_position.y        = y;
    m_size.x            = w;
    m_size.y            = h;
}

void Widget::SetSubGeometry(int x, int y, int w, int h)
{
    m_subPosition.x     = x;
    m_subPosition.y     = y;
    m_subSize.x         = w;
    m_subSize.y         = h;
}

void Widget::SetPosition(int x, int y)
{
    m_position.x        = x;
    m_position.y        = y;
}

void Widget::SetImage(const sf::Image &img)
{
    m_background = img;
}

void Widget::SetImage(const std::string &img)
{
    m_background.LoadFromFile(img);
}

void Widget::SetParent(Widget *widget)
{
    m_parent = widget;
}


const sf::Vector2i &Widget::GetSize()
{
    return (m_size);
}

const sf::Vector2i &Widget::GetPosition()
{
    return (m_position);
}

sf::Vector2i Widget::GetGlobalPosition()
{
    sf::Vector2i pos = m_position;

    if(m_parent != NULL)
    {
        pos.x   = m_position.x + m_parent->GetGlobalPosition().x;
        pos.y   = m_position.y + m_parent->GetGlobalPosition().y;
    }

    return (pos);
}
