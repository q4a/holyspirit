#include "Widget.hpp"

using namespace sf;

Widget::Widget() : m_x(0), m_y(0), m_width(0), m_height(0)
{

}

Widget::~Widget()
{

}

void Widget::SetGeometry(int x, int y, int w, int h)
{
    m_x = x;
    m_y = y;
    m_width = w;
    m_height = h;
}

void Widget::SetPosition(int x, int y)
{
    m_x = x;
    m_y = y;
}
